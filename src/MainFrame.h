#pragma once

#include <experimental/filesystem>

#include "wx/wxprec.h"
#include <experimental/filesystem>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

// the application icon (under Windows it is in resources and even
// though we could still include the XPM here it would be unused)
#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

// Define a new frame type: this is going to be our main frame
class MainFrame : public wxFrame
{
public:
	// ctor(s)
	MainFrame(const wxString& title);

	wxTextCtrl* resultText;

	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void StartBrowsing(wxCommandEvent&);
	int traverseDirTree(std::experimental::filesystem::path source, std::string target);

private:
	// any class wishing to process wxWidgets events must use this macro
	wxDECLARE_EVENT_TABLE();
};