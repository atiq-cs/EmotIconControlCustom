// EmoListPopUp.cpp : implementation file
//

#include "stdafx.h"
#include "TestEmoCustomControl.h"
#include "EmoListPopUp.h"
//#include "xSkinButton.h"
//#include "afxdialogex.h"


// CEmoListPopUp dialog
IMPLEMENT_DYNAMIC(CEmoListPopUp, CDialogEx)

CEmoListPopUp::CEmoListPopUp(CTestEmoCustomControlDlg* pDlg, CWnd* pParent)
	: CDialogEx(CEmoListPopUp::IDD, pParent),
	m_rChatBoxDlg(pDlg),
	selectedEmoIndex(-1)
{
	for (int i=0; i<EMO_MAX_NO; i++)
		m_ButtonEmo[i] = new CBitmapButton();

	for (int i=0; i<EMO_MAX_NO; i++)
	if (! m_ButtonEmo[i]->LoadBitmaps(IDB_BMP_EMOTICON_01+i, IDB_BMP_EMOTICON_01+i, IDB_BMP_EMOTICON_01+i, IDB_BMP_EMOTICON_01+i)) {
		AfxMessageBox(_T("Failed to load bitmaps for buttons\n"));
		AfxThrowResourceException();
	}
}

CEmoListPopUp::~CEmoListPopUp()
{
	for (int i=0; i<EMO_MAX_NO; i++)
		delete m_ButtonEmo[i];
	// Uninstall the WH_GETMESSAGE hook function.
     //VERIFY (::UnhookWindowsHookEx (hHook));

	delete m_pToolTipCtrl;
}

void CEmoListPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i=0; i<EMO_MAX_NO; i++)
		//DDX_Control(pDX, IDC_BUTTON_EMO01+i, m_ButtonEmo[i]);
		DDX_Control(pDX, IDC_BUTTON_EMO01+i, *m_ButtonEmo[i]);

	//DDX_Control(pDX, IDC_BUTTON_TEST, m_ButtonTest);
}

BOOL CEmoListPopUp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pToolTipCtrl = new CToolTipCtrl();
	if(!m_pToolTipCtrl->Create(this, TTS_ALWAYSTIP)) {
           TRACE("Unable To create ToolTip\n");
           return TRUE;
    }

	// Emo code
	TCHAR *EmoCodes[EMO_MAX_NO] = {_T(":)"), _T(":("), _T(":D"), _T(":P"), _T(":S"), _T(":O"), _T(":@"), _T(":-["), _T(":-]"), _T(":|"),
				_T(":'("), _T(":>"), _T(":3"), _T(":*"), _T(":V"), _T(":/"), _T(";)"), _T(">:("), _T(">:O"), _T("8)"), _T("8|"), _T("O.o"),
				_T("O:)"), _T("3:)"), _T("L(\")"), _T("L3"), _T(":))"), _T(":Z"), _T(":POOP"), _T(":$"), _T(":0"), _T("::3"), _T(":4"),
				_T(":-h"), _T(":6"), _T(":8"), _T(":9"), _T(":12"), _T(":-?"), _T("(Y)"), _T(":54"), _T(":56"), _T(":60"), _T(":67"),
				_T(":72"), _T(":88"), _T("=((")};
	// Tool-tip
	TCHAR *EmoToolTipText[EMO_MAX_NO] = {_T("Smile"), _T("Sad"), _T("Laugh"), _T("Cheeky"), _T("Worried"), _T("Surprised"), _T("Angry"), _T("Love"), _T("Vampire"), _T("Straight Face"),
				_T("Cry"), _T("Blush"), _T("Curly Lips"), _T("Kiss"), _T("Pacman "), _T("Uncertain"), _T("Wink"), _T("Grumpy"), _T("Upset"), _T("Glass"), _T("Sunglass"), _T("Confused"),
				_T("Angel"), _T("Devil"), _T("Penguin"), _T("Heart"), _T("Laugh"), _T("Sleepy"), _T("Poop"), _T("Embarassed"), _T("Leaps are sealed"), _T("Tear on on eye"), _T("Giggle"),
				_T("Wave hands"), _T("Hammer"), _T("Pig"), _T("Rose"), _T("Hush"), _T("Thinking"), _T("Thumbs up"), _T("Thumbs down"), _T("Victory"), _T("Birthday Cake"), _T("Tea"),
				_T("Fist"), _T("Bored"), _T("Broken Heart")};

	for (int i=0; i<EMO_MAX_NO; i++) {
		// doesn't work without PreTranslateMessage
		//if (m_pToolTipCtrl->AddTool(&m_ButtonEmo[i],CString(EmoToolTipText[i]) + _T("   ") + CString(EmoCodes[i])))
		if (m_pToolTipCtrl->AddTool(m_ButtonEmo[i],CString(EmoToolTipText[i]) + _T("   ") + CString(EmoCodes[i])))
			TRACE("Unable to add Dialog to the tooltip\n");
	}

	m_pToolTipCtrl->Activate(TRUE);

	/*for (int i=0; i<EMO_MAX_NO; i++) {
		m_ButtonEmo[i]->SetEnableText(0);
		m_ButtonEmo[i]->SetToolTipText(CString(EmoToolTipText[i]) + _T("   ") + CString(EmoCodes[i]));
		m_ButtonEmo[i]->SetSkin(IDB_BMP_EMOTICON_01+i,IDB_BMP_EMOTICON_01+i,IDB_BMP_EMOTICON_01+i,0,IDB_BMP_EMOTICON_01+i,0,0,0,0);
	}

	/*m_ButtonEmo.SetEnableText(0);
	m_ButtonEmo.SetToolTipText(_T("3"));
	m_ButtonEmo.SetSkin(IDB_BMP_EMOTICON_01,IDB_BMP_EMOTICON_01,IDB_BMP_EMOTICON_01,0,IDB_BMP_EMOTICON_01,0,0,0,0);

	//if (m_ButtonEmo->GetBitmap() == NULL)
		//m_ButtonEmo->SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_BMP_EMOTICON_01)));

	SetTransparentColor(RGB(0xFF, 0xFF, 0xFF));
	EnableEasyMove(FALSE);*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(CEmoListPopUp, CDialogEx)
	// ON_WM_KILLFOCUS()
	//ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	// ref: http://msdn.microsoft.com/en-us/library/84xtde24.aspx
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_EMO01, IDC_BUTTON_EMO47, &OnButtonEmoClicked)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CEmoListPopUp message handlers
void CEmoListPopUp::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	/*// TODO: Add your message handler code here
	// AfxMessageBox(_T("test"), MB_OK);
	ShowWindow(SW_HIDE);
	//OnCancel();
	delete this;*/
}

