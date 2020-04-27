// Statsgen Includes
#include "AwardEditorPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(AwardEditorPanel, wxPanel)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_AWARDCHOICESPANEL,AwardEditorPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_AWARDDEFINITIONPANEL,AwardEditorPanel::OnRightClick)
		EVT_TEXT(WINDOW_ID_TEXTCTRL_CONFIGVALUE,AwardEditorPanel::OnConfigChanged)
		EVT_MENU(AWARD_DEFINITION_DELETE_COMPONENT,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_POSITIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_NEGATIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_KILL_POSITIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_KILL_NEGATIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_DEATH_POSITIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_DEATH_NEGATIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_TK_POSITIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_TK_NEGATIVE,AwardEditorPanel::OnPopupMenu)
		EVT_MENU(AWARD_CHOICES_ADD_KILL_AND_DEATH,AwardEditorPanel::OnPopupMenu)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(AwardChoicesPanel, wxPanel)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_WEAPONLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_ACTIONLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_LOCATIONLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_MISCAWARDLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_XPAWARDLIST,AwardChoicesPanel::OnRightClick)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(AwardDefinitionPanel, wxPanel)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_WEAPONLIST,AwardDefinitionPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_ACTIONLIST,AwardDefinitionPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_LOCATIONLIST,AwardDefinitionPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_MISCAWARDLIST,AwardDefinitionPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_XPAWARDLIST,AwardDefinitionPanel::OnRightClick)
END_EVENT_TABLE()

AwardEditorPanel::AwardEditorPanel(
				wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxString &awardIDIn):
			wxPanel(parent,
				id,
				pos,
				size,
				style,
				name)

{
	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","Constructor")
	wxString		configGroup;
	wxString		configKey;
	wxSizeEvent		dummyEvent;
	wxString		serverTypeConfigKey="/tmp/awardeditorservertype";
	int				serverTypeIndex;
	wxString		serverTypeCode;
	wxString		serverTypeName;
	wxArrayString	serverTypeCodes;
	wxArrayString	serverTypeNames;

	mAwardID=awardIDIn;
	serverTypeCode="";
	serverTypeName="Any";
	serverTypeCodes.Add(serverTypeCode);
	serverTypeNames.Add(serverTypeName);
	for (serverTypeIndex=0;serverTypeIndex<SERVER_TYPE_COUNT;serverTypeIndex++)
	{
		serverTypeCode=ServerTypeCodes[serverTypeIndex];
		serverTypeName=ServerTypeNames[serverTypeIndex];
		serverTypeCodes.Add(serverTypeCode);
		serverTypeNames.Add(serverTypeName);
	}
	configGroup="AWARDDEFINITION"+mAwardID;
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"overallconfig");
	mOverallConfig=new GroupedConfigItemsPanel((char *)"Overall Details");
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"overallconfig Create");
	mOverallConfig->CreateDisplay(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"overallconfig AddSelection");
	mOverallConfig->AddSelection((char *)"Server Type Filter",serverTypeConfigKey,(char *)"",serverTypeCodes,serverTypeNames);
	configKey.Printf("/%s/Name",STRING_TO_CHAR(configGroup));
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"overallconfig Add");
	mOverallConfig->Add((char *)"Name", configKey,(char *)"",-1,NULL);
	configKey.Printf("/%s/Image",STRING_TO_CHAR(configGroup));
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"overallconfig Add");
	mOverallConfig->Add((char *)"Image URL", configKey,(char *)"",-1,NULL);
	configKey.Printf("/%s/Weighted",STRING_TO_CHAR(configGroup));
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"overallconfig AddBoolean");
	mOverallConfig->AddBoolean((char *)"Weighted", configKey,(char *)"N",NULL);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Splitter Window");
	mSplitter=new wxSplitterWindow(this, wxID_ANY, 
								wxDefaultPosition,
								wxDefaultSize,
								wxSP_3D);
								
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AwardChoicesPanel");
	mAwardChoices=new AwardChoicesPanel(
						mSplitter,
						WINDOW_ID_AWARDCHOICESPANEL,
						wxDefaultPosition,
						wxDefaultSize,
						0,
						_T(""),
						mAwardID);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AwardChoicesPanel Enable");
	mAwardChoices->Enable();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AwardDefinitionPanel");
	mAwardDefinition=new AwardDefinitionPanel(
						mSplitter,
						WINDOW_ID_AWARDDEFINITIONPANEL,
						wxDefaultPosition,
						wxDefaultSize,
						0,
						_T(""),
						mAwardID);

	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"AwardDefinitionPanel Enable");
	mAwardDefinition->Enable();
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Split Horizontally");
	mSplitter->SplitHorizontally(mAwardChoices,mAwardDefinition);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Sash Gravity");
	mSplitter->SetSashGravity(0.5);

	mMainSizer = new wxBoxSizer(wxVERTICAL);
	mMainSizer->Add(mOverallConfig,0,wxEXPAND);
	mMainSizer->Add(mSplitter,1,wxEXPAND);

	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);
	STATSGEN_DEBUG_FUNCTION_END
}

