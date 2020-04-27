// wx includes
#include "wx/artprov.h"
// Statsgen Includes
#include "ProgressPanel.h"
#include "ErrorPanel.h"
#include "ErrorData.h"
#include "Progress.h"
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(ProgressPanel, wxPanel)
		EVT_BUTTON(WINDOW_ID_ERROR_BUTTON,ProgressPanel::OnButtonPressed)
END_EVENT_TABLE()

ProgressPanel::ProgressPanel()
{

	mTimeRemaining		=NULL;
	mTimeSpent			=NULL;
	mCounter			=NULL;
	mDescription		=NULL;
	mStatus				=NULL;
	mMaxValue			=-1;
	mScale				=1;
	mCurrentValue		=0;
	mOffset				=0;
	mPreviousTime		=0;
	EnableTimeToGo();
	mCounterEnabled		=true;
	mTimeStepsCreated	=false;
	mStoringError		=false;
	mStatusIndex		=0;

	//EventUpdating(true);
	EventUpdating(false);

}

void ProgressPanel::Reset()
{
	mStartTime=time(&mStartTime);
	mCurrentValue=0;
	mOffset=0;
	mTimeStepsCreated=0;
	mStatusIndex=0;
}

void ProgressPanel::CreateScreen()
{
	wxString		label="";
	wxSize	buttonSize(16,16);
	wxSize	panelSize;

	if (mTimeRemaining==NULL)
	{
		mTimeRemaining=new wxStaticText(this,wxID_ANY,label);
		mTimeSpent=new wxStaticText(this,wxID_ANY,label);
		mCounter=new wxStaticText(this,wxID_ANY,label);
		mDescription=new wxStaticText(this,wxID_ANY,label);
		mRate=new wxStaticText(this,wxID_ANY,label);
		mStatus=new wxStaticText(this,GetId(),label);
		mResultButton=new wxBitmapButton(this,WINDOW_ID_ERROR_BUTTON,wxArtProvider::GetIcon(wxART_ERROR,wxART_OTHER,buttonSize));
		ClearErrors();


		mMainSizer	= new wxBoxSizer(wxHORIZONTAL);
		mMainSizer->Add(mDescription,	125,	wxEXPAND);
		mMainSizer->Add(mCounter,		125,	wxEXPAND);
		mMainSizer->Add(mTimeSpent,		50,		wxEXPAND);
		mMainSizer->Add(mTimeRemaining,	50,		wxEXPAND);
		mMainSizer->Add(mRate,			125,	wxEXPAND);
		mMainSizer->Add(mStatus,		250,	wxEXPAND);
		mMainSizer->Add(mResultButton);
		
		// As labels have no size unless they have values in them - i need
		// to set up a minimum size so the gui does not start off squashed
		// this is because i find sizers somewhat still confusing
		// so i am sure there is a better way of doing this
		mDescription->SetLabel((char *)"Locate Existing Players");	mDescription->SetMinSize(mDescription->GetSize());
		mCounter->SetLabel((char *)"1000 rounds");					mCounter->SetMinSize(mCounter->GetSize());
		mTimeSpent->SetLabel((char *)"00:00:00");					mTimeSpent->SetMinSize(mTimeSpent->GetSize());
		mTimeRemaining->SetLabel((char *)"00:00:00");				mTimeRemaining->SetMinSize(mTimeRemaining->GetSize());
		mRate->SetLabel((char *)"1000 rounds / sec");				mRate->SetMinSize(mRate->GetSize());
		mStatus->SetLabel((char *)"open the pod bay doors HAL");	mStatus->SetMinSize(mStatus->GetSize());

		mDescription->SetLabel((char *)"");
		mCounter->SetLabel((char *)"");
		mTimeSpent->SetLabel((char *)"");
		mTimeRemaining->SetLabel((char *)"");
		mRate->SetLabel((char *)"");
		mStatus->SetLabel((char *)"");

		mProgressBarPanel = new wxPanel(this,wxID_ANY);
		mProgressSizer = new wxBoxSizer(wxVERTICAL);
		mProgressSizer->Add(mMainSizer,9,wxEXPAND);
		mProgressSizer->Add(mProgressBarPanel,1,wxEXPAND);
		mProgressSizer->SetSizeHints(this);
		SetSizer(mProgressSizer);
	}
}

ProgressPanel::~ProgressPanel()
{
}

