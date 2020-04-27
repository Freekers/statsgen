// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>

// Statsgen includes
#include "TemplateVariable.h"
#include "GlobalStatistics.h"
#include "LogFileReader.h"
#include "ErrorData.h"
#include "Progress.h"
#include "StaticFunctions.h"

TemplateVariable::TemplateVariable()
{
	templateFile = NULL;
	currentProcedurePrefix="";
}

TemplateVariable::~TemplateVariable()
{
}

bool TemplateVariable::StoreVariable(wxString &variableOrg,wxString &value)
{
	bool					retVal=true;
	int						variableCount;
	int						variableIndex;
	TemplateVariableField	variableField;
	wxString				variable;

	STATSGEN_DEBUG_FUNCTION_START("TemplateVariable","StoreVariable")
	
	variable=variableOrg;
	variable.Replace("self_",currentProcedurePrefix);
	STATSGEN_DEBUG(DEBUG_RARELY,variableOrg)
	STATSGEN_DEBUG(DEBUG_RARELY,variable)
	STATSGEN_DEBUG(DEBUG_RARELY,currentProcedurePrefix)
	STATSGEN_DEBUG(DEBUG_RARELY,value)
	// Let us find the variable first (if it exists)
	variableCount=variableFields.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		variableField=variableFields.Item(variableIndex);
		if (variableField.variable.Cmp(variable)==0)
		{
			// This variable already exists - remove it - and stop
			// searching - we will add it at the end
			variableFields.RemoveAt(variableIndex);
			break;
		}
	}

	variableField.variable=variable;
	variableField.value=value;
	variableFields.Add(variableField);

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool TemplateVariable::QueryNextRow(wxString &variableOrg)
{
	bool					retVal=true;
	bool					found=false;
	int						variableCount;
	int						variableIndex;
	TemplateOpenQuery		*openQuery=NULL;
	wxString				msg;
	wxString				variable;
	

	STATSGEN_DEBUG_FUNCTION_START("TemplateVariable","QueryNextRow")
	variable=variableOrg;
	variable.Replace("self_",currentProcedurePrefix);
	// Let us find the variable first (if it exists)
	variableCount=openQueries.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		openQuery=openQueries.Detach(0);
		if (openQuery->variable.Cmp(variable)==0)
		{
			// This variable already exists 
			found=true;
			break;
		}
		else
		{
			// Add it back - we are not interested in it
			openQueries.Add(openQuery);
		}
	}

	if (found)
	{
		// found the query - get next row from it
		retVal=openQuery->NextRow();
		// now we have used it - we can add it back
		openQueries.Add(openQuery);
	}
	else
	{
		msg.Printf("Could not find stored query [%s]",STRING_TO_CHAR(variable));
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		progress->LogError(msg,SeverityCaution);
		retVal=false;
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

wxString TemplateVariable::SubstituteVariableValues(wxString &text)
{
	// Variables within a template code are identified by %% symbols
	int			charIndexStart;
	int			charIndexLength;
	wxString	variable;
	wxString	value;
	wxString	afterVarSymbols;
	bool		finished=false;
	wxString	msg;
	wxString	workingText;

	STATSGEN_DEBUG_FUNCTION_START("TemplateVariable","SubstituteVariableValues")

	workingText=text;

	while (!finished)
	{
		charIndexStart=workingText.Find("%%");
		if (charIndexStart<0)
		{
			finished=true;
		}
		else
		{
			// Found the first %% - need to find the end one
			afterVarSymbols=workingText.Mid(charIndexStart+2);
			charIndexLength=afterVarSymbols.Find("%%");
			if (charIndexLength<0)
			{
				msg.Printf("Closing %% missing from [%s]",STRING_TO_CHAR(workingText));
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
				progress->LogError(msg,SeverityError);
				finished=true;
			}
			else
			{
				wxString	search;
				wxString	tempVariable;
	
				variable=afterVarSymbols.Left(charIndexLength);
				tempVariable=variable;
				value=GetVariableValue(tempVariable);
				search="%%"+variable+"%%";
				workingText.Replace(search,value);
			}
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (workingText);
}

bool TemplateVariable::StoreQuery(wxString &variableOrg,wxString &SQL)
{
	bool					retVal=true;
	int						variableCount;
	int						variableIndex;
	TemplateOpenQuery		*openQuery=NULL;
	wxString				msg;
	wxString				variable;
	

	variable=variableOrg;
	variable.Replace("self_",currentProcedurePrefix);
	// Let us find the variable first (if it exists)
	variableCount=openQueries.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		openQuery=openQueries.Detach(0);
		if (openQuery->variable.Cmp(variable)==0)
		{
			// This variable already exists - remove it - and stop
			// searching - we will add it at the end
			delete openQuery;
			openQuery=NULL;
			break;
		}
		else
		{
			// We are not interested in this query - add it back
			openQueries.Add(openQuery);
		}
	}

	openQuery=new TemplateOpenQuery;
	openQuery->variable=variable;
	openQuery->Initiate(SQL,
						globalStatistics.statsgenDatabase.DBHandle());
	openQueries.Add(openQuery);

	return (retVal);
}

bool TemplateVariable::RemoveQuery(wxString &variableOrg)
{
	bool					retVal=true;
	int						variableCount;
	int						variableIndex;
	TemplateOpenQuery		*openQuery=NULL;
	wxString				variable;
	wxString				msg;
	

	// Let us find the variable first (if it exists)
	variable=variableOrg;
	variable.Replace("self_",currentProcedurePrefix);
	variableCount=openQueries.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		openQuery=openQueries.Detach(0);
		if (openQuery->variable.Cmp(variable)==0)
		{
			// This variable already exists - remove it - and stop
			// searching - we will add it at the end
			delete openQuery;
			openQuery=NULL;
			break;
		}
		else
		{
			// not interested in this query - add it back
			openQueries.Add(openQuery);
		}
	}

	return (retVal);
}

wxString TemplateVariable::Hash(wxString &text)
{
	wxString	retVal;
	int			textLength;
	int			charIndex;
	int			charValue;
	wxString	coded;

	retVal="";
	textLength=text.Length();
	for (charIndex=0;charIndex<textLength;charIndex++)
	{
		charValue=text[charIndex];
		if ((charValue>'f') && (charValue<='z'))
		{
			coded.Printf("%c",charValue);
		}
		else
		{
			coded.Printf("%02x",charValue);
		}
		retVal+=coded;
	}

	return (retVal);
}

wxString TemplateVariable::ColouredText2HTML(wxString &colouredText)
{
	wxString		font0;
	wxString		font1;
	wxString		font2;
	wxString		font3;
	wxString		font4;
	wxString		font5;
	wxString		font6;
	wxString		font7;
	wxString		font8;
	wxString		font9;

	wxString		configKey;
	wxString		replace;
	wxString		retVal;

	retVal=colouredText;

	configKey="/Colours/0";globalStatistics.configData.ReadTextValue(configKey,&font0,(char *)"000000");
	configKey="/Colours/1";globalStatistics.configData.ReadTextValue(configKey,&font1,(char *)"FF0000");
	configKey="/Colours/2";globalStatistics.configData.ReadTextValue(configKey,&font2,(char *)"00FF00");
	configKey="/Colours/3";globalStatistics.configData.ReadTextValue(configKey,&font3,(char *)"FFFF00");
	configKey="/Colours/4";globalStatistics.configData.ReadTextValue(configKey,&font4,(char *)"0000FF");
	configKey="/Colours/5";globalStatistics.configData.ReadTextValue(configKey,&font5,(char *)"00FFFF");
	configKey="/Colours/6";globalStatistics.configData.ReadTextValue(configKey,&font6,(char *)"FF00FF");
	configKey="/Colours/7";globalStatistics.configData.ReadTextValue(configKey,&font7,(char *)"FFFFFF");
	configKey="/Colours/8";globalStatistics.configData.ReadTextValue(configKey,&font8,(char *)"008000");
	configKey="/Colours/9";globalStatistics.configData.ReadTextValue(configKey,&font9,(char *)"808080");

	// safetyfi - that's a good word isn't it - the html
	retVal.Replace("&","&amp;");
	retVal.Replace("\"","&quot;");
	retVal.Replace("<","&lt;");
	retVal.Replace(">","&gt;");

	// Replace colours
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font0));retVal.Replace("^0",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font1));retVal.Replace("^1",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font2));retVal.Replace("^2",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font3));retVal.Replace("^3",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font4));retVal.Replace("^4",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font5));retVal.Replace("^5",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font6));retVal.Replace("^6",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font7));retVal.Replace("^7",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font8));retVal.Replace("^8",STRING_TO_CHAR(replace));
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font9));retVal.Replace("^9",STRING_TO_CHAR(replace));

	// add in close fonts
	retVal.Replace("<font ","</font><font ");
	// Open white font at the beginning
	replace.Printf("<font color=#%s>",STRING_TO_CHAR(font7));
	retVal=replace+retVal;
	// Add in closing font
	retVal=retVal+"</font>";

	return (retVal);
}

