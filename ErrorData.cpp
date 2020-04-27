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
	globalStatistics.configData.ReadTextValue(configKey,&debugFilename,"");

	debugMaxLevel.Clear();
	for (debugIndex=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		debugIndex<WINDOW_ID_PROGRESS_PANEL_MAX;
		debugIndex++)
	{
		configKey.Printf("/Debug/DebugLevel%d",
				debugIndex-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		

		globalStatistics.configData.ReadTextValue(configKey,&configValue,"0");
		debugMaxLevel.Add(atoi(configValue.GetData()));
	}
	initiateDebugFile=true;
	currentDebugLevel=0;
}

bool StatsgenDebugObject::DebugOn()
{
	return ((currentDebugLevel!=0)&&(debugFilename.Length()!=0));
}

void StatsgenDebugObject::Debug(const char *classname,const char *functionname,
			int debugLevel,const char *message)
{
	if ((currentDebugLevel!=0)&&(strlen(debugFilename.GetData())!=0))
	{
		// debug enabled
		if (currentDebugLevel>=debugLevel)
		{
			FILE *fp;
			if (initiateDebugFile)
			{
				fp=fopen(debugFilename.GetData(),"w");
				initiateDebugFile=false;
			}
			else
			{
				fp=fopen(debugFilename.GetData(),"a");
			}
			if (fp!=NULL)
			{
				fprintf(fp,"%s::%s():%s\n",
					classname,functionname,message);
				fclose(fp);
			}
		}
	}
}

void StatsgenDebugObject::Debug(const char *classname,const char *functionname,
			int debugLevel,wxString &message)
{
	Debug(classname,functionname,debugLevel,message.GetData());
}

void StatsgenDebugObject::Entering(const char *classname,const char *functionname)
{
	Debug(classname,functionname,DEBUG_SOMETIMES,"Entering");
}

void StatsgenDebugObject::Leaving(const char *classname,const char *functionname)
{
	Debug(classname,functionname,DEBUG_SOMETIMES,"Leaving");
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


