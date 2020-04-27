#ifndef __PROGRESSPANEL
#define __PROGRESSPANEL

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

#define MAX_TIME_STEPS		3

// Statsgen Includes
#include "WindowIDs.h"

		enum {
			SeverityNotRun=0,
			SeverityOK,
			SeverityCaution,
			SeverityError,
			SeverityMax};

class ProgressPanel : public wxPanel
{
	public:
		ProgressPanel();
		~ProgressPanel();
		void EventUpdating(bool value);
		
		void WakeUp();
		void SetLabel(const char *label);
		int LabelWidth();
		void SetLabelWidth(int width);
		void Initiate(
				long top,
				const char *units,
				long scale=1,
				const char *rateUnits="",
				long rateScale=1
				);
		void Update(long value);

		void OnResize(wxSizeEvent &event);
		void OnButtonPressed(wxCommandEvent &event);
		void SetOffset(long offsetIn);
		void Finalise();
		long CurrentValue();
		void EnableTimeToGo();
		void DisableTimeToGo();
		void DisableCounter();
		void SetStatus(wxString &statusText);
		void Reset();
		void SetSeverityIcon();
		void ClearErrors();
		void LogError(wxString &errorText,int severity);
		static wxBitmap SeverityImage(int severity);
		static wxString SeverityString(int severity);
		void DynamicSizing();
		static wxString TableName();
		static wxString CreateTableSQL();
		void LogToDatabase(int severity,wxString &errorText,wxString &statusText);

		wxString GetTimeRemaining();
		wxString GetTimeSpent();
		wxString GetDescription();
		wxString GetRate();
		wxString GetCounter();
		wxString GetStatusPanel();
		int GetErrorCount();
		void SetTimeRemaining(wxString &value);
		void SetTimeSpent(wxString &value);
		void SetDescription(wxString &value);
		void SetRate(wxString &value);
		void SetCounter(wxString &value);
		void SetStatusPanel(wxString &value);
		void SetErrors(wxArrayInt &severitiesIn,wxArrayString &errorsIn);
		bool UpdateClone(ProgressPanel *clonePanel);
		bool GetEventUpdating();
		void UpdateColouredBar();
		int OverallSeverity();

	protected:

	public:
		time_t		previousTime;
		time_t		startTime;
		bool		counterEnabled;
	private:
		void		CreateScreen();
		void		SetResultSeverity(int severity);
		bool		dynamicSizing;

		wxStaticText	*timeRemaining;
		wxStaticText	*timeSpent;
		wxStaticText	*counter;
		wxStaticText	*description;
		wxStaticText	*rate;
		wxStaticText	*status;
		wxBitmapButton	*resultButton;

		wxString	unitText;
		int		desiredLabelWidth;

		long		maxValue;
		long		currentValue;
		long		scale;
		long		offset;
		long		rateScale;
		wxString	rateUnits;

		bool		timeToGoEnabled;

		wxArrayInt		errorSeverities;
		wxArrayString	errors;
		time_t		historyTimes[MAX_TIME_STEPS];
		long		historyValues[MAX_TIME_STEPS];
		int		timeStepsCreated;
		bool		storingError;
		int		statusIndex;

		bool	eventUpdating;

		DECLARE_EVENT_TABLE()
};

#endif
