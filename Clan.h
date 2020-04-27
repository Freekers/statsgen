#ifndef __CLANDEFINITION
#define __CLANDEFINITION

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/regex.h>

// Statsgen Includes

class Clan;

WX_DECLARE_OBJARRAY(Clan,ArrayOfClan);
class Clan
{
	public:
		void ReadConfig();
		void WriteConfig();
		Clan(wxString &idIn);
		virtual ~Clan();
		static wxString SQLCreateTable();
		static wxString SQLTableName();
		bool WriteToDatabase(int itemIndex);
		bool IsMember(wxString &name);

		wxString	tag;
		wxString	name;
		wxString	webAddress;
		wxString	banner;
		wxString	regExpString;
		wxRegEx		*regExp;

		wxString	id;
		
};


#endif
