#include "Main.h"
#include "MainFrame.h"

#include <string>
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
	// true is to force the frame to close
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

/**
 * \brief traversing file tree recursively
 * \param source string indicating the directory path to read from
 * \param target string defining the target txt file to write to
 */
int MainFrame::TraverseDirTree(std::experimental::filesystem::path source, std::experimental::filesystem::path target) const
{
	for (const auto & file : directory_iterator(source))
	{
		if (std::experimental::filesystem::is_directory(file))
		{
			MainFrame::TraverseDirTree(file.path(), target);
		}
		else
		{
			resultText->AppendText(file.path().c_str());
			resultText->AppendText(wxString("\n"));

			std::ofstream imageFileList;
			imageFileList.open(target, std::ios_base::app);			
			if (imageFileList.is_open())
			{
				imageFileList << file.path() << "\n";
				imageFileList.close();
			}
			else
			{
				resultText->AppendText(wxString("ERROR: Unable to open file\n"));
			}
			imageFileList.close();
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