// ref: http://support.microsoft.com/kb/141758
BOOL CEmoListPopUp::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (NULL != m_pToolTipCtrl)
            m_pToolTipCtrl->RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CEmoListPopUp::OnEraseBkgnd(CDC* pDC)
{
    CRect rect;
    GetClientRect(&rect);
    CBrush myBrush(RGB(0xFF, 0xFF, 0xFF));    // dialog background color
    CBrush *pOld = pDC->SelectObject(&myBrush);
    BOOL bRes  = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
    pDC->SelectObject(pOld);    // restore old brush
    return bRes;                       // CDialog::OnEraseBkgnd(pDC);
}

void CEmoListPopUp::OnButtonEmoClicked(UINT nID) {
	int nButton = nID - IDC_BUTTON_EMO01;
	selectedEmoIndex = nButton;
	/*CString dbg;
	dbg.Format(_T("got click on button %d"), nButton);
	AfxMessageBox(dbg);*/
}


/*void CEmoListPopUp::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	// messages ref: http://msdn.microsoft.com/en-us/library/windows/desktop/ms646274(v=vs.85).aspx
	if (nState==WA_CLICKACTIVE) {
		AfxMessageBox(_T("we are in emo pop up dlg"));
	}
}

// PostNcDestroy not called on destruction
void CEmoListPopUp::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	//m_rChatBoxDlg->DestroyEmoPopUpDlg();

	// AfxMessageBox(_T("Post nc called"));
	m_rChatBoxDlg->DestroyEmoPopUpDlg(true);

	CDialogEx::PostNcDestroy();
}
*/


void CEmoListPopUp::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	m_rChatBoxDlg->DestroyEmoPopUpDlg(true);
	CDialogEx::OnCancel();
}


void CEmoListPopUp::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	// ref: http://msdn.microsoft.com/en-us/library/vstudio/66b3y2ab.aspx
	// no button is selected
	// if a button is selected, save its index and destroy the dialog
	if (selectedEmoIndex >= 0) {
		m_rChatBoxDlg->InsertEmoCode(selectedEmoIndex);
		m_rChatBoxDlg->DestroyEmoPopUpDlg(true);
		CDialogEx::OnOK();
	}
}
