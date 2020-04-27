#include <wx/xml/xml.h>
#include <wx/zipstrm.h>

#include "StatsgenWeb.h"
#include "StaticFunctions.h"
#include "WebFile.h"
#include "Website.h"
#include "GlobalStatistics.h"
#include "Progress.h"
#include "PerformUpgradeDialog.h"

StatsgenWeb::StatsgenWeb()
{
}

StatsgenWeb::~StatsgenWeb()
{
}

wxString StatsgenWeb::Hostname()
{
	wxString	retVal;

	//retVal="www.statsgen.co.uk";
	//retVal="localhost";
	retVal=STATSGEN_WEBSITE_ADDRESS;

	return (retVal);
}

int StatsgenWeb::Port()
{
	return STATSGEN_WEBSITE_PORT;
}

WebFile *StatsgenWeb::GetWebFile(wxString &filepath)
{
	STATSGEN_DEBUG_FUNCTION_START("StatsgenWeb","GetWebFile")
	WebFile	*retVal;
	wxString	hostname=Hostname();

	retVal=new WebFile(hostname,filepath,Port());

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool StatsgenWeb::GetFileList(wxArrayString &fileDescriptions,
								wxArrayString &filePaths,
								wxArrayInt &fileTypes,
								wxArrayString &fileServerTypes,
								wxArrayString &secondFilePaths,
								wxArrayString &templateFilenames,
								wxArrayString &upgradeFiles,
								wxArrayString &thumbnails)
{
	STATSGEN_DEBUG_FUNCTION_START("StatsgenWeb","GetFileList")
	wxString	localFilename;
	wxString	remoteFilename;
	bool		retVal;
	wxString	hostname=Hostname();
	int			port=Port();
	wxString	nodeName;
	wxString	nodeType;
	wxString	nodeAddress;
	wxString	nodeSecondAddress;
	wxString	nodeTemplateFilename;
	wxString	nodeServerType;
	wxString	nodeUpgradeFile;
	wxString	nodeThumbnail;
	wxString	nodeContent;
	wxString	property;
	int			nodeTypeInt;

	localFilename="contents.xml";
	remoteFilename="/contents.xml";

	WebFile	remoteFile(hostname,remoteFilename,port);
	filePaths.Clear();
	secondFilePaths.Clear();
	fileTypes.Clear();
	fileDescriptions.Clear();
	fileServerTypes.Clear();
	templateFilenames.Clear();
	upgradeFiles.Clear();
	thumbnails.Clear();

	retVal = remoteFile.Get(localFilename);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"downloaded contents.xml");
		wxXmlDocument	contentsXML;

		contentsXML.Load(localFilename);
		if (contentsXML.IsOk())
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"contents.xml is formatted ok");
			wxXmlNode	*node;

			node=contentsXML.GetRoot();
			if (node!=NULL)
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"got root ok");
				node=node->GetChildren();
				while (node!=NULL)
				{
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"processing child node");
					wxString	msg;

					nodeName				="";
					nodeType				="";
					nodeAddress				="";
					nodeServerType			="";
					nodeContent				="";
					nodeThumbnail			="";
					nodeSecondAddress		="";
					nodeUpgradeFile			="";
					nodeTemplateFilename	="";

					nodeName=node->GetName();
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeName");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeName);
					nodeContent=node->GetNodeContent();
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeContent");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeContent);
					/*
					property="type";node->GetPropVal(property,&nodeType);
					property="address";node->GetPropVal(property,&nodeAddress);
					property="servertype";node->GetPropVal(property,&nodeServerType);
					property="thumbnail";node->GetPropVal(property,&nodeThumbnail);
					property="secondaddress";node->GetPropVal(property,&nodeSecondAddress);
					property="upgradefile";node->GetPropVal(property,&nodeUpgradeFile);
					property="templatefilename";node->GetPropVal(property,&nodeTemplateFilename);
					*/
					/*
					property="type";node->AddAttribute(property,nodeType);
					property="address";node->AddAttribute(property,nodeAddress);
					property="servertype";node->AddAttribute(property,nodeServerType);
					property="thumbnail";node->AddAttribute(property,nodeThumbnail);
					property="secondaddress";node->AddAttribute(property,nodeSecondAddress);
					property="upgradefile";node->AddAttribute(property,nodeUpgradeFile);
					property="templatefilename";node->AddAttribute(property,nodeTemplateFilename);
					*/
					property="type";nodeType = node->GetAttribute(property);
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeType");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeType);
					property="address";nodeAddress = node->GetAttribute(property);
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeAddress");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeAddress);
					property="servertype";nodeServerType = node->GetAttribute(property);
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeServerType");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeServerType);
					property="thumbnail";nodeThumbnail = node->GetAttribute(property);
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeThumbnail");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeThumbnail);
					property="secondaddress";nodeSecondAddress = node->GetAttribute(property);
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeSecondAddress");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeSecondAddress);
					property="upgradefile";nodeUpgradeFile = node->GetAttribute(property);
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeUpgradeFile");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeUpgradeFile);
					property="templatefilename";nodeTemplateFilename = node->GetAttribute(property);
					STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"nodeTemplateFilename");
					STATSGEN_DEBUG(DEBUG_ALWAYS,nodeTemplateFilename);
					nodeTypeInt=WEBFILE_TYPE_UNKNOWN;
					if (nodeName.CmpNoCase("file")==0)
					{
						STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"file node found");
						if (nodeType.CmpNoCase(WEBFILE_TYPE_STR_IMAGEPACK)==0)
						{
							STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Image Pack");
							nodeTypeInt=WEBFILE_TYPE_IMAGEPACK;
						}
						if (nodeType.CmpNoCase(WEBFILE_TYPE_STR_TEMPLATE)==0)
						{
							STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Template Pack");
							nodeTypeInt=WEBFILE_TYPE_TEMPLATE;
						}
					}
					else
					{
						STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"not file node");
					}
					if (nodeTypeInt!=WEBFILE_TYPE_UNKNOWN)
					{
						STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"valid node found");
						filePaths.Add(nodeAddress);
						fileTypes.Add(nodeTypeInt);
						fileDescriptions.Add(nodeContent);
						fileServerTypes.Add(nodeServerType);
						thumbnails.Add(nodeThumbnail);
						secondFilePaths.Add(nodeSecondAddress);
						upgradeFiles.Add(nodeUpgradeFile);
						templateFilenames.Add(nodeTemplateFilename);
					}
					node=node->GetNext();
				}
			}
			else
			{
				STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"failed to get root");
			}
		}
		else
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"contents.xml is not formatted properly");
			retVal=false;
			wxString msg;
			msg.Printf("Failed to open contents.xml from the %s site, please try again at a later time",STRING_TO_CHAR(Hostname()));
			wxMessageBox(msg);
		}
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"failed to download contents.xml");
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}
bool StatsgenWeb::GetImagePackList(
						wxArrayString &fileDescriptionsIn,
						wxArrayString &filePathsIn,
						wxArrayString &fileServerTypesIn,
						wxArrayString &upgradeFilesIn,
						wxArrayString &thumbnailsIn)
{
	wxArrayString		fileDescriptions;
	wxArrayString		filePaths;
	wxArrayString		secondFilePaths;
	wxArrayString		templateFilenames;
	wxArrayString		fileServerTypes;
	wxArrayString		upgradeFiles;
	wxArrayString		thumbnails;
	wxArrayInt			fileTypes;
	int					fileCount;
	int					fileIndex;
	wxString			fileDescription;
	wxString			filePath;
	wxString			fileServerType;
	wxString			upgradeFile;
	wxString			thumbnail;
	int					fileType;
	bool				retVal;

	fileDescriptionsIn.Clear();
	filePathsIn.Clear();
	fileServerTypesIn.Clear();
	thumbnailsIn.Clear();
	upgradeFilesIn.Clear();
	retVal=GetFileList(fileDescriptions,filePaths,fileTypes,fileServerTypes,secondFilePaths,templateFilenames,upgradeFiles,thumbnails);
	if (retVal)
	{
		fileCount=fileDescriptions.GetCount();
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			fileDescription=fileDescriptions.Item(fileIndex);
			filePath=filePaths.Item(fileIndex);
			fileType=fileTypes.Item(fileIndex);
			fileServerType=fileServerTypes.Item(fileIndex);
			thumbnail=thumbnails.Item(fileIndex);
			upgradeFile=upgradeFiles.Item(fileIndex);
			if (fileType==WEBFILE_TYPE_IMAGEPACK)
			{
				fileDescriptionsIn.Add(fileDescription);
				filePathsIn.Add(filePath);
				fileServerTypesIn.Add(fileServerType);
				upgradeFilesIn.Add(upgradeFile);
				thumbnailsIn.Add(thumbnail);
			}
		}
	}

	return (retVal);
}

