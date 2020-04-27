// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>
#include <wx/menu.h>

// Statsgen Includes
#include "AliasEditorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(AliasEditorPanel, wxPanel)
		EVT_SIZE(AliasEditorPanel::OnResize)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_ALIASTREE,
								AliasEditorPanel::OnAliasRightClick)
		EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_PLAYERCACHE,
								AliasEditorPanel::OnPlayerRightClick)
		EVT_MENU(ALIAS_POPUP_DELETE_PARENT,AliasEditorPanel::OnPopupMenu)
		EVT_MENU(ALIAS_POPUP_DELETE_CHILD,AliasEditorPanel::OnPopupMenu)
		EVT_MENU(ALIAS_POPUP_REPARENT,AliasEditorPanel::OnPopupMenu)
		EVT_MENU(PLAYER_POPUP_NEW_ALIAS,AliasEditorPanel::OnPopupMenu)
		EVT_MENU(PLAYER_POPUP_ADD_ALIAS,AliasEditorPanel::OnPopupMenu)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,AliasEditorPanel::OnTextChange)
END_EVENT_TABLE()

AliasEditorPanel::AliasEditorPanel()
{
	playerCache=NULL;
	aliasList=NULL;

	// Lets load up the current Alias List into
	// the global statistics - we can transfer
	// it across into the tree during creation
	globalStatistics.ReadAliasList();
}

bool AliasEditorPanel::Create(wxWindow *parent,
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name)
{
	wxString	dummyConfigKey="/tmp/tmp";
	wxString	labelText="Player Filter";
	long		treeCtrlStyle=wxTR_HAS_BUTTONS;
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
	aliasList=new wxTreeCtrl(this,
							WINDOW_ID_ALIASTREE,
							wxDefaultPosition,
							wxDefaultSize,
							treeCtrlStyle,
							wxDefaultValidator);
	aliasConfigs=new GroupedConfigItemsPanel("Alias");
	aliasConfigs->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,wxDefaultPosition,wxDefaultSize);
	configKey=globalStatistics.AutomaticAliasConfigKey();
	aliasConfigs->AddBoolean("Use Automatic Aliasing During Log Processing",
							configKey,
							false);
	configKey="/tmp/aliasfilter";
	aliasConfigs->Add("Alias List Player Filter",
							configKey,
							"",
							-1);
	playerCache->SetFilterCallBack(FilterNameCallBack);
	RefreshAliasTree();

	return (true);
}

void AliasEditorPanel::OnResize(wxSizeEvent &event)
{
	wxSize		itemSize;
	int			playerCacheWidth;
	int			playerCacheHeight;
	int			panelWidth;
	int			panelHeight;
	int			aliasWidth;
	int			aliasHeight;
	int			aliasConfigsHeight;
	int			aliasConfigsWidth;
	wxString	msg;


	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();
	playerCacheWidth=(panelWidth*3)/5;
	playerCacheHeight=panelHeight;
	aliasWidth=panelWidth-playerCacheWidth;
	aliasHeight=panelHeight;

	if (aliasList!=NULL)
	{
		aliasConfigsHeight=20;
		aliasConfigsWidth=panelWidth;
		aliasConfigs->SetSize(0,0,aliasConfigsWidth,aliasConfigsHeight);
		itemSize=aliasConfigs->GetSize();
		aliasConfigsHeight=itemSize.GetHeight()+20;
		aliasConfigsHeight=aliasConfigs->PreferredHeight();
		aliasHeight-=aliasConfigsHeight;
		playerCacheHeight-=aliasConfigsHeight;
		aliasConfigs->SetSize(0,0,aliasConfigsWidth,aliasConfigsHeight);
		aliasList->SetSize(0,aliasConfigsHeight,aliasWidth,aliasHeight);
		playerCache->SetSize(aliasWidth,aliasConfigsHeight,playerCacheWidth,playerCacheHeight);
	}

}

wxTreeItemId AliasEditorPanel::AddPlayerToTree(
									wxTreeItemId &parentItem,
									wxString &guid,
									wxString &name)
{
	wxString			playerString;
	wxString			foundPlayerString;
	wxTreeItemId		childItem;
	wxTreeItemIdValue	cookie;
	bool				found=false;

	playerString.Printf("(%s) %s",
						guid.GetData(),
						name.GetData());

	found=false;

	childItem=aliasList->GetFirstChild(parentItem,cookie);
	while (childItem.IsOk())
	{
		foundPlayerString=aliasList->GetItemText(childItem);
		if (foundPlayerString.Cmp(playerString)==0)
		{
			found=true;
			break;
		}
		childItem=aliasList->GetNextChild(parentItem,cookie);
	}
	if (!found)
	{
		childItem=aliasList->AppendItem(parentItem,playerString);
	}

	return (childItem);
}

