// wx includes
#include <wx/filename.h>
#include <wx/zipstrm.h>

// Statsgen Includes
#include "Website.h"
#include "GlobalStatistics.h"
#include "RemoteMachine.h"
#include "Progress.h"

void Website::LoadConfig()
{
	wxString	configKey;
	wxString	configValue;
	wxString	configBaseKey;

	configGroup="Website";
	configBaseKey="/"+configGroup+"/";

	configKey=configBaseKey+"FTPEnabled";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"n");
	FTPEnabled=(configValue.CmpNoCase("y")==0);

	configKey=configBaseKey+"FTPRemoteDirectory";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
	FTPRemoteDirectory=configValue;
}
Website::Website()
{
	LoadConfig();
}

Website::~Website()
{
}

bool Website::UploadZipContents(wxString &localFile)
{
	bool				retVal;
	wxString			imagename;
	long				fileSize;
	long				totalSize;
	wxString			msg;
	RemoteMachine		remoteMachine(configGroup);
	wxString			directory;
	wxString			newDirectory;

	totalSize=0;

	if (FTPEnabled)
	{
		wxFileInputStream	inputStream(localFile);
		wxZipInputStream	zipStream(inputStream);
		wxZipEntry			*zipEntry;
		zipEntry=zipStream.GetNextEntry();
		while (zipEntry!=NULL)
		{
			fileSize=zipStream.GetLength();
			totalSize+=fileSize;
			zipEntry=zipStream.GetNextEntry();
		}
	}

	if (FTPEnabled)
	{
		wxFileInputStream	inputStream(localFile);
		wxZipInputStream	zipStream(inputStream);
		wxZipEntry			*zipEntry;
		wxString			configGroup="WEBSITE";
		RemoteMachine		website(configGroup);

		progress->Initiate(totalSize,
							"kb",
							1024,
							"kb",
							1024);
		zipEntry=zipStream.GetNextEntry();
		while (zipEntry!=NULL)
		{
			directory=FTPRemoteDirectory;
			progress->SetOffset(progress->CurrentValue());
			imagename=zipEntry->GetName();
			while (imagename.Contains('\\'))
			{
				newDirectory=imagename.BeforeFirst('\\');
				imagename=imagename.AfterFirst('\\');
				remoteMachine.MakeDir(directory,newDirectory);
				directory+="/";
				directory+=newDirectory;
			}
			progress->SetStatus(imagename);
			if (imagename.Length()>0)
			{
				remoteMachine.PutFile(directory,
									imagename,
									(wxInputStream *)&zipStream,
									imagename,
									SeverityError);
			}
			//wxMessageBox(imagename);
			zipEntry=zipStream.GetNextEntry();
		}
		progress->Finalise();
	}

	return (true);
}

bool Website::TransferFiles(wxArrayString &filelist)
{
	wxString		localFilename;
	int				fileCount;
	int				fileIndex;
	bool			allOK;
	RemoteMachine	remoteMachine(configGroup);
	wxString		msg;
	long			totalFileSize;
	long			fileSize;

	STATSGEN_DEBUG_FUNCTION_START("Website","TransferFiles")
	if (FTPEnabled)
	{
		fileCount=filelist.GetCount();
		STATSGEN_DEBUG_CODE(msg.Printf("Transferring %d files to website",fileCount);)
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		totalFileSize=0;
		msg="Getting File Sizes";
		progress->SetStatus(msg);
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			localFilename=filelist.Item(fileIndex);
			wxFile webFile(localFilename);
			fileSize=webFile.Length();
			totalFileSize+=fileSize;
		}
		progress->Initiate(totalFileSize,
							"kb",
							1024,
							"kb",
							1024);
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			progress->SetOffset(progress->CurrentValue());
			localFilename=filelist.Item(fileIndex);
			wxFileName	fname(localFilename);
			wxString	filenameonly;
			filenameonly=fname.GetFullName();
			progress->SetStatus(filenameonly);
			STATSGEN_DEBUG_CODE(msg.Printf("Transferring %d of %d [%s] to [%s]",
						fileIndex+1,
						fileCount,
						localFilename.GetData(),
						FTPRemoteDirectory.GetData());)
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
			allOK=remoteMachine.PutFile(FTPRemoteDirectory,localFilename);
			if (allOK)
			{
				STATSGEN_DEBUG(DEBUG_RARELY,"File Transferred OK")
			}
			else
			{
				msg.Printf("Failed to transfer %s to website",localFilename.GetData());
				progress->LogError(msg,SeverityError);
				STATSGEN_DEBUG(DEBUG_ALWAYS,"File Transferred FAIL")
			}
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

bool Website::UploadZipTemplateContents(wxString &localFile,wxString &templateFilename)
{
	bool				retVal;
	wxString			imagename;
	long				fileSize;
	long				totalSize;
	wxString			msg;
	wxString			directory;
	wxString			newDirectory;
	wxString			fullTemplateFilename;

	totalSize=0;

	{
		wxFileInputStream	inputStream(localFile);
		wxZipInputStream	zipStream(inputStream);
		wxZipEntry			*zipEntry;
		zipEntry=zipStream.GetNextEntry();
		while (zipEntry!=NULL)
		{
			fileSize=zipStream.GetLength();
			totalSize+=fileSize;
			zipEntry=zipStream.GetNextEntry();
		}
	}

	{
		wxFileInputStream	inputStream(localFile);
		wxZipInputStream	zipStream(inputStream);
		wxZipEntry			*zipEntry;

		progress->Initiate(totalSize,
							"kb",
							1024,
							"kb",
							1024);
		zipEntry=zipStream.GetNextEntry();
		while (zipEntry!=NULL)
		{
			wxString	configKey;
			wxString	configValue;

			configKey="/Template/FullRun";
			globalStatistics.configData.ReadTextValue(configKey,&configValue);
			wxFileName	templateEntryFile(configValue);
			
			progress->SetOffset(progress->CurrentValue());
			imagename=zipEntry->GetName();
			progress->SetStatus(imagename);
			if (imagename.Length()>0)
			{
				templateEntryFile.SetFullName(imagename);
				wxFileOutputStream	outputStream(templateEntryFile.GetFullPath());
				unsigned char	buffer[1024];

				while (!zipStream.Eof())
				{
					zipStream.Read(buffer,sizeof(buffer));
					if (zipStream.LastRead()>0)
					{
						outputStream.Write(buffer,zipStream.LastRead());
					}
				}
			}
			//wxMessageBox(imagename);
			zipEntry=zipStream.GetNextEntry();
			templateEntryFile.SetFullName(templateFilename);
			fullTemplateFilename=templateEntryFile.GetFullPath();
			globalStatistics.configData.WriteTextValue(configKey,fullTemplateFilename);
		}
		progress->Finalise();
	}

	return (true);
}

