#ifndef __ALIASEDITORPANEL
#define __ALIASEDITORPANEL

#include <wx/wx.h>
#include <wx/treectrl.h>

// Statsgen Includes
#include "PlayerCachePanel.h"
#include "PlayerCacheEntry.h"
#include "AliasListEntry.h"
#include "GroupedConfigItemsPanel.h"
#include "WindowIDs.h"

class AliasEditorPanel : public wxPanel
{
	public:
		AliasEditorPanel();
		virtual bool Create(wxWindow *parent,
				wxWindowID id=-1,
				const wxPoint &pos=wxDefaultPosition,
				const wxSize &size=wxDefaultSize,
				long style=wxTAB_TRAVERSAL,
				const wxString &name="panel");
		void OnResize(wxSizeEvent &event);
		void OnAliasRightClick(wxTreeEvent &event);
		void OnPlayerRightClick(wxListEvent &event);
		void OnPopupMenu(wxCommandEvent &event);
		void RefreshAliasTree();
		void GetShownPlayers(wxArrayInt &shownPlayers);

	protected:


	private:
		enum
		{
			ALIAS_POPUP_CANCEL=0,
			ALIAS_POPUP_DELETE_PARENT,
			ALIAS_POPUP_DELETE_CHILD,
			ALIAS_POPUP_REPARENT,
			PLAYER_POPUP_NEW_ALIAS,
			PLAYER_POPUP_ADD_ALIAS
		};
		void AliasItemSplit(wxTreeItemId &treeItem,
						AliasListEntry &aliasEntry);
		void DeleteAlias(wxTreeItemId &parentItem,
						wxTreeItemId &childItem);
		void AddAlias(AliasListEntry &aliasEntry);
		void AddCacheToParent(wxTreeItemId &parentItem,
							PlayerCacheEntry &cacheEntry);
		void AddAliasTree(wxTreeItemId &parentItem);
static		bool FilterNameCallBack(wxString &name);
		wxTreeItemId AddPlayerToTree(wxTreeItemId &parentItem,
						wxString &guid,
						wxString &name);
		void OnTextChange(wxCommandEvent &event);

		PlayerCachePanel		*playerCache;
		wxTreeCtrl				*aliasList;
		GroupedConfigItemsPanel	*aliasConfigs;
		wxString				aliasFilter;
		DECLARE_EVENT_TABLE()
};

#endif
