
// GvisRTR_PunchView.cpp : CGvisRTR_PunchView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GvisRTR_Punch.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
CGvisRTR_PunchView* pView;


// CGvisRTR_PunchView

IMPLEMENT_DYNCREATE(CGvisRTR_PunchView, CFormView)

BEGIN_MESSAGE_MAP(CGvisRTR_PunchView, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DLG_INFO, OnDlgInfo)
END_MESSAGE_MAP()

// CGvisRTR_PunchView 생성/소멸

CGvisRTR_PunchView::CGvisRTR_PunchView()
	: CFormView(IDD_GVISRTR_PUNCH_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	pView = this;
	m_bTIM_INIT_VIEW = FALSE;

	InitMgr();	
	InitDispMsg();
	InitDlg();


}

CGvisRTR_PunchView::~CGvisRTR_PunchView()
{
	m_bTIM_INIT_VIEW = FALSE;

	CloseMgr();
	CloseDispMsg();
}

void CGvisRTR_PunchView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CGvisRTR_PunchView::DestroyView()
{
	DelAllDlg();

	//CString sData;

	//if (!m_bDestroyedView)
	//{
	//	m_bDestroyedView = TRUE;

	//	sData.Format(_T("%d"), m_mgrProcedure->m_nMkStAuto);
	//	::WritePrivateProfileString(_T("Last Job"), _T("MkStAuto"), sData, PATH_WORKING_INFO);

	//	DelAllDlg();
	//	Sleep(100);
	//}
}

BOOL CGvisRTR_PunchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CGvisRTR_PunchView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	if (!m_bTIM_INIT_VIEW)
	{
		m_nStepInitView = 0;
		m_bTIM_INIT_VIEW = TRUE;
		SetTimer(TIM_INIT_VIEW, 300, NULL);
	}
}


// CGvisRTR_PunchView 진단

#ifdef _DEBUG
void CGvisRTR_PunchView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGvisRTR_PunchView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGvisRTR_PunchDoc* CGvisRTR_PunchView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGvisRTR_PunchDoc)));
	return (CGvisRTR_PunchDoc*)m_pDocument;
}
#endif //_DEBUG


// CGvisRTR_PunchView 메시지 처리기
void CGvisRTR_PunchView::InitMgr()
{
	m_mgrFeeding = NULL;
	m_mgrPunch = NULL;
	m_mgrReelmap = NULL;
	m_mgrThread = NULL;
	m_mgrProcedure = NULL;
	m_mgrStatus = NULL;
}

void CGvisRTR_PunchView::CreateMgr()
{
	CreateMgrFeeding();
	CreateMgrPunch();
	CreateMgrReelmap();
	CreateMgrThread();
	CreateMgrProcedure();
	CreateMgrStatus();
}

void CGvisRTR_PunchView::CreateMgrPunch()
{
	if (m_mgrPunch)
	{
		delete m_mgrPunch;
		m_mgrPunch = NULL;
	}
	m_mgrPunch = new CManagerPunch(this);
	//m_mgrPunch->Init();
}

void CGvisRTR_PunchView::CreateMgrProcedure()
{
	if (m_mgrProcedure)
	{
		delete m_mgrProcedure;
		m_mgrProcedure = NULL;
	}
	m_mgrProcedure = new CManagerProcedure(this);
	//m_mgrProcedure->Init();
}

void CGvisRTR_PunchView::CreateMgrReelmap()
{
	if (m_mgrReelmap)
	{
		delete m_mgrReelmap;
		m_mgrReelmap = NULL;
	}
	m_mgrReelmap = new CManagerReelmap(this);
}

void CGvisRTR_PunchView::CreateMgrThread()
{
	if (m_mgrThread)
	{
		delete m_mgrThread;
		m_mgrThread = NULL;
	}
	m_mgrThread = new CManagerThread(this);
}

void CGvisRTR_PunchView::CreateMgrFeeding()
{
	if (m_mgrFeeding)
	{
		delete m_mgrFeeding;
		m_mgrFeeding = NULL;
	}
	m_mgrFeeding = new CManagerFeeding(this);
}

void CGvisRTR_PunchView::CreateMgrStatus()
{
	if (m_mgrStatus)
	{
		delete m_mgrStatus;
		m_mgrStatus = NULL;
	}
	m_mgrStatus = new CManagerStatus(this);
}

