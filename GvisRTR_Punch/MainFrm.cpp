
// MainFrm.cpp : CMainFrame Ŭ������ ����
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
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
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
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	 | WS_SYSMENU;

	CGvisRTR_PunchApp *app = (CGvisRTR_PunchApp *)AfxGetApp();
	cs.lpszClass = app->m_singleInstance.GetClassName();

	cs.style = WS_VISIBLE | WS_POPUP;


	return TRUE;
}

// CMainFrame ����

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


// CMainFrame �޽��� ó����



BOOL CMainFrame::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	pDoc->DestroyDoc();
	pView->DestroyView();

	return CFrameWnd::DestroyWindow();
}


void CMainFrame::OnAppMinimize()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	WINDOWPLACEMENT wndPlace;
	GetWindowPlacement(&wndPlace);
	wndPlace.showCmd |= SW_MINIMIZE;
	SetWindowPlacement(&wndPlace);
}


void CMainFrame::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CFrameWnd::OnClose();
}


void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
