// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>
#include <wx/menu.h>

// Statsgen Includes
#include "DropListEditorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(DropListEditorPanel, wxPanel)
		EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_DROPLIST,
								DropListEditorPanel::OnDropListRightClick)
		EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_PLAYERCACHE,
								DropListEditorPanel::OnPlayerRightClick)
		EVT_MENU(PLAYER_POPUP_ADD_DROP,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(PLAYER_POPUP_ADD_ALLOW,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(PLAYER_POPUP_ADD_CHEAT,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_DELETE,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_ALLOW,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_DROP,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_DONT_DROP,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_CHEAT,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_DONTUSECOLOURCODES,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_USECOLOURCODES,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_USEEXACTMATCH,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_USEPREFIXMATCH,DropListEditorPanel::OnPopupMenu)
		EVT_MENU(DROPLIST_POPUP_USEREGEXPMATCH,DropListEditorPanel::OnPopupMenu)
		EVT_LIST_END_LABEL_EDIT(WINDOW_ID_DROPLIST,DropListEditorPanel::OnLabelEdit)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,DropListEditorPanel::OnTextChange)
END_EVENT_TABLE()

DropListEditorPanel::DropListEditorPanel()
{
	playerCache=NULL;
	dropListList=NULL;
	filterString="";

	// Lets load up the current DropList List into
	// the global statistics - we can transfer
	// it across into the tree during creation
	globalStatistics.dropList.Load();
}

bool DropListEditorPanel::Create(wxWindow *parent,
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
	playerCache->CreateScreen(this, WINDOW_ID_PLAYERCACHE);
	dropListList=new wxListCtrl(this,
							WINDOW_ID_DROPLIST,
							wxDefaultPosition,
							wxDefaultSize,
							wxLC_REPORT|
							wxLC_EDIT_LABELS);
	RefreshDropListTree();

	configItems=new GroupedConfigItemsPanel((char *)"Minimum Players");
	configItems->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);
	configKey="/General/MinPlayersInRound";
	configItems->Add((char *)"Minimum Players In Round For It To Count",configKey,(char *)"0",3);
	configKey="/General/MinRoundsPerPlayer";
	configItems->Add((char *)"Minimum Rounds Per Player",configKey,(char *)"0",6);
	configKey="/tmp/DropListPlayerFilter";
	configItems->Add((char *)"Drop List Player Filter",configKey,(char *)"",-1);

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(configItems,0,wxEXPAND);
	mMainSizer->Add(dropListList,10,wxEXPAND);
	mMainSizer->Add(playerCache,10,wxEXPAND);

	SetSizer(mMainSizer);
	mMainSizer->SetSizeHints(this);
	return (true);
}

void DropListEditorPanel::OnTextChange(wxCommandEvent &event)
{
	wxString	configKey;
	wxString	newFilterString;
	configKey="/tmp/DropListPlayerFilter";
	globalStatistics.configData.ReadTextValue(configKey,&newFilterString);
	if (newFilterString.Cmp(filterString)!=0)
	{
		// filter string has changed
		RefreshDropListTree();
	}
}

