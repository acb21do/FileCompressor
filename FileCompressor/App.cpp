#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
	MainFrame* mainframe = new MainFrame("File Compressor");
	mainframe->SetClientSize(800, 600);
	mainframe->Center();
	mainframe->Show();
	return true;
}