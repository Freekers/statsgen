#ifndef __SCHEDULECONFIGITEMGUI
#define __SCHEDULECONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "BaseConfigItemGUI.h"

class ScheduleConfigItemGUI : public BaseConfigItemGUI
{
	public:
		ScheduleConfigItemGUI();
		virtual ~ScheduleConfigItemGUI();
		void CreateDisplay(wxWindow *parent,int id,wxString &labelText);
		void OnTextChange(wxCommandEvent& event);
		void OnButtonPressed(wxCommandEvent& event);
		virtual void ApplyConfigKeyChange();
		void SetConfigKey(wxString configKey,wxString defaultValue,bool intervalWithBase);
		wxString GetValue();

	protected:

	private:
		wxTextCtrl		*mTextEdit;
		wxBitmapButton	*mButton;
		wxStaticText	*mLabel;
		wxString		mLabelText;
		bool			mIntervalWithBase;

		DECLARE_EVENT_TABLE()
};

#endif
