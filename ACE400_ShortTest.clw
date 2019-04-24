; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CShortTestDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ace400_shorttest.h"
LastPage=0

ClassCount=5
Class1=CACE400_ShortTestApp
Class2=CAboutDlg
Class3=CACE400MainDlg
Class4=CConfigDialog
Class5=CShortTestDialog

ResourceCount=4
Resource1=IDD_SHORT_TEST_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDD_MAIN_DIALOG
Resource4=IDD_CONFIG_DIALOG

[CLS:CACE400_ShortTestApp]
Type=0
BaseClass=CWinApp
HeaderFile=ACE400_ShortTest.h
ImplementationFile=ACE400_ShortTest.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ACE400MainDlg.cpp
ImplementationFile=ACE400MainDlg.cpp
LastObject=CAboutDlg

[CLS:CACE400MainDlg]
Type=0
BaseClass=CDialog
HeaderFile=ACE400MainDlg.h
ImplementationFile=ACE400MainDlg.cpp

[CLS:CConfigDialog]
Type=0
BaseClass=CDialog
HeaderFile=ConfigDialog.h
ImplementationFile=ConfigDialog.cpp

[CLS:CShortTestDialog]
Type=0
HeaderFile=shorttestdialog.h
ImplementationFile=shorttestdialog.cpp
BaseClass=CDialog
LastObject=CShortTestDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_MAIN_DIALOG]
Type=1
Class=CACE400MainDlg
ControlCount=3
Control1=IDC_BUTTON_SHORT_TEST,button,1342242816
Control2=IDC_BUTTON_CONFIG,button,1342242816
Control3=IDOK,button,1342242816

[DLG:IDD_SHORT_TEST_DIALOG]
Type=1
Class=CShortTestDialog
ControlCount=16
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_SQUARE_PIN_NUM,edit,1350631552
Control3=IDC_BUTTON_INPUT_SQUARE_PIN_NUM,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_SQUARE_N,edit,1350633600
Control6=IDC_STATIC,button,1342177287
Control7=IDC_EDIT_BAD_PIN,edit,1350631552
Control8=IDC_BUTTON_ADD_BAD_PIN,button,1342242816
Control9=IDC_LIST_BAD_PIN,listbox,1352728835
Control10=IDC_BUTTON_RESET_BAD_PIN,button,1342242816
Control11=IDC_EDIT_SQUARE_MATRIX,edit,1350633668
Control12=IDC_EDIT_MEAS_OUTPUT,edit,1353779204
Control13=IDC_BUTTON_MEASURE,button,1342242816
Control14=IDC_STATIC,static,1342308352
Control15=IDC_CHECK1,button,1342242819
Control16=IDC_CHECK_EXCLUDE_VC,button,1342242819

[DLG:IDD_CONFIG_DIALOG]
Type=1
Class=?
ControlCount=11
Control1=IDC_CHECK_PRT_BASIC,button,1342246915
Control2=IDC_STATIC,button,1342177287
Control3=IDC_CHECK_PRT_LEVEL1,button,1342246915
Control4=IDC_CHECK_PRT_Level2,button,1342246915
Control5=IDC_CHECK_PRT_LEVEL3,button,1342246915
Control6=IDC_CHECK_PRT_DEVICE,button,1342246915
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC_PRINT_STATE,static,1342308865
Control9=IDC_CHECK_PRT_DEV_D64,button,1342246915
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC_USED_MEM,static,1342308865

