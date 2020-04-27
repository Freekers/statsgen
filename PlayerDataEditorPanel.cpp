// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>
#include <wx/menu.h>

// Statsgen Includes
#include "PlayerDataEditorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(PlayerDataEditorPanel, wxPanel)
		EVT_SIZE(PlayerDataEditorPanel::OnResize)
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
	playerCache=NULL;
	playerDataListCtrl=NULL;
	playerDataList=playerDataListIn;
	defaultPlayerData=defaultPlayerDataIn;
	playerDataTitle=playerDataTitleIn;

	// Lets load up the current DropList List into
	// the global statistics - we can transfer
	// it across into the tree during creation
	globalStatistics.ReadAliasList();
	playerDataList->ReadFromFile();
}

bool PlayerDataEditorPanel::Create(wxWindow *parent,
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name)
{
	wxString	dummyConfigKey="/tmp/tmp";
	wxString	labelText="Player Filter";
	wxString	configKey;

	wxPanel::Create( parent, id,
		pos,
		size,
		style,
		name);

	playerCache=new PlayerCachePanel(dummyConfigKey,labelText);
	playerCache->SetFilterType(PlayerCachePanel::FILTER_TYPE_NO_CASE);
	playerCache->Create(this,
						WINDOW_ID_PLAYERCACHE,
						wxDefaultPosition,
						wxDefaultSize);
	playerDataListCtrl=new wxListCtrl(this,
							WINDOW_ID_DROPLIST,
							wxDefaultPosition,
							wxDefaultSize,
							wxLC_REPORT|
							wxLC_EDIT_LABELS);
	RefreshPlayerDataListTree();

	configItems=new GroupedConfigItemsPanel("Configuration File");
	configItems->Create(this,-1,wxDefaultPosition,wxDefaultSize);
	configItems->AddFile("Configuration File",
						playerDataList->filenameConfigKey,
						(char *)playerDataList->defaultFilename.GetData(),
						-1,
						NULL);

	wxSizeEvent	dummyEvent;
	OnResize(dummyEvent);
	return (true);
}

void PlayerDataEditorPanel::OnResize(wxSizeEvent &event)
{
	wxSize		itemSize;
	int			playerCacheWidth;
	int			playerCacheHeight;
	int			panelWidth;
	int			panelHeight;
	int			playerListWidth;
	int			playerListHeight;
	wxString	msg;
	int			configHeight;
	int			configWidth;


	if (playerDataListCtrl!=NULL)
	{
		itemSize=GetSize();
		panelWidth=itemSize.GetWidth();
		panelHeight=itemSize.GetHeight();

		configItems->SetSize(0,0,1,1);
		itemSize=configItems->GetSize();
		configWidth=panelWidth;
		configHeight=itemSize.GetHeight();

		panelHeight-=configHeight;

		playerCacheWidth=panelWidth;
		playerCacheHeight=panelHeight/2;
		playerListWidth=panelWidth;
		playerListHeight=panelHeight/2;

		configItems->SetSize(0,0,configWidth,configHeight);
		playerDataListCtrl->SetSize(0,configHeight,playerListWidth,playerListHeight);
		playerCache->SetSize(0,playerListHeight+configHeight,playerCacheWidth,playerCacheHeight);
	}

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

	listIndex=playerDataListCtrl->InsertItem(rowNumber,listEntry.playerData);
	STATSGEN_DEBUG_CODE(msg.Printf("row [%ld] listIndex=[%ld] itemIndex [%ld]"
				"[%s] [%s] [%s]",
				rowNumber,listIndex,itemIndex,
				listEntry.playerGUID.GetData(),
				listEntry.playerName.GetData(),
				listEntry.playerData.GetData());)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);

	playerDataListCtrl->SetItemData(listIndex,itemIndex);
	playerDataListCtrl->SetItem(rowNumber,1,listEntry.playerGUID);
	playerDataListCtrl->SetItem(rowNumber,2,DecolouriseName(listEntry.playerName));
	playerDataListCtrl->SetItem(rowNumber,3,listEntry.playerName);
	STATSGEN_DEBUG_FUNCTION_END
}

