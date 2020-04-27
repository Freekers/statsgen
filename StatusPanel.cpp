// wx includes
#include <wx/log.h>

// Statsgen Includes
#include "ProgressPanel.h"
#include "StatsgenLog.h"
#include "StatusPanel.h"
#include "ErrorData.h"
#include "WindowIDs.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(StatusPanel, wxPanel)
		EVT_SIZE(StatusPanel::OnResize)
END_EVENT_TABLE()

StatusPanel::StatusPanel()
{
	updateDisabled=false;
	panelIDs.Clear();
	panelPtrs.Clear();
	dynamicPanel=NULL;
	SetClone(NULL);
	wxLog::SetActiveTarget(&logger);
}

StatusPanel::~StatusPanel()
{
}

void StatusPanel::DisableUpdate()
{
	updateDisabled=true;
}

void StatusPanel::SetDynamicPanel(ProgressPanel *panelIn)
{
	dynamicPanel=panelIn;
}

void StatusPanel::ChoosePanel(int windowID)
{
	ProgressPanel	*panel;

	STATSGEN_DEBUG_UPDATE_DEBUG_LEVEL(windowID)
	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","ChoosePanel")
	STATSGEN_DEBUG(DEBUG_ALWAYS,PanelName(windowID))
	if (!updateDisabled)

	{
		currentWindowID=windowID;
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
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			{
				panel->UpdateColouredBar();
			}
		}
	}

}

void StatusPanel::UpdateLabel(const char *label)
{
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
	panel=WhichPanel();
	if (panel!=NULL)
	{
		{
			panel->SetLabel(label);
			NormaliseWidths();
			UpdateClone(panel);
		}
	}
	}
}

void StatusPanel::NormaliseWidths()
{
	ProgressPanel	*panel;
	int				labelWidth;
	int				maxLabelWidth;
	int				windowID;


	if (!updateDisabled)
	{
	maxLabelWidth=0;
	for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
		windowID++)
	{
		panel=(ProgressPanel *)panelPtrs.Item(windowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		labelWidth=panel->LabelWidth();
		if (labelWidth>maxLabelWidth)
		{
			maxLabelWidth=labelWidth;
		}
		
	}
	for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
		windowID++)
	{
		panel=(ProgressPanel *)panelPtrs.Item(windowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		panel->SetLabelWidth(maxLabelWidth);
		UpdateClone(panel);
	}
	}
}

void StatusPanel::OnResize(wxSizeEvent &event)
{
	wxString		msg;

	wxSize			itemSize;
	int				gap=3;
	int				windowID;
	int				currentY;
	int				width;
	int				maxWidth;
	ProgressPanel	*panel;
	
	if (!updateDisabled)
	{
	CreatePanels();
	currentY=0;
	maxWidth=0;
	for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
		windowID++)
	{
		panel=(ProgressPanel *)panelPtrs.Item(windowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		itemSize=panel->GetSize();
		panel->SetSize(0,currentY,itemSize.GetWidth(),itemSize.GetHeight());
		currentY+=itemSize.GetHeight();
		width=itemSize.GetWidth();
		if (width>maxWidth)
		{
			maxWidth=width;
		}
		currentY+=gap;
	}

	SetSize(maxWidth,currentY);
	}
}

void StatusPanel::Initiate(long topValue,
						const char *units,
						long scale,
						const char *rateUnits,
						long rateScale)
{
	ProgressPanel	*panel;
	if (!updateDisabled)
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
}

void StatusPanel::WakeUp()
{
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
	panel=WhichPanel();
	if (panel!=NULL)
	{
		{
		{
			panel->WakeUp();
			UpdateClone(panel);
		}
		}
	}
	}
}

void StatusPanel::Update(long value)
{
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
	panel=WhichPanel();
	if (panel!=NULL)
	{
		{
		{
			panel->Update(value);
			UpdateClone(panel);
		}
		}
	}
	}
}

void StatusPanel::Finalise()
{
	ProgressPanel	*panel;


	if (!updateDisabled)
	{
	panel=WhichPanel();
	if (panel!=NULL)
	{
		{
			panel->Finalise();
			currentWindowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
			UpdateClone(panel);
		}
	}
	}
}

long StatusPanel::CurrentValue()
{
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			return(panel->CurrentValue());
		}
		else
		{
			return ((long)0);
		}
	}

	return ((long)0);
}

void StatusPanel::SetOffset(long value)
{
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
		panel=WhichPanel();
		if (panel!=NULL)
		{
			panel->SetOffset(value);
			UpdateClone(panel);
		}
	}
}

void StatusPanel::CreatePanels()
{
	ProgressPanel	*panel;
	int				windowID;
	int				panelCount;

	if (!updateDisabled)
	{
	panelCount=panelIDs.GetCount();
	if (panelCount>0)
	{
		return;
	}

	currentWindowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;

	for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
		windowID++)
	{
		panel=new ProgressPanel();
		panel->Create(this,windowID,wxDefaultPosition,wxDefaultSize,
			wxRAISED_BORDER);

		panel->SetLabel(PanelName(windowID));
		panelIDs.Add(windowID);
		panelPtrs.Add((void *)panel);
	}

	NormaliseWidths();
	}
}

