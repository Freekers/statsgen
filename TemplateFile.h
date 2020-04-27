#ifndef __TEMPLATEFILE
#define __TEMPLATEFILE

// wxWindows includes
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/textfile.h>
#include <wx/arrstr.h>

// Statsgen Includes
#include "TemplateVariable.h"
#include "DynamicArrays.h"

enum
{
	TEMPLATE_COMMAND_VARIABLE,
	TEMPLATE_COMMAND_IF,
	TEMPLATE_COMMAND_ELSE,
	TEMPLATE_COMMAND_ENDIF,
	TEMPLATE_COMMAND_LOOP,
	TEMPLATE_COMMAND_ENDLOOP,
	TEMPLATE_COMMAND_OPENFILE,
	TEMPLATE_COMMAND_CLOSEFILE,
	TEMPLATE_COMMAND_APPENDFILE,
	TEMPLATE_COMMAND_PROCEDURE,
	TEMPLATE_COMMAND_RETURN
};
enum
{
	TEMPLATE_FINISHED_ERROR,
	TEMPLATE_FINISHED_OK,
	TEMPLATE_FINISHED_EOF,
	TEMPLATE_FINISHED_ELSE,
	TEMPLATE_FINISHED_ENDIF,
	TEMPLATE_FINISHED_ENDLOOP,
	TEMPLATE_FINISHED_RETURN
};


class TemplateFile;
class CompiledTemplateLine
{
	public:
		CompiledTemplateLine();
		virtual ~CompiledTemplateLine();
		bool SetTemplateCode(wxString &templateCodeIn);
		virtual bool Process(TemplateFile &templateFile,TemplateVariable &variableList);

	protected:
		virtual bool ParseTemplateCode();
		void SplitParameters();
		bool UsesSubstitution();

	protected:
		wxString	templateCode;
		wxString	templateCodeParameters;
		bool		usesSubstitution;
	
};

WX_DECLARE_OBJARRAY(CompiledTemplateLine,ArrayOfCompiledTemplateLine);
class CompiledTemplateSection
{
	public:
		CompiledTemplateSection();
		virtual ~CompiledTemplateSection();
		void Add(CompiledTemplateLine *compiledTemplateLine);
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);

	private:
		ArrayOfPointers	compiledTemplateLines;
	
};

class CompiledTemplateLineRaw : public CompiledTemplateLine
{
	public:
		void SetRaw(wxString &rawTextIn);
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);
		virtual ~CompiledTemplateLineRaw();

	protected:
		virtual bool ParseTemplateCode();

	private:
		wxString rawText;
};

class CompiledTemplateLineIF : public CompiledTemplateLine
{
	public:
		CompiledTemplateSection	trueSection;
		CompiledTemplateSection	falseSection;
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);
		virtual ~CompiledTemplateLineIF();
	
	protected:
		virtual bool ParseTemplateCode();
};

class CompiledTemplateLinePROCEDURE : public CompiledTemplateLine
{
	public:
		CompiledTemplateSection	procedureBody;
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);
		virtual ~CompiledTemplateLinePROCEDURE();
		void Execute(TemplateFile *templateFile,TemplateVariable &variableList,TemplateValueList *parameterValues);

		wxString	procedureName;
		wxArrayString	parameterVars;
	
	protected:
		virtual bool ParseTemplateCode();
};

class CompiledTemplateLineLOOP : public CompiledTemplateLine
{
	public:
		CompiledTemplateSection	subSection;
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);
		virtual ~CompiledTemplateLineLOOP();
	protected:
		virtual bool ParseTemplateCode();
	private:
		bool SplitLoopParameters(wxString &parameterString);
	private:
		wxString	rowVariableName;
		wxString	indexVariableName;
		wxString	sql;
};

class CompiledTemplateLineOPENFILE : public CompiledTemplateLine
{
	public:
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);
		virtual ~CompiledTemplateLineOPENFILE();
	protected:
		virtual bool ParseTemplateCode();
		virtual char *Mode();
};

class CompiledTemplateLineCLOSEFILE : public CompiledTemplateLine
{
	public:
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);
		virtual ~CompiledTemplateLineCLOSEFILE();
	protected:
		virtual bool ParseTemplateCode();
};

class CompiledTemplateLineAPPENDFILE : public CompiledTemplateLineOPENFILE
{
	public:
		CompiledTemplateLineAPPENDFILE();
		virtual ~CompiledTemplateLineAPPENDFILE();
	protected:
		virtual char *Mode();
		virtual bool ParseTemplateCode();
};

class CompiledTemplateLineVARIABLE : public CompiledTemplateLine
{
	public:
		virtual bool Process(TemplateFile &templateFile,
					TemplateVariable &variableList);
		virtual ~CompiledTemplateLineVARIABLE();
	protected:
		virtual bool ParseTemplateCode();
};

class TemplateFile
{
	public:
		TemplateFile();
		virtual ~TemplateFile();
		bool Compile(CompiledTemplateSection &compiledSection,
					int *currentLineIndex,
					wxString &currentTemplateLine,
					int *finishedType,
					TemplateVariable &templateVariable);
		bool Process();
		void Debug();
		bool ReadTemplate(wxString &filename);
		void GetCreatedFiles(wxArrayString &filelist);
		void ExportToCPP(wxString &filename,wxString &prefix);
		int IdentifyTemplateCode(wxString &templateCode);
		void AddCreatedFile(wxString &filename);
		bool WriteTemplateDataToOutput(wxString &text,
									TemplateVariable &templateVariable);
		bool CloseFile(TemplateVariable &templateVariable);
		void AddCustomProcedure(CompiledTemplateLinePROCEDURE *procedure);
		CompiledTemplateLinePROCEDURE *FindCustomProcedure(wxString &name);
	private:
		void Unload();
		bool SplitTemplateLine(wxString &templateLine,
					wxString *beforeTemplateCode,
					wxString *templateCode,
					wxString *afterTemplateCode);
		bool ReadTemplateFile(wxString &filename,wxString parent);
		bool ProcessTemplate(bool allowProcessing,
					TemplateVariable &templateVariable,
					int *lineIndex,
					wxString &templateLine,
					bool *endedOnElse);
		bool OpenFile(wxString &templateCode,
					char *mode,
					TemplateVariable &templateVariable);
		bool Loop(TemplateVariable &templateVariable,
					wxString &templateCodeText,
					wxString &rowVariable,
					wxString &rowIndexVariable,
					wxString &SQLQuery);
		void LoadTemplateFile(wxString &filename);
		void AddTemplateLine(wxString &line,wxString &lineRef);
		wxString TemplateErrorDetails(int lineIndex);
		wxArrayPtrVoid customProcedures;

	private:
		wxArrayString			templateLines;
		wxArrayString			templateLineRef;
		wxArrayString			createdFiles;
		wxString				currentTemplateFile;
		int						currentTemplateLine;
		wxString				originalTemplateFile;

		ArrayOfPointers			usedTemplateFiles;
		wxArrayString			usedTemplateFilenames;
		
};


#endif
