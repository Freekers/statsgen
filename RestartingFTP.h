#ifndef __RESTARTINGFTP
#define __RESTARTINGFTP
#include <wx/wx.h>
#include <wx/protocol/ftp.h>

class RestartingFTP :public wxFTP
{
	public:
		RestartingFTP();
		virtual ~RestartingFTP();

		virtual wxInputStream *GetInputStreamRestart(const wxString &path,int startPos=0);
};
#endif

