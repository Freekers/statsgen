// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "GenericKey.h"
#include "GlobalStatistics.h"

GenericKey::GenericKey()
{
}

GenericKey::~GenericKey()
{
}

wxString GenericKey::SQLCreateTable(char *tableName)
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"idx integer,"
				"%s,"
				"%s,"
				"%s"
			")",
			tableName,
			STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("id","generickeyid",FIELD_WIDTH_KEY_ID)),
			STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("realname","generickeyname",FIELD_WIDTH_KEY_REALNAME)),
			STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("image","generickeyimage",FIELD_WIDTH_KEY_IMAGE))
			);

	return SQL;
}


bool GenericKey::WriteToDatabase(char *tableName,int keyIndex)
{
	wxString	SQL;
	bool		retVal=true;
	wxString	image;
	wxString	realName="";
	wxString	configKey="";

	if (id.Length()>0)
	{
		configKey.Printf("/Images/%s_%s",tableName,STRING_TO_CHAR(id));
		globalStatistics.configData.ReadTextValue(configKey,&image,(char *)"");
		configKey.Printf("/RealNames/%s_%s",tableName,STRING_TO_CHAR(id));
		globalStatistics.configData.ReadTextValue(configKey,&realName,id);
	}

	SQL.Printf("insert into %s"
				"(idx,id,realname,image)"
				"values"
				"('%d','%s','%s','%s')",
				tableName,
				keyIndex,
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(id)),
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(realName)),
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(image)));
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

	return retVal;
}

