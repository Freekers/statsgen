// wxWindows includes
#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dynarray.h>
#include <wx/dir.h>
#include <wx/datetime.h>
#include <wx/protocol/ftp.h>

// Statsgen includes
#include "Server.h"
#include "ErrorData.h"
#include "GlobalStatistics.h"
#include "RemoteMachine.h"
#include "Progress.h"
#include "PunkbusterBanFile.h"
#include "CODBanFile.h"
#include "QuakeWarsLogFile.h"

char *ServerTypeCodes[SERVER_TYPE_COUNT]=
{
	SERVER_TYPE_COD1,
	SERVER_TYPE_COD2,
	SERVER_TYPE_COD4,
	SERVER_TYPE_COD5,
	SERVER_TYPE_MOHAA,
	SERVER_TYPE_QUAKEWARS,
	SERVER_TYPE_WOLFENSTEIN
};

char *ServerTypeNames[SERVER_TYPE_COUNT]=
{
	SERVER_TYPE_NAME_COD1,
	SERVER_TYPE_NAME_COD2,
	SERVER_TYPE_NAME_COD4,
	SERVER_TYPE_NAME_COD5,
	SERVER_TYPE_NAME_MOHAA,
	SERVER_TYPE_NAME_QUAKEWARS,
	SERVER_TYPE_NAME_WOLFENSTEIN
};

char *SkillTypeCodes[SKILL_TYPE_COUNT]=
{
	STAT_TYPE_CODE_WEAPON,
	STAT_TYPE_CODE_LOCATION,
	STAT_TYPE_CODE_GAMETYPE
};

char *SkillTypeNames[SKILL_TYPE_COUNT]=
{
	STAT_TYPE_NAME_WEAPON,
	STAT_TYPE_NAME_LOCATION,
	STAT_TYPE_NAME_GAMETYPE
};

char *ImageTypeCodes[IMAGE_TYPE_COUNT]=
{
	STAT_TYPE_CODE_WEAPON,
	STAT_TYPE_CODE_MAP,
	STAT_TYPE_CODE_LOCATION,
	STAT_TYPE_CODE_GAMETYPE,
	STAT_TYPE_CODE_TEAM,
	STAT_TYPE_CODE_ACTION,
	STAT_TYPE_CODE_CLASS
};

char *ImageTypeNames[IMAGE_TYPE_COUNT]=
{
	STAT_TYPE_NAME_WEAPON,
	STAT_TYPE_NAME_MAP,
	STAT_TYPE_NAME_LOCATION,
	STAT_TYPE_NAME_GAMETYPE,
	STAT_TYPE_NAME_TEAM,
	STAT_TYPE_NAME_ACTION,
	STAT_TYPE_NAME_CLASS
};

wxString	sortingWildCard;

Server::Server()
{
	STATSGEN_DEBUG_FUNCTION_START("Server","Constructor")
	ID="";
	logFiles=NULL;
	restartDownload=false;
	STATSGEN_DEBUG_FUNCTION_END
}

Server::~Server()
{
	STATSGEN_DEBUG_FUNCTION_START("Server","Destructor")
	if (logFiles!=NULL)
	{
		delete (logFiles);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void Server::SetID(wxString &IDIn)
{
	ID=IDIn;
}

bool Server::Initiate()
{
	bool		retVal=false;
	wxString	configBaseKey;
	wxString	configKey;
	wxString	configValue;

	if (ID.Length()>0)
	{
		// Read in the configuration items for this server
		configBaseKey="/Server"+ID;
		configBaseKey+="/";

		configKey=configBaseKey+"MaxLogfileSize";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"0");
		maxLogfileSize=atol(configValue.GetData());

		configKey=configBaseKey+"archiveDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		archiveDirectory=configValue;

		configKey=configBaseKey+"archiveWildcard";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		archiveWildcard=configValue;

		configKey=configBaseKey+"latestDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		latestDirectory=configValue;

		configKey=configBaseKey+"latestFilename";
		globalStatistics.configData.ReadTextValue(configKey,&configValue);
		latestFilename=configValue;

		configKey=configBaseKey+"secondaryArchiveDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		secondaryArchiveDirectory=configValue;

		configKey=configBaseKey+"secondaryArchiveWildcard";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		secondaryArchiveWildcard=configValue;

		configKey=configBaseKey+"secondaryLatestDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		secondaryLatestDirectory=configValue;

		configKey=configBaseKey+"secondaryLatestFilename";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		secondaryLatestFilename=configValue;

		configKey=configBaseKey+"serverType";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,SERVER_TYPE_COD1);
		serverType=configValue;

		configKey=configBaseKey+"banFile1Type";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,SERVER_BAN_TYPE_COD);
		banFile1Type=configValue;

		configKey=configBaseKey+"Ban1Directory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		ban1Directory=configValue;

		configKey=configBaseKey+"FTPBanFile1Directory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPBanFile1Directory=configValue;

		configKey=configBaseKey+"FTPBanFile1Filename";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPBanFile1Filename=configValue;

		configKey=configBaseKey+"banFile2Type";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,SERVER_BAN_TYPE_COD);
		banFile2Type=configValue;

		configKey=configBaseKey+"Ban2Directory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		ban2Directory=configValue;

		configKey=configBaseKey+"FTPBanFile2Directory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPBanFile2Directory=configValue;

		configKey=configBaseKey+"FTPBanFile2Filename";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPBanFile2Filename=configValue;

		configKey=configBaseKey+"gameIP";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		gameIP=configValue;

		configKey=configBaseKey+"gamePort";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		gamePort=-1;
		if (configValue.Length()>0)
		{
			gamePort=atoi(configValue.GetData());
		}

		configKey=configBaseKey+"FTPEnabled";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"n");
		FTPEnabled=(configValue.CmpNoCase("y")==0);

		configKey=configBaseKey+"FTPRestartDownload";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"y");
		restartDownload=(configValue.CmpNoCase("y")==0);

		configKey=configBaseKey+"FTPArchiveDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPArchiveDirectory=configValue;

		configKey=configBaseKey+"FTPArchiveWildcard";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPArchiveWildcard=configValue;

		configKey=configBaseKey+"FTPLatestDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPLatestDirectory=configValue;

		configKey=configBaseKey+"FTPLatestFilename";
		globalStatistics.configData.ReadTextValue(configKey,&configValue);
		FTPLatestFilename=configValue;

		configKey=configBaseKey+"FTPSecondaryArchiveDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPSecondaryArchiveDirectory=configValue;

		configKey=configBaseKey+"FTPSecondaryArchiveWildcard";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPSecondaryArchiveWildcard=configValue;

		configKey=configBaseKey+"FTPSecondaryLatestDirectory";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		FTPSecondaryLatestDirectory=configValue;

		configKey=configBaseKey+"FTPSecondaryLatestFilename";
		globalStatistics.configData.ReadTextValue(configKey,&configValue);
		FTPSecondaryLatestFilename=configValue;

		if (latestFilename.Length()==0)
		{
			latestFilename=FTPLatestFilename;
		}
		if (archiveWildcard.Length()==0)
		{
			archiveWildcard=FTPArchiveWildcard;
		}
		if (secondaryLatestFilename.Length()==0)
		{
			secondaryLatestFilename=FTPSecondaryLatestFilename;
		}
		if (secondaryArchiveWildcard.Length()==0)
		{
			secondaryArchiveWildcard=FTPSecondaryArchiveWildcard;
		}
		configKey=configBaseKey+"GuessTeamIDs";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"y");
		fakeTeamIDs=(configValue.CmpNoCase("y")==0);
	}

	return (retVal);
}

