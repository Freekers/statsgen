// wx includes
#include <wx/wx.h>
#include <wx/regex.h>

// Statsgen Includes
#include "PlayerCachePanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "ResizingListCtrl.h"

BEGIN_EVENT_TABLE(PlayerCachePanel, wxPanel)
		//EVT_SIZE(PlayerCachePanel::OnSize)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,PlayerCachePanel::OnFilterChanged)
		EVT_LIST_COL_CLICK(WINDOW_ID_LISTBOX_CONFIGITEMS,PlayerCachePanel::OnColumnClick)
		EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_LISTBOX_CONFIGITEMS,
					PlayerCachePanel::OnRightClick)
END_EVENT_TABLE()

PlayerCachePanel::PlayerCachePanel(wxString &configKeyIn,wxString &labelTextIn)
{
	mCacheDisplay	= NULL;
	mFilter			= NULL;
	mConfigKey		= configKeyIn;
	mLabelText		= labelTextIn;
	SetFilterType(FILTER_TYPE_NO_CASE);
	mFilterNameCallBack=(bool (*)(wxString &name))NULL;

}

void PlayerCachePanel::UpdateCacheList()
{
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

	globalStatistics.configData.ReadTextValue(mConfigKey,&filterString);
	
	switch (mFilterType)
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
	mCacheDisplay->Hide();
	mCacheDisplay->DeleteAllColumns();
	mCacheDisplay->DeleteAllItems();
	mCacheDisplay->InsertColumn(0,_T("GUID"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE_USEHEADER);
	mCacheDisplay->InsertColumn(1,_T("Name"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE_USEHEADER);
	mCacheDisplay->InsertColumn(2,_T("Decoloured Name"),wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE_USEHEADER);
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
			switch (mFilterType)
			{
				case FILTER_TYPE_NO_CASE:
					matchString.MakeLower();
					addRow=matchString.Contains(filterString);
					break;
				case FILTER_TYPE_CASE:
					addRow=matchString.Contains(filterString);
					break;
				case FILTER_TYPE_REGEXP:
					addRow=regExp.Matches(STRING_TO_CHAR(matchString));
					break;
			}
		}
		if (mFilterNameCallBack!=NULL)
		{
			addRow=(addRow && (!mFilterNameCallBack(cacheEntry.name)));
		}
		if (addRow)
		{
			listIndex=mCacheDisplay->InsertItem(rowNumber,cacheEntry.guid);
			mCacheDisplay->SetItemData(listIndex,cacheIndex);
			mCacheDisplay->SetItem(rowNumber,1,cacheEntry.name);
			mCacheDisplay->SetItem(rowNumber,2,decolouredName);
			rowNumber++;
		}
	}
	mCacheDisplay->SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);
	mCacheDisplay->SetColumnWidth(1,wxLIST_AUTOSIZE_USEHEADER);
	mCacheDisplay->SetColumnWidth(2,wxLIST_AUTOSIZE_USEHEADER);
	mCacheDisplay->Show();

	
	PostSizeEventToParent();
}

void PlayerCachePanel::CreateScreen(wxWindow *parent,int id)
{
	Create(parent,id,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE);
	wxString	defaultValue="";

	globalStatistics.ReadPlayerCache();
	mCacheDisplay=new wxListCtrl(this,
									WINDOW_ID_LISTBOX_CONFIGITEMS,
									wxDefaultPosition,
									wxDefaultSize,
									wxLC_REPORT);
	mFilter=new TextConfigItemGUI();
	mFilter->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,mLabelText);
	mFilter->SetConfigKey(mConfigKey,defaultValue);
	mFilter->ApplyConfigKeyChange();
	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(mCacheDisplay,1,wxEXPAND|wxALL);
	mMainSizer->Add(mFilter,0,wxEXPAND);
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
	UpdateCacheList();
}

void PlayerCachePanel::OnRightClick(wxListEvent &event)
{
	if (GetParent()!=NULL)
	{
		// Pop the event up to the next level
		event.SetId(this->GetId());
		//GetParent()->AddPendingEvent(event);
		GetParent()->GetEventHandler()->AddPendingEvent(event);
	}
}

void PlayerCachePanel::OnColumnClick(wxListEvent &event)
{
	mColumnToSort=event.GetColumn();
	mCacheDisplay->SortItems(PlayerCachePanel::CompareItems,(wxIntPtr)mColumnToSort);
}

int wxCALLBACK PlayerCachePanel::CompareItems(wxIntPtr item1, wxIntPtr item2, wxIntPtr mColumnToSort)
{
	wxString	field1;
	wxString	field2;
	PlayerCacheEntry	cacheEntry1;
	PlayerCacheEntry	cacheEntry2;
	int					retVal;

	cacheEntry1=globalStatistics.playerCache.Item(item1);
	cacheEntry2=globalStatistics.playerCache.Item(item2);
	switch (mColumnToSort)
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
	mFilterType	= filterTypeIn;
}

void PlayerCachePanel::GetSelectedPlayers(wxArrayInt &selectedPlayers)
{
	long	selectedItem;
	int		playerIndex;
	wxString	msg;

	selectedItem=-1;
	selectedItem=mCacheDisplay->GetNextItem(selectedItem,
								wxLIST_NEXT_ALL,
								wxLIST_STATE_SELECTED);
	while (selectedItem!=-1)
	{
		playerIndex=(int)mCacheDisplay->GetItemData(selectedItem);
		selectedPlayers.Add(playerIndex);
		selectedItem=mCacheDisplay->GetNextItem(selectedItem,
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
	shownPlayersCount=mCacheDisplay->GetItemCount();
	for (shownPlayersIndex=0;
		shownPlayersIndex<shownPlayersCount;
		shownPlayersIndex++)
	{
		playerCacheIndex=mCacheDisplay->GetItemData(shownPlayersIndex);
		shownPlayers.Add(playerCacheIndex);
	}
}

void PlayerCachePanel::SetFilter(wxString &filterString)
{
	globalStatistics.configData.WriteTextValue(mConfigKey,filterString);
	mFilter->SetConfigKey(mConfigKey);
}

void PlayerCachePanel::SetFilterCallBack(bool  (*filterNameCallBackIn)(wxString &name))

{
	mFilterNameCallBack = filterNameCallBackIn;
}
void PlayerCachePanel::OnSize(wxSizeEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("PlayerCachePanel","OnSize")
	STATSGEN_DEBUG_FUNCTION_END
}
