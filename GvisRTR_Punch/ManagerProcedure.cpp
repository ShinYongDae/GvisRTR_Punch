#include "stdafx.h"
#include "ManagerProcedure.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerProcedure::CManagerProcedure(CWnd* pParent)
{
	m_pParent = pParent;
	m_bTIM_INIT_PROCEDURE = FALSE;

	if (Create())
		AfxMessageBox(_T("CManagerProcedure::Create() Failed!!!"));
}


CManagerProcedure::~CManagerProcedure()
{
	m_bTIM_INIT_PROCEDURE = FALSE;
}

BEGIN_MESSAGE_MAP(CManagerProcedure, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerProcedure::Create()
{
	LPCTSTR lpszClassName = _T("ManagerProcedure");
	LPCTSTR lpszWindowName = _T("ManagerProcedure");
	DWORD dwStyle = WS_CHILD;
	const RECT& rect = CRect(0, 0, 0, 0);
	CWnd* pParentWnd = m_pParent;
	UINT nID = 5000;
	CCreateContext* pContext = NULL;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CManagerProcedure::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == TIM_INIT_PROCEDURE)
	{
		KillTimer(TIM_INIT_PROCEDURE);

		switch (m_nStepInitProcedure)
		{
		case 0:
			m_nStepInitProcedure++;
			break;
		}
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CManagerProcedure::InitAct()
{
	return TRUE;
}
