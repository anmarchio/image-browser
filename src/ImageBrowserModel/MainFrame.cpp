#include "Main.h"
#include "MainFrame.h"

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <experimental/filesystem>
#include <wx/filedlg.h>
#include <wx/wfstream.h>

using std::string;
using std::experimental::filesystem::directory_iterator;

enum
{
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(Minimal_Quit, MainFrame::OnQuit)
EVT_MENU(Minimal_About, MainFrame::OnAbout)
wxEND_EVENT_TABLE()


/**
 * \brief entry frame to the application UI
 * \param title string displayed in window top bar
 */
MainFrame::MainFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title)
{
	// frame icon
	SetIcon(wxICON(sample));
	SetSize(400, 600);
	
	// menu bar
	wxMenu *fileMenu = new wxMenu;

	//"About" item displayed in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

	// now append the freshly created menu to the menu bar...
	auto *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	// ... and attach this menu bar to the frame
	wxFrameBase::SetMenuBar(menuBar);

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// Open source directory dialog
	auto selectDirectoryBtn = new wxButton(this, wxID_ANY, "Select directory");
	selectDirectoryBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSelectDirectory, this);
	sizer->Add(selectDirectoryBtn, wxSizerFlags().Left());

	// Browse dir field
	selectedDirectory = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);
	sizer->Add(selectedDirectory, wxSizerFlags().Left());

	// Open file save dialog
	auto saveFileBtn = new wxButton(this, wxID_ANY, "Save as");
	saveFileBtn->Bind(wxEVT_BUTTON, &MainFrame::OnSaveAs, this);
	sizer->Add(saveFileBtn, wxSizerFlags().Left());
	
	// Browse dir field
	saveFilePath = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition);
	sizer->Add(saveFilePath, wxSizerFlags().Left());

	// Checkbox for deeply scanning images (with increased scan time and memory use)
	checkImageSizeAndDepthChkbx = new wxCheckBox(this, wxID_ANY, "Read image details? (slower scan, high memory usage)");	
	sizer->Add(checkImageSizeAndDepthChkbx, wxSizerFlags().Left());

	// Start browsing button
	auto startBrowsingBtn = new wxButton(this, wxID_ANY, "Start Browsing");
	startBrowsingBtn->Bind(wxEVT_BUTTON, &MainFrame::OnStartBrowsing, this);
	sizer->Add(startBrowsingBtn, wxSizerFlags().Left());

	// Text output field
	resultText = new wxTextCtrl(this, wxID_ANY, "Log\n", wxDefaultPosition, wxSize(400,400), wxTE_MULTILINE);
	sizer->Add(resultText, wxSizerFlags().Left());
	
	SetSizer(sizer);
	
#if wxUSE_STATUSBAR
	wxFrameBase::CreateStatusBar(2);
	wxFrameBase::SetStatusText(wxString::Format("Welcome to %s!", app_name));
#endif
}


void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}


void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxMessageBox(wxString::Format
	(
		"Welcome to %s!\n"
		"\n"
		"This version of %s is\n"
		"running under %s.",
		app_name,
		app_name,
		wxGetOsDescription()
	), 
		wxString::Format("About %s", app_name),
		wxOK | wxICON_INFORMATION,
		this);
}


void MainFrame::OnSelectDirectory(wxCommandEvent& WXUNUSED(event))
{
	wxDirDialog
		openDirDialog(NULL, "Choose input directory", "",
		wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);

	if (openDirDialog.ShowModal() == wxID_CANCEL)
		return;
	selectedDirectory->SetLabel(openDirDialog.GetPath());	
}


