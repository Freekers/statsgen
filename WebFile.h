#ifndef _WEBFILE_H
#define _WEBFILE_H

#include <wx/wx.h>
#include <wx/stream.h>
#include <wx/wfstream.h>

class WebFile
{
	public:
		WebFile(wxString &hostname,wxString &filename,int port=80);
		virtual ~WebFile();

		bool Get(wxString &localFile);
		wxInputStream *GetInputStream();
	private:
		wxString hostname;
		wxString filename;
		int		mPort;
};

#endif