void DropListEditorPanel::AddDropListEntry(int index,
											DropListEntry &dropEntry)
{
	long		itemIndex;
	long		listIndex;
	long		rowNumber;
	int			allowCount;
	wxString	allowDropText;
	wxString	whyDroppedText;
	wxString	matchTypeText;
	wxString	colourText;
	wxString	configKey;
	bool		addRow;

	configKey="/tmp/DropListPlayerFilter";
	globalStatistics.configData.ReadTextValue(configKey,&filterString);
	allowCount=globalStatistics.dropList.allowList.GetCount();

	rowNumber=index;
	itemIndex=index;
	itemIndex++;
	allowDropText	=DROPLISTPANEL_ALLOW;
	whyDroppedText	="";
	matchTypeText	=DROPLISTPANEL_EXACT;
	colourText		=DROPLISTPANEL_NONCOLOURED;

	if (dropEntry.DropType())
	{
		itemIndex*=-1;
		rowNumber+=allowCount;
		allowDropText=DROPLISTPANEL_DROP;
		whyDroppedText=DROPLISTPANEL_PLAYERDROP;
		if (dropEntry.cheat)
		{
			whyDroppedText=DROPLISTPANEL_PLAYERCHEAT;
		}
	}
	if (dropEntry.dontDrop)
	{
		allowDropText=DROPLISTPANEL_DONT_DROP;
	}

	if (dropEntry.prefix)
	{
		matchTypeText	=DROPLISTPANEL_PREFIX;
	}
	if (dropEntry.regularExpression)
	{
		matchTypeText	=DROPLISTPANEL_REGEXP;
	}
	if (dropEntry.coloured)
	{
		colourText		=DROPLISTPANEL_COLOURED;
	}

	addRow=true;
	if ((!dropEntry.prefix)&&
		(!dropEntry.regularExpression)&&
		(dropEntry.DropType())&&
		(filterString.Length()>0))
	{
		wxString	decolourisedName;
		decolourisedName=DecolouriseName(dropEntry.name).MakeLower();
		filterString=filterString.MakeLower();
		if (decolourisedName.Contains(filterString))
		{
			// filter enabled and name contains filter string
		}
		else
		{
			// filter string enabled and filter string not in
			// name
			addRow=false;
		}
	}
	if (addRow)
	{
		rowNumber=dropListList->GetItemCount();
		listIndex=dropListList->InsertItem(rowNumber,dropEntry.name);
		dropListList->SetItemData(listIndex,itemIndex);
		dropListList->SetItem(rowNumber,1,DecolouriseName(dropEntry.name));
		dropListList->SetItem(rowNumber,2,allowDropText);
		dropListList->SetItem(rowNumber,3,whyDroppedText);
		dropListList->SetItem(rowNumber,4,colourText);
		dropListList->SetItem(rowNumber,5,matchTypeText);
	}
}

void DropListEditorPanel::RefreshDropListTree()
{
	DropListEntry		dropListEntry;
	wxListItem			listColumn;
	int					dropListCount;
	int					dropListIndex;

	globalStatistics.dropList.Sort();
	dropListList->Hide();
	
	dropListList->DeleteAllColumns();
	dropListList->DeleteAllItems();
	listColumn.SetText("Name");
	dropListList->InsertColumn(0,listColumn);
	listColumn.SetText("");
	dropListList->InsertColumn(1,listColumn);
	listColumn.SetText("Allow/Drop");
	dropListList->InsertColumn(2,listColumn);
	listColumn.SetText("Why Dropped");
	dropListList->InsertColumn(3,listColumn);
	listColumn.SetText("Coloured Codes");
	dropListList->InsertColumn(4,listColumn);
	listColumn.SetText("Match Type");
	dropListList->InsertColumn(5,listColumn);

	dropListCount=globalStatistics.dropList.allowList.GetCount();
	for (dropListIndex=0;dropListIndex<dropListCount;dropListIndex++)
	{
		dropListEntry=globalStatistics.dropList.allowList.Item(dropListIndex);
		AddDropListEntry(dropListIndex,dropListEntry);
	}

	dropListCount=globalStatistics.dropList.dropList.GetCount();
	for (dropListIndex=0;dropListIndex<dropListCount;dropListIndex++)
	{
		dropListEntry=globalStatistics.dropList.dropList.Item(dropListIndex);
		AddDropListEntry(dropListIndex,dropListEntry);
	}

	dropListList->SetColumnWidth(0,wxLIST_AUTOSIZE);
	dropListList->SetColumnWidth(1,wxLIST_AUTOSIZE);
	dropListList->SetColumnWidth(2,wxLIST_AUTOSIZE);
	dropListList->SetColumnWidth(3,wxLIST_AUTOSIZE);
	dropListList->SetColumnWidth(4,wxLIST_AUTOSIZE);
	dropListList->SetColumnWidth(5,wxLIST_AUTOSIZE);
	dropListList->Show();
}

