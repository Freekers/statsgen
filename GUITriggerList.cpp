// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "GUITriggerList.h"
#include "GUITriggerItem.h"

void GUITriggerList::Add(GUITriggerItem &triggerItem)
{
	triggerList.Add(triggerItem);
}

bool GUITriggerList::PerformTrigger(wxWindow *window)
{
	int				itemCount;
	int				itemIndex;
	GUITriggerItem	triggerItem;
	bool			triggered;
	bool			currentlyShown;
	bool			currentlyEnabled;

	currentlyShown=true;
	currentlyEnabled=true;

	triggered=false;
	itemCount=triggerList.GetCount();
	for (itemIndex=0;itemIndex<itemCount;itemIndex++)
	{
		triggerItem=triggerList.Item(itemIndex);
		if (triggerItem.PerformTrigger(window,&currentlyShown,&currentlyEnabled))
		{
			triggered = true;
		}
	}
	if (currentlyEnabled)
	{
		window->Enable();
	}
	else
	{
		window->Disable();
	}

	if (currentlyShown)
	{
		window->Show();
	}
	else
	{
		window->Hide();
	}

	return (triggered);
}

