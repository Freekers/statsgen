#ifndef __TEMPLATEOPENQUERY
#define __TEMPLATEOPENQUERY

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes
#include "sqlite3.h"
//#include "libsqlitewrapped.h"

class TemplateOpenQuery;

WX_DECLARE_OBJARRAY(TemplateOpenQuery,ArrayOfTemplateOpenQuery);

class TemplateOpenQuery
{
	public:
		TemplateOpenQuery();
		virtual	 ~TemplateOpenQuery();
		bool	NextRow();
		bool	Initiate(wxString &SQLIn,
						sqlite3 *dbHandleIn);
		wxString RetrieveProperty(wxString &property);

		void Clone(TemplateOpenQuery *orgQuery);
		wxString GetColumnName(int index);
		wxString GetColumnValue(int index);
		int ColumnCount();
	public:
		wxString			variable;

	private:
		sqlite3_stmt		*query;
		sqlite3				*dbHandle;
		wxArrayString		columnNames;
		wxString			SQL;
		wxArrayString		columnValues;
		
};


#endif
