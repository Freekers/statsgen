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
		EVT_SIZE(ProgressPanel::OnResize)
		EVT_BUTTON(WINDOW_ID_ERROR_BUTTON,ProgressPanel::OnButtonPressed)
END_EVENT_TABLE()

ProgressPanel::ProgressPanel()
{

	timeRemaining		=NULL;
	timeSpent			=NULL;
	counter				=NULL;
	description			=NULL;
	status				=NULL;
	maxValue			=-1;
	scale				=1;
	currentValue		=0;
	offset				=0;
	desiredLabelWidth	=0;
	previousTime		=0;
	dynamicSizing		=false;
	EnableTimeToGo();
	counterEnabled		=true;
	timeStepsCreated	=0;
	storingError		=false;
	statusIndex			=0;

	EventUpdating(true);

}

void ProgressPanel::Reset()
{
	startTime=time(&startTime);
	currentValue=0;
	offset=0;
	timeStepsCreated=0;
	statusIndex=0;
}

void ProgressPanel::CreateScreen()
{
	wxString		label="";
	wxSize	buttonSize(16,16);

	if (timeRemaining==NULL)
	{
		timeRemaining=new wxStaticText(this,-1,label);
		timeSpent=new wxStaticText(this,-1,label);
		counter=new wxStaticText(this,-1,label);
		description=new wxStaticText(this,-1,label);
		rate=new wxStaticText(this,-1,label);
		status=new wxStaticText(this,GetId(),label);
		resultButton=new wxBitmapButton(this,WINDOW_ID_ERROR_BUTTON,wxArtProvider::GetIcon(wxART_ERROR,wxART_OTHER,buttonSize));
		ClearErrors();

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
	unitText=unitsIn;
	maxValue=maxIn;
	scale=scaleIn;
	rateScale=rateScaleIn;
	rateUnits=rateUnitsIn;

	
}

int ProgressPanel::LabelWidth()
{
	wxSize	itemSize;

	itemSize=description->GetSize();
	return (itemSize.GetWidth());
}

void ProgressPanel::SetLabelWidth(int labelWidth)
{
	desiredLabelWidth=labelWidth;

	wxSizeEvent	dummyEvent;

	OnResize(dummyEvent);
}

void ProgressPanel::SetLabel(const char *label)
{
	CreateScreen();
	description->SetLabel(label);
}

void ProgressPanel::SetOffset(long value)
{
	offset=value;
}

void ProgressPanel::WakeUp()
{
	Update(currentValue-offset);
}

void ProgressPanel::UpdateColouredBar()
{
	if (timeToGoEnabled)
	{
		wxClientDC	dc(this);
		wxPen		currentPen;
		wxBrush		currentBrush;
		wxColour	green(0,175,0);
		wxColour	red(175,0,0);
		wxColour	amber(175,0,175);
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
				chosenBrush=greenBrush;
				chosenPen=greenPen;
				break;
			case SeverityCaution:
				chosenBrush=amberBrush;
				chosenPen=amberPen;
				break;
			case SeverityError:
				chosenBrush=redBrush;
				chosenPen=redPen;
				break;
			default:
				chosenBrush=greenBrush;
				chosenPen=greenPen;
				break;
		}
		currentPen		=dc.GetPen();
		currentBrush	=dc.GetBrush();

		size=description->GetSize();
		maxWidth=size.GetWidth();
		size=GetSize();
		height=size.GetHeight();
		if (maxValue<=0)
		{
			pixelsPerUnit=(float)maxWidth;
		}
		else
		{
			pixelsPerUnit=(float)maxWidth/(float)maxValue;
		}
		width=(int)(pixelsPerUnit * (float)currentValue);

		dc.SetPen(chosenPen);
		dc.SetBrush(chosenBrush);
		dc.DrawRectangle(0,1,width,height-3);
		dc.SetPen(chosenPen);
		dc.SetBrush(currentBrush);
		dc.DrawRectangle(width,1,maxWidth-width,height-3);
	}

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
	
	currentTime=time(&currentTime);
	elapsedTime=currentTime-startTime;
	currentValue=value+offset;
	if ((elapsedTime!=previousTime)/*||(previousTime==0)*/)
	{
		// Add the current time and value to the history list
		// first shuffle the current values down one
		historyTimes[0]=historyTimes[1];
		historyValues[0]=historyValues[1];
		historyTimes[1]=historyTimes[2];
		historyValues[1]=historyValues[2];
		if (timeStepsCreated==0)
		{
			historyTimes[0]=currentTime;
			historyValues[0]=currentValue;
			historyTimes[1]=currentTime;
			historyValues[1]=currentValue;
			historyTimes[2]=currentTime;
			historyValues[2]=currentValue;
		}
		historyTimes[2]=currentTime;
		historyValues[2]=currentValue;
		previousTime=elapsedTime;
		timeRemainingString="???";
		rateString="???";
		progressString="???";
		progressToGo=maxValue-currentValue;
		if (progressToGo<0)
		{
			progressToGo=0;
		}
		if ((elapsedTime>0)&&(timeStepsCreated>0))
		{
			if (timeToGoEnabled)
			{
				time_t		rateElapsedTime;
				long		rateElapsedValue;

				rateElapsedTime=historyTimes[2]-historyTimes[0];
				rateElapsedValue=historyValues[2]-historyValues[0];
				//rateValue=((float)currentValue)/((float)elapsedTime);
				
				if ((rateElapsedTime>0)&&(rateElapsedValue>0))
				{
					rateValue=((float)rateElapsedValue)/((float)rateElapsedTime);
					timeToGo=(long)(((float)progressToGo)/rateValue);
					rateValue/=((float)rateScale);
					rateString.Printf("%0.1f%s / sec",rateValue,rateUnits.GetData());
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
					rateString.Printf("???%s / sec",rateUnits.GetData());
				}
			}
		}
		if (timeStepsCreated<2)
		{
			timeStepsCreated++;
		}
		if (timeToGoEnabled)
		{
			progressToGo/=scale;
			progressString.Printf("%ld%s",progressToGo,unitText.GetData());
		}
		else
		{
			progressString.Printf("%ld%s",currentValue/scale,unitText.GetData());
		}
		minutes=elapsedTime / 60;
		seconds=elapsedTime % 60;
		timeSpentString.Printf("%ld:%02ld",minutes,seconds);

		if (timeToGoEnabled)
		{
			//timeRemaining->SetLabel(timeRemainingString);
			wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,EVENT_ID_PROGRESS_STATUS_TEXT);
			event.SetClientData(timeRemaining);
			event.SetString(timeRemainingString);
			if (eventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				timeRemaining->SetLabel(timeRemainingString);
				wxSafeYield();
			}
			event.SetClientData(rate);
			event.SetString(rateString);
			if (eventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				rate->SetLabel(rateString);
				wxSafeYield();
			}
		}
		if (counterEnabled)
		{
			wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,EVENT_ID_PROGRESS_STATUS_TEXT);
			//counter->SetLabel(progressString);
			event.SetClientData(counter);
			event.SetString(progressString);
			if (eventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				counter->SetLabel(progressString);
				wxSafeYield();
			}
			event.SetClientData(timeSpent);
			event.SetString(timeSpentString);
			if (eventUpdating)
			{
				wxPostEvent(globalStatistics.mainEventHandler,event);
			}
			else
			{
				timeSpent->SetLabel(timeSpentString);
				wxSafeYield();
			}
			if (eventUpdating)
			{
				UpdateColouredBar();
			}
		}
	}
	
}

