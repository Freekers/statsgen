#ifndef __WEIGHTLISTENTRY
#define __WEIGHTLISTENTRY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/textfile.h>

// Statsgen Includes

class WeightListEntry;

WX_DECLARE_OBJARRAY(WeightListEntry,ArrayOfWeightListEntry);
class WeightListEntry
{
	public:
		WeightListEntry();
		~WeightListEntry();
		void UpdateWeighting(wxString &prefix);
		wxString	key;
		float		weight;
		float		defaultValue;
};


#endif
