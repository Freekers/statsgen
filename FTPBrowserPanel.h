#ifndef __FTPBROWSERPANEL
#define __FTPBROWSERPANEL

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/thread.h>
#include <wx/protocol/ftp.h>
#include <wx/imaglist.h>

// Statsgen Includes
#include "WindowIDs.h"
#include "RemoteMachine.h"
#include "ProgressPanel.h"

#define WORKER_EVENT		1000

#define REMOTE_REQUEST_DIR_LIST	

class RemoteFTPBrowserThread;
class FTPBrowserPanel : public wxPanel
{
	public:
		virtual ~FTPBrowserPanel();
		FTPBrowserPanel(
			wxString &configGroupIn,
			wxString &defaultPath,
			bool browsingForFile);
		void CreateScreen();
		void Update(int fileIndex);
		void CreateList();
		void OnRemoteRequest(wxCommandEvent &event);

		void StartThreadWork();	
		void StopThreadWork();	
		bool DoingThreadWork();
		void GetDirectoryListing();
		void ChangeDirectory(wxString &filename);
		void OnListDoubleClick(wxListEvent &event);
		void OnListSingleClick(wxListEvent &event);
		wxString GetCurrentSelection();
		wxString CurrentPath();

		RemoteMachine	*mRemoteMachine;
		wxArrayString	mFileNames;
		long			mSelectedSize;
	protected:
		wxListCtrl		*mDirectoryListing;


	private:
		wxImageList				mImageList;
		wxString				mConfigGroup;
		wxArrayLong				mFileSizes;
		wxString				mCurrentPath;
		bool					workingForThread;
		RemoteFTPBrowserThread	*thread;
		wxTextCtrl				*mCurrentSelection;
		ProgressPanel			*mProgressPanel;
		wxBoxSizer				*mMainSizer;

		DECLARE_EVENT_TABLE()
};

class RemoteFTPBrowserThread : public wxThread
{
	public:
		RemoteFTPBrowserThread(FTPBrowserPanel *browserPanelIn);
		virtual void *Entry();
		virtual void OnExit();

		FTPBrowserPanel	*browserPanel;
	
};

#endif