void ProgressPanel::Finalise()
{
	wxString	errorText="done";

	previousTime=0;
	Update(currentValue-offset);
	LogError(errorText,SeverityOK);
	SetStatus(errorText);
}

void ProgressPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	wxSize		itemSize;
	int			width;
	int			height;
	int			timeRemainingWidth;
	int			timeSpentWidth;
	int			counterWidth;
	int			descriptionWidth;
	int			rateWidth;
	int			statusWidth;
	int			buttonWidth;
	int			buttonHeight;
	int			gap=10;
	int			yBorder=11;
	
	CreateScreen();

	itemSize=counter->GetSize();
	height=itemSize.GetHeight();
	
	itemSize=resultButton->GetSize();
	buttonHeight=itemSize.GetHeight();
	buttonWidth=itemSize.GetWidth();

	counterWidth		=120;
	timeSpentWidth		=50;
	statusWidth			=250;

	timeRemainingWidth	=50;
	rateWidth			=120;
	

	if ((!counterEnabled) &&
		(dynamicSizing))
	{
		counterWidth=0;
		timeSpentWidth=0;
	}

	if ((!timeToGoEnabled) &&
		(dynamicSizing))
	{
		timeRemainingWidth=0;
		rateWidth=0;
	}
	itemSize=description->GetSize();
	descriptionWidth=itemSize.GetWidth();
	if (desiredLabelWidth>0)
	{
		descriptionWidth=desiredLabelWidth;
	}

	itemSize=GetSize();
	width=itemSize.GetWidth();
	if (dynamicSizing)
	{

		statusWidth=width-(timeRemainingWidth+gap+
				timeSpentWidth+gap+
				counterWidth+gap+
				descriptionWidth+gap+
				gap+
				buttonWidth+gap+
				rateWidth+gap);
		if (statusWidth<0)
		{
			statusWidth=0;
		}
	}
	description->SetSize(0,
						yBorder/2,
						descriptionWidth,height);

	counter->SetSize(descriptionWidth+gap,
							yBorder/2,
							counterWidth,height);

	timeSpent->SetSize(descriptionWidth+gap+
							counterWidth+gap,
							yBorder/2,
							timeSpentWidth,height);

	timeRemaining->SetSize(descriptionWidth+gap+
							counterWidth+gap+
							timeSpentWidth+gap,
							yBorder/2,
							timeRemainingWidth,height);

	rate->SetSize(descriptionWidth+gap+
				counterWidth+gap+
				timeSpentWidth+gap+
				timeRemainingWidth+gap,
				yBorder/2,
				rateWidth,height);
	status->SetSize(descriptionWidth+gap+
				counterWidth+gap+
				timeSpentWidth+gap+
				rateWidth+gap+
				timeRemainingWidth+gap,
				yBorder/2,
				statusWidth,height);
	resultButton->SetSize(descriptionWidth+gap+
				counterWidth+gap+
				timeSpentWidth+gap+
				rateWidth+gap+
				timeRemainingWidth+gap+
				statusWidth+gap,
				0,
				buttonWidth,buttonHeight);

	width=timeRemainingWidth+gap+
			timeSpentWidth+gap+
			counterWidth+gap+
			descriptionWidth+gap+
			statusWidth+gap+
			buttonWidth+gap+
			rateWidth+gap;
	SetSize(width,height+yBorder+5);
}

