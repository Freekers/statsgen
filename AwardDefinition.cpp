// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "AwardDefinition.h"
#include "GlobalStatistics.h"

void AwardDefinition::ReadConfig()
{
	wxString	configBase;
	wxString	configKey;
	wxString	configValue;
	wxString	baseGroup;

	baseGroup="AWARDDEFINITION"+id;

	globalStatistics.configData.ReadList(baseGroup,(char *)"WEAPONKILLCODES",weaponKillComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"WEAPONDEATHCODES",weaponDeathComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"WEAPONTKCODES",weaponTKComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"LOCATIONKILLCODES",locationKillComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"LOCATIONDEATHCODES",locationDeathComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"LOCATIONTKCODES",locationTKComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"ACTIONCODES",actionComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"MISCCODES",miscComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"XPCODES",xpComponents);
	globalStatistics.configData.ReadList(baseGroup,(char *)"GAMETYPECODES",allowedGameTypes);

	configBase.Printf("/%s/",STRING_TO_CHAR(baseGroup));
	configKey=configBase+"Name";
	globalStatistics.configData.ReadTextValue(configKey,&name,(char *)"");
	configKey=configBase+"Image";
	globalStatistics.configData.ReadTextValue(configKey,&image,(char *)"");
	configKey=configBase+"Weighted";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"N");
	weighted=(configValue.CmpNoCase("Y")==0);
}

AwardDefinition::AwardDefinition(wxString &idIn)
{

	id=idIn;

	ReadConfig();
	

}

AwardDefinition::~AwardDefinition()
{
}

wxString AwardDefinition::SQLTableNameComponent(const char *componentType)
{
	wxString	tableName="awarddefinition";

	tableName+=componentType;

	return (tableName);
}

wxString AwardDefinition::SQLTableName()
{
	wxString	tableName="awarddefinition";

	return (tableName);
}

wxString AwardDefinition::SQLCreateTableComponent(const char *component)
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"awardindex integer,"
				"componentindex integer,"
				"%s,"
				"posneg varchar(1),"
				"%s,"
				"%s"
			")",
			STRING_TO_CHAR(SQLTableNameComponent(component)),
		STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("id","awardid",FIELD_WIDTH_KEY_ID)),
		STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("name","awardname",FIELD_WIDTH_KEY_REALNAME)),
		STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("image","awardimage",FIELD_WIDTH_KEY_IMAGE))
		);

	return SQL;
}

wxString AwardDefinition::SQLCreateTable()
{
	wxString SQL;

	SQL.Printf("create table %s"
			"("
				"awardindex integer,"
				"%s,"
				"%s,"
				"%s,"
				"weighted varchar(1)"
			")",
			STRING_TO_CHAR(SQLTableName()),
		STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("id","awardcomponentid",FIELD_WIDTH_AWARD_ID)),
		STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("name","awardcomponentname",FIELD_WIDTH_AWARD_NAME)),
		STRING_TO_CHAR(StatsgenDatabase::StringFieldDefinition("image","awardcomponentimage",FIELD_WIDTH_AWARD_IMAGE))
		);

	return SQL;
}

bool AwardDefinition::WriteToDatabaseComponent(int itemIndex,
						const char *component,
						wxArrayString &componentList,
						const char *idPrefix)
{
	wxString		SQL;
	bool			retVal=true;
	int				componentCount;
	int				componentIndex;
	wxString		componentRealName;
	wxString		componentImage;
	wxString		configKey;
	wxString		thisID;
	wxString		componentID;
	char			posneg;
	wxArrayString	collectedIDs;
	int				idCount;
	int				idIndex;

	componentCount=componentList.GetCount();
	for (componentIndex=0;componentIndex<componentCount;componentIndex++)
	{
		collectedIDs.Clear();
		thisID=componentList.Item(componentIndex).Mid(1);
		posneg=componentList.Item(componentIndex)[0];

		if ((strlen(idPrefix)>0)&&
			(globalStatistics.configData.IsWeaponGroupKey(thisID)))
		{
			collectedIDs=globalStatistics.configData.WeaponGroupKeys(thisID);
		}
		else
		{
			collectedIDs.Add(thisID);
		}
		idCount=collectedIDs.GetCount();
		for (idIndex=0;idIndex<idCount;idIndex++)
		{
			thisID=collectedIDs.Item(idIndex);
			componentImage="";
			componentRealName="";
			if (strlen(idPrefix)>0)
			{
				componentID.Printf("%s_%s",idPrefix,STRING_TO_CHAR(thisID));
				configKey.Printf("/RealNames/%s",STRING_TO_CHAR(componentID));
				globalStatistics.configData.ReadTextValue(configKey,&componentRealName,componentID);
				configKey.Printf("/Images/%s",STRING_TO_CHAR(componentID));
				globalStatistics.configData.ReadTextValue(configKey,&componentImage);
			}
			else
			{
				componentID.Printf("%s",STRING_TO_CHAR(thisID));
			}
			SQL.Printf("Insert into %s"
				"(awardindex,componentindex,id,posneg,image,name)"
				"values('%d','%d','%s','%c','%s','%s')",
				STRING_TO_CHAR(SQLTableNameComponent(component)),
				itemIndex,
				componentIndex,
				STRING_TO_CHAR(componentID),
				posneg,
				STRING_TO_CHAR(componentImage),
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(componentRealName)));
			globalStatistics.statsgenDatabase.SimpleExecute(SQL);
		}
	}
	return retVal;
}