void DropListEditorPanel::OnDropListRightClick(wxListEvent &event)
{
	wxString		msg;
	wxMenu			popupMenu(_T(""));
	wxString		menuItem;

	// What have we right clicked on - parent or child?
	menuItem="Delete"; popupMenu.Append(DROPLIST_POPUP_DELETE,menuItem);
	popupMenu.AppendSeparator();

	menuItem="Dont Use Colour Codes"; popupMenu.Append(DROPLIST_POPUP_DONTUSECOLOURCODES,menuItem);
	menuItem="Use Colour Codes"; popupMenu.Append(DROPLIST_POPUP_USECOLOURCODES,menuItem);
	popupMenu.AppendSeparator();

	menuItem="Only Allow These Players"; popupMenu.Append(DROPLIST_POPUP_ALLOW,menuItem);
	menuItem="These Players Are Cheats"; popupMenu.Append(DROPLIST_POPUP_CHEAT,menuItem);
	menuItem="Drop These Players"; popupMenu.Append(DROPLIST_POPUP_DROP,menuItem);
	menuItem="Don't Drop These Players"; popupMenu.Append(DROPLIST_POPUP_DONT_DROP,menuItem);
	popupMenu.AppendSeparator();

	menuItem="Use Exact Match"; popupMenu.Append(DROPLIST_POPUP_USEEXACTMATCH,menuItem);
	menuItem="Use Prefix Match"; popupMenu.Append(DROPLIST_POPUP_USEPREFIXMATCH,menuItem);
	menuItem="Use Regular Exp Match"; popupMenu.Append(DROPLIST_POPUP_USEREGEXPMATCH,menuItem);
	popupMenu.AppendSeparator();
	popupMenu.Append(DROPLIST_POPUP_CANCEL,_T("Cancel"));
	PopupMenu(&popupMenu);
}

void DropListEditorPanel::OnPlayerRightClick(wxListEvent &event)
{
	wxString		msg;
	wxString		menuItem;
	wxMenu			popupMenu(_T(""));

	// Right clicked on an item in the Player Cache
	menuItem="Add To Players To Be Dropped";
	popupMenu.Append(PLAYER_POPUP_ADD_DROP,menuItem);
	menuItem="Add To Players Who Are Cheats";
	popupMenu.Append(PLAYER_POPUP_ADD_CHEAT,menuItem);
	menuItem="Add To Players To Allow";
	popupMenu.Append(PLAYER_POPUP_ADD_ALLOW,menuItem);
	popupMenu.AppendSeparator();
	popupMenu.Append(DROPLIST_POPUP_CANCEL,_T("Cancel"));
	PopupMenu(&popupMenu);
}

