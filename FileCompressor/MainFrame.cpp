#include "MainFrame.h"
#include <wx/file.h>
#include <wx/wfstream.h>
#include "Huffman.h"


enum IDs
{
	COMPRESSB_ID = 2,
	DECOMPRESSB_ID = 3,
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(COMPRESSB_ID, MainFrame::OnCompressBClicked)
	EVT_BUTTON(DECOMPRESSB_ID, MainFrame::OnDecompressBClicked)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& Title) 
	: wxFrame(nullptr, wxID_ANY, Title)
{
	wxPanel* panel = new wxPanel(this);
	wxButton* compressB = new wxButton(panel, COMPRESSB_ID, "Select File to Compress", wxPoint(300, 275), wxSize(200, 50));
	wxButton* decompressB = new wxButton(panel, DECOMPRESSB_ID, "Select File to Decompress", wxPoint(300, 275 + 50), wxSize(200, 50));

	CreateStatusBar();

}

void MainFrame::OnCompressBClicked(wxCommandEvent& evt)
{
	wxFileDialog openFileDialog(this, _("Open a Text file"), "", "",
		"Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
	{
		wxString filePath = openFileDialog.GetPath();

		wxFile file(filePath, wxFile::read);
		if (file.IsOpened())
		{
			wxString fileName = openFileDialog.GetFilename();
			std::string sFileName = fileName.ToStdString();
			size_t dotPos = sFileName.find('.');
			sFileName.insert(dotPos, "-encoded");
			sFileName = "res/" + sFileName;
			Huffman h(filePath.ToStdString(), sFileName);
			h.Compress();
			
			file.Close();
		}
	}
}

void MainFrame::OnDecompressBClicked(wxCommandEvent& evt)
{
	wxFileDialog openFileDialog(this, _("Open a Text file"), "", "",
		"Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
	{
		wxString filePath = openFileDialog.GetPath();

		wxFile file(filePath, wxFile::read);
		if (file.IsOpened())
		{
			wxString fileName = openFileDialog.GetFilename();
			std::string sFileName = fileName.ToStdString();
			size_t dotPos = sFileName.find('.');
			sFileName.insert(dotPos, "-decoded");
			sFileName = "res/" + sFileName;
			Huffman h(filePath.ToStdString(), sFileName);
			h.Decompress();

			file.Close();
		}
	}
}