void CGvisRTR_PunchView::CloseMgr()
{
	CloseMgrFeeding();
	CloseMgrPunch();
	CloseMgrReelmap();
	CloseMgrThread();
	CloseMgrProcedure();
	CloseMgrStatus();
}

void CGvisRTR_PunchView::CloseMgrPunch()
{
	if (m_mgrPunch)
	{
		delete m_mgrPunch;
		m_mgrPunch = NULL;
	}
}

void CGvisRTR_PunchView::CloseMgrProcedure()
{
	if (m_mgrProcedure)
	{
		delete m_mgrProcedure;
		m_mgrProcedure = NULL;
	}
}

void CGvisRTR_PunchView::CloseMgrReelmap()
{
	if (m_mgrReelmap)
	{
		delete m_mgrReelmap;
		m_mgrReelmap = NULL;
	}
}

void CGvisRTR_PunchView::CloseMgrThread()
{
	if (m_mgrThread)
	{
		delete m_mgrThread;
		m_mgrThread = NULL;
	}
}

void CGvisRTR_PunchView::CloseMgrFeeding()
{
	if (m_mgrFeeding)
	{
		delete m_mgrFeeding;
		m_mgrFeeding = NULL;
	}
}

void CGvisRTR_PunchView::CloseMgrStatus()
{
	if (m_mgrStatus)
	{
		delete m_mgrStatus;
		m_mgrStatus = NULL;
	}
}

void CGvisRTR_PunchView::InitDispMsg()
{
	m_bDispMsg = FALSE;
	m_pDlgMyMsg = NULL;
	m_pDlgMsgBox = NULL;
}

void CGvisRTR_PunchView::CloseDispMsg()
{
	CloseMyMsg();
	CloseMsgBox();
}

void CGvisRTR_PunchView::CloseMyMsg()
{
	if (m_pDlgMyMsg)
	{
		delete m_pDlgMyMsg;
		m_pDlgMyMsg = NULL;
	}
}

void CGvisRTR_PunchView::CloseMsgBox()
{
	if (m_pDlgMsgBox != NULL)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}
}

LONG CGvisRTR_PunchView::OnQuitDispMsg(UINT wParam, LONG lParam)
{
	if (m_pDlgMsgBox)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}

	return 0L;
}

void CGvisRTR_PunchView::ClrDispMsg()
{
	OnQuitDispMsg(NULL, NULL);
}

