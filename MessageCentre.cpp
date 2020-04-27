// wxWindows includes
#include <wx/wx.h>
#include <wx/wfstream.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/protocol/ftp.h>
#include <wx/socket.h>
#include <wx/filename.h>

// Statsgen includes
#include "ErrorData.h"
#include "GlobalStatistics.h"
#include "MessageCentre.h"
#include "RemoteMachine.h"
#include "Progress.h"
#include "StaticFunctions.h"

#define		MESSAGE_CENTRE_PREFIX				"Statsgen2:^7"
#define		MESSAGE_CENTRE_PREFIX_HIDDEN		""
MessageCentre::MessageCentre()
{
	ID="";
	messageIndex=0;
	overallEnabled=false;
	serverEnabled=false;
	usingPunkbuster=false;
	messagePrefix="";
}

MessageCentre::~MessageCentre()
{
}

void MessageCentre::SetID(wxString &IDIn)
{
	ID=IDIn;
}

void MessageCentre::Initiate()
{
	wxString		configBaseKey;
	wxString		configKey;
	wxString		configValue;
	wxArrayString	configKeys;
	wxArrayString	configValues;
	int				formatIndex;
	int				formatCount;
	wxString		format;
	wxString		key;
	wxString		msg;
	wxArrayString	tokenValues;
	wxArrayString	customMessages;
	wxArrayString	workingMessages;
	wxString		configGroup;
	int				customMessageInterval;
	int				customMessageCount;
	int				customMessageIndex;
	int				workingMessageCount;

	STATSGEN_DEBUG_FUNCTION_START("MessageCentre","Initiate")

	serverEnabled=false;
	if (ID.Length()>0)
	{
		// Read in the configuration items for this server
		configBaseKey="/Server"+ID;
		configBaseKey+="/";

		configKey=configBaseKey+"serverType";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		serverType=configValue;

		configKey=configBaseKey+"gameIP";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		gameIP=configValue;
		STATSGEN_DEBUG(DEBUG_ALWAYS,configValue)

		configKey=configBaseKey+"gamePort";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		STATSGEN_DEBUG(DEBUG_ALWAYS,configValue)
		gamePort=-1;
		if (configValue.Length()>0)
		{
			gamePort=atoi(configValue.GetData());
		}

		configKey="/RCONSettings/ClientPort";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"8000");
		STATSGEN_DEBUG(DEBUG_ALWAYS,configValue)
		localPort=-1;
		if (configValue.Length()>0)
		{
			localPort=atoi(configValue.GetData());
		}

		configKey=configBaseKey+"IPAddress";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		hostname=configValue;
		STATSGEN_DEBUG(DEBUG_ALWAYS,configValue)

		configKey=configBaseKey+"RCONPassword";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
		password=configValue;
		STATSGEN_DEBUG(DEBUG_ALWAYS,configValue)

		configKey=configBaseKey+"MessagingEnabled";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"n");
		serverEnabled=(configValue.CmpNoCase("y")==0);

		configKey=configBaseKey+"RCONUsePunkBuster";
		globalStatistics.configData.ReadTextValue(configKey,&configValue,"n");
		usingPunkbuster=(configValue.CmpNoCase("y")==0);

		remotePunkbusterDirectory="";
		if (usingPunkbuster)
		{
			configKey=configBaseKey+"PunkBusterPostCommand";
			globalStatistics.configData.ReadTextValue(configKey,&punkbusterPostCommand,"");

			configKey=configBaseKey+"banFile1Type";
			globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
			if (configValue.CmpNoCase("PUNKBUSTER")==0)
			{
				configKey=configBaseKey+"FTPBanFile1Directory";
				globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
				remotePunkbusterDirectory=configValue;
			}
			else
			{
				configKey=configBaseKey+"banFile2Type";
				globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
				if (configValue.CmpNoCase("PUNKBUSTER")==0)
				{
					configKey=configBaseKey+"ban2Directory";
					globalStatistics.configData.ReadTextValue(configKey,&configValue,"");
					remotePunkbusterDirectory=configValue;
				}
			}
		}
	}

	configKey="/RCONSettings/Enabled";
	globalStatistics.configData.ReadTextValue(configKey,&configValue,"n");
	overallEnabled=(configValue.CmpNoCase("y")==0);

	configKey="/RCONSettings/MessagePrefix";
	globalStatistics.configData.ReadTextValue(configKey,&messagePrefix,"^7");
	// Now read the message list

	STATSGEN_DEBUG(DEBUG_ALWAYS,"Reading Message Centre Formats")
	globalStatistics.configData.ReadGroup("MessageCentreFormats",
										configKeys,
										configValues);
	formatCount=configKeys.GetCount();
	workingMessages.Clear();

	for (formatIndex=0;formatIndex<formatCount;formatIndex++)
	{

		key=configKeys.Item(formatIndex);
		STATSGEN_DEBUG(DEBUG_SOMETIMES,key)
		format=configValues.Item(formatIndex);
		STATSGEN_DEBUG(DEBUG_SOMETIMES,format)
		// we have the format string
		if (format.Length()>0)
		{
			int			tokenValueCount;
			int			tokenValueIndex;
			wxString	tokenValue;
			wxString	substitutionValue;

			// we have a valid format string
			// let's replace the tokens with the
			// appropriate values
			globalStatistics.configData.ReadList("MessageCentreValues",
												key,tokenValues);
			tokenValueCount=tokenValues.GetCount();
			for (tokenValueIndex=0;
				tokenValueIndex<tokenValueCount;
				tokenValueIndex++)
			{
				tokenValue=tokenValues.Item(tokenValueIndex);
				substitutionValue.Printf("$%d$",tokenValueIndex+1);
				format.Replace(substitutionValue,tokenValue);
			}

			STATSGEN_DEBUG(DEBUG_SOMETIMES,format)
			// now all substituted - we have a final message
			if (tokenValueCount>0)
			{
				workingMessages.Add(format);
			}
		}
	}

	// Need to insert the custom messages
	globalStatistics.configData.ReadTextValue(
									"/RCONSettings/CustomMessageInterval",
									&configValue,"5");
	customMessageInterval=atoi(configValue.GetData());
	configGroup="CustomMessages";
	globalStatistics.configData.ReadList(configGroup,customMessages);
	customMessageCount=customMessages.GetCount();
	workingMessageCount=workingMessages.GetCount();
	messagesToSend.Clear();
	customMessageIndex=0;
	STATSGEN_DEBUG_CODE(msg.Printf("SLJCustom Messages [%d] Interval [%d] Working Messages [%d]", customMessageCount,customMessageInterval,workingMessageCount);)
	STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
	for (formatIndex=0;
		formatIndex<workingMessageCount;
		formatIndex++)
	{
		format=workingMessages.Item(formatIndex);
		STATSGEN_DEBUG(DEBUG_RARELY,format)
		messagesToSend.Add(format);
		if ((customMessageCount>0) && (customMessageInterval>0))
		{
			if ((formatIndex % customMessageInterval)==0)
			{
				format=customMessages.Item(customMessageIndex);
				STATSGEN_DEBUG(DEBUG_RARELY,format)
				messagesToSend.Add(format);
				customMessageIndex++;
				if (customMessageIndex>=customMessageCount)
				{
					customMessageIndex=0;
				}
			}
		}
	}

	STATSGEN_DEBUG_CODE(msg.Printf("Messages Calculated - Messages To Send [%d]", messagesToSend.GetCount());)
	STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
	// Self check the settings
	if (overallEnabled && serverEnabled)
	{
		STATSGEN_DEBUG_CODE(msg.Printf("Messaging Enabled - Messages To Send [%d]", messagesToSend.GetCount());)
		STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
		if (usingPunkbuster)
		{
			if (remotePunkbusterDirectory.Length()==0)
			{
				msg.Printf("Server [%s]:Punkbuster Messaging Enabled but no Punkbuster Ban Directory defined, switching to non-PunkBuster operation",ID.GetData());
				progress->LogError(msg,SeverityCaution);
				usingPunkbuster=false;
			}
		}
		if ((hostname.Length()==0)&&(gameIP.Length()==0))
		{
			msg.Printf("Server [%s]:RCON Messaging Enabled but hostname is blank for server, server messaging disabled",ID.GetData());
			progress->LogError(msg,SeverityCaution);
			serverEnabled=false;
		}
		if (password.Length()==0)
		{
			msg.Printf("Server [%s]:RCON Messaging Enabled but RCON Password is blank for server, server messaging disabled",ID.GetData());
			progress->LogError(msg,SeverityCaution);
			serverEnabled=false;
		}
		if (gamePort<1)
		{
			msg.Printf("Server [%s]:RCON Messaging Enabled but game port is invalid, server messaging disabled",ID.GetData());
			progress->LogError(msg,SeverityCaution);
			serverEnabled=false;
		}
		if (localPort<1)
		{
			msg.Printf("Server [%s]:RCON Messaging Enabled but local port is invalid, server messaging disabled",ID.GetData());
			progress->LogError(msg,SeverityCaution);
			serverEnabled=false;
		}
	}
	if (overallEnabled && serverEnabled && usingPunkbuster)
	{
		STATSGEN_DEBUG_CODE(msg.Printf("Punkbuster Messaging Enabled - Messages To Send [%d]", messagesToSend.GetCount());)
		STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
		// We need to construct the punkbuster message file
		// and upload it to the game server
		UpdatePunkbusterMessages();
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void MessageCentre::SendRCONCommand(wxString &command)
{
	wxIPV4address		remoteAddress;
	wxIPV4address		localAddress;
	wxDatagramSocket	*connection;
	wxString			msg;
	wxString			constructedCommand;
	wxSocketError		lastError;
	
	STATSGEN_DEBUG_FUNCTION_START("MessageCentre","SendRCONCommand")
	if (gameIP.Length()>0)
	{
		remoteAddress.Hostname(gameIP);
	}
	else
	{
		remoteAddress.Hostname(hostname);
	}
	remoteAddress.Service(gamePort);
	localAddress.AnyAddress();
	localAddress.Service(localPort);

	connection=new wxDatagramSocket(localAddress,wxSOCKET_BLOCK);
	if (connection==NULL)
	{
		msg.Printf("Could not connect to %s:%d for message sending",
				hostname.GetData(),gamePort);
		progress->LogError(msg,SeverityError);
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
	}
	else
	{
		if (serverType.CmpNoCase("QUAKEWARS")==0)
		{
			constructedCommand.Printf("%c%crcon%c%s%c%s%c",
								255,255,255,
								password.GetData(),
								255,
								command.GetData(),
								255);
		}
		else
		{
			constructedCommand.Printf("%c%c%c%crcon %s %s",
								255,255,255,255,
								password.GetData(),
								command.GetData());
		}
		STATSGEN_DEBUG(DEBUG_SOMETIMES,constructedCommand)
		STATSGEN_DEBUG(DEBUG_SOMETIMES,hostname)
		connection->SendTo(remoteAddress,
							constructedCommand.GetData(),
							constructedCommand.Length());
								
		if (connection->Error())
		{
			lastError=connection->LastError();
			msg.Printf("Error during rcon send %d",lastError);
			progress->LogError(msg,SeverityError);
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		}
	}

	if (connection!=NULL)
	{
		delete (connection);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void MessageCentre::SendMessage()
{
	int			messageCount;
	wxString	messageToSend;
	wxString	command;
	wxString	configKey;
	wxString	configValue;

	if (overallEnabled && serverEnabled)
	{
		messageCount=messagesToSend.GetCount();
		if (messageCount>0)
		{
			if (messageIndex>=messageCount)
			{
				messageIndex=0;
			}
			messageToSend=messagesToSend.Item(messageIndex);
			messageIndex++;

			if (!usingPunkbuster)
			{
				char *mandatoryMessagePrefix;
				if (globalStatistics.hiddenFeatures)
				{
					mandatoryMessagePrefix=MESSAGE_CENTRE_PREFIX_HIDDEN;
				}
				else
				{
					mandatoryMessagePrefix=MESSAGE_CENTRE_PREFIX;
				}
				command.Printf("say \"%s%s%s\"",messagePrefix.GetData(),mandatoryMessagePrefix,messageToSend.GetData());
				SendRCONCommand(command);
			}

			// Time to send the message
		}
	}
}

void MessageCentre::UpdatePunkbusterMessages()
{
	int			messageCount;
	wxString	messageToSend;
	wxString	command;
	wxString	configKey;
	wxString	configValue;
	int			currentMessageIndex;
	wxFileName	messageFileName;
	FILE		*fp;
	int			totalMessageTime;
	int			messageIntervalTimer;
	wxString	msg;
	wxString	configGroup;
	const char *punkbusterMessageFile="statsgenpbmessages.cfg";

	STATSGEN_DEBUG_FUNCTION_START("MessageCentre","UpdatePunkbusterMessages")
	configKey="/RCONSettings/MessageIntervalTimer";
	globalStatistics.configData.ReadTextValue(configKey,&configValue);
	messageIntervalTimer=atoi(configValue.GetData());

	if ((overallEnabled && serverEnabled) && (messageIntervalTimer>0))
	{
		messageFileName=BaseDirectoryFileName(punkbusterMessageFile);
		STATSGEN_DEBUG(DEBUG_ALWAYS,messageFileName.GetFullPath())
		messageCount=messagesToSend.GetCount();
		STATSGEN_DEBUG_CODE(msg.Printf("Messages To Send [%d]",messageCount);)
		STATSGEN_DEBUG(DEBUG_SOMETIMES,msg)
		if (messageCount>0)
		{
			totalMessageTime=messageIntervalTimer*messageCount;
			fp=fopen(messageFileName.GetFullPath().GetData(),"w");
			if (fp!=NULL)
			{
				fprintf(fp,"pb_sv_taskempty\n");
				if (messageIndex>=messageCount)
				{
					messageIndex=0;
				}
				for (currentMessageIndex=messageIndex;
						currentMessageIndex<messageCount;
						currentMessageIndex++)
				{
					messageToSend=messagesToSend.Item(currentMessageIndex);
					char *mandatoryMessagePrefix;
					if (globalStatistics.hiddenFeatures)
					{
						mandatoryMessagePrefix=MESSAGE_CENTRE_PREFIX_HIDDEN;
					}
					else
					{
						mandatoryMessagePrefix=MESSAGE_CENTRE_PREFIX;
					}
					command.Printf("pb_sv_task %d %d say \"%s%s%s\"",
									currentMessageIndex*messageIntervalTimer,
									totalMessageTime,
									messagePrefix.GetData(),
									mandatoryMessagePrefix,
									messageToSend.GetData());
					fprintf(fp,"%s\n",command.GetData());
				}
				for (currentMessageIndex=0;
						currentMessageIndex<messageIndex;
						currentMessageIndex++)
				{
					char *mandatoryMessagePrefix;
					if (globalStatistics.hiddenFeatures)
					{
						mandatoryMessagePrefix=MESSAGE_CENTRE_PREFIX_HIDDEN;
					}
					else
					{
						mandatoryMessagePrefix=MESSAGE_CENTRE_PREFIX;
					}
					messageToSend=messagesToSend.Item(currentMessageIndex);
					command.Printf("pb_sv_task %d %d say \"%s%s%s\"",
									currentMessageIndex*messageIntervalTimer,
									totalMessageTime,
									messagePrefix.GetData(),
									mandatoryMessagePrefix,
									messageToSend.GetData());
					fprintf(fp,"%s\n",command.GetData());
				}
				if (punkbusterPostCommand.Length()>0)
				{
					fprintf(fp,"%s\n",punkbusterPostCommand.GetData());
				}
				fclose(fp);
				configGroup="Server"+ID;
				RemoteMachine	server(configGroup);
				wxString		filename;

				filename=messageFileName.GetFullPath();
				server.PutFile(remotePunkbusterDirectory, filename);
				command.Printf("pb_sv_load %s",punkbusterMessageFile);
								
				SendRCONCommand(command);
			}
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