AwardEditorPanel::~AwardEditorPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardEditorPanel::OnConfigChanged(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","OnConfigChanged")
	wxString			serverTypeConfigKey="/tmp/awardeditorservertype";
	static wxString		lastConfigValue="";
	wxString			serverTypeConfigValue;

	globalStatistics.configData.ReadTextValue(serverTypeConfigKey,&serverTypeConfigValue);

	if (serverTypeConfigValue.Cmp(lastConfigValue)!=0)
	{
		lastConfigValue=serverTypeConfigValue;
		if ((mAwardChoices!=NULL)&&(mAwardDefinition!=NULL))
		{
			mAwardChoices->SetFilter(serverTypeConfigValue);
			mAwardDefinition->SetFilter(serverTypeConfigValue);
		}
	}

	STATSGEN_DEBUG_FUNCTION_END
}

void AwardEditorPanel::OnPopupMenu(wxCommandEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","OnPopupMenu")
	long			selectedTree;
	int				selectedCodesCount;
	int				selectedCodesIndex;
	wxString		selectedCode;
	int				menuSelection=event.GetId();
	wxArrayString	selectedCodes;
	bool			killList;
	bool			teamkillList;

	if (menuSelection==AWARD_DEFINITION_DELETE_COMPONENT)
	{
		// Award Definition 
		selectedTree	= mAwardDefinition->mSelectedDefinitionTree;
		selectedCodes	= mAwardDefinition->mSelectedCodes;
	}
	else
	{
		// Award Choice
		selectedTree	= mAwardChoices->mSelectedChoiceTree;
		selectedCodes	= mAwardChoices->mSelectedCodes;
	}
	selectedCodesCount=selectedCodes.GetCount();
	for (selectedCodesIndex=0;selectedCodesIndex<selectedCodesCount;selectedCodesIndex++)
	{
		selectedCode=selectedCodes.Item(selectedCodesIndex);
		switch (selectedTree)
		{
			case WINDOW_ID_WEAPONLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						mAwardDefinition->mAwardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						mAwardDefinition->RemoveWeaponComponent(selectedCode,killList,teamkillList);
						break;
					case AWARD_CHOICES_ADD_KILL_POSITIVE:
					case AWARD_CHOICES_ADD_KILL_NEGATIVE:
					case AWARD_CHOICES_ADD_DEATH_POSITIVE:
					case AWARD_CHOICES_ADD_DEATH_NEGATIVE:
					case AWARD_CHOICES_ADD_TK_POSITIVE:
					case AWARD_CHOICES_ADD_TK_NEGATIVE:
						mAwardDefinition->AddWeaponComponent(
											selectedCode,
											(menuSelection==AWARD_CHOICES_ADD_KILL_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_KILL_NEGATIVE),
											(menuSelection==AWARD_CHOICES_ADD_KILL_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_DEATH_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_TK_POSITIVE),
											(menuSelection==AWARD_CHOICES_ADD_TK_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_TK_NEGATIVE));
											
						break;
					case AWARD_CHOICES_ADD_KILL_AND_DEATH:
						mAwardDefinition->AddWeaponComponent(selectedCode,true,true,false);
						mAwardDefinition->AddWeaponComponent(selectedCode,false,false,false);
						break;
				}
				break;
			case WINDOW_ID_LOCATIONLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						mAwardDefinition->mAwardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						mAwardDefinition->RemoveLocationComponent(selectedCode,killList,teamkillList);
						break;
					case AWARD_CHOICES_ADD_KILL_POSITIVE:
					case AWARD_CHOICES_ADD_KILL_NEGATIVE:
					case AWARD_CHOICES_ADD_DEATH_POSITIVE:
					case AWARD_CHOICES_ADD_DEATH_NEGATIVE:
					case AWARD_CHOICES_ADD_TK_POSITIVE:
					case AWARD_CHOICES_ADD_TK_NEGATIVE:
						mAwardDefinition->AddLocationComponent(
											selectedCode,
											(menuSelection==AWARD_CHOICES_ADD_KILL_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_KILL_NEGATIVE),
											(menuSelection==AWARD_CHOICES_ADD_KILL_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_TK_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_DEATH_POSITIVE),
											(menuSelection==AWARD_CHOICES_ADD_TK_POSITIVE)||
											(menuSelection==AWARD_CHOICES_ADD_TK_NEGATIVE));
											
						break;
					case AWARD_CHOICES_ADD_KILL_AND_DEATH:
						mAwardDefinition->AddLocationComponent(selectedCode,true,true,false);
						mAwardDefinition->AddLocationComponent(selectedCode,false,false,false);
						break;
				}
				break;
			case WINDOW_ID_ACTIONLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						mAwardDefinition->mAwardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						mAwardDefinition->RemoveActionComponent(selectedCode);
						break;
					case AWARD_CHOICES_ADD_POSITIVE:
					case AWARD_CHOICES_ADD_NEGATIVE:
						mAwardDefinition->AddActionComponent(
											selectedCode,
											(menuSelection==AWARD_CHOICES_ADD_POSITIVE));
						break;
				}
				break;
			case WINDOW_ID_MISCAWARDLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						mAwardDefinition->mAwardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						mAwardDefinition->RemoveMiscComponent(selectedCode);
						break;
					case AWARD_CHOICES_ADD_POSITIVE:
					case AWARD_CHOICES_ADD_NEGATIVE:
						mAwardDefinition->AddMiscComponent(
											selectedCode,
											(menuSelection==AWARD_CHOICES_ADD_POSITIVE));
						break;
				}
				break;
			case WINDOW_ID_XPAWARDLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						mAwardDefinition->mAwardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						mAwardDefinition->RemoveXPComponent(selectedCode);
						break;
					case AWARD_CHOICES_ADD_POSITIVE:
					case AWARD_CHOICES_ADD_NEGATIVE:
						mAwardDefinition->AddXPComponent(
											selectedCode,
											(menuSelection==AWARD_CHOICES_ADD_POSITIVE));
						break;
				}
				break;
		}
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardEditorPanel::OnRightClick(wxTreeEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","OnRightClick")
	wxMenu	popupMenu(_T(""));
	long	panelId=event.GetId();
	long	selectedChoiceTree=mAwardChoices->mSelectedChoiceTree;

	if (panelId==WINDOW_ID_AWARDCHOICESPANEL)
	{
		switch (selectedChoiceTree)
		{
			case WINDOW_ID_WEAPONLIST:
			case WINDOW_ID_LOCATIONLIST:
				popupMenu.Append(AWARD_CHOICES_ADD_KILL_POSITIVE,_T("Add Positive Effect Kills"));
				popupMenu.Append(AWARD_CHOICES_ADD_KILL_NEGATIVE,_T("Add Negative Effect Kills"));
				popupMenu.Append(AWARD_CHOICES_ADD_DEATH_POSITIVE,_T("Add Positive Effect Deaths"));
				popupMenu.Append(AWARD_CHOICES_ADD_DEATH_NEGATIVE,_T("Add Negative Effect Deaths"));
				popupMenu.Append(AWARD_CHOICES_ADD_TK_POSITIVE,_T("Add Positive Effect Team Kills"));
				popupMenu.Append(AWARD_CHOICES_ADD_TK_NEGATIVE,_T("Add Negative Effect Team Kills"));
				popupMenu.Append(AWARD_CHOICES_ADD_KILL_AND_DEATH,_T("Add Kill And Death Combination"));
				break;
			case WINDOW_ID_ACTIONLIST:
				popupMenu.Append(AWARD_CHOICES_ADD_POSITIVE,_T("Add Objective Positive Effect"));
				popupMenu.Append(AWARD_CHOICES_ADD_NEGATIVE,_T("Add Objective Negative Effect"));
				break;
			case WINDOW_ID_MISCAWARDLIST:
				popupMenu.Append(AWARD_CHOICES_ADD_POSITIVE,_T("Add Miscellaneous Positive Effect"));
				popupMenu.Append(AWARD_CHOICES_ADD_NEGATIVE,_T("Add Miscellaneous Negative Effect"));
				break;
			case WINDOW_ID_XPAWARDLIST:
				popupMenu.Append(AWARD_CHOICES_ADD_POSITIVE,_T("Add XP Positive Effect"));
				popupMenu.Append(AWARD_CHOICES_ADD_NEGATIVE,_T("Add XP Negative Effect"));
				break;
		}
	}
	else
	{
		popupMenu.Append(AWARD_DEFINITION_DELETE_COMPONENT,_T("Delete Award Component(s)"));
	}

	popupMenu.Append(POPUP_CANCEL,_T("Cancel"));

	PopupMenu(&popupMenu);
	STATSGEN_DEBUG_FUNCTION_END
}

