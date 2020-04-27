// wxWindows includes
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/textfile.h>
#include <wx/filename.h>

// Statsgen includes
#include "TemplateFile.h"
#include "LogFileReader.h"
#include "Progress.h"
#include "GlobalStatistics.h"

TemplateFile::TemplateFile()
{
}

TemplateFile::~TemplateFile()
{
	Unload();
}

bool TemplateFile::ReadTemplateFile(wxString &filename,wxString parentOrg)
{
	bool			retVal;
	wxTextFile		templateFile;
	int				lineCount;
	int				lineIndex;
	wxString		line;
	wxString		errorMessage;
	int				previousTemplateLine;
	wxString		previousTemplateFile;
	int				charIndex;
	wxString		includeString;
	wxString		lowerCaseLine;
	wxString		beforeInclude;
	wxString		afterInclude;
	wxString		parent;
	wxString		msg;
	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","ReadTemplateFile")

	STATSGEN_DEBUG_CODE(msg.Printf("filename[%s]",STRING_TO_CHAR(filename));)
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	previousTemplateLine=currentTemplateLine;
	previousTemplateFile=currentTemplateFile;
	currentTemplateFile=filename;
	currentTemplateLine=0;
	retVal=templateFile.Open(filename);
	LoadTemplateFile(filename);
	if (retVal)
	{
		// Successfully opened file
		// Scan through each line of the file adding
		// to the templateLines and searching for "include" commands
		lineCount=templateFile.GetLineCount();
		for (lineIndex=0;lineIndex<lineCount;lineIndex++)
		{
			parent.Printf("%s:%s:%d",STRING_TO_CHAR(parentOrg),STRING_TO_CHAR(currentTemplateFile),lineIndex);
			progress->Update(templateLines.GetCount());
			currentTemplateLine=lineIndex;
			line=templateFile.GetLine(lineIndex);

			// Added for Compiled Version
			line+="\n";
			// Added for Compiled Version

			lowerCaseLine=line;
			//lowerCaseLine.MakeLower();
			// We need to see if there is an include statement in here
			charIndex=lowerCaseLine.Find("$$include");
			if (charIndex>=0)
			{
				// There is an include statement in this line
				if (charIndex>0)
				{
					// A bit before the include
					beforeInclude=line.Left(charIndex);
				}
				else
				{
					beforeInclude="";
				}
				line=line.Mid(charIndex+9);	// skip past "$$include"
				charIndex=line.Find("$$");
				if (charIndex>-1)
				{
					afterInclude=line.Mid(charIndex+2); // skip past term $$
					// Terminating $$ found - left of it should be the filename
					filename="";
					if (charIndex>0)
					{
						filename=line.Left(charIndex);
					}
					// trim any spaces from ends of filename
					filename.Trim(true);
					filename.Trim(false);
					if (filename.Length()>0)
					{
						// Filename looks like it is probably valid
						if (beforeInclude.Length()>0)
						{
							AddTemplateLine(beforeInclude,parent);
							//templateLines.Add(beforeInclude);
						}
						wxFileName	templateFilename(originalTemplateFile);
						templateFilename.SetFullName(filename);
						filename=templateFilename.GetFullPath();
						retVal=ReadTemplateFile(filename,parent);
						if (retVal)
						{
							// Included file read ok
							if (afterInclude.Length()>0)
							{
								AddTemplateLine(afterInclude,parent);
								//templateLines.Add(afterInclude);
							}
						}
						else
						{
							// included file not ok
							// error will have been recorded
							break;
						}
				
					}
					else
					{
						// No actual filename seems to be in the include statement
						errorMessage="empty filename in $$include $$ statement";
						retVal=false;
						break;
					}
				}
				else
				{
					// No terminating $$ for the include - error
					errorMessage="$$include not terminated with a $$";
					retVal=false;
					break;
				}
				
			}
			else
			{
				// No include statement - just add the line
				AddTemplateLine(line,parent);
				//templateLines.Add(line);
			}
		}
		templateFile.Close();
	}
	else
	{
		// Could not open the file
		// First open
		errorMessage="Failed to open template file";
	}

	if (!retVal)
	{
		wxString	addition;
		addition.Printf(", Template File [%s] error line [%d]",
						STRING_TO_CHAR(currentTemplateFile),
						currentTemplateLine);
		errorMessage+=addition;
		if (previousTemplateFile.Length()>0)
		{
	
			addition.Printf(", Opened from [%s] at line [%d",
						STRING_TO_CHAR(previousTemplateFile),
						previousTemplateLine);
			errorMessage+=addition;
		}
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
		progress->LogError(errorMessage,SeverityError);
	}
	currentTemplateLine=previousTemplateLine;
	currentTemplateFile=previousTemplateFile;
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void TemplateFile::AddTemplateLine(wxString &line,wxString &lineRef)
{
	templateLines.Add(line);
	templateLineRef.Add(lineRef);
}

bool TemplateFile::SplitTemplateLine(wxString &templateLine,
					wxString *beforeTemplateCode,
					wxString *templateCode,
					wxString *afterTemplateCode)
{
	bool		retVal=true;
	int			charIndex;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","SplitTemplateLine")
	*beforeTemplateCode="";
	*templateCode="";
	*afterTemplateCode="";
	charIndex=templateLine.Find("$$");
	if (charIndex>=0)
	{
		// There is $$ indicating a template code present in this
		// line
		if (charIndex>0)
		{
			// A bit before the $$
			*beforeTemplateCode=templateLine.Left(charIndex);
		}
		else
		{
			// nothing before the $$
			*beforeTemplateCode="";
		}
		// get the bit after the $$ (this is the template code)
		templateLine=templateLine.Mid(charIndex+2);	// skip past "$$"
		// now - we have an open $$ - skip to the close $$
		charIndex=templateLine.Find("$$");
		if (charIndex>=0)
		{
			// Close $$ found
			if (charIndex>0)
			{
				// template code is not empty
				*templateCode=templateLine.Left(charIndex);
				// make the templateCode line lower case
				//*templateCode->MakeLower();
				templateLine=templateLine.Mid(charIndex+2);	// skip past "$$"
				*afterTemplateCode=templateLine;
			}
			else
			{
				// empty template code - makes no sense
				msg="Empty template code encountered";
				STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
				progress->LogError(msg,SeverityCaution);
				retVal=false;
			}
		}
		else
		{
			// There is not a close $$ for this line - error
			msg="template code is not terminated by a $$";
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			progress->LogError(msg,SeverityCaution);
			retVal=false;
		}

	}
	else
	{
		// no template code found
		*beforeTemplateCode=templateLine;
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool TemplateFile::ReadTemplate(wxString &filename)
{
	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","ReadTemplate")
	bool							retVal;
	wxString						parent;

	Unload();
	progress->Initiate((long)0,
						" lines",
						(long)1,
						" lines",
						(long)1);
	templateLines.Clear();
	templateLineRef.Clear();
	originalTemplateFile=filename;
	currentTemplateFile="";
	currentTemplateLine=0;

	parent="";
	retVal=ReadTemplateFile(filename,parent);

	// Template Line read in

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool TemplateFile::Process()
{
	TemplateVariable		templateVariable;
	int						lineIndex=0;
	wxString				initialTemplateCode="";
	bool					retVal=true;
	int						finishedType;
	//int						initialTemplateCode;
	CompiledTemplateSection	compiledSection;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","Process")

	createdFiles.Clear();
	/*
	return (ProcessTemplate(true,
							templateVariable,
							&lineIndex,
							initialTemplateCode,
							&endedOnElse));
	*/

	lineIndex=-1;
	templateVariable.SetTemplateFile(this);
	retVal=Compile(compiledSection,
					&lineIndex,
					initialTemplateCode,
					&finishedType,
					templateVariable);
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Compiled Successfully")

		retVal=compiledSection.Process(*this,templateVariable);
		if (retVal)
		{
			msg="Processing Succeeded";progress->LogError(msg,SeverityOK);
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Processed Successfully")
		}
		else
		{
			msg="Processing Failed";progress->LogError(msg,SeverityError);
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Processing failed")
		}
	}
	else
	{
		msg="Compilation Failed";progress->LogError(msg,SeverityError);
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Compilation failed")
	}


	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

bool TemplateFile::WriteTemplateDataToOutput(wxString &text,
											TemplateVariable &templateVariable)
{
	bool		retVal=true;
	int			openFileCount;

	openFileCount=templateVariable.openFiles.GetCount();
	if (openFileCount>0)
	{
		TemplateOpenFile	*openFile;

		openFile=templateVariable.openFiles.Detach(openFileCount-1);
		// Write the data to the open file
		retVal=openFile->Write(text);
		// add it back to the end of the stack
		templateVariable.openFiles.Add(openFile);
	}
	else
	{
		// Trying to write data - but no open file yet
	}

	return (retVal);
}

bool TemplateFile::CloseFile(TemplateVariable &templateVariable)
{
	bool		retVal=true;
	int			openFileCount;

	openFileCount=templateVariable.openFiles.GetCount();

	if (openFileCount>0)
	{
		// pop off the top of the openFiles - this will automatically close it
		templateVariable.openFiles.RemoveAt(openFileCount-1);
	}
	else
	{
		// trying to close an output file that is not open
		//StatsgenDebug("Trying to close an output file that is not open");
		// no error for this - it does not really matter
	}

	return (retVal);
}

bool TemplateFile::OpenFile(wxString &templateCode,
							char *mode,
							TemplateVariable &templateVariable)
{
	bool				retVal=true;
	wxString			filename;
	wxString			createdFilename;
	TemplateOpenFile	*openFile;
	int					tokenCount;
	wxString			msg;
	int					createdFilesCount;
	int					createdFilesIndex;
	bool				found;
	wxString			configKey;
	wxString			localDirectory;
	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","OpenFile")

	filename=GetLineToken(templateCode,(char *)" ",2,&tokenCount);
	if (tokenCount!=2)
	{
		msg.Printf("openfile with too many parameters [%s]",STRING_TO_CHAR(templateCode));
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg)
		progress->LogError(msg,SeverityCaution);
		retVal=false;
	}
	else
	{
		configKey="/General/LocalOutput";
		globalStatistics.configData.ReadTextValue(configKey,&localDirectory,(char *)"");
		STATSGEN_DEBUG(DEBUG_RARELY,localDirectory);
		wxFileName		constructedFile=wxFileName::DirName(localDirectory);
		constructedFile.SetFullName(filename);
		filename=constructedFile.GetFullPath();
		STATSGEN_DEBUG(DEBUG_RARELY,filename);
		openFile=new TemplateOpenFile;
		retVal=openFile->Open(filename,mode);
		if (retVal)
		{
			// Successfully opened file - add it to the openfile stack
			templateVariable.openFiles.Add(openFile);
			createdFilesCount=createdFiles.GetCount();
			found=false;
			for (createdFilesIndex=0;
				createdFilesIndex<createdFilesCount;
				createdFilesIndex++)
			{
				createdFilename=createdFiles.Item(createdFilesIndex);
				if (createdFilename.Cmp(filename)==0)
				{
					found=true;
					break;
				}
			}
			if (!found)
			{
				createdFiles.Add(filename);
			}
		}
		else
		{
			delete openFile;
			msg.Printf("Failed to open output file [%s]",STRING_TO_CHAR(filename));
			STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
			progress->LogError(msg,SeverityError);
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void TemplateFile::AddCreatedFile(wxString &filename)
{
	int			createdFileCount;
	int			createdFileIndex;
	wxString	createdFilename;
	bool		found;

	createdFileCount=createdFiles.GetCount();
	found=false;
	for (createdFileIndex=0;createdFileIndex<createdFileCount;createdFileIndex++)
	{
		createdFilename=createdFiles.Item(createdFileIndex);
		if (createdFilename.Cmp(filename)==0)
		{
			found=true;
			break;
		}
	}
	if (!found)
	{
		createdFiles.Add(filename);
	}
}

bool TemplateFile::Loop(TemplateVariable &templateVariable,
					wxString &templateCodeText,
					wxString &rowVariable,
					wxString &rowIndexVariable,
					wxString &SQLQuery)
{
	bool		retVal=true;
	int			tokenCount;
	int			tokenIndex;
	wxString	token;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","Loop")
	rowVariable=GetLineToken(templateCodeText,(char *)" ",4,&tokenCount);
	rowIndexVariable=GetLineToken(templateCodeText,(char *)" ",7,&tokenCount);
	SQLQuery="";
	for (tokenIndex=8;tokenIndex<=tokenCount;tokenIndex++)
	{
		token=GetLineToken(templateCodeText,(char *)" ",tokenIndex,&tokenCount);
		SQLQuery+=token;
		if (tokenIndex<tokenCount)
		{
			SQLQuery+=" ";
		}
	}

	if (tokenCount<8)
	{
		msg.Printf("loop syntax incorrect [%s]",STRING_TO_CHAR(templateCodeText));
		STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
		progress->LogError(msg,SeverityCaution);
		retVal=false;
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool TemplateFile::ProcessTemplate(bool allowProcessing,
								TemplateVariable &templateVariable,
								int *lineIndex,
								wxString &templateLine,
								bool *endedOnElse)
{
	bool				retVal=true;
	int					lineCount;
	wxString			beforeTemplateCode;
	wxString			afterTemplateCode;
	wxString			templateCodeText;
	wxString			msg;
	int					tokenCount;
	wxString			templateCode;
	bool				conditionResult=true;
	bool				tempElseVar;

	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","ProcessTemplate")
	*endedOnElse=false;
	lineCount=templateLines.GetCount();
	if (templateLine.Length()==0)
	{
		if ((*lineIndex)<(lineCount))
		{
			templateLine=templateLines.Item(*lineIndex);
		}
		else
		{
			// No more lines left to read
			templateLine="";
		}
	}
	while ((retVal)&&((*lineIndex)<lineCount))
	{
		progress->Update(*lineIndex);

		retVal=SplitTemplateLine(templateLine,
						&beforeTemplateCode,
						&templateCodeText,
						&afterTemplateCode);
		// Substitute any included variables
		templateCodeText=templateVariable.SubstituteVariableValues(templateCodeText);
		// We have split the template line up
		if (allowProcessing)
		{
			WriteTemplateDataToOutput(beforeTemplateCode,templateVariable);
		}
		if (templateCodeText.Length()>0)
		{
			// We have a template code to handle
			templateCode=GetLineToken(templateCodeText,(char *)" ",1,&tokenCount);
			if (templateCode.Cmp("openfile")==0)
			{
				if (allowProcessing)
				{
					retVal=OpenFile(templateCodeText,(char *)"w",templateVariable);
				}
			}
			else if (templateCode.CmpNoCase("appendfile")==0)
			{
				if (allowProcessing)
				{
					retVal=OpenFile(templateCodeText,(char *)"a",templateVariable);
				}
			}
			else if (templateCode.CmpNoCase("closefile")==0)
			{
				if (allowProcessing)
				{
					retVal=CloseFile(templateVariable);
				}
			}
			else if (((templateCode.CmpNoCase("loop")==0)))
			{
				wxString	rowIndexValue;
				wxString	rowIndexVariable;
				int			rowIndex;
				wxString	rowVariable;
				wxString	SQLQuery;
				wxString	msg;
				
				retVal=Loop(templateVariable,
							templateCodeText,
							rowVariable,
							rowIndexVariable,
							SQLQuery);
				if (retVal)
				{
					wxString	loopStartAfterTemplateCode;
					int			loopStartIndex;
					bool		queryRan=false;
					
					if (allowProcessing)
					{
						// We have a good loop command
						rowIndex=0;
						// We are going to need to reset the current
						// template file position for each loop start
						loopStartAfterTemplateCode=afterTemplateCode;
						loopStartIndex=(*lineIndex);
						// Store the template query on the stack
						templateVariable.StoreQuery(rowVariable,SQLQuery);
						while (templateVariable.QueryNextRow(rowVariable)&&(retVal))
						{
							queryRan=true;
							// Reset template position to start of loop
							afterTemplateCode=loopStartAfterTemplateCode;
							*lineIndex=loopStartIndex;
							msg.Printf("while loop at lineIndex [%d]",*lineIndex);

							// Store current row index variable
							rowIndexValue.Printf("%d",rowIndex+1);
							templateVariable.StoreVariable(rowIndexVariable,rowIndexValue);
						
							if (afterTemplateCode.Length()==0)
							{
								// We need to move onto the next template line
								(*lineIndex)++;
							}
							retVal=ProcessTemplate(allowProcessing,
													templateVariable,
													lineIndex,
													afterTemplateCode,
													&tempElseVar);
							rowIndex++;
						}
						templateVariable.RemoveQuery(rowVariable);
						if (!queryRan)
						{
//							StatsgenDebug("Loop query produced no results");
							// The query did not run at all (no results) - therefore we need to
							// process the template inbetween with allowprocessing of false
							// to skip past the useless template code
							if (afterTemplateCode.Length()==0)
							{
								// We need to move onto the next template line
								(*lineIndex)++;
							}
							retVal=ProcessTemplate(false,
												templateVariable,
												lineIndex,
												afterTemplateCode,
												&tempElseVar);
						}
					}
					else
					{
						// Loop encountered but allow processing switched off
						// Just call the loop code - but do nothing
						if (afterTemplateCode.Length()==0)
						{
							// We need to move onto the next template line
							(*lineIndex)++;
						}
						retVal=ProcessTemplate(allowProcessing,
												templateVariable,
												lineIndex,
												afterTemplateCode,
												&tempElseVar);
					}
				}
				else
				{
					// Invalid Loop command
					msg.Printf("Invalid loop statement [%s]",STRING_TO_CHAR(templateCodeText));
					STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
					progress->LogError(msg,SeverityCaution);
				}
			}
			else if (templateCode.CmpNoCase("endloop")==0)
			{
				// just need to break out of this loop and return
				break;
			}
			else if (templateCode.CmpNoCase("procedure")==0)
			{
			}
			else if (templateCode.CmpNoCase("return")==0)
			{
			}
			else if (templateCode.CmpNoCase("if")==0)
			{
				wxString	conditionString;
				wxString	conditionResultString;

				conditionString=templateCodeText.AfterFirst(' ');
				conditionString.Trim(true);
				conditionString.Trim(false);
				conditionResultString=templateVariable.GetVariableValue(conditionString);
				conditionResult=(conditionResultString.CmpNoCase(TRUE_STRING)==0);
				if (afterTemplateCode.Length()==0)
				{
					// We need to move onto the next template line
					(*lineIndex)++;
				}
				tempElseVar=false;
				retVal=ProcessTemplate(conditionResult && allowProcessing,
										templateVariable,
										lineIndex,
										afterTemplateCode,
										&tempElseVar);
				if (tempElseVar)
				{
					// We have ended on an else - so process template again with
					// opposite status
					if (afterTemplateCode.Length()==0)
					{
						// We need to move onto the next template line
						(*lineIndex)++;
					}
					retVal=ProcessTemplate((!conditionResult) && allowProcessing,
										templateVariable,
										lineIndex,
										afterTemplateCode,
										&tempElseVar);
				}
			}
			else if (templateCode.CmpNoCase("else")==0)
			{
				// just need to finish the file (indicating an "ELSE" exit)
				*endedOnElse=true;
				break;
			}
			else if (templateCode.CmpNoCase("endif")==0)
			{
				// just need to finish the file (indicating an "ENDIF" exit)
				*endedOnElse=false;
				break;
			}
			else
			{
				// This template code text is just a variable value now
				if (allowProcessing)
				{
					wxString	value;

					value=templateVariable.GetVariableValue(templateCodeText);
					WriteTemplateDataToOutput(value,templateVariable);
				}
			}
		}
		if (afterTemplateCode.Length()>0)
		{
			// More data after the template code
			// just set the template line to be the
			// after bit and process round loop again
			// it will be treated as a new template line
			templateLine=afterTemplateCode;
		}
		else
		{
			// Finished with this line - move on to next
			// if there is one
			(*lineIndex)++;
			if ((*lineIndex)<lineCount)
			{
				if (allowProcessing)
				{
					wxString	newLine="\n";
					WriteTemplateDataToOutput(newLine,templateVariable);
				}
				templateLine=templateLines.Item((*lineIndex));
			}
		}
	}


	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void TemplateFile::Debug()
{
	wxString	errorMessage;
	int			lineCount;
	int			lineIndex;
	wxString	line;

	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","Debug")
	lineCount=templateLines.GetCount();
	STATSGEN_DEBUG_CODE(errorMessage.Printf("%d template lines loaded",lineCount);)
	STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	for (lineIndex=0;lineIndex<lineCount;lineIndex++)
	{
		line=templateLines.Item(lineIndex);
		STATSGEN_DEBUG_CODE(errorMessage.Printf("%5d: [%s]",lineIndex,STRING_TO_CHAR(line));)
		STATSGEN_DEBUG(DEBUG_ALWAYS,errorMessage);
	}
	STATSGEN_DEBUG_FUNCTION_END
}
void TemplateFile::GetCreatedFiles(wxArrayString &filelist)
{
	wxString	filename;
	int			fileCount;
	int			fileIndex;

	filelist.Clear();
	fileCount=createdFiles.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filename=createdFiles.Item(fileIndex);
		filelist.Add(filename);
	}
}
void TemplateFile::Unload()
{
	int								fileCount;
	int								fileIndex;
	int								procedureCount;
	int								procedureIndex;
	CompiledTemplateLinePROCEDURE	*procedure;

	fileCount=usedTemplateFiles.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		wxTextFile	*textFile=(wxTextFile *)usedTemplateFiles.Item(fileIndex);
		if (textFile!=NULL)
		{
			delete textFile;
		}
	}

	usedTemplateFiles.Clear();
	usedTemplateFilenames.Clear();
	procedureCount=customProcedures.GetCount();
	for (procedureIndex=0;procedureIndex<procedureCount;procedureIndex++)
	{
		procedure = (CompiledTemplateLinePROCEDURE *)customProcedures.Item(procedureIndex);
		delete procedure;
	}
}

void TemplateFile::LoadTemplateFile(wxString &filename)
{
	int			fileCount;
	int			fileIndex;
	bool		found=false;
	wxString	foundFilename;

	fileCount=usedTemplateFiles.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		foundFilename=usedTemplateFilenames.Item(fileIndex);
		if (foundFilename.Cmp(filename)==0)
		{
			// Already loaded this stuff - ignore
			found=true;
			break;
		}
	}

	if (!found)
	{
		wxTextFile		*textFile;

		textFile=new wxTextFile();
		textFile->Open(filename);
		usedTemplateFiles.Add(textFile);
		usedTemplateFilenames.Add(filename);
	}
}

void TemplateFile::ExportToCPP(wxString &filename,wxString &prefix)
{
	int			fileCount;
	int			fileIndex;
	wxString	templatefilename;
	wxTextFile	outputFile;
	wxString	cppLine;
	wxString	fileLine;
	int			lineCount;
	int			lineIndex;

	outputFile.Create(filename);
	cppLine="#include <wx/wx.h>";outputFile.AddLine(cppLine);
	cppLine="void ExportTemplateFile(wxString &directory,wxArrayString &fileLines,char *filename);";outputFile.AddLine(cppLine);
	cppLine.Printf("void ExportTemplate%s(wxString &directory)",STRING_TO_CHAR(prefix));outputFile.AddLine(cppLine);
	cppLine="{";outputFile.AddLine(cppLine);
	cppLine="    wxString      filename;";outputFile.AddLine(cppLine);
	cppLine="    wxArrayString fileLines;";outputFile.AddLine(cppLine);
	cppLine="    wxString      fileLine;";outputFile.AddLine(cppLine);

	fileCount=usedTemplateFiles.GetCount();
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		wxTextFile		*textFile;

		templatefilename=usedTemplateFilenames.Item(fileIndex);
		textFile=(wxTextFile *)usedTemplateFiles.Item(fileIndex);
		lineCount=textFile->GetLineCount();
		cppLine="    fileLines.Clear();";outputFile.AddLine(cppLine);
		for (lineIndex=0;lineIndex<lineCount;lineIndex++)
		{
			fileLine=textFile->GetLine(lineIndex);
			fileLine.Replace("\\","\\\\");
			fileLine.Replace("\"","\\\"");
			cppLine.Printf("    fileLine=\"%s\";fileLines.Add(fileLine);",STRING_TO_CHAR(fileLine));
			outputFile.AddLine(cppLine);
		}
		wxFileName	constructedName(templatefilename);

		cppLine.Printf("    ExportTemplateFile(directory,fileLines,(char *)\"%s\");",STRING_TO_CHAR(constructedName.GetFullName()));
		outputFile.AddLine(cppLine);
	}
	cppLine="}";outputFile.AddLine(cppLine);

	outputFile.Write();
	outputFile.Close();
}

bool TemplateFile::Compile(
						CompiledTemplateSection	&compiledSection,
						int *currentLineIndex,
						wxString &currentTemplateLine,
						int *finishedType,
						TemplateVariable	&templateVariable)
{
	// Compiling involves creating "Compiled Lines" of code
	// A compiled line is a list of CompiledStatements
	// Compiled Statements are individual commands
	// Such as Raw Output
	// Loop / End Loop
	// If / Else / End If
	// and any of the other various functions
	// Each can hold "sub lines" of text
	// Template Variables are global throughout
	wxString		msg;
	bool			retVal=true;
	wxString		beforeTemplateCode;
	wxString		afterTemplateCode;
	wxString		templateCodeText;
	bool			lineSplitOK;
	bool			compileResult;
	int				lineCount;
	wxString		rawTemplateCode;
	bool			finished;
	int				compiledTemplateCommand;
	int				sectionStartIndex;
	CompiledTemplateLinePROCEDURE	*compiledLinePROCEDURE;
	CompiledTemplateLineIF			*compiledLineIF;
	CompiledTemplateLineRaw			*compiledLineRaw;
	CompiledTemplateLineLOOP		*compiledLineLOOP;
	CompiledTemplateLineOPENFILE	*compiledLineOPENFILE;
	CompiledTemplateLineAPPENDFILE	*compiledLineAPPENDFILE;
	CompiledTemplateLineCLOSEFILE	*compiledLineCLOSEFILE;
	CompiledTemplateLineVARIABLE	*compiledLineVARIABLE;

	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","Compile")
	lineCount=templateLines.GetCount();

	// Hack around for when we are first starting template
	if ((*currentLineIndex)==-1)
	{
		(*currentLineIndex)++;
		if ((*currentLineIndex)<lineCount)
		{
			currentTemplateLine=templateLines.Item(*currentLineIndex);
		}
	}
	finished=false;
	rawTemplateCode="";
	
	while (!finished)
	{
		STATSGEN_DEBUG_CODE(
		msg.Printf("line#=[%d] of [%d] starting template code=[%s]",
			*currentLineIndex,lineCount,STRING_TO_CHAR(currentTemplateLine));
							)
		STATSGEN_DEBUG(DEBUG_RARELY,STRING_TO_CHAR(msg))
		progress->Update(*currentLineIndex);

		if (currentTemplateLine.Length()==0)
		{
			// The current template line is empty indicating we need the next
			// line or we have reached the end of the file
			(*currentLineIndex)++;
			if ((*currentLineIndex)<lineCount)
			{
				// we have more tempate lines to process
				currentTemplateLine=templateLines.Item(*currentLineIndex);
			}
			else
			{
				// Reached end of file - time to exit the section
				*finishedType=TEMPLATE_FINISHED_EOF;
				finished=true;
				break;
			}
		}
		lineSplitOK=SplitTemplateLine(currentTemplateLine,
						&beforeTemplateCode,
						&templateCodeText,
						&afterTemplateCode);
		rawTemplateCode+=beforeTemplateCode;
		STATSGEN_DEBUG(DEBUG_RARELY,rawTemplateCode)
		if (lineSplitOK)
		{
			compiledLineRaw=new CompiledTemplateLineRaw;
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"split the line ok")
			STATSGEN_DEBUG(DEBUG_RARELY,beforeTemplateCode)
			STATSGEN_DEBUG(DEBUG_RARELY,templateCodeText)
			STATSGEN_DEBUG(DEBUG_RARELY,afterTemplateCode)
			if (templateCodeText.Length()>0)
			{
				// We have found some template code - any current
				// rawTemplateCode needs adding to the list of compiled
				// template sections
				compiledLineRaw->SetRaw(rawTemplateCode);
				rawTemplateCode="";
				compiledSection.Add(compiledLineRaw);

				// now we have added the raw section we can decode the template
				// code bit
				compiledTemplateCommand=IdentifyTemplateCode(templateCodeText);
				STATSGEN_DEBUG_CODE(
				msg.Printf("compiledTemplateCommand=[%d]",compiledTemplateCommand);
								)
				sectionStartIndex=(*currentLineIndex);
				STATSGEN_DEBUG(DEBUG_RARELY,msg)
				switch (compiledTemplateCommand)
				{
					case TEMPLATE_COMMAND_IF:
						compiledLineIF=new CompiledTemplateLineIF;
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"IF")
						compiledLineIF->SetTemplateCode(templateCodeText);
						// Now compile again using the
						// next bit of the code
						// it should finish with either ELSE or ENDIF
						compileResult=Compile(compiledLineIF->trueSection,
											currentLineIndex,
											afterTemplateCode,
											finishedType,
											templateVariable);
						if ((*finishedType)==TEMPLATE_FINISHED_ELSE)
						{
							STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Finished on an else")
							// Else statement - another subsection
							compileResult=Compile(compiledLineIF->falseSection,
												currentLineIndex,
												afterTemplateCode,
												finishedType,
												templateVariable);
						}
						if ((*finishedType)==TEMPLATE_FINISHED_ENDIF)
						{
							STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Finished on an endif")
							// success - we have finished this section
							finished=false;
							*finishedType=TEMPLATE_FINISHED_OK;
							retVal=true;
						}
						else
						{
							msg.Printf("IF with no ENDIF [%s]",
								STRING_TO_CHAR(TemplateErrorDetails(
										sectionStartIndex)));
							progress->LogError(msg,SeverityError);
							STATSGEN_DEBUG_CODE(
							msg.Printf("Finished on %d",*finishedType);
												)
							STATSGEN_DEBUG(DEBUG_RARELY,msg)
							// if not terminated by an endif
							finished=true;
							*finishedType=TEMPLATE_FINISHED_ERROR;
							retVal=false;
						}
						compiledSection.Add(compiledLineIF);
						break;
					case TEMPLATE_COMMAND_ELSE:
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"ELSE")
						// Finished this section
						*finishedType=TEMPLATE_FINISHED_ELSE;
						finished=true;
						retVal=true;
						break;
					case TEMPLATE_COMMAND_ENDIF:
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"ENDIF")
						// Finished this section
						*finishedType=TEMPLATE_FINISHED_ENDIF;
						finished=true;
						retVal=true;
						break;
					case TEMPLATE_COMMAND_PROCEDURE:
						compiledLinePROCEDURE=new CompiledTemplateLinePROCEDURE;
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"PROCEDURE")
						compiledLinePROCEDURE->SetTemplateCode(templateCodeText);
						compileResult=Compile(compiledLinePROCEDURE->procedureBody,
											currentLineIndex,
											afterTemplateCode,
											finishedType,
											templateVariable);
						if ((*finishedType)==TEMPLATE_FINISHED_RETURN)
						{
							STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Finished on RETURN")
							finished=false;
							*finishedType=TEMPLATE_FINISHED_OK;
							retVal=true;
							AddCustomProcedure(compiledLinePROCEDURE);
						}
						else
						{
							msg.Printf("PROCEDURE with no RETURN [%s]",
								STRING_TO_CHAR(TemplateErrorDetails(
										sectionStartIndex)));
							progress->LogError(msg,SeverityError);
							STATSGEN_DEBUG_CODE(
							msg.Printf("Finished on %d",*finishedType);
												)
							STATSGEN_DEBUG(DEBUG_RARELY,msg)
							// if not terminated by an return
							finished=true;
							*finishedType=TEMPLATE_FINISHED_ERROR;
							retVal=false;
						}
						//compiledSection.Add(compiledLinePROCEDURE);
						break;
					case TEMPLATE_COMMAND_RETURN:
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"RETURN")
						// Finished this section
						*finishedType=TEMPLATE_FINISHED_RETURN;
						retVal=true;
						finished=true;
						break;
					case TEMPLATE_COMMAND_LOOP:
						compiledLineLOOP=new CompiledTemplateLineLOOP;
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"LOOP")
						compiledLineLOOP->SetTemplateCode(templateCodeText);
						// Now compile again using the
						// next bit of the code
						// it should finish with ENDLOOP
						compileResult=Compile(compiledLineLOOP->subSection,
											currentLineIndex,
											afterTemplateCode,
											finishedType,
											templateVariable);
						if ((*finishedType)==TEMPLATE_FINISHED_ENDLOOP)
						{
							STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Finished on ENDLOOP")
							finished=false;
							*finishedType=TEMPLATE_FINISHED_OK;
							retVal=true;
						}
						else
						{
							msg.Printf("LOOP with no ENDLOOP [%s]",
								STRING_TO_CHAR(TemplateErrorDetails(
										sectionStartIndex)));
							progress->LogError(msg,SeverityError);
							STATSGEN_DEBUG_CODE(
							msg.Printf("Finished on %d",*finishedType);
												)
							STATSGEN_DEBUG(DEBUG_RARELY,msg)
							// if not terminated by an endloop
							finished=true;
							*finishedType=TEMPLATE_FINISHED_ERROR;
							retVal=false;
						}
						compiledSection.Add(compiledLineLOOP);
						break;
					case TEMPLATE_COMMAND_ENDLOOP:
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"ENDLOOP")
						// Finished this section
						*finishedType=TEMPLATE_FINISHED_ENDLOOP;
						retVal=true;
						finished=true;
						break;
					case TEMPLATE_COMMAND_OPENFILE:
						compiledLineOPENFILE=new CompiledTemplateLineOPENFILE;
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"OPENFILE")
						compiledLineOPENFILE->SetTemplateCode(templateCodeText);
						compiledSection.Add(compiledLineOPENFILE);
						break;
					case TEMPLATE_COMMAND_APPENDFILE:
						compiledLineAPPENDFILE=new CompiledTemplateLineAPPENDFILE;
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"APPENDFILE")
						compiledLineAPPENDFILE->SetTemplateCode(templateCodeText);
						compiledSection.Add(compiledLineAPPENDFILE);
						break;
					case TEMPLATE_COMMAND_CLOSEFILE:
						compiledLineCLOSEFILE=new CompiledTemplateLineCLOSEFILE;
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"CLOSEFILE")
						compiledLineCLOSEFILE->SetTemplateCode(templateCodeText);
						compiledSection.Add(compiledLineCLOSEFILE);
						break;
					case TEMPLATE_COMMAND_VARIABLE:
						compiledLineVARIABLE=new CompiledTemplateLineVARIABLE;
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"VARIABLE")
						compiledLineVARIABLE->SetTemplateCode(templateCodeText);
						compiledSection.Add(compiledLineVARIABLE);
						break;
					default:
						msg.Printf("Unknown Template Code [%s]",
								STRING_TO_CHAR(TemplateErrorDetails(
										sectionStartIndex)));
						progress->LogError(msg,SeverityError);
						STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Unknown Template Code")
						// Unknown Template Command
						finished=true;
						*finishedType=TEMPLATE_FINISHED_ERROR;
						retVal=false;
						break;
				}

				STATSGEN_DEBUG(DEBUG_RARELY,afterTemplateCode)
				// now that we have sorted out the template code bit
				// we can advance to the section after the template code
				currentTemplateLine=afterTemplateCode;
			}
			else
			{
				STATSGEN_DEBUG(DEBUG_RARELY,(char *)"no template code left - add to raw")
				// No template code - so we are moving to a new line of raw data
				// add in the linefeed
				//rawTemplateCode+="\n";
				currentTemplateLine="";
			}
		}
		else
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Failed to split the template line")
			// failed to split the template line - we are finished
			*finishedType=TEMPLATE_FINISHED_ERROR;
			currentTemplateLine="";
			finished=true;
		}
	}

	return (retVal);
	STATSGEN_DEBUG_FUNCTION_END
}

