#ifndef __REMOTEMACHINE
#define __REMOTEMACHINE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/protocol/ftp.h>
#include "RestartingFTP.h"

// Statsgen Includes

#define MAX_CONNECTION_ATTEMPTS		1
class RemoteMachine
{
	public:
		RemoteMachine(wxString &IDIn);
		virtual ~RemoteMachine();

		bool MakeDir(wxString &baseDirectory,wxString &newDirectory);
		bool PutFile(wxString &remoteDirectory,
					wxString &localFilename);
		bool PutFile(wxString &remoteDirectory,
					wxString &localFilename,
					int maxSeverity);
		bool PutFile(wxString &remoteDirectory,
					wxString &localFilename,
					wxInputStream *localFileStream,
					wxString &remoteFilename,
					int maxSeverity);
		bool GetRemoteFile(
					wxString &remoteDirectory,
					wxString &remoteFilename,
					wxString &localDirectory);
		bool GetRemoteFileRestart(
					wxString &remoteDirectory,
					wxString &remoteFilename,
					wxString &localDirectory,
					bool restartDownload,
					long maxLogfileSize);
		long GetRemoteFileSize(
					wxString &remoteDirectory,
					wxString &remoteFilename);

		bool GetRemoteDirectoryListing(wxString &remoteDirectory,
					wxString &remoteWildcard,
					wxArrayString &remoteFiles);
		bool DownloadFileList(wxString &remoteDirectory,
					wxString &remoteWildcard,
					wxString &localDirectory);
		bool ChangeDirectory(wxString &fullPath);
		bool Connect(RestartingFTP &ftpConnection);
		void Disconnect();
		void MaxErrorSeverity(int severity);
		wxString GetLastErrorMessage();
		
	public:
		wxString	ID;
		wxString	IPAddress;
		wxString	FTPUsername;
		wxString	FTPPassword;
		int			FTPPort;
		bool		FTPPassive;
		int			FTPRetries;

	private:
//		RestartingFTP		ftpConnection;
		RestartingFTP		*staticFTPConnection;

		int			maxErrorSeverity;

		wxString	lastErrorMessage;
		
};


#endif
