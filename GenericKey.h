#ifndef __GENERICKEY
#define __GENERICKEY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes

class GenericKey;

WX_DECLARE_OBJARRAY(GenericKey,ArrayOfGenericKey);
class GenericKey
{
	public:
		GenericKey();
		virtual ~GenericKey();
		wxString id;
		static wxString SQLCreateTable(char *tableName);
		bool WriteToDatabase(char *tableName,int keyIndex);

};


#endif