bool AwardDefinition::WriteToDatabase(int itemIndex)
{
	wxString	SQL;
	bool		retVal=true;
	char		weightedChar;

	WriteToDatabaseComponent(itemIndex,"weaponkill",weaponKillComponents,"weapon");
	WriteToDatabaseComponent(itemIndex,"weapondeath",weaponDeathComponents,"weapon");
	WriteToDatabaseComponent(itemIndex,"weapontk",weaponTKComponents,"weapon");
	WriteToDatabaseComponent(itemIndex,"locationkill",locationKillComponents,"location");
	WriteToDatabaseComponent(itemIndex,"locationdeath",locationDeathComponents,"location");
	WriteToDatabaseComponent(itemIndex,"locationtk",locationTKComponents,"location");
	WriteToDatabaseComponent(itemIndex,"action",actionComponents,"action");
	WriteToDatabaseComponent(itemIndex,"misc",miscComponents,"");
	WriteToDatabaseComponent(itemIndex,"xp",xpComponents,"xp");

	if (weighted)
	{
		weightedChar='Y';
	}
	else
	{
		weightedChar='N';
	}
	SQL.Printf("Insert into %s"
				"(awardindex,id,image,name,weighted)"
				"values('%d','%s','%s','%s','%c')",
				STRING_TO_CHAR(SQLTableName()),
				itemIndex,
				STRING_TO_CHAR(id),
				STRING_TO_CHAR(image),
				STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(name)),
				weightedChar);
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	return retVal;
}

void AwardDefinition::SplitCode(wxString &code,bool *positive,bool *teamkill)
{
	char	signChar;

	signChar='+';
	if (code.Length()>0)
	{
		signChar=code[0];
		code=code.Mid(1);
	}
	if (signChar=='+')
	{
		*positive=true;
	}
	else
	{
		*positive=false;
	}
	if (signChar=='T')
	{
		*teamkill=true;
	}
	else
	{
		*teamkill=false;
	}
}

int AwardDefinition::ActionScore(bool *noScore,wxString &key)
{
	int			count;
	int			index;
	wxString	code;
	int			points;
	bool		positive;
	bool		teamkill;

	points=0;
	*noScore=true;
	count=actionComponents.GetCount();
	for (index=0;index<count;index++)
	{
		code=actionComponents.Item(index);
		SplitCode(code,&positive,&teamkill);
		
		if (code.CmpNoCase(key)==0)
		{
			*noScore=false;
			if (positive)
			{
				points++;
			}
			else
			{
				points--;
			}
		}
	}

	return (points);
}

int AwardDefinition::WeaponScore(bool *noScore,wxString &key,bool kill,bool teamkill)
{
	int			count;
	int			index;
	wxString	code;
	int			points;
	bool		positive;
	bool		teamkilldummy;

	points=0;
	*noScore=true;
	if (teamkill)
	{
		count=weaponTKComponents.GetCount();
	}
	else
	{
		if (kill)
		{
			count=weaponKillComponents.GetCount();
		}
		else
		{
			count=weaponDeathComponents.GetCount();
		}
	}
	for (index=0;index<count;index++)
	{
		if (teamkill)
		{
			code=weaponTKComponents.Item(index);
		}
		else
		{
			if (kill)
			{
				code=weaponKillComponents.Item(index);
			}
			else
			{
				code=weaponDeathComponents.Item(index);
			}
		}
		SplitCode(code,&positive,&teamkilldummy);
		//if (code.CmpNoCase(key)==0)
		if (globalStatistics.configData.WeaponMatches(key,code))
		{
			*noScore=false;
			if (positive)
			{
				points++;
			}
			else
			{
				points--;
			}
		}
	}

	return (points);
}

