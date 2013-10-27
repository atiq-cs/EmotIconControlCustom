// EmoListPopUp.cpp : implementation file
//

#include "stdafx.h"
#include "TestEmoCustomControl.h"
#include "EmoListPopUp.h"
//#include "xSkinButton.h"
//#include "afxdialogex.h"


// CEmoListPopUp dialog
IMPLEMENT_DYNAMIC(CEmoListPopUp, CDialogEx)

CEmoListPopUp::CEmoListPopUp(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEmoListPopUp::IDD, pParent),
	//hHook(NULL),
	m_pToolTipCtrl(NULL)
{
	//for (int i=0; i<EMO_MAX_NO; i++)
		//m_ButtonEmo[i] = new CxSkinButton();
	//m_ButtonEmo = new CButton();
	for (int i=0; i<EMO_MAX_NO; i++)
	if (! m_ButtonEmo[i].LoadBitmaps(IDB_BMP_EMOTICON_01+i, IDB_BMP_EMOTICON_01+i, IDB_BMP_EMOTICON_01+i, IDB_BMP_EMOTICON_01+i)) {
		AfxMessageBox(_T("Failed to load bitmaps for buttons\n"));
		AfxThrowResourceException();
	}
}

CEmoListPopUp::~CEmoListPopUp()
{
	//for (int i=0; i<EMO_MAX_NO; i++)
		//delete m_ButtonEmo[i];
	// Uninstall the WH_GETMESSAGE hook function.
     //VERIFY (::UnhookWindowsHookEx (hHook));

	delete m_pToolTipCtrl;
}

void CEmoListPopUp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i=0; i<EMO_MAX_NO; i++)
		DDX_Control(pDX, IDC_BUTTON_EMO01+i, m_ButtonEmo[i]);

	//DDX_Control(pDX, IDC_BUTTON_TEST, m_ButtonTest);
}


/*
// Hook procedure for WH_GETMESSAGE hook type.
LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // Switch the module state for the correct handle to be used.
    AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

    // If this is a keystrokes message, translate it in controls'
    // PreTranslateMessage().
    LPMSG lpMsg = (LPMSG) lParam;
    if( (nCode >= 0) &&
        PM_REMOVE == wParam &&
        (lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST) &&
        AfxGetApp()->PreTranslateMessage((LPMSG)lParam) )
        {
        // The value returned from this hookproc is ignored, and it cannot
        // be used to tell Windows the message has been handled. To avoid
        // further processing, convert the message to WM_NULL before
        // returning.
        lpMsg->message = WM_NULL;
        lpMsg->lParam = 0L;
        lpMsg->wParam = 0;
        }

    // Passes the hook information to the next hook procedure in
    // the current hook chain.
    return ::CallNextHookEx(hHook, nCode, wParam, lParam);
}*/


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
				_T(":'("), _T(":>"), _T(":3"), _T(":*"), _T(":V"), _T(":/"), _T(";)"), _T(">:("), _T(">:O"), _T("8)"), _T("8|"), _T("O.o"), _T("O:)"), _T("3:)"),
				_T("L(\")"), _T("L3"), _T(":))"), _T(":Z"), _T(":POOP"), _T(":$"), _T(":0"), _T("::3"), _T(":4"), _T(":-h"), _T(":6"), _T(":8"), _T(":9"), _T(":12"),
				_T(":-?"), _T("(Y)"), _T(":54"), _T(":56"), _T(":60"), _T(":67"), _T(":72"), _T(":88"), _T("=((")};
	// Tool-tip
	TCHAR *EmoToolTipText[EMO_MAX_NO] = {_T("Smile"), _T("Sad"), _T("Laugh"), _T("Cheeky"), _T("Worried"), _T("Surprised"), _T("Angry"), _T("Love"), _T("Vampire"), _T("Straight Face"),
				_T("Cry"), _T("Blush"), _T("Curly Lips"), _T("Kiss"), _T("Pacman "), _T("Uncertain"), _T("Wink"), _T("Grumpy"), _T("Upset"), _T("Glass"), _T("Sunglass"), _T("Confused"), _T("Angel"), _T("Devil"),
				_T("Penguin"), _T("Heart"), _T("Laugh"), _T("Sleepy"), _T("Poop"), _T("Embarassed"), _T("Leaps are sealed"), _T("Tear on on eye"), _T("Giggle"), _T("Wave hands"), _T("Hammer"), _T("Pig"), _T("Rose"), _T("Hush"),
				_T("Thinking"), _T("Thumbs up"), _T("Thumbs down"), _T("Victory"), _T("Birthday Cake"), _T("Tea"), _T("Fist"), _T("Bored"), _T("Broken Heart")};

	for (int i=0; i<EMO_MAX_NO; i++) {
		if (m_pToolTipCtrl->AddTool(&m_ButtonEmo[i],CString(EmoToolTipText[i]) + _T("   ") + CString(EmoCodes[i])))
			TRACE("Unable to add Dialog to the tooltip\n");
	}
	// if (m_pToolTipCtrl->AddTool(&m_ButtonEmo[1], _T("Sad :(")))
		 // TRACE("Unable to add Dialog to the tooltip\n");

	/*// Install the WH_GETMESSAGE hook function.
      hHook = ::SetWindowsHookEx(
         WH_GETMESSAGE,
         GetMessageProc,
         AfxGetInstanceHandle(),
         GetCurrentThreadId());
      ASSERT (hHook);*/

	m_pToolTipCtrl->Activate(TRUE);
	//m_pToolTipCtrl->Pop();

	/*for (int i=0; i<EMO_MAX_NO; i++) {
		m_ButtonEmo[i]->SetEnableText(0);
		m_ButtonEmo[i]->SetToolTipText(_T("3"));
		m_ButtonEmo[i]->SetSkin(IDB_BMP_EMOTICON_01+i,IDB_BMP_EMOTICON_01+i,IDB_BMP_EMOTICON_01+i,0,IDB_BMP_EMOTICON_01+i,0,0,0,0);
	}
	/*m_ButtonTest.SetEnableText(0);0
	m_ButtonTest.SetToolTipText(_T("3"));
	m_ButtonTest.SetSkin(IDB_BMP_EMOTICON_01,IDB_BMP_EMOTICON_01,IDB_BMP_EMOTICON_01,0,IDB_BMP_EMOTICON_01,0,0,0,0);

	/*m_ButtonEmo.SetEnableText(0);
	m_ButtonEmo.SetToolTipText(_T("3"));
	m_ButtonEmo.SetSkin(IDB_BMP_EMOTICON_01,IDB_BMP_EMOTICON_01,IDB_BMP_EMOTICON_01,0,IDB_BMP_EMOTICON_01,0,0,0,0);*/

	//if (m_ButtonEmo->GetBitmap() == NULL)
		//m_ButtonEmo->SetBitmap(::LoadBitmap(NULL, MAKEINTRESOURCE(IDB_BMP_EMOTICON_01)));

	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(CEmoListPopUp, CDialogEx)
	// ON_WM_KILLFOCUS()
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
