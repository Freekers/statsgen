// wx includes
#include <wx/wx.h>
#include <wx/regex.h>

// Statsgen Includes
#include "PlayerCachePanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(PlayerCachePanel, wxPanel)
		EVT_SIZE(PlayerCachePanel::OnResize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,PlayerCachePanel::OnFilterChanged)
		EVT_LIST_COL_CLICK(WINDOW_ID_LISTBOX_CONFIGITEMS,PlayerCachePanel::OnColumnClick)
		EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_LISTBOX_CONFIGITEMS,
					PlayerCachePanel::OnRightClick)
END_EVENT_TABLE()

PlayerCachePanel::PlayerCachePanel(wxString &configKeyIn,wxString &labelTextIn)
{
	cacheDisplay=NULL;
	filter=NULL;
	configKey=configKeyIn;
	labelText=labelTextIn;
	SetFilterType(FILTER_TYPE_NO_CASE);
	filterNameCallBack=(bool (*)(wxString &name))NULL;

}

void PlayerCachePanel::UpdateCacheList()
{
	wxListItem			listColumn;
	int					cacheIndex;
	int					cacheCount;
	PlayerCacheEntry	cacheEntry;
	long				listIndex;
	wxString			decolouredName;
	wxString			filterString;
	int					rowNumber;
	bool				addRow;
	wxString			matchString;
	wxRegEx				regExp;

	globalStatistics.configData.ReadTextValue(configKey,&filterString);
	
	switch (filterType)
	{
		case FILTER_TYPE_NO_CASE:
			filterString.MakeLower();
			break;
		case FILTER_TYPE_CASE:
			break;
		case FILTER_TYPE_REGEXP:
			if (regExp.Compile(filterString))
			{
			}
			else
			{
			}
			break;
	}
	cacheDisplay->Hide();
	cacheDisplay->DeleteAllColumns();
	cacheDisplay->DeleteAllItems();
	listColumn.SetText("GUID");
	cacheDisplay->InsertColumn(0,listColumn);
	listColumn.SetText("Name");
	cacheDisplay->InsertColumn(1,listColumn);
	listColumn.SetText("Decoloured Name");
	cacheDisplay->InsertColumn(2,listColumn);
	cacheCount=globalStatistics.playerCache.GetCount();
	rowNumber=0;
	for (cacheIndex=0;cacheIndex<cacheCount;cacheIndex++)
	{
		cacheEntry=globalStatistics.playerCache.Item(cacheIndex);
		decolouredName=DecolouriseName(cacheEntry.name);
		addRow=true;
		if (filterString.Length()>0)
		{
			// how to match the filter depends on the filter type
			addRow=false;
			matchString=decolouredName;
			switch (filterType)
			{
				case FILTER_TYPE_NO_CASE:
					matchString.MakeLower();
					addRow=matchString.Contains(filterString);
					break;
				case FILTER_TYPE_CASE:
					addRow=matchString.Contains(filterString);
					break;
				case FILTER_TYPE_REGEXP:
					addRow=regExp.Matches(matchString.GetData());
					break;
			}
		}
		if (filterNameCallBack!=NULL)
		{
			addRow=(addRow && (!filterNameCallBack(cacheEntry.name)));
		}
		if (addRow)
		{
			listIndex=cacheDisplay->InsertItem(rowNumber,cacheEntry.guid);
			cacheDisplay->SetItemData(listIndex,cacheIndex);
			cacheDisplay->SetItem(rowNumber,1,cacheEntry.name);
			cacheDisplay->SetItem(rowNumber,2,decolouredName);
			rowNumber++;
		}
	}
	cacheDisplay->SetColumnWidth(0,wxLIST_AUTOSIZE);
	cacheDisplay->SetColumnWidth(1,wxLIST_AUTOSIZE);
	cacheDisplay->SetColumnWidth(2,wxLIST_AUTOSIZE);
	cacheDisplay->Show();
}

void PlayerCachePanel::CreateScreen()
{
	wxString	defaultValue="";
	wxSizeEvent	event;

	if (cacheDisplay==NULL)
	{
		globalStatistics.ReadPlayerCache();
		cacheDisplay=new wxListCtrl(this,
									WINDOW_ID_LISTBOX_CONFIGITEMS,
									wxDefaultPosition,
									wxDefaultSize,
									wxLC_REPORT);
		filter=new TextConfigItemGUI();
		filter->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		filter->Set(configKey,labelText,defaultValue,-1);
		filter->OnResize(event);
//		filter=new TextConfigItemGUI(this,
//							WINDOW_ID_TEXTCTRL_CONFIGVALUE);
		UpdateCacheList();
							
	}
	else
	{
		// Screen is already created
	}
}

