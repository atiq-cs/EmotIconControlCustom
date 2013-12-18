
// TestEmoCustomControl.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestEmoCustomControl.h"
#include "TestEmoCustomControlDlg.h"
#include "ChatRecordDB.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CChatRecordDB* g_pChatRecords;


// CTestEmoCustomControlApp

BEGIN_MESSAGE_MAP(CTestEmoCustomControlApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestEmoCustomControlApp construction

CTestEmoCustomControlApp::CTestEmoCustomControlApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTestEmoCustomControlApp object

CTestEmoCustomControlApp theApp;


// CTestEmoCustomControlApp initialization

BOOL CTestEmoCustomControlApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// create database here
	BOOL bSuccess;
    bSuccess = FALSE;
	g_pChatRecords = new CChatRecordDB(bSuccess);

	//if (!bSuccess || NULL == g_pChatRecords || NULL == g_pChatRecords->Create(NULL, AfxGetInstanceHandle()))
	if (!bSuccess || NULL == g_pChatRecords || FALSE == g_pChatRecords->Open(m_hInstance))
    {
        delete g_pChatRecords;
        g_pChatRecords = NULL;
		return FALSE;
    }

	CTestEmoCustomControlDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
	}

	if (g_pChatRecords) {
        delete g_pChatRecords;
        g_pChatRecords = NULL;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
