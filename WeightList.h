#ifndef __WEIGHTLIST
#define __WEIGHTLIST

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/textfile.h>

// Statsgen Includes
#include "WeightListEntry.h"

class WeightList
{
	public:
		WeightList();
		void Initiate(const char *prefixIn);
		~WeightList();
		void Clear();

		float Weight(wxString &key,float defaultValue);
		void CreateDatabase();
		void WriteToDatabase();
		void ReadFromDatabase();
		void EmptyDatabase();

		ArrayOfWeightListEntry	weightList;
		wxString	prefix;
};


#endif
