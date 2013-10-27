
// TestEmoCustomControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestEmoCustomControl.h"
#include "TestEmoCustomControlDlg.h"
#include "EmoListPopUp.h" // for emo pop up
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestEmoCustomControlDlg dialog
CTestEmoCustomControlDlg::CTestEmoCustomControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEmoCustomControlDlg::IDD, pParent),
	m_emoListPopUpDlg(NULL) //,
	//m_pTooltip(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (! emoPopUpBmp.LoadBitmaps(_T("IDB_BMP_EMOPOP_UP"), _T("IDB_BMP_EMOPOP_DOWN"), _T("IDB_BMP_EMOPOP_FOCUS"), _T("IDB_BMP_EMOPOP_UP"))) {
	
	//if (! emoPopUpBmp.LoadBitmaps(_T("IDB_BMP_EMOTICON_01"), _T("IDB_BMP_EMOTICON_01"), _T("IDB_BMP_EMOTICON_01"), _T("IDB_BMP_EMOTICON_01"))) {
	// require definition in resource.h
	//if (! emoPopUpBmp.LoadBitmaps(IDB_BMP_EMOPOP_UP, IDB_BMP_EMOPOP_DOWN, IDB_BMP_EMOPOP_FOCUS, IDB_BMP_EMOPOP_UP)) {
		AfxMessageBox(_T("Failed to load bitmaps for buttons\n"));
		AfxThrowResourceException();
	}

}

CTestEmoCustomControlDlg::~CTestEmoCustomControlDlg() {
	//delete m_pTooltip;
}

void CTestEmoCustomControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHATCUSTOM, m_ChatEmoBox);
	DDX_Control(pDX, IDC_EDIT_CHAT_IN, m_chatBoxCtrl);
	DDX_Control(pDX, IDC_EMOPOP, emoPopUpBmp);
}

BEGIN_MESSAGE_MAP(CTestEmoCustomControlDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestEmoCustomControlDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_EMOPOP, &CTestEmoCustomControlDlg::OnBnClickedEmoPop)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CTestEmoCustomControlDlg message handlers

BOOL CTestEmoCustomControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// each dialog control has special bitmaps
	//emoPopUpBmp.AutoLoad(IDC_EMOPOP, this);

	//emoPopUpBmp.SizeToContent();
	/*
	tool tip experiment
	m_pTooltip = new CToolTipCtrl;
	if(!m_pTooltip->Create(this, WS_POPUP|TTS_ALWAYSTIP))
    {
        TRACE("Unable To create ToolTip\n");
        return TRUE;
    }
	// ref: http://msdn.microsoft.com/en-us/library/s2y2wf56(v=vs.110).aspx
	// The lpRectTool and nIDTool parameters must both be valid, or if lpRectTool is NULL, nIDTool must be 0.
	//VERIFY(m_pTooltip->AddTool(&buttonTest, _T("smileee text over here unbelievable!!!")));

	CWnd *pBWnd = GetDlgItem(IDC_EDIT_CHAT_IN);
	VERIFY(m_pTooltip->AddTool(pBWnd, _T("smileee text over here unbelievable!!!")));
	pBWnd = GetDlgItem(IDOK);
	VERIFY(m_pTooltip->AddTool(pBWnd, _T("smileee text over here unbelievable!!!")));
	//this->EnableToolTips();
	// ref:http://msdn.microsoft.com/en-us/library/7tabdbhs(v=vs.110).aspx 
	m_pTooltip->Activate(TRUE);
	*/


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestEmoCustomControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestEmoCustomControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestEmoCustomControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestEmoCustomControlDlg::OnBnClickedOk()
{
	CString m_chInText;		// chat input text
	// CEdit *chatBoxEditCtrl = (CEdit *) GetDlgItem(IDC_EDIT_CHAT_IN);
	//if (chatBoxEditCtrl == NULL)
		//return;
	//chatBoxEditCtrl->GetWindowText(m_chInText);
	m_chatBoxCtrl.GetWindowText(m_chInText);
	//GetDlgItemText(IDC_EDIT_CHAT_IN, m_chInText);
	if (m_chInText.GetLength() > 0) {
		// primarily important operation here is to sent the text and print
		CTime timeDate = CTime::GetCurrentTime();
		m_ChatEmoBox.PostChatMessage(m_chInText, timeDate);
		//m_emoListControl.InsertItemEmo(n, m_chInText);
		//ATLASSERT(s == _T("Friday, March 19, 1999"));   
		// m_emoListControl.SetItemText(n, 1, tmStr);
		//SetDlgItemText(IDC_EDIT_CHAT_IN, _T(""));
		m_chatBoxCtrl.SetWindowText(_T(""));
	}
	GotoDlgCtrl(&m_chatBoxCtrl);		// ref: http://blogs.msdn.com/b/oldnewthing/archive/2004/08/02/205624.aspx
	// No need to destroy now
	// CDialog::OnOK();
}


void CTestEmoCustomControlDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/*CRect rc;
	GetWindowRect(&rc);
	ClientToScreen(rc);
	CString dbgStr;
	dbgStr.Format(TEXT("cx = %d, cy = %d, width = %d, height = %d"), cx, cy, rc.Width(), rc.Height());
	AfxMessageBox(dbgStr, MB_OK);*/

	// TODO: Add your message handler code here
	// resize custom control
	// AfxMessageBox(TEXT("we are in dlg size"), MB_OK);

	// Resize edit to fill the whole view. 
   // OnSize can be called before OnInitialUpdate 
   // so make sure the edit control has been created. 
   if (::IsWindow(m_ChatEmoBox.GetSafeHwnd()))
   {
		int xEmoBox = cx/15;
		int yEmoBox = cy/15;
		int cxEmoBox = cx * 13 / 15;
		int cyEmoBox = cy * 343 / 600 - 21 / 2;
		m_ChatEmoBox.MoveWindow (xEmoBox, yEmoBox, cxEmoBox, cyEmoBox);
   }

	//CEdit *chatBoxEditCtrl = (CEdit *) GetDlgItem(IDC_EDIT_CHAT_IN);
	//if (chatBoxEditCtrl == NULL)
		//return;
   /*if (::IsWindow(m_chatBoxCtrl.GetSafeHwnd()))
	{
		int xEditBox = cx/15;
		int yEditBox = cy * 413 / 600 - 21 / 2;
		int cxEditBox = cx * 13 / 15;
		int cyEditBox = cy * 49 / 200 - 21 / 2;
		m_chatBoxCtrl.MoveWindow (xEditBox, yEditBox, cxEditBox, cyEditBox);
	}*/

   CButton *buttonCtl = (CButton *) GetDlgItem(IDOK);
	if (buttonCtl == NULL)
		return;

   if (::IsWindow(buttonCtl->GetSafeHwnd()))
   {
		int xButton = cx*14/15 - 80;
		int yButton = cy * 29 / 30 - 21;
		int cxButton = 80;
		int cyButton = 25;
		buttonCtl->MoveWindow (xButton, yButton, cxButton, cyButton);
   }
}


void CTestEmoCustomControlDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x = 480;
	lpMMI->ptMinTrackSize.y = 270;
	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CTestEmoCustomControlDlg::OnBnClickedEmoPop()
{
	// TODO: Add your control notification handler code here
	// modeless dialog creation ref: http://www.codeproject.com/Articles/1651/Tutorial-Modeless-Dialogs-with-MFC
	if (m_emoListPopUpDlg == NULL) {
		m_emoListPopUpDlg = new CEmoListPopUp();
		m_emoListPopUpDlg->Create(CEmoListPopUp::IDD);
		m_emoListPopUpDlg->ShowWindow(SW_SHOW);
	}
	/*else {
		delete m_emoListPopUpDlg;
		m_emoListPopUpDlg = NULL;
	}*/

}


void CTestEmoCustomControlDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	// destroy emo pop up
	if (m_emoListPopUpDlg) {
		// m_emoListPopUpDlg->ShowWindow(SW_HIDE);
		delete m_emoListPopUpDlg;
		m_emoListPopUpDlg = NULL;
	}
}

// ref: http://msdn.microsoft.com/en-us/library/1f31c334(v=vs.90).aspx
void CTestEmoCustomControlDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	// messages ref: http://msdn.microsoft.com/en-us/library/windows/desktop/ms646274(v=vs.85).aspx
	if (nState==WA_CLICKACTIVE && m_emoListPopUpDlg) {
		// m_emoListPopUpDlg->ShowWindow(SW_HIDE);
		delete m_emoListPopUpDlg;
		m_emoListPopUpDlg = NULL;
	}
}

/*BOOL CTestEmoCustomControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (NULL != m_pTooltip)
        m_pTooltip->RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}*/
