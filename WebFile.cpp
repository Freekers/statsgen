#include <wx/protocol/http.h>
#include <wx/stream.h>
#include <wx/wfstream.h>
#include "WebFile.h"
#include "ErrorData.h"

WebFile::WebFile(wxString &hostnameIn,wxString &filenameIn,int port)
{
	hostname = hostnameIn;
	filename = filenameIn;
	mPort	= port;
}

WebFile::~WebFile()
{
}

wxInputStream * WebFile::GetInputStream()
{
	wxHTTP			http;
	wxInputStream	*inputStream=NULL;
	
	if (http.Connect(hostname,mPort))
	{
		inputStream=http.GetInputStream(filename);
	}

	return (inputStream);
}

bool WebFile::Get(wxString &localFile)
{
	STATSGEN_DEBUG_FUNCTION_START("WebFile","Get")
	bool			retVal;
	wxHTTP			http;
	wxInputStream	*inputStream;
	wxString	msg;

	msg.Printf("Getting [%s] from %s:%d to [%s]",STRING_TO_CHAR(filename),STRING_TO_CHAR(hostname),mPort,STRING_TO_CHAR(localFile));
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	retVal=true;
	if (http.Connect(hostname,mPort))
	{
		inputStream=http.GetInputStream(filename);
		if (inputStream!=NULL)
		{
			wxFileOutputStream	outputStream(localFile);
			outputStream.Write(*inputStream);
			delete inputStream;
		}
		else
		{
			retVal=false;
			msg.Printf("Can't get input stream for %s",STRING_TO_CHAR(filename));
			wxMessageBox(msg);
		}
	}
	else
	{
		wxMessageBox("Can't connect");
		retVal=false;
	}
	if (!retVal)
	{
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}
