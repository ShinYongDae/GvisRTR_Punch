#include "stdafx.h"
#include "ManagerFeeding.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerFeeding::CManagerFeeding(CWnd* pParent)
{
	m_pParent = pParent;
	m_bTIM_INIT_FEEDING = FALSE;

	Init();

	if (Create())
	{
		pView->ClrDispMsg(); 
		AfxMessageBox(_T("CManagerFeeding::Create() Failed!!!"));
	}
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
	UINT nID = 5000;
	CCreateContext* pContext = NULL;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CManagerFeeding::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CWnd::OnTimer(nIDEvent);
}

void CManagerFeeding::Init()
{
}

BOOL CManagerFeeding::InitAct()
{
	double dPos = _tstof(pDoc->WorkingInfo.Motion.sStBufPos);
	SetBufInitPos(dPos);
	double dVel = _tstof(pDoc->WorkingInfo.Motion.sBufHomeSpd);
	double dAcc = _tstof(pDoc->WorkingInfo.Motion.sBufHomeAcc);
	//SetBufHomeParam(dVel, dAcc);

	return TRUE;
}

void CManagerFeeding::SetBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	pDoc->WorkingInfo.Motion.sStBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(_T("ML45016"), lData);	// ���� ���� ���� �ѷ� �ʱ���ġ(���� mm * 1000)
#endif
}

void CManagerFeeding::SetBufHomeParam(double dVel, double dAcc)
{
	long lVel = long(dVel*1000.0);
	long lAcc = long(dAcc*1000.0);
	//	MpeWrite(_T("ML00000"), lVel); // ��ŷ�� ���� Ȩ �ӵ�
	//	MpeWrite(_T("ML00000"), lAcc); // ��ŷ�� ���� Ȩ ���ӵ�
	//	pDoc->SetBufInitPos(dVel, dAcc);
}

