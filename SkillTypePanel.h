#ifndef __SKILLTYPEPANEL
#define __SKILLTYPEPANEL

// wxWindows includes
#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/combobox.h>

// Statsgen Includes
#include "BoxedDropDown.h"

class SkillTypePanel : public BoxedDropDown
{
	protected:
		virtual wxString GetLabel();
		virtual void DropDownEntries(wxArrayString &entries,
						wxString &defaultSelection);
		virtual wxString AllCode(); 
		virtual wxString AllName(); 
		virtual wxString FindCodeFromName(wxString &name);
	private:
};


#endif
