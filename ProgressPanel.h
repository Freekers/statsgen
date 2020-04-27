#ifndef __PROGRESSPANEL
#define __PROGRESSPANEL

#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>

#define MAX_TIME_STEPS		10

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
		void Initiate(
				long top,
				const char *units,
				long scale=1,
				const char *rateUnits="",
				long rateScale=1
				);
		void Update(long value);

		void OnButtonPressed(wxCommandEvent &event);
		void SetOffset(long offsetIn);
		void Finalise();
		long CurrentValue();
		void EnableTimeToGo();
		void DisableTimeToGo();
		void DisableCounter();
		void EnableCounter();
		void SetStatus(wxString &statusText);
		void Reset();
		void SetSeverityIcon();
		void ClearErrors();
		void LogError(wxString &errorText,int severity);
		static wxBitmap SeverityImage(int severity);
		static wxString SeverityString(int severity);
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
		wxSize GetDescriptionSize();
		void SetDescriptionMinimumSize(wxSize size);
		void		CreateScreen();

	protected:

	public:
		time_t		mPreviousTime;
		time_t		mStartTime;
		bool		mCounterEnabled;
	private:
		void		SetResultSeverity(int severity);

		wxStaticText	*mTimeRemaining;
		wxStaticText	*mTimeSpent;
		wxStaticText	*mCounter;
		wxStaticText	*mDescription;
		wxStaticText	*mRate;
		wxStaticText	*mStatus;
		wxBitmapButton	*mResultButton;

		wxString		mUnitText;

		long			mMaxValue;
		long			mCurrentValue;
		long			mScale;
		long			mOffset;
		long			mRateScale;
		wxString		mRateUnits;

		bool			mTimeToGoEnabled;

		wxArrayInt		mErrorSeverities;
		wxArrayString	mErrors;
		time_t			mHistoryTimes[MAX_TIME_STEPS];
		long			mHistoryValues[MAX_TIME_STEPS];
		bool			mTimeStepsCreated;
		bool			mStoringError;
		int				mStatusIndex;
		wxPanel			*mProgressBarPanel;
		wxBoxSizer		*mProgressSizer;

		bool			mEventUpdating;

		wxBoxSizer		*mMainSizer;
		DECLARE_EVENT_TABLE()
};

#endif
