#ifndef __TEXTLISTEDITORPANEL
#define __TEXTLISTEDITORPANEL

#include <wx/wx.h>
#include <wx/listctrl.h>

// Statsgen Includes
#include "GroupedConfigItemsPanel.h"
#include "WindowIDs.h"

class TextListEditorPanel : public wxPanel
{
	public:
		TextListEditorPanel();
		void SetList(wxArrayString &textListIn);
		virtual bool Create(wxWindow *parent,
				wxWindowID id=wxID_ANY,
				const wxPoint &pos=wxDefaultPosition,
				const wxSize &size=wxDefaultSize,
				long style=wxTAB_TRAVERSAL,
				const wxString &name="panel");
		void OnTextListRightClick(wxListEvent &event);
		void OnLabelEdit(wxListEvent &event);
		void OnPopupMenu(wxCommandEvent &event);
		void RefreshTextListTree();
		void GetList(wxArrayString &textList);
		void Add(wxString &entry);
		void SortOn(bool value);

	protected:


	private:
		void AddTextListEntry(int index,wxString &listEntry);
		enum
		{
			TEXTLIST_POPUP_CANCEL=1000,
			TEXTLIST_POPUP_DELETE
		};

		wxListCtrl		*mTextListList;
		wxArrayString	mTextList;
		bool			mSortList;
		wxBoxSizer		*mMainSizer;
		DECLARE_EVENT_TABLE()
};

#endif
