#ifndef __BASECONFIGITEMGUI
#define __BASECONFIGITEMGUI

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "GUITriggerList.h"

class BaseConfigItemGUI : public wxPanel
{
	public:
		BaseConfigItemGUI();
		void SetTriggerCondition(GUITriggerList &listIn);
		bool UpdateFromTrigger();
		void SetConfigKey(wxString configKey,char *defaultValue);
		void SetConfigKey(wxString configKey,wxString defaultValue);
		void SetConfigKey(wxString configKey);
		virtual void ApplyConfigKeyChange()=0;
	protected:
		void ConfigureSizer();
		wxString		mConfigKey;
		wxString		mDefaultValue;
		GUITriggerList	mTriggerList;
		wxSizer			*mMainSizer;
};

#endif
