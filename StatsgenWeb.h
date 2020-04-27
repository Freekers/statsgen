#ifndef _STATSGEN_WEB_H
#define _STATSGEN_WEB_H

#include <wx/wx.h>
#include "WebFile.h"

enum
{
	WEBFILE_TYPE_UNKNOWN=-1,
	WEBFILE_TYPE_IMAGEPACK=0,
	WEBFILE_TYPE_TEMPLATE
};

#define WEBFILE_TYPE_STR_IMAGEPACK	"IMAGEPACK"
#define WEBFILE_TYPE_STR_TEMPLATE	"TEMPLATE"

class StatsgenWeb
{
	public:
		StatsgenWeb();
		virtual ~StatsgenWeb();
		bool GetFileList(wxArrayString &fileDescriptions,wxArrayString &filePaths,
						wxArrayInt &fileTypes,
						wxArrayString &serverTypes,
						wxArrayString &secondFilePaths,
						wxArrayString &templateFilenames,
						wxArrayString &upgradeFiles,
						wxArrayString &thumbnails);
		bool GetImagePackList(wxArrayString &fileDescriptions,
						wxArrayString &filePaths,
						wxArrayString &serverTypes,
						wxArrayString &upgradeFilesIn,
						wxArrayString &thumbnails);
		bool GetTemplateList(wxArrayString &fileDescriptions,
						wxArrayString &filePaths,
						wxArrayString &serverTypes,
						wxArrayString &secondFilePaths,
						wxArrayString &templateFilenames,
						wxArrayString &upgradeFiles,
						wxArrayString &thumbnails);
		bool UploadImagePack(wxString &description,wxString &path,bool storeLocally,wxString &localOutputFolder,wxString &upgradeFile);
		bool UploadTemplatePack(wxString &description,wxString &path,wxString &templatePath,wxString &remoteUpgradePath,wxString &templateFilename);
		WebFile *GetWebFile(wxString &filepath);
	private:
		wxString Hostname();
		int Port();

};

#endif