void AliasEditorPanel::AddAlias(AliasListEntry &aliasEntry)
{
	wxTreeItemId		rootItem;
	wxTreeItemId		parentItem;
	wxTreeItemId		childItem;

	rootItem=aliasList->GetRootItem();
	parentItem=AddPlayerToTree( rootItem,
								aliasEntry.primaryGUID,
								aliasEntry.primaryName);
	childItem=AddPlayerToTree( parentItem,
								aliasEntry.aliasGUID,
								aliasEntry.aliasName);
	aliasList->Expand(rootItem);
}

void AliasEditorPanel::RefreshAliasTree()
{
	AliasListEntry		aliasEntry;
	int					aliasCount;
	int					aliasIndex;
	wxTreeItemId		rootItem;
	wxString			rootText="Alias List";
	wxString			decolouredAlias;
	wxString			decolouredName;
	bool				dontFilter;

	aliasList->Hide();
	aliasList->DeleteAllItems();
	
	rootItem=aliasList->AddRoot(rootText);
	aliasCount=globalStatistics.aliasList.GetCount();
	for (aliasIndex=0;aliasIndex<aliasCount;aliasIndex++)
	{
		aliasEntry=globalStatistics.aliasList.Item(aliasIndex);
		decolouredAlias=aliasEntry.aliasName;
		decolouredName=aliasEntry.primaryName;
		
		decolouredAlias=DecolouriseName(decolouredAlias.MakeLower());
		decolouredName=DecolouriseName(decolouredName.MakeLower());
		dontFilter=true;
		if (aliasFilter.Length()>0)
		{
			dontFilter=((decolouredAlias.Find(aliasFilter)>=0) ||
						(decolouredName.Find(aliasFilter)>=0));
	
		}
		if (dontFilter)
		{
			AddAlias(aliasEntry);
		}
	}
	aliasList->Expand(rootItem);
	aliasList->Show();
}

void AliasEditorPanel::GetShownPlayers(wxArrayInt &shownPlayers)
{
	playerCache->GetShownPlayers(shownPlayers);
}

void AliasEditorPanel::OnAliasRightClick(wxTreeEvent &event)
{
	wxString		msg;
	wxTreeItemId	rootItem;
	wxTreeItemId	parentItem;
	wxTreeItemId	clickedItem;
	wxMenu			popupMenu(_T(""));
	wxString		menuItem;
	wxString		parentString;
	wxString		childString;

	// What have we right clicked on - parent or child?
	rootItem=aliasList->GetRootItem();
	clickedItem=event.GetItem();
	if (clickedItem!=rootItem)
	{
		parentItem=aliasList->GetItemParent(clickedItem);
		if (parentItem==rootItem)
		{
			parentString=aliasList->GetItemText(clickedItem);
			// Clicked on a parent
			menuItem="Delete All Alias Entries for "+parentString;
			popupMenu.Append(ALIAS_POPUP_DELETE_PARENT,menuItem);
		}
		else
		{
			// Clicked on a child
			childString=aliasList->GetItemText(clickedItem);
			parentString=aliasList->GetItemText(parentItem);
			menuItem="Delete Alias Entry "+childString;
			popupMenu.Append(ALIAS_POPUP_DELETE_CHILD,menuItem);
			menuItem="Make "+childString+" the new parent";
			popupMenu.Append(ALIAS_POPUP_REPARENT,menuItem);
		}
		popupMenu.Append(ALIAS_POPUP_CANCEL,_T("Cancel"));
		PopupMenu(&popupMenu);
	}
}

void AliasEditorPanel::OnPlayerRightClick(wxListEvent &event)
{
	wxString		msg;
	wxString		menuItem;
	wxMenu			popupMenu(_T(""));

	// Right clicked on an item in the Player Cache
	menuItem="Create New Alias";
	popupMenu.Append(PLAYER_POPUP_NEW_ALIAS,menuItem);
	menuItem="Add To Alias";
	popupMenu.Append(PLAYER_POPUP_ADD_ALIAS,menuItem);
	popupMenu.Append(ALIAS_POPUP_CANCEL,_T("Cancel"));
	PopupMenu(&popupMenu);
}

