#ifndef __PLAYERDATAEDITORPANEL
#define __PLAYERDATAEDITORPANEL

#include <wx/wx.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "PlayerCachePanel.h"
#include "PlayerCacheEntry.h"
#include "PlayerDataEntry.h"
#include "PlayerDataList.h"
#include "GroupedConfigItemsPanel.h"
#include "WindowIDs.h"

class PlayerDataEditorPanel : public wxPanel
{
	public:
		PlayerDataEditorPanel(PlayerDataList *playerDataListIn,
							wxString &defaultPlayerDataIn,
							wxString &playerDataTitleIn);
		virtual bool CreateDisplay(wxWindow *parent, wxWindowID id=wxID_ANY);
		void OnPlayerDataListRightClick(wxListEvent &event);
		void OnLabelEdit(wxListEvent &event);
		void OnPlayerRightClick(wxListEvent &event);
		void OnPopupMenu(wxCommandEvent &event);
		void RefreshPlayerDataListTree();

	protected:


	private:
		void AddPlayerDataEntry(int index,PlayerDataEntry &listEntry);
		enum
		{
			PLAYER_DATA_CANCEL=1000,
			PLAYER_DATA_ADD,
			PLAYER_DATA_DELETE
		};

		PlayerCachePanel		*mPlayerCache;
		wxListCtrl				*mPlayerDataListCtrl;
		PlayerDataList			*mPlayerDataList;
		wxString				mDefaultPlayerData;
		wxString				mPlayerDataTitle;
		GroupedConfigItemsPanel	*mConfigItems;

		wxBoxSizer				*mMainSizer;
		DECLARE_EVENT_TABLE()
};

#endif
