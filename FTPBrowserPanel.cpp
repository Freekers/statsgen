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
		EVT_SIZE(FTPBrowserPanel::OnResize)
		EVT_MENU(WORKER_EVENT,FTPBrowserPanel::OnRemoteRequest)
		EVT_LIST_ITEM_SELECTED(WINDOW_ID_LISTBOX_CONFIGITEMS,FTPBrowserPanel::OnListSingleClick)
		EVT_LIST_ITEM_ACTIVATED(WINDOW_ID_LISTBOX_CONFIGITEMS,FTPBrowserPanel::OnListDoubleClick)
END_EVENT_TABLE()

FTPBrowserPanel::~FTPBrowserPanel()
{
	if (remoteMachine!=NULL)
	{
		delete (remoteMachine);
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
	directoryListing=NULL;
	configGroup=configGroupIn;
	currentPath=defaultPathIn;
	remoteMachine=new RemoteMachine(configGroup);
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
	if (listIndex<fileNames.GetCount())
	{
		fileName=fileNames.Item(listIndex);
		fileSize=fileSizes.Item(listIndex);
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

		fileNames.Clear();
		fileSizes.Clear();

		wxString		wildcard="";
	
		remoteMachine->GetRemoteDirectoryListing(currentPath,
											wildcard,
											fileNames);
		filename="..";
		fileNames.Insert(filename,0);

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
	directoryCount=fileNames.GetCount();
	if (directoryIndex<directoryCount)
	{
		int			imageIndex;
		wxString	fileSizeString;

		fileSize=fileSizes.Item(directoryIndex);
		if (fileSize==-2)
		{
			// We need to update the file size
			fileName=fileNames.Item(directoryIndex);
			status="Getting Size";progress->SetStatus(status);
			fileSize=remoteMachine->GetRemoteFileSize(currentPath,fileName);
			status="Done";progress->SetStatus(status);
			fileSizes.RemoveAt(directoryIndex);
			fileSizes.Insert(fileSize,directoryIndex);
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
			directoryListing->SetItem(directoryIndex,0,fileName,imageIndex);
			directoryListing->SetItem(directoryIndex,1,fileSizeString);

			directoryListing->SetColumnWidth(0,wxLIST_AUTOSIZE);
			directoryListing->SetColumnWidth(1,wxLIST_AUTOSIZE);
		}
	}

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

	fileNames.Clear();
	fileSizes.Clear();

	wxString		wildcard="";
	
	remoteMachine->GetRemoteDirectoryListing(currentPath,
											wildcard,
											fileNames);
	status="Done"; progress->SetStatus(status);
	fileName="..";
	fileNames.Insert(fileName,0);

	directoryCount=fileNames.GetCount();
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
		fileSizes.Add(fileSize);
		fileName=fileNames.Item(directoryIndex);

		listIndex=directoryListing->InsertItem(directoryIndex,_T(""));
		directoryListing->SetItemData(listIndex,directoryIndex);
		directoryListing->SetItem(directoryIndex,0,fileName,imageIndex);
		directoryListing->SetItem(directoryIndex,1,_T(""));
	}

	directoryListing->SetColumnWidth(0,wxLIST_AUTOSIZE);
	directoryListing->SetColumnWidth(1,wxLIST_AUTOSIZE);
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
	STATSGEN_DEBUG_FUNCTION_END
}

void FTPBrowserPanel::CreateScreen()
{
	wxString	defaultValue="";
	wxSizeEvent	event;

	if (directoryListing==NULL)
	{
		globalStatistics.ReadPlayerCache();
		directoryListing=new wxListCtrl(this,
									WINDOW_ID_LISTBOX_CONFIGITEMS,
									wxDefaultPosition,
									wxDefaultSize,
									wxLC_REPORT);
		currentSelection=new wxTextCtrl(this,
									-1,
									currentPath,
									wxDefaultPosition,
									wxDefaultSize,
									wxTE_READONLY);

		progressPanel=new ProgressPanel();
		progressPanel->DynamicSizing();
		progressPanel->DisableTimeToGo();
		progressPanel->DisableCounter();

		progressPanel->Create(this,
							WINDOW_ID_PROGRESS_PANEL_DYNAMIC,
							wxDefaultPosition,
							wxDefaultSize,
							wxRAISED_BORDER);
		progress->SetDynamicPanel(progressPanel);
		progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DYNAMIC);
		progress->UpdateLabel("");
									
		GetDirectoryListing();
							
	}
	else
	{
		// Screen is already created
	}
}

