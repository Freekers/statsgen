#ifndef __ALIASLISTENTRY
#define __ALIASLISTENTRY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/textfile.h>

// Statsgen Includes

class AliasListEntry;

WX_DECLARE_OBJARRAY(AliasListEntry,ArrayOfAliasListEntry);
class AliasListEntry
{
	public:
		AliasListEntry();
		~AliasListEntry();
		void WriteToFile(FILE *fp);
		void ReadFromFile(wxTextFile *fp,int index);
		int UpdateInDatabase();
		wxString	primaryGUID;
		wxString	primaryName;
		wxString	aliasGUID;
		wxString	aliasName;
};


#endif
