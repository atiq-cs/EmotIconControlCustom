
// TestEmoCustomControlDlg.h : header file
//

#pragma once
#include "chatcontrol.h"
#include "chatedit.h"
#include "afxext.h"
#include "afxwin.h"

class CEmoListPopUp;		// partial declaration
const int EMO_MAX_NO = 47;

// CTestEmoCustomControlDlg dialog
class CTestEmoCustomControlDlg : public CDialog
{
// private members here
private:
	CChatControl m_ChatEmoBox;
	CChatEdit m_chatBoxEditCtrl;
	CEmoListPopUp *m_emoListPopUpDlg;
	CBitmapButton emoPopUpBmp;

	BOOL m_bDialogDestroyed;		// indicates whether emo pop up dialog is destroyed by On Ok or On Cancel
		// Emo code
	TCHAR *EmoCodes[EMO_MAX_NO];
	TCHAR *EmoToolTipText[EMO_MAX_NO];
	// Tool-tip


// Construction
public:
	CTestEmoCustomControlDlg(CWnd* pParent = NULL);	// standard constructor
	~CTestEmoCustomControlDlg();
	void DestroyEmoPopUpDlg(bool CalledFromEmoClass = false);
	// insert emot icons
	void InsertEmoCode(int emoCodeIndex);
	CString GetEmoToolTipText(int index);

// Dialog Data
	enum { IDD = IDD_TESTEMOCUSTOMCONTROL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedEmoPop();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//private:
	//CToolTipCtrl* m_pTooltip;
};
