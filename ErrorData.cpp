// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>

// statsgen includes
#include "ErrorData.h"
#include "GlobalStatistics.h"

StatsgenDebugObject	statsgenDebug;

StatsgenDebugObject::StatsgenDebugObject()
{
	initiateDebugFile=true;
	currentDebugLevel=0;

}

void StatsgenDebugObject::UpdateDebugConfig()
{
	wxString	configKey;
	int		debugIndex;
	wxString	configValue;
	
	configKey="/Debug/DebugFile";
	globalStatistics.configData.ReadTextValue(configKey,&debugFilename,(char *)"");

	debugMaxLevel.Clear();
	for (debugIndex=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		debugIndex<WINDOW_ID_PROGRESS_PANEL_MAX;
		debugIndex++)
	{
		configKey.Printf("/Debug/DebugLevel%d",
				debugIndex-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		

		globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"0");
		debugMaxLevel.Add(atoi(STRING_TO_CHAR(configValue)));
	}
	initiateDebugFile=true;
	currentDebugLevel=0;
}

bool StatsgenDebugObject::DebugOn()
{
	return ((currentDebugLevel!=0)&&(debugFilename.Length()!=0));
}

void StatsgenDebugObject::Debug(wxString classname,wxString functionname,
			int debugLevel,char *message)
{
	wxString messageStr = message;
	Debug(classname,functionname, debugLevel,messageStr);
}
void StatsgenDebugObject::Debug(wxString classname,wxString functionname,
			int debugLevel,wxString message)
{
	if ((currentDebugLevel!=0)&&(strlen(STRING_TO_CHAR(debugFilename))!=0))
	{
		// debug enabled
		if (currentDebugLevel>=debugLevel)
		{
			FILE *fp;
			if (initiateDebugFile)
			{
				fp=fopen(STRING_TO_CHAR(debugFilename),"w");
				initiateDebugFile=false;
			}
			else
			{
				fp=fopen(STRING_TO_CHAR(debugFilename),"a");
			}
			if (fp!=NULL)
			{
				fprintf(fp,"%s::%s():%s\n",
					STRING_TO_CHAR(classname),STRING_TO_CHAR(functionname),STRING_TO_CHAR(message));
				fclose(fp);
			}
		}
	}
}

void StatsgenDebugObject::Entering(wxString classname,wxString functionname)
{
	wxString	message="Entering";
	Debug(classname,functionname,DEBUG_SOMETIMES,message);
}

void StatsgenDebugObject::Leaving(wxString classname,wxString functionname)
{
	wxString	message="Leaving";
	Debug(classname,functionname,DEBUG_SOMETIMES,message);
}

void StatsgenDebugObject::UpdateCurrentDebugLevel(int progressPanelID)
{
	if (progressPanelID<WINDOW_ID_PROGRESS_PANEL_GENERAL)
	{
		progressPanelID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
	}
	if (progressPanelID>=WINDOW_ID_PROGRESS_PANEL_MAX)
	{
		progressPanelID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
	}

	currentDebugLevel=debugMaxLevel.Item(progressPanelID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
}

ErrorData::ErrorData()
{
	ClearError();
}

void ErrorData::ClearError()
{
	errorCode=ERROR_CODE_OK;
}

ErrorCodes ErrorData::CurrentError()
{
	return (errorCode);
}

bool ErrorData::IsOK()
{
	return (errorCode==ERROR_CODE_OK);
}

void ErrorData::SetError(ErrorCodes error,char *description)
{
	errorCode=error;
	errorDescription=description;
}

void ErrorData::SetError(ErrorCodes error,wxString &description)
{
	errorCode=error;
	errorDescription=description;
}


