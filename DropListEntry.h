#ifndef __DROPLISTENTRY
#define __DROPLISTENTRY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/textfile.h>
#include <wx/regex.h>

// Statsgen Includes

class DropListEntry;

WX_DECLARE_OBJARRAY(DropListEntry,ArrayOfDropListEntry);
class DropListEntry
{
	public:
		DropListEntry();
		~DropListEntry();
		bool Matches(wxString &nameIn,bool *cheatIn);
		void WriteToFile(FILE *fp);
		void ReadFromFile(wxTextFile *fp,int index);
		bool		drop;
		bool		cheat;
		bool		coloured;
		bool		prefix;
		bool		regularExpression;
		bool		dontDrop;
		wxString	name;
		wxString	lastCompiledName;
		wxRegEx		*regExp;
		static int Compare(DropListEntry *item1,DropListEntry *item2);

		bool DropType();
};


#endif
