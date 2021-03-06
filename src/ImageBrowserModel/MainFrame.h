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

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

class MainFrame : public wxFrame
{
public:
	explicit MainFrame(const wxString& title);

	wxTextCtrl* selectedDirectory;
	wxTextCtrl* saveFilePath;
	wxTextCtrl* resultText;
	int OnFileSaveDialog;

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSelectDirectory(wxCommandEvent&);
	void OnSaveAs(wxCommandEvent&);
	int TraverseDirTree(std::experimental::filesystem::path source, std::experimental::filesystem::path target) const;
	void OnStartBrowsing(wxCommandEvent&) const;

private:
	wxDECLARE_EVENT_TABLE();
};