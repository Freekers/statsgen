// Statsgen Includes
#include "WeaponGroupDialog.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(WeaponGroupDialog, wxDialog)
	EVT_BUTTON(WINDOW_ID_BUTTON_SAVE,WeaponGroupDialog::OnSave)
	EVT_BUTTON(WINDOW_ID_BUTTON_QUIT,WeaponGroupDialog::OnQuit)
	EVT_BUTTON(WINDOW_ID_BUTTON_NEW,WeaponGroupDialog::OnNew)
	EVT_BUTTON(WINDOW_ID_BUTTON_DELETE,WeaponGroupDialog::OnDelete)
	EVT_CHECKLISTBOX(WINDOW_ID_SELECTION_CONFIGVALUE,WeaponGroupDialog::OnWeaponSelected)
	EVT_LISTBOX(WINDOW_ID_LISTBOX_CONFIGITEMS,WeaponGroupDialog::OnWeaponGroupSelected)
END_EVENT_TABLE()

WeaponGroupDialog::WeaponGroupDialog(
				wxWindow *parent, 
				wxWindowID id,
				wxString &title,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name) :
			wxDialog(parent,
				id,
				title,
				pos,
				size,
				style,
				name)
{
	currentWeaponGroup="";
	CreateScreen();
}

void WeaponGroupDialog::CreateScreen()
{
	newButton		=new wxButton(this,
						WINDOW_ID_BUTTON_NEW,
						_T(WINDOW_ID_BUTTON_NEW_TEXT));

	deleteButton	=new wxButton(this,
						WINDOW_ID_BUTTON_DELETE,
						_T(WINDOW_ID_BUTTON_DELETE_TEXT));

	saveButton		=new wxButton(this,
						WINDOW_ID_BUTTON_SAVE,
						_T(WINDOW_ID_BUTTON_SAVE_TEXT));

	quitButton		=new wxButton(this,
						WINDOW_ID_BUTTON_QUIT,
						_T(WINDOW_ID_BUTTON_QUIT_TEXT));

	weaponGroupsList=new wxListBox(this,WINDOW_ID_LISTBOX_CONFIGITEMS);
	weaponKeysList=new wxCheckListBox(this,WINDOW_ID_SELECTION_CONFIGVALUE);

	buttonSizer=new wxBoxSizer(wxHORIZONTAL);
	displaySizer=new wxBoxSizer(wxHORIZONTAL);
	mainSizer=new wxBoxSizer(wxVERTICAL);

	buttonSizer->Add(newButton);
	buttonSizer->Add(deleteButton);
	buttonSizer->Add(saveButton);
	buttonSizer->Add(quitButton);

	displaySizer->Add(weaponGroupsList,1,wxEXPAND);
	displaySizer->Add(weaponKeysList,1,wxEXPAND);

	displaySizer->SetMinSize(wxSize(640,480));
	mainSizer->Add(displaySizer,1,wxEXPAND);
	mainSizer->Add(buttonSizer);

	SetSizer(mainSizer);
	mainSizer->SetSizeHints(this);

	PopulateGroupsList();
	PopulateKeysList();
}

