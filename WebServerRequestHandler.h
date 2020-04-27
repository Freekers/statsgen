#include <wx/wx.h>
#include <wx/string.h>
#include <wx/socket.h>
#include <wx/thread.h>
#include <wx/mimetype.h>
#include <wx/xml/xml.h>

#ifndef __WEBSERVERREQUESTHANDLER
#define __WEBSERVERREQUESTHANDLER

#define XML_REQUEST_CONFIG_GROUP		"configgroup"
#define XML_REQUEST_STAT_TYPE_LIST		"stattypelist"
#define XML_REQUEST_SERVER_TYPE_LIST	"servertypelist"

class WebServerRequestHandler : public wxThread
{
	public:
		WebServerRequestHandler(wxSocketBase *clientIn);
		~WebServerRequestHandler();

		virtual void *Entry();
		

	private:
		wxXmlDocument ConfigGroupToXML(wxString &groupID);
		wxXmlDocument StatTypeListToXML();
		wxXmlDocument ServerTypeListToXML();
		wxXmlDocument KeyListToXML(
				wxArrayString &rootPropertyKeys,
				wxArrayString &rootPropertyValues,
				wxArrayString &keys,
				wxArrayString &values);
		void WriteBufferToClient(const char *buffer,long bufferSize);
		void WriteStandardResponse(int responseCode,
									wxString &contentType,
									const char *responseBuffer,
									long bufferSize);
		void HandleRequest(wxString &method,wxString &url);
		void HandleURL(wxString &url);
		void HandleXMLRequest(wxFileName &xmlRequest);
		void WriteErrorResponse(int code,char *response);
		void ReceiveDataFromClient();
		wxSocketBase	*client;
};

#endif