void AliasEditorPanel::AliasItemSplit(wxTreeItemId &treeItem,AliasListEntry &aliasEntry)
{
	wxString		combinedText;
	wxTreeItemId	rootItem;
	wxTreeItemId	parentItem;
	wxTreeItemId	childItem;
	wxString		guid;

	rootItem=aliasList->GetRootItem();
	parentItem=aliasList->GetItemParent(treeItem);
	if (parentItem!=rootItem)
	{
		childItem=treeItem;
	}
	else
	{
		parentItem=treeItem;
	}

	if (childItem.IsOk())
	{
		combinedText=aliasList->GetItemText(childItem);
		aliasEntry.aliasGUID=combinedText.BeforeFirst(' ');
		aliasEntry.aliasGUID=aliasEntry.aliasGUID.AfterFirst('(');
		aliasEntry.aliasGUID=aliasEntry.aliasGUID.BeforeLast(')');
		aliasEntry.aliasName=combinedText.AfterFirst(' ');
	}
	if (parentItem.IsOk())
	{
		combinedText=aliasList->GetItemText(parentItem);
		aliasEntry.primaryGUID=combinedText.BeforeFirst(' ');
		aliasEntry.primaryGUID=aliasEntry.primaryGUID.AfterFirst('(');
		aliasEntry.primaryGUID=aliasEntry.primaryGUID.BeforeLast(')');
		aliasEntry.primaryName=combinedText.AfterFirst(' ');
	}
}

void AliasEditorPanel::DeleteAlias(wxTreeItemId &parentItem,
									wxTreeItemId &childItem)
{
	int				childCount;
	AliasListEntry	aliasEntry;
	wxString		msg;

	AliasItemSplit(childItem,aliasEntry);

	aliasList->Delete(childItem);
	childCount=aliasList->GetChildrenCount(parentItem);
	if (childCount==0)
	{
		// No children - so delete parent
		aliasList->Delete(parentItem);
	}
	globalStatistics.DeleteAlias(aliasEntry);
}

void AliasEditorPanel::AddCacheToParent(wxTreeItemId &parentItem,
										PlayerCacheEntry &cacheEntry)
{
	AliasListEntry	aliasEntry;

	AliasItemSplit(parentItem,aliasEntry);
	aliasEntry.aliasGUID=cacheEntry.guid;
	aliasEntry.aliasName=cacheEntry.name;
	AddAlias(aliasEntry);
	globalStatistics.AddAlias(aliasEntry);
}

