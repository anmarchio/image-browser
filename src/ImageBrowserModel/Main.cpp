#include "pch.h"
#include "Main.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(Main);

bool Main::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	MainFrame *frame = new MainFrame("Tiny Image Browser");

	frame->Show(true);

	return true;
}
