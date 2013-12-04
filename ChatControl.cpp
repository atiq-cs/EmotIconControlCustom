// ChatControl.cpp : implementation file for our custom chat control
// We create a new window class for our control
// register it into the system, control window is created automatically by dialog using resource file
//   hence, scrollbar is automatically added to the control
// Implements drawing and chat data structure for it


#include "stdafx.h"
#include "ChatControl.h"
#include "TestEmoCustomControlDlg.h"	// for referene to this dialog
//#include "ChatUIPainter.h"	// for custom drawing operation class
//#include <math.h>		// for ceil

// our contructor
CChatControl::CChatControl(CTestEmoCustomControlDlg* pDlg):
	nMaxScreen(0),
	yItem(0),
	nMaxScroll(0),
	timeWidth(0),
	deliveryStatusWidth(17),
	nMaxLines(0),
	yScrollAmount(0),
	yClient(0),
	cyChatItem(0),
	scrollbarWidth(18),
	scrollbarEnabled(false),
	cyScrollUnit(0),
	// startUIElemIndex(0),
	// endUIElemIndex(-1),
	pDC(NULL),
	pOldFont(NULL),
	dateText(TEXT("")),
	headingText(TEXT("")),
	cxChatText(0),
	cxChatTextHSpace(2),
	m_pMainDlg(pDlg),
	pEmoCodesList(NULL)
	// isAlternateNameColor(false)
	// lastSBACtion(0)
{
	//Register our custom chat control class
	if (RegisterWndClass() == FALSE) {
		AfxMessageBox(_T("Window proc registration failed"), MB_OK);
	}
	yCharDate = yCharHeading = yCharChatText = yCharTime = 0;
}

// delete anything we have manually created/allocated
CChatControl::~CChatControl(void)
{
	// delete fonts objects
	dateFont.DeleteObject();
	headingFont.DeleteObject();
	textFont.DeleteObject();
	timeFont.DeleteObject();
}

// registers the window class for custom control
// without registering control window creation will not succeed
BOOL CChatControl::RegisterWndClass() {
	// Register your unique class name that you wish to use
	// ref: http://msdn.microsoft.com/en-us/library/kcb1w44w.aspx
	WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

	memset(&wndcls, 0, sizeof(WNDCLASS));   // start with NULL defaults

    //Check weather the class is registerd already
    if (!(::GetClassInfo(hInst, CUSTOMWNDCLASS, &wndcls)))
    {
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		//you can specify your own window procedure
		wndcls.lpfnWndProc = ::DefWindowProc; 
		wndcls.hInstance = hInst;
		// set it later, when we have an icon
		//wndcls.hIcon = LoadIcon(wndcls.hInstance, MAKEINTRESOURCE(IDI_MYICON));
		wndcls.hIcon = NULL;
		wndcls.hCursor = LoadCursor(wndcls.hInstance, MAKEINTRESOURCE(IDC_ARROW));
		wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndcls.lpszMenuName = NULL;
		// Specify your own class name for using FindWindow later
		wndcls.lpszClassName = CUSTOMWNDCLASS;

		// Register the new class and trace if it fails 
		if(!AfxRegisterClass(&wndcls))
		{
		   TRACE("Class Registration Failed\n");
		   return FALSE;
		}
	}
    /* ref: http://www.codeproject.com/Articles/5032/Creating-and-Using-custom-controls-in-VC
	WNDCLASS windowclass;

    //Check weather the class is registerd already
    if (!(::GetClassInfo(hInst, CUSTOMWNDCLASS, &windowclass)))
    {
        //If not then we have to register the new class
        windowclass.style = CS_DBLCLKS;// | CS_HREDRAW | CS_VREDRAW;
        windowclass.lpfnWndProc = ::DefWindowProc;
        windowclass.cbClsExtra = windowclass.cbWndExtra = 0;
        windowclass.hInstance = hInst;
        windowclass.hIcon = NULL;
        windowclass.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        windowclass.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);
        windowclass.lpszMenuName = NULL;
        windowclass.lpszClassName = CUSTOMWNDCLASS;

        if (!AfxRegisterClass(&windowclass))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }*/

    return TRUE;
}

BEGIN_MESSAGE_MAP(CChatControl, CWnd)
		// ON_WM_CREATE() doesn't work dialog box creates controls from resource earlier, use PreSubClassWindow instead
		ON_WM_PAINT()
		ON_WM_VSCROLL()
		ON_WM_SIZE()
		ON_WM_MOUSEHWHEEL()
END_MESSAGE_MAP()

/*
OnCreate is not reached as WM_CREATE message is suppressed
int CChatControl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}*/

// ref: http://msdn.microsoft.com/en-us/library/vstudio/8d463726.aspx
void CChatControl::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	OnInitChatControl();

	CWnd::PreSubclassWindow();
}

