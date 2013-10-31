#pragma once
#include "afxcmn.h"
#include "afxwin.h"
//#include "xSkinButton.h"
//#include "CDialogSK.h"	// for custom dialog sk
#include "TestEmoCustomControlDlg.h"	// for referene to this dialog

//class CxSkinButton;
const int EMO_MAX_NO = 47;

// CEmoListPopUp dialog
//class CEmoListPopUp : public CDialogEx
class CEmoListPopUp : public CDialogEx
{
	DECLARE_DYNAMIC(CEmoListPopUp)

public:
	CEmoListPopUp(CTestEmoCustomControlDlg* pDlg, CWnd* pParent = NULL);   // standard constructor
	virtual ~CEmoListPopUp();
	//CTestEmoCustomControlDlg& m_rChatBoxDlg;
	CTestEmoCustomControlDlg* m_rChatBoxDlg;

	// Dialog Data
	enum { IDD = IDD_DIALOG_EMOPOP };

protected:
	//LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonEmoClicked(UINT nID);


private:
	// we do not need pointer if we use CBitmapButton instead of CxSkinButton
	CBitmapButton* m_ButtonEmo[EMO_MAX_NO];
	CToolTipCtrl* m_pToolTipCtrl;
	int selectedEmoIndex;
	//CBrush m_brush;
//public:
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//virtual void PostNcDestroy();
	virtual void OnCancel();
	virtual void OnOK();
};

//LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
