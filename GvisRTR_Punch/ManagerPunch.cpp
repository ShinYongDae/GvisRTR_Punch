#include "stdafx.h"
#include "ManagerPunch.h"


CManagerPunch::CManagerPunch(CWnd* pParent)
{
	m_pParent = pParent;
}


CManagerPunch::~CManagerPunch()
{
}

BEGIN_MESSAGE_MAP(CManagerPunch, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerPunch::Create()
{
	LPCTSTR lpszClassName = _T("ManagerPunch");
	LPCTSTR lpszWindowName = _T("ManagerPunch");
	DWORD dwStyle = WS_CHILD;
	const RECT& rect = CRect(0, 0, 0, 0);
	CWnd* pParentWnd = m_pParent;
	UINT nID = 5002;
	CCreateContext* pContext = NULL;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CManagerPunch::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnTimer(nIDEvent);
}
