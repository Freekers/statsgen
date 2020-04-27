// wx includes
#include <wx/log.h>

// Statsgen Includes
#include "ProgressPanel.h"
#include "StatsgenLog.h"
#include "StatusPanel.h"
#include "ErrorData.h"
#include "WindowIDs.h"
#include "GlobalStatistics.h"

StatusPanel::StatusPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","StatusPanel")
	mUpdateDisabled	=false;
	mPanelIDs.Clear();
	mPanelPtrs.Clear();
	mDynamicPanel	=NULL;
	SetClone(NULL);
	wxLog::SetActiveTarget(&mLogger);
	STATSGEN_DEBUG_FUNCTION_END
}

StatusPanel::~StatusPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","~StatusPanel")
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::DisableUpdate()
{
	mUpdateDisabled = true;
}

void StatusPanel::SetDynamicPanel(ProgressPanel *panelIn)
{
	mDynamicPanel = panelIn;
}

void StatusPanel::ChoosePanel(int windowID)
{
	ProgressPanel	*panel;

	STATSGEN_DEBUG_UPDATE_DEBUG_LEVEL(windowID)
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","ChoosePanel")
	STATSGEN_DEBUG(DEBUG_ALWAYS,STRING_TO_CHAR(PanelName(windowID)))
	if (!mUpdateDisabled)

	{
		mCurrentWindowID=windowID;
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->Reset();
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::UpdateColouredBar()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","UpdateColouredBar")
	ProgressPanel	*panel;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			{
				panel->UpdateColouredBar();
			}
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::UpdateLabel(const char *label)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","UpdateLabel")
	ProgressPanel	*panel;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->SetLabel(label);
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::Initiate(long topValue,
						const char *units,
						long scale,
						const char *rateUnits,
						long rateScale)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","Initiate")
	ProgressPanel	*panel;
	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			{
				panel->Initiate(topValue,units,scale,rateUnits,rateScale);
				UpdateClone(panel);
			}
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::WakeUp()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","Wakeup")
	ProgressPanel	*panel;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->WakeUp();
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::Update(long value)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","Update")
	ProgressPanel	*panel;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->Update(value);
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::Finalise()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","Finalise")
	ProgressPanel	*panel;


	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->Finalise();
			mCurrentWindowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

long StatusPanel::CurrentValue()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","CurrentValue")
	ProgressPanel	*panel;
	long			result;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			result = panel->CurrentValue();
		}
		else
		{
			result = (long)0;
		}
	}
	else
	{
		result = (long)0;
	}

	STATSGEN_DEBUG_FUNCTION_END
	return result;
}

