// Statsgen Includes
#include "AwardEditorPanel.h"
#include "GlobalStatistics.h"

BEGIN_EVENT_TABLE(AwardEditorPanel, wxPanel)
		EVT_SIZE(AwardEditorPanel::OnResize)
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
		EVT_SIZE(AwardChoicesPanel::OnResize)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_WEAPONLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_ACTIONLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_LOCATIONLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_MISCAWARDLIST,AwardChoicesPanel::OnRightClick)
		EVT_TREE_ITEM_RIGHT_CLICK(WINDOW_ID_XPAWARDLIST,AwardChoicesPanel::OnRightClick)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(AwardDefinitionPanel, wxPanel)
		EVT_SIZE(AwardDefinitionPanel::OnResize)
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
	wxString		configGroup;
	wxString		configKey;
	wxSizeEvent		dummyEvent;
	wxString		serverTypeConfigKey="/tmp/awardeditorservertype";
	int				serverTypeIndex;
	wxString		serverTypeCode;
	wxString		serverTypeName;
	wxArrayString	serverTypeCodes;
	wxArrayString	serverTypeNames;

	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","Constructor")
	awardID=awardIDIn;
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
	configGroup="AWARDDEFINITION"+awardID;
	overallConfig=new GroupedConfigItemsPanel("Overall Details");
	overallConfig->Create(this,WINDOW_ID_TEXTCTRL_CONFIGVALUE,wxDefaultPosition,wxDefaultSize);

	overallConfig->AddSelection("Server Type Filter",serverTypeConfigKey,"",serverTypeCodes,serverTypeNames);
	configKey.Printf("/%s/Name",configGroup.GetData());
	overallConfig->Add("Name", configKey,"",-1,NULL);
	configKey.Printf("/%s/Image",configGroup.GetData());
	overallConfig->Add("Image URL", configKey,"",-1,NULL);
	configKey.Printf("/%s/Weighted",configGroup.GetData());
	overallConfig->AddBoolean("Weighted", configKey,"N",NULL);
	splitter=new wxSplitterWindow(this, -1, 
								wxDefaultPosition,
								wxDefaultSize,
								wxSP_3D);
								
	awardChoices=new AwardChoicesPanel(
						splitter,
						WINDOW_ID_AWARDCHOICESPANEL,
						wxDefaultPosition,
						wxDefaultSize,
						0,
						_T(""),
						awardID);
	awardChoices->Enable();
	awardDefinition=new AwardDefinitionPanel(
						splitter,
						WINDOW_ID_AWARDDEFINITIONPANEL,
						wxDefaultPosition,
						wxDefaultSize,
						0,
						_T(""),
						awardID);

	awardDefinition->Enable();
	splitter->SplitHorizontally(awardChoices,awardDefinition);
	splitter->SetSashGravity(0.5);

	OnResize(dummyEvent);
	STATSGEN_DEBUG_FUNCTION_END
}

AwardEditorPanel::~AwardEditorPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","Destructor")
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardEditorPanel::OnConfigChanged(wxCommandEvent &event)
{
	wxString			serverTypeConfigKey="/tmp/awardeditorservertype";
	static wxString		lastConfigValue="";
	wxString			serverTypeConfigValue;

	globalStatistics.configData.ReadTextValue(serverTypeConfigKey,&serverTypeConfigValue);

	if (serverTypeConfigValue.Cmp(lastConfigValue)!=0)
	{
		lastConfigValue=serverTypeConfigValue;
		if ((awardChoices!=NULL)&&(awardDefinition!=NULL))
		{
			awardChoices->SetFilter(serverTypeConfigValue);
			awardDefinition->SetFilter(serverTypeConfigValue);
		}
	}

}