void TemplateFile::AddCustomProcedure(CompiledTemplateLinePROCEDURE *procedure)
{
	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","AddCustomProcedure")
	CompiledTemplateLinePROCEDURE	*foundProcedure;

	foundProcedure = FindCustomProcedure(procedure->procedureName);
	if (foundProcedure == NULL)
	{
		customProcedures.Add(procedure);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

CompiledTemplateLinePROCEDURE *TemplateFile::FindCustomProcedure(wxString &procedureName)
{
	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","FindCustomProcedure")
	int			procedureCount;
	int			procedureIndex;
	wxString	msg;

	CompiledTemplateLinePROCEDURE	*foundProcedure=NULL;
	CompiledTemplateLinePROCEDURE	*currentProcedure;

	procedureCount = customProcedures.GetCount();
	for (procedureIndex = 0;procedureIndex < procedureCount;procedureIndex++)
	{
		STATSGEN_DEBUG_CODE(msg.Printf("checking procedure %d of %d for [%s]",procedureIndex,procedureCount,STRING_TO_CHAR(procedureName));)
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
		currentProcedure = (CompiledTemplateLinePROCEDURE *)customProcedures.Item(procedureIndex);
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"got a procedure - checking it's name")
		STATSGEN_DEBUG(DEBUG_RARELY,procedureName)
		if (currentProcedure->procedureName.CmpNoCase(procedureName) == 0)
		{
			foundProcedure = currentProcedure;
			break;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (foundProcedure);

}

wxString TemplateFile::TemplateErrorDetails(int lineIndex)
{
	wxString	templateLine="";
	wxString	lineRef="";
	wxString	errorDetails;

	if (lineIndex>=0)
	{
		if (lineIndex<templateLines.GetCount())
		{
			templateLine=templateLines.Item(lineIndex);
		}
		if (lineIndex<templateLineRef.GetCount())
		{
			lineRef=templateLineRef.Item(lineIndex);
		}
	}

	errorDetails.Printf("[%s]@[%s]",STRING_TO_CHAR(templateLine),STRING_TO_CHAR(lineRef));
	return (errorDetails);
}
int TemplateFile::IdentifyTemplateCode(wxString &templateCode)
{
	int			retVal;
	int			tokenCount;
	wxString	templateCommand;

	STATSGEN_DEBUG_FUNCTION_START("TemplateFile","IdentifyTemplateCode")
	STATSGEN_DEBUG(DEBUG_RARELY,templateCode)
	retVal=TEMPLATE_COMMAND_VARIABLE;
	templateCommand=GetLineToken(templateCode,(char *)" ",1,&tokenCount);
	templateCommand.MakeLower();
	STATSGEN_DEBUG(DEBUG_RARELY,templateCommand)
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("if")==0))
	{
		retVal = TEMPLATE_COMMAND_IF;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("else")==0))
	{
		retVal = TEMPLATE_COMMAND_ELSE;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("endif")==0))
	{
		retVal = TEMPLATE_COMMAND_ENDIF;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("loop")==0))
	{
		retVal = TEMPLATE_COMMAND_LOOP;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("endloop")==0))
	{
		retVal = TEMPLATE_COMMAND_ENDLOOP;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("openfile")==0))
	{
		retVal = TEMPLATE_COMMAND_OPENFILE;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("appendfile")==0))
	{
		retVal = TEMPLATE_COMMAND_APPENDFILE;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("closefile")==0))
	{
		retVal = TEMPLATE_COMMAND_CLOSEFILE;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("procedure")==0))
	{
		retVal = TEMPLATE_COMMAND_PROCEDURE;
	}
	if ((retVal == TEMPLATE_COMMAND_VARIABLE) && (templateCommand.Cmp("return")==0))
	{
		retVal = TEMPLATE_COMMAND_RETURN;
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

CompiledTemplateLine::CompiledTemplateLine()
{
	usesSubstitution=false;
	templateCode="";
	templateCodeParameters="";
}

CompiledTemplateLine::~CompiledTemplateLine()
{
}

bool CompiledTemplateLine::SetTemplateCode(wxString &templateCodeIn)
{
	bool retVal;
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLine","SetTemplateCode")
	templateCode=templateCodeIn;
	retVal=ParseTemplateCode();
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void CompiledTemplateLine::SplitParameters()
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLine","SplitParameters")
	// take the template code and split out the general parameter string
	templateCodeParameters=templateCode.AfterFirst(' ');
	STATSGEN_DEBUG(DEBUG_RARELY,templateCodeParameters)
	STATSGEN_DEBUG_FUNCTION_END
}

bool CompiledTemplateLine::ParseTemplateCode()
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLine","ParseTemplateCode")
	// Should never get here
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

bool CompiledTemplateLine::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLine","Process")
	// Should never get here
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

bool CompiledTemplateLine::UsesSubstitution()
{
	int	 		charIndexParameters;
	int	 		charIndexCode;
	bool		retVal=false;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLine","UsesSubstitution")
	charIndexParameters=templateCodeParameters.Find("%%");
	charIndexCode=templateCode.Find("%%");
	retVal=((charIndexCode>=0)||(charIndexParameters>=0));

	STATSGEN_DEBUG_CODE(
	msg.Printf("charIndex=%d,%d",charIndexCode,charIndexParameters);
						)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

CompiledTemplateLineRaw::~CompiledTemplateLineRaw()
{
}

void CompiledTemplateLineRaw::SetRaw(wxString &rawTextIn)
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineRaw","SetRaw")
	rawText=rawTextIn;
	STATSGEN_DEBUG(DEBUG_RARELY,rawText)
	STATSGEN_DEBUG_FUNCTION_END
}

bool CompiledTemplateLineRaw::ParseTemplateCode()
{
	// Nothing to parse for raw
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineRaw","ParseTemplateCode")
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

bool CompiledTemplateLineRaw::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineRaw","Process")
	progress->WakeUp();
	templateFile.WriteTemplateDataToOutput(rawText,variableList);
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

CompiledTemplateLineIF::~CompiledTemplateLineIF()
{
}

bool CompiledTemplateLineIF::ParseTemplateCode()
{
	bool retVal;
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineIF","ParseTemplateCode")

	SplitParameters();
	// If should have a parameter surrounded by ( )
	templateCodeParameters.Trim(true);
	templateCodeParameters.Trim(false);
	STATSGEN_DEBUG(DEBUG_RARELY,templateCodeParameters)
	if (templateCodeParameters.StartsWith("(") &&
		templateCodeParameters.EndsWith(")"))
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Appears correctly formatted")
		retVal=true;
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Syntax error")
		// error in syntax
		retVal=false;
	}
	usesSubstitution=UsesSubstitution();
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool CompiledTemplateLineIF::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	wxString	conditionResultString;
	bool		conditionResult;
	wxString	conditionStatement;
	bool		retVal=true;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineIF","Process")
	progress->WakeUp();
	if (usesSubstitution)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Substitution required")
		conditionStatement=variableList.SubstituteVariableValues(templateCodeParameters);
	}
	else
	{
		conditionStatement=templateCodeParameters;
	}
	STATSGEN_DEBUG(DEBUG_RARELY,conditionStatement)
	conditionResultString=variableList.GetVariableValue(conditionStatement);
	STATSGEN_DEBUG(DEBUG_RARELY,conditionResultString)
	conditionResult=(conditionResultString.CmpNoCase(TRUE_STRING)==0);
	if (conditionResult)
	{
		retVal=trueSection.Process(templateFile,variableList);
	}
	else
	{
		retVal=falseSection.Process(templateFile,variableList);
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

CompiledTemplateLineLOOP::~CompiledTemplateLineLOOP()
{
}

bool CompiledTemplateLineLOOP::ParseTemplateCode()
{
	bool retVal=true;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineLOOP","ParseTemplateCode")
	SplitParameters();
	usesSubstitution=UsesSubstitution();

	if (!usesSubstitution)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Doesn't use substitution")
		// as it does not use substitution we can extract stuff out
		// straight away
		retVal = SplitLoopParameters(templateCodeParameters);
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool CompiledTemplateLineLOOP::SplitLoopParameters(wxString &parameterString)
{
	bool		retVal=true;
	int			tokenCount;
	wxString	token;
	int			tokenIndex;
	wxString	msg;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineLOOP","SplitLoopParameters")
	STATSGEN_DEBUG(DEBUG_RARELY,parameterString)
	// separate out the loop parameters
	rowVariableName="";
	indexVariableName="";
	sql="";
	token=GetLineToken(parameterString,(char *)" ",3,&tokenCount);
	STATSGEN_DEBUG_CODE(
	msg.Printf("tokenCount [%d], first important token [%s]",tokenCount,STRING_TO_CHAR(token));
						)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)
	if (tokenCount<7)
	{
		// not enough tokens
		retVal=false;
	}
	else
	{
		rowVariableName=token;
		token=GetLineToken(parameterString,(char *)" ",6,&tokenCount);
		indexVariableName=token;
	}

	if (retVal)
	{
		for (tokenIndex=7;tokenIndex<=tokenCount;tokenIndex++)
		{
			token=GetLineToken(parameterString,(char *)" ",tokenIndex,&tokenCount);
			if (sql.Length()>0)
			{
				sql+=" ";
			}
			sql+=token;
		}
		STATSGEN_DEBUG(DEBUG_RARELY,rowVariableName)
		STATSGEN_DEBUG(DEBUG_RARELY,indexVariableName)
		STATSGEN_DEBUG(DEBUG_RARELY,sql)
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool CompiledTemplateLineLOOP::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	bool		retVal=true;
	bool		queryRan=false;
	int			rowIndex=0;
	wxString	rowIndexValue;
	wxString	loopStatement;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineLOOP","Process")
	// Store the template query on the stack
	if (usesSubstitution)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Substitution required")
		loopStatement=variableList.SubstituteVariableValues(templateCodeParameters);
		STATSGEN_DEBUG(DEBUG_RARELY,loopStatement)
		SplitLoopParameters(loopStatement);
	}
	variableList.StoreQuery(rowVariableName,sql);
	while (variableList.QueryNextRow(rowVariableName)&&(retVal))
	{
		progress->WakeUp();
		queryRan=true;

		// Store current row index variable
		rowIndexValue.Printf("%d",rowIndex+1);
		STATSGEN_DEBUG(DEBUG_RARELY,rowIndexValue)
		variableList.StoreVariable(indexVariableName,rowIndexValue);
		retVal=subSection.Process(templateFile,variableList);
		if (!retVal)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"subsection failed")
			break;
		}
		rowIndex++;
	}
	variableList.RemoveQuery(rowVariableName);
	if (!queryRan)
	{
	}
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}