int AwardDefinition::LocationScore(bool *noScore,wxString &key,bool kill,bool teamkill)
{
	int			count;
	int			index;
	wxString	code;
	int			points;
	bool		positive;
	bool		teamkilldummy;

	points=0;
	*noScore=true;
	if (teamkill)
	{
		count=locationTKComponents.GetCount();
	}
	else
	{
		if (kill)
		{
			count=locationKillComponents.GetCount();
		}
		else
		{
			count=locationDeathComponents.GetCount();
		}
	}
	for (index=0;index<count;index++)
	{
		if (teamkill)
		{
			code=locationTKComponents.Item(index);
		}
		else
		{
			if (kill)
			{
				code=locationKillComponents.Item(index);
			}
			else
			{
				code=locationDeathComponents.Item(index);
			}
		}
		SplitCode(code,&positive,&teamkilldummy);
		if (code.CmpNoCase(key)==0)
		{
			*noScore=false;
			if (positive)
			{
				points++;
			}
			else
			{
				points--;
			}
		}
	}

	return (points);
}

int AwardDefinition::MiscScore(bool *noScore,char *key,int count)
{
	int			miscCount;
	int			index;
	wxString	code;
	int			points;
	wxString	keyStr;
	bool		positive;
	bool		teamkilldummy;

	keyStr=key;
	points=0;
	*noScore=true;
	miscCount=miscComponents.GetCount();
	if (count>0)
	{
		for (index=0;index<miscCount;index++)
		{
			code=miscComponents.Item(index);
			SplitCode(code,&positive,&teamkilldummy);
			if (code.CmpNoCase(keyStr)==0)
			{
				*noScore=false;
				if (positive)
				{
					points++;
				}
				else
				{
					points--;
				}
			}
		}
	}

	points*=count;
	return (points);
}

void AwardDefinition::WriteConfig()
{
	wxString	configBase;
	wxString	configKey;
	wxString	configValue;
	wxString	baseGroup;

	baseGroup="AWARDDEFINITION"+id;

	globalStatistics.configData.WriteList(baseGroup,(char *)"WEAPONKILLCODES",weaponKillComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"WEAPONDEATHCODES",weaponDeathComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"WEAPONTKCODES",weaponTKComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"LOCATIONKILLCODES",locationKillComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"LOCATIONDEATHCODES",locationDeathComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"LOCATIONTKCODES",locationTKComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"ACTIONCODES",actionComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"MISCCODES",miscComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"XPCODES",xpComponents);
	globalStatistics.configData.WriteList(baseGroup,(char *)"GAMETYPECODES",allowedGameTypes);

	/*
		THESE DON'T NEED TO BE WRITTEN - THEY ARE TAKEN CARE OF
		IN THE EDITOR

	configBase.Printf("/%s/",baseGroup.GetData());
	configKey=configBase+"Name";
	globalStatistics.configData.WriteTextValue(configKey,name);
	configKey=configBase+"Image";
	globalStatistics.configData.WriteTextValue(configKey,image);

	*/
}
void AwardDefinition::AddWeaponComponent(wxString &code,bool kill,bool positive,bool teamkill)
{
	wxString	componentCode;

	if (positive)
	{
		componentCode="+"+code;
	}
	else
	{
		componentCode="-"+code;
	}
	if (teamkill)
	{
		weaponTKComponents.Add(componentCode);
	}
	else
	{
		if (kill)
		{
			weaponKillComponents.Add(componentCode);
		}
		else
		{
			weaponDeathComponents.Add(componentCode);
		}
	}
	WriteConfig();
}

void AwardDefinition::AddLocationComponent(wxString &code,bool kill,bool positive,bool teamkill)
{
	wxString	componentCode;

	if (positive)
	{
		componentCode="+"+code;
	}
	else
	{
		componentCode="-"+code;
	}
	if (teamkill)
	{
		locationTKComponents.Add(componentCode);
	}
	else
	{
		if (kill)
		{
			locationKillComponents.Add(componentCode);
		}
		else
		{
			locationDeathComponents.Add(componentCode);
		}
	}
	WriteConfig();
}

