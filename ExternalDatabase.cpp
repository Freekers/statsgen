#include <wx/wx.h>
#include <wx/string.h>
#ifdef EXTERNAL_DB_ENABLED
#include <mysql/errmsg.h>
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>
#endif
#include "ExternalDatabase.h"
#include "GlobalStatistics.h"
#include "TemplateOpenQuery.h"
#include "Progress.h"
#ifdef EXTERNAL_DB_ENABLED
ExternalDatabase::ExternalDatabase()
{
	mySQLHandle=NULL;
}

ExternalDatabase::~ExternalDatabase()
{
	if (mySQLHandle!=NULL)
	{
		mysql_close(mySQLHandle);
		mySQLHandle=NULL;
	}
}

void ExternalDatabase::UpdateFromConfig()
{
	wxString	configKey;
	wxString	configValue;

	configKey="/ExternalDatabase/Enabled";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"N");
	enabled=(configValue.CmpNoCase("Y")==0);

	configKey="/ExternalDatabase/CreateIndexes";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"N");
	createIndexes=(configValue.CmpNoCase("Y")==0);

	configKey="/ExternalDatabase/Hostname";
	globalStatistics.configData.ReadTextValue(configKey,&hostname,"");

	configKey="/ExternalDatabase/Username";
	globalStatistics.configData.ReadTextValue(configKey,&username,"");

	configKey="/ExternalDatabase/Password";
	globalStatistics.configData.ReadTextValue(configKey,&password,"");

	configKey="/ExternalDatabase/DatabaseName";
	globalStatistics.configData.ReadTextValue(configKey,&databasename,"statsgen");

	configKey="/ExternalDatabase/Port";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"3306");
	port=atoi(STRING_TO_CHAR(configValue));
}

void ExternalDatabase::SplitFieldNames(wxString &createSQL,wxArrayString &fieldNames)
{
	wxString	basicFieldList;
	int			fieldCount;
	int			fieldIndex;
	wxString	field;

	fieldNames.Clear();
	basicFieldList=createSQL.AfterFirst('(');
	basicFieldList=basicFieldList.BeforeLast(')');
	field=GetLineToken(basicFieldList,",",1,&fieldCount);
	for (fieldIndex=1;fieldIndex<=fieldCount;fieldIndex++)
	{
		field=GetLineToken(basicFieldList,",",fieldIndex,&fieldCount);
		field=field.BeforeFirst(' ');
		fieldNames.Add(field);
	}
}

long ExternalDatabase::RowCount(wxString &tableName)
{
	wxString			SQL;
	TemplateOpenQuery	query;
	long				rowCount=0;
	wxString			fieldName="rowcount";
	wxString			fieldValue;

	SQL.Printf("select count(*) as rowcount from %s",STRING_TO_CHAR(tableName));
	query.Initiate(SQL,globalStatistics.statsgenDatabase.DBHandle());
	if (query.NextRow())
	{
		fieldValue=query.RetrieveProperty(fieldName);
		rowCount=atoi(STRING_TO_CHAR(fieldValue));
	}

	return (rowCount);
}