void StatusPanel::EnableTimeToGo()
{
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
	panel=WhichPanel();
	if (panel!=NULL)
	{
		panel->EnableTimeToGo();
		UpdateClone(panel);
	}
	}
}

void StatusPanel::DisableTimeToGo()
{
	ProgressPanel	*panel;

	if (!updateDisabled)
	{
	panel=WhichPanel();
	if (panel!=NULL)
	{
		panel->DisableTimeToGo();
		UpdateClone(panel);
	}
	}
}

void StatusPanel::SetStatus(wxString &statusText)
{
	ProgressPanel	*panel;


	if (!updateDisabled)
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
}

void StatusPanel::ClearErrors()
{
	ProgressPanel	*panel;
	int				windowID;

	if (!updateDisabled)
	{
	for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
		windowID++)
	{
		panel=(ProgressPanel *)panelPtrs.Item(windowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		panel->ClearErrors();
		UpdateClone(panel);
	}
	}
}

void StatusPanel::LogError(wxString &errorText,int severity)
{
	ProgressPanel	*panel;


	if (!updateDisabled)
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
}

ProgressPanel *StatusPanel::WhichPanel()
{
	ProgressPanel	*panel=NULL;

	if (!updateDisabled)
	{
	if (currentWindowID==WINDOW_ID_PROGRESS_PANEL_DYNAMIC)
	{
		panel=dynamicPanel;
	}
	else
	{
		panel=(ProgressPanel *)panelPtrs.Item(currentWindowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
	}
	}
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
	wxString statusText;

	statusText=statusTextIn;
	if (!updateDisabled)
	{
	SetStatus(statusText);
	}
}

bool StatusPanel::StageRan(int windowID)
{
	wxString			SQL;
	wxString			panelName;
	TemplateOpenQuery	query;
	int					counter;
	int					retVal;

	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","StageRan")
	panelName=PanelName(windowID);

	SQL.Printf("select * from progressbar where stage='%s' and status like '\% done'",
				panelName.GetData());
	STATSGEN_DEBUG(DEBUG_ALWAYS,SQL);
	query.Initiate(SQL,globalStatistics.statsgenDatabase.DBHandle());
	retVal=query.NextRow();
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"stage ran");
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"stage not ran");
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool StatusPanel::RecoveryMode()
{
	bool retVal=false;
	bool writeDatabaseRan;
	bool processTemplateRan;

	STATSGEN_DEBUG_FUNCTION_START("StatusPanel","RecoveryMode")
	writeDatabaseRan=StageRan(WINDOW_ID_PROGRESS_PANEL_WRITE_DATABASE);
	processTemplateRan=StageRan(WINDOW_ID_PROGRESS_PANEL_PROCESS_TEMPLATE);

	retVal=writeDatabaseRan && (!processTemplateRan);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"recovery mode");
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"not in recovery");
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void StatusPanel::StoreProgress(wxString &filename)
{
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

	fp=fopen(filename.GetData(),"w");
	if (fp!=NULL)
	{
		globalStatistics.statsgenDatabase.OpenDB();
		query.Initiate(SQL,globalStatistics.statsgenDatabase.DBHandle());
		while (query.NextRow())
		{
			stage=query.RetrieveProperty(propertyStage);
			logtime=query.RetrieveProperty(propertyLogtime);
			wxDateTime	dateTime((time_t)atoi(logtime.GetData()));
			logtime=dateTime.FormatISODate() + " " + dateTime.FormatISOTime();
			status=query.RetrieveProperty(propertyStatus);
			severity=query.RetrieveProperty(propertySeverity);
			error=query.RetrieveProperty(propertyError);

			lineToWrite.Printf("%24s %14s %10s %30s %s",
				stage.GetData(),
				logtime.GetData(),
				status.GetData(),
				severity.GetData(),
				error.GetData()
						);
			fprintf(fp,"%s\n",lineToWrite.GetData());
		}
		fclose(fp);
	}
}

void StatusPanel::SetClone(ProgressPanel *clonePanelIn)
{
	clonePanel=clonePanelIn;
}

void StatusPanel::UpdateClone(ProgressPanel *sourcePanel)
{
	if (clonePanel!=NULL)
	{
		if (sourcePanel!=NULL)
		{
			sourcePanel->UpdateClone(clonePanel);
		}
	}
}


bool StatusPanel::GetEventUpdating()
{
	ProgressPanel	*panel;
	panel=(ProgressPanel *)panelPtrs.Item(0);
	return (panel->GetEventUpdating());
}

void StatusPanel::EventUpdating(bool value)
{
	ProgressPanel	*panel;
	int				windowID;


	for (windowID=WINDOW_ID_PROGRESS_PANEL_GENERAL;
		windowID<WINDOW_ID_PROGRESS_PANEL_MAX;
		windowID++)
	{
		panel=(ProgressPanel *)panelPtrs.Item(windowID-WINDOW_ID_PROGRESS_PANEL_GENERAL);
		panel->EventUpdating(value);
	}
}