void PlayerCachePanel::OnRightClick(wxListEvent &event)
{
	if (GetParent()!=NULL)
	{
		// Pop the event up to the next level
		event.SetId(this->GetId());
		GetParent()->AddPendingEvent(event);
	}
}

void PlayerCachePanel::OnResize(wxSizeEvent &event)
{
	wxSize		itemSize;
	int			filterWidth;
	int			filterHeight;
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
	itemSize=filter->GetSize();
	filterWidth=itemSize.GetWidth();
	filterHeight=itemSize.GetHeight();

	listWidth=panelWidth-10;
	listHeight=panelHeight-filterHeight;
	if (listHeight<0)
	{
		listHeight=0;
	}

	filterWidth=panelWidth-10;
	cacheDisplay->SetSize(0,0,listWidth,listHeight);

	filter->SetSize(0,listHeight,filterWidth,filterHeight);
}

void PlayerCachePanel::OnColumnClick(wxListEvent &event)
{
	columnToSort=event.GetColumn();
	cacheDisplay->SortItems(PlayerCachePanel::CompareItems,(long)columnToSort);
}

int wxCALLBACK PlayerCachePanel::CompareItems(long item1, long item2, long columnToSort)
{
	wxString	field1;
	wxString	field2;
	PlayerCacheEntry	cacheEntry1;
	PlayerCacheEntry	cacheEntry2;
	int					retVal;

	cacheEntry1=globalStatistics.playerCache.Item(item1);
	cacheEntry2=globalStatistics.playerCache.Item(item2);
	switch (columnToSort)
	{
		case 0:	// GUID
			field1=cacheEntry1.guid;
			field2=cacheEntry2.guid;
			break;
		case 1:	// Coloured Name
			field1=cacheEntry1.name;
			field2=cacheEntry2.name;
			break;
		case 2:	// Decoloured Name
		default:
			field1=DecolouriseName(cacheEntry1.name);
			field2=DecolouriseName(cacheEntry2.name);
			break;
	}

	retVal=field1.Cmp(field2);

	return (retVal);
}

void PlayerCachePanel::OnFilterChanged(wxCommandEvent &event)
{
	ApplyFilter();
}

void PlayerCachePanel::ApplyFilter()
{
	UpdateCacheList();
}

void PlayerCachePanel::SetFilterType(FilterTypes filterTypeIn)
{
	filterType=filterTypeIn;
}

void PlayerCachePanel::GetSelectedPlayers(wxArrayInt &selectedPlayers)
{
	long	selectedItem;
	int		playerIndex;
	wxString	msg;

	selectedItem=-1;
	selectedItem=cacheDisplay->GetNextItem(selectedItem,
								wxLIST_NEXT_ALL,
								wxLIST_STATE_SELECTED);
	while (selectedItem!=-1)
	{
		playerIndex=(int)cacheDisplay->GetItemData(selectedItem);
		selectedPlayers.Add(playerIndex);
		selectedItem=cacheDisplay->GetNextItem(selectedItem,
								wxLIST_NEXT_ALL,
								wxLIST_STATE_SELECTED);
	}
}

void PlayerCachePanel::GetShownPlayers(wxArrayInt &shownPlayers)
{
	int			shownPlayersCount;
	int			shownPlayersIndex;
	wxListItem	listItem;
	int			playerCacheIndex;
	wxString	msg;
	
	shownPlayers.Clear();
	shownPlayersCount=cacheDisplay->GetItemCount();
	for (shownPlayersIndex=0;
		shownPlayersIndex<shownPlayersCount;
		shownPlayersIndex++)
	{
		playerCacheIndex=cacheDisplay->GetItemData(shownPlayersIndex);
		shownPlayers.Add(playerCacheIndex);
	}
}

void PlayerCachePanel::SetFilter(wxString &filterString)
{
	globalStatistics.configData.WriteTextValue(configKey,filterString);
	filter->SetConfigKey(configKey);
}

void PlayerCachePanel::SetFilterCallBack(bool  (*filterNameCallBackIn)(wxString &name))

{
	filterNameCallBack = filterNameCallBackIn;
}
