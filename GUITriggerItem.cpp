// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "GUITriggerItem.h"
#include "GlobalStatistics.h"

GUITriggerItem::GUITriggerItem()
{
	configKey="";
	matchValue="";
	matchSwitchesOff=true;
	triggerHides=false;
}

GUITriggerItem::~GUITriggerItem()
{
}

void GUITriggerItem::SetPositiveHideTrigger(wxString &key,wxString &value)
{
	triggerHides	=true;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=true;
}
void GUITriggerItem::SetNegativeHideTrigger(wxString &key,wxString &value)
{
	triggerHides	=true;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=false;
}
void GUITriggerItem::SetPositiveDisableTrigger(wxString &key,wxString &value)
{
	triggerHides	=false;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=true;
}
void GUITriggerItem::SetNegativeDisableTrigger(wxString &key,wxString &value)
{
	triggerHides	=false;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=false;
}
void GUITriggerItem::SetPositiveHideTrigger(wxString &key,const char *value)
{
	triggerHides	=true;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=true;
}
void GUITriggerItem::SetNegativeHideTrigger(wxString &key,const char *value)
{
	triggerHides	=true;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=false;
}
void GUITriggerItem::SetPositiveDisableTrigger(wxString &key,const char *value)
{
	triggerHides	=false;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=true;
}
void GUITriggerItem::SetNegativeDisableTrigger(wxString &key,const char *value)
{
	triggerHides	=false;
	configKey	=key;
	matchValue	=value;
	matchSwitchesOff=false;
}

bool GUITriggerItem::PerformTrigger(wxWindow *window,
				bool *currentlyShown,
				bool *currentlyEnabled)
{
	bool		triggered=false;
	wxString	configValue;

	if (configKey.Length()>0)
	{
		globalStatistics.configData.ReadTextValue(configKey,&configValue);
		if (configValue.CmpNoCase(matchValue)==0)
		{
			if (triggerHides)
			{
				if (matchSwitchesOff)
				{
					*currentlyShown=false;
				}
			}
			else
			{
				if (matchSwitchesOff)
				{
					*currentlyEnabled=false;
				}
			}
		}
		else
		{
			if (triggerHides)
			{
				if (!matchSwitchesOff)
				{
					*currentlyShown=false;
				}
			}
			else
			{
				if (!matchSwitchesOff)
				{
					*currentlyEnabled=false;
				}
			}
		}
	}

	return (triggered);
}

