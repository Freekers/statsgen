// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>

// Statsgen Includes
#include "ErrorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "ProgressPanel.h"

ErrorPanel::ErrorPanel(wxArrayString &errorsIn,wxArrayInt &severitiesIn)
{
	mErrorDisplay	= NULL;
	mErrors			= errorsIn;
	mSeverities		= severitiesIn;
}

void ErrorPanel::Update()
{
	int			errorIndex;
	int			errorCount;
	int			listIndex;
	wxString	error;
	int			severity;
	wxBitmap	bitmap;

	mImageList.Create(16,16);
	for (severity=SeverityNotRun;severity<SeverityMax;severity++)
	{
		bitmap=ProgressPanel::SeverityImage(severity);
		mImageList.Add(bitmap);
	}

	mErrorDisplay->SetImageList(&mImageList,wxIMAGE_LIST_NORMAL);
	mErrorDisplay->SetImageList(&mImageList,wxIMAGE_LIST_SMALL);

	mErrorDisplay->Hide();
	mErrorDisplay->DeleteAllColumns();
	mErrorDisplay->DeleteAllItems();
	mErrorDisplay->InsertColumn(0,_T("Severity"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	mErrorDisplay->InsertColumn(1,_T("Text"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	errorCount=mErrors.GetCount();
	for (errorIndex=0;errorIndex<errorCount;errorIndex++)
	{
		wxString	severityString;
		error		= mErrors.Item(errorIndex);
		severity	= mSeverities.Item(errorIndex);

		severityString=ProgressPanel::SeverityString(severity);
		listIndex = mErrorDisplay->InsertItem(errorIndex,severityString);
		mErrorDisplay->SetItemData(listIndex,errorIndex);
		mErrorDisplay->SetItem(errorIndex,1,error);
		mErrorDisplay->SetItem(errorIndex,0,severityString,severity);
	}
	mErrorDisplay->SetColumnWidth(0,wxLIST_AUTOSIZE);
	mErrorDisplay->SetColumnWidth(1,wxLIST_AUTOSIZE);

	mErrorDisplay->Show();
	PostSizeEventToParent();
}

void ErrorPanel::CreateScreen()
{
	wxString	defaultValue="";
	wxSizeEvent	event;

	if (mErrorDisplay==NULL)
	{
		globalStatistics.ReadPlayerCache();
		mErrorDisplay=new wxListCtrl(this,
									WINDOW_ID_LISTBOX_CONFIGITEMS,
									wxDefaultPosition,
									wxDefaultSize,
									wxLC_REPORT);
		mMainSizer = new wxBoxSizer(wxVERTICAL);
		mMainSizer->Add(mErrorDisplay,1,wxEXPAND);
		SetSizer(mMainSizer);
		mMainSizer->SetSizeHints(this);
		Update();
	}
	else
	{
		// Screen is already created
	}
}