void PlayerDataEditorPanel::RefreshPlayerDataListTree()
{
	PlayerDataEntry		listEntry;
	wxListItem			listColumn;
	int					listCount;
	int					listIndex;

	playerDataListCtrl->Hide();
	
	playerDataListCtrl->DeleteAllColumns();
	playerDataListCtrl->DeleteAllItems();
	listColumn.SetText(playerDataTitle);
	playerDataListCtrl->InsertColumn(0,listColumn);
	listColumn.SetText("GUID");
	playerDataListCtrl->InsertColumn(1,listColumn);
	listColumn.SetText("Name (Uncoloured)");
	playerDataListCtrl->InsertColumn(2,listColumn);
	listColumn.SetText("Name (Coloured)");
	playerDataListCtrl->InsertColumn(3,listColumn);

	listCount=playerDataList->playerDataList.GetCount();
	for (listIndex=0;listIndex<listCount;listIndex++)
	{
		listEntry=playerDataList->playerDataList.Item(listIndex);
		AddPlayerDataEntry(listIndex,listEntry);
	}

	playerDataListCtrl->SetColumnWidth(0,wxLIST_AUTOSIZE);
	playerDataListCtrl->SetColumnWidth(1,wxLIST_AUTOSIZE);
	playerDataListCtrl->SetColumnWidth(2,wxLIST_AUTOSIZE);
	playerDataListCtrl->SetColumnWidth(3,wxLIST_AUTOSIZE);
	playerDataListCtrl->Show();
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
	menuItem="New Avatar Entry";
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
	wxString			data=defaultPlayerData;

	selectedPlayerData.Clear();
	selectedItem=-1;
	selectedItem=playerDataListCtrl->GetNextItem(selectedItem,
												wxLIST_NEXT_ALL,
												wxLIST_STATE_SELECTED);
	while (selectedItem!=-1)
	{
		listIndex=playerDataListCtrl->GetItemData(selectedItem);
		selectedPlayerData.Add(listIndex);
		selectedItem=playerDataListCtrl->GetNextItem(selectedItem,
													wxLIST_NEXT_ALL,
													wxLIST_STATE_SELECTED);
	}
	id=event.GetId();
	switch (id)
	{
		case PLAYER_DATA_ADD:
			playerCache->GetSelectedPlayers(selectedPlayers);
			selectedCount=selectedPlayers.GetCount();
			for (cacheIndex=0;cacheIndex<selectedCount;cacheIndex++)
			{
				playerIndex=selectedPlayers.Item(cacheIndex);
				cacheEntry=globalStatistics.playerCache.Item(playerIndex);
				playerDataList->AddPlayer(cacheEntry.guid,cacheEntry.name,data);
			}
			RefreshPlayerDataListTree();
			break;
		case PLAYER_DATA_DELETE:
			selectedCount=selectedPlayerData.GetCount();
			if (selectedCount>0)
			{
				listIndex=selectedPlayerData.Item(0);
				listEntry=playerDataList->playerDataList.Item(listIndex);
				playerDataList->DeletePlayer(listEntry.playerGUID,
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

	listIndex=playerDataListCtrl->GetItemData(index);
	listCount=playerDataList->playerDataList.GetCount();

	STATSGEN_DEBUG_CODE(msg.Printf("Index=[%d] of [%d]",listIndex,listCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	if (listIndex<listCount)
	{
		// Should always be less
		listEntry=playerDataList->playerDataList.Item(listIndex);
		listEntry.playerData=event.GetText();
		STATSGEN_DEBUG_CODE(msg.Printf("Adding [%s] [%s] with data [%s]",
					listEntry.playerGUID.GetData(),
					listEntry.playerName.GetData(),
					listEntry.playerData.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		playerDataList->AddPlayer(listEntry.playerGUID,
									listEntry.playerName,
									listEntry.playerData);
	}
	RefreshPlayerDataListTree();
	event.StopPropagation();
	STATSGEN_DEBUG_FUNCTION_END
}