void ExternalDatabase::TransferFromInternal()
{
	wxArrayString	tableNames;
	wxArrayString	tableSQL;
	wxString		tableName;
	wxString		indexName;
	wxString		viewName;
	wxArrayString	indexNames;
	wxArrayString	indexSQL;
	wxArrayString	viewNames;
	wxArrayString	viewSQL;
	wxString		createSQL;
	wxArrayString	fieldNames;
	int				tableCount;
	int				tableIndex;
	int				indexCount;
	int				indexIndex;
	int				viewCount;
	int				viewIndex;
	wxString		msg;
	wxString		SQL;
	long			rowCount;
	long			currentRowCount;
	wxString		beginSQL="begin";
	wxString		commitSQL="commit";
	bool			connectOK;
	STATSGEN_DEBUG_FUNCTION_START("ExternalDatabase","TransferFromInternal")

	UpdateFromConfig();
	if (enabled)
	{
		connectOK=Connect();
		if (connectOK)
		{
		globalStatistics.statsgenDatabase.GetDatabaseComponent("table",tableNames,tableSQL);
		globalStatistics.statsgenDatabase.GetDatabaseComponent("index",indexNames,indexSQL);
		globalStatistics.statsgenDatabase.GetDatabaseComponent("view",viewNames,viewSQL);

		tableCount=tableNames.GetCount();
		indexCount=indexNames.GetCount();
		viewCount=viewNames.GetCount();
		currentRowCount=0;
		rowCount=0;
		for (tableIndex=0;tableIndex<tableCount;tableIndex++)
		{
			tableName=tableNames.Item(tableIndex);
			rowCount+=RowCount(tableName);
		}
		progress->Initiate(rowCount," rows",1," rows",1);
		progress->Update(currentRowCount);
		ExecuteSQL(beginSQL);
		for (tableIndex=0;tableIndex<tableCount;tableIndex++)
		{
			progress->Update(currentRowCount);
			tableName=tableNames.Item(tableIndex);
			msg.Printf("Create Table:%s",STRING_TO_CHAR(tableName));
			progress->SetStatus(msg);
			createSQL=tableSQL.Item(tableIndex);
			createSQL.Replace(" string"," varchar(255)",true);
			createSQL.Replace("(255)(","(",true);
			createSQL.Replace("(255) ("," (",true);
			createSQL.Replace("key "," ky ",true);
			STATSGEN_DEBUG(DEBUG_ALWAYS,createSQL);
			ExecuteSQL(createSQL);
		}
		ExecuteSQL(commitSQL);

		/*
		if (createIndexes)
		{
		ExecuteSQL(beginSQL);
		for (indexIndex=0;indexIndex<indexCount;indexIndex++)
		{
			indexName=indexNames.Item(indexIndex);
			tableName=indexName.AfterFirst('_');
			tableName=tableName.BeforeFirst('_');
			msg.Printf("drop index:%s",indexName.GetData());
			progress->SetStatus(msg);
			createSQL.Printf("drop index %s on %s",
							indexName.GetData(),
							tableName.GetData());
			ExecuteSQL(createSQL);
		}
		ExecuteSQL(commitSQL);
		}
		*/
		for (tableIndex=0;tableIndex<tableCount;tableIndex++)
		{
			ExecuteSQL(beginSQL);
			progress->Update(currentRowCount);
			tableName=tableNames.Item(tableIndex);
			msg.Printf("Empty Table:%s",STRING_TO_CHAR(tableName));
			progress->SetStatus(msg);
			SQL.Printf("delete from %s",STRING_TO_CHAR(tableName));
			ExecuteSQL(SQL);
			ExecuteSQL(commitSQL);
		}
		for (tableIndex=0;tableIndex<tableCount;tableIndex++)
		{
			createSQL=tableSQL.Item(tableIndex);
			tableName=tableNames.Item(tableIndex);
			msg.Printf("Transfer Table:%s",STRING_TO_CHAR(tableName));
			progress->SetStatus(msg);
			SplitFieldNames(createSQL,fieldNames);
			TransferTableData(tableName,fieldNames,&currentRowCount);
		}
		if (createIndexes)
		{
			for (indexIndex=0;indexIndex<indexCount;indexIndex++)
			{
				ExecuteSQL(beginSQL);
				progress->Update(currentRowCount);
				indexName=indexNames.Item(indexIndex);
				msg.Printf("Create index:%s",STRING_TO_CHAR(indexName));
				progress->SetStatus(msg);
				createSQL=indexSQL.Item(indexIndex);
				ExecuteSQL(createSQL);
				ExecuteSQL(commitSQL);
			}
		}
		ExecuteSQL(beginSQL);
		for (viewIndex=0;viewIndex<viewCount;viewIndex++)
		{
			progress->Update(currentRowCount);
			viewName=viewNames.Item(viewIndex);
			msg.Printf("Create view:%s",STRING_TO_CHAR(viewName));
			progress->SetStatus(msg);
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			createSQL=viewSQL.Item(viewIndex);
			char *buffer;
			unsigned long	length;
			wxString	escapedSQL;
			//insertSQL+="'";
			length=(createSQL.Length()*2)+1;
			buffer=(char *)malloc(length);
			if (buffer!=NULL)
			{
				mysql_real_escape_string(mySQLHandle,
									buffer,
									STRING_TO_CHAR(createSQL),
									createSQL.Length());
				escapedSQL=buffer;
				free(buffer);
			}
			STATSGEN_DEBUG(DEBUG_ALWAYS,createSQL)
			//ExecuteSQL(escapedSQL);
			ExecuteSQL(createSQL);
		}
		ExecuteSQL(commitSQL);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void ExternalDatabase::TransferTableData(wxString &tableName,
												wxArrayString &fieldNames,
										long *currentRowCount)
{
	wxString			retrieveSQL;
	wxString			insertSQLPrefix;
	wxString			insertSQL;
	int					fieldCount;
	int					fieldIndex;
	wxString			fieldName;
	wxString			fieldValue;
	TemplateOpenQuery	query;
	wxString			msg;
	wxString			beginSQL="begin";
	wxString			commitSQL="commit";
	wxString			filename="mysql.upload";
	FILE				*fp;
	int					rowCount;
	wxString			lineTerminator;

	lineTerminator="\r\n";

	STATSGEN_DEBUG_FUNCTION_START("ExternalDatabase","TransferTableData")

	progress->SetStatus(tableName);
	filename.Printf("mysqlupload.%s",STRING_TO_CHAR(tableName));
	fieldCount=fieldNames.GetCount();
	retrieveSQL.Printf("select * from %s",STRING_TO_CHAR(tableName));
	insertSQLPrefix.Printf("insert into %s (",STRING_TO_CHAR(tableName));
	
	for (fieldIndex=0;fieldIndex<fieldCount;fieldIndex++)
	{
		fieldName=fieldNames.Item(fieldIndex);
		if (fieldName.CmpNoCase("key")==0)
		{
			fieldName="ky";
		}
		if (fieldIndex!=0)
		{
			insertSQLPrefix+=",";
		}
		insertSQLPrefix+=fieldName;
	}
	insertSQLPrefix+=") values(";
	query.Initiate(retrieveSQL,globalStatistics.statsgenDatabase.DBHandle());
	ExecuteSQL(beginSQL);
	fp=fopen(STRING_TO_CHAR(filename),"w");
	rowCount=0;
	while (query.NextRow())
	{
		rowCount++;
		insertSQL=insertSQLPrefix;

		insertSQL="";

		for (fieldIndex=0;fieldIndex<fieldCount;fieldIndex++)
		{
			fieldName=fieldNames.Item(fieldIndex);
			if (fieldIndex!=0)
			{
				//insertSQL+=",";
				insertSQL+="\t";
			}
			fieldValue=query.RetrieveProperty(fieldName);
			char *buffer;
			unsigned long	length;
			//insertSQL+="'";
			length=(fieldValue.Length()*2)+1;
			buffer=(char *)malloc(length);
			if (buffer!=NULL)
			{
				mysql_real_escape_string(mySQLHandle,
									buffer,
									STRING_TO_CHAR(fieldValue),
									fieldValue.Length());
				insertSQL+=buffer;
				free(buffer);
			}
			//insertSQL+="'";
		}
		if (fp!=NULL)
		{
			fprintf(fp,"%s\n",(const char *)STRING_TO_CHAR(insertSQL));
		}
		/*
		insertSQL+=")";
		ExecuteSQL(insertSQL);
		(*currentRowCount)++;
		if (((*currentRowCount) % 10000)==0)
		{
			ExecuteSQL(commitSQL);
			ExecuteSQL(beginSQL);
		}
		progress->Update(*currentRowCount);
		*/
		//STATSGEN_DEBUG(DEBUG_RARELY,insertSQL)
	}
	if (fp!=NULL)
	{
		fclose(fp);
	}
/*
	insertSQL.Printf(
				"LOAD DATA LOCAL INFILE '%s' "
				"INTO TABLE %s "
				"FIELDS "
				"TERMINATED BY '\\%c' "
				"ENCLOSED BY '\\%c' "
				"ESCAPED BY '\\%c'",
				filename.GetData(),
				tableName.GetData(),
				',',	// Fields terminated by
				'\'',	// Enclosed by
				'\\');	// Escaped by
*/
	insertSQL.Printf(
				"LOAD DATA LOCAL INFILE '%s' "
				"INTO TABLE %s "
				"FIELDS "
				"TERMINATED BY '\\%c' "
				"ESCAPED BY '\\%c'"
				"LINES "
				"TERMINATED BY '\\%s' "
				,
				STRING_TO_CHAR(filename),
				STRING_TO_CHAR(tableName),
				'\t',	// Fields terminated by
				'\\',	// Fields escaped by
				STRING_TO_CHAR(lineTerminator)	// Line terminated by
				);	// Escaped by
	ExecuteSQL(insertSQL);
	(*currentRowCount)+=(rowCount);
	progress->Update(*currentRowCount);
	ExecuteSQL(commitSQL);
	STATSGEN_DEBUG_FUNCTION_END
}

wxString ExternalDatabase::EscapedString(wxString &escaped)
{
	wxString	retVal;
	int			length;
	char		*buffer;

	STATSGEN_DEBUG_FUNCTION_START("ExternalDatabase","EscapedString")
	STATSGEN_DEBUG(DEBUG_RARELY,escaped)
	retVal=escaped;
	length=(escaped.Length()*2)+1;
	buffer=(char *)malloc(length);
	if (buffer!=NULL)
	{
		mysql_real_escape_string(mySQLHandle,
									buffer,
									STRING_TO_CHAR(retVal),
									retVal.Length());
		retVal=buffer;
		free(buffer);
	}
//	STATSGEN_DEBUG(DEBUG_RARELY,retVal)
//	retVal.Replace("-","\\-");

	STATSGEN_DEBUG(DEBUG_RARELY,retVal)
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool ExternalDatabase::CreateDatabaseInstance()
{
	bool		retVal=true;
	wxString	SQL;
	SQL.Printf("create database %s",STRING_TO_CHAR(databasename));
	retVal=ExecuteSQL(SQL);
	return (retVal);
}

bool ExternalDatabase::Connect()
{
	bool		retVal=true;
	wxString	msg;

	if (mySQLHandle==NULL)
	{
		mySQLHandle=mysql_init(NULL);
	}
	if (mySQLHandle!=NULL)
	{

		if (mysql_real_connect(mySQLHandle,
							STRING_TO_CHAR(hostname),
							STRING_TO_CHAR(username),
							STRING_TO_CHAR(password),
							//databasename.GetData(),
							NULL,
							port,
							NULL,
							CLIENT_COMPRESS)==NULL)
		{
			// Failed to connect
			retVal=false;
			msg.Printf("Connect Error: %s",mysql_error(mySQLHandle));
			progress->LogError(msg,SeverityError);
		}
		else
		{
			retVal=CreateDatabaseInstance();

			if (mysql_real_connect(mySQLHandle,
							STRING_TO_CHAR(hostname),
							STRING_TO_CHAR(username),
							STRING_TO_CHAR(password),
							STRING_TO_CHAR(databasename),
							port,
							NULL,
							CLIENT_COMPRESS)==NULL)
			{
				// Failed to connect
				retVal=false;
				msg.Printf("Connect Error: %s",mysql_error(mySQLHandle));
				progress->LogError(msg,SeverityError);
			}
		}
	}
	else
	{
		msg.Printf("Error: %s",mysql_error(mySQLHandle));
		progress->LogError(msg,SeverityError);
	}

	return (retVal);
}

bool ExternalDatabase::ExecuteSQL(wxString &SQL)
{
	bool		retVal=true;
	int			status;
	wxString	msg;

	if (mySQLHandle!=NULL)
	{
		status=mysql_real_query(mySQLHandle,
								STRING_TO_CHAR(SQL),
								SQL.Length());
		status=mysql_errno(mySQLHandle);
		switch (status)
		{
			case 0:
			case ER_TABLE_EXISTS_ERROR:
			case ER_DUP_KEYNAME:
			case ER_DB_CREATE_EXISTS:
				break;
			default:
				msg.Printf("SQL Error: %d %s",status,mysql_error(mySQLHandle));
				progress->LogError(msg,SeverityError);
				msg.Printf("SQL Attempted: %s",STRING_TO_CHAR(SQL));
				progress->LogError(msg,SeverityError);
				retVal=false;
				break;
		}
	}
	
	return (retVal);
}
#else
ExternalDatabase::ExternalDatabase()
{
}

ExternalDatabase::~ExternalDatabase()
{
}

void ExternalDatabase::TransferFromInternal()
{
	wxString error = "External Database Connectivity Disabled";
	progress->LogError(error,SeverityError);
}

#endif
