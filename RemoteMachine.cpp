// wxWindows includes
#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/stream.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/protocol/ftp.h>
#include <wx/filename.h>

// Statsgen includes
#include "RemoteMachine.h"
#include "ErrorData.h"
#include "GlobalStatistics.h"
#include "Progress.h"
#include "RestartingFTP.h"
#include "ErrorData.h"

RemoteMachine::RemoteMachine(wxString &IDIn)
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteMachine","RemoteMachine")
	wxString	configBaseKey;
	wxString	configKey;
	wxString	configValue;
	wxString	msg;

	staticFTPConnection=NULL;
	maxErrorSeverity=SeverityError;

	ID=IDIn;
	configBaseKey="/"+ID+"/";

	// Read in the configuration items for this machine
	configKey=configBaseKey+"IPAddress";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"");
	IPAddress=configValue;
	msg.Printf("IPAddress: %s",STRING_TO_CHAR(configValue));STATSGEN_DEBUG(DEBUG_ALWAYS,msg)

	configKey=configBaseKey+"FTPUsername";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"");
	FTPUsername=configValue;
	msg.Printf("FTPUsername: %s",STRING_TO_CHAR(configValue));STATSGEN_DEBUG(DEBUG_ALWAYS,msg)

	configKey=configBaseKey+"FTPPassword";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"");
	FTPPassword=configValue;
	msg.Printf("FTPPassword: %s",STRING_TO_CHAR(configValue));STATSGEN_DEBUG(DEBUG_ALWAYS,msg)

	configKey=configBaseKey+"FTPPort";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"21");
	FTPPort=-1;
	if (configValue.Length()>0)
	{
		FTPPort=atoi(STRING_TO_CHAR(configValue));
	}
	msg.Printf("FTPPort: %s",STRING_TO_CHAR(configValue));STATSGEN_DEBUG(DEBUG_ALWAYS,msg)

	configKey=configBaseKey+"FTPPassive";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"y");
	FTPPassive=(configValue.CmpNoCase("y")==0);
	msg.Printf("FTPPassive: %s",STRING_TO_CHAR(configValue));STATSGEN_DEBUG(DEBUG_ALWAYS,msg)

	configKey=configBaseKey+"FTPRetries";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"3");
	FTPRetries=atoi(STRING_TO_CHAR(configValue));
	msg.Printf("FTPRetries: %s",STRING_TO_CHAR(configValue));STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	STATSGEN_DEBUG_FUNCTION_END
}

RemoteMachine::~RemoteMachine()
{
	if (staticFTPConnection!=NULL)
	{
		delete staticFTPConnection;
		staticFTPConnection=NULL;
	}
}

bool RemoteMachine::GetRemoteFile(
							wxString &remoteDirectory,
							wxString &remoteFilename,
							wxString &localDirectory)
{
	return (GetRemoteFileRestart(remoteDirectory,
								remoteFilename,
								localDirectory,
								false,
								0));
}

