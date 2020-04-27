// wx includes
#include <wx/artprov.h>

// Statsgen Includes
#include "FTPBrowserPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "Progress.h"
#include "ProgressPanel.h"
#include "RemoteMachine.h"

BEGIN_EVENT_TABLE(FTPBrowserPanel, wxPanel)
		EVT_MENU(WORKER_EVENT,FTPBrowserPanel::OnRemoteRequest)
		EVT_LIST_ITEM_SELECTED(WINDOW_ID_LISTBOX_CONFIGITEMS,FTPBrowserPanel::OnListSingleClick)
		EVT_LIST_ITEM_ACTIVATED(WINDOW_ID_LISTBOX_CONFIGITEMS,FTPBrowserPanel::OnListDoubleClick)
END_EVENT_TABLE()

FTPBrowserPanel::~FTPBrowserPanel()
{
	if (mRemoteMachine!=NULL)
	{
		delete (mRemoteMachine);
	}
	progress->SetDynamicPanel(NULL);
	/*
	if (thread!=NULL)
	{
		thread->Delete();
		delete (thread);
	}
	*/
}

FTPBrowserPanel::FTPBrowserPanel(
						wxString &configGroupIn,
						wxString &defaultPathIn,
						bool browsingForFile)
{
	workingForThread=false;
	mDirectoryListing=NULL;
	mConfigGroup=configGroupIn;
	mCurrentPath=defaultPathIn;
	mRemoteMachine=new RemoteMachine(mConfigGroup);
	thread=NULL;
}