CompiledTemplateLineOPENFILE::~CompiledTemplateLineOPENFILE()
{
}

bool CompiledTemplateLineOPENFILE::ParseTemplateCode()
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineOPENFILE","ParseTemplateCode")
	SplitParameters();
	usesSubstitution=UsesSubstitution();
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

char *CompiledTemplateLineOPENFILE::Mode()
{
	return((char *)"w");
}

bool CompiledTemplateLineOPENFILE::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	wxString			configKey;
	wxString			localDirectory;
	bool				retVal;
	wxString			filename;
	TemplateOpenFile	*openFile;
	wxString			msg;
	
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineOPENFILE","Process")
	progress->WakeUp();
	configKey="/General/LocalOutput";
	globalStatistics.configData.ReadTextValue(configKey,&localDirectory,(char *)"");

	wxFileName		constructedFile=wxFileName::DirName(localDirectory);
	STATSGEN_DEBUG(DEBUG_RARELY,templateCodeParameters)
	if (usesSubstitution)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Substitution required")
		filename=variableList.SubstituteVariableValues(templateCodeParameters);
	}
	else
	{
		filename=templateCodeParameters;
	}
	STATSGEN_DEBUG(DEBUG_RARELY,filename)
	constructedFile.SetFullName(filename);
	filename=constructedFile.GetFullPath();
	STATSGEN_DEBUG(DEBUG_RARELY,filename)
	openFile=new TemplateOpenFile;
	retVal=openFile->Open(filename,Mode());
	if (retVal)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"file opened ok")
		// Successfully opened file - add it to the openfile stack
		variableList.openFiles.Add(openFile);
		templateFile.AddCreatedFile(filename);
	}
	else
	{
		delete openFile;
		msg.Printf("Failed to open output file [%s]",STRING_TO_CHAR(filename));
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
		progress->LogError(msg,SeverityError);
	}

	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