bool RemoteMachine::GetRemoteFileRestart(
							wxString &remoteDirectory,
							wxString &remoteFilename,
							wxString &localDirectory,
							bool restartDownload,
							long maxLogfileSize)
{
	bool				retVal=true;
	wxString			msg;
	wxString			ftpResponse;

	wxString			fullLocalFilename;
	wxString			fullRemoteFilename;
	int					remoteFileSize;
	unsigned char		buffer[1024];
	int					bytesRead;
	int					chunkRead;
	wxInputStream		*remoteFileStream=NULL;
	wxOutputStream	*localFileStream=NULL;
	bool				finished=false;
	RestartingFTP				ftpConnection;
	int					localFileSize=-1;

	STATSGEN_DEBUG_FUNCTION_START("RemoteFile","GetRemoteFile")
	msg.Printf("Downloading %s\n",STRING_TO_CHAR(remoteFilename));
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	progress->SetStatus(msg);
	fullRemoteFilename=remoteDirectory+"/"+remoteFilename;
	fullLocalFilename=localDirectory+"/"+remoteFilename;

	if (wxFileExists(fullLocalFilename))
	{
		wxFile	localFile(fullLocalFilename);
		localFileSize=localFile.Length();
	}
	else
	{
		localFileSize=-1;
	}
	retVal=Connect(ftpConnection);
	if (retVal)
	{
		// Successfully connected to the FTP RemoteMachine
		retVal=ftpConnection.SetBinary();
		if (retVal)
		{
			// Successfully set to Binary mode
			remoteFileSize=ftpConnection.GetFileSize(fullRemoteFilename);

			if ((remoteFileSize==localFileSize)&&(localFileSize!=-1))
			{
				// We don't need to download as a file with the same size
				// is already local
				msg.Printf("Remote file [%s] with size [%d] already downloaded - skipping transfer",
						STRING_TO_CHAR(remoteFilename),remoteFileSize);
				progress->LogError(msg,SeverityOK);
				progress->Update(remoteFileSize);
			}
			else
			{
				if ((localFileSize==-1)||(remoteFileSize==-1))
				{
					// Can't determine a file size so cannot restart
					restartDownload=false;
				}
				else
				{
					if (localFileSize>remoteFileSize)
					{
						// Remote file is smaller than local file
						// so cannot restart
						restartDownload=false;
					}
				}
				if (restartDownload)
				{
					remoteFileStream=ftpConnection.GetInputStreamRestart(fullRemoteFilename,localFileSize);
				}
				if (remoteFileStream==NULL)
				{
					restartDownload=false;
					remoteFileStream=ftpConnection.GetInputStream(fullRemoteFilename);
				}
				if (remoteFileStream==NULL)
				{
					msg.Printf("Could not find [%s] in remote directory [%s]",
								STRING_TO_CHAR(remoteFilename),STRING_TO_CHAR(remoteDirectory));
					progress->LogError(msg,maxErrorSeverity);
					STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
					retVal=false;
				}
				if (retVal)
				{
					if (restartDownload)
					{

						localFileStream=(wxOutputStream *)new wxFFileOutputStream(fullLocalFilename,"a+b");
						if (localFileStream!=NULL)
						{
							localFileStream->SeekO(localFileSize);
						}
						else
						{
							restartDownload=false;
						}
					}
					if (localFileStream==NULL)
					{
						localFileStream=(wxOutputStream *)new wxFileOutputStream(fullLocalFilename);
					}
					
					if (localFileStream==NULL)
					{
						msg.Printf("Could not write [%s] in local directory [%s]",
								STRING_TO_CHAR(remoteFilename),STRING_TO_CHAR(localDirectory));
						progress->LogError(msg,maxErrorSeverity);
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
						retVal=false;
					}
				}

				finished=false;
				bytesRead=0;
				if (restartDownload)
				{
					progress->SetOffset(localFileSize+progress->CurrentValue());
				}
				while ((retVal) && (!finished) && (!remoteFileStream->Eof()))
				{
					remoteFileStream->Read(buffer,sizeof(buffer));
					chunkRead=remoteFileStream->LastRead();
					bytesRead+=chunkRead;
					if (chunkRead>0)
					{
						localFileStream->Write(buffer,chunkRead);
					}
					else
					{
						finished=true;
					}
					progress->Update(bytesRead);
					if ((maxLogfileSize!=0)&&(bytesRead>=(maxLogfileSize*1024*1024)))
					{
						// finished downloading the required amount of logfile data
						finished=true;
					}
					STATSGEN_DEBUG_CODE(msg.Printf("Downloaded [%d] of [%d] bytes",bytesRead,remoteFileSize);)
					STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
				}
			}
			if (remoteFileStream!=NULL)
			{
				delete remoteFileStream;
				remoteFileStream=NULL;
			}
			if (localFileStream!=NULL)
			{
				delete localFileStream;
				localFileStream=NULL;
			}
		}
		else
		{
			ftpResponse=ftpConnection.GetLastResult();
			// Failed to set to Ascii mode
			msg.Printf("Failed to set to ascii mode, FTP Response [%s]",
				STRING_TO_CHAR(ftpResponse));
			progress->LogError(msg,maxErrorSeverity);
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		}
	}
	else
	{
		ftpResponse=ftpConnection.GetLastResult();
		// Failed to connect
		msg.Printf("Failed to connect to [%s] with username [%s] FTP Response [%s]",
				STRING_TO_CHAR(IPAddress),
				STRING_TO_CHAR(FTPUsername),
				STRING_TO_CHAR(ftpResponse));
		progress->LogError(msg,maxErrorSeverity);
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool RemoteMachine::GetRemoteDirectoryListing(wxString &remoteDirectory,
										wxString &remoteWildcard,
										wxArrayString &remoteFiles)
{
	bool			retVal=true;
	wxString		msg;
	wxString		ftpResponse;
	int				remoteFileCount;
	RestartingFTP			ftpConnection;
	wxArrayString	workingRemoteFiles;
	int				remoteFileIndex;
	wxString		remoteFilename;

	wxString		localFilename;

	STATSGEN_DEBUG_FUNCTION_START("RemoteFile","GetRemoteDirectoryListing")
	remoteFiles.Clear();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Connecting")
	retVal=Connect(ftpConnection);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Connected")
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Successfully Connected")
		// Successfully connected to the FTP RemoteMachine
		// Successfully set to ASCII mode
		// Change to the appropriate directory
		if (remoteDirectory.Length()==0)
		{
			// Don't need to change if blank
			retVal=true;
		}
		else
		{
			retVal=ftpConnection.ChDir(remoteDirectory);
		}
		if (retVal)
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Successfully Changed To Remote Directory");
			// Changed to the remote directory
			// Get a directory listing using the wild
			// card setting
			retVal=ftpConnection.GetFilesList(workingRemoteFiles,"");
			if (retVal)
			{
				remoteFileCount=workingRemoteFiles.GetCount();
				STATSGEN_DEBUG_CODE(msg.Printf("Got %d files from directory listing",remoteFileCount);)
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
				for (remoteFileIndex=0;
					remoteFileIndex<remoteFileCount;
					remoteFileIndex++)
				{
					remoteFilename=workingRemoteFiles.Item(remoteFileIndex);
					STATSGEN_DEBUG_CODE(msg.Printf("File [%d]=[%s]",remoteFileIndex,STRING_TO_CHAR(remoteFilename));)
					STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
					if ((remoteWildcard.Length()==0)||
						(wxMatchWild(remoteWildcard,
									remoteFilename,
									false)))
					{
						STATSGEN_DEBUG_CODE(msg.Printf("File [%d]=[%s] matched [%s]", remoteFileIndex, STRING_TO_CHAR(remoteFilename), STRING_TO_CHAR(remoteWildcard));)
						STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
						remoteFiles.Add(remoteFilename);
					}
				}
				remoteFileCount=remoteFiles.GetCount();
				// Got list of remote files
				STATSGEN_DEBUG_CODE(msg.Printf("Found [%d] files to download",remoteFileCount);)
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			}
			else
			{
				ftpResponse=ftpConnection.GetLastResult();
				// Failed to change to the remote directory
				msg.Printf("Failed to get listing of remote directory [%s] with wildcard [%s] FTP Response [%s] trying single file only",
						STRING_TO_CHAR(remoteDirectory),
						STRING_TO_CHAR(remoteWildcard),
						STRING_TO_CHAR(ftpResponse));
				remoteFiles.Add(remoteWildcard);
				remoteFileCount=remoteFiles.GetCount();
				progress->LogError(msg,SeverityCaution);
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
				retVal=true;
			}
		}
		else
		{
			ftpResponse=ftpConnection.GetLastResult();
			// Failed to change to the remote directory
			msg.Printf("Failed to change to remote directory [%s] FTP Response [%s]",
					STRING_TO_CHAR(remoteDirectory),
					STRING_TO_CHAR(ftpResponse));
			progress->LogError(msg,maxErrorSeverity);
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		}
	}
	else
	{
		ftpResponse=ftpConnection.GetLastResult();
		// Failed to connect
		msg.Printf("Failed to connect to [%s] with username [%s] FTP Response [%s]",
				STRING_TO_CHAR(IPAddress),
				STRING_TO_CHAR(FTPUsername),
				STRING_TO_CHAR(ftpResponse));
		progress->LogError(msg,maxErrorSeverity);
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool RemoteMachine::DownloadFileList(
					wxString &remoteDirectory,
					wxString &remoteWildcard,
					wxString &localDirectory)
{
	bool			retVal=true;
	wxString		msg;

	wxString		localFilename;
	wxString		remoteFilename;
	wxArrayString	remoteFiles;
	int				remoteFileCount;
	int				remoteFileIndex;
	RestartingFTP			ftpConnection;

	Connect(ftpConnection);
	retVal=GetRemoteDirectoryListing(remoteDirectory,remoteWildcard,remoteFiles);
	if (retVal)
	{
		remoteFileCount=remoteFiles.GetCount();
		for (remoteFileIndex=0;remoteFileIndex<remoteFileCount;remoteFileIndex++)
		{
			remoteFilename=remoteFiles.Item(remoteFileIndex);
			retVal=GetRemoteFile(
								remoteDirectory,
								remoteFilename,
								localDirectory);
		}
	}

	return (retVal);
}

bool RemoteMachine::PutFile(wxString &remoteDirectory,
							wxString &localFilename,
							int maxSeverity)
{
	wxFileInputStream		*localFileStream=NULL;
	wxString				remoteFilename;
	wxFileName				localName(localFilename);
	bool					retVal;
	wxString				msg;

	STATSGEN_DEBUG_FUNCTION_START("RemoteFile","PutFile")

	retVal=true;
	remoteFilename=localName.GetFullName();

	localFileStream=new wxFileInputStream(localFilename);
	if (localFileStream==NULL)
	{
		msg.Printf("Could not open local file %s",STRING_TO_CHAR(localFilename));
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		progress->LogError(msg,maxErrorSeverity);
		retVal=false;
	}
	if (retVal)
	{
		retVal=PutFile(remoteDirectory,
						localFilename,
						(wxInputStream *)localFileStream,
						remoteFilename,
						maxSeverity);
	}
	if (localFileStream!=NULL)
	{
		delete localFileStream;
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool RemoteMachine::PutFile(wxString &remoteDirectory,
							wxString &localFilename,
							wxInputStream *localFileStream,
							wxString &remoteFilename,
							int maxSeverity)
{
	bool					allOK=true;
	wxOutputStream			*remoteFileStream=NULL;
	wxString				errorMessage;
	int						fileSize;
	int						bytesTransferred;
	unsigned char			buffer[1024];
	wxFile					localFile;
	wxString				message;
	wxString				ftpResponse;
	wxString				msg;
//	wxFTP					ftpConnection;

	STATSGEN_DEBUG_FUNCTION_START("RemoteMachine","PutFile")
	maxErrorSeverity=maxSeverity;
	if (staticFTPConnection!=NULL)
	{
		// Already connected
		allOK=true;
	}
	else
	{
		staticFTPConnection=new RestartingFTP;
		//allOK=Connect(ftpConnection);
		allOK=Connect(*staticFTPConnection);
	}

	if (allOK)
	{
		STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Connected OK")
		if (remoteDirectory.Length()==0)
		{
			allOK=true;
		}
		else
		{
			//allOK=ftpConnection.ChDir(remoteDirectory);
			allOK=staticFTPConnection->ChDir(remoteDirectory);
		}
	}

	if (!allOK)
	{
		msg.Printf("Failed to change to remote directory %s",
					STRING_TO_CHAR(remoteDirectory));
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		progress->LogError(msg,maxErrorSeverity);
	}
	if (allOK)
	{
		//remoteFileStream=ftpConnection.GetOutputStream(remoteFilename);
		remoteFileStream=staticFTPConnection->GetOutputStream(remoteFilename);
		if (remoteFileStream==NULL)
		{
			errorMessage.Printf("Could not create %s in remote directory %s",
								STRING_TO_CHAR(remoteFilename),
								STRING_TO_CHAR(remoteDirectory));
			progress->LogError(errorMessage,maxErrorSeverity);
			STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage)
			allOK=false;
		}
	}

	bytesTransferred=0;

	while (allOK && !localFileStream->Eof())
	{
		localFileStream->Read(buffer,sizeof(buffer));
		if (localFileStream->LastRead()>0)
		{
			remoteFileStream->Write(buffer,localFileStream->LastRead());
		}
		bytesTransferred+=localFileStream->LastRead();
		progress->Update(bytesTransferred);
		STATSGEN_DEBUG_CODE(message.Printf("Transferring %s - %dk", STRING_TO_CHAR(remoteFilename), bytesTransferred/1024);)
		STATSGEN_DEBUG(DEBUG_RARELY,message)
	}
	//if (ftpConnection.GetError()!=wxPROTO_NOERR)
	if (staticFTPConnection!=NULL)
	{
		//if (ftpConnection.GetError()!=wxPROTO_NOERR)
		if (staticFTPConnection->GetError()!=wxPROTO_NOERR)
		{
			msg.Printf("error during transfer of %s",STRING_TO_CHAR(localFilename));
			progress->LogError(msg,maxErrorSeverity);
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			allOK=false;
		}
	}

	if (remoteFileStream!=NULL)
	{
		if (remoteFileStream->GetLastError()!=wxSTREAM_NO_ERROR)
		{
			msg.Printf("error during transfer of %s",STRING_TO_CHAR(localFilename));
			progress->LogError(msg,maxErrorSeverity);
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			allOK=false;
		}
		delete remoteFileStream;
	}
    
	STATSGEN_DEBUG_FUNCTION_END
	return allOK;    
}

long RemoteMachine::GetRemoteFileSize(
							wxString &remoteDirectory,
							wxString &remoteFilename)
{
	wxString			msg;
	bool				retVal;
	wxString			ftpResponse;

	wxString			fullRemoteFilename;
	int					remoteFileSize=0;
	RestartingFTP				ftpConnection;

	STATSGEN_DEBUG_FUNCTION_START("RemoteMachine","GetRemoteFileSize")

	fullRemoteFilename=remoteDirectory+"/"+remoteFilename;


	retVal=Connect(ftpConnection);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Connected OK")
		remoteFileSize=ftpConnection.GetFileSize(fullRemoteFilename);
		STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Got File Size")
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (remoteFileSize);
}

wxString RemoteMachine::GetLastErrorMessage()
{
	return(lastErrorMessage);
}

bool RemoteMachine::Connect(RestartingFTP &ftpConnection)
{
	STATSGEN_DEBUG_FUNCTION_START("RemoteMachine","Connect")
	bool			retVal=true;
	wxString		msg;
	wxIPV4address	address;
	wxString		ftpResponse;
	int				attempts;



	address.Hostname(IPAddress);
	address.Service(FTPPort);

	ftpConnection.SetUser(FTPUsername);
	ftpConnection.SetPassword(FTPPassword);
	ftpConnection.SetPassive(FTPPassive);

	attempts=1;
	retVal=false;
	while ((!retVal)&&(attempts<=MAX_CONNECTION_ATTEMPTS))
	{
		retVal=ftpConnection.Connect(address);
		ftpResponse=ftpConnection.GetLastResult();
		if (!retVal)
		{
			lastErrorMessage.Printf("Connect Attempt %d of %d failed - last response [%s]",
					attempts,MAX_CONNECTION_ATTEMPTS,
					STRING_TO_CHAR(ftpResponse));
			progress->LogError(lastErrorMessage,maxErrorSeverity);
		}
		attempts++;
	}
	if (!retVal)
	{
		STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Connected not ok")
		// Failed to connect
		lastErrorMessage.Printf("Failed to connect to [%s] with username [%s] FTP Response [%s]",
				STRING_TO_CHAR(IPAddress),
				STRING_TO_CHAR(FTPUsername),
				STRING_TO_CHAR(ftpResponse));
		progress->LogError(lastErrorMessage,maxErrorSeverity);
		STATSGEN_DEBUG(DEBUG_ALWAYS,lastErrorMessage);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

/*
void RemoteMachine::Disconnect()
{
	ftpConnection.Destroy();
}
*/

bool RemoteMachine::ChangeDirectory(wxString &fullPath)
{
	bool		retVal;
	wxString	newPath;
	RestartingFTP		ftpConnection;

	STATSGEN_DEBUG_FUNCTION_START("RemoteMachine","ChangeDirectory")
	Connect(ftpConnection);
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Connected")
	if (fullPath.Length()==0)
	{
		// don't need to change if blank
		retVal=true;
	}
	else
	{
		retVal=ftpConnection.ChDir(fullPath);
	}
	STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Directory Changed")
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Current Directory Changed")
		newPath=ftpConnection.Pwd();
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Current Directory Changed")
		fullPath=newPath;
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool RemoteMachine::PutFile(wxString &remoteDirectory,
							wxString &localFilename)
{
	bool	allOK;
	int		attemptsLeft;

	maxErrorSeverity=SeverityCaution;

	attemptsLeft=FTPRetries+1;
	allOK=false;
	while ((!allOK) && (attemptsLeft>0))
	{
		if (attemptsLeft>1)
		{
			maxErrorSeverity=SeverityCaution;
		}
		else
		{
			maxErrorSeverity=SeverityError;
		}
		allOK=PutFile(remoteDirectory,localFilename,maxErrorSeverity);
		if (!allOK)
		{
			delete staticFTPConnection;
			staticFTPConnection=NULL;
		}
		attemptsLeft--;
	}
	maxErrorSeverity=SeverityError;
        
	return allOK;    
}

void RemoteMachine::MaxErrorSeverity(int severity)
{
	maxErrorSeverity=severity;
}

bool RemoteMachine::MakeDir(wxString &remoteDirectory,
							wxString &newDirectory)
{
	bool					allOK=true;
	wxString				errorMessage;
	wxString				message;
	wxString				ftpResponse;
	wxString				msg;
//	wxFTP					ftpConnection;

	STATSGEN_DEBUG_FUNCTION_START("RemoteMachine","MakeDir")
	if (staticFTPConnection!=NULL)
	{
		// Already connected
		allOK=true;
	}
	else
	{
		staticFTPConnection=new RestartingFTP;
		//allOK=Connect(ftpConnection);
		allOK=Connect(*staticFTPConnection);
	}

	if (allOK)
	{
		STATSGEN_DEBUG(DEBUG_SOMETIMES,(char *)"Connected OK")
		if (remoteDirectory.Length()==0)
		{
			allOK=true;
		}
		else
		{
			//allOK=ftpConnection.ChDir(remoteDirectory);
			allOK=staticFTPConnection->ChDir(remoteDirectory);
		}
	}
	if (!allOK)
	{
		msg.Printf("Failed to change to remote directory %s",
						STRING_TO_CHAR(remoteDirectory));
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		progress->LogError(msg,maxErrorSeverity);
	}
	else
	{
		allOK=staticFTPConnection->MkDir(newDirectory);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return allOK;    
}

