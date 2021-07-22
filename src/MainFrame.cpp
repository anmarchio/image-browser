#include "Main.h"
#include "MainFrame.h"

#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>
#include <experimental/filesystem>

enum
{
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT
};

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_MENU(Minimal_Quit, MainFrame::OnQuit)
EVT_MENU(Minimal_About, MainFrame::OnAbout)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title)
{
	// frame icon
	SetIcon(wxICON(sample));
	
#if wxUSE_MENUBAR
	// menu bar
	wxMenu *fileMenu = new wxMenu;

	//"About" item displayed in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	// ... and attach this menu bar to the frame
	wxFrameBase::SetMenuBar(menuBar);
#else
	// If menus are not available add a button to access the about box
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
	aboutBtn->Bind(wxEVT_BUTTON, &MainFrame::OnAbout, this);
	sizer->Add(aboutBtn, wxSizerFlags().Center());
	SetSizer(sizer);
#endif

	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	
	// Add image browser button
	wxButton* startBrowsingBtn = new wxButton(this, wxID_ANY, "Start Browsing");
	startBrowsingBtn->Bind(wxEVT_BUTTON, &MainFrame::StartBrowsing, this);
	sizer->Add(startBrowsingBtn, wxSizerFlags().Center());

	// Add text field
	resultText = new wxTextCtrl(this, wxID_ANY, "empty\n", wxDefaultPosition, wxSize(400,400), wxTE_MULTILINE);
	sizer->Add(resultText, wxSizerFlags().Center());
	
	SetSizer(sizer);
	
#if wxUSE_STATUSBAR
	// create a status bar just for fun (by default with 1 pane only)
	wxFrameBase::CreateStatusBar(2);
	wxFrameBase::SetStatusText(wxString::Format("Welcome to %s!", app_name));
#endif // wxUSE_STATUSBAR
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


using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::experimental::filesystem::directory_iterator;

const string DEFAULT_FILE_NAME = "image_index.html";
const string IMAGE_FILE_FORMATS[7] = { ".bmp", ".gif", ".jpg", ".jpeg", ".png", ".tif", ".tiff" };

/*
 * Method recursively iterating through directory
 */
int MainFrame::traverseDirTree(std::experimental::filesystem::path source, string target)
{
	for (const auto & file : directory_iterator(source))
	{
		if (std::experimental::filesystem::is_directory(file))
		{
			MainFrame::traverseDirTree(file.path(), target);
		}
		else
		{
			cout << file.path() << endl;
			resultText->AppendText(wxString("\n"));
			resultText->AppendText(file.path().c_str());

			std::ofstream imageFileList(target + "\\" + DEFAULT_FILE_NAME);
			if (imageFileList.is_open())
			{
				imageFileList << file.path() << "\n";
				imageFileList.close();
			}
			else cout << "Unable to open file";
		}
	}

	return EXIT_SUCCESS;
}

void MainFrame::StartBrowsing(wxCommandEvent& WXUNUSED(event))
{
	string target = ".";
	resultText->AppendText("Writing to:\n" + target);
	MainFrame::traverseDirTree(".", target);
}