bool Server::DownloadFiles()
{
	bool			retVal=true;
	wxString		serverID;
	wxArrayString	archiveFiles;
	wxArrayString	currentFiles;
	wxArrayString	secondaryArchiveFiles;
	wxArrayString	secondaryCurrentFiles;
	bool			archiveEnabled=false;
	bool			currentEnabled=false;
	bool			secondaryArchiveEnabled=false;
	bool			secondaryCurrentEnabled=false;
	long			totalFileSize=0;
	long			currentFileSize=0;
	int				fileIndex=0;
	int				fileCount=0;
	wxString		remoteFilename;
	wxString		msg;

	STATSGEN_DEBUG_FUNCTION_START("Server","DownloadFiles")
	
	progress->Initiate(totalFileSize,
							" kb",
							1024,
							"kb",
							1024);
	progress->Update(0);
	if (FTPEnabled)
	{
		if ((archiveDirectory.Length()>0) &&
			(archiveWildcard.Length()>0) &&
			(FTPArchiveDirectory.Length()>0) &&
			(FTPArchiveWildcard.Length()>0))
		{
			archiveEnabled=true;
		}
		if ((secondaryArchiveDirectory.Length()>0) &&
			(secondaryArchiveWildcard.Length()>0) &&
			(FTPSecondaryArchiveDirectory.Length()>0) &&
			(FTPSecondaryArchiveWildcard.Length()>0))
		{
			secondaryArchiveEnabled=true;
		}
		if (latestFilename.Length()==0)
		{
			latestFilename=FTPLatestFilename;
		}
		if ((latestDirectory.Length()>0) &&
			(latestFilename.Length()>0) &&
			(FTPLatestDirectory.Length()>0) &&
			(FTPLatestFilename.Length()>0))
		{
			currentEnabled=true;
		}
		if (secondaryLatestFilename.Length()==0)
		{
			secondaryLatestFilename=FTPSecondaryLatestFilename;
		}
		if ((secondaryLatestDirectory.Length()>0) &&
			(secondaryLatestFilename.Length()>0) &&
			(FTPSecondaryLatestDirectory.Length()>0) &&
			(FTPSecondaryLatestFilename.Length()>0))
		{
			secondaryCurrentEnabled=true;
		}

		if ((!archiveEnabled)&&(!currentEnabled)&&
			(!secondaryArchiveEnabled)&&(!secondaryCurrentEnabled))
		{
			// We have FTP enabled but no logfile downloading enabled
			STATSGEN_DEBUG(DEBUG_ALWAYS,"FTP Download enabled, but no directory details");
			msg.Printf("FTP Enabled but no directories and/or filenames setup for this server.Latest Directory=[%s], Filename=[%s], FTP Directory [%s], FTP Latest Filename [%s]",
						latestDirectory.GetData(),
						latestFilename.GetData(),
						FTPLatestDirectory.GetData(),
						FTPLatestFilename.GetData());
			progress->LogError(msg,SeverityError);
		}
	}
	if ((archiveEnabled)||(currentEnabled)||
		(secondaryArchiveEnabled)||(secondaryCurrentEnabled))
	{
		msg="Getting Directory Listing";
		progress->SetStatus(msg);
		serverID="Server"+ID;
		RemoteMachine	remoteMachine(serverID);
		if (archiveEnabled)
		{
			wxString	wildCard;

			wildCard=WildCardFormatToWildCard(FTPArchiveWildcard);
			retVal=remoteMachine.GetRemoteDirectoryListing(
											FTPArchiveDirectory,
											wildCard,
											archiveFiles);
			if (retVal)
			{
				fileCount=archiveFiles.GetCount();
				for (fileIndex=0;fileIndex<fileCount;fileIndex++)
				{
					remoteFilename=archiveFiles.Item(fileIndex);
					currentFileSize=remoteMachine.GetRemoteFileSize(
														FTPArchiveDirectory,
														remoteFilename);
					totalFileSize+=currentFileSize;
				}
			}
			else
			{
				msg.Printf("Failed to get directory listing [%s] using [%s]",
					FTPArchiveDirectory.GetData(),
					FTPArchiveWildcard.GetData());
				progress->LogError(msg,SeverityError);
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			}
		}
		if (currentEnabled)
		{
			retVal=remoteMachine.GetRemoteDirectoryListing(
											FTPLatestDirectory,
											FTPLatestFilename,
											currentFiles);
			if (retVal)
			{
				fileCount=currentFiles.GetCount();
				for (fileIndex=0;fileIndex<fileCount;fileIndex++)
				{
					remoteFilename=currentFiles.Item(fileIndex);
					currentFileSize=remoteMachine.GetRemoteFileSize(
														FTPLatestDirectory,
														remoteFilename);
					totalFileSize+=currentFileSize;
				}
			}
			else
			{
				msg.Printf("Failed to get directory listing [%s] using [%s]",
					FTPLatestDirectory.GetData(),
					FTPLatestFilename.GetData());
				progress->LogError(msg,SeverityError);
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			}
			if (currentFiles.GetCount()==0)
			{
				if (FTPLatestFilename.Length()>0)
				{
					currentFiles.Add(FTPLatestFilename);
				}
			}
		}
		if (secondaryArchiveEnabled)
		{
			retVal=remoteMachine.GetRemoteDirectoryListing(
											FTPSecondaryArchiveDirectory,
											FTPSecondaryArchiveWildcard,
											secondaryArchiveFiles);
			if (retVal)
			{
				fileCount=secondaryArchiveFiles.GetCount();
				for (fileIndex=0;fileIndex<fileCount;fileIndex++)
				{
					remoteFilename=secondaryArchiveFiles.Item(fileIndex);
					currentFileSize=remoteMachine.GetRemoteFileSize(
														FTPSecondaryArchiveDirectory,
														remoteFilename);
					totalFileSize+=currentFileSize;
				}
			}
			else
			{
				msg.Printf("Failed to get directory listing [%s] using [%s]",
					FTPSecondaryArchiveDirectory.GetData(),
					FTPSecondaryArchiveWildcard.GetData());
				progress->LogError(msg,SeverityError);
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			}
		}
		if (secondaryCurrentEnabled)
		{
			retVal=remoteMachine.GetRemoteDirectoryListing(
											FTPSecondaryLatestDirectory,
											FTPSecondaryLatestFilename,
											secondaryCurrentFiles);
			if (retVal)
			{
				fileCount=secondaryCurrentFiles.GetCount();
				for (fileIndex=0;fileIndex<fileCount;fileIndex++)
				{
					remoteFilename=secondaryCurrentFiles.Item(fileIndex);
					currentFileSize=remoteMachine.GetRemoteFileSize(
														FTPSecondaryLatestDirectory,
														remoteFilename);
					totalFileSize+=currentFileSize;
				}
			}
			else
			{
				msg.Printf("Failed to get directory listing [%s] using [%s]",
					FTPSecondaryLatestDirectory.GetData(),
					FTPSecondaryLatestFilename.GetData());
				progress->LogError(msg,SeverityError);
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			}
			if (secondaryCurrentFiles.GetCount()==0)
			{
				if (FTPSecondaryLatestFilename.Length()>0)
				{
					secondaryCurrentFiles.Add(FTPSecondaryLatestFilename);
				}
			}
		}
		progress->Initiate(totalFileSize,
							" kb",
							1024,
							"kb",
							1024);
		fileCount=archiveFiles.GetCount();
		//progress->SetStatus(msg);
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			remoteFilename=archiveFiles.Item(fileIndex);
			progress->SetOffset(progress->CurrentValue());
			remoteMachine.GetRemoteFileRestart(FTPArchiveDirectory,
										remoteFilename,
										archiveDirectory,
										restartDownload,
										maxLogfileSize);
		}
		fileCount=currentFiles.GetCount();
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			remoteFilename=currentFiles.Item(fileIndex);
			progress->SetOffset(progress->CurrentValue());
			remoteMachine.GetRemoteFileRestart(FTPLatestDirectory,
										remoteFilename,
										latestDirectory,
										restartDownload,
										maxLogfileSize);
		}
		fileCount=secondaryArchiveFiles.GetCount();
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			remoteFilename=secondaryArchiveFiles.Item(fileIndex);
			progress->SetOffset(progress->CurrentValue());
			remoteMachine.GetRemoteFileRestart(FTPSecondaryArchiveDirectory,
										remoteFilename,
										secondaryArchiveDirectory,
										restartDownload,
										maxLogfileSize);
		}
		fileCount=secondaryCurrentFiles.GetCount();
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			remoteFilename=secondaryCurrentFiles.Item(fileIndex);
			progress->SetOffset(progress->CurrentValue());
			remoteMachine.GetRemoteFileRestart(FTPSecondaryLatestDirectory,
										remoteFilename,
										secondaryLatestDirectory,
										restartDownload,
										maxLogfileSize);
		}
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"FTP Download not enabled");
		msg="Transfer Not Enabled";
		progress->LogError(msg,SeverityCaution);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}


