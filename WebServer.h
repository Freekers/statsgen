#ifndef __WEBSERVER
#define __WEBSERVER

// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/socket.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "ErrorData.h"

class WebServer : public wxEvtHandler
{
	public:
		WebServer();
		virtual ~WebServer();
		wxString	ConfigKeyBase();
		wxString	ConfigKeyEnabled();
		wxString	ConfigKeyPort();
		wxString	ConfigKeyAdminUser();
		wxString	ConfigKeyAdminPassword();

		void UpdateFromConfig();
		void StopServer();
		void StartServer();

		void OnServerEvent(wxSocketEvent &event);

		bool IsListening();

	public:
		wxString	listenPort;
		wxString	adminUser;
		wxString	adminPassword;
		bool		enabled;

	private:
		wxSocketServer	*listenServer;

	DECLARE_EVENT_TABLE()
};

#endif
