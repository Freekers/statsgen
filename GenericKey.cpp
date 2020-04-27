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
			StatsgenDatabase::StringFieldDefinition("id","generickeyid",FIELD_WIDTH_KEY_ID).GetData(),
			StatsgenDatabase::StringFieldDefinition("realname","generickeyname",FIELD_WIDTH_KEY_REALNAME).GetData(),
			StatsgenDatabase::StringFieldDefinition("image","generickeyimage",FIELD_WIDTH_KEY_IMAGE).GetData()
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
		configKey.Printf("/Images/%s_%s",tableName,id.GetData());
		globalStatistics.configData.ReadTextValue(configKey,&image,"");
		configKey.Printf("/RealNames/%s_%s",tableName,id.GetData());
		globalStatistics.configData.ReadTextValue(configKey,&realName,(char *)id.GetData());
	}

	SQL.Printf("insert into %s"
				"(idx,id,realname,image)"
				"values"
				"('%d','%s','%s','%s')",
				tableName,
				keyIndex,
				StatsgenDatabase::SafeForInsert(id).GetData(),
				StatsgenDatabase::SafeForInsert(realName).GetData(),
				StatsgenDatabase::SafeForInsert(image).GetData());
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);

	return retVal;
}

