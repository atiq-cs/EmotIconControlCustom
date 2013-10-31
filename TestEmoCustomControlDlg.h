// TestEmoCustomControlDlg.h : header file
//

#pragma once
#include "chatcontrol.h"
#include "chatedit.h"
#include "afxext.h"
#include "afxwin.h"

class CEmoListPopUp;		// partial declaration

// CTestEmoCustomControlDlg dialog
class CTestEmoCustomControlDlg : public CDialog
{
// private members here
private:
	CChatControl m_ChatEmoBox;
	CChatEdit m_chatBoxCtrl;
	CEmoListPopUp *m_emoListPopUpDlg;
	CBitmapButton emoPopUpBmp;

	BOOL m_bDialogDestroyed;		// indicates whether emo pop up dialog is destroyed by On Ok or On Cancel

// Construction
public:
	CTestEmoCustomControlDlg(CWnd* pParent = NULL);	// standard constructor
	~CTestEmoCustomControlDlg();
	void DestroyEmoPopUpDlg(bool CalledFromEmoClass = false);
	void InsertEmoCode(int emoCodeIndex);
	// insert emot icons

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