void StatusPanel::SetOffset(long value)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","SetOffset")
	ProgressPanel	*panel;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->SetOffset(value);
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::CreatePanels()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","CreatePanels")
	ProgressPanel	*panel;
	int				windowID;
	int				panelCount;
	wxSize			maxSize;
	wxSize			descriptionSize;
	int				windowIndex;

	if (!mUpdateDisabled)
	{
		panelCount=mPanelIDs.GetCount();
		if (panelCount>0)
		{
			STATSGEN_DEBUG_FUNCTION_END
			return;
		}

		mCurrentWindowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;

		mMainSizer = new wxBoxSizer(wxVERTICAL);
		for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
			windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
			windowID++)
		{
			panel=new ProgressPanel();
			panel->Create(this,windowID,wxDefaultPosition,wxDefaultSize,
				wxRAISED_BORDER);

			panel->SetLabel(PanelName(windowID));
			mPanelIDs.Add(windowID);
			mPanelPtrs.Add((void *)panel);
			mMainSizer->Add(panel,1,wxEXPAND);
			descriptionSize = panel->GetDescriptionSize();
			if (maxSize.GetWidth()<descriptionSize.GetWidth())
			{
				maxSize.SetWidth(descriptionSize.GetWidth());
			}
			if (maxSize.GetHeight()<descriptionSize.GetHeight())
			{
				maxSize.SetHeight(descriptionSize.GetHeight());
			}
		}
		windowIndex = 0;
		for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
			windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
			windowID++)
		{
			panel = (ProgressPanel *)mPanelPtrs[windowIndex];
			panel->SetDescriptionMinimumSize(maxSize);
			windowIndex++;
		}

		mMainSizer->SetSizeHints(this);
		SetSizer(mMainSizer);

	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::EnableTimeToGo()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","EnableTimeToGo")
	ProgressPanel	*panel;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->EnableTimeToGo();
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::DisableTimeToGo()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","DisableTimeToGo")
	ProgressPanel	*panel;

	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->DisableTimeToGo();
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::SetStatus(wxString &statusText)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","SetStatus")
	ProgressPanel	*panel;


	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			{
				panel->SetStatus(statusText);
				UpdateClone(panel);
			}
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::ClearErrors()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","ClearErrors")
	ProgressPanel	*panel;
	int				windowID;

	if (!mUpdateDisabled)
	{
		for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
			windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
			windowID++)
		{
			panel=(ProgressPanel *)mPanelPtrs.Item(windowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
			panel->ClearErrors();
			UpdateClone(panel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::LogError(wxString &errorText,int severity)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","LogError")
	ProgressPanel	*panel;


	if (!mUpdateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			{
				panel->LogError(errorText,severity);
				UpdateClone(panel);
			}
		}
		else
		{
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

ProgressPanel *StatusPanel::WhichPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","WhichPanel")
	ProgressPanel	*panel=NULL;

	if (!mUpdateDisabled)
	{
		if (mCurrentWindowID==WINDOW_ID_PROGRESS_PANEL_DYNAMIC)
		{
			panel=mDynamicPanel;
		}
		else
		{
			panel=(ProgressPanel *)mPanelPtrs.Item(mCurrentWindowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (panel);
}

wxString StatusPanel::PanelName(int windowID)
{

	wxString	panelName="";


	switch (windowID)
	{
		case WINDOW_ID_PROGRESS_PANEL_GENERAL:
			panelName="General";
			break;
		case WINDOW_ID_PROGRESS_PANEL_APPLY_ALIASES:
			panelName="Apply Aliases";
			break;
		case WINDOW_ID_PROGRESS_PANEL_DOWNLOAD_LOGS:
			panelName="Download Logs";
			break;
		case WINDOW_ID_PROGRESS_PANEL_DATABASE_READ_STATIC:
			panelName="Read Static";
			break;
		case WINDOW_ID_PROGRESS_PANEL_PROCESS_LOGS:
			panelName="Process Logs";
			break;
		case WINDOW_ID_PROGRESS_PANEL_DROPPING_PLAYERS:
			panelName="Dropping Players";
			break;
		case WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES1:
			panelName="Calculating Scores 1";
			break;
		case WINDOW_ID_PROGRESS_PANEL_CALCULATING_SCORES2:
			panelName="Calculating Scores 2";
			break;
		case WINDOW_ID_PROGRESS_PANEL_CREATE_DATABASE:
			panelName="Create Database";
			break;
		case WINDOW_ID_PROGRESS_PANEL_EMPTY_DATABASE:
			panelName="Empty Database";
			break;
		case WINDOW_ID_PROGRESS_PANEL_DATABASE_LOCATE_EXISTING_PLAYERS:
			panelName="Locate Existing Players";
			break;
		case WINDOW_ID_PROGRESS_PANEL_WRITE_DATABASE:
			panelName="Write Database";
			break;
		case WINDOW_ID_PROGRESS_PANEL_TRANSFER_DATABASE:
			panelName="Transfer Database";
			break;
		case WINDOW_ID_PROGRESS_PANEL_READ_TEMPLATE:
			panelName="Read Template";
			break;
		case WINDOW_ID_PROGRESS_PANEL_PROCESS_TEMPLATE:
			panelName="Process Template";
			break;
		case WINDOW_ID_PROGRESS_PANEL_WEBSITE:
			panelName="Transfer To Website";
			break;
	}

	return (panelName);
}

void StatusPanel::SetStatus(const char *statusTextIn)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","SetStatus")
	wxString statusText;

	statusText=statusTextIn;
	if (!mUpdateDisabled)
	{
		SetStatus(statusText);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

bool StatusPanel::StageRan(int windowID)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","StageRan")
	wxString			SQL;
	wxString			panelName;
	TemplateOpenQuery	query;
	int					counter;
	int					retVal;

	panelName=PanelName(windowID);

	SQL.Printf("select * from progressbar where stage='%s' and status like '\% done'",
				STRING_TO_CHAR(panelName));
	STATSGEN_DEBUG(DEBUG_ALWAYS,SQL);
	query.Initiate(SQL,globalStatistics.statsgenDatabase.DBHandle());
	retVal=query.NextRow();
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"stage ran");
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"stage not ran");
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool StatusPanel::RecoveryMode()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","RecoveryMode")
	bool retVal=false;
	bool writeDatabaseRan;
	bool processTemplateRan;

	writeDatabaseRan=StageRan(WINDOW_ID_PROGRESS_PANEL_WRITE_DATABASE);
	processTemplateRan=StageRan(WINDOW_ID_PROGRESS_PANEL_PROCESS_TEMPLATE);

	retVal=writeDatabaseRan && (!processTemplateRan);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"recovery mode");
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"not in recovery");
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void StatusPanel::StoreProgress(wxString &filename)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","StoreProgress")
	FILE				*fp;
	TemplateOpenQuery	query;
	wxString			SQL="select * from progressbar order by logtime,stage,statusindex";
	wxString			stage;
	wxString			status;
	wxString			logtime;
	wxString			severity;
	wxString			error;
	wxString			propertyStage="stage";
	wxString			propertyLogtime="logtime";
	wxString			propertyStatus="status";
	wxString			propertySeverity="severity";
	wxString			propertyError="error";

	wxString			lineToWrite;

	fp=fopen(STRING_TO_CHAR(filename),"w");
	if (fp!=NULL)
	{
		globalStatistics.statsgenDatabase.OpenDB();
		query.Initiate(SQL,globalStatistics.statsgenDatabase.DBHandle());
		while (query.NextRow())
		{
			stage=query.RetrieveProperty(propertyStage);
			logtime=query.RetrieveProperty(propertyLogtime);
			wxDateTime	dateTime((time_t)atoi(STRING_TO_CHAR(logtime)));
			logtime=dateTime.FormatISODate() + " " + dateTime.FormatISOTime();
			status=query.RetrieveProperty(propertyStatus);
			severity=query.RetrieveProperty(propertySeverity);
			error=query.RetrieveProperty(propertyError);

			lineToWrite.Printf("%24s %14s %10s %30s %s",
				STRING_TO_CHAR(stage),
				STRING_TO_CHAR(logtime),
				STRING_TO_CHAR(status),
				STRING_TO_CHAR(severity),
				STRING_TO_CHAR(error)
						);
			fprintf(fp,"%s\n",STRING_TO_CHAR(lineToWrite));
		}
		fclose(fp);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void StatusPanel::SetClone(ProgressPanel *clonePanelIn)
{
	mClonePanel = clonePanelIn;
}

void StatusPanel::UpdateClone(ProgressPanel *sourcePanel)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","UpdateClone")
	if (mClonePanel!=NULL)
	{
		if (sourcePanel!=NULL)
		{
			sourcePanel->UpdateClone(mClonePanel);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}


bool StatusPanel::GetEventUpdating()
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","GetEventUpdating")
	ProgressPanel	*panel;
	bool			result;
	panel=(ProgressPanel *)mPanelPtrs.Item(0);
	result =  panel->GetEventUpdating();

	STATSGEN_DEBUG_FUNCTION_END
	return result;
}

void StatusPanel::EventUpdating(bool value)
{
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","EventUpdating")
	ProgressPanel	*panel;
	int				windowID;


	for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
		windowID++)
	{
		panel=(ProgressPanel *)mPanelPtrs.Item(windowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		panel->EventUpdating(value);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