// Perform initialization before window creation here
void CChatControl::OnInitChatControl() {
	// Create required fonts
	// ref: http://msdn.microsoft.com/en-US/library/94hsa63e(v=vs.110).aspx
	LOGFONT lf;

	// Date font
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 20;                      // request a 16-pixel-height font
	//lf.lfWeight = FW_DEMIBOLD;
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial"), 7);                    // request a face name "Arial"
	VERIFY(dateFont.CreateFontIndirect(&lf));  // create the font 

	// heading font
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 16;                      // request a 16-pixel-height font
	lf.lfWeight = FW_DEMIBOLD;
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, 
	   _T("Times New Roman"), 7);                    // request a face name "Times New Roman"
	VERIFY(headingFont.CreateFontIndirect(&lf));  // create the font 

	// text font
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 14;                      // request a 14-pixel-height font
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, 
	   _T("Verdana"), 7);                    // request a face name "Verdana"
	VERIFY(textFont.CreateFontIndirect(&lf));  // create the font 
	
	// time font
	memset(&lf, 0, sizeof(LOGFONT));       // zero out structure
	lf.lfHeight = 12;                      // request a 12-pixel-height font
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, 
	   _T("Verdana"), 7);                    // request a face name "Verdana"
	VERIFY(timeFont.CreateFontIndirect(&lf));  // create the font

	// Pre-calculate font heights

	// Get the handle to the client area's device context, use GETDC to retrieve outside of WM_Paint
    CDC* pdc = GetDC(); 
	timeWidth = pdc->GetOutputTextExtent(TEXT("11:59:59 PM")).cx;			// ref: http://msdn.microsoft.com/en-us/library/xt679a2s(v=vs.110).aspx
	// quick fix, need to improve the logic
	//deliveryStatusWidth += scrollbarWidth;
    // Extract font dimensions from the text metrics. 
	TEXTMETRIC tm;
	// get height for font 0
	CFont* pOldFont = pdc->SelectObject(&dateFont);
    pdc->GetTextMetrics(&tm);
    // xChar = tm.tmAveCharWidth; 
    // xUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * xChar/2; 
	yCharDate = tm.tmHeight + tm.tmExternalLeading + 4;
	cyScrollUnit = yCharDate;
	// get height for font 1
	pdc->SelectObject(&headingFont);
    pdc->GetTextMetrics(&tm);
	yCharHeading = tm.tmHeight + tm.tmExternalLeading + 4;
 
	// get height for font 2
	pdc->SelectObject(&textFont);
    pdc->GetTextMetrics(&tm);
	yCharChatText = tm.tmHeight + tm.tmExternalLeading+10;

	// get height for font 3
	pdc->SelectObject(&timeFont);
    pdc->GetTextMetrics(&tm); 
	yCharTime = tm.tmHeight + tm.tmExternalLeading; 

	// get clip rectangle fails here, probably control is not created yet

	// restore old font
	pdc->SelectObject(pOldFont);
    // Free the device context. 
    ReleaseDC(pdc);

	// perfect place for setting scroll bar, or use EnableScrollBarCtrl
	// ShowScrollBar(SB_VERT, FALSE);		// if we want the vertical scrollbar to hide explicitly
	/* ref: http://msdn.microsoft.com/en-us/library/windows/desktop/bb787527(v=vs.85).aspx
	Scroll Bar Visibility:
	 The system hides and disables a standard scroll bar when equal minimum and maximum values are specified. The system also hides and disables a standard scroll bar if you specify a page size that
	 includes the entire scroll range of the scroll bar. This is the way to temporarily hide a scroll bar when it is not needed for the content of the client area. There is no need to make scrolling
	 requests through the scroll bar when it is hidden. The system enables the scroll bar and shows it again when you set the minimum and maximum values to unequal values or when the page size that 
	 does not include the entire scroll range. The ShowScrollBar function can also be used to hide or show a scroll bar. It does not affect the scroll bar's range, page size, or scroll box position. 

	 The EnableScrollBar function can be used to disable one or both arrows of a scroll bar. An application displays disabled arrows in gray and does not respond to user input. */
	// no lines in drawn in control initially, nothing to scroll
	SetScrollRange(SB_VERT, 0, 0, FALSE);
	
	//CTime timeDate = CTime::GetCurrentTime();
	//PostChatMessage(_T("lots of text"), timeDate);
	//m_chatBoxCtrl.SetWindowText(_T(""));
	//GotoDlgCtrl(&m_chatBoxCtrl);		// ref: http://blogs.msdn.com/b/oldnewthing/archive/2004/08/02/205624.aspx
}

// Paint to draw the chat box
// to do tasks
// 1. has to set transparent
void CChatControl::OnPaint() {
	// avoid paint when record is inserted
	if (chatUIElements.GetCount() == 0) {
		// Dummy code to unblock tool-tip for all controls in the dialog
		PAINTSTRUCT ps;
		pDC = BeginPaint(&ps);
		pDC->TextOut(0, 0, _T(" "));
		EndPaint(&ps);
		pDC = NULL;
		return;
	}

	if (pDC)
		return ;

	if (ptEnd == CPoint(0,0))
		return;

	PAINTSTRUCT ps;
	pDC = BeginPaint(&ps);
	// initialization which were previously done in ChatUIPainter
	// get clip rectangle and save as points
	/*CRect rcClip;
	pDC->GetClipBox(&rcClip);
	ptClipStart = rcClip.TopLeft();		// draw from this point

	ptEnd = rcClip.BottomRight();		// this is the limiting point
	// ptEnd is only for chat text (not datetime or heading text)
	ptEnd.x -= timeWidth;		// for field type Time don't use ptEnd
	*/
	// updated in PostchatMessage, no need to update on each paint
	// cxChatText = rcClip.Width() -  timeWidth;		// for field type Time use this instead

	// CChatUIPainter m_clsUIPainter(pDC, chatUIElements,  yScrollAmount, timeWidth, lastPaintElement);
	//m_clsUIPainter.SetFonts(m_fontArray, m_fontHeight);
	//m_clsUIPainter.PaintElements();
	PaintUIElements();

	// restore font
	if (pOldFont) {
		pDC->SelectObject(&pOldFont);
		pOldFont = NULL;
	}

	if (pDC)
		pDC = NULL;

	EndPaint(&ps);
}

// in case user resizes the window, fix page size
void CChatControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

