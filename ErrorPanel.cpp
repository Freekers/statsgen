// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>

// Statsgen Includes
#include "ErrorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "ProgressPanel.h"

BEGIN_EVENT_TABLE(ErrorPanel, wxPanel)
		EVT_SIZE(ErrorPanel::OnResize)
END_EVENT_TABLE()

ErrorPanel::ErrorPanel(wxArrayString &errorsIn,wxArrayInt &severitiesIn)
{
	errorDisplay=NULL;
	errors=errorsIn;
	severities=severitiesIn;
}

void ErrorPanel::Update()
{
	wxListItem	listColumn;
	int			errorIndex;
	int			errorCount;
	int			listIndex;
	wxString	error;
	int			severity;
	wxBitmap	bitmap;

	imageList.Create(16,16);
	for (severity=SeverityNotRun;severity<SeverityMax;severity++)
	{
		bitmap=ProgressPanel::SeverityImage(severity);
		imageList.Add(bitmap);
	}

	errorDisplay->SetImageList(&imageList,wxIMAGE_LIST_NORMAL);
	errorDisplay->SetImageList(&imageList,wxIMAGE_LIST_SMALL);

	errorDisplay->Hide();
	errorDisplay->DeleteAllColumns();
	errorDisplay->DeleteAllItems();
	listColumn.SetText("Severity");
	errorDisplay->InsertColumn(0,listColumn);
	listColumn.SetText("Text");
	errorDisplay->InsertColumn(2,listColumn);
	errorCount=errors.GetCount();
	for (errorIndex=0;errorIndex<errorCount;errorIndex++)
	{
		wxString	severityString;
		error=errors.Item(errorIndex);
		severity=severities.Item(errorIndex);

		severityString=ProgressPanel::SeverityString(severity);
		listIndex=errorDisplay->InsertItem(errorIndex,severityString);
		errorDisplay->SetItemData(listIndex,errorIndex);
		errorDisplay->SetItem(errorIndex,1,error);
		errorDisplay->SetItem(errorIndex,0,severityString,severity);
	}
	errorDisplay->SetColumnWidth(0,wxLIST_AUTOSIZE);
	errorDisplay->SetColumnWidth(1,wxLIST_AUTOSIZE);

	errorDisplay->Show();
}

void ErrorPanel::CreateScreen()
{
	wxString	defaultValue="";
	wxSizeEvent	event;

	if (errorDisplay==NULL)
	{
		globalStatistics.ReadPlayerCache();
		errorDisplay=new wxListCtrl(this,
									WINDOW_ID_LISTBOX_CONFIGITEMS,
									wxDefaultPosition,
									wxDefaultSize,
									wxLC_REPORT);
		Update();
							
	}
	else
	{
		// Screen is already created
	}
}

void ErrorPanel::OnResize(wxSizeEvent &event)
{
	wxSize		itemSize;
	int			panelWidth;
	int			panelHeight;
	int			listHeight;
	int			listWidth;
	wxString	msg;

	// Make sure the screen has been created
	CreateScreen();

	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();

	listWidth=panelWidth-10;
	listHeight=panelHeight;
	if (listHeight<0)
	{
		listHeight=0;
	}

	errorDisplay->SetSize(0,0,listWidth,listHeight);

}

