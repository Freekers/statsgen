// wx includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>
#include <wx/menu.h>

// Statsgen Includes
#include "ClanEditorPanel.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

BEGIN_EVENT_TABLE(ClanEditorPanel, GenericConfigPanel)
		EVT_LIST_ITEM_RIGHT_CLICK(WINDOW_ID_PLAYERCACHE,
								ClanEditorPanel::OnPlayerRightClick)
		EVT_MENU(PLAYER_POPUP_SELECT,ClanEditorPanel::OnPopupMenu)
END_EVENT_TABLE()

ClanEditorPanel::ClanEditorPanel(wxString &configGroupIn,
								wxWindow *parent,
								wxWindowID id,
								const wxPoint &pos,
								const wxSize &size,
								long style,
								const wxString &name) :
					GenericConfigPanel(
								parent,
								id,
								pos,
								size,
								style,
								name)
{
	playerCache=NULL;
	configGroup=configGroupIn;

	wxString	labelText="Regular Expression";
	wxString	configKey;

	GroupedConfigItemsPanel	*clanPanel;

	clanPanel=new GroupedConfigItemsPanel("Clan Details");
	clanPanel->Create(this,
				-1,
				wxDefaultPosition,
				wxDefaultSize);
	
	configKey.Printf("/%s/TagColoured",configGroup.GetData());
	clanPanel->Add("Coloured Tag",configKey,"",-1);
	configKey.Printf("/%s/FullName",configGroup.GetData());
	clanPanel->Add("Coloured Name",configKey,"",-1);
	configKey.Printf("/%s/WebAddress",configGroup.GetData());
	clanPanel->Add("Web Address",configKey,"",-1);
	configKey.Printf("/%s/BannerAddress",configGroup.GetData());
	clanPanel->Add("Banner Image Address",configKey,"",-1);

	AddConfigGroup(clanPanel);

	configKey.Printf("/%s/RegExp",configGroup.GetData());
	playerCache=new PlayerCachePanel(configKey,labelText);

	playerCache->SetFilterType(PlayerCachePanel::FILTER_TYPE_REGEXP);
	playerCache->Create(this,
				WINDOW_ID_PLAYERCACHE,
				wxDefaultPosition,
				wxDefaultSize);
	AddConfigGroup(playerCache);
}

void ClanEditorPanel::OnPlayerRightClick(wxListEvent &event)
{
	wxString		msg;
	wxString		menuItem;
	wxMenu			popupMenu(_T(""));
	wxArrayInt		selectedPlayers;
	int				cacheCount;

	// Right clicked on an item in the Player Cache
	playerCache->GetSelectedPlayers(selectedPlayers);
	cacheCount=selectedPlayers.GetCount();

	if (cacheCount>1)
	{
		menuItem="Auto Regular Expression";
		popupMenu.Append(PLAYER_POPUP_SELECT,menuItem);
	}
	popupMenu.Append(PLAYER_POPUP_CANCEL,_T("Cancel"));
	PopupMenu(&popupMenu);
}

void ClanEditorPanel::OnPopupMenu(wxCommandEvent &event)
{
	wxString			msg;
	int					id;
	wxArrayInt			selectedPlayers;
	PlayerCacheEntry	cacheEntry;
	int					cacheCount;
	int					cacheIndex;
	int					selectedIndex;
	int					playerIndex;
	wxString			longestMatch;
	wxString			decolouredName;
	int					length;
	int					charIndex;

	id=event.GetId();
	msg.Printf("ClanEditorPanel::OnPopupMenu():Event Received %d",id);
	playerCache->GetSelectedPlayers(selectedPlayers);
	cacheCount=selectedPlayers.GetCount();
	switch (event.GetId())
	{
		case PLAYER_POPUP_SELECT:
			if (cacheCount>1)
			{
				playerIndex=selectedPlayers.Item(0);
				cacheEntry=globalStatistics.playerCache.Item(playerIndex);
				longestMatch=DecolouriseName(cacheEntry.name);
				for (cacheIndex=1;cacheIndex<cacheCount;cacheIndex++)
				{
					playerIndex=selectedPlayers.Item(cacheIndex);
					cacheEntry=globalStatistics.playerCache.Item(playerIndex);
					decolouredName=DecolouriseName(cacheEntry.name);
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

					if (longestMatch.Length()>0)
					{
						// found a common piece of string
						// convert it to a regular expression
						wxString	regularExpression="^";
						length=longestMatch.Length();
						for (charIndex=0;charIndex<length;charIndex++)
						{
							regularExpression+="\\";
							regularExpression+=longestMatch[charIndex];
						}
						regularExpression+=".*";
						playerCache->SetFilter(regularExpression);
					}
					else
					{
						wxMessageBox(_T("Could not find a common match in names"));
					}
				}
			}
			break;
	}
}
