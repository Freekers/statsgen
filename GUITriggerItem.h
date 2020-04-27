#ifndef __GUITRIGGERITEM
#define __GUITRIGGERITEM

// wxWindows includes
#include <wx/wx.h>
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class GUITriggerItem;

WX_DECLARE_OBJARRAY(GUITriggerItem,ArrayOfGUITriggerItem);
class GUITriggerItem
{
	public:
		wxString	configKey;
		wxString	matchValue;
		bool		matchSwitchesOff;
		bool		triggerHides;
	public:
		GUITriggerItem();
		~GUITriggerItem();
		bool		PerformTrigger(wxWindow *panel,
							bool *currentlyShown,
							bool *currentlyEnabled);
		void		SetPositiveDisableTrigger(wxString &key,const char *value);
		void		SetPositiveDisableTrigger(wxString &key,wxString &value);
		void		SetNegativeDisableTrigger(wxString &key,const char *value);
		void		SetNegativeDisableTrigger(wxString &key,wxString &value);
		void		SetPositiveHideTrigger(wxString &key,const char *value);
		void		SetPositiveHideTrigger(wxString &key,wxString &value);
		void		SetNegativeHideTrigger(wxString &key,const char *value);
		void		SetNegativeHideTrigger(wxString &key,wxString &value);
		
};


#endif
