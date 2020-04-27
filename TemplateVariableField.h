#ifndef __TEMPLATEVARIABLEFIELD
#define __TEMPLATEVARIABLEFIELD

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes
class TemplateVariableField;

WX_DECLARE_OBJARRAY(TemplateVariableField,ArrayOfTemplateVariableField);

class TemplateVariableField
{
	public:
		TemplateVariableField();
		virtual ~TemplateVariableField();

	public:
		wxString	variable;
		wxString	value;
		
};


#endif