bool Server::FindLogfiles()
{
	// Find all the logfiles identified using the archive and latest directories / wildcards
	wxArrayString	archiveFiles;
	wxArrayString	latestFiles;
	wxArrayString	secondaryArchiveFiles;
	wxArrayString	secondaryLatestFiles;
	wxString		msg;
	int				count;
	int				secondaryCount;
	int				index;
	wxString		filename;
	bool			retVal=true;

	STATSGEN_DEBUG_FUNCTION_START("Server","FindLogfiles")

	if (serverType.CmpNoCase(SERVER_TYPE_QUAKEWARS)==0)
	{
		// Quake Wars is a pain in the arse with it's secondary "objective" based
		// logfile - what a bunch of cocks id are for doing this
		if ((secondaryArchiveDirectory.Length()>0) &&
			(secondaryArchiveWildcard.Length()>0))
		{
			wxString wildCard;

			wildCard=WildCardFormatToWildCard(secondaryArchiveWildcard);
			STATSGEN_DEBUG_CODE(msg.Printf("Secondary Archive Enabled for directory [%s] wildcard [%s]",
					secondaryArchiveDirectory.GetData(),
					wildCard.GetData());)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			if (wxDir::Exists(secondaryArchiveDirectory))
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,"Getting Secondary Archive File List")
				wxDir::GetAllFiles(secondaryArchiveDirectory,
									&secondaryArchiveFiles,
									wildCard,
									wxDIR_FILES);
			}
			else
			{
				// Archiving enabled - but directory does not exist
				retVal=false;
				msg.Printf("Secondary Archive Directory [%s] does not exist",secondaryArchiveDirectory.GetData());
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
				progress->LogError(msg,SeverityError);
			}
			SortFiles(secondaryArchiveFiles,secondaryArchiveWildcard);
		}
		else
		{
			// Archive directory access disabled - this is ok
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Secondary Archive directory access disabled")
			retVal=true;
		}
		if ((secondaryLatestDirectory.Length()>0) &&
			(secondaryLatestFilename.Length()>0))
		{
			STATSGEN_DEBUG_CODE(msg.Printf("Secondary Latest Enabled for directory [%s] wildcard [%s]",
					secondaryLatestDirectory.GetData(),
					secondaryLatestFilename.GetData());)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			if (wxDir::Exists(secondaryLatestDirectory))
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,"Getting Secondary Latest File List");
				wxDir::GetAllFiles(secondaryLatestDirectory,
									&secondaryLatestFiles,
									secondaryLatestFilename,
									wxDIR_FILES);
			}
			else
			{
				// latest directory enabled but directory does not exist
				retVal=false;
				msg.Printf("Secondary Latest Directory [%s] does not exist",secondaryLatestDirectory.GetData());
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
				progress->LogError(msg,SeverityError);
			}
		}
		else
		{
			retVal=true;
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Latest directory access disabled");
		}
	}

	if ((archiveDirectory.Length()>0) &&
		(archiveWildcard.Length()>0))
	{
		wxString	wildCard;
		wildCard=WildCardFormatToWildCard(archiveWildcard);
		STATSGEN_DEBUG_CODE(msg.Printf("Archive Enabled for directory [%s] wildcard [%s]",
				archiveDirectory.GetData(),
				wildCard.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		if (wxDir::Exists(archiveDirectory))
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Getting Archive File List")
			wxDir::GetAllFiles(archiveDirectory,
								&archiveFiles,
								wildCard,
								wxDIR_FILES);
		}
		else
		{
			// Archiving enabled - but directory does not exist
			retVal=false;
			msg.Printf("Archive Directory [%s] does not exist",archiveDirectory.GetData());
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			progress->LogError(msg,SeverityError);
		}
		SortFiles(archiveFiles,archiveWildcard);
	}
	else
	{
		// Archive directory access disabled - this is ok
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Archive directory access disabled")
		retVal=true;
	}
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"After Archive we are ok")
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"After Archive we are not ok")
	}
	if ((latestDirectory.Length()>0) &&
		(latestFilename.Length()>0))
	{
		STATSGEN_DEBUG_CODE(msg.Printf("latest Enabled for directory [%s] wildcard [%s]",
				latestDirectory.GetData(),
				latestFilename.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		if (wxDir::Exists(latestDirectory))
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,"Getting Latest File List");
			wxDir::GetAllFiles(latestDirectory,
								&latestFiles,
								latestFilename,
								wxDIR_FILES);
		}
		else
		{
			// latest directory enabled but directory does not exist
			retVal=false;
			msg.Printf("Latest Directory [%s] does not exist",latestDirectory.GetData());
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			progress->LogError(msg,SeverityError);
		}
	}
	else
	{
		retVal=true;
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Latest directory access disabled");
	}

	logFilenames.Clear();
	secondaryLogFilenames.Clear();

	if (retVal)
	{
		count=archiveFiles.GetCount();
		STATSGEN_DEBUG_CODE(msg.Printf("Archive Files Found = %d\n",count);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		for (index=0;index<count;index++)
		{
			filename=archiveFiles.Item(index);
			logFilenames.Add(filename);
		}
		count=latestFiles.GetCount();
		STATSGEN_DEBUG_CODE(msg.Printf("Latest Files Found = %d\n",count);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		for (index=0;index<count;index++)
		{
			filename=latestFiles.Item(index);
			logFilenames.Add(filename);
		}
		if (serverType.CmpNoCase(SERVER_TYPE_QUAKEWARS)==0)
		{
			count=secondaryArchiveFiles.GetCount();
			STATSGEN_DEBUG_CODE(msg.Printf("Secondary Archive Files Found = %d\n",count);)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			for (index=0;index<count;index++)
			{
				filename=secondaryArchiveFiles.Item(index);
				secondaryLogFilenames.Add(filename);
			}
			count=secondaryLatestFiles.GetCount();
			STATSGEN_DEBUG_CODE(msg.Printf("Secondary Latest Files Found = %d\n",count);)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			for (index=0;index<count;index++)
			{
				filename=secondaryLatestFiles.Item(index);
				secondaryLogFilenames.Add(filename);
			}
		}
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"retVal is false???")
	}

	count=logFilenames.GetCount();
	secondaryCount=secondaryLogFilenames.GetCount();
	STATSGEN_DEBUG_CODE(msg.Printf("%d logfiles found",count);)
	STATSGEN_DEBUG_CODE(msg.Printf("%d secondary logfiles found",secondaryCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
/*
	if (serverType.CmpNoCase(SERVER_TYPE_QUAKEWARS)==0)
	{
		if (count!=secondaryCount)
		{
			msg.Printf("Server Type is Quake Wars and secondary logfile(objective) count %d, does not match primary logfile count %d - you are going to get unexpected results",secondaryCount,count);
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			progress->LogError(msg,SeverityError);
		}
	}
*/
	for (index=0;index<count;index++)
	{
		filename=logFilenames.Item(index);
		STATSGEN_DEBUG_CODE(msg.Printf("Logfile [%d] = [%s]",index,filename.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	}
	for (index=0;index<secondaryCount;index++)
	{
		filename=secondaryLogFilenames.Item(index);
		STATSGEN_DEBUG_CODE(msg.Printf("Secondary Logfile [%d] = [%s]",index,filename.GetData());)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool Server::CreateLogFiles()
{
	wxString			msg;
	QuakeWarsLogFile	*quakeWarsLogFile;
	LogFile				*standardLogFile;
	bool				retVal=true;
	long				lastRoundEndedAt=0;
	long				secondaryLastRoundEndedAt=0;

	STATSGEN_DEBUG_FUNCTION_START("Server","CreateLogFiles")

	if (logFiles!=NULL)
	{
		// Pre-existing logfile
		// grab the "last round" values and re-add them back 
		// after logfile created
		logFiles->RetrieveLastRoundPositions(&lastRoundEndedAt,
											&secondaryLastRoundEndedAt);
		delete (logFiles);
		logFiles=NULL;
	}

	if (serverType.Cmp("QUAKEWARS")==0)
	{
		quakeWarsLogFile=new QuakeWarsLogFile();
		quakeWarsLogFile->Initiate(serverType,logFilenames,secondaryLogFilenames);
		logFiles=quakeWarsLogFile;
	}
	else
	{
		standardLogFile=new LogFile();
		standardLogFile->Initiate(serverType,logFilenames);
		logFiles=standardLogFile;
		standardLogFile->FakeTeamIDs(fakeTeamIDs);
	}
	logFiles->SetLastRoundPositions(lastRoundEndedAt,
									secondaryLastRoundEndedAt);

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

long Server::LogfileSize()
{
	return (logFiles->Size());
}

bool Server::ProcessLogFiles(int serverIndex)
{
	bool		retVal=true;

	STATSGEN_DEBUG_FUNCTION_START("Server","ProcessLogFiles")

	logfileVariableKeys.Clear();
	logfileVariableValues.Clear();
	logfilesSize=logFiles->Size();
	progress->Initiate(
			logfilesSize,
			" kb",
			1024,
			"kb",
			1024);
	logFiles->Process(this,serverIndex,maxLogfileSize);

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool Server::DownloadBanFiles()
{
	bool			retVal=true;
	wxString		serverID;
	wxString		msg;
	int				banFilesIndex;
	wxString		remoteDirectory;
	wxString		remoteFilename;
	wxString		localDirectory;
	wxString		thisBanType;
	long			currentProgress;
	wxArrayString	remoteFiles;
	
	if (FTPEnabled)
	{
		serverID="Server"+ID;
		RemoteMachine	remoteMachine(serverID);
		for (banFilesIndex=1;banFilesIndex<3;banFilesIndex++)
		{
			switch (banFilesIndex)
			{
				case 1:
					remoteDirectory=FTPBanFile1Directory;
					remoteFilename=FTPBanFile1Filename;
					thisBanType=banFile1Type;
					localDirectory=ban1Directory;
					break;
				case 2:
					remoteDirectory=FTPBanFile2Directory;
					remoteFilename=FTPBanFile2Filename;
					thisBanType=banFile2Type;
					localDirectory=ban2Directory;
					break;
			}
			if ((remoteDirectory.Length()>0) &&
				(remoteFilename.Length()>0) &&
				(localDirectory.Length()>0))
			{
				currentProgress=progress->CurrentValue();
				// For some reason i have to do a directory
				// listing otherwise connection can fail
				remoteMachine.MaxErrorSeverity(SeverityOK);
				remoteMachine.GetRemoteDirectoryListing(
											remoteDirectory,
											remoteFilename,
											remoteFiles);
				remoteMachine.MaxErrorSeverity(SeverityError);
				remoteMachine.GetRemoteFile(remoteDirectory,
										remoteFilename,
										localDirectory);
				progress->Update(currentProgress);

				// Create a ban file object based upon the ban type
			}
		}
	}

	return (retVal);
}

bool Server::ProcessBanFiles()
{
	bool			retVal=true;
	wxString		msg;
	int				banFilesIndex;
	wxString		remoteFilename;
	wxString		localDirectory;
	wxString		thisBanType;
	//BanFile			*banFile;
	
	for (banFilesIndex=1;banFilesIndex<3;banFilesIndex++)
	{
		switch (banFilesIndex)
		{
			case 1:
				remoteFilename=FTPBanFile1Filename;
				thisBanType=banFile1Type;
				localDirectory=ban1Directory;
				break;
			case 2:
				remoteFilename=FTPBanFile2Filename;
				thisBanType=banFile2Type;
				localDirectory=ban2Directory;
				break;
		}
		if ((remoteFilename.Length()>0) &&
			(localDirectory.Length()>0))
		{
			if (thisBanType.Cmp("COD")==0)
			{
				wxFileName	filename;
				filename.AssignDir(localDirectory);
				filename.SetFullName(remoteFilename);

				CODBanFile	banfile(filename);

				banfile.AddToDropList();
			}
			if (thisBanType.Cmp("PUNKBUSTER")==0)
			{
				wxFileName	filename;
				filename.AssignDir(localDirectory);
				filename.SetFullName(remoteFilename);

				PunkbusterBanFile	banfile(filename);

				banfile.AddToDropList();
			}
		}
	}

	return (retVal);
}

void Server::StartFromBeginning()
{
	logFiles->StartFromBeginning();
}

void Server::InitiateMessageCentre()
{
	messageCentre.SetID(ID);
	messageCentre.Initiate();
}

void Server::SendMessage()
{
	messageCentre.SendMessage();
}

void Server::AddVariable(wxString &variable,wxString &value)
{
	int			variableCount;
	int			variableIndex;
	wxString	key;

	variableCount=logfileVariableKeys.GetCount();
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		key=logfileVariableKeys.Item(variableIndex);
		if (key.Cmp(variable)==0)
		{
			// remove the existing variable and value
			logfileVariableKeys.RemoveAt(variableIndex);
			logfileVariableValues.RemoveAt(variableIndex);
			break;
		}
	}
	logfileVariableKeys.Add(variable);
	logfileVariableValues.Add(value);
}

wxString Server::SQLTableName()
{
	wxString	name="servers";

	return (name);
}

wxString Server::SQLVariableTableName()
{
	wxString	name="servervariables";

	return (name);
}

wxString Server::SQLProgressTableName()
{
	wxString	name="serverprogress";

	return (name);
}

wxString Server::SQLCreateTable()
{
	wxString	SQL;

	SQL.Printf("create table %s"
			"("
				"serveridx integer,"
				"%s,"
				"logfilesize integer,"
				"%s,"
				"gameport int"
			")",
			SQLTableName().GetData(),
			StatsgenDatabase::StringFieldDefinition("servertype","servertype",FIELD_WIDTH_SERVER_TYPE).GetData(),
			StatsgenDatabase::StringFieldDefinition("hostname","serverhostname",FIELD_WIDTH_HOSTNAME).GetData()
);

	return (SQL);
}

wxString Server::SQLCreateVariableTable()
{
	wxString	SQL;

	SQL.Printf("create table %s"
			"("
				"serveridx integer,"
				"%s,"
				"%s"
			")",
			SQLVariableTableName().GetData(),
			StatsgenDatabase::StringFieldDefinition("variable","servervariable",FIELD_WIDTH_VARIABLE).GetData(),
			StatsgenDatabase::StringFieldDefinition("value","servervalue",FIELD_WIDTH_VALUE).GetData()
			);

	return (SQL);
}

wxString Server::SQLCreateProgressTable()
{
	wxString	SQL;

	SQL.Printf("create table %s"
			"("
				"serverid string,"
				"primaryposition integer,"
				"secondaryposition integer"
			")",
			SQLProgressTableName().GetData());

	return (SQL);
}

void Server::WriteToDatabase(int serverIndex)
{
	wxString	SQL;
	int			variableCount;
	int			variableIndex;
	wxString	variable;
	wxString	value;
	wxString	serverID;

	serverID="Server"+ID;
	RemoteMachine	remoteMachine(serverID);

	SQL.Printf("insert into %s "
				"(serveridx,servertype,logfilesize,hostname,gameport) "
				"values ('%d','%s','%ld','%s','%d')",
				Server::SQLTableName().GetData(),
				serverIndex,
				serverType.GetData(),
				logfilesSize,
				remoteMachine.IPAddress.GetData(),
				gamePort);

	globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	variableCount=logfileVariableKeys.GetCount();
	if (variableCount>0)
	{
		// if we have variables to process then delete any existing
		// ones first
		SQL.Printf("delete from %s where serveridx=%d",
			Server::SQLVariableTableName().GetData(),
			serverIndex);
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	}
	for (variableIndex=0;variableIndex<variableCount;variableIndex++)
	{
		variable=StatsgenDatabase::SafeForInsert(logfileVariableKeys.Item(variableIndex));
		value=StatsgenDatabase::SafeForInsert(logfileVariableValues.Item(variableIndex));
		SQL.Printf("insert into %s "
					"(serveridx,variable,value) "
					"values ('%d','%s','%s')",
					Server::SQLVariableTableName().GetData(),
					serverIndex,
					variable.GetData(),
					value.GetData());
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
	}
	if (logFiles!=NULL)
	{
		long	primaryPosition;
		long	secondaryPosition;

		// Delete existing progress first
		SQL.Printf("delete from %s where serverid='%s'",
			Server::SQLProgressTableName().GetData(),
			StatsgenDatabase::SafeForInsert(ID).GetData());
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);

		logFiles->RetrieveLastRoundPositions(&primaryPosition,&secondaryPosition);
		SQL.Printf("insert into %s "
					"(serverid, primaryposition, secondaryposition) "
					"values ('%s','%ld','%ld')",
					Server::SQLProgressTableName().GetData(),
					StatsgenDatabase::SafeForInsert(ID).GetData(),
					primaryPosition,
					secondaryPosition);
		globalStatistics.statsgenDatabase.SimpleExecute(SQL);
					
	}
}

void Server::AdjustLogfilePosition()
{
	TemplateOpenQuery	query;
	wxString			sql;
	long				primaryPosition;
	long				secondaryPosition;
	wxString			fieldValue;
	wxString			property;

	primaryPosition=0;
	secondaryPosition=0;
	sql.Printf(
		"select primaryposition,secondaryposition from %s where serverid=\'%s\' limit 1",
		SQLProgressTableName().GetData(),
		StatsgenDatabase::SafeForInsert(ID).GetData());
	query.Initiate(sql,globalStatistics.statsgenDatabase.DBHandle());
	if (query.NextRow())
	{
		property="primaryposition";
		fieldValue=query.RetrieveProperty(property);
		primaryPosition=atol(fieldValue.GetData());

		property="secondaryposition";
		fieldValue=query.RetrieveProperty(property);
		secondaryPosition=atol(fieldValue.GetData());
	}

	if (logFiles!=NULL)
	{
		logFiles->SetLastRoundPositions(primaryPosition,secondaryPosition);
	}
}

wxString Server::WildCardFormatToWildCard(wxString &wildCardFormat)
{
	wxString	retVal;

	retVal=wildCardFormat;

	// Wild Card Format Codes are surrounded by $$ symbols
	// DAY		= 1-31
	// 0DAY		= 01-31
	// MONTH	= 1 - 12
	// 0MONTH	= 01 - 12
	// YEAR		= 2000 - 2999
	// HOUR		= 0 - 23
	// 0HOUR	= 00 - 23
	// MINUTE	= 0 - 59
	// 0MINUTE	= 00 - 59
	// SECOND	= 0 - 59
	// 0SECOND	= 00 - 59
	retVal.Replace("$$DAY$$","*");
	retVal.Replace("$$0DAY$$","*");
	retVal.Replace("$$MONTH$$","*");
	retVal.Replace("$$0MONTH$$","*");
	retVal.Replace("$$YEAR$$","*");
	retVal.Replace("$$HOUR$$","*");
	retVal.Replace("$$0HOUR$$","*");
	retVal.Replace("$$MINUTE$$","*");
	retVal.Replace("$$0MINUTE$$","*");
	retVal.Replace("$$SECOND$$","*");
	retVal.Replace("$$0SECOND$$","*");

	return (retVal);
}

void Server::SortFiles(wxArrayString &filenameList,wxString &wildCard)
{
	// We have a wild card - we need to sort the file based upon any
	// Date components
	int			filenameCount;
	int			filenameIndex;
	wxString	filename;

	STATSGEN_DEBUG_FUNCTION_START("Server","SortFiles")

	STATSGEN_DEBUG(DEBUG_ALWAYS,wildCard)
	filenameCount=filenameList.GetCount();
	STATSGEN_DEBUG(DEBUG_ALWAYS,"Before Sort")
	for (filenameIndex=0;filenameIndex<filenameCount;filenameIndex++)
	{
		filename=filenameList.Item(filenameIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,filename)
	}
	sortingWildCard=wildCard;
	sortingWildCard.Replace(".","\\.");
	filenameList.Sort(Server::CompareFilenames);
	STATSGEN_DEBUG(DEBUG_ALWAYS,"After Sort")
	for (filenameIndex=0;filenameIndex<filenameCount;filenameIndex++)
	{
		filename=filenameList.Item(filenameIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,filename)
	}

	STATSGEN_DEBUG_FUNCTION_END
}

int Server::CompareFilenames(const wxString &firstIn, const wxString &secondIn)
{
	// We should have the static variable sortingWildCard set
	// we need to work out if any date/time components are found in
	// the format - if so - extract them out and sort by the date
	// if no date formats are found - then just sort alphabetically

	int					positionYear	=wxNOT_FOUND;
	int					positionMonth	=wxNOT_FOUND;
	int					positionDay		=wxNOT_FOUND;
	int					positionHour	=wxNOT_FOUND;
	int					positionMinute	=wxNOT_FOUND;
	int					positionSecond	=wxNOT_FOUND;
	int					firstMatchCount;
	int					secondMatchCount;
	wxArrayString		firstMatches;
	wxArrayString		secondMatches;
	bool				dateUsed;
	wxArrayInt			positions;
	int					retVal=0;
	wxString			msg;
	int					positionCount;
	wxFileName			firstFilename(firstIn);
	wxFileName			secondFilename(secondIn);
	wxString			first;
	wxString			second;

	STATSGEN_DEBUG_FUNCTION_START("Server","CompareFilenames")
	first=firstFilename.GetFullName();
	second=secondFilename.GetFullName();

	STATSGEN_DEBUG(DEBUG_ALWAYS,first)
	STATSGEN_DEBUG(DEBUG_ALWAYS,second)

	positionYear=sortingWildCard.Find("$$YEAR$$");
	positionMonth=sortingWildCard.Find("$$MONTH$$");
	if (positionMonth==wxNOT_FOUND)
	{
		positionMonth=sortingWildCard.Find("$$0MONTH$$");
	}
	positionDay=sortingWildCard.Find("$$DAY$$");
	if (positionDay==wxNOT_FOUND)
	{
		positionDay=sortingWildCard.Find("$$0DAY$$");
	}
	positionHour=sortingWildCard.Find("$$HOUR$$");
	if (positionHour==wxNOT_FOUND)
	{
		positionHour=sortingWildCard.Find("$$0HOUR$$");
	}
	positionMinute=sortingWildCard.Find("$$MINUTE$$");
	if (positionMinute==wxNOT_FOUND)
	{
		positionMinute=sortingWildCard.Find("$$0MINUTE$$");
	}
	positionSecond=sortingWildCard.Find("$$SECOND$$");
	if (positionSecond==wxNOT_FOUND)
	{
		positionSecond=sortingWildCard.Find("$$0SECOND$$");
	}

	dateUsed=((positionYear!=wxNOT_FOUND)||
				(positionMonth!=wxNOT_FOUND)||
				(positionDay!=wxNOT_FOUND)||
				(positionHour!=wxNOT_FOUND)||
				(positionMinute!=wxNOT_FOUND)||
				(positionSecond!=wxNOT_FOUND));
	if (dateUsed)
	{
		// The wildcard has a date format in it - we need to pluck out
		// the date components and construct the dates for each filename
		// then we compare the dates
		wxString	regularExpString;
		regularExpString=sortingWildCard;
		regularExpString.Replace("$$YEAR$$","([1-2][0-9][0-9][0-9])");
		regularExpString.Replace("$$MONTH$$","([0-9]+)");
		regularExpString.Replace("$$0MONTH$$","([0-9][0-9])");
		regularExpString.Replace("$$DAY$$","([0-9]+)");
		regularExpString.Replace("$$0DAY$$","([0-3][0-9])");
		regularExpString.Replace("$$HOUR$$","([0-9]+)");
		regularExpString.Replace("$$0HOUR$$","([0-2][0-9])");
		regularExpString.Replace("$$MINUTE$$","([0-9]+)");
		regularExpString.Replace("$$0MINUTE$$","([0-5][0-9])");
		regularExpString.Replace("$$SECOND$$","([0-9]+)");
		regularExpString.Replace("$$0SECOND$$","([0-5][0-9])");

		wxRegEx	regularExp;

		if (regularExp.Compile(regularExpString))
		{
			regularExp.Matches(first);
			firstMatchCount=regularExp.GetMatchCount()-1;
			regularExp.Matches(second);
			secondMatchCount=regularExp.GetMatchCount()-1;
			if (firstMatchCount!=secondMatchCount)
			{
				msg.Printf("wildcard [%s] evaluated to regex [%s] but produced 2 different match counts [%s][%d] [%s][%d], using alphabetical sorting",
						sortingWildCard.GetData(),
						regularExpString.GetData(),
						first.GetData(),
						firstMatchCount,
						second.GetData(),
						secondMatchCount
					);
				progress->LogError(msg,SeverityCaution);
				dateUsed=false;
			}
			else
			{
				int		matchIndex;
				regularExp.Matches(first);
				for (matchIndex=0;matchIndex<firstMatchCount;matchIndex++)
				{
					firstMatches.Add(regularExp.GetMatch(first,matchIndex+1));
				}
				regularExp.Matches(second);
				for (matchIndex=0;matchIndex<secondMatchCount;matchIndex++)
				{
					secondMatches.Add(regularExp.GetMatch(second,matchIndex+1));
				}
				if (positionYear!=wxNOT_FOUND)
				{
					positions.Add(positionYear);
				}
				if (positionMonth!=wxNOT_FOUND)
				{
					positions.Add(positionMonth);
				}
				if (positionDay!=wxNOT_FOUND)
				{
					positions.Add(positionDay);
				}
				if (positionHour!=wxNOT_FOUND)
				{
					positions.Add(positionHour);
				}
				if (positionMinute!=wxNOT_FOUND)
				{
					positions.Add(positionMinute);
				}
				if (positionSecond!=wxNOT_FOUND)
				{
					positions.Add(positionSecond);
				}
				positions.Sort(Server::CompareInts);
				positionCount=positions.GetCount();
				if (positionCount!=firstMatchCount)
				{
					msg.Printf("wildcard [%s] evaluated to regular expression [%s] but produced a different match count to positions [%s] [%s] matchCount=[%d], positionCount=[%d], using alphabetical sorting",
						sortingWildCard.GetData(),
						regularExpString.GetData(),
						first.GetData(),
						second.GetData(),
						firstMatchCount,
						positions.GetCount()
					);
					progress->LogError(msg,SeverityCaution);
					dateUsed=false;
				}
				else
				{
					wxDateTime		firstDate;
					wxDateTime		secondDate;
					wxString		firstValueStr;
					wxString		secondValueStr;
					int				firstValue;
					int				secondValue;
					int				positionIndex;
					int				position;

					for (positionIndex=0;
						positionIndex<positionCount;
						positionIndex++)
					{
						position=positions.Item(positionIndex);
						firstValueStr=firstMatches.Item(positionIndex);
						secondValueStr=secondMatches.Item(positionIndex);
						firstValue=atoi(firstValueStr.GetData());
						secondValue=atoi(secondValueStr.GetData());
						if (position==positionYear)
						{
							msg.Printf("Setting year to [%d] [%d]",firstValue,secondValue);
							STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
							firstDate.SetYear(firstValue);
							secondDate.SetYear(secondValue);
						}
						if (position==positionMonth)
						{
							msg.Printf("Setting month to [%d] [%d]",firstValue,secondValue);
							STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
							firstDate.SetMonth((wxDateTime::Month)(firstValue-1));
							secondDate.SetMonth((wxDateTime::Month)(secondValue-1));
						}
						if (position==positionDay)
						{
							msg.Printf("Setting day to [%d] [%d]",firstValue,secondValue);
							STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
							firstDate.SetDay(firstValue);
							secondDate.SetDay(secondValue);
						}
						if (position==positionHour)
						{
							msg.Printf("Setting hour to [%d] [%d]",firstValue,secondValue);
							STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
							firstDate.SetHour(firstValue);
							secondDate.SetHour(secondValue);
						}
						if (position==positionMinute)
						{
							msg.Printf("Setting minute to [%d] [%d]",firstValue,secondValue);
							STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
							firstDate.SetMinute(firstValue);
							secondDate.SetMinute(secondValue);
						}
						if (position==positionSecond)
						{
							msg.Printf("Setting second to [%d] [%d]",firstValue,secondValue);
							STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
							firstDate.SetSecond(firstValue);
							secondDate.SetSecond(secondValue);
						}
						STATSGEN_DEBUG(DEBUG_ALWAYS,firstDate.FormatISODate());
						STATSGEN_DEBUG(DEBUG_ALWAYS,firstDate.FormatISOTime());
						STATSGEN_DEBUG(DEBUG_ALWAYS,secondDate.FormatISODate());
						STATSGEN_DEBUG(DEBUG_ALWAYS,secondDate.FormatISOTime());
					}

					retVal=0;
					msg.Printf("comparing %s to %s",first.GetData(),second.GetData());
					STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
					if (firstDate.IsEarlierThan(secondDate))
					{
						STATSGEN_DEBUG(DEBUG_ALWAYS,"First less than second")
						retVal=-1;
					}
					if (firstDate.IsLaterThan(secondDate))
					{
						STATSGEN_DEBUG(DEBUG_ALWAYS,"First greater than second")
						retVal=+1;
					}
					STATSGEN_DEBUG(DEBUG_ALWAYS,"Comparison Complete")
				}
			}
		}
		else
		{
			msg.Printf("wildcard [%s] evaluated to regular expression [%s] but it won't compile using alphabetical sorting",sortingWildCard.GetData(),regularExpString.GetData());
			progress->LogError(msg,SeverityCaution);
			dateUsed=false;
		}
	}
	if (!dateUsed)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,"Date Not Used")
		retVal=first.Cmp(second);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

int Server::CompareInts(int *first,int *second)
{
	int		retVal=0;

	if ((*first) < (*second))
	{
		retVal=-1;
	}
	if ((*first) > (*second))
	{
		retVal=+1;
	}

	return (retVal);
}