CompiledTemplateLineAPPENDFILE::CompiledTemplateLineAPPENDFILE()
{
}

CompiledTemplateLineAPPENDFILE::~CompiledTemplateLineAPPENDFILE()
{
}

bool CompiledTemplateLineAPPENDFILE::ParseTemplateCode()
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineAPPENDFILE","ParseTemplateCode")
	SplitParameters();
	usesSubstitution=UsesSubstitution();
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

char *CompiledTemplateLineAPPENDFILE::Mode()
{
	return((char *)"a");
}

CompiledTemplateLineCLOSEFILE::~CompiledTemplateLineCLOSEFILE()
{
}

bool CompiledTemplateLineCLOSEFILE::ParseTemplateCode()
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineCLOSEFILE","CloseFile")
	SplitParameters();
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

bool CompiledTemplateLineCLOSEFILE::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineAPPENDFILE","Process")
	progress->WakeUp();
	templateFile.CloseFile(variableList);
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

CompiledTemplateLineVARIABLE::~CompiledTemplateLineVARIABLE()
{
}

bool CompiledTemplateLineVARIABLE::ParseTemplateCode()
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineVARIABLE","ParseTemplateCode")
	usesSubstitution=UsesSubstitution();
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

bool CompiledTemplateLineVARIABLE::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLineVARIABLE","Process")
	wxString	value;
	wxString	variableName;

	progress->WakeUp();
	STATSGEN_DEBUG(DEBUG_RARELY,templateCode)
	if (usesSubstitution)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Substitution required")
		variableName=variableList.SubstituteVariableValues(templateCode);
	}
	else
	{
		variableName=templateCode;
	}
	STATSGEN_DEBUG(DEBUG_RARELY,variableName)
	value=variableList.GetVariableValue(variableName);
	templateFile.WriteTemplateDataToOutput(value,variableList);
	STATSGEN_DEBUG(DEBUG_RARELY,value)
	STATSGEN_DEBUG_FUNCTION_END
	return (true);
}