wxString TemplateVariable::GetVariableValue(wxString &templateCode)
{
	int					tokenCount;
	wxString			variable;
	TemplateOpenQuery	*openQuery;
	wxString			value;
	wxString			queryProperty;
	wxString			msg;
	wxString			functionName;
	wxString			parameter;
	wxString			parameterValue;
	wxString			assignmentVar;

	STATSGEN_DEBUG_FUNCTION_START("TemplateVariable","GetVariableValue")

	STATSGEN_DEBUG(DEBUG_RARELY,templateCode)
	if (templateCode[0]=='\"')
	{
		// we have found a literal e.g. "shaun"
		// Skip past opening quotes
		templateCode=templateCode.AfterFirst('\"');
		// parameter is before closing quote
		parameterValue=templateCode.BeforeFirst('\"');
		// remaining template code is after closing quote
		templateCode=templateCode.AfterFirst('\"');

		value=parameterValue;
	}
	else if (templateCode[0]=='(')
	{
		// We have found a condition
		value=EvaluateCondition(templateCode);
	}
	else
	{
		assignmentVar="";
		while	(
					(templateCode.Length()>0) &&
					(	isalnum(templateCode[0]) || 
						(templateCode[0]=='.') ||
						(templateCode[0]=='-') ||
						(templateCode[0]=='_')
					)
				)
		{
			assignmentVar+=templateCode[0];
			templateCode=templateCode.Mid(1);
		}

		if ((templateCode.Length()>0) &&
			(templateCode[0]=='='))
		{
			// This is an assignment a=b
			templateCode=templateCode.AfterFirst('=');
			parameterValue=GetVariableValue(templateCode);
			StoreVariable(assignmentVar,parameterValue);
			// no value returned
			value="";
		}
		else
		{
			if ((templateCode.Length()>0) &&
				(templateCode[0]=='('))
			{
				wxArrayString			parameterValues;
				int						parameterIndex;
				int						parameterCount;
				wxString				parameterValue2;
				TemplateValueList		parameterValueList;
				TemplateValue			*parameterValueItem;

				functionName=assignmentVar;
				// functions are functioname([parameter],)
				// We have found an open bracket - function name before it
				templateCode=templateCode.AfterFirst('(');
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Extracting parameter list")
				while ((templateCode.Length()>0) && 
						(templateCode[0]!=')'))
				{
					parameterValue=GetVariableValue(templateCode);
					parameterValues.Add(parameterValue);
					parameterValueItem=new TemplateValue;
					TemplateOpenQuery	*query;
					query=RetrieveQuery(templateCode);
					if (query == NULL)
					{
						parameterValueItem->SetString(parameterValue);
					}
					else
					{
						parameterValueItem->SetQuery(query);
					}
					parameterValueList.Add(parameterValueItem);
					STATSGEN_DEBUG(DEBUG_RARELY,templateCode)
					STATSGEN_DEBUG(DEBUG_RARELY,parameterValue)
					if ((templateCode.Length()>0) &&
						(templateCode[0]!=')'))
					{
						// More text in template code and it is not a close bracket
						// Probably a separating comma - just skip past it
						templateCode=templateCode.AfterFirst(',');
					}
				}
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Extracted parameter list")
				// We have scanned all the template code now and are exiting
				// parameter search, either because reached end of string or
				// found a closing bracket
				if (templateCode.Length()>0)
				{
					templateCode=templateCode.AfterFirst(')');
				}
				parameterCount=parameterValues.GetCount();
				if (functionName.CmpNoCase("colouredtext2html")==0)
				{
					if (parameterCount==1)
					{
						parameterValue=parameterValues.Item(0);
						value=ColouredText2HTML(parameterValue);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting %d",
								STRING_TO_CHAR(functionName),
								parameterCount,
								1);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else if (functionName.CmpNoCase("hash")==0)
				{
					if (parameterCount==1)
					{
						parameterValue=parameterValues.Item(0);
						value=Hash(parameterValue);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting %d",
								STRING_TO_CHAR(functionName),
								parameterCount,
								1);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else if (functionName.CmpNoCase("formatfloat")==0)
				{
					if (parameterCount==2)
					{
						parameterValue=parameterValues.Item(0);
						parameterValue2=parameterValues.Item(1);
						value=FormatFloat(parameterValue,parameterValue2);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting %d",
								STRING_TO_CHAR(functionName),
								parameterCount,
								2);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else if (functionName.CmpNoCase("formatinteger")==0)
				{
					if (parameterCount==2)
					{
						parameterValue=parameterValues.Item(0);
						parameterValue2=parameterValues.Item(1);
						value=FormatInteger(parameterValue,parameterValue2);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting %d",
								STRING_TO_CHAR(functionName),
								parameterCount,
								2);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else if (functionName.CmpNoCase("updatestatus")==0)
				{
					value=UpdateStatus(parameterValues);
				}
				else if (functionName.CmpNoCase("sql")==0)
				{
					value=SimpleSQL(parameterValues);
				}
				else if (functionName.CmpNoCase("case")==0)
				{
					value=Case(parameterValues);
				}
				else if (functionName.CmpNoCase("readtemplateconfig")==0)
				{
					value=ReadTemplateConfig(parameterValues);
				}
				else if (functionName.CmpNoCase("writeconfig")==0)
				{
					value=WriteConfig(parameterValues);
				}
				else if (functionName.CmpNoCase("readconfig")==0)
				{
					value=ReadConfig(parameterValues);
				}
				else if (functionName.CmpNoCase("formatdate")==0)
				{
					if (parameterCount==2)
					{
						parameterValue=parameterValues.Item(0);
						parameterValue2=parameterValues.Item(1);
						value=FormatDate(parameterValue,parameterValue2);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting %d",
									STRING_TO_CHAR(functionName),
									parameterCount,
									2);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else if (functionName.CmpNoCase("formatduration")==0)
				{
					if (parameterCount==2)
					{
						parameterValue=parameterValues.Item(0);
						parameterValue2=parameterValues.Item(1);
						value=FormatDuration(parameterValue,parameterValue2);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting %d",
									STRING_TO_CHAR(functionName),
									parameterCount,
									2);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else if (functionName.CmpNoCase("uppercase")==0)
				{
					value="";
					for (parameterIndex=0;
						parameterIndex<parameterCount;
						parameterIndex++)
					{
						parameterValue=parameterValues.Item(parameterIndex).Upper();
						value+=parameterValue;
					}
				}
				else if (functionName.CmpNoCase("lowercase")==0)
				{
					value="";
					for (parameterIndex=0;
						parameterIndex<parameterCount;
						parameterIndex++)
					{
						parameterValue=parameterValues.Item(parameterIndex).Lower();
						value+=parameterValue;
					}
				}
				else if (functionName.CmpNoCase("messagecentre")==0)
				{
					if (parameterCount>=2)
					{
						parameterValue=parameterValues.Item(0);
						parameterValue2=parameterValues.Item(1);
						value=MessageCentre(parameterValues);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting at least %d",
									STRING_TO_CHAR(functionName),
									parameterCount,
									2);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else if (functionName.CmpNoCase("ReadStatistic")==0)
				{
					if (parameterCount==1)
					{
						parameterValue=parameterValues.Item(0);
						value=ReadStatistic(parameterValue);
					}
					else
					{
						msg.Printf("function name [%s()] has %d parameters, but expecting %d",
									STRING_TO_CHAR(functionName),
									parameterCount,
									2);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						progress->LogError(msg,SeverityError);
					}
				}
				else
				{
					/*
					msg.Printf("Unknown function name [%s(%s)]",functionName.GetData(),
						parameter.GetData());
					STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
					progress->LogError(msg,SeverityError);
					*/
					ExecuteCustomProcedure(functionName,&parameterValueList);
				}
			}
			else
			{
				wxString	rawVariable;

				rawVariable=assignmentVar;
				variable=GetLineToken(rawVariable,(char *)".",1,&tokenCount);
				value="";
				if (tokenCount!=1)
				{
					// We are after a query as there is a "." in there
					openQuery=RetrieveQuery(variable);
					if (openQuery!=NULL)
					{
						queryProperty=GetLineToken(rawVariable,(char *)".",2,&tokenCount);
						value=openQuery->RetrieveProperty(queryProperty);
					}
					else
					{
//						msg.Printf("Could not find an open query for [%s]",
//								rawVariable.GetData());
//						StatsgenDebug(msg);
					}
				}
				else
				{
					// we are after a single variable
					value=RetrieveVariableValue(variable);
				}
			}
		}
	}
	
	STATSGEN_DEBUG_FUNCTION_END
	return (value);
}

TemplateOpenQuery *TemplateVariable::RetrieveQuery(wxString &variableOrg)
{
	TemplateOpenQuery		*retVal=NULL;
	int						variableCount;
	int						variableIndex;
	TemplateOpenQuery		*openQuery=NULL;
	wxString				variable;
	wxString				msg;

	variable=variableOrg;
	variable.Replace("self_",currentProcedurePrefix);
	// Let us find the variable first (if it exists)
	variableCount=openQueries.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		openQuery=openQueries.Detach(0);
		openQueries.Add(openQuery);
		if (openQuery->variable.Cmp(variable)==0)
		{
			// This is the query we are after
			retVal=openQuery;
			break;
		}
	}

	return (retVal);
}

wxString TemplateVariable::RetrieveVariableValue(wxString &variableOrg)
{
	wxString				retVal="";
	int						variableCount;
	int						variableIndex;
	TemplateVariableField	variableField;
	wxString				variable;
	wxString				msg;

	STATSGEN_DEBUG_FUNCTION_START("TemplateVariable","RetrieveVariableValue")
	variable=variableOrg;
	variable.Replace("self_",currentProcedurePrefix);

	STATSGEN_DEBUG(DEBUG_RARELY,variableOrg)
	STATSGEN_DEBUG(DEBUG_RARELY,variable)
	STATSGEN_DEBUG(DEBUG_RARELY,currentProcedurePrefix)

	// Let us find the variable first (if it exists)
	variableCount=variableFields.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		variableField=variableFields.Item(variableIndex);
		if (variableField.variable.Cmp(variable)==0)
		{
			// This is the variable we are after
			retVal=variableField.value;
			break;
		}
	}
	STATSGEN_DEBUG(DEBUG_RARELY,retVal)
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

wxString TemplateVariable::FormatDuration(wxString &format,wxString &duration)
{
	wxString	retVal;
	long		seconds;
	wxString	msg;

	seconds=atol(STRING_TO_CHAR(duration));
	wxTimeSpan	timeSpan=wxTimeSpan::Seconds(seconds);

	retVal=timeSpan.Format(STRING_TO_CHAR(format));

	return (retVal);
}

wxString TemplateVariable::FormatInteger(wxString &format,wxString &integerStr)
{
	wxString	retVal;
	long		number;
	wxString	msg;

	number=atol(STRING_TO_CHAR(integerStr));
	retVal.Printf(STRING_TO_CHAR(format),number);

	return (retVal);
}

wxString TemplateVariable::EvaluateCondition(wxString &conditionString)
{
	bool		result=false;
	wxString	leftValue;
	wxString	rightValue;
	wxString	operatorString;
	wxString	retVal;
	wxString	msg;
	bool		bothLong;
	bool		bothDouble;
	double		leftDouble;
	double		rightDouble;
	long		leftLong;
	long		rightLong;

	// remove leading and trailing spaces
	conditionString.Trim(true);
	conditionString.Trim(false);

	// first character should be a "(" - skip past it
	conditionString=conditionString.AfterFirst('(');

	// Get the first variable - this should advance condition string
	// to be after the first variable
	leftValue=GetVariableValue(conditionString);
	// condition string should now have moved on to the operator
	conditionString.Trim(true);
	conditionString.Trim(false);
	operatorString=conditionString.BeforeFirst(' ');
	// second variable now
	conditionString=conditionString.AfterFirst(' ');
	rightValue=GetVariableValue(conditionString);
	// conditionString should have advanced past the second variable now
	// next bit should be a closing brace
	conditionString=conditionString.AfterFirst(')');


	bothDouble=((leftValue.ToDouble(&leftDouble))&&
				(rightValue.ToDouble(&rightDouble)));
	bothLong=((leftValue.ToLong(&leftLong))&&
				(rightValue.ToLong(&rightLong)));

	result=(leftValue.CmpNoCase(TRUE_STRING)==0);
	if (operatorString.CmpNoCase("EQUALS")==0)
	{
		result=(leftValue.Cmp(rightValue)==0);
		if (bothDouble)
		{
			result=(leftDouble==rightDouble);
		}
		if (bothLong)
		{
			result=(leftLong==rightLong);
		}
	}
	else if (operatorString.CmpNoCase("!EQUALS")==0)
	{
		result=(leftValue.Cmp(rightValue)!=0);
		if (bothDouble)
		{
			result=(leftDouble!=rightDouble);
		}
		if (bothLong)
		{
			result=(leftLong!=rightLong);
		}
	}
	else if (operatorString.CmpNoCase(">")==0)
	{
		result=(leftValue.Cmp(rightValue)>0);
		if (bothDouble)
		{
			result=(leftDouble>rightDouble);
		}
		if (bothLong)
		{
			result=(leftLong>rightLong);
		}
	}
	else if (operatorString.CmpNoCase("<")==0)
	{
		result=(leftValue.Cmp(rightValue)<0);
		if (bothDouble)
		{
			result=(leftDouble<rightDouble);
		}
		if (bothLong)
		{
			result=(leftLong<rightLong);
		}
	}
	else if (operatorString.CmpNoCase("AND")==0)
	{
		bool	leftSide;
		bool	rightSide;

		leftSide=(leftValue.CmpNoCase(TRUE_STRING)==0);
		rightSide=(rightValue.CmpNoCase(TRUE_STRING)==0);
		result=(leftSide && rightSide);
	}
	else if (operatorString.CmpNoCase("OR")==0)
	{
		bool	leftSide;
		bool	rightSide;

		leftSide=(leftValue.CmpNoCase(TRUE_STRING)==0);
		rightSide=(rightValue.CmpNoCase(TRUE_STRING)==0);
		result=(leftSide || rightSide);
	}
	// Should be done now - 2 variables and an operator
	if (result)
	{
		retVal=TRUE_STRING;
	}
	else
	{
		retVal=FALSE_STRING;
	}
	return (retVal);
}

wxString TemplateVariable::ReadStatistic(wxString &variable)
{
	// Read a basic statistic from the miscellaneous table

	wxString	SQL;
	wxString	value="";
	wxString	queryProperty;
	bool		retVal;

	SQL.Printf("select * from miscellaneous where statistic='%s'",STRING_TO_CHAR(variable));
	TemplateOpenQuery openQuery;
	openQuery.variable="internal";
	openQuery.Initiate(SQL, globalStatistics.statsgenDatabase.DBHandle());
	retVal=openQuery.NextRow();
	if (retVal)
	{
		queryProperty="value";
		value=openQuery.RetrieveProperty(queryProperty);
	}

	return (value);
}

wxString TemplateVariable::FormatDate(wxString &format,wxString &secondssince1970)
{
	wxString	retVal;
	long		seconds;
	wxString	msg;

	seconds=atol(STRING_TO_CHAR(secondssince1970));
	wxDateTime	timeSpan=wxDateTime((time_t) seconds);

	retVal=timeSpan.Format(STRING_TO_CHAR(format));

	return (retVal);
}

wxString TemplateVariable::MessageCentre(wxArrayString &parameters)
{
	wxString		retVal="";
	wxString		key;
	wxArrayString	values;
	wxString		group;
	wxString		configKey;
	wxString		configValue;

	values=parameters;
	key=values.Item(0);
	values.RemoveAt(0);

	SafeString(key);
	// Make sure a Format string has been stored
	configKey.Printf("/MessageCentreFormats/%s",STRING_TO_CHAR(key));
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"");

	// Now write all the values associated with it
	group="MessageCentreValues";
	globalStatistics.configData.WriteList(group,key,values);

	return (retVal);
}

wxString TemplateVariable::Case(wxArrayString &parameters)
{
	wxString	retVal="";
	int			parameterCount;
	int			parameterIndex;
	wxString	token;
	wxString	key;
	wxString	value;
	wxString	matchValue;

	parameterCount=parameters.GetCount();
	for (parameterIndex=0;parameterIndex<parameterCount;parameterIndex++)
	{
		token=parameters.Item(parameterIndex);
		if (parameterIndex==0)
		{
			matchValue=token;
		}
		else
		{
			if ((parameterIndex % 2) == 0)
			{
				// 2, 4, 6
				// value
				value=token;
				if (key.Cmp(matchValue)==0)
				{
					retVal=value;
				}
			}
			else
			{
				// 1, 3, 5
				// key
				key=token;
			}
		}
	}
	
	return (retVal);
}

wxString TemplateVariable::FormatFloat(wxString &format,wxString &floatStr)
{
	wxString	retVal;
	float		number;
	wxString	msg;

	number=atof(STRING_TO_CHAR(floatStr));
	retVal.Printf(STRING_TO_CHAR(format),number);

	return (retVal);
}

wxString TemplateVariable::UpdateStatus(wxArrayString &parameters)
{
	wxString	retVal="";
	int			parameterCount;
	wxString	token;

	parameterCount=parameters.GetCount();
	if (parameterCount>0)
	{
		token=parameters.Item(0);
		progress->SetStatus(token);
	}
	
	return (retVal);
}

wxString TemplateVariable::ReadTemplateConfig(wxArrayString &parameters)
{
	wxString	retVal="";
	int			parameterCount;
	wxString	configKey="";
	wxString	configValue="";
	wxString	defaultValue="";
	wxString	variableHelp="";
	wxString	helpConfigKey;

	parameterCount=parameters.GetCount();
	switch (parameterCount)
	{
		case 0:
			// no parameters supplied
			break;
		case 1:
			configKey=parameters.Item(0);
			configKey="/TemplateConfigs/"+configKey;
			globalStatistics.configData.ReadTextValue(configKey,&configValue);
			break;
		default:
			// at least 2
			configKey=parameters.Item(0);
			helpConfigKey="/TemplateConfigsHelp/"+configKey;
			configKey="/TemplateConfigs/"+configKey;
			defaultValue=parameters.Item(1);
			globalStatistics.configData.ReadTextValue(configKey,
													&configValue,
													defaultValue);
			if (parameterCount == 3)
			{
				variableHelp=parameters.Item(2);
				globalStatistics.configData.WriteTextValue(helpConfigKey,variableHelp);
			}
			break;
	}
	
	retVal=configValue;
	return (retVal);
}

wxString TemplateVariable::ReadConfig(wxArrayString &parameters)
{
	wxString	retVal="";
	int			parameterCount;
	wxString	configKey="";
	wxString	configValue="";
	wxString	defaultValue="";

	parameterCount=parameters.GetCount();
	switch (parameterCount)
	{
		case 0:
			// no parameters supplied
			break;
		case 1:
			configKey=parameters.Item(0);
			globalStatistics.configData.ReadTextValue(configKey,&configValue);
			break;
		default:
			// at least 2
			configKey=parameters.Item(0);
			defaultValue=parameters.Item(1);
			globalStatistics.configData.ReadTextValue(configKey,
													&configValue,
													defaultValue);
			break;
	}
	
	retVal=configValue;
	return (retVal);
}

wxString TemplateVariable::WriteConfig(wxArrayString &parameters)
{
	wxString	retVal="";
	int			parameterCount;
	wxString	configKey="";
	wxString	configValue="";
	wxString	defaultValue="";

	parameterCount=parameters.GetCount();
	switch (parameterCount)
	{
		case 0:
		case 1:
			// not enough parameters supplied
			break;
		default:
			configKey=parameters.Item(0);
			configValue=parameters.Item(1);
			globalStatistics.configData.WriteTextValue(configKey,configValue);
			break;
	}
	
	retVal="";
	return (retVal);
}

wxString TemplateVariable::SimpleSQL(wxArrayString &parameters)
{
	wxString	retVal="";
	int			parameterCount;
	int			parameterIndex;
	wxString	token;
	wxString	SQL;

	SQL="";
	parameterCount=parameters.GetCount();
	for (parameterIndex=0;parameterIndex<parameterCount;parameterIndex++)
	{
		token=parameters.Item(parameterIndex);
		SQL+=token;
	}
	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	
	return (retVal);
}

void TemplateVariable::SetTemplateFile(TemplateFile *templateFileIn)
{
	templateFile = templateFileIn;
}

void TemplateVariable::ExecuteCustomProcedure(wxString &procedureName,TemplateValueList *parameterValues)
{
	STATSGEN_DEBUG_FUNCTION_START("TemplateVariable","ExecuteCustomProcedure")
	CompiledTemplateLinePROCEDURE	*procedure;
	wxString						msg;
	wxString						oldProcedurePrefix;

	STATSGEN_DEBUG(DEBUG_RARELY,procedureName);
	if (templateFile == NULL)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Doh!, I haven't set the template file")
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"template file set, trying to find procedure")
		procedure = templateFile->FindCustomProcedure(procedureName);
		if (procedure != NULL)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Found It");
			oldProcedurePrefix=currentProcedurePrefix;
			currentProcedurePrefix.Printf("procedure%s",STRING_TO_CHAR(procedureName));
			procedure->Execute(templateFile,*this,parameterValues);
			currentProcedurePrefix=oldProcedurePrefix;
		}
		else
		{
			msg.Printf("trying to execute non-existant procedure [%s]",STRING_TO_CHAR(procedureName));
			progress->LogError(msg,SeverityCaution);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

bool TemplateVariable::StoreOpenQuery(wxString &variableOrg,TemplateOpenQuery *openQuery)
{
	wxString				variable;

	variable=variableOrg;
	variable.Replace("self_",currentProcedurePrefix);
	openQuery->variable=variable;
	openQueries.Add(openQuery);

	return (true);
}

TemplateValue::TemplateValue()
{
	value = "";
	query = NULL;
}

TemplateValue::~TemplateValue()
{
	Unload();
}

void TemplateValue::Unload()
{
	// No need to unload - all queries are links to existing ones
}

bool TemplateValue::IsQuery()
{
	return (query!=NULL);
}

void TemplateValue::SetQuery(TemplateOpenQuery *queryIn)
{
	query=queryIn;
}

void TemplateValue::SetString(wxString &valueIn)
{
	value=valueIn;
}

TemplateOpenQuery *TemplateValue::GetQuery()
{
	return (query);
}

wxString TemplateValue::GetString()
{
	return (value);
}

TemplateValueList::TemplateValueList()
{
}


TemplateValueList::~TemplateValueList()
{
	Unload();
}

void TemplateValueList::Add(TemplateValue *value)
{
	values.Add(value);
}

int TemplateValueList::GetCount()
{
	return (values.GetCount());
}

TemplateValue *TemplateValueList::Item(int index)
{
	TemplateValue *retVal=NULL;
	if ((index >=0) && (index < GetCount()))
	{
		retVal=(TemplateValue *)values.Item(index);
	}

	return (retVal);
}

void TemplateValueList::Unload()
{
	TemplateValue	*value;
	int				count;
	int				index;

	count=GetCount();
	for (index=0;index<count;index++)
	{
		value = Item(index);
		delete value;
	}

	values.Clear();
}