void AwardDefinition::AddActionComponent(wxString &code,bool positive)
{
	wxString	componentCode;

	if (positive)
	{
		componentCode="+"+code;
	}
	else
	{
		componentCode="-"+code;
	}
	actionComponents.Add(componentCode);
	WriteConfig();
}

void AwardDefinition::AddMiscComponent(wxString &code,bool positive)
{
	wxString	componentCode;

	if (positive)
	{
		componentCode="+"+code;
	}
	else
	{
		componentCode="-"+code;
	}
	miscComponents.Add(componentCode);
	WriteConfig();
}


void AwardDefinition::RemoveWeaponComponent(wxString &code,bool killList,bool teamkillList)
{
	int	componentIndex;

	if (teamkillList)
	{
		componentIndex=weaponTKComponents.Index(code);
		if (componentIndex!=wxNOT_FOUND)
		{
			weaponTKComponents.Remove(code);
		}
	}
	else
	{
		if (killList)
		{
			componentIndex=weaponKillComponents.Index(code);
			if (componentIndex!=wxNOT_FOUND)
			{
				weaponKillComponents.Remove(code);
			}
		}
		else
		{
			componentIndex=weaponDeathComponents.Index(code);
			if (componentIndex!=wxNOT_FOUND)
			{
				weaponDeathComponents.Remove(code);
			}
		}
	}
	WriteConfig();
}

void AwardDefinition::RemoveLocationComponent(wxString &code,bool killList,bool teamkillList)
{
	int	componentIndex;

	if (teamkillList)
	{
		componentIndex=locationTKComponents.Index(code);
		if (componentIndex!=wxNOT_FOUND)
		{
			locationTKComponents.Remove(code);
		}
	}
	else
	{
		if (killList)
		{
			componentIndex=locationKillComponents.Index(code);
			if (componentIndex!=wxNOT_FOUND)
			{
				locationKillComponents.Remove(code);
			}
		}
		else
		{
			componentIndex=locationDeathComponents.Index(code);
			if (componentIndex!=wxNOT_FOUND)
			{
				locationDeathComponents.Remove(code);
			}
		}
	}
	WriteConfig();
}

void AwardDefinition::RemoveActionComponent(wxString &code)
{
	int	componentIndex;

	componentIndex=actionComponents.Index(code);
	if (componentIndex!=wxNOT_FOUND)
	{
		actionComponents.Remove(code);
	}
	WriteConfig();
}

void AwardDefinition::RemoveMiscComponent(wxString &code)
{
	int	componentIndex;

	componentIndex=miscComponents.Index(code);
	if (componentIndex!=wxNOT_FOUND)
	{
		miscComponents.Remove(code);
	}
	WriteConfig();
}

int AwardDefinition::XPScore(bool *noScore,wxString &key,float xpPoints)
{
	int			count;
	int			index;
	wxString	code;
	int			points;
	bool		positive;
	bool		teamkilldummy;
	int			xpPointsInt;

	// XP Points are accurate to 2 decimal places
	xpPoints*=100;
	xpPoints+=0.5;
	xpPointsInt=(int) xpPoints;
	points=0;
	*noScore=true;
	count=xpComponents.GetCount();
	for (index=0;index<count;index++)
	{
		code=xpComponents.Item(index);
		SplitCode(code,&positive,&teamkilldummy);
		
		if (code.CmpNoCase(key)==0)
		{
			*noScore=false;
			if (positive)
			{
				points+=xpPointsInt;
			}
			else
			{
				points-=xpPointsInt;
			}
		}
	}

	return (points);
}

void AwardDefinition::AddXPComponent(wxString &code,bool positive)
{
	wxString	componentCode;

	if (positive)
	{
		componentCode="+"+code;
	}
	else
	{
		componentCode="-"+code;
	}
	xpComponents.Add(componentCode);
	WriteConfig();
}

void AwardDefinition::RemoveXPComponent(wxString &code)
{
	int	componentIndex;

	componentIndex=xpComponents.Index(code);
	if (componentIndex!=wxNOT_FOUND)
	{
		xpComponents.Remove(code);
	}
	WriteConfig();
}