AwardChoicesPanel::AwardChoicesPanel(wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxString &awardIDIn):
			wxPanel(parent,
				id,
				pos,
				size,
				style,
				name)

{
	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","Constructor")
	wxTreeItemId	rootItem;
	long			listStyle;
	wxString		filterConfig="/tmp/awardeditorservertype";


	mAwardID=awardIDIn;
	globalStatistics.configData.ReadTextValue(filterConfig,&mFilter);

	//listStyle=wxTR_NO_BUTTONS | wxTR_HIDE_ROOT | wxTR_EXTENDED | wxTR_MULTIPLE;
	//listStyle=wxTR_NO_BUTTONS | wxTR_HIDE_ROOT | wxTR_MULTIPLE;
	listStyle=wxTR_HAS_BUTTONS |  wxTR_MULTIPLE;

	mWeaponList=new wxTreeCtrl(this, WINDOW_ID_WEAPONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mActionList=new wxTreeCtrl(this, WINDOW_ID_ACTIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mLocationList=new wxTreeCtrl(this, WINDOW_ID_LOCATIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mMiscList=new wxTreeCtrl(this, WINDOW_ID_MISCAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mXPList=new wxTreeCtrl(this, WINDOW_ID_XPAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mWeaponList,1,wxEXPAND);
	mMainSizer->Add(mLocationList,1,wxEXPAND);
	mMainSizer->Add(mActionList,1,wxEXPAND);
	mMainSizer->Add(mMiscList,1,wxEXPAND);
	mMainSizer->Add(mXPList,1,wxEXPAND);
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);

	mWeaponList->AddRoot(_T("Weapons"));
	mActionList->AddRoot(_T("Actions"));
	mXPList->AddRoot(_T("XP"));
	mLocationList->AddRoot(_T("Locations"));
	rootItem=mMiscList->AddRoot(_T("Misc"));

	mLabel=new wxStaticText(this,-1,_T("Choose From The Following Award Components"));
	mWeaponList->Hide();
	mLocationList->Hide();
	mActionList->Hide();
	mXPList->Hide();
	mMiscList->Hide();
	PopulateList(mWeaponList,"WEAPON","weapon");
	PopulateList(mActionList,"ACTION","action");
	PopulateList(mXPList,"XP","xp");
	PopulateList(mLocationList,"LOCATION","location");

	wxArrayString	miscAwardComponents;
	wxString	awardComponent;

	awardComponent="Speech";miscAwardComponents.Add(awardComponent);
	mMiscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Team Win";miscAwardComponents.Add(awardComponent);
	mMiscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Team Loss";miscAwardComponents.Add(awardComponent);
	mMiscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Suicides";miscAwardComponents.Add(awardComponent);
	mMiscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Kills";miscAwardComponents.Add(awardComponent);
	mMiscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Deaths";miscAwardComponents.Add(awardComponent);
	mMiscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Team Kills";miscAwardComponents.Add(awardComponent);
	mMiscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	//miscList->InsertItems(miscAwardComponents,0);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Expanding miscList root");
	mMiscList->Expand(rootItem);
	mWeaponList->Show();
	mLocationList->Show();
	mActionList->Show();
	mMiscList->Show();
	mXPList->Show();
				
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardChoicesPanel::SetFilter(wxString &filterString)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","SetFilter")
	mFilter=filterString;
	PopulateList(mWeaponList,"WEAPON","weapon");
	PopulateList(mActionList,"ACTION","action");
	PopulateList(mXPList,"XP","xp");
	PopulateList(mLocationList,"LOCATION","location");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardChoicesPanel::PopulateList(wxTreeCtrl *listBox,
									const char *group,
									const char *realnamePrefix)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","PopulateList")
	wxString		logEntryGroup;
	wxArrayString	keys;
	wxArrayString	values;
	wxArrayString	realNames;
	wxString		key;
	int				keyCount;
	int				keyIndex;
	wxString		realName;
	wxString		configKey;
	wxTreeItemId	rootItem;
	wxArrayString	weaponGroups;
	wxString		msg;

	msg.Printf("group [%s] realnamePrefix [%s]",group,realnamePrefix);
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	if (strcmp("WEAPON",group)==0)
	{
		weaponGroups=globalStatistics.configData.ReadWeaponGroups();
	}
	listBox->Hide();
	rootItem=listBox->GetRootItem();
	listBox->DeleteChildren(rootItem);
	logEntryGroup.Printf("LogEntries%s",group);
	globalStatistics.configData.ReadGroup(logEntryGroup, keys, values);
	WX_APPEND_ARRAY(keys,weaponGroups);
	keyCount=keys.GetCount();
	msg.Printf("keyCount = %d",keyCount);
	STATSGEN_DEBUG(DEBUG_ALWAYS,msg);
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"filter");
	STATSGEN_DEBUG(DEBUG_ALWAYS,mFilter);
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"key");
		key=keys.Item(keyIndex);
		STATSGEN_DEBUG(DEBUG_ALWAYS,key);
		if (mFilter.Length()>0)
		{
			mFilter = mFilter.Lower();
			wxString filterKey;
			filterKey=key.Lower();
			if (!filterKey.StartsWith(mFilter))
			{
				continue;
			}
		}
		configKey.Printf("/RealNames/%s_%s",
						realnamePrefix,
						STRING_TO_CHAR(key));
		globalStatistics.configData.ReadTextValue(configKey,&realName, key);
		STATSGEN_DEBUG(DEBUG_ALWAYS,realName);
		realNames.Add(realName);
		listBox->AppendItem(rootItem,realName,-1,-1,new StringTreeItemData(key));
	}
	listBox->Expand(rootItem);
	listBox->Show();
	STATSGEN_DEBUG_FUNCTION_END
}

