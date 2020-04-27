#ifndef __GUITRIGGERLIST
#define __GUITRIGGERLIST

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>


// Statsgen Includes
#include "GUITriggerItem.h"

class GUITriggerList
{
	public:
		void Add(GUITriggerItem &triggerItem);
		bool PerformTrigger(wxWindow *window);
	public:
		ArrayOfGUITriggerItem	triggerList;
		
};


#endif