void FTPBrowserPanel::OnResize(wxSizeEvent &event)
{
	wxSize		itemSize;
	int			panelWidth;
	int			panelHeight;
	int			selectionWidth;
	int			selectionHeight;
	int			progressWidth;
	int			progressHeight;
	int			listHeight;
	int			listWidth;
	wxString	msg;

	// Make sure the screen has been created
	CreateScreen();

	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();
	itemSize=currentSelection->GetSize();
	selectionWidth=itemSize.GetWidth();
	selectionHeight=itemSize.GetHeight();
	itemSize=progressPanel->GetSize();
	progressWidth=itemSize.GetWidth();
	progressHeight=itemSize.GetHeight();

	progressWidth=panelWidth-10;

	listWidth=panelWidth-10;
	listHeight=panelHeight;
	listHeight-=selectionHeight;
	listHeight-=progressHeight;
	if (listHeight<0)
	{
		listHeight=0;
	}

	selectionWidth=panelWidth;

	directoryListing->SetSize(0,0,listWidth,listHeight);
	currentSelection->SetSize(0,listHeight,selectionWidth,selectionHeight);
	progressPanel->SetSize(0,listHeight+selectionHeight,
							progressWidth,progressHeight);

}

void FTPBrowserPanel::CreateList()
{
	wxListItem	listColumn;
	wxBitmap	bitmap;
	wxSize		buttonSize(16,16);
	
	imageList.Create(16,16);
	bitmap=wxArtProvider::GetIcon(wxART_FOLDER,wxART_OTHER,buttonSize);
	imageList.Add(bitmap);
	bitmap=wxArtProvider::GetIcon(wxART_GO_DIR_UP,wxART_OTHER,buttonSize);
	imageList.Add(bitmap);
	bitmap=wxArtProvider::GetIcon(wxART_NORMAL_FILE,wxART_OTHER,buttonSize);
	imageList.Add(bitmap);
	bitmap=wxArtProvider::GetIcon(wxART_QUESTION,wxART_OTHER,buttonSize);
	imageList.Add(bitmap);

	directoryListing->SetImageList(&imageList,wxIMAGE_LIST_NORMAL);
	directoryListing->SetImageList(&imageList,wxIMAGE_LIST_SMALL);

	directoryListing->DeleteAllColumns();
	directoryListing->DeleteAllItems();
	listColumn.SetText("Filename");
	directoryListing->InsertColumn(0,listColumn);
	listColumn.SetText("Size");
	directoryListing->InsertColumn(1,listColumn);

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

	directoryCount=browserPanel->fileNames.GetCount();
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
	if (listIndex<fileNames.GetCount())
	{
		fileName=fileNames.Item(listIndex);
		selection=currentPath+"/"+fileName;
		currentSelection->SetValue(selection);
		Update(listIndex);
		fileSize=fileSizes.Item(listIndex);
		selectedSize=fileSize;
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	STATSGEN_DEBUG_FUNCTION_END
}

wxString FTPBrowserPanel::GetCurrentSelection()
{
	return (currentSelection->GetValue());
}

void FTPBrowserPanel::ChangeDirectory(wxString &filename)
{
	wxString	newDirectory=currentPath+"/"+filename;

	if (remoteMachine->ChangeDirectory(newDirectory))
	{
		currentPath=newDirectory;
		currentSelection->SetValue(currentPath);
		selectedSize=-1;
		GetDirectoryListing();
	}
}

wxString FTPBrowserPanel::CurrentPath()
{
	return (currentPath);
}
