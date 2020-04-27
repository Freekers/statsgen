// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "WeightListEntry.h"
#include "GlobalStatistics.h"

WeightListEntry::WeightListEntry()
{
}

WeightListEntry::~WeightListEntry()
{
}

void WeightListEntry::UpdateWeighting(wxString &prefix)
{
	wxString	configKey;
	wxString	configValue;
	wxString	defaultString;

	configKey="/"+prefix+"/"+key;
	defaultString.Printf("%f",defaultValue);
	globalStatistics.configData.ReadTextValue(configKey,
						&configValue,
						defaultString);

	weight=atof(STRING_TO_CHAR(configValue));
}

