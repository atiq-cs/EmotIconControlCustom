#pragma once
#include "afxcmn.h"
#include "afxwin.h"
//#include "xSkinButton.h"
//#include "CDialogSK.h"	// for custom dialog sk

//class CxSkinButton;
const int EMO_MAX_NO = 47;

// CEmoListPopUp dialog
class CEmoListPopUp : public CDialogEx
{
	DECLARE_DYNAMIC(CEmoListPopUp)

public:
	CEmoListPopUp(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEmoListPopUp();

// Dialog Data
	enum { IDD = IDD_DIALOG_EMOPOP };

protected:
	//LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);

private:
	// we do not need pointer if we use CBitmapButton instead of CxSkinButton
	CBitmapButton m_ButtonEmo[EMO_MAX_NO];
	CToolTipCtrl* m_pToolTipCtrl;

	//CImageList* m_pImage;
	//CButton m_ButtonEmo_01;
	// will not work for forward declaration
	// CxSkinButton* m_ButtonEmo[EMO_MAX_NO];
	//CxSkinButton m_ButtonEmo_01;
	//CButton m_ButtonEmo_01;
	// test button for tooltip
	//CxSkinButton m_ButtonTest;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);