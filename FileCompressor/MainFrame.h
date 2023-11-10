#pragma once
//#include "Huffman.h"
#include <wx/wx.h>


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& Title);

private:
	void OnCompressBClicked(wxCommandEvent& evt);
	void OnDecompressBClicked(wxCommandEvent& evt);
	wxDECLARE_EVENT_TABLE();

	wxTextCtrl* textCtrl;
};

