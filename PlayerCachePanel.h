#ifndef __PLAYERCACHEPANEL
#define __PLAYERCACHEPANEL

#include <wx/wx.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "TextConfigItemGUI.h"
#include "WindowIDs.h"

class PlayerCachePanel : public wxPanel
{
	public:
		enum FilterTypes
		{
			FILTER_TYPE_NO_CASE=0,
			FILTER_TYPE_CASE,
			FILTER_TYPE_REGEXP
		};
		PlayerCachePanel(wxString &configKeyIn,
				wxString &labelTextIn);
		void CreateScreen();
		void OnRightClick(wxListEvent &event);
		void OnResize(wxSizeEvent &event);
		void OnColumnClick(wxListEvent &event);
		void ApplyFilter();
		void UpdateCacheList();
		void GetShownPlayers(wxArrayInt &players);
		void GetSelectedPlayers(wxArrayInt &players);
		void OnFilterChanged(wxCommandEvent &event);
		void SetFilterType(FilterTypes filterTypeIn);
		void SetFilter(wxString &filterData);
		void SetFilterCallBack(bool (*filterNameCallBack)(wxString &name));
		static int wxCALLBACK CompareItems(
					long item1,
					long item2,
					long columntosort);


	protected:
		wxListCtrl			*cacheDisplay;
		TextConfigItemGUI	*filter;
		FilterTypes			filterType;
		bool  (*filterNameCallBack)(wxString &name);


	private:
		int columnToSort;
		wxString	configKey;
		wxString	labelText;
		DECLARE_EVENT_TABLE()
};

#endif
