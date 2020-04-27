#include <wx/protocol/http.h>
#include <wx/stream.h>
#include <wx/wfstream.h>
#include "WebFile.h"

WebFile::WebFile(wxString &hostnameIn,wxString &filenameIn)
{
	hostname = hostnameIn;
	filename = filenameIn;
}

WebFile::~WebFile()
{
}

wxInputStream * WebFile::GetInputStream()
{
	wxHTTP			http;
	wxInputStream	*inputStream=NULL;
	
	if (http.Connect(hostname))
	{
		inputStream=http.GetInputStream(filename);
	}

	return (inputStream);
}

bool WebFile::Get(wxString &localFile)
{
	bool			retVal;
	wxHTTP			http;
	wxInputStream	*inputStream;
	wxString	msg;
	
	retVal=true;
	if (http.Connect(hostname))
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
			msg.Printf("Can't get input stream for %s",filename.GetData());
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
	return (retVal);
}
