// wxWindows includes
#include <wx/wx.h>
#include <wx/sizer.h>

// Statsgen includes
#include "BoxedDropDown.h"
#include "WindowIDs.h"
#include "ErrorData.h"

BoxedDropDown::BoxedDropDown()
{
	STATSGEN_DEBUG_FUNCTION_START("BoxedDropDown","BoxedDropDown")
	allEntriesAllowed=true;
	STATSGEN_DEBUG_FUNCTION_END
}

void BoxedDropDown::AllEntriesAllowed(bool allEntries)
{
	allEntriesAllowed=allEntries;
}

BoxedDropDown::~BoxedDropDown()
{
	STATSGEN_DEBUG_FUNCTION_START("BoxedDropDown","~BoxedDropDown")
	STATSGEN_DEBUG_FUNCTION_END
}

bool BoxedDropDown::CreateDialog(wxWindow *parent,
					wxWindowID id,
					const wxPoint &pos,
					const wxSize &size,
					long style,
					const wxString &name)
{
	bool			retVal=true;
	wxString		label;
	wxArrayString	dropDownEntries;
	wxSize			itemSize;
	wxPoint			itemPosition;
	wxString		msg;
	int				dropDownWidth;
	int				dropDownHeight;
	int				boxWidth;
	int				boxHeight;
	wxString		defaultSelection="??";
	wxString		dialogName="BoxedDropDown";

	STATSGEN_DEBUG_FUNCTION_START("BoxedDropDown","CreateDialog")
	label=GetLabel();
	DropDownEntries(dropDownEntries,defaultSelection);
	if (allEntriesAllowed)
	{
		dropDownEntries.Insert(AllName(),0);
		defaultSelection=AllName();
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Create")
	Create(parent,id,pos,size,style,name);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"staticBox.Create")
	staticBox.Create(this,-1,label);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"dropDown.Create")
	//dropDown=new wxComboBox(&staticBox,
	dropDown=new wxComboBox(this,
						id,
						defaultSelection,
						wxDefaultPosition,
						wxDefaultSize,
						dropDownEntries,
						wxCB_DROPDOWN |
						wxCB_READONLY,
						wxDefaultValidator,
						dialogName);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"dropDown.SetValue")
	dropDown->SetValue(defaultSelection);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"dropDown.GetSize")
	itemSize=dropDown->GetSize();
	dropDownHeight=itemSize.GetHeight();
	dropDownWidth=itemSize.GetWidth();

	boxWidth=dropDownWidth+STATIC_BOX_LEFT_GAP+STATIC_BOX_RIGHT_GAP;
	boxHeight=dropDownHeight+STATIC_BOX_TOP_GAP+STATIC_BOX_BOTTOM_GAP;

	itemPosition.x=0;
	itemPosition.y=0;
	STATSGEN_DEBUG(DEBUG_ALWAYS,"staticBox.set size")
	staticBox.SetPosition(itemPosition);
	staticBox.SetSize(wxSize(boxWidth,boxHeight));

	itemPosition.x=STATIC_BOX_LEFT_GAP;
	itemPosition.y=STATIC_BOX_TOP_GAP;
	STATSGEN_DEBUG(DEBUG_ALWAYS,"dropDown.SetPosition")
	dropDown->SetPosition(itemPosition);

	STATSGEN_DEBUG(DEBUG_ALWAYS,"SetSize")
	SetSize(wxSize(boxWidth+00,boxHeight+00));
						
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}


wxString BoxedDropDown::GetSelectedCode()
{
	wxString	name;
	wxString	code;

	name=dropDown->GetValue();

	code=FindCodeFromName(name);

	return (code);
}

