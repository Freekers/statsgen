#ifndef __CHEATER
#define __CHEATER

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>

// Statsgen Includes

class Cheaters
{
	public:
		Cheaters();
		virtual ~Cheaters();

		bool WriteToDatabase();
		void CreateDatabase();
		void Clear();
		static wxString SQLTableName();
		static wxString SQLCreateTable();
		void Add(wxString &name);
		wxArrayString	names;
		
};


#endif
