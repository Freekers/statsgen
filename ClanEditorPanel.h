#ifndef __CLANEDITORPANEL
#define __CLANEDITORPANEL

#include <wx/wx.h>
#include <wx/treectrl.h>

// Statsgen Includes
#include "PlayerCachePanel.h"
#include "PlayerCacheEntry.h"
#include "GenericConfigPanel.h"
#include "WindowIDs.h"

class ClanEditorPanel : public GenericConfigPanel
{
	public:
		ClanEditorPanel(wxString &configGroup,
						wxWindow *parent,
						wxWindowID id=-1,
						const wxPoint &pos=wxDefaultPosition,
						const wxSize &size=wxDefaultSize,
						long style=wxTAB_TRAVERSAL,
						const wxString &name="panel");
		void OnPlayerRightClick(wxListEvent &event);
		void OnPopupMenu(wxCommandEvent &event);

	protected:


	private:
		enum
		{
			PLAYER_POPUP_SELECT=1000,
			PLAYER_POPUP_CANCEL
		};
		PlayerCachePanel	*playerCache;
		wxString			configGroup;
		DECLARE_EVENT_TABLE()
};

#endif
