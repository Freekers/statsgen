// wxWindows includes
#include <wx/wx.h>

// Statsgen includes
#include "BaseConfigItemGUI.h"
#include "WindowIDs.h"
#include "GlobalStatistics.h"

void BaseConfigItemGUI::SetTriggerCondition(GUITriggerList &listIn)
{
	triggerList=listIn;
}

bool BaseConfigItemGUI::UpdateFromTrigger()
{
	bool	retVal;

	retVal=triggerList.PerformTrigger(this);
	return (retVal);
}

int BaseConfigItemGUI::PreferredHeight()
{
	return (20);
}