void ProgressPanel::Initiate(
							long maxIn,
							const char *unitsIn,
							long scaleIn,
							const char *rateUnitsIn,
							long rateScaleIn
							)
{

	CreateScreen();

	Reset();
	mUnitText	= unitsIn;
	mMaxValue	= maxIn;
	mScale		= scaleIn;
	mRateScale	= rateScaleIn;
	mRateUnits	= rateUnitsIn;

	
}

void ProgressPanel::SetLabel(const char *label)
{
	CreateScreen();
	mDescription->SetLabel(label);
}

void ProgressPanel::SetOffset(long value)
{
	mOffset		= value;
}

void ProgressPanel::WakeUp()
{
	Update(mCurrentValue-mOffset);
}

void ProgressPanel::UpdateColouredBar()
{
	STATSGEN_DEBUG_FUNCTION_START("ProgressPanel","UpdateColouredBar")
	if (mTimeToGoEnabled)
	{
		wxClientDC	dc(mProgressBarPanel);
		wxPen		currentPen;
		wxBrush		currentBrush;
		wxColour	green(0,255,0);
		wxColour	red(255,0,0);
		wxColour	amber(255,127,0);
		wxPen	greenPen(green);
		wxBrush	greenBrush(green);
		wxPen	redPen(red);
		wxBrush	redBrush(red);
		wxPen	amberPen(amber);
		wxBrush	amberBrush(amber);
		wxBrush	chosenBrush;
		wxPen	chosenPen;

		wxSize	size;
		int		maxWidth;
		int		width;
		int		height;

		float	pixelsPerUnit;

		switch (OverallSeverity())
		{
			case SeverityOK:
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Severity OK");
				chosenBrush=greenBrush;
				chosenPen=greenPen;
				break;
			case SeverityCaution:
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Severity Caution");
				chosenBrush=amberBrush;
				chosenPen=amberPen;
				break;
			case SeverityError:
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Severity Error");
				chosenBrush=redBrush;
				chosenPen=redPen;
				break;
			default:
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Severity Default");
				chosenBrush=greenBrush;
				chosenPen=greenPen;
				break;
		}
		currentPen		=dc.GetPen();
		currentBrush	=dc.GetBrush();

		size=mProgressBarPanel->GetSize();
		maxWidth=size.GetWidth();
		size=GetSize();
		height=size.GetHeight();
		if (mMaxValue<=0)
		{
			pixelsPerUnit=(float)maxWidth;
		}
		else
		{
			pixelsPerUnit=(float)maxWidth/(float)mMaxValue;
		}
		width=(int)(pixelsPerUnit * (float)mCurrentValue);

		dc.SetPen(chosenPen);
		dc.SetBrush(chosenBrush);
		dc.DrawRectangle(0,0,width,height);
		dc.SetPen(currentPen);
		dc.SetBrush(currentBrush);
		dc.DrawRectangle(width,0,maxWidth-width,height);
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void ProgressPanel::Update(long value)
{
	wxString	timeRemainingString;
	wxString	timeSpentString;
	wxString	progressString;
	wxString	rateString;
	wxString	msg;
	long		progressToGo;
	time_t		currentTime;
	time_t		elapsedTime;
	time_t		timeToGo;
	long		minutes;
	long		seconds;
	float		rateValue;
	int			historyIndex;
	
	currentTime		= time(&currentTime);
	elapsedTime		= currentTime-mStartTime;
	mCurrentValue	= value+mOffset;
	if ((elapsedTime != mPreviousTime)/*||(previousTime==0)*/)
	{
		// Add the current time and value to the history list
		// first shuffle the current values down one
		if (mTimeStepsCreated)
		{
			for (historyIndex = 0;historyIndex<(MAX_TIME_STEPS-1);historyIndex++)
			{
				mHistoryTimes[historyIndex] = mHistoryTimes[historyIndex+1];
				mHistoryValues[historyIndex] = mHistoryValues[historyIndex+1];
			}
		}
		else
		{
			for (historyIndex = 0;historyIndex<(MAX_TIME_STEPS-1);historyIndex++)
			{
				mHistoryTimes[historyIndex] = currentTime;
				mHistoryValues[historyIndex] = mCurrentValue;
			}
		}
		mHistoryTimes[MAX_TIME_STEPS-1]		= currentTime;
		mHistoryValues[MAX_TIME_STEPS-1]	= mCurrentValue;
		mPreviousTime		= elapsedTime;
		timeRemainingString	= "???";
		rateString			= "???";
		progressString		= "???";
		progressToGo= mMaxValue-mCurrentValue;
		if (progressToGo<0)
		{
			progressToGo = 0;
		}
		if ((elapsedTime>0)&&(mTimeStepsCreated))
		{

			if (mTimeToGoEnabled)
			{
				time_t		rateElapsedTime;
				long		rateElapsedValue;

				rateElapsedTime		=mHistoryTimes[MAX_TIME_STEPS-1]	- mHistoryTimes[0];
				rateElapsedValue	=mHistoryValues[MAX_TIME_STEPS-1]	- mHistoryValues[0];
				//rateValue=((float)currentValue)/((float)elapsedTime);
				
				if ((rateElapsedTime>0)&&(rateElapsedValue>0))
				{
					rateValue=((float)rateElapsedValue)/((float)rateElapsedTime);
					timeToGo=(long)(((float)progressToGo)/rateValue);
					rateValue/=((float)mRateScale);
					rateString.Printf("%0.1f%s / sec",rateValue,STRING_TO_CHAR(mRateUnits));
					minutes=timeToGo/60;
					seconds=timeToGo % 60;
					timeRemainingString.Printf("%ld:%02ld",minutes,seconds);
					/*
					if (timeRemainingString.StartsWith("-"))
					{
						wxString	outputMsg;

						outputMsg.Printf("rateElapsedTime [%ld] rateElapsedValue [%ld] rateValue [%f] timeToGo [%ld] rateValue [%f] minutes [%ld] seconds [%ld]",
							rateElapsedTime,rateElapsedValue,rateValue,
							timeToGo,rateValue,minutes,seconds);
						wxMessageBox(outputMsg);
						exit(0);
					}
					*/
				}
				else
				{
					timeRemainingString="???";
					rateString.Printf("???%s / sec",STRING_TO_CHAR(mRateUnits));
				}
			}
		}
		mTimeStepsCreated = true;
		if (mTimeToGoEnabled)
		{
			progressToGo/=mScale;
			progressString.Printf("%ld%s",progressToGo,STRING_TO_CHAR(mUnitText));
		}
		else
		{
			progressString.Printf("%ld%s",mCurrentValue/mScale,STRING_TO_CHAR(mUnitText));
		}
		minutes=elapsedTime / 60;
		seconds=elapsedTime % 60;
		timeSpentString.Printf("%ld:%02ld",minutes,seconds);

		if (mTimeToGoEnabled)
		{
			//timeRemaining->SetLabel(timeRemainingString);
			wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,EVENT_ID_PROGRESS_STATUS_TEXT);
			event.SetClientData(mTimeRemaining);
			event.SetString(timeRemainingString);
			if (mEventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				mTimeRemaining->SetLabel(timeRemainingString);
				wxSafeYield();
			}
			event.SetClientData(mRate);
			event.SetString(rateString);
			if (mEventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				mRate->SetLabel(rateString);
				wxSafeYield();
			}
		}
		if (mCounterEnabled)
		{
			wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,EVENT_ID_PROGRESS_STATUS_TEXT);
			//counter->SetLabel(progressString);
			event.SetClientData(mCounter);
			event.SetString(progressString);
			if (mEventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				mCounter->SetLabel(progressString);
				wxSafeYield();
			}
			event.SetClientData(mTimeSpent);
			event.SetString(timeSpentString);
			if (mEventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				mTimeSpent->SetLabel(timeSpentString);
				wxSafeYield();
			}
			//if (mEventUpdating)
			{
				UpdateColouredBar();
			}
		}
		wxSafeYield();
	}
	
}

