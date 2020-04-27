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

	retVal="www.statsgen.co.uk";
	//retVal="localhost";

	return (retVal);
}

WebFile *StatsgenWeb::GetWebFile(wxString &filepath)
{
	WebFile	*retVal;
	wxString	hostname=Hostname();

	retVal=new WebFile(hostname,filepath);

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
	wxString	localFilename;
	wxString	remoteFilename;
	bool		retVal;
	wxString	hostname=Hostname();
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

	WebFile	remoteFile(hostname,remoteFilename);
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
		wxXmlDocument	contentsXML;

		contentsXML.Load(localFilename);
		if (contentsXML.IsOk())
		{
			wxXmlNode	*node;

			node=contentsXML.GetRoot();
			if (node!=NULL)
			{
				node=node->GetChildren();
				while (node!=NULL)
				{
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
					nodeContent=node->GetNodeContent();
					property="type";node->GetPropVal(property,&nodeType);
					property="address";node->GetPropVal(property,&nodeAddress);
					property="servertype";node->GetPropVal(property,&nodeServerType);
					property="thumbnail";node->GetPropVal(property,&nodeThumbnail);
					property="secondaddress";node->GetPropVal(property,&nodeSecondAddress);
					property="upgradefile";node->GetPropVal(property,&nodeUpgradeFile);
					property="templatefilename";node->GetPropVal(property,&nodeTemplateFilename);
					nodeTypeInt=WEBFILE_TYPE_UNKNOWN;
					if (nodeName.CmpNoCase("file")==0)
					{
						if (nodeType.CmpNoCase(WEBFILE_TYPE_STR_IMAGEPACK)==0)
						{
							nodeTypeInt=WEBFILE_TYPE_IMAGEPACK;
						}
						if (nodeType.CmpNoCase(WEBFILE_TYPE_STR_TEMPLATE)==0)
						{
							nodeTypeInt=WEBFILE_TYPE_TEMPLATE;
						}
					}
					if (nodeTypeInt!=WEBFILE_TYPE_UNKNOWN)
					{
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
		}
		else
		{
			retVal=false;
			wxString msg;
			msg.Printf("Failed to open contents.xml from the %s site, please try again at a later time",Hostname().GetData());
			wxMessageBox(msg);
		}
	}

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
	wxString		localFile="imagepack.zip";
	wxString		hostname=Hostname();
	WebFile			webfile(hostname,path);
	bool			retVal;
	wxString		localConfigFile;

	retVal=webfile.Get(localFile);
	if (retVal)
	{
		Website	website;
		retVal=website.UploadZipContents(localFile);

		if (storeLocally)
		{
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
		WebFile			webfile(hostname,upgradeFile);
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
		WebFile			webfile(hostname,path);

		retVal=webfile.Get(localImageFile);
		if (retVal)
		{
			Website	website;
			retVal=website.UploadZipContents(localImageFile);
		}
	}
	if (remoteTemplatePath.Length()>0)
	{
		WebFile			webfile(hostname,remoteTemplatePath);
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
		WebFile			webfile(hostname,remoteUpgradePath);
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