AwardChoicesPanel::~AwardChoicesPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardChoicesPanel::OnRightClick(wxTreeEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","OnRightClick")
	StringTreeItemData	*itemData;
	wxArrayTreeItemIds	selections;
	wxString			codeSelected;
	wxTreeItemId		selection;
	int					selectionCount;
	int					selectionIndex;
	wxTreeCtrl			*selectedTree=NULL;

	mSelectedCodes.Clear();

	mSelectedChoiceTree=event.GetId();
	switch (mSelectedChoiceTree)
	{
		case WINDOW_ID_WEAPONLIST:
			selectedTree=mWeaponList;
			break;
		case WINDOW_ID_ACTIONLIST:
			selectedTree=mActionList;
			break;
		case WINDOW_ID_LOCATIONLIST:
			selectedTree=mLocationList;
			break;
		case WINDOW_ID_MISCAWARDLIST:
			selectedTree=mMiscList;
			break;
		case WINDOW_ID_XPAWARDLIST:
			selectedTree=mXPList;
			break;
	}
	if (selectedTree!=NULL)
	{
		selectedTree->GetSelections(selections);
	}
	selectionCount=selections.GetCount();
	for (selectionIndex=0;selectionIndex<selectionCount;selectionIndex++)
	{
		selection=selections.Item(selectionIndex);
		itemData=(StringTreeItemData *)selectedTree->GetItemData(selection);
		if (itemData != NULL)
		{
			codeSelected=itemData->GetString();
			mSelectedCodes.Add(codeSelected);
		}
	}

	event.SetId(this->GetId());
	//GetParent()->AddPendingEvent(event);
	GetParent()->GetEventHandler()->AddPendingEvent(event);
	
	STATSGEN_DEBUG_FUNCTION_END
}

