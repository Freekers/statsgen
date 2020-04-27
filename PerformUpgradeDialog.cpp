// Statsgen Includes
#include <wx/wx.h>
#include <wx/dir.h>
#include "PerformUpgradeDialog.h"
#include "GlobalStatistics.h"
#include "StaticFunctions.h"
#include "Progress.h"

BEGIN_EVENT_TABLE(PerformUpgradeDialog, wxDialog)
		EVT_SIZE(PerformUpgradeDialog::OnResize)
		EVT_BUTTON(WINDOW_ID_BUTTON_SAVE,PerformUpgradeDialog::OnSave)
		EVT_BUTTON(WINDOW_ID_BUTTON_QUIT,PerformUpgradeDialog::OnQuit)
END_EVENT_TABLE()

PerformUpgradeDialog::PerformUpgradeDialog(wxWindow *parent, 
				wxWindowID id,
				const wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name):
			wxDialog(parent,
				id,
				title,
				pos,
				size,
				style,
				name)

{
	upgradeList=NULL;
}

void PerformUpgradeDialog::OnQuit(wxCommandEvent& event)
{
	EndModal(event.GetId());
}

void PerformUpgradeDialog::OnSave(wxCommandEvent& event)
{
	long		selectedItem;
	int			upgradeIndex;
	wxString	upgradeFilename;
	wxString	description;
	wxString	upgradeType;
	wxString	message;
	wxString	caption="Are You Sure?";

	selectedItem=-1;
	selectedItem=upgradeList->GetNextItem(selectedItem,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if (selectedItem!=-1)
	{
		ConfigData	upgradeFile;
		upgradeIndex=(int)upgradeList->GetItemData(selectedItem);
		upgradeFilename=upgradeFiles.Item(upgradeIndex);
		wxFileName	upgradeFileName(upgradeFilename);
		upgradeFile.Initiate(upgradeFileName);
		upgradeFile.UpgradeDetails(description,upgradeType);
		if (upgradeType.CmpNoCase(UPGRADE_TYPE_UNKNOWN)!=0)
		{
			message.Printf("Are you sure you want to upgrade the %s using the upgrade file [%s], [%s]",
					upgradeType.GetData(),
					upgradeFilename.GetData(),
					description.GetData());
			if (wxMessageBox(message,caption,wxYES_NO)==wxYES)
			{
				if (upgradeType.CmpNoCase(UPGRADE_TYPE_CONFIG)==0)
				{
					UpgradeConfigFile(upgradeFilename);
				}
				if (upgradeType.CmpNoCase(UPGRADE_TYPE_DATABASE)==0)
				{
					TemplateFile	templateFile;
					templateFile.ReadTemplate(upgradeFilename);
					templateFile.Process();
				}
				progress->Finalise();
			}
		}
	}
}

PerformUpgradeDialog::~PerformUpgradeDialog()
{
	progress->SetDynamicPanel(NULL);
}

void PerformUpgradeDialog::CreateDialog()
{
	wxString	label="Generic Configuration";
	wxString	defaultValue="";
	wxString	configKey;
	wxSizeEvent	event;

	wxPoint		configItemsPosition=wxDefaultPosition;
	wxSize		configItemsSize=wxDefaultSize;
	STATSGEN_DEBUG_FUNCTION_START("PerformUpgradeDialog","CreateDialog")


	saveButton.Create(this,
					WINDOW_ID_BUTTON_SAVE,
					_T("Peform Upgrade"),
					wxDefaultPosition);
	quitButton.Create(this,
					WINDOW_ID_BUTTON_QUIT,
					_T(WINDOW_ID_BUTTON_QUIT_TEXT),
					wxDefaultPosition);


	upgradeList=new wxListCtrl(this,
				WINDOW_ID_LISTBOX_CONFIGITEMS,
				wxDefaultPosition,
				wxDefaultSize,
				wxLC_REPORT);
	progressPanel=new ProgressPanel();
	progressPanel->DynamicSizing();
	progressPanel->DisableTimeToGo();
	progressPanel->DisableCounter();
	progressPanel->Create(this,
						WINDOW_ID_PROGRESS_PANEL_DYNAMIC,
						wxDefaultPosition,
						wxDefaultSize,
						wxRAISED_BORDER);
	progress->SetDynamicPanel(progressPanel);
	progress->ChoosePanel(WINDOW_ID_PROGRESS_PANEL_DYNAMIC);
	progress->UpdateLabel("Status");
	STATSGEN_DEBUG(DEBUG_RARELY,"items created")
	// populate the list with upgrade files
	wxFileName	baseDirectory;
	wxString	fileSpec="*.ini";
	int			flags=wxDIR_FILES;
	int			fileCount;
	int			fileIndex;
	wxString	filename;
	wxString	description;
	wxString	upgradeType;
	int			rowNumber=0;
	wxListItem	listColumn;
	long		listIndex;
	bool		rowAdded;

	baseDirectory=BaseDirectoryFileName("statsgen2.ini");
	wxDir::GetAllFiles(
			baseDirectory.GetPath(),
			&upgradeFiles,
			fileSpec,
			flags);
	STATSGEN_DEBUG(DEBUG_RARELY,"got file list")
	listColumn.SetText("Type");
	upgradeList->InsertColumn(0,listColumn);
	listColumn.SetText("Description");
	upgradeList->InsertColumn(1,listColumn);
	listColumn.SetText("Filename");
	upgradeList->InsertColumn(2,listColumn);

	STATSGEN_DEBUG(DEBUG_RARELY,"created columns")
	fileCount=upgradeFiles.GetCount();
	rowNumber=0;
	for (fileIndex=0;fileIndex<fileCount;fileIndex++)
	{
		filename=upgradeFiles.Item(fileIndex);
		ConfigData		upgradeConfigFile;
		wxFileName		upgradeFileName(filename);

		upgradeConfigFile.Initiate(upgradeFileName);
		upgradeConfigFile.UpgradeDetails(description,upgradeType);
		rowAdded=true;
		rowAdded=(upgradeType.CmpNoCase(UPGRADE_TYPE_UNKNOWN)!=0);
		if (rowAdded)
		{
			listIndex=upgradeList->InsertItem(rowNumber,upgradeType);
			upgradeList->SetItemData(listIndex,fileIndex);
			upgradeList->SetItem(rowNumber,1,description);
			upgradeList->SetItem(rowNumber,2,upgradeFileName.GetFullName());
			rowNumber++;
		}
	}
	upgradeList->SetColumnWidth(0,wxLIST_AUTOSIZE);
	upgradeList->SetColumnWidth(1,wxLIST_AUTOSIZE);
	upgradeList->SetColumnWidth(2,wxLIST_AUTOSIZE);
	STATSGEN_DEBUG(DEBUG_RARELY,"populated columns")
	OnResize(event);
	STATSGEN_DEBUG_FUNCTION_END
}

bool PerformUpgradeDialog::DisplayDialog()
{
	int			dialogRetVal;
	bool			retVal;

	// Called when we want to pop the dialog box
	// into existance for the first time

	// First we want to create all the items in the dialog box
	CreateDialog();

	// Now we can resize every item in the dialog to fit nicely

	// Then we pop it into existance
	dialogRetVal=ShowModal();

	// Now we do what is necessary dependent on the return code
	switch (dialogRetVal)
	{
		case WINDOW_ID_BUTTON_SAVE:
			retVal=true;
			break;
		case WINDOW_ID_BUTTON_QUIT:
		default:
			retVal=false;
			break;
	}

	return (retVal);
}

void PerformUpgradeDialog::OnResize(wxSizeEvent &event)
{
	wxString	msg;

	int		dialogWidth;
	int		dialogHeight;
	int		quitWidth;
	int		quitHeight;
	int		saveWidth;
	int		saveHeight;
	int		panelWidth;
	int		panelHeight;
	wxSize	itemSize;
	wxPoint	itemPosition;
	int		yPosition;
	int		progressWidth;
	int		progressHeight;

	STATSGEN_DEBUG_FUNCTION_START("PerformUpgradeDialog","OnResize")

	itemSize=GetSize();
	dialogWidth=itemSize.GetWidth();
	dialogHeight=itemSize.GetHeight();

	// Quit and Save buttons are at the bottom of the screen
	itemSize=quitButton.GetSize();
	quitWidth=itemSize.GetWidth();
	quitHeight=itemSize.GetHeight();

	itemSize=saveButton.GetSize();
	saveWidth=itemSize.GetWidth();
	saveHeight=itemSize.GetHeight();

	itemSize=progressPanel->GetSize();
	progressHeight=itemSize.GetHeight();
	progressWidth=itemSize.GetWidth();
	panelHeight=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE-progressHeight;
	panelWidth=dialogWidth;
	if (progressWidth>panelWidth)
	{
		panelWidth=progressWidth;
	}
	else
	{
		progressWidth=panelWidth;
	}

	// upgradeList
	yPosition=0;
	upgradeList->SetSize(0,yPosition,panelWidth,panelHeight);

	// progress panel
	yPosition=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE-progressHeight;
	progressPanel->SetSize(0,yPosition,progressWidth,progressHeight);

	// Save button
	itemPosition.x=BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-saveHeight-DIALOG_BOTTOM_BORDER_SIZE;
	saveButton.SetPosition(itemPosition);

	// Quit button
	itemPosition.x=saveWidth+BUTTON_WIDTH_GAP+BUTTON_WIDTH_GAP;
	itemPosition.y=dialogHeight-quitHeight-DIALOG_BOTTOM_BORDER_SIZE;
	quitButton.SetPosition(itemPosition);

	STATSGEN_DEBUG_FUNCTION_END
}

