#include "stdafx.h"
#include "ManagerReelmap.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerReelmap::CManagerReelmap(CWnd* pParent)
{
	m_pParent = pParent;
	Init();

	if (!Create())
	{
		pView->ClrDispMsg();
	}
}


CManagerReelmap::~CManagerReelmap()
{
}

BEGIN_MESSAGE_MAP(CManagerReelmap, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerReelmap::Create()
{
	LPCTSTR lpszClassName = NULL;
	LPCTSTR lpszWindowName = _T("None");
	DWORD dwStyle = WS_CHILD;
	const RECT& rect = CRect(0, 0, 0, 0);
	CWnd* pParentWnd = m_pParent;
	UINT nID = (UINT)this;
	CCreateContext* pContext = NULL;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CManagerReelmap::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnTimer(nIDEvent);
}

void CManagerReelmap::Init()
{
	LoadConfig();
}

BOOL CManagerReelmap::InitAct()
{

	return TRUE;
}

void CManagerReelmap::LoadConfig()
{
}