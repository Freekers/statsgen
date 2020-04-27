#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/xml/xml.h>
#include <wx/mstream.h>
#include <stdio.h>
#include "WebServerRequestHandler.h"
#include "StaticFunctions.h"
#include "ErrorData.h"
#include "ContentTypes.h"
#include "GlobalStatistics.h"


WebServerRequestHandler::WebServerRequestHandler(wxSocketBase *clientIn)
{
	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","Constructor")
	client=clientIn;

	Create();
	Run();
	STATSGEN_DEBUG_FUNCTION_END
}

WebServerRequestHandler::~WebServerRequestHandler()
{
	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void WebServerRequestHandler::WriteErrorResponse(int errorCode,char *response)
{
	wxString	responseStr;

	responseStr.Printf("HTTP/1.1 %ld %s\r\nserver: statsgen\r\n"
					"content-type: text/plain\r\n"
					"content-length: %ld\r\n"
					"\r\n"
					"%s",
					errorCode,
					response,
					strlen(response),
					response);
	WriteBufferToClient(responseStr.GetData(),responseStr.Length());
}

void WebServerRequestHandler::WriteBufferToClient(
					const char *buffer,
					long bufferSize)
{
	long bytesWritten;
	long bytesRemaining;
	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","WriteBufferToClient")
	if (client==NULL)
	{
		return;
	}
	bytesWritten=0;
	bytesRemaining=bufferSize;
	while (bytesRemaining>0)
	{
		client->Write(buffer, bytesRemaining);
		bytesWritten=client->LastCount();
		if (bytesWritten>=0)
		{
			buffer+=bytesWritten;
			bytesRemaining-=bytesWritten;
		}
		else
		{
			break;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void WebServerRequestHandler::WriteStandardResponse(
					int responseCode,
					wxString &contentType,
					const char *responseBuffer,
					long bufferSize)
{
	wxString	responseHeader;

	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","WriteStandardResponse")
	responseHeader.Printf("HTTP/1.1 %d 0K\r\n"
							"server: statsgen\r\n"
							"content=type: %s\r\n"
							"content-length: %ld\r\n"
							"\r\n",
							responseCode,
							contentType.GetData(),
							bufferSize);
	WriteBufferToClient(responseHeader.GetData(),responseHeader.Length());
	WriteBufferToClient(responseBuffer,bufferSize);
	STATSGEN_DEBUG_FUNCTION_END
}

wxXmlDocument WebServerRequestHandler::ServerTypeListToXML()
{
	wxArrayString	rootPropertyKeys;
	wxArrayString	rootPropertyValues;
	wxString		rootPropertyKey;
	wxString		rootPropertyValue;
	wxArrayString	keys;
	wxArrayString	values;
	int				index;

	rootPropertyKey="id";
	rootPropertyValue="servertypelist";
	rootPropertyKeys.Add(rootPropertyKey);
	rootPropertyValues.Add(rootPropertyValue);

	for (index=0;index<SERVER_TYPE_COUNT;index++)
	{
		keys.Add(ServerTypeCodes[index]);values.Add(ServerTypeNames[index]);
	}

	return(KeyListToXML(rootPropertyKeys,
						rootPropertyValues,
						keys,
						values));
}

wxXmlDocument WebServerRequestHandler::StatTypeListToXML()
{
	wxArrayString	rootPropertyKeys;
	wxArrayString	rootPropertyValues;
	wxString		rootPropertyKey;
	wxString		rootPropertyValue;
	wxArrayString	keys;
	wxArrayString	values;

	rootPropertyKey="id";
	rootPropertyValue="stattypelist";
	rootPropertyKeys.Add(rootPropertyKey);
	rootPropertyValues.Add(rootPropertyValue);

	keys.Add("action");values.Add("Objective");
	keys.Add("team");values.Add("Team");
	keys.Add("class");values.Add("Class");
	keys.Add("gametype");values.Add("Game Type");
	keys.Add("location");values.Add("Location");
	keys.Add("weapon");values.Add("Weapon");
	keys.Add("map");values.Add("Map");

	return(KeyListToXML(rootPropertyKeys,
						rootPropertyValues,
						keys,
						values));
}

wxXmlDocument WebServerRequestHandler::ConfigGroupToXML(wxString &configGroup)
{
	wxArrayString	rootPropertyKeys;
	wxArrayString	rootPropertyValues;
	wxString		rootPropertyKey;
	wxString		rootPropertyValue;
	wxArrayString	keys;
	wxArrayString	values;

	rootPropertyKey="id";
	rootPropertyValue="configgroup";
	rootPropertyKeys.Add(rootPropertyKey);
	rootPropertyValues.Add(rootPropertyValue);

	rootPropertyKey="groupid";
	rootPropertyValue=configGroup;
	rootPropertyKeys.Add(rootPropertyKey);
	rootPropertyValues.Add(rootPropertyValue);

	globalStatistics.configData.ReadGroup(configGroup,keys,values);

	return(KeyListToXML(rootPropertyKeys,
						rootPropertyValues,
						keys,
						values));
}

wxXmlDocument WebServerRequestHandler::KeyListToXML(
								wxArrayString &rootPropertyKeys,
								wxArrayString &rootPropertyValues,
								wxArrayString &keys,
								wxArrayString &values
								)
{
	wxXmlDocument	xmlResponse;
	wxXmlNode		*root;
	int				keyCount;
	int				keyIndex;
	wxString		key;
	wxString		value;
	wxXmlNode		*lastConfigItem=NULL;
	wxString		rootPropertyKey;
	wxString		rootPropertyValue;
	int				rootKeyIndex;
	int				rootKeyCount;

	root=new wxXmlNode(wxXML_ELEMENT_NODE,"root");
	rootKeyCount=rootPropertyKeys.GetCount();
	for (rootKeyIndex=0;rootKeyIndex<rootKeyCount;rootKeyIndex++)
	{
		rootPropertyKey=rootPropertyKeys.Item(rootKeyIndex);
		rootPropertyValue=rootPropertyValues.Item(rootKeyIndex);
		root->AddProperty(rootPropertyKey,rootPropertyValue);
	}
	xmlResponse.SetRoot(root);
	keyCount=keys.GetCount();
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		wxXmlNode	*configItemNode;
		wxXmlNode	*keyNodeValue;
		wxXmlNode	*valueNodeValue;
		wxXmlNode	*keyNode;
		wxXmlNode	*valueNode;
		
		key=keys.Item(keyIndex);
		value=values.Item(keyIndex);

		configItemNode=new wxXmlNode(wxXML_ELEMENT_NODE,"configitem");
		keyNode=new wxXmlNode(wxXML_ELEMENT_NODE,"key");
		valueNode=new wxXmlNode(wxXML_ELEMENT_NODE,"value");

		keyNodeValue=new wxXmlNode(wxXML_TEXT_NODE,"key",key);
		valueNodeValue=new wxXmlNode(wxXML_TEXT_NODE,"value",value);

		keyNode->AddChild(keyNodeValue);
		valueNode->AddChild(valueNodeValue);

		configItemNode->AddChild(keyNode);
		keyNode->SetNext(valueNode);

		if (lastConfigItem==NULL)
		{
			root->AddChild(configItemNode);
		}
		else
		{
			lastConfigItem->SetNext(configItemNode);
		}
		lastConfigItem=configItemNode;
	}

	return (xmlResponse);
}

void WebServerRequestHandler::HandleXMLRequest(wxFileName &xmlRequest)
{
	wxString				xmlFilename;
	wxXmlDocument			xmlResponse;
	wxMemoryOutputStream	xmlBufferStream;
	long					xmlSize;
	char					*xmlBuffer;
	wxString				ext="xml";

	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","HandleXMLRequest")
	xmlFilename=xmlRequest.GetName().Lower();
	if (xmlFilename.StartsWith(XML_REQUEST_CONFIG_GROUP))
	{
		wxString		configGroup="";

		configGroup=xmlFilename.Mid(strlen(XML_REQUEST_CONFIG_GROUP));
		xmlResponse=ConfigGroupToXML(configGroup);
	}
	else
	if (xmlFilename.StartsWith(XML_REQUEST_STAT_TYPE_LIST))
	{
		xmlResponse=StatTypeListToXML();
	}
	else
	if (xmlFilename.StartsWith(XML_REQUEST_SERVER_TYPE_LIST))
	{
		xmlResponse=ServerTypeListToXML();
	}
	xmlResponse.Save(xmlFilename);
	xmlResponse.Save(xmlBufferStream);
	xmlSize=xmlBufferStream.GetSize();
	xmlBuffer=(char *)malloc(xmlSize);
	if (xmlBuffer!=NULL)
	{
		wxString	contentType;
		xmlBufferStream.CopyTo(xmlBuffer,xmlSize);

		ContentTypes contentTypes;
		contentType=contentTypes.GetContentType(ext);
		WriteStandardResponse(200,contentType,xmlBuffer,xmlSize);
		free(xmlBuffer);
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void WebServerRequestHandler::HandleURL(wxString &url)
{
	wxFileName	urlFileName;
	wxString	msg;
	wxString	contentType;
	wxString	ext;

	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","HandleURL")
	STATSGEN_DEBUG(DEBUG_ALWAYS,url)
	url=url.AfterFirst('/');
	urlFileName=BaseDirectoryFileName(url.GetData());
	ext=urlFileName.GetExt().Lower();
	if (ext.CmpNoCase("xml")==0)
	{
		// xml response - this is data
		HandleXMLRequest(urlFileName);
	}
	else
	{
		if (urlFileName.FileExists())
		{
			ContentTypes contentTypes;
			contentType=contentTypes.GetContentType(ext);
			wxFile	responseFile;

			if (responseFile.Open(urlFileName.GetFullPath(),wxFile::read))
			{
				long fileSize=responseFile.Length();
				wxString	responseHeader;
				char *responseBuffer;

				responseBuffer=new char[fileSize];
				responseFile.Read(responseBuffer,fileSize);
				WriteStandardResponse(200,contentType,responseBuffer,fileSize);
				if (responseBuffer!=NULL)
				{
					delete responseBuffer;
				}
			}
			else
			{
				// File exists but could not open
				msg.Printf("url=[%s] Could Not Be Opened",url.GetData());
				WriteErrorResponse(404,(char *)msg.GetData());
			}
		}
		else
		{
			STATSGEN_DEBUG(DEBUG_ALWAYS,"NOT FOUND")
			msg.Printf("url=[%s] Not Found",url.GetData());
			WriteErrorResponse(404,(char *)msg.GetData());
		}
	}
	if (client!=NULL)
	{
		client->Destroy();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void WebServerRequestHandler::HandleRequest(wxString &method,wxString &url)
{
	wxString	msg;
	url=url.Lower();
	HandleURL(url);
}

void *WebServerRequestHandler::Entry()
{
	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","Entry")
	while (client!=NULL && client->IsConnected())
	{
		ReceiveDataFromClient();
		client->Close();
	}
	STATSGEN_DEBUG_FUNCTION_END
	return NULL;
}

void WebServerRequestHandler::ReceiveDataFromClient()
{
	char 		receiveBuffer[5000];
	int			receivedBytes;
	wxString	responseLine;
	wxString	method;
	wxString	url;
	wxString	version;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("WebServerRequestHandler","ReceiveDataFromClient")
	try
	{
		receiveBuffer[0]=0;
		client->Read(receiveBuffer,sizeof(receiveBuffer));
		receivedBytes=client->LastCount();
		// Received a response
		// decode the response details
		responseLine=receiveBuffer;
		responseLine=responseLine.BeforeFirst('\r');
		method=responseLine.BeforeFirst(' ');
		responseLine=responseLine.AfterFirst(' ');
		url=responseLine.BeforeFirst(' ');
		version=responseLine.AfterFirst(' ');
		if ((method.Length()==0) || 
			(url.Length()==0) || 
			(version.Length()==0) ||
			(!url.StartsWith("/")))
		{
		
			msg.Printf("Bad Request: Method=[%s], URL=[%s], Version=[%s]",
					method.GetData(),
					url.GetData(),
					version.GetData());
			WriteErrorResponse(400,(char *)msg.GetData());
			return;
		}
		else
		{
			HandleRequest(method,url);
			return;
		}
	}
	catch (int e)
	{
		WriteErrorResponse(500,"Server Error");
		return;
		// Server Error (500)
	}
	STATSGEN_DEBUG_FUNCTION_END
}

