#ifndef __TEMPLATEVARIABLE
#define __TEMPLATEVARIABLE

// wxWindows includes
#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/dynarray.h>

// Statsgen Includes
#include "TemplateOpenFile.h"
#include "TemplateOpenQuery.h"
#include "TemplateVariableField.h"

#define TRUE_STRING	"TRUE"
#define FALSE_STRING	"FALSE"

class TemplateFile;
class TemplateValue
{
	public:
		TemplateValue();
		~TemplateValue();
		bool IsQuery();
		void SetQuery(TemplateOpenQuery *queryIn);
		void SetString(wxString &valueIn);
		TemplateOpenQuery *GetQuery();
		wxString GetString();
	private:
		void Unload();
		wxString			value;
		TemplateOpenQuery	*query;
};

class TemplateValueList
{
	public:
		TemplateValueList();
		~TemplateValueList();
		void Add(TemplateValue *value);
		int GetCount();
		TemplateValue *Item(int index);
	private:
		void Unload();
		wxArrayPtrVoid		values;
};

class TemplateVariable
{
	public:
		TemplateVariable();
		virtual ~TemplateVariable();
		bool StoreVariable(wxString &variable,wxString &value);
		bool StoreQuery(wxString &variable,wxString &SQL);
		bool StoreOpenQuery(wxString &variable,TemplateOpenQuery *queryIn);
		bool QueryNextRow(wxString &variable);
		bool RemoveQuery(wxString &variable);
		wxString GetVariableValue(wxString &templateCode);
		wxString SubstituteVariableValues(wxString &text);
		static wxString ColouredText2HTML(wxString &colouredText);
		wxString FormatDuration(wxString &format, wxString &duration);
		wxString FormatDate(wxString &format, wxString &secondssince1970);
		wxString FormatInteger(wxString &format, wxString &integerStr);
		wxString FormatFloat(wxString &format, wxString &floatStr);
		wxString EvaluateCondition(wxString &condition);
		static wxString Hash(wxString &text);
		wxString ReadStatistic(wxString &variable);
		wxString MessageCentre(wxArrayString &parameters);
		wxString Case(wxArrayString &parameters);
		wxString UpdateStatus(wxArrayString &parameters);
		wxString SimpleSQL(wxArrayString &parameters);
		wxString ReadTemplateConfig(wxArrayString &parameters);
		wxString ReadConfig(wxArrayString &parameters);
		wxString WriteConfig(wxArrayString &parameters);
		void SetTemplateFile(TemplateFile *templateFileIn);
		void ExecuteCustomProcedure(wxString &procedureName,TemplateValueList *parameterValues);

	public:
		ArrayOfTemplateOpenFile			openFiles;
		ArrayOfTemplateOpenQuery		openQueries;
		ArrayOfTemplateVariableField	variableFields;

	private:
		TemplateOpenQuery *RetrieveQuery(wxString &variable);
		wxString RetrieveVariableValue(wxString &variable);
		TemplateFile	*templateFile;

		wxString	currentProcedurePrefix;
		
};


#endif
