// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>
#include <wx/menu.h>

// Statsgen Includes
#include "PlayerDataEditorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "ResizingListCtrl.h"

BEGIN_EVENT_TABLE(PlayerDataEditorPanel, wxPanel)
	EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_DROPLIST,
								PlayerDataEditorPanel::OnPlayerDataListRightClick)
	EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_PLAYERCACHE,
								PlayerDataEditorPanel::OnPlayerRightClick)
	EVT_MENU(PLAYER_DATA_ADD,PlayerDataEditorPanel::OnPopupMenu)
	EVT_MENU(PLAYER_DATA_DELETE,PlayerDataEditorPanel::OnPopupMenu)
	EVT_LIST_END_LABEL_EDIT(WINDOW_ID_DROPLIST,PlayerDataEditorPanel::OnLabelEdit)
END_EVENT_TABLE()

PlayerDataEditorPanel::PlayerDataEditorPanel(PlayerDataList *playerDataListIn,
											wxString &defaultPlayerDataIn,
											wxString &playerDataTitleIn)
{
	mPlayerCache=NULL;
	mPlayerDataListCtrl=NULL;
	mPlayerDataList=playerDataListIn;
	mDefaultPlayerData=defaultPlayerDataIn;
	mPlayerDataTitle=playerDataTitleIn;

	// Lets load up the current DropList List into
	// the global statistics - we can transfer
	// it across into the tree during creation
	globalStatistics.ReadAliasList();
	mPlayerDataList->ReadFromFile();
}

bool PlayerDataEditorPanel::CreateDisplay(wxWindow *parent, wxWindowID id)
{
	wxString	dummyConfigKey="/tmp/tmp";
	wxString	labelText="Player Filter";


	wxPanel::Create( parent, id,
		wxDefaultPosition,
		wxDefaultSize,
		wxTAB_TRAVERSAL|wxFULL_REPAINT_ON_RESIZE);

	mPlayerCache=new PlayerCachePanel(dummyConfigKey,labelText);
	mPlayerCache->SetFilterType(PlayerCachePanel::FILTER_TYPE_NO_CASE);
	mPlayerCache->CreateScreen(this, WINDOW_ID_PLAYERCACHE);
	mPlayerDataListCtrl=new wxListCtrl(this,
							WINDOW_ID_DROPLIST,
							wxDefaultPosition,
							wxDefaultSize,
							wxLC_REPORT|
							wxLC_EDIT_LABELS|wxFULL_REPAINT_ON_RESIZE);

	mConfigItems=new GroupedConfigItemsPanel((char *)"Configuration File");
	mConfigItems->CreateDisplay(this,wxID_ANY);
	mConfigItems->AddFile((char *)"Configuration File",
						mPlayerDataList->filenameConfigKey,
						mPlayerDataList->defaultFilename,
						wxID_ANY,
						NULL);

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(mConfigItems,0,wxEXPAND);
	mMainSizer->Add(mPlayerDataListCtrl,1,wxEXPAND|wxALL);
	mMainSizer->Add(mPlayerCache,1,wxEXPAND|wxALL);
	SetSizer(mMainSizer);
	mMainSizer->SetSizeHints(this);
	RefreshPlayerDataListTree();
	return (true);
}

void PlayerDataEditorPanel::AddPlayerDataEntry(int index,
											PlayerDataEntry &listEntry)
{
	long		itemIndex;
	long		listIndex;
	long		rowNumber;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("PlayerDataEditorPanel","AddPlayerDataEntry")
	rowNumber=index;
	itemIndex=index;
//	itemIndex++;

	listIndex=mPlayerDataListCtrl->InsertItem(rowNumber,listEntry.playerData);
	STATSGEN_DEBUG_CODE(msg.Printf("row [%ld] listIndex=[%ld] itemIndex [%ld]"
				"[%s] [%s] [%s]",
				rowNumber,listIndex,itemIndex,
				STRING_TO_CHAR(listEntry.playerGUID),
				STRING_TO_CHAR(listEntry.playerName),
				STRING_TO_CHAR(listEntry.playerData));)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);

	mPlayerDataListCtrl->SetItemData(listIndex,itemIndex);
	mPlayerDataListCtrl->SetItem(rowNumber,1,listEntry.playerGUID);
	mPlayerDataListCtrl->SetItem(rowNumber,2,DecolouriseName(listEntry.playerName));
	mPlayerDataListCtrl->SetItem(rowNumber,3,listEntry.playerName);
	STATSGEN_DEBUG_FUNCTION_END
}

