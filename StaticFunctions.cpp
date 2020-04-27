// wxWindows includes
#include <wx/string.h>
#include <wx/filename.h>

// Statsgen includes
#include "StaticFunctions.h"
#include "GlobalStatistics.h"

void ExportTemplateQuick(wxString &directory);
void ExportTemplateFile(wxString &directory,wxArrayString &fileLines,char *filename);
wxFileName BaseDirectoryFileName(const char *filename)
{
	wxFileName	baseDirectory(wxGetApp().argv[0]);
	wxFileName	retVal;
	wxString	filenameStr;

	filenameStr=filename;
	baseDirectory.MakeAbsolute();
	retVal=baseDirectory;
	retVal.SetFullName(filenameStr);

	return (retVal);
}

bool NeedsFirstTimeRun()
{
	wxString	configKey;
	wxString	configValue;

	configKey="/General/FirstTimeRun";
	globalStatistics.configData.ReadTextValue(configKey,&configValue);
	return (configValue.CmpNoCase("n")!=0);
}

bool FirstTimeRun()
{
	bool		retVal=false;
	wxFileName	baseDirectory(wxGetApp().argv[0]);
	wxFileName	htmlDirectory;
	wxFileName	templateDirectory;
	wxFileName	playercacheFilename;
	wxFileName	aliasFilename;
	wxFileName	droplistFilename;
	wxFileName	fullTemplateFileName;
	wxString	fullTemplateName="full.template";
	wxString	droplistname="droplist.dat";
	wxString	aliasname="aliaslist.dat";
	wxString	playercachename="playercache.dat";
	wxString	configKey;
	wxString	configValue;
	wxString	directory;

	baseDirectory.MakeAbsolute();
	STATSGEN_DEBUG_FUNCTION_START("","FirstTimeRun")
	//baseDirectory=baseDirectory.GetPath();
	htmlDirectory=baseDirectory;
	templateDirectory=baseDirectory;
	playercacheFilename=baseDirectory;
	aliasFilename=baseDirectory;
	droplistFilename=baseDirectory;

	htmlDirectory.AppendDir("html");
	templateDirectory.AppendDir("templates");
	fullTemplateFileName=templateDirectory;
	fullTemplateFileName.SetFullName(fullTemplateName);
	droplistFilename.SetFullName(droplistname);
	aliasFilename.SetFullName(aliasname);
	playercacheFilename.SetFullName(playercachename);

	configKey="/General/FirstTimeRun";
	globalStatistics.configData.ReadTextValue(configKey,&configValue);
	if (configValue.CmpNoCase("n")!=0)
	{
		retVal=true;
		// First time run
		// Create basic directories
		#ifdef _LINUX
			mkdir(htmlDirectory.GetPath(),0777);
			mkdir(templateDirectory.GetPath(),0777);
		#else
			wxMkDir(htmlDirectory.GetPath());
			wxMkDir(templateDirectory.GetPath());
		#endif
		// Create base config entries
		configValue="n";
		globalStatistics.configData.WriteTextValue(configKey,configValue);

		configKey="/General/playercache";
		configValue=playercacheFilename.GetFullPath();
		globalStatistics.configData.WriteTextValue(configKey,configValue);

		configKey="/General/aliaslist";
		configValue=aliasFilename.GetFullPath();
		globalStatistics.configData.WriteTextValue(configKey,configValue);

		configKey="/General/LocalOutput";
		configValue=htmlDirectory.GetPath();
		globalStatistics.configData.WriteTextValue(configKey,configValue);

		configKey="/General/DropList";
		configValue=droplistFilename.GetFullPath();
		globalStatistics.configData.WriteTextValue(configKey,configValue);

		configKey="/Template/FullRun";
		configValue=fullTemplateFileName.GetFullPath();
		globalStatistics.configData.WriteTextValue(configKey,configValue);

		directory=fullTemplateFileName.GetPath();
		ExportTemplateFull(directory);

		globalStatistics.configData.CommitChanges();
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void SafeString(wxString &string)
{
	int	index;
	int	length;
	wxString	workString;

	length=string.Length();
	for (index=0;index<length;index++)
	{
		if (
			(string[index]>='A')&&(string[index]<='Z') ||
			(string[index]>='a')&&(string[index]<='z') ||
			(string[index]>='0')&&(string[index]<='9')
			)
		{
			workString+=string[index];
		}
		else
		{
			workString+="_";
		}
	}
	string=workString.Lower();
	/*
	string.Replace("*","_");
	string.Replace(":","_");
	string.Replace(".","_");
	string.Replace(" ","_");
	string.Replace("[","_");
	string.Replace("]","_");
	string.Replace("=","_");
	*/
}

wxString DecolouriseName(wxString &name)
{
	wxString decolourisedName=name;

	decolourisedName.Replace("^0","");
	decolourisedName.Replace("^1","");
	decolourisedName.Replace("^2","");
	decolourisedName.Replace("^3","");
	decolourisedName.Replace("^4","");
	decolourisedName.Replace("^5","");
	decolourisedName.Replace("^6","");
	decolourisedName.Replace("^7","");
	decolourisedName.Replace("^8","");
	decolourisedName.Replace("^9","");

	return (decolourisedName);
}

void ExportTemplateFile(wxString &directory,wxArrayString &fileLines,char *filename)
{
	wxString	filenameStr;
	wxFileName	templateFilename=wxFileName::DirName(directory);
	wxTextFile	templateFile;
	

	filenameStr=filename;
	templateFilename.SetFullName(filenameStr);

	if (wxFileExists(templateFilename.GetFullPath()))
	{
		wxRemoveFile(templateFilename.GetFullPath());
	}
	if (templateFile.Create(templateFilename.GetFullPath()))
	{
		int			lineCount;
		int			lineIndex;
		wxString	line;

		lineCount=fileLines.GetCount();
		for (lineIndex=0;lineIndex<lineCount;lineIndex++)
		{
			line=fileLines.Item(lineIndex);
			templateFile.AddLine(line);
		}
		templateFile.Write();
		templateFile.Close();
	}
}

void UpgradeConfigFile(wxString &upgradeFilenameStr)
{
	wxString	configKey;
	wxString	configValue;
	wxString	message;
	wxString	caption;
	wxString		dummy;

	if (upgradeFilenameStr.Length()>0)
	{
		ConfigData		upgradeConfigFile;
		wxFileName		upgradeFileName(upgradeFilenameStr);
		wxString		group;
		wxArrayString	groups;
		int				groupCount;
		int				groupIndex;
		wxString		description;
		wxString		recommendation;
		wxString		answerTypeStr;
		wxArrayString	choices;
		wxString		choice;
		int				choiceIndex;
		int				answerType;
		choices.Clear();
		choice=UPGRADE_CHOICE_DEFAULT;choices.Add(choice);
		choice=UPGRADE_CHOICE_DO_NOTHING;choices.Add(choice);
		choice=UPGRADE_CHOICE_UPDATE;choices.Add(choice);
		choice=UPGRADE_CHOICE_OVERWRITE;choices.Add(choice);
								
		upgradeConfigFile.Initiate(upgradeFileName);
		group="UPGRADELIST";
		upgradeConfigFile.ReadList(group,groups);
		groupCount=groups.GetCount();
		choiceIndex=2;

		for (groupIndex=0;groupIndex<groupCount;groupIndex++)
		{
			int				configCount;
			int 			configIndex;
			wxArrayString	configKeys;
			wxArrayString	configValues;
			wxString		fullConfigKey;

			group=groups.Item(groupIndex);
			configKey="/";
			configKey+=group;
			configKey+="/UPGRADEDESCRIPTION";
			upgradeConfigFile.ReadTextValue(configKey,&description);
			configKey="/";
			configKey+=group;
			configKey+="/UPGRADERECOMMENDATION";
			upgradeConfigFile.ReadTextValue(configKey,&recommendation);
			configKey="/";
			configKey+=group;
			configKey+="/UPGRADEANSWERTYPE";
			upgradeConfigFile.ReadTextValue(configKey,&answerTypeStr);
			upgradeConfigFile.ReadGroup(group,configKeys,configValues);
			configCount=configKeys.GetCount();

			answerType=atoi(STRING_TO_CHAR(answerTypeStr));
			if (answerType==CONFIG_ANSWER_TYPE_ASK)
			{
				message.Printf("%s\n\nRecommendation\n%s\n",
						STRING_TO_CHAR(description),
						STRING_TO_CHAR(recommendation));
				caption="What Do You Want To Do?";

				choiceIndex=wxGetSingleChoiceIndex(message,caption,choices);
			}
			if (choiceIndex == 0)
			{
				// Do recommendation
				if (recommendation.CmpNoCase(UPGRADE_CHOICE_DO_NOTHING)==0)
				{
					choiceIndex = 1;
				}
				if (recommendation.CmpNoCase(UPGRADE_CHOICE_UPDATE)==0)
				{
					choiceIndex = 2;
				}
				if (recommendation.CmpNoCase(UPGRADE_CHOICE_OVERWRITE)==0)
				{
					choiceIndex = 3;
				}
			}
			switch (choiceIndex)
			{
				case 1:		// Do Nothing
					break;
				case 3:		// Overwrite
					globalStatistics.configData.DeleteGroup(group);
					globalStatistics.configData.CommitChanges();
				case 2:		// Update
					for (configIndex=0;configIndex<configCount;configIndex++)
					{
						configKey=configKeys.Item(configIndex);
						configValue=configValues.Item(configIndex);
						if (!configKey.StartsWith("UPGRADE"))
						{
							fullConfigKey.Printf("/%s/%s",STRING_TO_CHAR(group),STRING_TO_CHAR(configKey));
							globalStatistics.configData.WriteTextValue(fullConfigKey,configValue);
						}
					}
					globalStatistics.configData.CommitChanges();
					break;
			}
		}
	}
}

bool IsHexString(wxString &stringToCheck)
{
	bool		retVal=true;
	int			stringLength;
	char		stringChar;
	int			stringIndex;

	stringLength=stringToCheck.Length();
	if (stringLength > 0)
	{
		if ((stringLength % 2) == 0)
		{
			// string is multiples of 2 chars
			for (stringIndex=0;stringIndex<stringLength;stringIndex++)
			{
				stringChar=stringToCheck[stringIndex];
				if (!isdigit(stringChar))
				{
					if (!(
						((stringChar >= 'a') && (stringChar <='f')) ||
						((stringChar >= 'A') && (stringChar <='Z'))
						))
						
					{
						// not a number and not a valid hex digit
						retVal=false;
						break;
					}
				}
			}
		}
		else
		{
			// not multiples of 2 chars
			retVal=false;
		}
	}
	else
	{
		// Zero length
		retVal=false;
	}

	return (retVal);
}
