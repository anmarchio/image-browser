#include "Main.h"
#include "MainFrame.h"

enum
{
	Minimal_Quit = wxID_EXIT,
	Minimal_About = wxID_ABOUT
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(Minimal_Quit, MainFrame::OnQuit)
	EVT_MENU(Minimal_About, MainFrame::OnAbout)
wxEND_EVENT_TABLE()


// frame constructor
MainFrame::MainFrame(const wxString& title)
	: wxFrame(NULL, wxID_ANY, title)
{
	// set the frame icon
	SetIcon(wxICON(sample));

#if wxUSE_MENUBAR
	// create a menu bar
	wxMenu *fileMenu = new wxMenu;

	// the "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, "&About\tF1", "Show about dialog");

	fileMenu->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, "&File");
	menuBar->Append(helpMenu, "&Help");

	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);
#else // !wxUSE_MENUBAR
	// If menus are not available add a button to access the about box
	wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* aboutBtn = new wxButton(this, wxID_ANY, "About...");
	aboutBtn->Bind(wxEVT_BUTTON, &MyFrame::OnAbout, this);
	sizer->Add(aboutBtn, wxSizerFlags().Center());
	SetSizer(sizer);
#endif // wxUSE_MENUBAR/!wxUSE_MENUBAR

#if wxUSE_STATUSBAR
	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(2);
	SetStatusText("Welcome to wxWidgets!");
#endif // wxUSE_STATUSBAR
}


// event handlers

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
		"This is the tiny image browser\n"
		"running under %s.",
		wxVERSION_STRING,
		wxGetOsDescription()
	),
		"About tiny image browser",
		wxOK | wxICON_INFORMATION,
		this);
}

/*
void MainFrame::OnSetValue(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	wxMessageBox(wxString::Format
	(
		"Set the param!\n"
		"\n"
		"for the first CV parameter\n"
		"in this box.",
		wxGetOsDescription()
	),
		"Set Parameter",
		wxOK | wxICON_INFORMATION,
		this);
}
*/