void PlayerDataEditorPanel::RefreshPlayerDataListTree()
{
	PlayerDataEntry		listEntry;
	wxListItem			listColumn;
	int					listCount;
	int					listIndex;

	mPlayerDataListCtrl->Hide();
	
	mPlayerDataListCtrl->DeleteAllColumns();
	mPlayerDataListCtrl->DeleteAllItems();
	mPlayerDataListCtrl->InsertColumn(0,mPlayerDataTitle,wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE_USEHEADER);
	mPlayerDataListCtrl->InsertColumn(1,_T("GUID"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE_USEHEADER);
	mPlayerDataListCtrl->InsertColumn(2,_T("Name (Uncoloured)"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE_USEHEADER);
	mPlayerDataListCtrl->InsertColumn(3,_T("Name (Coloured)"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE_USEHEADER);

	listCount=mPlayerDataList->playerDataList.GetCount();
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		listEntry=mPlayerDataList->playerDataList.Item(listIndex);
		AddPlayerDataEntry(listIndex,listEntry);
	}

	mPlayerDataListCtrl->SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);
	mPlayerDataListCtrl->SetColumnWidth(1,wxLIST_AUTOSIZE_USEHEADER);
	mPlayerDataListCtrl->SetColumnWidth(2,wxLIST_AUTOSIZE_USEHEADER);
	mPlayerDataListCtrl->SetColumnWidth(3,wxLIST_AUTOSIZE_USEHEADER);
	mPlayerDataListCtrl->Show();
	PostSizeEventToParent();
	Fit();
}

void PlayerDataEditorPanel::OnPlayerDataListRightClick(wxListEvent &event)
{
	wxString		msg;
	wxMenu			popupMenu(_T(""));
	wxString		menuItem;

	// What have we right clicked on - parent or child?
	menuItem="Delete Entry"; popupMenu.Append(PLAYER_DATA_DELETE,menuItem);
	popupMenu.AppendSeparator();

	popupMenu.Append(PLAYER_DATA_CANCEL,_T("Cancel"));
	PopupMenu(&popupMenu);
}

void PlayerDataEditorPanel::OnPlayerRightClick(wxListEvent &event)
{
	wxString		msg;
	wxString		menuItem;
	wxMenu			popupMenu(_T(""));

	// Right clicked on an item in the Player Cache
	menuItem="New " + mPlayerDataTitle + " Entry";
	popupMenu.Append(PLAYER_DATA_ADD,menuItem);
	popupMenu.AppendSeparator();
	popupMenu.Append(PLAYER_DATA_CANCEL,_T("Cancel"));
	PopupMenu(&popupMenu);
}

void PlayerDataEditorPanel::OnPopupMenu(wxCommandEvent &event)
{
	wxString			msg;
	wxArrayInt			selectedPlayers;
	int					selectedCount;
	int					cacheIndex;
	PlayerCacheEntry	cacheEntry;
	int					playerIndex;
	int					playerCount;
	long				selectedItem=-1;
	int					listIndex;
	int					id;
	wxArrayInt			selectedPlayerData;
	PlayerDataEntry		listEntry;

	STATSGEN_DEBUG_FUNCTION_START("PlayerDataEditorPanel","OnPopupMenu")
	wxString			data=mDefaultPlayerData;

	selectedPlayerData.Clear();
	selectedItem=-1;
	selectedItem=mPlayerDataListCtrl->GetNextItem(selectedItem,
												wxLIST_NEXT_ALL,
												wxLIST_STATE_SELECTED);
	while (selectedItem!=-1)
	{
		listIndex=mPlayerDataListCtrl->GetItemData(selectedItem);
		selectedPlayerData.Add(listIndex);
		selectedItem=mPlayerDataListCtrl->GetNextItem(selectedItem,
													wxLIST_NEXT_ALL,
													wxLIST_STATE_SELECTED);
	}
	id=event.GetId();
	switch (id)
	{
		case PLAYER_DATA_ADD:
			mPlayerCache->GetSelectedPlayers(selectedPlayers);
			selectedCount=selectedPlayers.GetCount();
			for (cacheIndex=0;cacheIndex<selectedCount;cacheIndex++)
			{
				playerIndex=selectedPlayers.Item(cacheIndex);
				cacheEntry=globalStatistics.playerCache.Item(playerIndex);
				mPlayerDataList->AddPlayer(cacheEntry.guid,cacheEntry.name,data);
			}
			RefreshPlayerDataListTree();
			break;
		case PLAYER_DATA_DELETE:
			selectedCount=selectedPlayerData.GetCount();
			if (selectedCount>0)
			{
				listIndex=selectedPlayerData.Item(0);
				listEntry=mPlayerDataList->playerDataList.Item(listIndex);
				mPlayerDataList->DeletePlayer(listEntry.playerGUID,
											listEntry.playerName);
			}
			RefreshPlayerDataListTree();
			break;
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void PlayerDataEditorPanel::OnLabelEdit(wxListEvent &event)
{
	wxMenu			popupMenu(_T(""));
	long			index;
	long			listIndex;
	PlayerDataEntry	listEntry;
	int				listCount;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("PlayerDataEditorPanel","OnLabelEdit")
	index=event.GetIndex();

	listIndex=mPlayerDataListCtrl->GetItemData(index);
	listCount=mPlayerDataList->playerDataList.GetCount();

	STATSGEN_DEBUG_CODE(msg.Printf("Index=[%d] of [%d]",listIndex,listCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	if (listIndex<listCount)
	{
		// Should always be less
		listEntry=mPlayerDataList->playerDataList.Item(listIndex);
		listEntry.playerData=event.GetText();
		STATSGEN_DEBUG_CODE(msg.Printf("Adding [%s] [%s] with data [%s]",
					STRING_TO_CHAR(listEntry.playerGUID),
					STRING_TO_CHAR(listEntry.playerName),
					STRING_TO_CHAR(listEntry.playerData));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		mPlayerDataList->AddPlayer(listEntry.playerGUID,
									listEntry.playerName,
									listEntry.playerData);
	}
	RefreshPlayerDataListTree();
	event.StopPropagation();
	STATSGEN_DEBUG_FUNCTION_END
}

