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
		virtual void SetLabelWidth(int width)=0;
		virtual int GetLabelWidth()=0;
		void SetTriggerCondition(GUITriggerList &listIn);
		bool UpdateFromTrigger();
		int PreferredHeight();
	protected:
		wxString	configKey;
		GUITriggerList	triggerList;
};

#endif
