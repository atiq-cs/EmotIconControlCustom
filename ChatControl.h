// CChatControl.h : header file for custom chat control
// We derive from CWnd
// We had an alternative which is CStatic
// CStatic has some features like acquiring bitmap which are not required for our purpose details: http://msdn.microsoft.com/en-us/library/t98kd6f7(v=vs.110).aspx 
// Chris Maunder, founder of codeproject has created a custom control, the article link: http://www.codeproject.com/Articles/521/Creating-Custom-Controls
// our currently followed reference here: http://www.codeproject.com/Articles/5032/Creating-and-Using-custom-controls-in-VC

#pragma once
#include "afxwin.h"
#include "resource.h"		// for IDC_CHATCUSTOM
#define CUSTOMWNDCLASS TEXT("CChatControl")


class CTestEmoCustomControlDlg;
// sent status updates
enum MESSAGE_SEND_STATUS { Sent, Pending, Delivered };

// item field type, type for members of chat items
// we could reuse CHATBOX_ELEMENT_TYPE for CChatUIPainter but we may need our own in future if there is structural change to the declation of CHATBOX_ELEMENT
enum CHATBOX_FIELD_TYPE { FieldDateType, FieldNameType, FieldMessageType, FieldTimeType };

// element type
enum CHATBOX_ELEMENT_TYPE { ElemDateType, ElemNameType, ElemMessageType, ElemEmotIconType, ElemTimeType };

// Replace vector when random access is required or we encounter a problem with CList which deduces CList as buggy
// structure for maintaining chat items
// Single record definition
typedef struct CHATBOX_ITEM {
	MESSAGE_SEND_STATUS send_status;		// SENT, PENDING, DELIVERED
	CString message;							// message data
	//CTime	msgTimeDate;						// for keeping date and time of last sent message
	CString	time;
	CString	date;
	CString user_name;
	// CString user_image_path;		 not used
} CHATBOX_ITEM;

// structure for maintaining paint UI elements
typedef struct CHATBOX_ELEMENT {
	CHATBOX_ELEMENT_TYPE type;		// SENT, PENDING, DELIVERED
	CString text;					// message data, date, time, emo index only in case of emo
	CPoint ptStart;
	CSize size;
	int recordIndex;			// index on chat records, to facilitate a mapping
} CHATBOX_ELEMENT;

/* // structure for maintaining backup data for easy data communication between CChatControl and CChatUIPainter
typedef struct CHATBOX_SHARED_DATA {
	CString date;
	CString name;
	CPoint ptStart;
} CHATBOX_SHARED_DATA;*/


class CChatControl :
	public CWnd
{
private:
		// Dialog Data
	enum { IDD = IDC_CHATCUSTOM };
	// CHATBOX_ITEM m_currentChatItem;
	CList<CHATBOX_ITEM, CHATBOX_ITEM&> chatRecords;
	CList<CHATBOX_ELEMENT, CHATBOX_ELEMENT&> chatUIElements;

	// CList<CRect, CRect&> invalideRectList;
	
	int nMaxScreen;
	int yItem;
	int yClient;		// height of client rectangle
	int nMaxLines;
	int nMaxScroll;
	int yScrollAmount;
	int cyScrollUnit;

	int cyChatItem;

	//int startUIElemIndex;
	//int endUIElemIndex;
	
	int cxChatText;		// defines x from where time will be painted from
	int cxChatTextHSpace;

	CFont dateFont;
	CFont headingFont;
	CFont textFont;
	CFont timeFont;

	// font heights
	int yCharDate;
	int yCharHeading;
	int yCharChatText;
	int yCharTime;


	// CSize m_clientSize;
	int timeWidth;
	bool scrollbarEnabled;
	// int lastSBACtion;
	// CHATBOX_SHARED_DATA lastPaintElement;

	// UI members and properties
	//string and variables
	CPoint ptStart;		// draw from this point
	CPoint ptClipStart;	// left top point of clip rectangle
	CPoint ptEnd;		// this is the limiting point

	// DC for painting
	CDC* pDC;
	CFont* pOldFont;

	CString headingText;
	CString dateText;
	CTestEmoCustomControlDlg* m_pMainDlg;
	TCHAR **pEmoCodesList;

	// Private functions
	void OnInitChatControl();

	// for drawing pre-calculation
	int DrawMessageEmo(CString message);
	void DrawChatText(CString str);
	bool VirtualDrawTextMultiLine(CString str);
	void VirtualDrawEmotIcon(int index);
	CString ExtractFittableLineFromStr(const CString str);
	int GetFittablePositionRecursive(const CString str, int iMin, int iMax);
	bool IsFittableInRectangle(const CString gStr, const int index);

	// UI paint functions
	void PaintUIElements();
	int AddChatItemToPaintElements(CHATBOX_ITEM& chatItem);
		// functions to pre-calculate drawing elements
	int AddPaintElement(const CString gStr, CHATBOX_FIELD_TYPE strType);
	void PaintElements();
	BOOL RegisterWndClass();
	int FindEmoCode(int startIndex, CString str, int* foundEmoIndex);

public:
	CChatControl(CTestEmoCustomControlDlg* pDlg);
	~CChatControl(void);


	// not necessary
	//virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	void PostChatMessage(CString chat_message, CTime timedate);


	// Implementation
protected:
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnPaint( );
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	// afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

// Other functions which are good without sharing data from the class
bool IsPointInsideClipRectangle(const CPoint topLeft, const CPoint bottomRight, const CPoint point);
