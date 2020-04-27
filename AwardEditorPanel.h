#ifndef __AWARDEDITORPANEL
#define __AWARDEDITORPANEL

#include <wx/wx.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GroupedConfigItemsPanel.h"
#include "AwardDefinition.h"

class AwardDefinitionPanel : public wxPanel
{
	public:
		AwardDefinitionPanel(wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxString &awardIDIn);
		virtual ~AwardDefinitionPanel();

		void AddWeaponComponent(wxString &code,bool kill,bool positive,bool teamkill);
		void AddLocationComponent(wxString &code,bool kill,bool positive,bool teamkill);
		void AddActionComponent(wxString &code,bool positive);
		void AddMiscComponent(wxString &code,bool positive);
		void AddXPComponent(wxString &code,bool positive);

		void RemoveWeaponComponent(wxString &code,bool killList,bool teamkillList);
		void RemoveLocationComponent(wxString &code,bool killList,bool teamkillList);
		void RemoveActionComponent(wxString &code);
		void RemoveMiscComponent(wxString &code);
		void RemoveXPComponent(wxString &code);

		void OnRightClick(wxTreeEvent& event);
		void PopulateList(wxTreeCtrl *listBox,
						wxArrayString &codeList,
						const char *realnamePrefix,
						const char *suffix);

		wxArrayString				mSelectedCodes;
		long						mSelectedDefinitionTree;
		AwardDefinition				*mAwardDefinition;
		wxTreeCtrl					*mWeaponList;
		wxTreeCtrl					*mLocationList;
		wxTreeCtrl					*mActionList;
		wxTreeCtrl					*mMiscList;
		wxTreeCtrl					*mXPList;
		wxStaticText				*mLabel;
		void SetFilter(wxString &filterString);

	protected:

	private:
		wxString			mAwardID;
		wxString			mFilter;
		wxBoxSizer			*mMainSizer;

		DECLARE_EVENT_TABLE()
};

class AwardChoicesPanel : public wxPanel
{
	public:
		AwardChoicesPanel(wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxString &awardIDIn);
		virtual ~AwardChoicesPanel();

		void OnRightClick(wxTreeEvent& event);

		wxArrayString	mSelectedCodes;
		long			mSelectedChoiceTree;
		void SetFilter(wxString &filterString);

	protected:

	private:
		void PopulateList(wxTreeCtrl *listBox,
						const char *group,
						const char *realnamePrefix);

		wxString			mAwardID;
		wxTreeCtrl			*mWeaponList;
		wxTreeCtrl			*mLocationList;
		wxTreeCtrl			*mActionList;
		wxTreeCtrl			*mMiscList;
		wxTreeCtrl			*mXPList;
		wxStaticText		*mLabel;
		wxString			mFilter;
		wxBoxSizer			*mMainSizer;

		DECLARE_EVENT_TABLE()
};

class AwardEditorPanel : public wxPanel
{
	public:
		AwardEditorPanel(wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxString &awardIDIn);
		virtual ~AwardEditorPanel();

		void OnRightClick(wxTreeEvent& event);
		void OnPopupMenu(wxCommandEvent& event);
		void OnConfigChanged(wxCommandEvent &event);

	protected:

	private:
		wxString					mAwardID;
		GroupedConfigItemsPanel		*mOverallConfig;
		AwardChoicesPanel			*mAwardChoices;
		AwardDefinitionPanel		*mAwardDefinition;

		wxSplitterWindow			*mSplitter;
		wxBoxSizer					*mMainSizer;
		DECLARE_EVENT_TABLE()
};

class StringTreeItemData : public wxTreeItemData
{
public:
    StringTreeItemData(const wxString& desc) : m_desc(desc) { }

    wxString GetString() const { return m_desc; }

private:
    wxString m_desc;
};

#endif
