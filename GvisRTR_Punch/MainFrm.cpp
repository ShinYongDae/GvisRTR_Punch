
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "GvisRTR_Punch.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

CMainFrame* pFrm;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_APP_MINIMIZE, &CMainFrame::OnAppMinimize)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.

	for (int i = 0; i < 10; i++)
		m_sDispStatusBar[i] = _T("");
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	CRect rect;
	this->GetWindowRect(&rect);  //get client rect... 	
	int nScreenWidth = 1230;

	// Panel 크기조절 
	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 1 / 120);
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 32 / 120);
	m_wndStatusBar.SetPaneInfo(2, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 4 / 120);
	m_wndStatusBar.SetPaneInfo(3, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 55 / 120);
	m_wndStatusBar.SetPaneInfo(4, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 4 / 120);
	m_wndStatusBar.SetPaneInfo(5, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 5 / 120);
	m_wndStatusBar.SetPaneInfo(6, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 7 / 120);
	m_wndStatusBar.SetPaneInfo(7, ID_SEPARATOR, SBPS_NORMAL, nScreenWidth * 12 / 120);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	 | WS_SYSMENU;

	CGvisRTR_PunchApp *app = (CGvisRTR_PunchApp *)AfxGetApp();
	cs.lpszClass = app->m_singleInstance.GetClassName();

	cs.style = WS_VISIBLE | WS_POPUP;


	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//pDoc->DestroyDoc();
	//pView->DestroyView();

	return CFrameWnd::DestroyWindow();
}


void CMainFrame::OnAppMinimize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	WINDOWPLACEMENT wndPlace;
	GetWindowPlacement(&wndPlace);
	wndPlace.showCmd |= SW_MINIMIZE;
	SetWindowPlacement(&wndPlace);
}


void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd::OnClose();
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMainFrame::DispStatusBar(CString strMsg, int nStatusBarID)
{
	if (m_sDispStatusBar[nStatusBarID] != strMsg)
	{
		m_sDispStatusBar[nStatusBarID] = strMsg;
		m_wndStatusBar.SetPaneText(nStatusBarID, strMsg);
	}
}