CompiledTemplateSection::CompiledTemplateSection()
{
}


CompiledTemplateSection::~CompiledTemplateSection()
{
	int						compiledTemplateLineIndex;
	CompiledTemplateLine	*compiledTemplateLine;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateSection","Destructor")

	/*
	for (compiledTemplateLineIndex=0;
		compiledTemplateLineIndex<compiledTemplateLineCount;
		compiledTemplateLineIndex++)
	{
		compiledTemplateLine=(CompiledTemplateLine *)compiledTemplateLines.Item(compiledTemplateLineIndex);
		delete compiledTemplateLine;
	}
	*/

	STATSGEN_DEBUG_FUNCTION_END
}


void CompiledTemplateSection::Add(CompiledTemplateLine *compiledTemplateLine)
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateSection","Add")
	compiledTemplateLines.Add(compiledTemplateLine);
	STATSGEN_DEBUG_FUNCTION_END
}

bool CompiledTemplateSection::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	bool					retVal;
	int						compiledTemplateLineCount;
	int						compiledTemplateLineIndex;
	CompiledTemplateLine	*compiledTemplateLine;
	wxString				msg;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateSection","Process")
	compiledTemplateLineCount=compiledTemplateLines.GetCount();

	retVal=true;
		STATSGEN_DEBUG_CODE(
	msg.Printf("section compiled line count = %d",compiledTemplateLineCount);
							)
	STATSGEN_DEBUG(DEBUG_RARELY,msg)
	for (compiledTemplateLineIndex=0;
		compiledTemplateLineIndex<compiledTemplateLineCount;
		compiledTemplateLineIndex++)
	{
		progress->WakeUp();
		STATSGEN_DEBUG_CODE(
		msg.Printf("line index = %d",compiledTemplateLineIndex);
							)
		STATSGEN_DEBUG(DEBUG_RARELY,msg)
		compiledTemplateLine=(CompiledTemplateLine *)compiledTemplateLines.Item(compiledTemplateLineIndex);
		retVal=compiledTemplateLine->Process(templateFile,variableList);
		if (!retVal)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Failed to process compiled line")
			break;
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
	return(retVal);
}