void DropListEditorPanel::OnPopupMenu(wxCommandEvent &event)
{
	wxString			msg;
	int					id;
	bool				removeColourCodes;
	wxString			howMatched;
	wxArrayInt			selectedPlayers;
	int					selectedCount;
	int					cacheIndex;
	wxString			decolouredName;
	wxString			longestMatch;
	int					length;
	int					charIndex;
	PlayerCacheEntry	cacheEntry;
	int					playerIndex;
	int					playerCount;
	long				selectedItem=-1;
	int					listIndex;
	wxString			nameToRemove;

	id=event.GetId();

	switch (id)
	{
		case DROPLIST_POPUP_ALLOW:
		case DROPLIST_POPUP_DONT_DROP:
		case DROPLIST_POPUP_DROP:
		case DROPLIST_POPUP_CHEAT:
		case DROPLIST_POPUP_DONTUSECOLOURCODES:
		case DROPLIST_POPUP_USECOLOURCODES:
		case DROPLIST_POPUP_USEEXACTMATCH:
		case DROPLIST_POPUP_USEPREFIXMATCH:
		case DROPLIST_POPUP_USEREGEXPMATCH:
			selectedPlayers.Clear();
			selectedItem=dropListList->GetNextItem(selectedItem,
													wxLIST_NEXT_ALL,
													wxLIST_STATE_SELECTED);
			while (selectedItem!=-1)
			{
				listIndex=(int)dropListList->GetItemData(selectedItem);
				DropListEntry	oldEntry;
				DropListEntry	newEntry;
				if (listIndex<0)
				{
					listIndex=abs(listIndex);
					listIndex--;
					oldEntry=globalStatistics.dropList.dropList.Item(listIndex);
				}
				else
				{
					listIndex--;
					oldEntry=globalStatistics.dropList.allowList.Item(listIndex);
				}
				newEntry=oldEntry;
				switch (id)
				{
					case DROPLIST_POPUP_ALLOW:
						newEntry.drop=false;
						newEntry.cheat=false;
						newEntry.dontDrop=false;
						break;
					case DROPLIST_POPUP_DONT_DROP:
						newEntry.drop=false;
						newEntry.cheat=false;
						newEntry.dontDrop=true;
						break;
					case DROPLIST_POPUP_DROP:
						newEntry.drop=true;
						newEntry.cheat=false;
						newEntry.dontDrop=false;
						break;
					case DROPLIST_POPUP_CHEAT:
						newEntry.drop=true;
						newEntry.cheat=true;
						newEntry.dontDrop=false;
						break;
					case DROPLIST_POPUP_DONTUSECOLOURCODES:
						newEntry.coloured=false;
						break;
					case DROPLIST_POPUP_USECOLOURCODES:
						newEntry.coloured=true;
						break;
					case DROPLIST_POPUP_USEEXACTMATCH:
						newEntry.prefix=false;
						newEntry.regularExpression=false;
						break;
					case DROPLIST_POPUP_USEPREFIXMATCH:
						newEntry.prefix=true;
						newEntry.regularExpression=false;
						break;
					case DROPLIST_POPUP_USEREGEXPMATCH:
						newEntry.prefix=true;
						newEntry.regularExpression=true;
						break;
				}

				globalStatistics.dropList.RemoveEntry(oldEntry);
				globalStatistics.dropList.AddEntry(newEntry);
				selectedItem=dropListList->GetNextItem(selectedItem,
													wxLIST_NEXT_ALL,
													wxLIST_STATE_SELECTED);
			}
			RefreshDropListTree();
			break;
		case DROPLIST_POPUP_DELETE:
			selectedPlayers.Clear();
			selectedItem=dropListList->GetNextItem(selectedItem,
													wxLIST_NEXT_ALL,
													wxLIST_STATE_SELECTED);
			while (selectedItem!=-1)
			{
				listIndex=(int)dropListList->GetItemData(selectedItem);
				nameToRemove=dropListList->GetItemText(selectedItem);
				DropListEntry	dropListEntry;
				dropListEntry.drop=(listIndex<0);	// negative index for drop
				dropListEntry.name=nameToRemove;
				globalStatistics.dropList.RemoveEntry(dropListEntry);
				selectedItem=dropListList->GetNextItem(selectedItem,
													wxLIST_NEXT_ALL,
													wxLIST_STATE_SELECTED);
			}
			RefreshDropListTree();
			break;
		case PLAYER_POPUP_ADD_ALLOW:
		case PLAYER_POPUP_ADD_DROP:
		case PLAYER_POPUP_ADD_CHEAT:
			// Some sub category questions now
			// some are dependent on the amount of
			// players selected
			removeColourCodes=(wxMessageBox("Remove Colour Codes?","Confirm",wxYES_NO)==wxYES);
			playerCache->GetSelectedPlayers(selectedPlayers);
			selectedCount=selectedPlayers.GetCount();
			if (selectedCount>1)
			{
				// we are possibly adding a prefix
				wxArrayString	choices;
				wxString		choice;
				choice=DROPLISTPANEL_EXACT;choices.Add(choice);
				choice=DROPLISTPANEL_PREFIX;choices.Add(choice);
				choice=DROPLISTPANEL_REGEXP;choices.Add(choice);

				howMatched=wxGetSingleChoice("How Do You Want To Match The Names?","Choose",choices);
			}
			else
			{
				howMatched=DROPLISTPANEL_EXACT;
			}
			wxArrayString		namesToAdd;
			wxString			nameToAdd;
			if (howMatched.Cmp(DROPLISTPANEL_EXACT)!=0)
			{
				// we have asked for a prefix type match
				playerIndex=selectedPlayers.Item(0);
				cacheEntry=globalStatistics.playerCache.Item(playerIndex);
				if (removeColourCodes)
				{
					longestMatch=DecolouriseName(cacheEntry.name);
				}
				else
				{
					longestMatch=cacheEntry.name;
				}
				for (cacheIndex=1;cacheIndex<selectedCount;cacheIndex++)
				{
					playerIndex=selectedPlayers.Item(cacheIndex);
					cacheEntry=globalStatistics.playerCache.Item(playerIndex);
					if (removeColourCodes)
					{
						decolouredName=DecolouriseName(cacheEntry.name);
					}
					else
					{
						decolouredName=cacheEntry.name;
					}
					length=longestMatch.Length();
					if (length>decolouredName.Length())
					{
						length=decolouredName.Length();
					}

					// now we need to scan each character
					// in the longest match and the decoloured name
					// and match when we reach a new character
					for (charIndex=0;charIndex<length;charIndex++)
					{
						if (longestMatch[charIndex]!=decolouredName[charIndex])
						{
							longestMatch=longestMatch.Left(charIndex);
						}
					}
				}

				if (longestMatch.Length()>1)
				{
					// found a common piece of string at least 2 char long
					// convert it to a regular expression if needed
					wxString	regularExpression="^";
					if (howMatched.Cmp(DROPLISTPANEL_REGEXP)==0)
					{
						length=longestMatch.Length();
						for (charIndex=0;charIndex<length;charIndex++)
						{
							regularExpression+="\\";
							regularExpression+=longestMatch[charIndex];
						}
						regularExpression+=".*";
						longestMatch=regularExpression;
					}

					// Longest Match is the prefix 
					nameToAdd=longestMatch;
					namesToAdd.Add(nameToAdd);
				}
				else
				{
					// Sod it - no match - just add each name individually
					howMatched=DROPLISTPANEL_EXACT;
					for (cacheIndex=0;cacheIndex<selectedCount;cacheIndex++)
					{
						playerIndex=selectedPlayers.Item(cacheIndex);
						cacheEntry=globalStatistics.playerCache.Item(playerIndex);
						if (removeColourCodes)
						{
							nameToAdd=DecolouriseName(cacheEntry.name);
						}
						else
						{
							nameToAdd=cacheEntry.name;
						}
						namesToAdd.Add(nameToAdd);
					}
				}

			}
			else
			{
				// exact match - 
				for (cacheIndex=0;cacheIndex<selectedCount;cacheIndex++)
				{
					playerIndex=selectedPlayers.Item(cacheIndex);
					cacheEntry=globalStatistics.playerCache.Item(playerIndex);
					if (removeColourCodes)
					{
						nameToAdd=DecolouriseName(cacheEntry.name);
					}
					else
					{
						nameToAdd=cacheEntry.name;
					}
					namesToAdd.Add(nameToAdd);
				}
			}

			// ok - by now we have a list of names to add - 
			// we know what to add them to and what they are
			// we know how to match them
			playerCount=namesToAdd.GetCount();
			for (playerIndex=0;playerIndex<playerCount;playerIndex++)
			{
				DropListEntry	dropListEntry;

				nameToAdd=namesToAdd.Item(playerIndex);
				dropListEntry.name=nameToAdd;
				dropListEntry.dontDrop=false;
				dropListEntry.drop=(id!=PLAYER_POPUP_ADD_ALLOW);
				dropListEntry.cheat=(id==PLAYER_POPUP_ADD_CHEAT);
				dropListEntry.coloured=(!removeColourCodes);
				dropListEntry.prefix=(howMatched.Cmp(DROPLISTPANEL_EXACT)!=0);
				dropListEntry.regularExpression=(howMatched.Cmp(DROPLISTPANEL_REGEXP)==0);

				globalStatistics.dropList.AddEntry(dropListEntry);
			}
			RefreshDropListTree();
			break;
	}
}

void DropListEditorPanel::OnLabelEdit(wxListEvent &event)
{
	wxString		msg;
	wxString		menuItem;
	wxMenu			popupMenu(_T(""));
	long			index;
	long			dropListIndex;
	DropListEntry	dropListEntry;

	index=event.GetIndex();

	dropListIndex=dropListList->GetItemData(index);

	if (dropListIndex<0)
	{
		// Drop List
		dropListEntry=globalStatistics.dropList.dropList.Item(abs(dropListIndex)-1);
	}
	else
	{
		// Allow List
		dropListEntry=globalStatistics.dropList.allowList.Item(abs(dropListIndex)-1);
	}

}

