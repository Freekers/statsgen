#ifndef __DROPLISTEDITORPANEL
#define __DROPLISTEDITORPANEL

#include <wx/wx.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "PlayerCachePanel.h"
#include "PlayerCacheEntry.h"
#include "DropListEntry.h"
#include "GroupedConfigItemsPanel.h"
#include "WindowIDs.h"

#define DROPLISTPANEL_PLAYERCHEAT	"Cheat"
#define DROPLISTPANEL_PLAYERDROP	"Dropped"
#define DROPLISTPANEL_DROP			"Drop This Player"
#define DROPLISTPANEL_DONT_DROP			"Don't Drop This Player"
#define DROPLISTPANEL_ALLOW			"Allow Only"
#define DROPLISTPANEL_EXACT			"Exact Name"
#define DROPLISTPANEL_PREFIX		"Name Prefix"
#define DROPLISTPANEL_REGEXP		"Name Regular Expression"
#define DROPLISTPANEL_COLOURED		"Colour Codes Present"
#define DROPLISTPANEL_NONCOLOURED	"Colour Codes Removed"
class DropListEditorPanel : public wxPanel
{
	public:
		DropListEditorPanel();
		virtual bool Create(wxWindow *parent,
				wxWindowID id=-1,
				const wxPoint &pos=wxDefaultPosition,
				const wxSize &size=wxDefaultSize,
				long style=wxTAB_TRAVERSAL,
				const wxString &name="panel");
		void OnResize(wxSizeEvent &event);
		void OnDropListRightClick(wxListEvent &event);
		void OnLabelEdit(wxListEvent &event);
		void OnPlayerRightClick(wxListEvent &event);
		void OnPopupMenu(wxCommandEvent &event);
		void RefreshDropListTree();
		void OnTextChange(wxCommandEvent &event);

	protected:


	private:
		void AddDropListEntry(int index,DropListEntry &listEntry);
		enum
		{
			DROPLIST_POPUP_CANCEL=1000,
			DROPLIST_POPUP_DELETE,
			DROPLIST_POPUP_ALLOW,
			DROPLIST_POPUP_DROP,
			DROPLIST_POPUP_DONT_DROP,
			DROPLIST_POPUP_CHEAT,
			DROPLIST_POPUP_DONTUSECOLOURCODES,
			DROPLIST_POPUP_USECOLOURCODES,
			DROPLIST_POPUP_USEEXACTMATCH,
			DROPLIST_POPUP_USEPREFIXMATCH,
			DROPLIST_POPUP_USEREGEXPMATCH,
			PLAYER_POPUP_ADD_ALLOW,
			PLAYER_POPUP_ADD_DROP,
			PLAYER_POPUP_ADD_CHEAT
		};

		PlayerCachePanel		*playerCache;
		wxListCtrl				*dropListList;
		GroupedConfigItemsPanel	*configItems;
		wxString				filterString;
		wxBoxSizer				*mMainSizer;
		DECLARE_EVENT_TABLE()
};

#endif