void ProgressPanel::Finalise()
{
	wxString	errorText="done";

	mPreviousTime=0;
	Update(mCurrentValue-mOffset);
	LogError(errorText,SeverityOK);
	SetStatus(errorText);
}

long ProgressPanel::CurrentValue()
{
	return (mCurrentValue);
}

bool ProgressPanel::GetEventUpdating()
{
	return (mEventUpdating);
}

void ProgressPanel::EnableCounter()
{
	mCounterEnabled=true;
}
void ProgressPanel::DisableCounter()
{
	mCounterEnabled=false;
}
void ProgressPanel::DisableTimeToGo()
{
	mTimeToGoEnabled=false;
}

void ProgressPanel::EnableTimeToGo()
{
	mTimeToGoEnabled=true;
}

void ProgressPanel::SetStatus(wxString &statusText)
{
	STATSGEN_DEBUG_FUNCTION_START("ProgressPanel","SetStatus")
	STATSGEN_DEBUG(DEBUG_ALWAYS,statusText);
	wxString	errorText="";
	int			severity=-1;
	//status->SetLabel(statusText);
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(mStatus);
	event.SetString(statusText);
	if (mEventUpdating)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Posting event");
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Not Posting event");
		mStatus->SetLabel(statusText);
		wxSafeYield();
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Logging To Database");
	LogToDatabase(severity,errorText,statusText);
	STATSGEN_DEBUG_FUNCTION_END
}

