// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// standard includes
#include <string.h>
#include <stdlib.h>
// Statsgen includes
#include "TemplateOpenQuery.h"
#include "GlobalStatistics.h"
#include "Progress.h"

TemplateOpenQuery::TemplateOpenQuery()
{
	query=NULL;
}

bool TemplateOpenQuery::Initiate(wxString &SQLIn,
								sqlite3 *dbHandleIn)
{
	bool			retVal=true;
	wxString		msg;
	const char		*unusedSQL;
	int				status;

	STATSGEN_DEBUG_FUNCTION_START("TemplateOpenQuery","Initiate")

	SQL=SQLIn;
	dbHandle=dbHandleIn;
	if (dbHandle!=NULL)
	{
		status=sqlite3_prepare(dbHandle,
					SQL.GetData(),
					-1,
					&query,
					&unusedSQL);

		if (status==SQLITE_OK)
		{
			int			columnIndex;
			const char	*columnNamePtr;
			wxString	columnName;

			retVal=true;
			columnIndex=0;
			columnNamePtr=sqlite3_column_name(query,columnIndex);
			while (columnNamePtr!=NULL)
			{
				columnName=columnNamePtr;
				columnNames.Add(columnName);
				columnIndex++;
				columnNamePtr=sqlite3_column_name(query,columnIndex);
			}
		}
		else
		{
			retVal=false;
			msg.Printf("Failed to prepare [%s]",SQL.GetData());
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			progress->LogError(msg,SeverityError);
			query=NULL;
		}
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"null db handle");
		retVal=false;
		query=NULL;
	}
					
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

TemplateOpenQuery::~TemplateOpenQuery()
{
	if (query!=NULL)
	{
		// Clear down any memory used by the query and delete it
		sqlite3_finalize(query);
		query=NULL;
	}
}

bool TemplateOpenQuery::NextRow()
{
	bool			retVal=false;
	wxString		msg;
	int				status;
	int				columnCount;
	int				columnIndex;
	wxString		value;
	STATSGEN_DEBUG_FUNCTION_START("TemplateOpenQuery","NextRow")

	columnValues.Clear();
	if (query!=NULL)
	{
		status=sqlite3_step(query);
		switch (status)
		{
			case SQLITE_ROW:
				columnCount=columnNames.GetCount();
				for (columnIndex=0;columnIndex<columnCount;columnIndex++)
				{
					value=sqlite3_column_text(query,columnIndex);
					columnValues.Add(value);
				}
				retVal=true;
				break;
			case SQLITE_DONE:
				retVal=false;
				break;
			case SQLITE_BUSY:
				STATSGEN_DEBUG(DEBUG_ALWAYS,"BUSY")
				retVal=false;
				break;
			case SQLITE_ERROR:
				STATSGEN_DEBUG(DEBUG_ALWAYS,"ERROR")
				retVal=false;
				break;
			case SQLITE_MISUSE:
				STATSGEN_DEBUG(DEBUG_ALWAYS,"MISUSE")
				retVal=false;
				break;
		}
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Strange - query is null during nextrow")
		retVal=false;
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

wxString TemplateOpenQuery::RetrieveProperty(wxString &property)
{
	wxString	retVal="";
	int			columnIndex;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("TemplateOpenQuery","RetrieveProperty")
	//if (query!=NULL)
	{
		columnIndex=wxNOT_FOUND;
		if (property.IsNumber())
		{
			// property is a direct column access
			columnIndex=atoi(property.GetData());
		}
		else
		{
			columnIndex=columnNames.Index(property.GetData());
		}
		if ((columnIndex>=0) && (columnIndex<columnValues.GetCount()))
		{
			retVal=columnValues.Item(columnIndex);
		}
	}
	//else
	//{
	//	STATSGEN_DEBUG(DEBUG_ALWAYS,"Strange - query is null during retreiveproperty")
	//}
	
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}


void TemplateOpenQuery::Clone(TemplateOpenQuery *orgQuery)
{
	int			columnCount;
	int			columnIndex;
	wxString	columnValue;
	wxString	columnName;

	columnNames.Clear();
	columnValues.Clear();
	columnCount=orgQuery->ColumnCount();
	for (columnIndex=0;columnIndex<columnCount;columnIndex++)
	{
		columnName=orgQuery->GetColumnName(columnIndex);
		columnValue=orgQuery->GetColumnValue(columnIndex);
		columnNames.Add(columnName);
		columnValues.Add(columnValue);
	}
}

wxString TemplateOpenQuery::GetColumnName(int index)
{
	wxString	retVal="";

	if ((index>=0)&&(index<columnNames.GetCount()))
	{
		retVal=columnNames.Item(index);
	}

	return retVal;
}

wxString TemplateOpenQuery::GetColumnValue(int index)
{
	wxString	retVal="";

	if ((index>=0)&&(index<columnValues.GetCount()))
	{
		retVal=columnValues.Item(index);
	}

	return retVal;
}

int TemplateOpenQuery::ColumnCount()
{
	return (columnNames.GetCount());
}