CompiledTemplateLinePROCEDURE::~CompiledTemplateLinePROCEDURE()
{
}

bool CompiledTemplateLinePROCEDURE::ParseTemplateCode()
{
	bool retVal;
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLinePROCEDURE","ParseTemplateCode")
	wxString	parameterList;
	wxString	parameter;

	SplitParameters();
	// should have a parameter surrounded by ( ) and preceeded by the procedure name
	templateCodeParameters.Trim(true);
	templateCodeParameters.Trim(false);
	STATSGEN_DEBUG(DEBUG_RARELY,templateCodeParameters)
	procedureName=templateCodeParameters.BeforeFirst('(');
	parameterList=templateCodeParameters.AfterFirst('(');
	parameterList=parameterList.BeforeLast(')');
	STATSGEN_DEBUG(DEBUG_RARELY,procedureName);
	STATSGEN_DEBUG(DEBUG_RARELY,parameterList);
	if (procedureName.Length()>0)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Appears correctly formatted");
		while (parameterList.Length()>0)
		{
			STATSGEN_DEBUG(DEBUG_RARELY,parameterList);
			parameter=parameterList.BeforeFirst(',');
			STATSGEN_DEBUG(DEBUG_RARELY,parameter);
			parameterList=parameterList.AfterFirst(',');
			STATSGEN_DEBUG(DEBUG_RARELY,parameterList);
			parameterVars.Add(parameter);
		}
		retVal=true;
	}
	else
	{
		STATSGEN_DEBUG(DEBUG_RARELY,(char *)"Syntax error")
		// error in syntax
		retVal=false;
	}
	usesSubstitution=false;
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