long ProgressPanel::CurrentValue()
{
	return (currentValue);
}

bool ProgressPanel::GetEventUpdating()
{
	return (eventUpdating);
}

void ProgressPanel::DisableCounter()
{
	counterEnabled=false;
}
void ProgressPanel::DisableTimeToGo()
{
	timeToGoEnabled=false;
}

void ProgressPanel::EnableTimeToGo()
{
	timeToGoEnabled=true;
}

void ProgressPanel::SetStatus(wxString &statusText)
{
	wxString	errorText="";
	int			severity=-1;
	//status->SetLabel(statusText);
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(status);
	event.SetString(statusText);
	if (eventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		status->SetLabel(statusText);
		wxSafeYield();
	}
	LogToDatabase(severity,errorText,statusText);
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
	event.SetClientData(resultButton);
	event.SetInt(severity);
	wxPostEvent(globalStatistics.mainEventHandler,event);
}

void ProgressPanel::ClearErrors()
{
	wxString	label="";
	wxString	SQL;

	errorSeverities.Clear();
	errors.Clear();
	SetSeverityIcon();
	timeRemaining->SetLabel(label);
	timeSpent->SetLabel(label);
	counter->SetLabel(label);
	rate->SetLabel(label);
	status->SetLabel(label);
	statusIndex=0;
	if (!storingError)
	{
		if (description!=NULL)
		{
			storingError=true;
			SQL.Printf("delete from %s where stage='%s'",
				TableName().GetData(),
				description->GetLabel().GetData());
			globalStatistics.statsgenDatabase.SimpleExecute(SQL);
			storingError=false;
		}
	}
}