void MainFrame::OnSaveAs(wxCommandEvent& WXUNUSED(event))
{
	wxFileDialog
		saveFileDialog(this, _("Save txt file"), "", "",
			"txt files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	const wxFileOutputStream output_stream(saveFileDialog.GetPath());
	if (!output_stream.IsOk())
	{
		wxLogError("Cannot save to file '%s'.", saveFileDialog.GetPath());
		return;
	}

	saveFilePath->SetLabel(saveFileDialog.GetPath());
}


wxString MainFrame::checkForImage(std::experimental::filesystem::path source)
{
	// Regex to check valid image file extension.
	const std::regex pattern("[^\\s]+(.*?)\\.(jpg|jpeg|png|gif|tif|bmp|JPG|JPEG|PNG|GIF|TIF|BMP)$");

	// If the image file extension
	// is empty return false
	if (source.empty())
	{
		return "none";
	}
	// Return true if the image file extension
	// matched the ReGex
	if (std::regex_match(source.generic_string(), pattern))
	{
		return wxString("image");
	}

	return wxString("any");
}


wxString MainFrame::checkImageSizeAndDepth(std::experimental::filesystem::path source)
{
	// _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	/*
	 https://answers.opencv.org/question/60753/counting-black-white-pixels-with-a-threshold/
	 (...)
	  do not know enough about your approach, but you can use threshold two times: 
	  one for almost white, and one for almost black, then use countNonZero to get the number of white/black pixels. 
	  Be careful to put on white all the pixels that you want (make white the almost black pixels, so you can count non zeros).
	*/
	// String: width;height;depth\n
	wxString result;
	// Initialize variables
	//cv::Mat src;
	//cv::Mat dst;
	//cv::Mat black;
	//cv::Mat white;
	int countBlacks;
	int countWhites;
	int numberOfRows;
	int numberOfCols;
	int depth;
	try {
		// Load and read image
		cv::Mat src = cv::imread(source.generic_string(), cv::IMREAD_ANYCOLOR);
		cv::Mat dst;
		if (src.depth() != CV_8U)
			cv::cvtColor(src, dst, cv::COLOR_BGR2GRAY);
		else
			dst = src;		
		
		//cv::imshow("dst", dst);
		//int v = cv::waitKey(0);
		result = wxString::Format(wxT("%i"), src.cols) + wxString(";") + wxString::Format(wxT("%i"), src.rows) + wxString(";");

		cv::Mat black;
		cv::threshold(dst, black, 254, 255, cv::THRESH_BINARY_INV);
		countBlacks = cv::countNonZero(black);
		black.release();
	
		//cv::imshow("black", black);
		//int v = cv::waitKey(0);

		cv::Mat white;
	 	cv::threshold(dst, white, 254, 255, cv::THRESH_BINARY);
		dst.release();
		countWhites = cv::countNonZero(white);		
		white.release();

		//cv::imshow("white", white);
		//int l = cv::waitKey(0);

		// Store save values and free image space
		numberOfRows = src.rows;
		numberOfCols = src.cols;
		depth = src.depth();
		src.release();

		if ((numberOfCols * numberOfRows) == (countBlacks + countWhites) && !(countBlacks == (numberOfCols * numberOfRows) || countWhites == 0)) {
			// if there are only pixels with values 0 and 255
			// we can assume the image contains only binary information
			return result + wxString("binary");
		}

		switch (depth) {
			case CV_8U:
				return result + wxString("8 bit"); // 8 bit unsigned
			case CV_16U:
				return result + wxString("16 bit"); // 16 bit unsigned
			case CV_32S:
				return result + wxString("32 bit"); // 32 bit unsigned
			case CV_32F:
				return result + wxString("float"); // float
			case CV_64F:
				return result + wxString("double"); // double
		}
	}
	catch (const std::exception& e)
	{
		std::cout << " a standard exception was caught, with message '"
			<< e.what() << "'\n";
		// Remove newline characters from string in C++	
		std::string mErrWithoutNewline = std::regex_replace(e.what(), std::basic_regex<char>("\\r\\n|\\r|\\n"), "");
		return wxString("err;err;") + wxString(mErrWithoutNewline);
	}
	
	return result + wxString("none");
}


std::string getCTime() {
	auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	return std::ctime(&now_time);
}


std::string insertHeader() {
	std::string header = "---\nstarted " + 
		getCTime() + 
		"---\n";
	header += "File;Type;Width;Height;ColorDepth\n";
	return header;
}


std::string insertFooter() {
	return "---------\nterminated " +
		getCTime() + 
		"---------\n";
}


/**
 * \brief traversing file tree recursively
 * \param source string indicating the directory path to read from
 * \param target string defining the target txt file to write to
 */
int ITERATION = 0;
int MainFrame::TraverseDirTree(std::experimental::filesystem::path source, std::experimental::filesystem::path target, bool isCheckImageSizeAndDepth)
{

	for (const auto & file : directory_iterator(source))
	{
		if (ITERATION % 1000 == 0) {
			resultText->AppendText("Scanned " + std::to_string(ITERATION) + " images ...\n");
		}
		/*
		 * ---------
		 * Uncomment for debugging purposes
		 * ---------
		*/
		if (ITERATION > 5000)
			std::cout << "BREAKPOINT AT ITERATION: " << ITERATION;

		if (std::experimental::filesystem::is_directory(file))
		{
			MainFrame::TraverseDirTree(file.path(), target, isCheckImageSizeAndDepth);
		}
		else
		{
			std::cout << file.path().c_str();
			
			// Display only if running without reading image details
			if (!isCheckImageSizeAndDepth)
				resultText->AppendText(file.path().c_str() + wxString(";") + checkForImage(file.path()) + "\n");

			std::ofstream imageFileList;
			imageFileList.open(target, std::ios_base::app);				
			if (imageFileList.is_open())
			{
				if (ITERATION == 0) {
					imageFileList << insertHeader();
				}
				wxString isImage = checkForImage(file.path());
				if(isImage == "image")
					if (isCheckImageSizeAndDepth)
						imageFileList << file.path() << ";" << isImage << ";" << checkImageSizeAndDepth(file.path()) << "\n";
					else
						imageFileList << file.path() << ";" << isImage << ";;;\n";
				else
					imageFileList << file.path() << ";" << "any" << ";;;\n";
				imageFileList.close();
			}
			else
			{
				resultText->AppendText(wxString("ERROR: Unable to open file\n"));
			}
			imageFileList.close();
			ITERATION++;

		}
	}
	return EXIT_SUCCESS;
}


void MainFrame::OnStartBrowsing(wxCommandEvent& WXUNUSED(event))
{
	std::experimental::filesystem::path source = selectedDirectory->GetLabelText().ToStdString();
	if (source.empty()) source = std::experimental::filesystem::current_path();
	
	std::experimental::filesystem::path target = saveFilePath->GetLabelText().ToStdString();
	if(target.empty()) target = std::experimental::filesystem::current_path();

	bool isCheckImageSizeAndDepth = checkImageSizeAndDepthChkbx->GetValue();

	resultText->SetLabelText("Start browsing...");
	resultText->AppendText(
		"\n\n"
		"Browse directory:\n" 
		+ source.string() +
		"\n---------\n"
		"Log file path:\n"
		+ target.string() +
		"\n---------\n\n"
	);
	ITERATION = 0;
	MainFrame::TraverseDirTree(source, target, isCheckImageSizeAndDepth);

	// Write footer
	std::ofstream imageFileList;
	imageFileList.open(target, std::ios_base::app);
	imageFileList << insertFooter();
	imageFileList.close();
}
