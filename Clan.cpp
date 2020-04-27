// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "Clan.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"

void Clan::ReadConfig()
{
	wxString	configBase;
	wxString	configKey;
	wxString	configValue;
	wxString	baseGroup;

	baseGroup="Clan"+id;

	configBase.Printf("/%s/",baseGroup.GetData());
	configKey=configBase+"TagColoured";
	globalStatistics.configData.ReadTextValue(configKey,&tag,"");
	configKey=configBase+"FullName";
	globalStatistics.configData.ReadTextValue(configKey,&name,"");
	configKey=configBase+"WebAddress";
	globalStatistics.configData.ReadTextValue(configKey,&webAddress,"");
	configKey=configBase+"BannerAddress";
	globalStatistics.configData.ReadTextValue(configKey,&banner,"");
	configKey=configBase+"RegExp";
	globalStatistics.configData.ReadTextValue(configKey,&regExpString,"");
	regExp=new wxRegEx();
	regExp->Compile(regExpString);
}

Clan::Clan(wxString &idIn)
{

	id=idIn;

	ReadConfig();
	

}

Clan::~Clan()
{
	delete (regExp);
}

wxString Clan::SQLTableName()
{
	wxString	tableName="clan";

	return (tableName);
}

wxString Clan::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"clanindex integer,"
				"%s,"
				"%s,"
				"%s,"
				"%s"
			")",
			SQLTableName().GetData(),
		StatsgenDatabase::StringFieldDefinition("tag","clantag",FIELD_WIDTH_CLAN_TAG).GetData(),
		StatsgenDatabase::StringFieldDefinition("name","clanname",FIELD_WIDTH_CLAN_NAME).GetData(),
		StatsgenDatabase::StringFieldDefinition("webaddress","clannweb",FIELD_WIDTH_CLAN_WEBADDRESS).GetData(),
		StatsgenDatabase::StringFieldDefinition("banner","clanbanner",FIELD_WIDTH_CLAN_BANNER).GetData()
);

	return SQL;
}

bool Clan::WriteToDatabase(int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;

	SQL.Printf("Insert into %s"
				"(clanindex,tag,name,webaddress,banner)"
				"values('%d','%s','%s','%s','%s')",
				SQLTableName().GetData(),
				itemIndex,
				tag.GetData(),
				name.GetData(),
				webAddress.GetData(),
				banner.GetData());
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}

void Clan::WriteConfig()
{
	wxString	configBase;
	wxString	configKey;
	wxString	configValue;
	wxString	baseGroup;

	baseGroup="Clan"+id;

}

bool Clan::IsMember(wxString &name)
{
	wxString	decoloured;
	bool		retVal;

	decoloured=DecolouriseName(name);
	retVal=regExp->Matches(decoloured);
	return (retVal);
}