wxBitmap ProgressPanel::SeverityImage(int severity)
{
	wxBitmap	bitmap;
	wxSize	buttonSize(16,16);
	switch (severity)
	{
		default:
		case SeverityNotRun:
			bitmap=wxArtProvider::GetIcon(wxART_NORMAL_FILE,wxART_OTHER,buttonSize);
			break;
		case SeverityOK:
			bitmap=wxArtProvider::GetIcon(wxART_TICK_MARK,wxART_OTHER,buttonSize);
			break;
		case SeverityCaution:
			bitmap=wxArtProvider::GetIcon(wxART_INFORMATION,wxART_OTHER,buttonSize);
			break;
		case SeverityError:
			bitmap=wxArtProvider::GetIcon(wxART_ERROR,wxART_OTHER,buttonSize);
			break;
	}

	return (bitmap);
}

void ProgressPanel::SetResultSeverity(int severity)
{
	//wxBitmap	bitmap;

	//bitmap=SeverityImage(severity);
	//resultButton->SetBitmapLabel(bitmap);

	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,EVENT_ID_PROGRESS_BUTTON);
	event.SetClientData(mResultButton);
	event.SetInt(severity);
	if (mEventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		wxBitmap		bitmap=SeverityImage(severity);
		mResultButton->SetBitmapLabel(bitmap);
	}
}

void ProgressPanel::ClearErrors()
{
	wxString	label="";
	wxString	SQL;

	mErrorSeverities.Clear();
	mErrors.Clear();
	SetSeverityIcon();
	mTimeRemaining->SetLabel(label);
	mTimeSpent->SetLabel(label);
	mCounter->SetLabel(label);
	mRate->SetLabel(label);
	mStatus->SetLabel(label);
	mStatusIndex=0;
	if (!mStoringError)
	{
		if (mDescription!=NULL)
		{
			mStoringError=true;
			SQL.Printf("delete from %s where stage='%s'",
				STRING_TO_CHAR(TableName()),
				STRING_TO_CHAR(mDescription->GetLabel()));
			globalStatistics.statsgenDatabase.SimpleExecute(SQL);
			mStoringError=false;
		}
	}
}

int ProgressPanel::OverallSeverity()
{
	int	severity=SeverityNotRun;
	int	severityCount;
	int	thisSeverity;
	int	severityIndex;

	severityCount=mErrorSeverities.GetCount();
	for (severityIndex=0;severityIndex<severityCount;severityIndex++)
	{
		thisSeverity=mErrorSeverities.Item(severityIndex);
		if (thisSeverity>severity)
		{
			severity=thisSeverity;
		}
	}

	return (severity);
}

void ProgressPanel::SetSeverityIcon()
{
	SetResultSeverity(OverallSeverity());
}

void ProgressPanel::LogError(wxString &errorTextIn,int severity)
{
	wxString	status="";
	wxString	errorText;
	wxString	panelDescription;
	if (!mStoringError)
	{
		panelDescription=GetDescription();
		panelDescription=panelDescription+"                                          ";
		panelDescription=panelDescription.Left(25);
		errorText.Printf("%s: %s",STRING_TO_CHAR(panelDescription),STRING_TO_CHAR(errorTextIn));
		mErrorSeverities.Add(severity);
		mErrors.Add(errorText);
		SetSeverityIcon();
		LogToDatabase(severity,errorText,status);
	}
}