void CGvisRTR_PunchView::DoDispMsg(CString strMsg, CString strTitle, COLORREF color, DWORD dwDispTime, BOOL bOverWrite)
{
	if (dwDispTime == 0)
	{
		dwDispTime = 24 * 3600 * 1000;
	}

	if (m_pDlgMsgBox != NULL)
	{
		if (bOverWrite)
		{
			if (m_pDlgMsgBox)
				m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
		}
		if (m_pDlgMsgBox)
			m_pDlgMsgBox->ShowWindow(SW_SHOW);
		if (m_pDlgMsgBox)
			m_pDlgMsgBox->SetFocus();
		if (m_pDlgMsgBox)
			((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
	}
	else
	{
		m_pDlgMsgBox = new CDlgMsgBox(this, strTitle, strMsg, dwDispTime, color);
		if (m_pDlgMsgBox->GetSafeHwnd() == 0)
		{
			m_pDlgMsgBox->Create();
			m_pDlgMsgBox->ShowWindow(SW_SHOW);
			m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
			m_pDlgMsgBox->SetFocus();
			((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
		}
	}
}

void CGvisRTR_PunchView::GetDispMsg(CString &strMsg, CString &strTitle)
{
	if (m_pDlgMsgBox)
		m_pDlgMsgBox->GetDispMsg(strMsg, strTitle);
}

void CGvisRTR_PunchView::DispMsg(CString strMsg, CString strTitle, COLORREF color, DWORD dwDispTime, BOOL bOverWrite)
{
	if (m_bDispMsg)
		return;

	m_bDispMsg = TRUE;
	DoDispMsg(strMsg, strTitle, color, dwDispTime, bOverWrite);
	m_bDispMsg = FALSE;
}

int CGvisRTR_PunchView::MsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut, BOOL bEngave)
{
	int nRtnVal = -1; // Reply(-1) is None.

	//if (bEngave)
	//{
	//	if (m_pEngrave)
	//	{
	//		pDoc->m_sMsgBox = sMsg;
	//		if (pDoc->m_sIsMsgBox != pDoc->m_sMsgBox)
	//		{
	//			if (m_pEngrave)
	//				m_pEngrave->SetMsgBox(pDoc->m_sMsgBox, nType);
	//		}
	//	}
	//}

	if (m_pDlgMyMsg)
		nRtnVal = m_pDlgMyMsg->SyncMsgBox(sMsg, nThreadIdx, nType, nTimOut);

	return nRtnVal;
}

void CGvisRTR_PunchView::InitDlg()
{
	m_pDlgInfo = NULL;
	m_pDlgFrameHigh = NULL;
	m_pDlgMenu01 = NULL;
	m_pDlgMenu02 = NULL;
	m_pDlgMenu03 = NULL;
	m_pDlgMenu04 = NULL;
	m_pDlgMenu05 = NULL;
	m_pDlgMenu06 = NULL;
	//m_pDlgMenu07 = NULL;
	//m_pDlgOption01 = NULL;

}

void CGvisRTR_PunchView::ShowDlg(int nID)
{
	HideAllDlg();

	switch (nID)
	{
	case IDD_DLG_FRAME_HIGH:
		if (!m_pDlgFrameHigh)
			m_pDlgFrameHigh = new CDlgFrameHigh(this);
		if (m_pDlgFrameHigh->GetSafeHwnd())
			m_pDlgFrameHigh->ShowWindow(SW_SHOW);
		break;

	case IDD_DLG_MENU_01:
		if (!m_pDlgMenu01)
			m_pDlgMenu01 = new CDlgMenu01(this);
		if (m_pDlgMenu01->GetSafeHwnd())
			m_pDlgMenu01->ShowWindow(SW_SHOW);
		break;

	case IDD_DLG_MENU_02:
		if (!m_pDlgMenu02)
			m_pDlgMenu02 = new CDlgMenu02(this);
		if (m_pDlgMenu02->GetSafeHwnd())
			m_pDlgMenu02->ShowWindow(SW_SHOW);
		break;

	case IDD_DLG_MENU_03:
		if (!m_pDlgMenu03)
			m_pDlgMenu03 = new CDlgMenu03(this);
		if (m_pDlgMenu03->GetSafeHwnd())
			m_pDlgMenu03->ShowWindow(SW_SHOW);
		break;

	case IDD_DLG_MENU_04:
		if (!m_pDlgMenu04)
			m_pDlgMenu04 = new CDlgMenu04(this);
		if (m_pDlgMenu04->GetSafeHwnd())
			m_pDlgMenu04->ShowWindow(SW_SHOW);
		break;

	case IDD_DLG_MENU_05:
		if (!m_pDlgMenu05)
			m_pDlgMenu05 = new CDlgMenu05(this);
		if (m_pDlgMenu05->GetSafeHwnd())
			m_pDlgMenu05->ShowWindow(SW_SHOW);
		break;

	case IDD_DLG_MENU_06:
		if (!m_pDlgMenu06)
			m_pDlgMenu06 = new CDlgMenu06(this);
		if (m_pDlgMenu06->GetSafeHwnd())
			m_pDlgMenu06->ShowWindow(SW_SHOW);
		break;
	}
}

void CGvisRTR_PunchView::HideAllDlg()
{
	if (m_pDlgMenu01 && m_pDlgMenu01->GetSafeHwnd())
	{
		if (m_pDlgMenu01->IsWindowVisible())
			m_pDlgMenu01->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu02 && m_pDlgMenu02->GetSafeHwnd())
	{
		if (m_pDlgMenu02->IsWindowVisible())
			m_pDlgMenu02->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu03 && m_pDlgMenu03->GetSafeHwnd())
	{
		if (m_pDlgMenu03->IsWindowVisible())
			m_pDlgMenu03->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu04 && m_pDlgMenu04->GetSafeHwnd())
	{
		if (m_pDlgMenu04->IsWindowVisible())
			m_pDlgMenu04->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu05 && m_pDlgMenu05->GetSafeHwnd())
	{
		if (m_pDlgMenu05->IsWindowVisible())
			m_pDlgMenu05->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu06 && m_pDlgMenu06->GetSafeHwnd())
	{
		if (m_pDlgMenu06->IsWindowVisible())
			m_pDlgMenu06->ShowWindow(SW_HIDE);
	}
}

void CGvisRTR_PunchView::DelAllDlg()
{
	//if (m_pDlgMenu07 != NULL)
	//{
	//	delete m_pDlgMenu07;
	//	m_pDlgMenu07 = NULL;
	//}
	if (m_pDlgMenu06 != NULL)
	{
		delete m_pDlgMenu06;
		m_pDlgMenu06 = NULL;
	}
	if (m_pDlgMenu05 != NULL)
	{
		delete m_pDlgMenu05;
		m_pDlgMenu05 = NULL;
	}
	if (m_pDlgMenu04 != NULL)
	{
		delete m_pDlgMenu04;
		m_pDlgMenu04 = NULL;
	}
	if (m_pDlgMenu03 != NULL)
	{
		delete m_pDlgMenu03;
		m_pDlgMenu03 = NULL;
	}
	if (m_pDlgMenu02 != NULL)
	{
		delete m_pDlgMenu02;
		m_pDlgMenu02 = NULL;
	}
	if (m_pDlgMenu01 != NULL)
	{
		delete m_pDlgMenu01;
		m_pDlgMenu01 = NULL;
	}
	if (m_pDlgFrameHigh != NULL)
	{
		delete m_pDlgFrameHigh;
		m_pDlgFrameHigh = NULL;
	}

	if (m_pDlgMsgBox != NULL)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}
}

LRESULT CGvisRTR_PunchView::OnDlgInfo(WPARAM wParam, LPARAM lParam)
{
	ClrDispMsg();
	CDlgInfo Dlg;
	m_pDlgInfo = &Dlg;
	Dlg.DoModal();
	m_pDlgInfo = NULL;

	if (m_pDlgMenu01)
		m_pDlgMenu01->ChkUserInfo(FALSE);

	return 0L;
}

void CGvisRTR_PunchView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIM_INIT_VIEW)
	{
		KillTimer(TIM_INIT_VIEW);

		switch (m_nStepInitView)
		{
		case 0:
			DoDispMsg(_T("프로그램을 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			m_nStepInitView++;
			break;
		case 1:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 1"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_02);
			break;
		case 2:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.-2"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_01);
			//if (bDualTest)
			//	m_pDlgMenu01->SelMap(ALL);
			//else
			//	m_pDlgMenu01->SelMap(UP);
			//break;
		case 3:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 3"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			//ShowDlg(IDD_DLG_MENU_02);
			break;
		case 4:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 4"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_03);
			break;
		case 5:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 5"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_04);
			break;
		case 6:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 6"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_05);
			break;
		case 7:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 7"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_06);
			break;
		case 8:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 8"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_07);
			break;
		case 9:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- 9"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_FRAME_HIGH);
			if (m_pDlgFrameHigh)
				m_pDlgFrameHigh->ChkMenu01();
			//SetDualTest(pDoc->WorkingInfo.LastJob.bDualTest);

			//if (pDoc->GetCurrentInfoEng())
			//{
			//	if (m_mgrReelmap->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
			//	{
			//		//if (pDoc->GetTestMode() == MODE_OUTER)
			//		if (m_mgrReelmap->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
			//		{
			//			if (m_pDlgMenu06)
			//				m_pDlgMenu06->RedrawWindow();
			//		}
			//	}
			//}
			Sleep(300);

		case 10:
			m_nStepInitView++;
			DispMsg(_T("Manager를 생성합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			CreateMgr();
			break;
			break;

		case 11:
			m_nStepInitView++;
			DispMsg(_T("Motion을 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			if (m_mgrPunch)
				m_mgrPunch->ResetMotion();
			Sleep(300);
			break;
		case 12:
			m_nStepInitView++;
			//m_bLoadMstInfo = TRUE;
			DispMsg(_T("H/W를 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			m_mgrPunch->InitAct();
			m_mgrReelmap->InitAct();
			m_mgrFeeding->InitAct();
			//m_mgrProcedure->m_bStopFeeding = TRUE;
			//MpeWrite(_T("MB440115"), 1); // 마킹부Feeding금지
			//Sleep(300);
			m_bTIM_INIT_VIEW = FALSE;
			break;
		}

		if (m_bTIM_INIT_VIEW)
			SetTimer(TIM_INIT_VIEW, 100, NULL);
	}

	CFormView::OnTimer(nIDEvent);
}