void AliasEditorPanel::AddAliasTree(wxTreeItemId &parentItem)
{
	wxTreeItemId		childItem;
	wxTreeItemIdValue	cookie;
	AliasListEntry		aliasEntry;

	STATSGEN_DEBUG_FUNCTION_START("AliasEditorPanel","AddAliasTree")

	childItem=aliasList->GetFirstChild(parentItem,cookie);
	while (childItem.IsOk())
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Adding alias to global area")
		AliasItemSplit(childItem,aliasEntry);
		globalStatistics.AddAlias(aliasEntry);
		childItem=aliasList->GetNextChild(parentItem,cookie);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void AliasEditorPanel::OnPopupMenu(wxCommandEvent &event)
{
	wxString			msg;
	int					id;
	wxArrayInt			selectedPlayers;
	wxTreeItemId		selectedAlias;
	wxTreeItemId		parentItem;
	wxTreeItemId		childItem;
	wxTreeItemId		rootItem;
	wxTreeItemIdValue	cookie;
	PlayerCacheEntry	cacheEntry;
	int					cacheCount;
	int					cacheIndex;
	int					selectedIndex;

	STATSGEN_DEBUG_FUNCTION_START("AliasEditorPanel","OnPopupMenu")
	int					playerIndex;
	AliasListEntry		aliasEntry;
	wxString			parentString;
	wxString			childString;

	id=event.GetId();
	msg.Printf("AliasEditorPanel::OnPopupMenu():Event Received %d",id);
	playerCache->GetSelectedPlayers(selectedPlayers);
	cacheCount=selectedPlayers.GetCount();
	selectedAlias=aliasList->GetSelection();
	rootItem=aliasList->GetRootItem();
	switch (event.GetId())
	{
		case ALIAS_POPUP_DELETE_PARENT:
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Delete Parent")
			parentItem=selectedAlias;
			childItem=aliasList->GetFirstChild(parentItem,cookie);
			while (childItem.IsOk())
			{
				AliasItemSplit(childItem,aliasEntry);
				globalStatistics.DeleteAlias(aliasEntry);
				childItem=aliasList->GetNextChild(parentItem,cookie);
			}
			aliasList->DeleteChildren(parentItem);
			aliasList->Delete(parentItem);
			playerCache->ApplyFilter();
			break;
		case ALIAS_POPUP_DELETE_CHILD:
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Delete Child")
			childItem=selectedAlias;
			parentItem=aliasList->GetItemParent(childItem);
			DeleteAlias(parentItem,childItem);
			playerCache->ApplyFilter();
			break;
		case ALIAS_POPUP_REPARENT:
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Reparent")
			childItem=selectedAlias;
			parentItem=aliasList->GetItemParent(childItem);

			// Let us delete aliases from the global area first
			childItem=aliasList->GetFirstChild(parentItem,cookie);
			while (childItem.IsOk())
			{
				AliasItemSplit(childItem,aliasEntry);
				globalStatistics.DeleteAlias(aliasEntry);
				childItem=aliasList->GetNextChild(parentItem,cookie);
			}

			// Now Reparent
			childItem=selectedAlias;
			parentString=aliasList->GetItemText(parentItem);
			childString=aliasList->GetItemText(childItem);
			aliasList->SetItemText(parentItem,childString);
			aliasList->SetItemText(childItem,parentString);
			// Now Add New Alias entries to global area
			AddAliasTree(parentItem);
			playerCache->ApplyFilter();
			break;
		case PLAYER_POPUP_NEW_ALIAS:
			STATSGEN_DEBUG(DEBUG_ALWAYS,"New Alias")
			if (cacheCount>1)
			{
				// Need at least 2 entries selected to create a new alias
				// First entry will be the parent
				playerIndex=selectedPlayers.Item(0);
				cacheEntry=globalStatistics.playerCache.Item(playerIndex);
				// Add the root item to the tree
				parentItem=AddPlayerToTree( rootItem,
								cacheEntry.guid,
								cacheEntry.name);
				// Now add all the children
				for (cacheIndex=1;cacheIndex<cacheCount;cacheIndex++)
				{
					playerIndex=selectedPlayers.Item(cacheIndex);
					cacheEntry=globalStatistics.playerCache.Item(playerIndex);
					AddCacheToParent(parentItem,cacheEntry);
				}
				playerCache->ApplyFilter();
			}
			break;
		case PLAYER_POPUP_ADD_ALIAS:
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Add Alias")
			childItem=selectedAlias;
			if (childItem.IsOk())
			{
				parentItem=aliasList->GetItemParent(childItem);
				if (parentItem==rootItem)
				{
					parentItem=childItem;
				}
				for (cacheIndex=0;cacheIndex<cacheCount;cacheIndex++)
				{
					playerIndex=selectedPlayers.Item(cacheIndex);
					cacheEntry=globalStatistics.playerCache.Item(playerIndex);
					AddCacheToParent(parentItem,cacheEntry);
				}
				playerCache->ApplyFilter();
			}
			break;
	}
	STATSGEN_DEBUG_FUNCTION_END
}

bool AliasEditorPanel::FilterNameCallBack(wxString &name)
{
	bool			retVal=false;
	AliasListEntry	aliasEntry;
	int				aliasIndex;
	int				aliasCount;

	aliasCount=globalStatistics.aliasList.GetCount();
	for (aliasIndex=0;aliasIndex<aliasCount;aliasIndex++)
	{
		aliasEntry=globalStatistics.aliasList.Item(aliasIndex);
		if ((name.Cmp(aliasEntry.primaryName)==0) ||
			(name.Cmp(aliasEntry.aliasName)==0))
		{
			retVal=true;
			break;
		}
	}

	return (retVal);
}

void AliasEditorPanel::OnTextChange(wxCommandEvent &event)
{
	wxString	configKey;
	wxString	configValue;

	configKey="/tmp/aliasfilter";
	globalStatistics.configData.ReadTextValue(configKey,&configValue);

	aliasFilter=configValue;
	RefreshAliasTree();
}

