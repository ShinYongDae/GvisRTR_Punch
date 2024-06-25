#include "stdafx.h"
#include "ManagerStatus.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerStatus::CManagerStatus(CWnd* pParent)
{
	m_pParent = pParent;

	Reset();
	Init();

	if (!Create())
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("CManagerStatus::Create() Failed!!!"));
	}
}


CManagerStatus::~CManagerStatus()
{
}

BEGIN_MESSAGE_MAP(CManagerStatus, CWnd)
END_MESSAGE_MAP()


void CManagerStatus::Init()
{

}

void CManagerStatus::Reset()
{
}

BOOL CManagerStatus::Create()
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
