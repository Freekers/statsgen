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
	mAllEntriesAllowed=true;
	STATSGEN_DEBUG_FUNCTION_END
}

void BoxedDropDown::AllEntriesAllowed(bool allEntries)
{
	mAllEntriesAllowed=allEntries;
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
	wxString		msg;
	wxString		defaultSelection="??";
	wxString		dialogName="BoxedDropDown";

	STATSGEN_DEBUG_FUNCTION_START("BoxedDropDown","CreateDialog")
	label=GetLabel();
	DropDownEntries(dropDownEntries,defaultSelection);
	if (mAllEntriesAllowed)
	{
		dropDownEntries.Insert(AllName(),0);
		defaultSelection=AllName();
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Create")
	Create(parent,id,pos,size,style,name);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"staticBox.Create")
	mTitleBox = new wxStaticBox(this,wxID_ANY,label);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"dropDown.Create")
	//dropDown=new wxComboBox(&staticBox,
	mDropDown=new wxComboBox(this,
						id,
						defaultSelection,
						wxDefaultPosition,
						wxDefaultSize,
						dropDownEntries,
						wxCB_DROPDOWN |
						wxCB_READONLY,
						wxDefaultValidator,
						dialogName);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"dropDown.SetValue")
	mDropDown->SetValue(defaultSelection);

	mTitleSizer		= new wxStaticBoxSizer(mTitleBox,wxVERTICAL);
	mContentsSizer	= new wxBoxSizer(wxVERTICAL);
	mContentsSizer->Add(mDropDown,1,wxEXPAND);
	mTitleSizer->Add(mContentsSizer,1,wxEXPAND);
	mTitleSizer->SetSizeHints(this);
	SetSizer(mTitleSizer);
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}


wxString BoxedDropDown::GetSelectedCode()
{
	wxString	name;
	wxString	code;

	name=mDropDown->GetValue();

	code=FindCodeFromName(name);

	return (code);
}