bool CompiledTemplateLinePROCEDURE::Process(TemplateFile &templateFile,
									TemplateVariable &variableList)
{
	wxString	parameterListValues;
	bool		retVal=true;

	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLinePROCEDURE","Process")
	progress->WakeUp();
	/*
	if (usesSubstitution)
	{
		STATSGEN_DEBUG(DEBUG_RARELY,"Substitution required")
		parameterListValues=variableList.SubstituteVariableValues(templateCodeParameters);
	}
	else
	{
		parameterListValues=templateCodeParameters;
	}
	STATSGEN_DEBUG(DEBUG_RARELY,parameterListValues)
	//conditionResultString=variableList.GetVariableValue(conditionStatement);
	*/
	STATSGEN_DEBUG_FUNCTION_END
	return (retVal);
}

void CompiledTemplateLinePROCEDURE::Execute(TemplateFile *templateFile,TemplateVariable &variableList,TemplateValueList *parameterValues)
{
	STATSGEN_DEBUG_FUNCTION_START("CompiledTemplateLinePROCEDURE","Execute")
	int						parameterCount;
	int						parameterIndex;
	int						parameterVarCount;
	wxString				parameter;
	wxString				parameterVar;
	wxString				msg;
	TemplateValue			*parameterValue;
	wxString				valueString;

	parameterVarCount=parameterVars.GetCount();
	parameterCount = parameterValues->GetCount();

	for (parameterIndex=0;(parameterIndex<parameterCount) && (parameterIndex<parameterVarCount);parameterIndex++)
	{
		parameterValue=parameterValues->Item(parameterIndex);
		parameter=parameterVars.Item(parameterIndex);
		parameterVar.Printf("self_%s",STRING_TO_CHAR(parameter));
		STATSGEN_DEBUG(DEBUG_RARELY,parameter)
		STATSGEN_DEBUG(DEBUG_RARELY,parameterVar)
		STATSGEN_DEBUG(DEBUG_RARELY,parameterValue->GetString())
		if (parameterValue->IsQuery())
		{
			TemplateOpenQuery	*clonedQuery;

			clonedQuery=new TemplateOpenQuery();
			clonedQuery->Clone(parameterValue->GetQuery());
			variableList.StoreOpenQuery(parameterVar,clonedQuery);
		}
		else
		{
			valueString = parameterValue->GetString();
			variableList.StoreVariable(parameterVar,valueString);
		}
	}
	procedureBody.Process(*templateFile,variableList);
	STATSGEN_DEBUG_FUNCTION_END
}
