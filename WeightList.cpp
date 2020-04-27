// wxWindows includes
#include <wx/string.h>

// Statsgen includes
#include "WeightListEntry.h"
#include "GlobalStatistics.h"

WeightList::WeightList()
{
}

void WeightList::Initiate(const char *prefixIn)
{
	Clear();
	prefix=prefixIn;
}

WeightList::~WeightList()
{
}

float WeightList::Weight(wxString &key,float defaultValue)
{
	WeightListEntry	weightListEntry;
	int		weightListCount;
	int		weightListIndex;
	float		retVal=0.0;
	bool		found;

	weightListCount=weightList.GetCount();
	found=false;
	for (weightListIndex=0;
		weightListIndex<weightListCount;
		weightListIndex++)
	{
		weightListEntry=weightList.Item(weightListIndex);
		if (weightListEntry.key.Cmp(key)==0)
		{
			retVal=weightListEntry.weight;
			found=true;
		}
	}
	if (!found)
	{
		weightListEntry.key=key;
		weightListEntry.defaultValue=defaultValue;
		weightListEntry.UpdateWeighting(prefix);
		retVal=weightListEntry.weight;
		weightList.Add(weightListEntry);
	}

	return (retVal);
}

void WeightList::Clear()
{
	weightList.Clear();
}

void WeightList::CreateDatabase()
{
	wxString	SQL;

	SQL.Printf("create table %s"
			"("
				"key string,"
				"weight float,"
				"realname string,"
				"image string"
			");",
			STRING_TO_CHAR(prefix));
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
}

void WeightList::WriteToDatabase()
{
	wxString			SQL;
	int					weightCount;
	int					weightIndex;
	WeightListEntry		weight;
	wxString			realName;
	wxString			image;
	wxString			configKey;

	weightCount=weightList.GetCount();
	for (weightIndex=0;weightIndex<weightCount;weightIndex++)
	{
		weight=weightList.Item(weightIndex);
		configKey.Printf("/RealNames/%s",STRING_TO_CHAR(weight.key));
		globalStatistics.configData.ReadTextValue(configKey,&realName,weight.key);
		configKey.Printf("/Images/%s",STRING_TO_CHAR(weight.key));
		globalStatistics.configData.ReadTextValue(configKey,&image);
		SQL.Printf("insert into %s "
			"(key,weight,realname,image)"
			"values ('%s','%f','%s','%s');",
			STRING_TO_CHAR(prefix),
			STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(weight.key)),
			weight.weight,
			STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(realName)),
			STRING_TO_CHAR(StatsgenDatabase::SafeForInsert(image)));
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	}

}
void WeightList::EmptyDatabase()
{
	wxString			SQL;

	SQL.Printf("delete from %s ",STRING_TO_CHAR(prefix));
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
}

void WeightList::ReadFromDatabase()
{
	wxString			SQL;
	WeightListEntry		weight;
	TemplateOpenQuery	query;
	static wxString		propertyKey="key";
	static wxString		propertyWeight="weight";
	wxString			value;

	weightList.Clear();
	SQL.Printf("select * from %s",
			STRING_TO_CHAR(prefix));
	query.Initiate(SQL,globalStatistics.statsgenDatabase.DBHandle());
	while (query.NextRow())
	{
		value=query.RetrieveProperty(propertyKey);
		weight.key=value;
		value=query.RetrieveProperty(propertyWeight);
		weight.weight=atof(STRING_TO_CHAR(value));
		weightList.Add(weight);
	}
}
