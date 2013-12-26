
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

	/*	Create database here if does not exist
		If exist open it and load records
	*/
	// g_pChatRecords = new CChatRecordDB();

	//if (!bSuccess || NULL == g_pChatRecords || NULL == g_pChatRecords->Create(NULL, AfxGetInstanceHandle()))
	/*if (NULL == g_pChatRecords || FALSE == g_pChatRecords->Open())
    {
		MessageBox(NULL, L"Create Database Failure.", L"Database", MB_OK);
        delete g_pChatRecords;
        g_pChatRecords = NULL;
		return FALSE;
    }*/

	// ref for working sample with modeless dialog: http://simplesamples.info/MFC/ModelessDialogs.html
	// msdn ref: http://msdn.microsoft.com/en-us/library/hf0yazk7.aspx
	// ref: http://support.microsoft.com/kb/103788
	CTestEmoCustomControlDlg *pdlgModeless = new CTestEmoCustomControlDlg(NULL);
	m_pMainWnd = pdlgModeless;
	if (pdlgModeless->Create(CTestEmoCustomControlDlg::IDD))
		return TRUE;

	// ShowWindow not required for dialog with WS_VISIBLE style
	/*pdlgModeless->ShowWindow(SW_SHOWNORMAL);
	pdlgModeless->UpdateWindow();


	MSG Msg;
	while(GetMessage(&Msg, NULL, 0, 0))
    {
		if(!IsDialogMessage(pdlgModeless->GetSafeHwnd(), &Msg))
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }


	/*if (g_pChatRecords) {
        delete g_pChatRecords;
        g_pChatRecords = NULL;
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.

	return FALSE;
}
