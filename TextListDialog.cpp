// Statsgen Includes
#include "TextListDialog.h"
#include "GenericOKCancelDialog.h"
#include "GlobalStatistics.h"
#include "LogFileReader.h"

BEGIN_EVENT_TABLE(TextListDialog,GenericOKCancelDialog)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,TextListDialog::OnNew)
END_EVENT_TABLE()

TextListDialog::TextListDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				const char *groupIn) : GenericOKCancelDialog
											(parent, 
											id,
											title,
											pos,
											size,
											style,
											name)
{
	wxArrayString	textItems;

	mGroup			= groupIn;
	mSingleWords	= true;

	mNewButton.Create(this,
					WINDOW_ID_BUTTON_NEW,
					_T(WINDOW_ID_BUTTON_NEW_TEXT),
					wxDefaultPosition);
	globalStatistics.configData.ReadList(mGroup,textItems);
	mPanel=new TextListEditorPanel();
	mPanel->SetList(textItems);
	mPanel->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
}

bool TextListDialog::DisplayDialog()
{
	return (GenericOKCancelDialog::DisplayDialog((wxPanel *)mPanel));
}

TextListDialog::~TextListDialog()
{
}

void TextListDialog::OnSave(wxCommandEvent &event)
{
	wxString			msg;
	wxArrayString	textItems;

	mPanel->GetList(textItems);
	// Write the TextList List to disk

	globalStatistics.configData.WriteList(mGroup,textItems);
	globalStatistics.configData.CommitChanges();
	// Do any standard Save
	GenericOKCancelDialog::OnSave(event);
}

void TextListDialog::OnQuit(wxCommandEvent &event)
{
	wxString			msg;

	// Do any standard Save
	GenericOKCancelDialog::OnQuit(event);
}

void TextListDialog::OnNew(wxCommandEvent &event)
{
	wxString	message="Enter New";
	wxString	caption="Enter New";
	wxString	newEntry;
	wxString	userEntry;
	int			tokenCount;
	int			tokenIndex;

	userEntry=wxGetTextFromUser(message,caption);
	if (userEntry.Length()>0)
	{
		if (mSingleWords)
		{
			GetLineToken(userEntry,(char *)" ",1,&tokenCount);
			for (tokenIndex=1;tokenIndex<=tokenCount;tokenIndex++)
			{
				newEntry=GetLineToken(userEntry,(char *)" ",tokenIndex,&tokenCount);
				mPanel->Add(newEntry);
			}
		}
		else
		{
			mPanel->Add(userEntry);
		}
	}
}
void TextListDialog::ControlsSizerPre()
{
	mControlsSizer->Add(&mNewButton);
}
void TextListDialog::ControlsSizerPost()
{
}
void TextListDialog::SortOn(bool value)
{
	mPanel->SortOn(value);
}
void TextListDialog::SingleWords(bool value)
{
	mSingleWords=value;
}