int ProgressPanel::OverallSeverity()
{
	int	severity=SeverityNotRun;
	int	severityCount;
	int	thisSeverity;
	int	severityIndex;

	severityCount=errorSeverities.GetCount();
	for (severityIndex=0;severityIndex<severityCount;severityIndex++)
	{
		thisSeverity=errorSeverities.Item(severityIndex);
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
	if (!storingError)
	{
		panelDescription=GetDescription();
		panelDescription=panelDescription+"                                          ";
		panelDescription=panelDescription.Left(25);
		errorText.Printf("%s: %s",panelDescription.GetData(),errorTextIn.GetData());
		errorSeverities.Add(severity);
		errors.Add(errorText);
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
	
	GenericOKCancelDialog	dialog(this,-1,title,wxDefaultPosition,wxDefaultSize,
						wxCAPTION |
						wxCLOSE_BOX |
						wxSYSTEM_MENU |
						wxRESIZE_BORDER |
						wxMAXIMIZE_BOX,
									"");

	errorPanel=new ErrorPanel(errors,errorSeverities);
	errorPanel->Create(&dialog,-1,
						wxDefaultPosition,wxDefaultSize,
						wxTAB_TRAVERSAL,
						_T("panel"));
	errorPanel->CreateScreen();
	dialog.SetPanel(errorPanel);
	dialog.CreateDialog();
	save=(dialog.ShowModal()==WINDOW_ID_BUTTON_SAVE);
	if (save)
	{
		wxString	filename;

		filename=wxFileSelector("Choose a save location for error file");
		if (filename.Length()>0)
		{
			FILE *fp;

			fp=fopen(filename.GetData(),"w");
			if (fp!=NULL)
			{
				int			errorCount;
				int			errorIndex;
				wxString	errorText;
				wxString	errorSeverity;
				int			severity;

				errorCount=errors.GetCount();
				for (errorIndex=0;errorIndex<errorCount;errorIndex++)
				{
					errorText=errors.Item(errorIndex);
					severity=errorSeverities.Item(errorIndex);
					errorSeverity=SeverityString(severity);

					fprintf(fp,"%15s %s\n",errorSeverity.GetData(),errorText.GetData());
				}
				fclose(fp);
			}
		}
	}
}

void ProgressPanel::DynamicSizing()
{
	dynamicSizing=true;
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
				TableName().GetData());

	return (SQL);

}

void ProgressPanel::LogToDatabase(int severity,
									wxString &errorText,
									wxString &statusText)
{
	wxString	severityString="";
	wxString	SQL;
	wxString	stage;
	time_t		logTime;

	if ((!storingError)&&(description!=NULL) &&(severity!=SeverityNotRun))
	{
		severityString = SeverityString(severity);
		logTime=time(&logTime);
		stage=description->GetLabel();
		storingError=true;
		SQL.Printf("insert into %s "
				"(stage,statusindex,logtime,status,severity,error)"
				"values"
				"('%s',%d,%d,'%s','%s','%s')",
				TableName().GetData(),
				stage.GetData(),
				statusIndex,
				logTime,
				statusText.GetData(),
				severityString.GetData(),
				globalStatistics.statsgenDatabase.SafeForInsert(errorText).GetData());
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
		statusIndex++;
		storingError=false;
	}
}

wxString ProgressPanel::GetTimeRemaining()
{
	return (timeRemaining->GetLabel());
}

wxString ProgressPanel::GetTimeSpent()
{
	return (timeSpent->GetLabel());
}

wxString ProgressPanel::GetDescription()
{
	return (description->GetLabel());
}

wxString ProgressPanel::GetRate()
{
	return (rate->GetLabel());
}

wxString ProgressPanel::GetCounter()
{
	return (counter->GetLabel());
}

wxString ProgressPanel::GetStatusPanel()
{
	return (status->GetLabel());
}

int ProgressPanel::GetErrorCount()
{
	return (errorSeverities.GetCount());
}

void ProgressPanel::SetTimeRemaining(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(timeRemaining);
	event.SetString(value);
	if (eventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		timeRemaining->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetTimeSpent(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(timeSpent);
	event.SetString(value);
	if (eventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		timeSpent->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetDescription(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(description);
	event.SetString(value);
	if (eventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		description->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetRate(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(rate);
	event.SetString(value);
	if (eventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		rate->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetCounter(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(counter);
	event.SetString(value);
	if (eventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		counter->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetStatusPanel(wxString &value)
{
	wxCommandEvent	event(wxEVT_COMMAND_MENU_SELECTED,
				EVENT_ID_PROGRESS_STATUS_TEXT);
	event.SetClientData(status);
	event.SetString(value);
	if (eventUpdating)
	{
		wxPostEvent(globalStatistics.mainEventHandler,event);
	}
	else
	{
		status->SetLabel(value);
		wxSafeYield();
	}
}

void ProgressPanel::SetErrors(wxArrayInt &severitiesIn,wxArrayString &errorsIn)
{
	WX_APPEND_ARRAY(errorSeverities,severitiesIn);
	WX_APPEND_ARRAY(errors,errorsIn);
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

	
	destinationPanel->counterEnabled=counterEnabled;
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
		destinationPanel->SetErrors(errorSeverities,errors);
	}

	if  (panelUpdated)
	{
		wxSizeEvent event;
		destinationPanel->OnResize(event);
		wxSafeYield();
	}
	return (panelUpdated);
}

void ProgressPanel::EventUpdating(bool value)
{
	eventUpdating=value;
}