// MFC scrolling implementation example ref: http://msdn.microsoft.com/en-us/library/e14hhbe6.aspx
void CChatControl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int CurPos = GetScrollPos(SB_VERT);
	int yPos = CurPos;
	// lastSBACtion = nSBCode;

	switch (nSBCode) {
		// User clicked the HOME keyboard key.
        case SB_TOP:
            CurPos = 0;
            break;

        // User clicked the END keyboard key.
        case SB_BOTTOM:
            CurPos = nMaxScroll;
            break;

        // User clicked the top arrow.
        case SB_LINEUP:
            CurPos = max(0, CurPos-1);

            break;

        // User clicked the bottom arrow.
        case SB_LINEDOWN:
			CurPos = min(nMaxScroll, CurPos+1);
            break;

        // User clicked the scroll bar shaft above the scroll box.
        case SB_PAGEUP:
			{
            // Get the page size
			SCROLLINFO   si;
			GetScrollInfo(SB_VERT, &si);
   
			CurPos = max(0, CurPos - (int) si.nPage);
			// si.nPos -= si.nPage;
            break;
              
			}
        // User clicked the scroll bar shaft below the scroll box.+-
        case SB_PAGEDOWN:
			{
            // Get the page size. 
			SCROLLINFO   si;
			GetScrollInfo(SB_VERT, &si);
   			CurPos = min(nMaxScroll, CurPos + (int) si.nPage);
            break;
            // si.nPos += si.nPage;
			}
        // User dragged the scroll box.
        case SB_THUMBTRACK:
            CurPos = nPos;
			// si.nPos = si.nTrackPos;
            break;

        default:
			break; 
	}


	if (CurPos != yPos) {
		SetScrollPos(SB_VERT, CurPos);
		// ref: http://msdn.microsoft.com/en-us/library/bdea5dt6(v=vs.110).aspx
		// yAmount: Specifies the amount, in device units, of vertical scrolling. This parameter must be a negative value to scroll up.
		// differs 284 - 241

		// scrolling to other position from Max Scroll position
		// we need to 
		//  && CurPos == nMaxScroll-1
		int scrollY = 0;
		if (yPos==nMaxScroll) {
			scrollY =  (yItem-yClient) % cyScrollUnit-cyScrollUnit;
		}
		else if (CurPos == nMaxScroll) {
			scrollY =  cyScrollUnit-(yItem-yClient) % cyScrollUnit;
		}
		scrollY += cyScrollUnit * (yPos-CurPos);
		yScrollAmount -= scrollY;
		ScrollWindow(0, scrollY);

		/*{
			//yScrollAmount = 0;
			// adjust extra scrolling
			int scrollY = yScrollAmount % cyScrollUnit;
			yScrollAmount -= scrollY;
	        ScrollWindow(0, scrollY);
			// Invalidate();
		}
		else if (yPos==nMaxScroll-1 && CurPos == nMaxScroll) {
		}*/
		// else {
			// scrollY += cyScrollUnit * (yPos-CurPos);
			// yScrollAmount -= ;
			// yScrollAmount -= scrollY;

			// ScrollWindow(0, cyScrollUnit * (yPos-CurPos));
			// ScrollWindow(0, scrollY);
		// Invalidate();

		/*if (invalideRectList.IsEmpty() == false) {
			POSITION pos = invalideRectList.GetHeadPosition();
			int i = 0;
			// ** implement drawing according to co-ordinates of update region
			//  && endUIElemIndex == -1
			for (; i < invalideRectList.GetCount(); i++)
			{
				CRect invalidRect = (CRect) invalideRectList.GetNext(pos);
				invalidRect.top += cyScrollUnit * (yPos-CurPos);
				invalidRect.bottom += cyScrollUnit * (yPos-CurPos);
				InvalidateRect(invalidRect);
			}
			invalideRectList.RemoveAll();
		}*/
		UpdateWindow ();
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


// post chat message to display on custom chat box
// this function has the responsibility to set everything up
// so that next OnPaint can easily draw elements and scrollbar with appropriate positions
void CChatControl::PostChatMessage(CString chat_message, CTime timedate) {
	CHATBOX_ITEM m_currentChatItem = {};
	m_currentChatItem.message = chat_message;
	//m_currentChatItem.date = timedate.Format(_T("%B %d, %Y"));
	m_currentChatItem.date = TEXT("TODAY");

	// Set a random user name
	/* Seed the random-number generator with current time so that
    * the numbers will be different every time we run.
	* ref: http://msdn.microsoft.com/en-us/library/aa272875(v=VS.60).aspx
    */
    srand( (unsigned)time( NULL ) );
	bool isRecievedFromNetwork = (bool) (((int) rand()) % 2);
	if (isRecievedFromNetwork) {
		m_currentChatItem.user_name = TEXT("Client 01");
		m_currentChatItem.time = timedate.Format(_T("%I:%M:%S %p"));	// ref: http://msdn.microsoft.com/en-us/library/29btb3sw.aspx & http://msdn.microsoft.com/en-us/library/fe06s4ak.aspx
	}
	else {
		m_currentChatItem.user_name = TEXT("Client 02");

		// Convert the time to GMT/UTC
		CTimeSpan tz(0, 6, 0, 0);	// for timezone
		timedate -= tz;

		// convert time to local now; ref: http://support.microsoft.com/kb/245786
		FILETIME FileTime, LocalFileTime;
		SYSTEMTIME UTCTime, LocalTime;
		timedate.GetAsSystemTime(UTCTime);	// ref: http://msdn.microsoft.com/en-US/library/hcc89tce(v=vs.110).aspx
		SystemTimeToFileTime(&UTCTime,&FileTime);
		FileTimeToLocalFileTime(&FileTime, &LocalFileTime);
		FileTimeToSystemTime(&LocalFileTime, &LocalTime);
		timedate = CTime(LocalTime);

		// to simulate out of order timestamp
		int randOffset = ((int) rand()) % 3600;
		int minOffest = randOffset / 60;
		int secOffset = randOffset % 60;
		CTimeSpan tzRev(0, 0, minOffest, secOffset);	// for timezone
		timedate -= tzRev;

		//  

		// randomize time, expect time as gmt as well
		m_currentChatItem.time = timedate.Format(_T("%I:%M:%S %p"));	// ref: http://msdn.microsoft.com/en-us/library/29btb3sw.aspx & http://msdn.microsoft.com/en-us/library/fe06s4ak.aspx
	}

	m_currentChatItem.send_status = Trying;
	// Do preparsing here, for example, detecting all emot icons here so that it saves time for onPaint
	chatRecords.AddTail(m_currentChatItem);
	// add the chat item as UI Elements
	int yItemDiff = AddChatItemToPaintElements(m_currentChatItem);

	// pre-calculation
	// get max number of lines fittable in client rect
	// we need this to calculate max scroll position
	if (nMaxScreen == 0) {
		// Get the device context
		CDC* pdc = GetDC();
		CRect rc;
		// gives wrong dimension
		// CSize sz = pDC->GetWindowExt();
		//nMaxScreen = sz.cy / m_fontHeight[2];
		pdc->GetClipBox(&rc);
		yClient = rc.Height();
		nMaxScreen = yClient / cyScrollUnit;
		/*CString dbgStr;
		dbgStr.Format(TEXT("exact mak screen lines = %f"), (double) (rc.Height()) / m_fontHeight[2]);
		AfxMessageBox(dbgStr, MB_OK);*/

		// Free the device context. 
		ReleaseDC(pdc);
	}

	/*int yExtra = (yItem - yClient) % cyScrollUnit;
	nMaxLines = yItem /  cyScrollUnit;
	if (yExtra)
		nMaxLines++;*/

	// enable scrollbar if total height exceeds current client rectangle height for the first time
	// if scrollbar is already enabled
	//  check if we are in maxScrollPos, old one
	//   if yes then we have to scroll automatically 
	//   if no then we have to adjust scrollbar range and page size

	// total height exceeds current client rectangle height
	if (yClient < yItem) {
		// get current scroll position, no need to know this to verify condition for automatic scrolling
		int yScrollPos = 0;
		if (scrollbarEnabled) {
			yScrollPos = GetScrollPos(SB_VERT);
		}

		// set scrollbar
		SCROLLINFO si;
		// Set the vertical scrolling range and page size
		si.cbSize = sizeof(si);
		// automaticlly scroll only if current scroll position is max
		// we use old nMaxScroll to check this
		if (yScrollPos==nMaxScroll) {
			yScrollAmount += yItemDiff;
			ScrollWindow(0, -yItemDiff);
			nMaxScroll = (yItem - yClient) % cyScrollUnit?(yItem - yClient) / cyScrollUnit+1:(yItem - yClient) / cyScrollUnit;
			si.nPos = nMaxScroll;
			si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;
		}
		else {
			si.fMask  = SIF_PAGE | SIF_RANGE;
			nMaxScroll = (yItem - yClient) % cyScrollUnit?(yItem - yClient) / cyScrollUnit+1:(yItem - yClient) / cyScrollUnit;
		}

		si.nMin = 0;
		nMaxLines = nMaxScreen+nMaxScroll;
		si.nMax = nMaxLines-1;
		si.nPage  = nMaxScreen;
		SetScrollInfo(SB_VERT, &si);

		if (scrollbarEnabled==false) {
			yScrollAmount = yItem - yClient;
			// Update paint elements here
			// Invalidate();	// need to redraw entire client as scrollbar is being drawn first time, to get time in correct position
			scrollbarEnabled = true;
			//timeWidth -= 18;
			//deliveryStatusWidth -= scrollbarWidth;
			//ScrollWindow(0, );
			UpdateWindow ();
		}
	}
	else {
		// Invalidate entire client area; client rectangle is redrawn when UpdateWindow is called.
		// improve this
		Invalidate();
		// Update Window to cause View to redraw.
		// how to update only a region
		UpdateWindow();
	}
}

// Update elements for UI
// Each raw element can be output to the display using single textout or drawbitmap call
int CChatControl::AddChatItemToPaintElements(CHATBOX_ITEM& chatItem) {
	// pDC must be null for continueing
	if (pDC)
		return 0;
	// Get the device context
    pDC = GetDC();

	// get clip rectangle and save as points
	// this causes trouble for minimize
	CRect rcClip;
	pDC->GetClipBox(&rcClip);
	ptClipStart = rcClip.TopLeft();		// draw from this point

	ptEnd = rcClip.BottomRight();		// this is the limiting point
	// ptEnd is only for chat text (not datetime or heading text or other elements)
	int resWidth = timeWidth + deliveryStatusWidth;
	if (scrollbarEnabled == false)
		resWidth += scrollbarWidth;
	ptEnd.x -= resWidth;		// for field type Time don't use ptEnd
	cxChatText = ptEnd.x - ptClipStart.x;		// for field type Time use this instead

	int preYItem = yItem;
	yItem += AddPaintElement(chatItem.date, FieldDateType);
	yItem += AddPaintElement(chatItem.user_name, FieldNameType);
	// will add 0 anyway, but this y Offset will be added by AddPaintElement for FieldTimeType
	yItem += AddPaintElement(chatItem.message, FieldMessageType);
	yItem += AddPaintElement(chatItem.send_status);
	yItem += AddPaintElement(chatItem.time, FieldTimeType);

    // Free the device context. 
    ReleaseDC(pDC);
	pDC = NULL;
	return (yItem  - preYItem);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UI Paint functions start
int CChatControl::AddPaintElement(const CString gStr, CHATBOX_FIELD_TYPE strType) {
	// logic derived from drawElement
	switch(strType) {
	case FieldDateType:
	{
		// set date font
		if (pOldFont == NULL)
			pOldFont = pDC->SelectObject(&dateFont);
		else
			pDC->SelectObject(&dateFont);
		int preY = ptStart.y;
		// add space from second time
		if (!dateText.IsEmpty() && dateText != gStr)
			ptStart.y += yCharDate;

		// calculate dimension only if different
		if (dateText != gStr) {
			// get extent of heading Text
			CSize size = pDC->GetOutputTextExtent(gStr);

			// add vertical space between chat items when headingText is different
			dateText = gStr;
			CHATBOX_ELEMENT tmp = { ElemDateType, dateText, ptStart, size };
			chatUIElements.AddTail(tmp);
			ptStart.y += yCharDate;
		}
		return (ptStart.y-preY);
	}
	case FieldNameType:
	{	// set name heading font
		if (pOldFont == NULL)
			pOldFont = pDC->SelectObject(&headingFont);
		else
			pDC->SelectObject(&headingFont);
		int preY = ptStart.y;
		// add space from second time
		if (!headingText.IsEmpty() && headingText != gStr)
			ptStart.y += yCharHeading/2;

		// int res = 0;
		// draw only if different
		if (headingText != gStr) {
			// get extent of heading Text
			CSize size = pDC->GetOutputTextExtent(gStr);

			// add vertical space between chat items when headingText is different
			headingText = gStr;
			CHATBOX_ELEMENT tmp = { ElemNameType, headingText, ptStart, size };
			chatUIElements.AddTail(tmp);
			ptStart.y += yCharHeading;
		}
		return (ptStart.y-preY);
	}
	case FieldMessageType:
	{
		// set chat text font
		pDC->SelectObject(textFont);
		CPoint oldPoint = ptStart;

		// draw text with emo if exists
		// clip width is lessed by date time clip
		// use variable cyChatItem to store the height, have to add after drawing date time
		// DrawMessageEmo does/should not touch draw ending point, it should work with a copy of ptEnd
		cyChatItem = DrawMessageEmo(gStr);
		ptStart = oldPoint;
		return cyChatItem;
	}
	case FieldTimeType:
	{
		// set font for time
		if (pOldFont == NULL)
			pOldFont = pDC->SelectObject(&timeFont);
		else
			pDC->SelectObject(&timeFont);

		// get extent of time Text
		CSize size = pDC->GetOutputTextExtent(gStr);
		// does ChatUIPainter has knowledge on where the starting drawing point of time would be??
		// let' see we have 
		CPoint ptTime = CPoint(cxChatText, ptStart.y);
		CHATBOX_ELEMENT tmp = { ElemTimeType, gStr,  ptTime, size };
		chatUIElements.AddTail(tmp);
		// got it from previous message drawing calculation
		ptStart.y += cyChatItem;
		return 0;
	}
	default:
		return 0;
	}
}

int CChatControl::AddPaintElement(MESSAGE_SEND_STATUS status) {
	// logic derived from drawElement
	// set font not required
	CPoint delivPtStart(cxChatText+timeWidth+1, ptStart.y);
	CHATBOX_ELEMENT tmp = { ElemDeliveryStatusType, TEXT(""), delivPtStart, CPoint(deliveryStatusWidth, deliveryStatusWidth) };
	chatUIElements.AddTail(tmp);

	// not changing ptStart for delivery status print
	//ptStart.x += cch.cx;
	return 0;
}

// helper function for DrawElement
// efficient implementation of emot icon code matching
int CChatControl::DrawMessageEmo(CString message)
{
	if (message.IsEmpty())
		return 0;

	if (pEmoCodesList == NULL) {
		pEmoCodesList = m_pMainDlg->GetEmoCodeList();
	}

	// minimum length of emot icons = 2
	// maximum length of emot icons = 3, one emo of length 5  :POOP
	// starting charset of emot icons ":;>8O3L(="

	CString token;
	int YPre = ptStart.y;
	enum TextEmoType {NONE, BITMAP, TEXTSTR};
	TextEmoType PreviousItemType = NONE;
	int startPos = 0;
	int emoPos = 0;
	int emoIndex = 0;
	while ((emoPos = FindEmoCode(startPos, message, &emoIndex)) >= 0) {
		// emo found, draw now
		// whether there is any text before emot icon
		if (emoPos>startPos) {
			token = message.Mid(startPos, emoPos-startPos);
			if (PreviousItemType == BITMAP)
				ptStart.x += cxChatTextHSpace;
			DrawChatText(token);
			PreviousItemType = TEXTSTR;
		}

		// add vspace before and after emo, to solve spacing problem of text after emo
		if (PreviousItemType == TEXTSTR)
			ptStart.x += cxChatTextHSpace;
		VirtualDrawEmotIcon(emoIndex);
		CString tmpDbg(pEmoCodesList[emoIndex]);
		startPos = emoPos + _tcslen(pEmoCodesList[emoIndex]);
	}
	if ( message.GetLength()-startPos > 0) {
		token = message.Mid(startPos, message.GetLength()-startPos);			// ref: http://msdn.microsoft.com/en-us/library/aa300543(v=vs.60).aspx
		if (PreviousItemType == BITMAP)
			ptStart.x += cxChatTextHSpace;
		DrawChatText(token);
	}
	// draw left over text
	

	/*CString emo = TEXT(":)");
	
	int n = message.Find(emo);		// ref: http://msdn.microsoft.com/en-us/library/aa300543(v=vs.60).aspx
	int startPos = 0;
	//CPoint ptStartDraw = ptStart;		// draw from this point
	// CPoint ptEndDraw = ptEnd;		// this is the limiting point
	// 
	// ptEndDraw.x = ptStart.x + cxChatText;

	// CRect limitRect(rcMsgClip);

	while (n != -1) {
		// get substring before emo
		if (n-startPos > 0) {
			token = message.Mid(startPos, n-startPos);			// ref: http://msdn.microsoft.com/en-us/library/aa300543(v=vs.60).aspx
			// draw text
			//pDC->DrawText(itemText, itemRect, DT_CALCRECT| DT_LEFT | DT_NOCLIP | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			// DrawTextMultiLine will modify ptStartDraw if it draws
			if (PreviousItemType == BITMAP)
				ptStart.x += cxChatTextHSpace;
			DrawChatText(token);
			PreviousItemType = TEXTSTR;
			// pDC->DrawText(token, limitRect, DT_NOPREFIX | DT_WORDBREAK);		// ref: http://msdn.microsoft.com/en-US/library/a6x7y2a4(v=vs.110).aspx
			// limitRect.left += pDC->GetTextExtent(token).cx;		// ref: http://msdn.microsoft.com/en-us/library/z7e878zz(v=vs.110).aspx
		}
		// add vspace before and after emo, to solve spacing problem of text after emo
		if (PreviousItemType == TEXTSTR)
			ptStart.x += cxChatTextHSpace;
		VirtualDrawEmotIcon();

		//ptStartDraw.x += cxChatTextHSpace;
		PreviousItemType = BITMAP;
		startPos = n + emo.GetLength();
		n = message.Find(emo, startPos);
	}

	if ( message.GetLength()-startPos > 0) {
		token = message.Mid(startPos, message.GetLength()-startPos);			// ref: http://msdn.microsoft.com/en-us/library/aa300543(v=vs.60).aspx
		if (PreviousItemType == BITMAP)
			ptStart.x += cxChatTextHSpace;
		DrawChatText(token);
	}*/
	return (ptStart.y - YPre + yCharChatText);		// add Single Line Height to move to next row
}

/*
	Author		:		Saint Atique
				:		returns -1 if not found
*/
int CChatControl::FindEmoCode(int startIndex, CString str, int* foundEmoIndex) {
	TCHAR* startEmoChar = _T(":;>8O3L(=");
	int nStartEmoChar = 9;
	int nEmoCount = m_pMainDlg->GetEmoCount();

	// effect of :))
	// 0 => 26
	// 1 <-> 26 => --

	// effect of :88
	// 35 => 45
	// 36 <-> 45 => 35 <-> 44

	// effect of :60 and :67
	// 34, 35 => 42, 43
	// 36 <-> 43 => 34, 41

	// effect of :POOP
	// 4 => 28
	// 5 <-> 27 => -= 2
	// 28 => --
	// 29 <-> 33 => same position
	// 34, 35 => 42, 43
	// 36 => -= 2
	// 37 => 45
	// 38 <-> 44 => -=3
	// 45 => 44
	// 46 => same position
	static TCHAR *EmoCodeMap[EMO_MAX_NO] = {_T(":))"), _T(":)"), _T(":("), _T(":D"), _T(":POOP"), _T(":P"), _T(":S"), _T(":O"), _T(":@"), _T(":-["), _T(":-]"), _T(":|"),
				_T(":'("), _T(":>"), _T(":3"), _T(":*"), _T(":V"), _T(":/"), _T(";)"), _T(">:("), _T(">:O"), _T("8)"), _T("8|"), _T("O.o"),		// starting index = 12
				_T("O:)"), _T("3:)"), _T("L(\")"), _T("L3"), _T(":Z"), _T(":$"), _T(":0"), _T("::3"), _T(":4"),					// starting index = 24
				_T(":-h"), _T(":60"), _T(":67"), _T(":6"), _T(":88"), _T(":8"), _T(":9"), _T(":12"), _T(":-?"), _T("(Y)"), _T(":54"),			// starting index = 33
				_T(":56"), _T(":72"), _T("=((")};					// starting index = 44

	static int emoCodeMapToIndex[EMO_MAX_NO] = {26, 0, 1, 2, 28, 3, 4, 5, 6, 7, 8 ,9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 27, 29, 30, 31, 32, 33,
				42, 43, 34, 45, 35, 36, 37, 38, 39, 40, 41, 44, 46};

	for (int i=startIndex; i<str.GetLength()-1; i++) {
		int j = 0;
		// match each char with beginning charset
		for (;j<nStartEmoChar; j++)
			if (str[i] == startEmoChar[j])
				break;

		// emo first char match
		if (j < nStartEmoChar) {
			// check for each emo match from current char position
			// get length of emo
			int k=0;
			int emoLength = 0;
			for (; k<nEmoCount; k++) {
				emoLength = _tcslen(EmoCodeMap[k]);
				// should have more or equal number of characters left
				if (emoLength <= str.GetLength()-i) {
					if (CString(EmoCodeMap[k]) == str.Mid(i, emoLength))
						break;
				}
			}
			// emo matched
			if (k<nEmoCount) {
				*foundEmoIndex = emoCodeMapToIndex[k];
				return i;
			}

		}
	}
	*foundEmoIndex = -1;
	return -1;
}

// DrawChatText independent function
// output: output is sent via CPoint* pointer to modify to next starting point of drawing
//		only adds required value to get correct next draw point without adding extra height
//		it should not add extra single line height
void CChatControl::DrawChatText(CString str)
{
	if (str.IsEmpty())
		return ;
	// break for "\r\n", tokenize doesn't work because we lose the count
	int startPos = 0;

	CString newline = TEXT("\r\n");
	int n = str.Find(newline);
	CString tokStr;

	while (n != -1) {
		// get substring before emo
		if (n-startPos > 0) {
			tokStr = str.Mid(startPos, n-startPos);
			VirtualDrawTextMultiLine(tokStr);
		}
		ptStart.y += yCharChatText;
		ptStart.x = cxChatTextHSpace;

		startPos = n + newline.GetLength();
		n = str.Find(newline, startPos);
	}

	if (str.GetLength()-startPos > 0) {
		tokStr = str.Mid(startPos, str.GetLength()-startPos);
		VirtualDrawTextMultiLine(tokStr);
	}
}


// imported from previous project of CListCtrl modification
// msdn example: http://msdn.microsoft.com/en-us/library/kwxzck32(v=vs.110).aspx
// Our functions definitions
// This handler loads a bitmap from system resources, 
// centers it in the view, and uses BitBlt() to paint the bitmap 
// bits. 
void CChatControl::VirtualDrawEmotIcon(int index)
{
	/*// load IDB_BMP_SMILE01 from our resources
	CBitmap bmp;
	// if (bmp.LoadBitmap(IDB_BMP_SMILE01))
	CPngImage pngImage;
	//image.Load(AfxGetInstanceHandle(), IDB_PNG_SMILE01);
	pngImage.Load(IDB_PNG_SMILE01);

	// if (bmp.LoadBitmap(IDB_PNG_SMILE01))
	if (bmp.Attach(pngImage.Detach()))// ref: http://msdn.microsoft.com/en-us/library/97h2k0zx.aspx 
	{
		// Get the size of the bitmap
		BITMAP bmpInfo;
		bmp.GetBitmap(&bmpInfo);*/
		//CSize cch(bmpInfo.bmWidth, bmpInfo.bmHeight);		// ref: http://msdn.microsoft.com/en-us/library/xt679a2s(v=vs.110).aspx
		CSize cch(24, 24);
		// check if drawing is not possible
		CPoint ptRes = ptEnd - (ptStart+cch);			// ref: http://msdn.microsoft.com/en-us/library/t792xy69(v=vs.110).aspx
		// not possible in single line
		if (ptRes.x < 0) {
			// move to next line
			// there is still space for next line
			//if (ptRes.y >= cch.cy) {
				ptStart.x =  cxChatTextHSpace;
				ptStart.y += yCharChatText;
			/*}
			else {
				// AfxMessageBox(TEXT("Not enough space to draw bitmap!"), MB_OK);
				return ;
			}*/
		}
		CHATBOX_ELEMENT tmp = { ElemEmotIconType, TEXT(":)"), ptStart, cch, index };
		chatUIElements.AddTail(tmp);

		/* if we want image scaling, ref: 
		// dcMemory.SetStretchBltMode(HALFTONE);
		CImage img;
		img.LoadFromResource(AfxGetResourceHandle(), IDB_BMP_SMILE01);
		img.StretchBlt(dcMemory.m_hDC, 0, 0, limitRect, dimy, 0, 0, img1.GetWidth(), img1.GetHeight(), SRCCOPY);		ref: http://stackoverflow.com/questions/2339702/setting-resized-bitmap-file-to-an-mfc-picture-control
		and ref: http://social.msdn.microsoft.com/Forums/vstudio/en-US/8a636954-5a3a-4a10-9e84-386ce057b2d9/colour-problems-when-scaling-a-bitmap-with-cdcstretchblt */	

		ptStart.x += cch.cx;
	/*}
	else
	{
		TRACE0("ERROR: Where's IDB_BMP_SMILE01?\n");
		return ;
	}*/
}

// multiline drawing using drawText
bool CChatControl::VirtualDrawTextMultiLine(CString str)
{
	// check if draw is possible in single line
	CSize cch = pDC->GetOutputTextExtent(str);
	CPoint ptRes = ptEnd - (ptStart+cch);			// ref: http://msdn.microsoft.com/en-us/library/t792xy69(v=vs.110).aspx
	// possible in single line
	// if (ptRes.x >= 0 && ptRes.y >= 0) {
	// we want to scroll, do not restrict anymore on y
	if (ptRes.x >= 0) {
		CHATBOX_ELEMENT tmp = { ElemMessageType, str, ptStart, cch };
		chatUIElements.AddTail(tmp);

		// pDC->TextOut(ptStart.x, ptStart.y, str);
		ptStart.x += cch.cx;
		return true;
	}

	// multiline implementation here, input text must exceed current line during draw as it comes here
	// Implementation without using DrawText MODIFYSTRING
	// take the string, if string length is zero break
	// get extent, get the fittable string using binary divisive mechanism


	bool firstLine = true;
	// int height = 0;
	CString line;

	while (! str.IsEmpty()) 
	{
		if (firstLine)
			firstLine = false;
		else {
			// beginning x of next line
			ptStart.x = cxChatTextHSpace;
			// next line y axis
			// ptStart.y += cch.cy;
			ptStart.y += yCharChatText;
		}
		// if we are inside clip we can proceed to calculate and draw, otherwise recursion function will be unpredictable
		// as context for component function IsFittableInRectangle becomes invalid
		// no more invalid, we are only calculating not drawing in an invalid rectangle
		line = ExtractFittableLineFromStr(str);
		cch = pDC->GetOutputTextExtent(line);

		CHATBOX_ELEMENT tmp = { ElemMessageType, line, ptStart, cch };
		chatUIElements.AddTail(tmp);

		str = str.Mid(line.GetLength());
	}
	if (! str.IsEmpty())
		return false;

	if (! line.IsEmpty()) {
		// for correct calculation of extent
		ptStart.x += cch.cx;
	}
	return true;
}

/*
 *	Some input string will be fittable
 */
CString CChatControl::ExtractFittableLineFromStr(const CString str) {
	if (IsFittableInRectangle(str, str.GetLength()-1))
		return str;
	int pos = GetFittablePositionRecursive(str, 0, str.GetLength()-1);

	// find a space or newline before this position to break
	// if not found we break there anyway
	CString exStr = str.Mid(0, pos+1);

	int breakIndex = exStr.ReverseFind(_T(' '));			// ref: http://msdn.microsoft.com/en-us/library/aa300587(v=vs.60).aspx
	// none found: ' '
	if (breakIndex == -1)
		// get string from pos
		return exStr;
	return exStr.Mid(0, breakIndex+1);
}

/*	Author		:	Saint Atique
 *	Desc		:	Recursive function to get fittable position
 *				:	follows binary search mechanism to fast reach fittable string
 *				:	Fastest algorithm to get fittable position
 */
int CChatControl::GetFittablePositionRecursive(const CString str, int iMin, int iMax) {
	int iMid = (iMin + iMax) / 2;

	if (IsFittableInRectangle(str, iMid) && ! IsFittableInRectangle(str, iMid+1))
		return iMid;

	if (IsFittableInRectangle(str, iMid))
		return GetFittablePositionRecursive(str, iMid+1, iMax);
	else
		return GetFittablePositionRecursive(str, iMin, iMid-1);
}

bool CChatControl::IsFittableInRectangle(const CString gStr, const int index) {
	// index is 1 less than count/length
	CString line = gStr.Mid(0, index+1);
	CSize cch = pDC->GetOutputTextExtent(line);
	CPoint ptRes = ptEnd - (ptStart+cch);
	// if (ptRes.x >= 0 && ptRes.y >= 0) {
	if (ptRes.x >= 0) {  // y check disabled for scrolling
		return true;
	}
	return false;
}

// This function does the final act which is painting
// Paints UI Elements
//  if scrollbar is not enabled startUIElemIndex is initialized with endUIElemIndex
//  if scrollbar is enabled startUIElemIndex 
void CChatControl::PaintUIElements() {
	// should not modify drawing indexes here, we should only draw here
	// startUIElemIndex = -1;
	// endUIElemIndex = -1;
	// save previous color
	COLORREF oldColor = pDC->GetTextColor();

	// To detect new line
	//int preY=ptStart.y;		// dummy initialization ptStart.y might not be necessary

	POSITION pos = chatUIElements.GetHeadPosition();
	bool drawStarted = false;
	bool isInsideRect = true;
	bool isAlternateNameColor = false;
	bool rowDrawComplete = false;

	// ** implement drawing according to co-ordinates of update region
	//  && endUIElemIndex == -1; ) && (i<endUIElemIndex+1)
	for (int i=0; i < chatUIElements.GetCount() && rowDrawComplete==false; i++)
	{
		CHATBOX_ELEMENT curChatElement = (CHATBOX_ELEMENT) chatUIElements.GetNext(pos);
		curChatElement.ptStart.y -=  yScrollAmount;

		switch(curChatElement.type) {
		case ElemDateType:
		{	
			if (IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart) == false && IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart+curChatElement.size) == false) {
				if (drawStarted)
					isInsideRect = false;
				break;
			}
			// set date color
			pDC->SetTextColor(RGB(4,95,192));
			// set background color
			COLORREF oldBkColor = pDC->SetBkColor(RGB(170,194,154));
			// set date font
			if (pOldFont == NULL)
				pOldFont = pDC->SelectObject(&dateFont);
			else
				pDC->SelectObject(&dateFont);
			// set text align
			// UINT oldTextAlignment = pDC->SetTextAlign(TA_CENTER | TA_TOP | TA_NOUPDATECP);
			int offsetX = ((ptEnd.x + timeWidth - ptClipStart.x) - curChatElement.size.cx)/2;
			// pDC->TextOut(curChatElement.ptStart.x+offsetX, curChatElement.ptStart.y, curChatElement.text);
			// pDC->DrawText(curChatElement.text, CRect(ptClipStart,ptEnd), DT_CENTER);
			pDC->ExtTextOut(curChatElement.ptStart.x+offsetX, curChatElement.ptStart.y, ETO_OPAQUE, CRect(ptClipStart.x, curChatElement.ptStart.y, ptEnd.x+timeWidth+deliveryStatusWidth+scrollbarWidth, curChatElement.ptStart.y+yCharDate), curChatElement.text, NULL);
			// pDC->SetTextAlign(oldTextAlignment);
			pDC->SetBkColor(oldBkColor);
			drawStarted = true;
			break;
		}
		case ElemNameType:
		{	
			if (isAlternateNameColor) {
				isAlternateNameColor = false;
			}
			else {
				isAlternateNameColor = true;
			}
			if (IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart) == false && IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart+curChatElement.size) == false) {
				if (drawStarted)
					isInsideRect = false;
				break;
			}

			if (isAlternateNameColor)
				pDC->SetTextColor(RGB(136,97,11));
			else
				pDC->SetTextColor(RGB(11,136,99));		// ref: http://msdn.microsoft.com/en-us/library/vstudio/wf4k5sew.aspx

			// we are remoing backgroud color for name
			// set name heading font
			if (pOldFont == NULL)
				pOldFont = pDC->SelectObject(&headingFont);
			else
				pDC->SelectObject(&headingFont);

			pDC->TextOut(curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.text);

			pDC->SetTextColor(oldColor);
			drawStarted = true;
			break;
		}
		case ElemMessageType:
		{
			if (IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart) == false && IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart+curChatElement.size) == false) {
				if (drawStarted)
					isInsideRect = false;
				break;
			}
			// set chat text font
			if (pOldFont == NULL)
				pOldFont = pDC->SelectObject(&textFont);
			else
				pDC->SelectObject(&textFont);

			pDC->TextOut(curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.text);
			drawStarted = true;
			break;
		}
		case ElemEmotIconType:
		{
			if (IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart) == false && IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart+curChatElement.size) == false) {
				if (drawStarted)
					isInsideRect = false;
				break;
			}
			// have to implement selection according to index
			// requires implementation on VirtualDrawEmotIcons as well
			CBitmap bmp;
			// if (bmp.LoadBitmap(IDB_BMP_SMILE01))
			/*// for PNG images
			CImage image;
			image.LoadFromResource(AfxGetInstanceHandle(), IDB_PNG_SMILE01);
			//image.AlphaBlend(pDC, curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.size.cx, curChatElement.size.cy, 0, 0, curChatElement.size.cx, curChatElement.size.cy);
			//image.AlphaBlend(pDC->GetSafeHdc(), curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.size.cx, curChatElement.size.cy);
			image.AlphaBlend(pDC->GetSafeHdc(), curChatElement.ptStart.x, curChatElement.ptStart.y, 255, AC_SRC_OVER);
			/*CPngImage pngImage;
			if (pngImage.Load(IDB_PNG_SMILE01)) {
				pngImage.transp
			}
			*/
			//if (bmp.Attach(pngImage.Detach()))// ref: http://msdn.microsoft.com/en-us/library/97h2k0zx.aspx 
			if (bmp.LoadBitmap(IDB_BMP_EMOTICON_01+curChatElement.recordIndex))
			{
				// COLORREF oldBkColor = pDC->SetBkColor(TRANSPARENT);
				// Create an in-memory DC compatible with the display DC we're using to paint
				/*COLORREF oldBkColor;
				if (isAlternate) {
					oldBkColor = pDC->SetBkColor(RGB(170,194,154));
				}*/
				CDC dcMemory;
				dcMemory.CreateCompatibleDC(pDC);
				// Select the bitmap into the in-memory DC
				CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);
				// Copy the bits from the in-memory DC into the on-screen DC to actually do the painting.
				pDC->BitBlt(curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.size.cx, curChatElement.size.cy, &dcMemory, 0, 0, SRCCOPY);
				// transparency not working yet
				// pDC->TransparentBlt(curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.size.cx, curChatElement.size.cy, &dcMemory, 0, 0, curChatElement.size.cx, curChatElement.size.cy, RGB(255,255,255));
				dcMemory.SelectObject(pOldBitmap);
				// pDC->SetBkColor(oldBkColor);
			}
			drawStarted = true;
			break;
		}
		case ElemTimeType:
		{
			if (IsPointInsideClipRectangle(ptClipStart+CSize(cxChatText, 0), ptEnd+CSize(timeWidth, 0), curChatElement.ptStart) == false && IsPointInsideClipRectangle(ptClipStart+CSize(cxChatText, 0), ptEnd+CSize(timeWidth, 0), curChatElement.ptStart+curChatElement.size) == false) {
				// but don't break loop setting 'isInsideRect' because time can be out of rectangle for multiline chat text still next item can be inside rect
				break;
			}

			// not checking clip now, probably later
			// set font for time
			if (pOldFont == NULL)
				pOldFont = pDC->SelectObject(&timeFont);
			else
				pDC->SelectObject(&timeFont);

			// set text align
			UINT oldTextAlignment = pDC->SetTextAlign(TA_RIGHT);
			pDC->TextOut(ptEnd.x+timeWidth, curChatElement.ptStart.y, curChatElement.text);
			pDC->SetTextAlign(oldTextAlignment);
			drawStarted = true;
			if (isInsideRect == false)
				rowDrawComplete = true;
			break;
		}
		case ElemDeliveryStatusType:
		{
			/*if (IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart) == false && IsPointInsideClipRectangle(ptClipStart, ptEnd, curChatElement.ptStart+curChatElement.size) == false) {
				if (drawStarted)
					isInsideRect = false;
				break;
			}*/
			CBitmap bmp;
			if (bmp.LoadBitmap(IDB_BMP_DELIV_STATUS_01))
			{
				// COLORREF oldBkColor = pDC->SetBkColor(TRANSPARENT);
				// Create an in-memory DC compatible with the display DC we're using to paint
				/*COLORREF oldBkColor;
				if (isAlternate) {
					oldBkColor = pDC->SetBkColor(RGB(170,194,154));
				}*/
				CDC dcMemory; 
				dcMemory.CreateCompatibleDC(pDC);
				// Select the bitmap into the in-memory DC
				CBitmap* pOldBitmap = dcMemory.SelectObject(&bmp);
				// Copy the bits from the in-memory DC into the on-screen DC to actually do the painting.
				pDC->BitBlt(curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.size.cx, curChatElement.size.cy, &dcMemory, 0, 0, SRCCOPY);
				// transparency not working yet
				// pDC->TransparentBlt(curChatElement.ptStart.x, curChatElement.ptStart.y, curChatElement.size.cx, curChatElement.size.cy, &dcMemory, 0, 0, curChatElement.size.cx, curChatElement.size.cy, RGB(255,255,255));
				dcMemory.SelectObject(pOldBitmap);
				// pDC->SetBkColor(oldBkColor);
			}
			//drawStarted = true;
			break;
		}
		default:
			break;
		}
	}
}

// checks if point is inside provided rectangle
bool IsPointInsideClipRectangle(const CPoint topLeft, const CPoint bottomRight, const CPoint point) {
	// check x
	if (topLeft.x > point.x)
		return false;
	if (point.x > bottomRight.x)
		return false;
	// check y
	if (topLeft.y > point.y)
		return false;
	if (point.y > bottomRight.y)
		return false;
	return true;
}


void CChatControl::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// This feature requires Windows Vista or greater.
	// The symbol _WIN32_WINNT must be >= 0x0600.
	// TODO: Add your message handler code here and/or call default
	AfxMessageBox(_T("we are on mouse wheel child"));
	CWnd::OnMouseHWheel(nFlags, zDelta, pt);
}

// in case user resizes the window, fix page size
void CChatControl::Repaint()
{
	Invalidate();
}