void AwardEditorPanel::OnPopupMenu(wxCommandEvent &event)
{
	long			selectedTree;
	int				selectedCodesCount;
	int				selectedCodesIndex;
	wxString		selectedCode;
	int				menuSelection=event.GetId();
	wxArrayString	selectedCodes;
	bool			killList;
	bool			teamkillList;

	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","OnPopupMenu")
	if (menuSelection==AWARD_DEFINITION_DELETE_COMPONENT)
	{
		// Award Definition 
		selectedTree=awardDefinition->selectedDefinitionTree;
		selectedCodes=awardDefinition->selectedCodes;
	}
	else
	{
		// Award Choice
		selectedTree=awardChoices->selectedChoiceTree;
		selectedCodes=awardChoices->selectedCodes;
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
						awardDefinition->awardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						awardDefinition->RemoveWeaponComponent(selectedCode,killList,teamkillList);
						break;
					case AWARD_CHOICES_ADD_KILL_POSITIVE:
					case AWARD_CHOICES_ADD_KILL_NEGATIVE:
					case AWARD_CHOICES_ADD_DEATH_POSITIVE:
					case AWARD_CHOICES_ADD_DEATH_NEGATIVE:
					case AWARD_CHOICES_ADD_TK_POSITIVE:
					case AWARD_CHOICES_ADD_TK_NEGATIVE:
						awardDefinition->AddWeaponComponent(
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
						awardDefinition->AddWeaponComponent(selectedCode,true,true,false);
						awardDefinition->AddWeaponComponent(selectedCode,false,false,false);
						break;
				}
				break;
			case WINDOW_ID_LOCATIONLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						awardDefinition->awardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						awardDefinition->RemoveLocationComponent(selectedCode,killList,teamkillList);
						break;
					case AWARD_CHOICES_ADD_KILL_POSITIVE:
					case AWARD_CHOICES_ADD_KILL_NEGATIVE:
					case AWARD_CHOICES_ADD_DEATH_POSITIVE:
					case AWARD_CHOICES_ADD_DEATH_NEGATIVE:
					case AWARD_CHOICES_ADD_TK_POSITIVE:
					case AWARD_CHOICES_ADD_TK_NEGATIVE:
						awardDefinition->AddLocationComponent(
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
						awardDefinition->AddLocationComponent(selectedCode,true,true,false);
						awardDefinition->AddLocationComponent(selectedCode,false,false,false);
						break;
				}
				break;
			case WINDOW_ID_ACTIONLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						awardDefinition->awardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						awardDefinition->RemoveActionComponent(selectedCode);
						break;
					case AWARD_CHOICES_ADD_POSITIVE:
					case AWARD_CHOICES_ADD_NEGATIVE:
						awardDefinition->AddActionComponent(
											selectedCode,
											(menuSelection==AWARD_CHOICES_ADD_POSITIVE));
						break;
				}
				break;
			case WINDOW_ID_MISCAWARDLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						awardDefinition->awardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						awardDefinition->RemoveMiscComponent(selectedCode);
						break;
					case AWARD_CHOICES_ADD_POSITIVE:
					case AWARD_CHOICES_ADD_NEGATIVE:
						awardDefinition->AddMiscComponent(
											selectedCode,
											(menuSelection==AWARD_CHOICES_ADD_POSITIVE));
						break;
				}
				break;
			case WINDOW_ID_XPAWARDLIST:
				switch (menuSelection)
				{
					case AWARD_DEFINITION_DELETE_COMPONENT:
						awardDefinition->awardDefinition->SplitCode(selectedCode,&killList,&teamkillList);
						awardDefinition->RemoveXPComponent(selectedCode);
						break;
					case AWARD_CHOICES_ADD_POSITIVE:
					case AWARD_CHOICES_ADD_NEGATIVE:
						awardDefinition->AddXPComponent(
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
	wxMenu	popupMenu(_T(""));
	long	panelId=event.GetId();
	long	selectedChoiceTree=awardChoices->selectedChoiceTree;

	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","OnRightClick")
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

void AwardEditorPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;
	wxSize		itemSize;
	int			panelWidth;
	int		panelHeight;
	int		overallConfigWidth;
	int		overallConfigHeight;
	int		choiceHeight;
	int		choiceWidth;

	STATSGEN_DEBUG_FUNCTION_START("AwardEditorPanel","OnResize")
	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();
	
	overallConfigWidth=panelWidth;
	overallConfigHeight=panelHeight;


	overallConfigHeight=100;
	choiceHeight=(panelHeight-overallConfigHeight);
	choiceWidth=panelWidth;
	
	splitter->SetSize(0,overallConfigHeight,
						choiceWidth,choiceHeight);
	splitter->SetSashPosition(choiceHeight/2);

	overallConfig->SetSize(0,0,
				overallConfigWidth,
				overallConfigHeight);
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
	wxTreeItemId	rootItem;
	long			listStyle;
	wxString		filterConfig="/tmp/awardeditorservertype";


	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","Constructor")
	awardID=awardIDIn;
	globalStatistics.configData.ReadTextValue(filterConfig,&filter);

	listStyle=wxTR_NO_BUTTONS | wxTR_HIDE_ROOT | wxTR_EXTENDED | wxTR_MULTIPLE;

	weaponList=new wxTreeCtrl(this, WINDOW_ID_WEAPONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	actionList=new wxTreeCtrl(this, WINDOW_ID_ACTIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	locationList=new wxTreeCtrl(this, WINDOW_ID_LOCATIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	miscList=new wxTreeCtrl(this, WINDOW_ID_MISCAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	xpList=new wxTreeCtrl(this, WINDOW_ID_XPAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);

	weaponList->AddRoot(_T(""));
	actionList->AddRoot(_T(""));
	xpList->AddRoot(_T(""));
	locationList->AddRoot(_T(""));
	rootItem=miscList->AddRoot(_T(""));

	label=new wxStaticText(this,-1,_T("Choose From The Following Award Components"));
	weaponList->Hide();
	locationList->Hide();
	actionList->Hide();
	xpList->Hide();
	miscList->Hide();
	PopulateList(weaponList,"WEAPON","weapon");
	PopulateList(actionList,"ACTION","action");
	PopulateList(xpList,"XP","xp");
	PopulateList(locationList,"LOCATION","location");

	wxArrayString	miscAwardComponents;
	wxString	awardComponent;

	awardComponent="Speech";miscAwardComponents.Add(awardComponent);
	miscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Team Win";miscAwardComponents.Add(awardComponent);
	miscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Team Loss";miscAwardComponents.Add(awardComponent);
	miscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Suicides";miscAwardComponents.Add(awardComponent);
	miscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Kills";miscAwardComponents.Add(awardComponent);
	miscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Deaths";miscAwardComponents.Add(awardComponent);
	miscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	awardComponent="Team Kills";miscAwardComponents.Add(awardComponent);
	miscList->AppendItem(rootItem,awardComponent,-1,-1,new StringTreeItemData(awardComponent));
	//miscList->InsertItems(miscAwardComponents,0);
	miscList->Expand(rootItem);
	weaponList->Show();
	locationList->Show();
	actionList->Show();
	miscList->Show();
	xpList->Show();
				
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardChoicesPanel::SetFilter(wxString &filterString)
{
	filter=filterString;
	PopulateList(weaponList,"WEAPON","weapon");
	PopulateList(actionList,"ACTION","action");
	PopulateList(xpList,"XP","xp");
	PopulateList(locationList,"LOCATION","location");
}

void AwardChoicesPanel::PopulateList(wxTreeCtrl *listBox,
									const char *group,
									const char *realnamePrefix)
{
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
	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","PopulateList")

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
	for (keyIndex=0;keyIndex<keyCount;keyIndex++)
	{
		key=keys.Item(keyIndex);
		if (filter.Length()>0)
		{
			filter = filter.Lower();
			wxString filterKey;
			filterKey=key.Lower();
			if (!filterKey.StartsWith(filter))
			{
				continue;
			}
		}
		configKey.Printf("/RealNames/%s_%s",
						realnamePrefix,
						key.GetData());
		globalStatistics.configData.ReadTextValue(configKey,&realName,
												(char *)key.GetData());
		realNames.Add(realName);
		listBox->AppendItem(rootItem,realName,-1,-1,new StringTreeItemData(key));
	}
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
	StringTreeItemData	*itemData;
	wxArrayTreeItemIds	selections;
	wxString			codeSelected;
	wxTreeItemId		selection;
	int					selectionCount;
	int					selectionIndex;
	wxTreeCtrl			*selectedTree=NULL;

	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","OnRightClick")
	selectedCodes.Clear();

	selectedChoiceTree=event.GetId();
	switch (selectedChoiceTree)
	{
		case WINDOW_ID_WEAPONLIST:
			selectedTree=weaponList;
			break;
		case WINDOW_ID_ACTIONLIST:
			selectedTree=actionList;
			break;
		case WINDOW_ID_LOCATIONLIST:
			selectedTree=locationList;
			break;
		case WINDOW_ID_MISCAWARDLIST:
			selectedTree=miscList;
			break;
		case WINDOW_ID_XPAWARDLIST:
			selectedTree=xpList;
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
		selectedCodes.Add(codeSelected);
	}

	event.SetId(this->GetId());
	GetParent()->AddPendingEvent(event);
	
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardChoicesPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;
	wxSize		itemSize;
	int		panelWidth;
	int		panelHeight;
	int		choiceHeight;
	int		choiceWidth;
	int		labelHeight;
	int		labelWidth;
	int		labelX;

	STATSGEN_DEBUG_FUNCTION_START("AwardChoicesPanel","OnResize")
	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();
	
	itemSize=label->GetSize();
	labelWidth=itemSize.GetWidth();
	labelHeight=itemSize.GetHeight();

	labelX=(panelWidth-labelWidth)/2;
	choiceHeight=panelHeight-labelHeight;
	choiceWidth=panelWidth/5;

	label->SetSize(labelX,0, labelWidth,labelHeight);
	weaponList->SetSize(0,labelHeight, choiceWidth,choiceHeight);
	locationList->SetSize(choiceWidth*1,labelHeight, choiceWidth,choiceHeight);
	actionList->SetSize(choiceWidth*2,labelHeight, choiceWidth,choiceHeight);
	miscList->SetSize(choiceWidth*3,labelHeight, choiceWidth,choiceHeight);
	xpList->SetSize(choiceWidth*4,labelHeight, choiceWidth,choiceHeight);
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
	long	listStyle;
	wxString			filterConfig="/tmp/awardeditorservertype";

	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","Constructor")

	globalStatistics.configData.ReadTextValue(filterConfig,&filter);
	awardID=awardIDIn;

	awardDefinition=new AwardDefinition(awardID);

	listStyle=wxTR_NO_BUTTONS | wxTR_HIDE_ROOT | wxTR_EXTENDED | wxTR_MULTIPLE;

	weaponList=new wxTreeCtrl(this, WINDOW_ID_WEAPONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	actionList=new wxTreeCtrl(this, WINDOW_ID_ACTIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	locationList=new wxTreeCtrl(this, WINDOW_ID_LOCATIONLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	miscList=new wxTreeCtrl(this, WINDOW_ID_MISCAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);
	xpList=new wxTreeCtrl(this, WINDOW_ID_XPAWARDLIST, wxDefaultPosition,wxDefaultSize, listStyle);

	weaponList->AddRoot(_T(""));
	actionList->AddRoot(_T(""));
	locationList->AddRoot(_T(""));
	miscList->AddRoot(_T(""));
	xpList->AddRoot(_T(""));

	label=new wxStaticText(this,-1,_T("Currently Defined Components For This Award"));

	weaponList->Hide();
	locationList->Hide();
	actionList->Hide();
	miscList->Hide();
	xpList->Hide();
	PopulateList(weaponList,awardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(weaponList,awardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(weaponList,awardDefinition->weaponTKComponents,"weapon","TK");
	PopulateList(locationList,awardDefinition->locationKillComponents,"location","KILL");
	PopulateList(locationList,awardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(locationList,awardDefinition->locationTKComponents,"location","TK");
	PopulateList(actionList,awardDefinition->actionComponents,"action","");
	PopulateList(xpList,awardDefinition->xpComponents,"xp","");
	PopulateList(miscList,awardDefinition->miscComponents,"","");
	weaponList->Show();
	locationList->Show();
	actionList->Show();
	miscList->Show();
	xpList->Show();
	STATSGEN_DEBUG_FUNCTION_END

}

AwardDefinitionPanel::~AwardDefinitionPanel()
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","Destructor")
	if (awardDefinition!=NULL)
	{
		delete(awardDefinition);
	}
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::SetFilter(wxString &filterString)
{
	filter=filterString;
	wxTreeItemId	weaponRootItem=weaponList->GetRootItem();
	wxTreeItemId	locationRootItem=locationList->GetRootItem();
	wxTreeItemId	actionRootItem=actionList->GetRootItem();
	wxTreeItemId	xpRootItem=xpList->GetRootItem();

	weaponList->Hide();
	weaponList->DeleteChildren(weaponRootItem);
	locationList->Hide();
	locationList->DeleteChildren(locationRootItem);
	actionList->Hide();
	actionList->DeleteChildren(actionRootItem);
	xpList->Hide();
	xpList->DeleteChildren(xpRootItem);
	PopulateList(weaponList,awardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(weaponList,awardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(weaponList,awardDefinition->weaponTKComponents,"weapon","TK");
	PopulateList(locationList,awardDefinition->locationKillComponents,"location","KILL");
	PopulateList(locationList,awardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(locationList,awardDefinition->locationTKComponents,"location","TK");
	PopulateList(actionList,awardDefinition->actionComponents,"action","");
	PopulateList(xpList,awardDefinition->xpComponents,"xp","");
}

void AwardDefinitionPanel::AddWeaponComponent(wxString &code,bool kill,bool positive,bool teamkill)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddWeaponComponent")
	awardDefinition->AddWeaponComponent(code,kill,positive,teamkill);
	weaponList->DeleteAllItems();
	weaponList->AddRoot(_T(""));
	PopulateList(weaponList,awardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(weaponList,awardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(weaponList,awardDefinition->weaponTKComponents,"weapon","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveWeaponComponent(wxString &code,bool killList,bool teamkillList)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveWeaponComponent")
	awardDefinition->RemoveWeaponComponent(code,killList,teamkillList);
	weaponList->DeleteAllItems();
	weaponList->AddRoot(_T(""));
	PopulateList(weaponList,awardDefinition->weaponKillComponents,"weapon","KILL");
	PopulateList(weaponList,awardDefinition->weaponDeathComponents,"weapon","DEATH");
	PopulateList(weaponList,awardDefinition->weaponTKComponents,"weapon","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddLocationComponent(wxString &code,bool kill,bool positive,bool teamkill)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddLocationComponent")
	awardDefinition->AddLocationComponent(code,kill,positive,teamkill);
	locationList->DeleteAllItems();
	locationList->AddRoot(_T(""));
	PopulateList(locationList,awardDefinition->locationKillComponents,"location","KILL");
	PopulateList(locationList,awardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(locationList,awardDefinition->locationTKComponents,"location","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveLocationComponent(wxString &code,bool killList,bool teamkillList)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveLocationComponent")
	awardDefinition->RemoveLocationComponent(code,killList,teamkillList);
	locationList->DeleteAllItems();
	locationList->AddRoot(_T(""));
	PopulateList(locationList,awardDefinition->locationKillComponents,"location","KILL");
	PopulateList(locationList,awardDefinition->locationDeathComponents,"location","DEATH");
	PopulateList(locationList,awardDefinition->locationTKComponents,"location","TK");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddActionComponent(wxString &code,bool positive)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddActionComponent")
	awardDefinition->AddActionComponent(code,positive);
	actionList->DeleteAllItems();
	actionList->AddRoot(_T(""));
	PopulateList(actionList,awardDefinition->actionComponents,"action","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveActionComponent(wxString &code)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveActionComponent")
	awardDefinition->RemoveActionComponent(code);
	actionList->DeleteAllItems();
	actionList->AddRoot(_T(""));
	PopulateList(actionList,awardDefinition->actionComponents,"action","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddMiscComponent(wxString &code,bool positive)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddMiscComponent")
	awardDefinition->AddMiscComponent(code,positive);
	miscList->DeleteAllItems();
	miscList->AddRoot(_T(""));
	PopulateList(miscList,awardDefinition->miscComponents,"","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveMiscComponent(wxString &code)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveMiscComponent")
	awardDefinition->RemoveMiscComponent(code);
	miscList->DeleteAllItems();
	miscList->AddRoot(_T(""));
	PopulateList(miscList,awardDefinition->miscComponents,"","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::OnResize(wxSizeEvent &event)
{
	wxString	msg;
	wxSize		itemSize;
	int		panelWidth;
	int		panelHeight;
	int		choiceHeight;
	int		choiceWidth;
	int		labelWidth;
	int		labelHeight;
	int		labelX;

	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","OnResize")
	itemSize=GetSize();
	panelWidth=itemSize.GetWidth();
	panelHeight=itemSize.GetHeight();

	itemSize=label->GetSize();
	labelWidth=itemSize.GetWidth();
	labelHeight=itemSize.GetHeight();
	labelX=(panelWidth-labelWidth)/2;

	choiceHeight=panelHeight-labelHeight;
	choiceWidth=panelWidth/5;

	label->SetSize(labelX,0,labelWidth,labelHeight);
	weaponList->SetSize(0,labelHeight, choiceWidth,choiceHeight);
	locationList->SetSize(choiceWidth*1,labelHeight, choiceWidth,choiceHeight);
	actionList->SetSize(choiceWidth*2,labelHeight, choiceWidth,choiceHeight);
	miscList->SetSize(choiceWidth*3,labelHeight, choiceWidth,choiceHeight);
	xpList->SetSize(choiceWidth*4,labelHeight, choiceWidth,choiceHeight);

	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::PopulateList(wxTreeCtrl *listBox,
									wxArrayString &codes,
									const char *realnamePrefix,
									const char *suffix)
{
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
	

	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","PopulateList")
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
		awardDefinition->SplitCode(code,&positive,&teamkilldummy);

		if (strlen(realnamePrefix)>0)
		{
			wxString filterCode;

			filterCode = code.Lower();
			filter=filter.Lower();
			if ((filter.Length()>0)&&(!globalStatistics.configData.IsWeaponGroupKey(filterCode)))
			{
				if (!filterCode.StartsWith(filter))
				{
					continue;
				}
			}
			configKey.Printf("/RealNames/%s_%s",
						realnamePrefix,
						code.GetData());
			globalStatistics.configData.ReadTextValue(configKey,&realName,
												(char *)code.GetData());
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
	listBox->Expand(rootItem);
	listBox->Show();
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::OnRightClick(wxTreeEvent &event)
{
	StringTreeItemData	*itemData;
	wxArrayTreeItemIds	selections;
	wxString			codeSelected;
	wxTreeItemId		selection;
	int					selectionCount;
	int					selectionIndex;
	wxTreeCtrl			*selectedTree=NULL;

	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","OnRightClick")
	selectedCodes.Clear();

	selectedDefinitionTree=event.GetId();
	switch (selectedDefinitionTree)
	{
		case WINDOW_ID_WEAPONLIST:
			selectedTree=weaponList;
			break;
		case WINDOW_ID_ACTIONLIST:
			selectedTree=actionList;
			break;
		case WINDOW_ID_LOCATIONLIST:
			selectedTree=locationList;
			break;
		case WINDOW_ID_MISCAWARDLIST:
			selectedTree=miscList;
			break;
		case WINDOW_ID_XPAWARDLIST:
			selectedTree=xpList;
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
		selectedCodes.Add(codeSelected);
	}

	event.SetId(this->GetId());
	GetParent()->AddPendingEvent(event);
	
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::AddXPComponent(wxString &code,bool positive)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","AddXPComponent")
	awardDefinition->AddXPComponent(code,positive);
	xpList->DeleteAllItems();
	xpList->AddRoot(_T(""));
	PopulateList(xpList,awardDefinition->xpComponents,"xp","");
	STATSGEN_DEBUG_FUNCTION_END
}

void AwardDefinitionPanel::RemoveXPComponent(wxString &code)
{
	STATSGEN_DEBUG_FUNCTION_START("AwardDefinitionPanel","RemoveXPComponent")
	awardDefinition->RemoveXPComponent(code);
	xpList->DeleteAllItems();
	xpList->AddRoot(_T(""));
	PopulateList(xpList,awardDefinition->xpComponents,"xp","");
	STATSGEN_DEBUG_FUNCTION_END
}

