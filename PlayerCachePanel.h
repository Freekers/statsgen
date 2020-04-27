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
		void CreateScreen(wxWindow *parent,int id);
		void OnRightClick(wxListEvent &event);
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
					wxIntPtr item1,
					wxIntPtr item2,
					wxIntPtr columntosort);

		void OnSize(wxSizeEvent &event);

	protected:
		wxListCtrl			*mCacheDisplay;
		TextConfigItemGUI	*mFilter;
		FilterTypes			mFilterType;
		bool  (*mFilterNameCallBack)(wxString &name);
		wxBoxSizer			*mMainSizer;


	private:
		int			mColumnToSort;
		wxString	mConfigKey;
		wxString	mLabelText;
		DECLARE_EVENT_TABLE()
};

#endif
