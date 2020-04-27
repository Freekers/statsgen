// wxWindows includes
#include <wx/wx.h>

// Statsgen includes
#include "BaseConfigItemGUI.h"
#include "WindowIDs.h"
#include "GlobalStatistics.h"

BaseConfigItemGUI::BaseConfigItemGUI()
{
	mConfigKey		= "";
	mDefaultValue	= "";
	mMainSizer		= NULL;
}

void BaseConfigItemGUI::SetConfigKey(wxString configKey,char *defaultValue)
{
	wxString	defaultValueStr = defaultValue;
	SetConfigKey(configKey,defaultValueStr);
}
void BaseConfigItemGUI::SetConfigKey(wxString configKey,wxString defaultValue)
{
	mConfigKey		= configKey;
	mDefaultValue	= defaultValue;
}
void BaseConfigItemGUI::SetConfigKey(wxString configKey)
{
	mConfigKey		= configKey;
}

void BaseConfigItemGUI::SetTriggerCondition(GUITriggerList &listIn)
{
	mTriggerList=listIn;
}

bool BaseConfigItemGUI::UpdateFromTrigger()
{
	bool	retVal;

	retVal=mTriggerList.PerformTrigger(this);
	return (retVal);
}

void BaseConfigItemGUI::ConfigureSizer()
{
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
}