AwardDefinitionPanel::AwardDefinitionPanel(wxWindow *parent, 
				wxWindowID id,
				const wxPoint &pos,
				const wxSize &size,
				long style,
				const wxString &name,
				wxString &awardIDIn):
			wxPanel(parent,
				id,
				pos,
				size,
				style,
				name)

{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","Constructor")
	long	listStyle;
	wxString			filterConfig="/tmp/awardeditorservertype";


	globalStatistics.configData.ReadTextValue(filterConfig,&mFilter);
	mAwardID=awardIDIn;

	mAwardDefinition=new AwardDefinition(mAwardID);

	//listStyle=wxTR_NO_BUTTONS | wxTR_HIDE_ROOT | wxTR_EXTENDED | wxTR_MULTIPLE;
	//listStyle=wxTR_NO_BUTTONS | wxTR_HIDE_ROOT | wxTR_MULTIPLE;
	listStyle=wxTR_HAS_BUTTONS |  wxTR_MULTIPLE;

	mWeaponList=new wxTreeCtrl(this, WINDOW_ID_WEAPONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mActionList=new wxTreeCtrl(this, WINDOW_ID_ACTIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mLocationList=new wxTreeCtrl(this, WINDOW_ID_LOCATIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mMiscList=new wxTreeCtrl(this, WINDOW_ID_MISCAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	mXPList=new wxTreeCtrl(this, WINDOW_ID_XPAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);

	mMainSizer = new wxBoxSizer(wxHORIZONTAL);
	mMainSizer->Add(mWeaponList,1,wxEXPAND);
	mMainSizer->Add(mLocationList,1,wxEXPAND);
	mMainSizer->Add(mActionList,1,wxEXPAND);
	mMainSizer->Add(mMiscList,1,wxEXPAND);
	mMainSizer->Add(mXPList,1,wxEXPAND);
	mMainSizer->SetSizeHints(this);
	SetSizer(mMainSizer);

	mWeaponList->AddRoot(_T("Weapons"));
	mActionList->AddRoot(_T("Actions"));
	mLocationList->AddRoot(_T("Locations"));
	mMiscList->AddRoot(_T("Misc"));
	mXPList->AddRoot(_T("XP"));

	mLabel=new wxStaticText(this,wxID_ANY,_T("Currently Defined Components For This Award"));

	mWeaponList->Hide();
	mLocationList->Hide();
	mActionList->Hide();
	mMiscList->Hide();
	mXPList->Hide();
	PopulateList(mWeaponList,mAwardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(mWeaponList,mAwardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(mWeaponList,mAwardDefinition->weaponTKComponents,"weapon","TK");
	PopulateList(mLocationList,mAwardDefinition->locationKillComponents,"location","KILL");
	PopulateList(mLocationList,mAwardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(mLocationList,mAwardDefinition->locationTKComponents,"location","TK");
	PopulateList(mActionList,mAwardDefinition->actionComponents,"action","");
	PopulateList(mXPList,mAwardDefinition->xpComponents,"xp","");
	PopulateList(mMiscList,mAwardDefinition->miscComponents,"","");
	mWeaponList->Show();
	mLocationList->Show();
	mActionList->Show();
	mMiscList->Show();
	mXPList->Show();
	STATSGEN_DEBUG_FUNCTION_END

}

AwardDefinitionPanel::~AwardDefinitionPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","Destructor")
	if (mAwardDefinition!=NULL)
	{
		delete(mAwardDefinition);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::SetFilter(wxString &filterString)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","SetFilter")
	mFilter=filterString;
	wxTreeItemId	weaponRootItem		= mWeaponList->GetRootItem();
	wxTreeItemId	locationRootItem	= mLocationList->GetRootItem();
	wxTreeItemId	actionRootItem		= mActionList->GetRootItem();
	wxTreeItemId	xpRootItem			= mXPList->GetRootItem();

	mWeaponList->Hide();
	mWeaponList->DeleteChildren(weaponRootItem);
	mLocationList->Hide();
	mLocationList->DeleteChildren(locationRootItem);
	mActionList->Hide();
	mActionList->DeleteChildren(actionRootItem);
	mXPList->Hide();
	mXPList->DeleteChildren(xpRootItem);
	PopulateList(mWeaponList,mAwardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(mWeaponList,mAwardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(mWeaponList,mAwardDefinition->weaponTKComponents,"weapon","TK");
	PopulateList(mLocationList,mAwardDefinition->locationKillComponents,"location","KILL");
	PopulateList(mLocationList,mAwardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(mLocationList,mAwardDefinition->locationTKComponents,"location","TK");
	PopulateList(mActionList,mAwardDefinition->actionComponents,"action","");
	PopulateList(mXPList,mAwardDefinition->xpComponents,"xp","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddWeaponComponent(wxString &code,bool kill,bool positive,bool teamkill)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddWeaponComponent")
	mAwardDefinition->AddWeaponComponent(code,kill,positive,teamkill);
	mWeaponList->DeleteAllItems();
	mWeaponList->AddRoot(_T("Weapons"));
	PopulateList(mWeaponList,mAwardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(mWeaponList,mAwardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(mWeaponList,mAwardDefinition->weaponTKComponents,"weapon","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveWeaponComponent(wxString &code,bool killList,bool teamkillList)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveWeaponComponent")
	mAwardDefinition->RemoveWeaponComponent(code,killList,teamkillList);
	mWeaponList->DeleteAllItems();
	mWeaponList->AddRoot(_T("Weapons"));
	PopulateList(mWeaponList,mAwardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(mWeaponList,mAwardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(mWeaponList,mAwardDefinition->weaponTKComponents,"weapon","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddLocationComponent(wxString &code,bool kill,bool positive,bool teamkill)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddLocationComponent")
	mAwardDefinition->AddLocationComponent(code,kill,positive,teamkill);
	mLocationList->DeleteAllItems();
	mLocationList->AddRoot(_T("Locations"));
	PopulateList(mLocationList,mAwardDefinition->locationKillComponents,"location","KILL");
	PopulateList(mLocationList,mAwardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(mLocationList,mAwardDefinition->locationTKComponents,"location","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveLocationComponent(wxString &code,bool killList,bool teamkillList)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveLocationComponent")
	mAwardDefinition->RemoveLocationComponent(code,killList,teamkillList);
	mLocationList->DeleteAllItems();
	mLocationList->AddRoot(_T("Locations"));
	PopulateList(mLocationList,mAwardDefinition->locationKillComponents,"location","KILL");
	PopulateList(mLocationList,mAwardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(mLocationList,mAwardDefinition->locationTKComponents,"location","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddActionComponent(wxString &code,bool positive)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddActionComponent")
	mAwardDefinition->AddActionComponent(code,positive);
	mActionList->DeleteAllItems();
	mActionList->AddRoot(_T("Actions"));
	PopulateList(mActionList,mAwardDefinition->actionComponents,"action","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveActionComponent(wxString &code)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveActionComponent")
	mAwardDefinition->RemoveActionComponent(code);
	mActionList->DeleteAllItems();
	mActionList->AddRoot(_T("Actions"));
	PopulateList(mActionList,mAwardDefinition->actionComponents,"action","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddMiscComponent(wxString &code,bool positive)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddMiscComponent")
	mAwardDefinition->AddMiscComponent(code,positive);
	mMiscList->DeleteAllItems();
	mMiscList->AddRoot(_T("Misc"));
	PopulateList(mMiscList,mAwardDefinition->miscComponents,"","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveMiscComponent(wxString &code)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveMiscComponent")
	mAwardDefinition->RemoveMiscComponent(code);
	mMiscList->DeleteAllItems();
	mMiscList->AddRoot(_T("Misc"));
	PopulateList(mMiscList,mAwardDefinition->miscComponents,"","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::PopulateList(wxTreeCtrl *listBox,
									wxArrayString &codes,
									const char *realnamePrefix,
									const char *suffix)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","PopulateList")
	wxString		code;
	int				codeCount;
	int				codeIndex;
	wxString		realName;
	wxString		configKey;
	bool			positive;
	bool			teamkilldummy;
	wxString		prefixedCode;
	wxTreeItemId	rootItem=listBox->GetRootItem();
	char			suffixChar;
	

	listBox->Hide();
	if (strcmp(suffix,"TK")==0)
	{
		suffixChar='T';
	}
	else
	{
		if (strcmp(suffix,"KILL")==0)
		{
			suffixChar='+';
		}
		else
		{
			suffixChar='-';
		}
	}

	codeCount=codes.GetCount();
	for (codeIndex=0;codeIndex<codeCount;codeIndex++)
	{
		code=codes.Item(codeIndex);
		prefixedCode=code;
		mAwardDefinition->SplitCode(code,&positive,&teamkilldummy);

		if (strlen(realnamePrefix)>0)
		{
			wxString filterCode;

			filterCode = code.Lower();
			mFilter=mFilter.Lower();
			if ((mFilter.Length()>0)&&(!globalStatistics.configData.IsWeaponGroupKey(filterCode)))
			{
				if (!filterCode.StartsWith(mFilter))
				{
					continue;
				}
			}
			configKey.Printf("/RealNames/%s_%s",
						realnamePrefix,
						STRING_TO_CHAR(code));
			globalStatistics.configData.ReadTextValue(configKey,&realName, code);
		}
		else
		{
			realName=code;
		}
		if (positive)
		{
			realName="(+)"+realName;
		}
		else
		{
			realName="(-)"+realName;
		}
		if (strlen(suffix)>0)
		{
			realName+="(";
			realName+=suffix;
			realName+=")";
		}
		prefixedCode=suffixChar+prefixedCode;
		listBox->AppendItem(rootItem,realName,-1,-1,new StringTreeItemData(prefixedCode));
	}
	STATSGEN_DEBUG(DEBUG_ALWAYS,(char *)"Expanding listBox root");
	listBox->Expand(rootItem);
	listBox->Show();
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::OnRightClick(wxTreeEvent &event)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","OnRightClick")
	StringTreeItemData	*itemData;
	wxArrayTreeItemIds	selections;
	wxString			codeSelected;
	wxTreeItemId		selection;
	int					selectionCount;
	int					selectionIndex;
	wxTreeCtrl			*selectedTree=NULL;

	mSelectedCodes.Clear();

	mSelectedDefinitionTree=event.GetId();
	switch (mSelectedDefinitionTree)
	{
		case WINDOW_ID_WEAPONLIST:
			selectedTree=mWeaponList;
			break;
		case WINDOW_ID_ACTIONLIST:
			selectedTree=mActionList;
			break;
		case WINDOW_ID_LOCATIONLIST:
			selectedTree=mLocationList;
			break;
		case WINDOW_ID_MISCAWARDLIST:
			selectedTree=mMiscList;
			break;
		case WINDOW_ID_XPAWARDLIST:
			selectedTree=mXPList;
			break;
	}
	if (selectedTree!=NULL)
	{
		selectedTree->GetSelections(selections);
	}
	selectionCount=selections.GetCount();
	for (selectionIndex=0;selectionIndex<selectionCount;selectionIndex++)
	{
		selection=selections.Item(selectionIndex);
		itemData=(StringTreeItemData *)selectedTree->GetItemData(selection);
		codeSelected=itemData->GetString();
		mSelectedCodes.Add(codeSelected);
	}

	event.SetId(this->GetId());
	//GetParent()->AddPendingEvent(event);
	GetParent()->GetEventHandler()->AddPendingEvent(event);
	
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddXPComponent(wxString &code,bool positive)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddXPComponent")
	mAwardDefinition->AddXPComponent(code,positive);
	mXPList->DeleteAllItems();
	mXPList->AddRoot(_T("XP"));
	PopulateList(mXPList,mAwardDefinition->xpComponents,"xp","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveXPComponent(wxString &code)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveXPComponent")
	mAwardDefinition->RemoveXPComponent(code);
	mXPList->DeleteAllItems();
	mXPList->AddRoot(_T("XP"));
	PopulateList(mXPList,mAwardDefinition->xpComponents,"xp","");
	STATSGEN_DEBUG_FUNCTION_END
}

