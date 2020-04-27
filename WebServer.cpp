// wxWindows includes
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/datetime.h>
#include <wx/tokenzr.h>

// statsgen includes
#include "GlobalStatistics.h"
#include "WebServer.h"
#include "WindowIDs.h"
#include "WebServerRequestHandler.h"

BEGIN_EVENT_TABLE(WebServer,wxEvtHandler)

	EVT_SOCKET(SOCKET_ID_WEB_SERVER,WebServer::OnServerEvent)

END_EVENT_TABLE()

WebServer::WebServer()
{
	STATSGEN_DEBUG_FUNCTION_START("WebServer","Constructor");
	listenServer=NULL;
	STATSGEN_DEBUG_FUNCTION_END
}

WebServer::~WebServer()
{
	STATSGEN_DEBUG_FUNCTION_START("WebServer","Destructor");
	StopServer();
	STATSGEN_DEBUG_FUNCTION_END
}

wxString WebServer::ConfigKeyBase()
{
	wxString	configKey="WebServer";

	return (configKey);
}

wxString WebServer::ConfigKeyEnabled()
{
	wxString	configKey;

	configKey.Printf("/%s/Enabled",STRING_TO_CHAR(ConfigKeyBase()));
	return (configKey);
}

wxString WebServer::ConfigKeyPort()
{
	wxString	configKey;

	configKey.Printf("/%s/Port",STRING_TO_CHAR(ConfigKeyBase()));
	return (configKey);
}

wxString WebServer::ConfigKeyAdminUser()
{
	wxString	configKey;

	configKey.Printf("/%s/AdminUser",STRING_TO_CHAR(ConfigKeyBase()));
	return (configKey);
}

wxString WebServer::ConfigKeyAdminPassword()
{
	wxString	configKey;

	configKey.Printf("/%s/AdminPassword",STRING_TO_CHAR(ConfigKeyBase()));
	return (configKey);
}

void WebServer::UpdateFromConfig()
{
	wxString	configKey;
	wxString	configValue;

	STATSGEN_DEBUG_FUNCTION_START("WebServer","UpdateFromConfig")
	configKey=ConfigKeyEnabled();
	globalStatistics.configData.ReadTextValue(configKey,&configValue,(char *)"n");
	enabled=(configValue.CmpNoCase("y")==0);

	configKey=ConfigKeyPort();
	globalStatistics.configData.ReadTextValue(configKey,&listenPort,(char *)"9000");

	configKey=ConfigKeyAdminUser();
	globalStatistics.configData.ReadTextValue(configKey,&adminUser,(char *)"");

	configKey=ConfigKeyAdminPassword();
	globalStatistics.configData.ReadTextValue(configKey,&adminPassword,(char *)"");

	STATSGEN_DEBUG_FUNCTION_END
}

void WebServer::OnServerEvent(wxSocketEvent &event)
{
	wxSocketBase			*client;
	WebServerRequestHandler	*requestHandler;
	int clientFlags=wxSOCKET_WAITALL;

	STATSGEN_DEBUG_FUNCTION_START("WebServer","OnServerEvent")
	if (listenServer!=NULL)
	{
		client=listenServer->Accept(true);
		client->SetTimeout(0);
		client->SetFlags(clientFlags);

		requestHandler=new WebServerRequestHandler(client);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

bool WebServer::IsListening()
{
	bool	retVal=false;

	if (listenServer!=NULL)
	{
		if (listenServer->Ok())
		{
			retVal=true;
		}
	}

	return (retVal);
}

void WebServer::StartServer()
{
	STATSGEN_DEBUG_FUNCTION_START("WebServer","StartServer")
	int serverFlags=wxSOCKET_NOWAIT;
	// First stop the server if one is already started
	UpdateFromConfig();
	StopServer();
	if (enabled)
	{
		wxIPV4address	addr;
		addr.Service(atoi(STRING_TO_CHAR(listenPort)));
		listenServer=new wxSocketServer(addr,serverFlags);
		if (listenServer->Ok())
		{
			listenServer->SetEventHandler(*this,SOCKET_ID_WEB_SERVER);
			listenServer->SetNotify(wxSOCKET_CONNECTION_FLAG);
			listenServer->Notify(true);
		}
		else
		{
			// Not listening???
			StopServer();
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void WebServer::StopServer()
{
	STATSGEN_DEBUG_FUNCTION_START("WebServer","StopServer")
	if (listenServer!=NULL)
	{
		listenServer->Destroy();
		delete listenServer;
		listenServer=NULL;
	}
	STATSGEN_DEBUG_FUNCTION_END
}

