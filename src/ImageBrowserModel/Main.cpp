#include "pch.h"
#include "Main.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(Main);

bool Main::OnInit()
{	
	if (!wxApp::OnInit())
		return false;

	frame = new MainFrame(app_name);

	frame->Show(true);

	return true;
}