void ProgressPanel::OnButtonPressed(wxCommandEvent &event)
{
	wxString	title="Errors";
	wxString	name="";
	ErrorPanel	*errorPanel;
	bool		save;
	
	GenericOKCancelDialog	dialog(this,wxID_ANY,title,wxDefaultPosition,wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
									"");

	errorPanel=new ErrorPanel(mErrors,mErrorSeverities);
	errorPanel->Create(&dialog,wxID_ANY,
						wxDefaultPosition,wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	errorPanel->CreateScreen();
	//dialog.SetPanel(errorPanel);
	//dialog.CreateDialog();
	save = dialog.DisplayDialog(errorPanel);
	if (save)
	{
		wxString	filename;

		filename=wxFileSelector("Choose a save location for error file");
		if (filename.Length()>0)
		{
			FILE *fp;

			fp=fopen(STRING_TO_CHAR(filename),"w");
			if (fp!=NULL)
			{
				int			errorCount;
				int			errorIndex;
				wxString	errorText;
				wxString	errorSeverity;
				int			severity;

				errorCount=mErrors.GetCount();
				for (errorIndex=0;errorIndex<errorCount;errorIndex++)
				{
					errorText=mErrors.Item(errorIndex);
					severity=mErrorSeverities.Item(errorIndex);
					errorSeverity=SeverityString(severity);

					fprintf(fp,"%15s %s\n",STRING_TO_CHAR(errorSeverity),STRING_TO_CHAR(errorText));
				}
				fclose(fp);
			}
		}
	}
}

wxString ProgressPanel::SeverityString(int severity)
{
	wxString	status;
	switch (severity)
	{
		default:
			status="";
			break;
		case SeverityNotRun:
			status="Not Run";
			break;
		case SeverityOK:
			status="OK";
			break;
		case SeverityCaution:
			status="Caution";
			break;
		case SeverityError:
			status="Error";
			break;
	}

	return (status);
}

wxString ProgressPanel::TableName()
{
	wxString	tableName="progressbar";
	return (tableName);
}

wxString ProgressPanel::CreateTableSQL()
{
	wxString	SQL;

	SQL.Printf("create table %s "
				"("
				"stage			string,"
				"statusindex	integer,"
				"logtime		integer,"
				"status			string,"
				"severity		string,"
				"error			string"
				")",
				STRING_TO_CHAR(TableName()));

	return (SQL);

}

void ProgressPanel::LogToDatabase(int severity,
									wxString &errorText,
									wxString &statusText)
{
	STATSGEN_DEBUG_FUNCTION_START("ProgressPanel","LogToDatabase")
	wxString	severityString="";
	wxString	SQL;
	wxString	stage;
	time_t		logTime;
	int			logTimeInt;

	if ((!mStoringError)&&(mDescription!=NULL) &&(severity!=SeverityNotRun))
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Constructing SQL");
		severityString = SeverityString(severity);
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Severity String");
		STATSGEN_DEBUG(DEBUG_ALWAYS,severityString)
		logTime=time(&logTime);
		logTimeInt = logTime;
		stage=mDescription->GetLabel();
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Label");
		STATSGEN_DEBUG(DEBUG_ALWAYS,stage)
		mStoringError=true;
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"SQL");
		SQL.Printf("insert into %s "
				"(stage,statusindex,logtime,status,severity,error)"
				"values"
				"('%s',%d,%d,'%s','%s','%s')",
				STRING_TO_CHAR(TableName()),
				STRING_TO_CHAR(stage),
				mStatusIndex,
				logTimeInt,
				STRING_TO_CHAR(statusText),
				STRING_TO_CHAR(severityString),
				STRING_TO_CHAR(globalStatistics.statsgenDatabase.SafeForInsert(errorText)));
		STATSGEN_DEBUG(DEBUG_ALWAYS,STRING_TO_CHAR(SQL))
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
		mStatusIndex++;
		mStoringError=false;
	}
	STATSGEN_DEBUG_FUNCTION_END
}

wxString ProgressPanel::GetTimeRemaining()
{
	return (mTimeRemaining->GetLabel());
}

wxString ProgressPanel::GetTimeSpent()
{
	return (mTimeSpent->GetLabel());
}

wxString ProgressPanel::GetDescription()
{
	return (mDescription->GetLabel());
}

wxString ProgressPanel::GetRate()
{
	return (mRate->GetLabel());
}

wxString ProgressPanel::GetCounter()
{
	return (mCounter->GetLabel());
}