void FTPBrowserPanel::OnListDoubleClick(wxListEvent &event)
{
	wxString	msg;
	wxString	fileName;
	long		fileSize;
	int			listIndex;

	STATSGEN_DEBUG_FUNCTION_START("FTPBrowserPanel","OnListDoubleClick")
	listIndex=event.GetIndex();
	STATSGEN_DEBUG_CODE(msg.Printf("item double clicked %ld",event.GetIndex());)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	if (listIndex<mFileNames.GetCount())
	{
		fileName=mFileNames.Item(listIndex);
		fileSize=mFileSizes.Item(listIndex);
		if (fileSize<0)
		{
			ChangeDirectory(fileName);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void FTPBrowserPanel::StartThreadWork()
{
	workingForThread=true;
}

void FTPBrowserPanel::StopThreadWork()
{
	workingForThread=false;
}

bool FTPBrowserPanel::DoingThreadWork()
{
	return (workingForThread);
}

void FTPBrowserPanel::OnRemoteRequest(wxCommandEvent &event)
{
	// This event governs the remote request required
	// in the main thread
	int			fileIndex;
	wxString	filename;

	fileIndex=event.GetInt();
	if (fileIndex==-1)
	{
		// Been asked to get a directory listing
		CreateList();

		mFileNames.Clear();
		mFileSizes.Clear();

		wxString		wildcard="";
	
		mRemoteMachine->GetRemoteDirectoryListing(mCurrentPath,
											wildcard,
											mFileNames);
		filename="..";
		mFileNames.Insert(filename,0);

		StopThreadWork();
	}
	else
	{
		// been asked to get the file size of this index item
		Update(fileIndex);
		StopThreadWork();
	}
}

void FTPBrowserPanel::Update(int directoryIndex)
{
	int			listIndex;
	int			directoryCount;
	long		fileSize;
	wxString	fileName;
	wxString	status;
	

	STATSGEN_DEBUG_FUNCTION_START("FTPBrowserPanel","Update")
	directoryCount=mFileNames.GetCount();
	if (directoryIndex<directoryCount)
	{
		int			imageIndex;
		wxString	fileSizeString;

		fileSize=mFileSizes.Item(directoryIndex);
		if (fileSize==-2)
		{
			// We need to update the file size
			fileName=mFileNames.Item(directoryIndex);
			status="Getting Size";progress->SetStatus(status);
			fileSize=mRemoteMachine->GetRemoteFileSize(mCurrentPath,fileName);
			status="Done";progress->SetStatus(status);
			mFileSizes.RemoveAt(directoryIndex);
			mFileSizes.Insert(fileSize,directoryIndex);
			if (fileSize<0)
			{
				fileSizeString="";
				imageIndex=0;
			}
			else
			{
				fileSizeString.Printf("%ld bytes",fileSize);
				if (fileSize> (2 * 1024))
				{
					fileSizeString.Printf("%.1f kb",((float)fileSize/1024.0));
				}
				if (fileSize> (2 * 1024 * 1024))
				{
					fileSizeString.Printf("%.1f mb",((float)fileSize/(1024.0 * 1024.0)));
				}
				imageIndex=2;
			}
			mDirectoryListing->SetItem(directoryIndex,0,fileName,imageIndex);
			mDirectoryListing->SetItem(directoryIndex,1,fileSizeString);

			mDirectoryListing->SetColumnWidth(0,wxLIST_AUTOSIZE);
			mDirectoryListing->SetColumnWidth(1,wxLIST_AUTOSIZE);
		}
	}

	PostSizeEventToParent();
	STATSGEN_DEBUG_FUNCTION_END
}

void FTPBrowserPanel::GetDirectoryListing()
{
	int	 		directoryCount;
	int	 		directoryIndex;
	long		fileSize;
	wxString	fileName;
	wxString	wildCard;
	int			imageIndex;
	int			listIndex;
	wxString	status;

	STATSGEN_DEBUG_FUNCTION_START("FTPBrowserPanel","GetDirectoryListing")
	status="Getting Directory Listing"; progress->SetStatus(status);
	CreateList();

	mFileNames.Clear();
	mFileSizes.Clear();

	wxString		wildcard="";
	
	mRemoteMachine->GetRemoteDirectoryListing(mCurrentPath,
											wildcard,
											mFileNames);
	status="Done"; progress->SetStatus(status);
	fileName="..";
	mFileNames.Insert(fileName,0);

	directoryCount=mFileNames.GetCount();
	for (directoryIndex=0;directoryIndex<directoryCount;directoryIndex++)
	{
		if (directoryIndex>0)
		{
			fileSize=-2;
			imageIndex=3;
		}
		else
		{
			fileSize=-1;
			imageIndex=1;
		}
		mFileSizes.Add(fileSize);
		fileName=mFileNames.Item(directoryIndex);

		listIndex=mDirectoryListing->InsertItem(directoryIndex,_T(""));
		mDirectoryListing->SetItemData(listIndex,directoryIndex);
		mDirectoryListing->SetItem(directoryIndex,0,fileName,imageIndex);
		mDirectoryListing->SetItem(directoryIndex,1,_T(""));
	}

	mDirectoryListing->SetColumnWidth(0,wxLIST_AUTOSIZE);
	mDirectoryListing->SetColumnWidth(1,wxLIST_AUTOSIZE);
	/*
	if (thread!=NULL)
	{
		thread->Delete();
	}
	else
	{
		thread=new RemoteFTPBrowserThread(this);
	}

	thread->Create();
	thread->Run();
	*/
	PostSizeEventToParent();
	STATSGEN_DEBUG_FUNCTION_END
}

void FTPBrowserPanel::CreateScreen()
{
	STATSGEN_DEBUG_FUNCTION_START("FTPBrowserPanel","CreateScreen")
	wxString	defaultValue="";
	wxSizeEvent	event;

	if (mDirectoryListing==NULL)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"mDirectoryList is NULL")
		globalStatistics.ReadPlayerCache();
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating directorylisting")
		mDirectoryListing=new wxListCtrl(this,
									WINDOW_ID_LISTBOX_CONFIGITEMS,
									wxDefaultPosition,
									wxDefaultSize,
									wxLC_REPORT);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating currentSelection")
		mCurrentSelection=new wxTextCtrl(this,
									-1,
									mCurrentPath,
									wxDefaultPosition,
									wxDefaultSize,
									wxTE_READONLY);

		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating progresspanel")
		mProgressPanel=new ProgressPanel();
		//progressPanel->DynamicSizing();
		mProgressPanel->DisableTimeToGo();
		mProgressPanel->DisableCounter();

		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"progresspanel->Create")
		mProgressPanel->Create(this,
							WINDOW_ID_PROGRESS_PANEL_DYNAMIC,
							wxDefaultPosition,
							wxDefaultSize,
							wxRAISED_BORDER);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"creating sizer")
		mMainSizer = new wxBoxSizer(wxVERTICAL);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding directorylisting")
		mMainSizer->Add(mDirectoryListing,1,wxEXPAND);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding currentselection")
		mMainSizer->Add(mCurrentSelection,0,wxEXPAND);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"adding currentselection")
		mMainSizer->Add(mProgressPanel,0,wxEXPAND);

		mMainSizer->SetSizeHints(this);
		SetSizer(mMainSizer);

		progress->SetDynamicPanel(mProgressPanel);
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DYNAMIC);
		progress->UpdateLabel("");
									
		GetDirectoryListing();
	}
	else
	{
		// Screen is already created
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void FTPBrowserPanel::CreateList()
{
	wxBitmap	bitmap;
	wxSize		buttonSize(16,16);
	
	Refresh();
	mImageList.Create(16,16);
	bitmap=wxArtProvider::GetIcon(wxART_FOLDER,wxART_OTHER,buttonSize);
	mImageList.Add(bitmap);
	bitmap=wxArtProvider::GetIcon(wxART_GO_DIR_UP,wxART_OTHER,buttonSize);
	mImageList.Add(bitmap);
	bitmap=wxArtProvider::GetIcon(wxART_NORMAL_FILE,wxART_OTHER,buttonSize);
	mImageList.Add(bitmap);
	bitmap=wxArtProvider::GetIcon(wxART_QUESTION,wxART_OTHER,buttonSize);
	mImageList.Add(bitmap);

	mDirectoryListing->SetImageList(&mImageList,wxIMAGE_LIST_NORMAL);
	mDirectoryListing->SetImageList(&mImageList,wxIMAGE_LIST_SMALL);

	mDirectoryListing->DeleteAllColumns();
	mDirectoryListing->DeleteAllItems();
	mDirectoryListing->InsertColumn(0,_T("Filename"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	mDirectoryListing->InsertColumn(1,_T("Size"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);

	PostSizeEventToParent();
}

RemoteFTPBrowserThread::RemoteFTPBrowserThread(
				FTPBrowserPanel *panelIn) : wxThread(wxTHREAD_JOINABLE)
{
	browserPanel=panelIn;
}

void RemoteFTPBrowserThread::OnExit()
{
	// Called when the thread has exited
}

void *RemoteFTPBrowserThread::Entry()
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED, WORKER_EVENT);

	// This is the main entry point of the directory getter thread
	browserPanel->StartThreadWork();
	event.SetInt(-1);
	wxPostEvent(browserPanel,event);
	while (browserPanel->DoingThreadWork())
	{
		wxYield();
	}
	if (TestDestroy())
	{
		return (NULL);
	}
	// Right - should have got the directory listing
	int		directoryCount;
	int		directoryIndex;

	directoryCount=browserPanel->mFileNames.GetCount();
	for (directoryIndex=0;directoryIndex<directoryCount;directoryIndex++)
	{
		// Pop loads of events onto the stack to get the file size
		browserPanel->StartThreadWork();
		event.SetInt(directoryIndex);
		wxPostEvent(browserPanel,event);
		while (browserPanel->DoingThreadWork())
		{
			wxYield();
		}
		if (TestDestroy())
		{
			return (NULL);
		}
	}
	return (NULL);
}

void FTPBrowserPanel::OnListSingleClick(wxListEvent &event)
{
	wxString	msg;
	wxString	fileName;
	long		fileSize;
	int			listIndex;
	wxString	selection;

	STATSGEN_DEBUG_FUNCTION_START("FTPBrowserPanel","OnListSingleClick")
	listIndex=event.GetIndex();
	STATSGEN_DEBUG_CODE(msg.Printf("item selected %ld",event.GetIndex());)
	if (listIndex<mFileNames.GetCount())
	{
		fileName=mFileNames.Item(listIndex);
		selection=mCurrentPath+"/"+fileName;
		mCurrentSelection->SetValue(selection);
		Update(listIndex);
		fileSize=mFileSizes.Item(listIndex);
		mSelectedSize=fileSize;
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	PostSizeEventToParent();
	
	STATSGEN_DEBUG_FUNCTION_END
}

wxString FTPBrowserPanel::GetCurrentSelection()
{
	return (mCurrentSelection->GetValue());
}

void FTPBrowserPanel::ChangeDirectory(wxString &filename)
{
	wxString	newDirectory=mCurrentPath+"/"+filename;

	if (mRemoteMachine->ChangeDirectory(newDirectory))
	{
		mCurrentPath=newDirectory;
		mCurrentSelection->SetValue(mCurrentPath);
		mSelectedSize=-1;
		GetDirectoryListing();
	}
}

wxString FTPBrowserPanel::CurrentPath()
{
	return (mCurrentPath);
}
