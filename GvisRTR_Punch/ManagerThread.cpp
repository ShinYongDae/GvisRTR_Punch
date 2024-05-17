#include "stdafx.h"
#include "ManagerThread.h"


CManagerThread::CManagerThread(CWnd* pParent)
{
	m_pParent = pParent;
}


CManagerThread::~CManagerThread()
{
}

BEGIN_MESSAGE_MAP(CManagerThread, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerThread::Create()
{
	LPCTSTR lpszClassName = _T("ManagerThread");
	LPCTSTR lpszWindowName = _T("ManagerThread");
	DWORD dwStyle = WS_CHILD;
	const RECT& rect = CRect(0, 0, 0, 0);
	CWnd* pParentWnd = m_pParent;
	UINT nID = 5004;
	CCreateContext* pContext = NULL;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CManagerThread::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnTimer(nIDEvent);
}
