#include <wx/wx.h>
#include <wx/image.h>
#include <wx/help.h>
#include <wx/html/helpctrl.h>
#include <wx/cshelp.h>
#include <wx/event.h>
#include "StatsgenApp.h"
#include "StatsgenFrame.h"
#include "Progress.h"
#include "ErrorData.h"
#include "StaticFunctions.h"


IMPLEMENT_APP(wxWidgetsApp)

// ============================================================================
// implementation
// ============================================================================
wxWidgetsApp::wxWidgetsApp()
{
}

wxWidgetsApp::~wxWidgetsApp()
{
}

bool wxWidgetsApp::OnInit()
{
	wxImage::AddHandler(new wxGIFHandler);
	wxImage::AddHandler(new wxJPEGHandler);
	wxString	filename;
	wxString	titleBar;

	filename=wxGetApp().argv[0];
	titleBar.Printf("Statsgen 2: (%s)",filename.GetData());
	wxFileSystem::AddHandler(new wxZipFSHandler);
	wxWidgetsFrame *frame = new wxWidgetsFrame(titleBar,
	wxPoint(50, 50), wxSize(640, 480));

	wxHelpControllerHelpProvider	*provider=new wxHelpControllerHelpProvider;
	wxHelpProvider::Set(provider);
	provider->SetHelpController(&frame->GetAdvancedHtmlHelpController());
	frame->Show(TRUE);

	SetTopWindow(frame);

	wxSize	itemSize;
	int	menuWidth;
	int	menuHeight;
	int	statusWidth;
	int	statusHeight;
	int	progressWidth;
	int	progressHeight;
	int	frameWidth;
	int	frameHeight;
	int	panelHeight;
	int	heightOffset;
	wxString	msg;


	itemSize=frame->GetMenuBar()->GetSize();
	menuWidth=itemSize.GetWidth();
	menuHeight=itemSize.GetHeight();
	menuHeight=46;
	itemSize=frame->GetStatusBar()->GetSize();
	statusWidth=itemSize.GetWidth();
	statusHeight=itemSize.GetHeight();
	
	itemSize=progress->GetSize();
	progressWidth=itemSize.GetWidth();
	progressHeight=itemSize.GetHeight();
	
	itemSize=frame->GetSize();
	frameWidth=itemSize.GetWidth();
	frameHeight=itemSize.GetHeight();
	
	frameWidth=progressWidth;
	panelHeight=frameHeight-(statusHeight+menuHeight);
	heightOffset=progressHeight-panelHeight;
	frameHeight+=heightOffset;
	frame->SetSize(frameWidth+10,frameHeight);

	frame->GetAdvancedHtmlHelpController().Initialize(_T("help"));

	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,WINDOW_ID_WIZARD);

	//wxPostEvent(frame,event);
	return TRUE;
}

