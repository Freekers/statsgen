// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "Cheater.h"
#include "GlobalStatistics.h"

Cheaters::Cheaters()
{
	names.Clear();
}

Cheaters::~Cheaters()
{
}

void Cheaters::CreateDatabase()
{
	wxString	SQL;

	SQL=SQLCreateTable();
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

}

wxString Cheaters::SQLTableName()
{
	wxString	tableName;

	tableName="cheaters";

	return (tableName);
}

wxString Cheaters::SQLCreateTable()
{
	wxString	SQL;

	SQL.Printf("create table %s"
			"("
				"name string"
			")",
			STRING_TO_CHAR(SQLTableName()));

	return SQL;
}

bool Cheaters::WriteToDatabase()
{
	wxString	SQL;
	bool		retVal=true;
	int			nameCount;
	int			nameIndex;
	wxString	name;

	nameCount=names.GetCount();
	for (nameIndex=0;nameIndex<nameCount;nameIndex++)
	{
		name=names.Item(nameIndex);
		SQL.Printf("delete from %s where name=\'%s\'", 
				STRING_TO_CHAR(SQLTableName()),
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(name)));
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
		SQL.Printf("insert into %s"
				"(name)"
				"values"
				"('%s')",
				STRING_TO_CHAR(SQLTableName()),
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(name)));
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	}

	return retVal;
}
void Cheaters::Add(wxString &name)
{
	int		searchIndex;

	searchIndex=names.Index(name);
	if (searchIndex==wxNOT_FOUND)
	{
		names.Add(name);
	}
}

void Cheaters::Clear()
{
	names.Clear();
}