bool StatsgenWeb::UploadImagePack(wxString &description,wxString &path,bool storeLocally,wxString &localOutputPath,wxString &upgradeFile)
{
	STATSGEN_DEBUG_FUNCTION_START("StatsgenWeb","UploadImagePack")
	wxString		localFile="imagepack.zip";
	wxString		hostname=Hostname();
	WebFile			webfile(hostname,path,Port());
	bool			retVal;
	wxString		localConfigFile;

	retVal=webfile.Get(localFile);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Got Local File");
		Website	website;
		retVal=website.UploadZipContents(localFile);

		if (storeLocally)
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Storing Locally");
			wxFileInputStream	inputStream(localFile);
			wxZipInputStream	zipStream(inputStream);
			wxZipEntry			*zipEntry;
			wxString			directory;
			wxString			imagename;
			wxString			newDirectory;
			wxString			outputFile;

			zipEntry=zipStream.GetNextEntry();
			while (zipEntry!=NULL)
			{
				directory=localOutputPath;
				imagename=zipEntry->GetName();
				while (imagename.Contains('\\'))
				{
					newDirectory=imagename.BeforeFirst('\\');
					imagename=imagename.AfterFirst('\\');
					directory+='\\';
					directory+=newDirectory;
					wxMkDir(directory);
				}
				if (imagename.Length()>0)
				{
					outputFile=directory;
					outputFile+='\\';
					outputFile+=imagename;

					wxFileOutputStream	outputFileStream(outputFile);

					outputFileStream.Write(zipStream);
				}
				zipEntry=zipStream.GetNextEntry();
			}
		}
	}
	localConfigFile=upgradeFile.AfterLast('/');
	if ((retVal)&& (upgradeFile.Length()>0))
	{
		WebFile			webfile(hostname,upgradeFile,Port());
		retVal=webfile.Get(localConfigFile);
		if (retVal)
		{
			wxFileName		upgradeFileName;
			wxString		localConfigFileStr;
			upgradeFileName=BaseDirectoryFileName(localConfigFile);
			localConfigFileStr=upgradeFileName.GetFullPath();
			globalStatistics.statsgenDatabase.OpenDB();

			UpgradeConfigFile(localConfigFileStr);
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool StatsgenWeb::GetTemplateList(
						wxArrayString &fileDescriptionsIn,
						wxArrayString &filePathsIn,
						wxArrayString &fileServerTypesIn,
						wxArrayString &secondFilePathsIn,
						wxArrayString &templateFilenamesIn,
						wxArrayString &upgradeFilesIn,
						wxArrayString &thumbnailsIn)
{
	wxArrayString		fileDescriptions;
	wxArrayString		filePaths;
	wxArrayString		secondFilePaths;
	wxArrayString		templateFilenames;
	wxArrayString		upgradeFiles;
	wxArrayString		fileServerTypes;
	wxArrayString		thumbnails;
	wxArrayInt			fileTypes;
	int					fileCount;
	int					fileIndex;
	wxString			fileDescription;
	wxString			filePath;
	wxString			secondFilePath;
	wxString			templateFilename;
	wxString			fileServerType;
	wxString			thumbnail;
	wxString			upgradeFile;
	int					fileType;
	bool				retVal;

	fileDescriptionsIn.Clear();
	filePathsIn.Clear();
	secondFilePathsIn.Clear();
	fileServerTypesIn.Clear();
	thumbnailsIn.Clear();
	templateFilenamesIn.Clear();
	upgradeFilesIn.Clear();
	retVal=GetFileList(fileDescriptions,filePaths,fileTypes,fileServerTypes,secondFilePaths,templateFilenames,upgradeFiles,thumbnails);
	if (retVal)
	{
		fileCount=fileDescriptions.GetCount();
		for (fileIndex=0;fileIndex<fileCount;fileIndex++)
		{
			fileDescription=fileDescriptions.Item(fileIndex);
			filePath=filePaths.Item(fileIndex);
			fileType=fileTypes.Item(fileIndex);
			fileServerType=fileServerTypes.Item(fileIndex);
			secondFilePath=secondFilePaths.Item(fileIndex);
			templateFilename=templateFilenames.Item(fileIndex);
			upgradeFile=upgradeFiles.Item(fileIndex);
			thumbnail=thumbnails.Item(fileIndex);
			if (fileType==WEBFILE_TYPE_TEMPLATE)
			{
				fileDescriptionsIn.Add(fileDescription);
				filePathsIn.Add(filePath);
				secondFilePathsIn.Add(secondFilePath);
				templateFilenamesIn.Add(templateFilename);
				fileServerTypesIn.Add(fileServerType);
				upgradeFilesIn.Add(upgradeFile);
				thumbnailsIn.Add(thumbnail);
			}
		}
	}

	return (retVal);
}

bool StatsgenWeb::UploadTemplatePack(wxString &description,wxString &path,wxString &remoteTemplatePath,wxString &remoteUpgradePath,wxString &templateFilename)
{
	wxString		localImageFile="templatewebsite.zip";
	wxString		localTemplateFile="template.zip";
	wxString		localUpgradeFile="templateupgrade.ini";
	wxString		hostname=Hostname();
	bool			retVal=true;
	wxString		localConfigFile;

	if (path.Length()>0)
	{
		WebFile			webfile(hostname,path,Port());

		retVal=webfile.Get(localImageFile);
		if (retVal)
		{
			Website	website;
			retVal=website.UploadZipContents(localImageFile);
		}
	}
	if (remoteTemplatePath.Length()>0)
	{
		WebFile			webfile(hostname,remoteTemplatePath,Port());
		retVal=webfile.Get(localTemplateFile);
		if (retVal)
		{
			Website	website;
			retVal=website.UploadZipTemplateContents(localTemplateFile,templateFilename);
		}
	}
	localConfigFile=remoteUpgradePath.AfterLast('/');
	if (remoteUpgradePath.Length()>0)
	{
		WebFile			webfile(hostname,remoteUpgradePath,Port());
		retVal=webfile.Get(localConfigFile);
		if (retVal)
		{
			wxFileName		upgradeFileName;
			wxString		localConfigFileStr;
			upgradeFileName=BaseDirectoryFileName(localConfigFile);
			localConfigFileStr=upgradeFileName.GetFullPath();
			globalStatistics.statsgenDatabase.OpenDB();

			UpgradeConfigFile(localConfigFileStr);
		}
	}
	return (retVal);
}

