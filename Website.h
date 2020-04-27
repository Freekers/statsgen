#ifndef __WEBSITE
#define __WEBSITE

#include <wx/wx.h>
#include <wx/arrstr.h>

// Statsgen Includes

class Website
{
	public:
		Website();
		~Website();
		void LoadConfig();
		bool TransferFiles(wxArrayString &filelist);
		bool UploadZipContents(wxString &zipfile);
		bool UploadZipTemplateContents(wxString &zipfile,wxString &templateFilename);
	private:
		wxString		configGroup;
		wxString		FTPRemoteDirectory;
		bool			FTPEnabled;
};

#endif
