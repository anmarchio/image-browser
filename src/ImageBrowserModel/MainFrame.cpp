#include "Main.h"
#include "MainFrame.h"

#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <opencv2/opencv.hpp>
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
		wxVERSION_STRING,
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
	const std::regex pattern("[^\\s]+(.*?)\\.(jpg|jpeg|png|gif|tif|JPG|JPEG|PNG|GIF|TIF)$");

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
	/*
	 https://answers.opencv.org/question/60753/counting-black-white-pixels-with-a-threshold/
	 (...)
	  do not know enough about your approach, but you can use threshold two times: 
	  one for almost white, and one for almost black, then use countNonZero to get the number of white/black pixels. 
	  Be careful to put on white all the pixels that you want (make white the almost black pixels, so you can count non zeros).
	*/
	// String: width;height;depth\n
	wxString result;
	// Load and read image
	cv::Mat src = cv::imread(source.generic_string(), cv::IMREAD_UNCHANGED);
	result = wxString::Format(wxT("%i"), src.cols) + wxString(";") + wxString::Format(wxT("%i"), src.rows) + wxString(";");
	cv::Mat black;
	cv::threshold(src, black, 0, 1, cv::THRESH_BINARY);
	int countBlacks = cv::countNonZero(black);
	cv::Mat white;
	cv::threshold(src, white, 0, 1, cv::THRESH_BINARY_INV);
	int countWhites = cv::countNonZero(white);
	
	if ((src.cols * src.rows) == (countBlacks + countWhites)) {
		// if there are only pixels with values 0 and 255
		// we can assume the image contains only binary information
		return result + wxString("binary");
	}

	switch (src.depth()) {
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
	return result + wxString("none");
}

/**
 * \brief traversing file tree recursively
 * \param source string indicating the directory path to read from
 * \param target string defining the target txt file to write to
 */
int MainFrame::TraverseDirTree(std::experimental::filesystem::path source, std::experimental::filesystem::path target)
{
	int iteration = 0;
	for (const auto & file : directory_iterator(source))
	{
		if (std::experimental::filesystem::is_directory(file))
		{
			MainFrame::TraverseDirTree(file.path(), target);
		}
		else
		{
			resultText->AppendText(file.path().c_str() + wxString(";") + checkForImage(file.path()));

			std::ofstream imageFileList;
			imageFileList.open(target, std::ios_base::app);				
			if (imageFileList.is_open())
			{
				if (iteration == 0) {
					// write header
					imageFileList << "File;Type;Width;Height;ColorDepth\n";
				}
				wxString isImage = checkForImage(file.path());
				if(isImage == "image")
					imageFileList << file.path() << ";" << isImage << ";" << checkImageSizeAndDepth(file.path()) << "\n";
				else
					imageFileList << file.path() << ";" << "any" << ";;;\n";
				imageFileList.close();
			}
			else
			{
				resultText->AppendText(wxString("ERROR: Unable to open file\n"));
			}
			imageFileList.close();
			iteration++;
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
	
	resultText->SetLabelText("Start browsing...");
	resultText->AppendText(
		"\n\n"
		"Browse directory: " 
		+ source.string() +
		"\n"
		"Log file path: "
		+ target.string() +
		"\n\n"
	);
	MainFrame::TraverseDirTree(source, target);
}