wxString ProgressPanel::GetStatusPanel()
{
	return (mStatus->GetLabel());
}

int ProgressPanel::GetErrorCount()
{
	return (mErrorSeverities.GetCount());
}

void ProgressPanel::SetTimeRemaining(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(mTimeRemaining);
	event.SetString(value);
	if (mEventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		mTimeRemaining->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetTimeSpent(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(mTimeSpent);
	event.SetString(value);
	if (mEventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		mTimeSpent->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetDescription(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(mDescription);
	event.SetString(value);
	if (mEventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		mDescription->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetRate(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(mRate);
	event.SetString(value);
	if (mEventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		mRate->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetCounter(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(mCounter);
	event.SetString(value);
	if (mEventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		mCounter->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetStatusPanel(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(mStatus);
	event.SetString(value);
	if (mEventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		mStatus->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetErrors(wxArrayInt &severitiesIn,wxArrayString &errorsIn)
{
	WX_APPEND_ARRAY(mErrorSeverities,severitiesIn);
	WX_APPEND_ARRAY(mErrors,errorsIn);
	SetSeverityIcon();
}

bool ProgressPanel::UpdateClone(ProgressPanel *destinationPanel)
{
	wxString	sourceTimeRemaining;
	wxString	sourceTimeSpent;
	wxString	sourceDescription;
	wxString	sourceRate;
	wxString	sourceStatus;
	wxString	sourceCounter;
	int			sourceErrorCount;

	wxString	destinationTimeRemaining;
	wxString	destinationTimeSpent;
	wxString	destinationDescription;
	wxString	destinationRate;
	wxString	destinationStatus;
	wxString	destinationCounter;
	int			destinationErrorCount;

	bool	panelUpdated = false;
	if (destinationPanel==NULL)
	{
		return false;
	}
	sourceTimeRemaining			=GetTimeRemaining();
	sourceTimeSpent				=GetTimeSpent();
	sourceDescription			=GetDescription();
	sourceRate					=GetRate();
	sourceStatus				=GetStatusPanel();
	sourceErrorCount			=GetErrorCount();
	sourceCounter				=GetCounter();

	destinationTimeRemaining	=destinationPanel->GetTimeRemaining();
	destinationTimeSpent		=destinationPanel->GetTimeSpent();
	destinationDescription		=destinationPanel->GetDescription();
	destinationRate				=destinationPanel->GetRate();
	destinationStatus			=destinationPanel->GetStatusPanel();
	destinationErrorCount		=destinationPanel->GetErrorCount();
	destinationCounter			=destinationPanel->GetCounter();

	
	destinationPanel->mCounterEnabled=mCounterEnabled;
	if (sourceTimeRemaining.Cmp(destinationTimeRemaining)!=0)
	{
		panelUpdated = true;
		destinationPanel->SetTimeRemaining(sourceTimeRemaining);
	}
	if (sourceTimeSpent.Cmp(destinationTimeSpent)!=0)
	{
		panelUpdated = true;
		destinationPanel->SetTimeSpent(sourceTimeSpent);
	}
	if (sourceDescription.Cmp(destinationDescription)!=0)
	{
		panelUpdated = true;
		destinationPanel->SetDescription(sourceDescription);
	}
	if (sourceRate.Cmp(destinationRate)!=0)
	{
		panelUpdated = true;
		destinationPanel->SetRate(sourceRate);
	}
	if (sourceStatus.Cmp(destinationStatus)!=0)
	{
		panelUpdated = true;
		destinationPanel->SetStatus(sourceStatus);
	}
	if (sourceCounter.Cmp(destinationCounter)!=0)
	{
		panelUpdated = true;
		destinationPanel->SetCounter(sourceCounter);
	}
	if (sourceErrorCount != destinationErrorCount)
	{
		// Need to add to errors here actually
		destinationPanel->SetErrors(mErrorSeverities,mErrors);
	}

	if  (panelUpdated)
	{
		wxSizeEvent event;
		wxSafeYield();
	}
	return (panelUpdated);
}

void ProgressPanel::EventUpdating(bool value)
{
	mEventUpdating=value;
}

wxSize ProgressPanel::GetDescriptionSize()
{
	return mDescription->GetSize();
}
void ProgressPanel::SetDescriptionMinimumSize(wxSize size)
{
	mDescription->SetMinSize(size);
}