WeaponGroupDialog::~WeaponGroupDialog()
{
	STATSGEN_DEBUG_FUNCTION_START("WeaponGroupDialog","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void WeaponGroupDialog::PopulateGroupsList()
{
	STATSGEN_DEBUG_FUNCTION_START("WeaponGroupDialog","PopulateGroupsList")
	wxArrayString	weaponGroups;

	weaponGroups = globalStatistics.configData.ReadWeaponGroups();

	weaponGroupsList->Clear();
	weaponGroupsList->Append(weaponGroups);
	
	STATSGEN_DEBUG_FUNCTION_END
}

void WeaponGroupDialog::CheckKeysList()
{
	STATSGEN_DEBUG_FUNCTION_START("WeaponGroupDialog","CheckKeysList")
	wxArrayString	currentWeapons;
	wxString		weaponKey;
	wxArrayString	weaponGroupKeys;
	int				weaponGroupIndex;
	int				weaponKeyIndex;
	int				weaponKeyCount;

	if (currentWeaponGroup.Length()>0)
	{
		weaponGroupKeys=globalStatistics.configData.ReadWeaponGroup(currentWeaponGroup);
		weaponKeyIndex=weaponKeysList->GetCount();
		for (weaponKeyIndex=0;weaponKeyIndex<weaponKeyCount;weaponKeyIndex++)
		{
			weaponKey=weaponKeysList->GetString(weaponKeyIndex);
			weaponGroupIndex=weaponGroupKeys.Index(weaponKey);
			weaponKeysList->Check(weaponKeyIndex, 
									(weaponGroupIndex!=wxNOT_FOUND));
		}
	}
	
	STATSGEN_DEBUG_FUNCTION_END
}

void WeaponGroupDialog::PopulateKeysList()
{
	STATSGEN_DEBUG_FUNCTION_START("WeaponGroupDialog","PopulateKeysList")
	wxArrayString	keys;
	wxArrayString	values;
	wxString		group;

	group="LogEntriesWeapon";

	weaponKeysList->Clear();
	keys=globalStatistics.configData.ReadWeaponGroups();
	weaponKeysList->Append(keys);
	globalStatistics.configData.ReadGroup(group,keys,values);
	weaponKeysList->Append(keys);
	STATSGEN_DEBUG_FUNCTION_END
}

void WeaponGroupDialog::WriteCurrentWeaponGroup()
{
	wxArrayString	weaponKeys;
	int				weaponKeyCount;
	wxString		weaponKey;
	int				weaponKeyIndex;
	wxString		configKey;

	if (currentWeaponGroup.Length()>0)
	{
		weaponKeys=weaponKeysList->GetStrings();
		weaponKeyCount=weaponKeys.GetCount();
		globalStatistics.configData.DeleteGroup(currentWeaponGroup);
		globalStatistics.configData.CommitChanges();
		for (weaponKeyIndex=0;weaponKeyIndex<weaponKeyCount;weaponKeyIndex++)
		{
			weaponKey=weaponKeys.Item(weaponKeyIndex);
			if 	(weaponKeysList->IsChecked(weaponKeyIndex))
			{
				configKey.Printf("/%s/%s",
						currentWeaponGroup.GetData(),
						weaponKey.GetData());
				globalStatistics.configData.WriteTextValue(configKey,"");
			}
		}
		globalStatistics.configData.CommitChanges();
	}
}

void WeaponGroupDialog::OnWeaponSelected(wxCommandEvent &event)
{
}

void WeaponGroupDialog::OnWeaponGroupSelected(wxCommandEvent &event)
{
	WriteCurrentWeaponGroup();
	currentWeaponGroup=weaponGroupsList->GetStringSelection();
	CheckKeysList();
}

void WeaponGroupDialog::OnSave(wxCommandEvent &event)
{
	WriteCurrentWeaponGroup();
	globalStatistics.configData.CommitChanges();
	EndModal(true);
}

void WeaponGroupDialog::OnQuit(wxCommandEvent &event)
{
	globalStatistics.configData.RollbackChanges();
	EndModal(false);
}

void WeaponGroupDialog::OnNew(wxCommandEvent &event)
{
	wxString	message="New Weapon Group Identifier";
	wxString	caption="Weapon Group";
	wxString	weaponGroup;
	wxString	configKey;
	wxString	configValue;

	weaponGroup=wxGetTextFromUser(message,caption);

	if (weaponGroup.Length()>0)
	{
		WriteCurrentWeaponGroup();
		weaponGroup=weaponGroup.Lower();
		if (!globalStatistics.configData.IsWeaponGroupKey(weaponGroup))
		{
			weaponGroup=CONFIG_WEAPON_GROUP_PREFIX+weaponGroup;
		}
		currentWeaponGroup=weaponGroup;
		CheckKeysList();
		weaponGroupsList->Append(currentWeaponGroup);
		weaponGroupsList->SetStringSelection(currentWeaponGroup);
	}
}

void WeaponGroupDialog::OnDelete(wxCommandEvent &event)
{
	if (currentWeaponGroup.Length()>0)
	{
		globalStatistics.configData.DeleteGroup(currentWeaponGroup);
		globalStatistics.configData.CommitChanges();
		PopulateGroupsList();
		PopulateKeysList();
	}
}

void WeaponGroupDialog::DisplayDialog()
{
	bool	result;

	result=ShowModal();
}
