#include "stdafx.h"
#include "ManagerFeeding.h"


CManagerFeeding::CManagerFeeding(CWnd* pParent)
{
	m_pParent = pParent;
	m_bTIM_INIT_FEEDING = FALSE;

	if (Create())
		AfxMessageBox(_T("CManagerFeeding::Create() Failed!!!"));
}


CManagerFeeding::~CManagerFeeding()
{
}

BEGIN_MESSAGE_MAP(CManagerFeeding, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerFeeding::Create()
{
	LPCTSTR lpszClassName = _T("ManagerFeeding");
	LPCTSTR lpszWindowName = _T("ManagerFeeding");
	DWORD dwStyle = WS_CHILD;
	const RECT& rect = CRect(0, 0, 0, 0);
	CWnd* pParentWnd = m_pParent;
	UINT nID = 5001;
	CCreateContext* pContext = NULL;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CManagerFeeding::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnTimer(nIDEvent);
}
