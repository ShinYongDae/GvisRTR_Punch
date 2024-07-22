
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

#include "Dialog/DlgMyPassword.h"
#include "Dialog/DlgMyMsgSub00.h"
#include "Dialog/DlgMyMsgSub01.h"
#include "Dialog/DlgMyMsgSub02.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
CGvisRTR_PunchView* pView;

#define TIM_SHOW_MENU01			18
#define TIM_SHOW_MENU02			19

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
	m_bTIM_DISP_STATUS = FALSE;
	m_sDispMain = _T("");
	m_sDispTime = _T("");
	m_dwLotSt = 0; m_dwLotEd = 0;

	for (int i = 0; i < 10; i++)
		m_sDispStatusBar[i] = _T("");

	m_sShare[0] = _T("");
	m_sBuf[0] = _T("");
	m_sShare[1] = _T("");
	m_sBuf[1] = _T("");

	m_bTIM_START_UPDATE = FALSE; 
	m_bTIM_CAMMASTER_UPDATE = FALSE;
	m_bLoadMstInfo = FALSE;  
	m_bLoadMstInfoF = FALSE;

	m_bSetSig = FALSE;
	m_bSetSigF = FALSE;
	m_bSetData = FALSE;
	m_bSetDataF = FALSE;

	m_bDrawGL_Menu01 = TRUE;
	m_bDrawGL_Menu06 = TRUE;

	ClrAlarm();
	InitMgr();	
	InitDispMsg();
	InitDlg();
}

CGvisRTR_PunchView::~CGvisRTR_PunchView()
{
	m_bTIM_INIT_VIEW = FALSE;
	m_bTIM_DISP_STATUS = FALSE;

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

	pDoc->SetMonDispMain(_T(""));

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
			//if (m_mgrPunch)
			//	m_mgrPunch->ResetMotion();
			Sleep(300);
			break;
		case 12:
			m_nStepInitView++;
			//m_bLoadMstInfo = TRUE;
			DispMsg(_T("H/W를 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			if (!m_mgrPunch->InitAct() || !m_mgrFeeding->InitAct())
			{
				PostMessage(WM_CLOSE);;
			}
			m_mgrReelmap->InitAct();
			m_mgrPunch->StopFeeding(TRUE); // 마킹부Feeding금지
			Sleep(300);
			break;
		case 13:
			m_nStepInitView++;
			ClrDispMsg();
			ChkBuf(); // 1, 3
			SetListBuf();
			m_bTIM_DISP_STATUS = TRUE;
			SetTimer(TIM_DISP_STATUS, 100, NULL);
			m_mgrPunch->StopFeeding(FALSE); // 마킹부Feeding금지해제
			m_bTIM_INIT_VIEW = FALSE;
			break;
		}

		if (m_bTIM_INIT_VIEW)
			SetTimer(TIM_INIT_VIEW, 100, NULL);
	}

	if (nIDEvent == TIM_DISP_STATUS)
	{
		KillTimer(TIM_DISP_STATUS);

		DispStsBar();
		DoDispMain();

		if (m_bTIM_DISP_STATUS)
			SetTimer(TIM_DISP_STATUS, 100, NULL);
	}

	if (nIDEvent == TIM_SHOW_MENU01)
	{
		KillTimer(TIM_SHOW_MENU01);
		if (m_pDlgFrameHigh)
			m_pDlgFrameHigh->ChkMenu01();
	}

	if (nIDEvent == TIM_SHOW_MENU02)
	{
		KillTimer(TIM_SHOW_MENU02);
		if (m_pDlgFrameHigh)
			m_pDlgFrameHigh->ChkMenu02();
	}

	if (nIDEvent == TIM_START_UPDATE)
	{
		KillTimer(TIM_START_UPDATE);

		if (m_bLoadMstInfo && !m_bLoadMstInfoF)
		{
			if (!pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() && !pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			{
				m_bLoadMstInfoF = TRUE;
				SetTimer(TIM_CAMMASTER_UPDATE, 500, NULL);
			}
			else
				m_bLoadMstInfo = FALSE;
		}

		if (m_bSetSig && !m_bSetSigF)
		{
			m_bSetSigF = TRUE;

			if (GetEngOpInfo() || GetEngInfo())
			{
				if (m_pDlgInfo)
					m_pDlgInfo->UpdateData();

				if (m_pDlgMenu01)
					m_pDlgMenu01->UpdateData();

				SetEngOpInfo(FALSE);
				SetEngInfo(FALSE);
			}
			else
			{
				if (m_pDlgMenu03)
					m_pDlgMenu03->UpdateSignal();
			}

			m_bSetSig = FALSE;
		}
		else if (!m_bSetSig && m_bSetSigF)
		{
			m_bSetSigF = FALSE;
		}

		if (m_bSetData && !m_bSetDataF)
		{
			m_bSetDataF = TRUE;

			if (GetEngOpInfo() || GetEngInfo())
			{
				if (m_pDlgInfo)
					m_pDlgInfo->UpdateData();

				if (m_pDlgMenu01)
					m_pDlgMenu01->UpdateData();

				SetEngOpInfo(FALSE);
				SetEngInfo(FALSE);
			}

			if (m_pDlgMenu02)
				m_pDlgMenu02->UpdateData();

			if (m_pDlgMenu03)
				m_pDlgMenu03->UpdateData();

			if (m_pDlgMenu04)
				m_pDlgMenu04->UpdateData();

			m_bSetData = FALSE;
		}
		else if (!m_bSetData && m_bSetDataF)
		{
			m_bSetDataF = FALSE;
		}

		if (m_bTIM_START_UPDATE)
			SetTimer(TIM_START_UPDATE, 100, NULL);
	}

	if (nIDEvent == TIM_CAMMASTER_UPDATE)
	{
		KillTimer(TIM_CAMMASTER_UPDATE);
		if (LoadMstInfo())
		{
			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();
		}
		m_bLoadMstInfoF = FALSE;
		m_bLoadMstInfo = FALSE;
	}

	CFormView::OnTimer(nIDEvent);
}

void CGvisRTR_PunchView::DispStsBar()
{
	DispStsMainMsg();	// 0
	DispTime();			// 7
	ChkShare();			// 2, 4
	ChkBuf();			// 1, 3
	SetListBuf();
}

void CGvisRTR_PunchView::DispStsBar(CString sMsg, int nIdx)
{
	if (m_sDispStatusBar[nIdx] != sMsg)
		m_sDispStatusBar[nIdx] = sMsg;
}

void CGvisRTR_PunchView::DispStsMainMsg(int nIdx)
{
	CString str;
	str = m_sDispStatusBar[nIdx];
	pFrm->DispStatusBar(str, nIdx);
}

void CGvisRTR_PunchView::DispTime()
{
	stLotTime LotTime;
	CString str;
	str = GetTime(LotTime);
	if (m_sDispTime != str)
	{
		m_sDispTime = str;
		pFrm->DispStatusBar(str, 7);

		pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
		pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
		pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
		pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
		pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
		pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

		if (m_pDlgMenu01)
			m_pDlgMenu01->DispRunTime();
	}
}

CString CGvisRTR_PunchView::GetTime(stLotTime &LotTime)
{
	CString strVal;
	time_t osBinTime;				// C run-time time (defined in <time.h>)
	time(&osBinTime);				// Get the current time from the 
									// operating system.
	CTime Tim(osBinTime);

	LotTime.nYear = Tim.GetYear();
	LotTime.nMonth = Tim.GetMonth();
	LotTime.nDay = Tim.GetDay();
	LotTime.nHour = Tim.GetHour();
	LotTime.nMin = Tim.GetMinute();
	LotTime.nSec = Tim.GetSecond();

	strVal.Format(_T("%04d-%02d-%02d,%02d:%02d:%02d"), LotTime.nYear, LotTime.nMonth, LotTime.nDay,
		LotTime.nHour, LotTime.nMin, LotTime.nSec);
	return strVal;
}

void CGvisRTR_PunchView::ChkShare()
{
	ChkShareUp();
	ChkShareDn();
}

void CGvisRTR_PunchView::ChkShareUp()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShare) return;
	stPcrShare* PcrShare = (m_mgrStatus->PcrShare);

	CString str, str2;
	int nSerial, nAoiUpAutoSerial;

	if (ChkShareUp(nSerial))
	{
		str.Format(_T("US: %d"), nSerial);
		PcrShare[0].bExist = TRUE;
		PcrShare[0].nSerial = nSerial;
		str2.Format(_T("PCR파일 Received - US: %d"), nSerial);
		pDoc->LogAuto(str2);
		MpeWrite(_T("ML45112"), (long)nSerial);		// 검사한 Panel의 AOI 상 Serial
		MpeWrite(_T("MB44012B"), 1);				// AOI 상 : PCR파일 Received
	}
	else
	{
		PcrShare[0].bExist = FALSE;
		PcrShare[0].nSerial = -1;
		str.Format(_T("US: "));
	}
	if (pFrm)
	{
		if (m_sShare[0] != str)
		{
			m_sShare[0] = str;
			pFrm->DispStatusBar(str, 4);
		}
	}
}

void CGvisRTR_PunchView::ChkShareDn()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShare) return;
	stPcrShare* PcrShare = (m_mgrStatus->PcrShare);

	CString str, str2;
	int nSerial, nAoiDnAutoSerial;

	if (ChkShareDn(nSerial))
	{
		str.Format(_T("DS: %d"), nSerial);
		PcrShare[1].bExist = TRUE;
		PcrShare[1].nSerial = nSerial;

		str2.Format(_T("PCR파일 Received - DS: %d"), nSerial);
		pDoc->LogAuto(str2);

		MpeWrite(_T("ML45114"), (long)nSerial);		// 검사한 Panel의 AOI 하 Serial
		MpeWrite(_T("MB44012C"), 1);				// AOI 하 : PCR파일 Received
	}
	else
	{
		PcrShare[1].bExist = FALSE;
		PcrShare[1].nSerial = -1;
		str.Format(_T("DS: "));
	}
	if (pFrm)
	{
		if (m_sShare[1] != str)
		{
			m_sShare[1] = str;
			pFrm->DispStatusBar(str, 2);
		}
	}
}

BOOL CGvisRTR_PunchView::ChkShare(int &nSerial)
{
	int nS0, nS1;
	BOOL b0 = ChkShareUp(nS0);
	BOOL b1 = ChkShareDn(nS1);

	if (!b0 || !b1)
	{
		nSerial = -1;
		return FALSE;
	}
	else if (nS0 != nS1)
	{
		nSerial = -1;
		return FALSE;
	}

	nSerial = nS0;
	return TRUE;
}

BOOL CGvisRTR_PunchView::ChkShareUp(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareUp + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CGvisRTR_PunchView::ChkShareDn(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareDn + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

void CGvisRTR_PunchView::ChkBuf()
{
	ChkBufUp();
	ChkBufDn();
}

void CGvisRTR_PunchView::ChkBufUp()
{
	CString str, sTemp;

	str = _T("UB: ");
	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
	{
		for (int i = 0; i < m_nBufTot[0]; i++)
		{
			DelOverLotEndSerialUp(m_pBufSerial[0][i]);

			if (i == m_nBufTot[0] - 1)
				sTemp.Format(_T("%d"), m_pBufSerial[0][i]);
			else
				sTemp.Format(_T("%d,"), m_pBufSerial[0][i]);
			str += sTemp;
		}
	}
	else
	{
		m_nBufTot[0] = 0;
	}

	if (pFrm)
	{
		if (m_sBuf[0] != str)
		{
			m_sBuf[0] = str;
			pFrm->DispStatusBar(str, 3);

			m_mgrProcedure->SetCurrentInfoBufUpTot(m_nBufTot[0]);
			for (int k = 0; k < m_nBufTot[0]; k++)
				m_mgrProcedure->SetCurrentInfoBufUp(k, m_pBufSerial[0][k]);

			if (m_nBufTot[0] == 1)
			{
				m_mgrStatus->General.nAoiCamInfoStrPcs[0] = m_mgrProcedure->GetAoiUpCamMstInfo(); // AOI상 strpcs.bin 연결
			}
		}
	}
}

void CGvisRTR_PunchView::ChkBufDn()
{
	CString str, sTemp;

	str = _T("DB: ");
	if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
	{
		for (int i = 0; i < m_nBufTot[1]; i++)
		{
			DelOverLotEndSerialDn(m_pBufSerial[1][i]);

			if (i == m_nBufTot[1] - 1)
				sTemp.Format(_T("%d"), m_pBufSerial[1][i]);
			else
				sTemp.Format(_T("%d,"), m_pBufSerial[1][i]);
			str += sTemp;
		}
	}
	else
	{
		m_nBufTot[1] = 0;
	}

	if (pFrm)
	{
		if (m_sBuf[1] != str)
		{
			m_sBuf[1] = str;
			pFrm->DispStatusBar(str, 1);

			m_mgrProcedure->SetCurrentInfoBufDnTot(m_nBufTot[1]);
			for (int k = 0; k < m_nBufTot[1]; k++)
				m_mgrProcedure->SetCurrentInfoBufDn(k, m_pBufSerial[1][k]);

			if (m_nBufTot[1] == 1)
			{
				m_mgrStatus->General.nAoiCamInfoStrPcs[1] = m_mgrProcedure->GetAoiDnCamMstInfo(); // AOI하 strpcs.bin 연결
			}
		}
	}
}

BOOL CGvisRTR_PunchView::ChkBufUp(int* pSerial, int &nTot)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufUp + _T("*.pcr"));
	if (!bExist)
	{
		m_mgrStatus->General.bBufEmpty[0] = TRUE;
		if (!m_mgrStatus->General.bBufEmptyF[0])
			m_mgrStatus->General.bBufEmptyF[0] = TRUE;		// 최초 한번 버퍼가 비어있으면(초기화를 하고 난 이후) TRUE.

		return FALSE; // pcr파일이 존재하지 않음.
	}

	int nPos, nSerial;

	CString sFileName, sSerial;
	CString sNewName;

	nTot = 0;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			sFileName = cFile.GetFileName();

			if (!SortingInUp(pDoc->WorkingInfo.System.sPathVrsBufUp + sFileName, nTot))
				return FALSE;

			nTot++;
		}
	}

	BOOL bRtn = SortingOutUp(pSerial, nTot);

	if (nTot == 0)
		m_mgrStatus->General.bBufEmpty[0] = TRUE;
	else
		m_mgrStatus->General.bBufEmpty[0] = FALSE;

	return (bRtn);
}

BOOL CGvisRTR_PunchView::ChkBufDn(int* pSerial, int &nTot)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufDn + _T("*.pcr"));
	if (!bExist)
	{
		m_mgrStatus->General.bBufEmpty[1] = TRUE;
		if (!m_mgrStatus->General.bBufEmptyF[1])
			m_mgrStatus->General.bBufEmptyF[1] = TRUE;
		return FALSE; // pcr파일이 존재하지 않음.
	}

	int nPos, nSerial;

	CString sFileName, sSerial;
	nTot = 0;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			sFileName = cFile.GetFileName();
			if (!SortingInDn(pDoc->WorkingInfo.System.sPathVrsBufDn + sFileName, nTot))
				return FALSE;

			nTot++;
		}
	}


	BOOL bRtn = SortingOutDn(pSerial, nTot);

	if (nTot == 0)
		m_mgrStatus->General.bBufEmpty[1] = TRUE;
	else
		m_mgrStatus->General.bBufEmpty[1] = FALSE;

	return (bRtn);
}

BOOL CGvisRTR_PunchView::SortingInUp(CString sPath, int nIndex)
{
	struct _stat buf;
	struct tm *t;

	CString sMsg, sFileName, sSerial;
	int nPos, nSerial;
	char filename[MAX_PATH];
	StringToChar(sPath, filename);

	if (_stat(filename, &buf) != 0)
	{
		sMsg.Format(_T("일시정지 - Failed getting information."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}
	else
	{
		sFileName = sPath;
		nPos = sFileName.ReverseFind('.');
		if (nPos > 0)
		{
			sSerial = sFileName.Left(nPos);
			sSerial = sSerial.Right(4);
		}

		nSerial = _tstoi(sSerial);

		t = localtime(&buf.st_ctime);

		CString sYear, sMonth, sDay, sHour, sMin, sSec;
		sYear.Format(_T("%04d"), t->tm_year + 1900);
		sMonth.Format(_T("%02d"), t->tm_mon + 1);
		sDay.Format(_T("%02d"), t->tm_mday);
		sHour.Format(_T("%02d"), t->tm_hour);
		sMin.Format(_T("%02d"), t->tm_min);
		sSec.Format(_T("%02d"), t->tm_sec);

		__int64 nYear = _tstoi(sYear);
		__int64 nMonth = _tstoi(sMonth);
		__int64 nDay = _tstoi(sDay);
		__int64 nHour = _tstoi(sHour);
		__int64 nMin = _tstoi(sMin);
		__int64 nSec = _tstoi(sSec);

		m_nBufSerialSorting[0][nIndex] = nYear * 100000000000000 + nMonth * 1000000000000 + nDay * 10000000000 +
			nHour * 100000000 + nMin * 1000000 + nSec * 10000 + nSerial;
	}

	return TRUE;
}

BOOL CGvisRTR_PunchView::SortingOutUp(int* pSerial, int nTot)
{
	int i, k;

	for (k = 0; k < nTot; k++) 			// 버블 정렬 소스 시작
	{
		for (i = 0; i < (nTot - 1) - k; i++)
		{

			if (m_nBufSerialSorting[0][i] > m_nBufSerialSorting[0][i + 1])
			{
				SwapUp(&m_nBufSerialSorting[0][i + 1], &m_nBufSerialSorting[0][i]);
			}
		}
	} // 버블 정렬 소스 끝

	for (i = 0; i < nTot; i++)
	{
		pSerial[i] = (int)(m_nBufSerialSorting[0][i] % 10000);
	}
	return TRUE;
}

BOOL CGvisRTR_PunchView::SortingInDn(CString sPath, int nIndex)
{
	struct _stat buf;
	struct tm *t;

	CString sMsg, sFileName, sSerial;
	int nPos, nSerial;
	char filename[MAX_PATH];
	StringToChar(sPath, filename);

	if (_stat(filename, &buf) != 0)
	{
		sMsg.Format(_T("일시정지 - Failed getting information."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}
	else
	{
		sFileName = sPath;
		nPos = sFileName.ReverseFind('.');
		if (nPos > 0)
		{
			sSerial = sFileName.Left(nPos);
			sSerial = sSerial.Right(4);
		}

		nSerial = _tstoi(sSerial);

		t = localtime(&buf.st_ctime);

		CString sYear, sMonth, sDay, sHour, sMin, sSec;
		sYear.Format(_T("%04d"), t->tm_year + 1900);
		sMonth.Format(_T("%02d"), t->tm_mon + 1);
		sDay.Format(_T("%02d"), t->tm_mday);
		sHour.Format(_T("%02d"), t->tm_hour);
		sMin.Format(_T("%02d"), t->tm_min);
		sSec.Format(_T("%02d"), t->tm_sec);

		__int64 nYear = _tstoi(sYear);
		__int64 nMonth = _tstoi(sMonth);
		__int64 nDay = _tstoi(sDay);
		__int64 nHour = _tstoi(sHour);
		__int64 nMin = _tstoi(sMin);
		__int64 nSec = _tstoi(sSec);

		m_nBufSerialSorting[1][nIndex] = nYear * 100000000000000 + nMonth * 1000000000000 + nDay * 10000000000 +
			nHour * 100000000 + nMin * 1000000 + nSec * 10000 + nSerial;

	}

	return TRUE;
}

BOOL CGvisRTR_PunchView::SortingOutDn(int* pSerial, int nTot)
{
	int i, k;

	for (k = 0; k < nTot; k++) 			// 버블 정렬 소스 시작
	{
		for (i = 0; i < (nTot - 1) - k; i++)
		{

			if (m_nBufSerialSorting[1][i] > m_nBufSerialSorting[1][i + 1])
			{
				SwapUp(&m_nBufSerialSorting[1][i + 1], &m_nBufSerialSorting[1][i]);
			}
		}
	} // 버블 정렬 소스 끝

	for (i = 0; i < nTot; i++)
	{
		pSerial[i] = (int)(m_nBufSerialSorting[1][i] % 10000);
	}
	return TRUE;
}

void CGvisRTR_PunchView::SwapDn(__int64 *num1, __int64 *num2) 	// 위치 바꾸는 함수
{
	__int64 temp;

	temp = *num2;
	*num2 = *num1;
	*num1 = temp;
}

void CGvisRTR_PunchView::SwapUp(__int64 *num1, __int64 *num2) 	// 위치 바꾸는 함수
{
	__int64 temp;

	temp = *num2;
	*num2 = *num1;
	*num1 = temp;
}

void CGvisRTR_PunchView::SetListBuf()	// m_mgrStatus->ListBuf에 버퍼 폴더의 시리얼번호를 가지고 재갱신함.
{
	m_mgrStatus->ListBuf[0].Clear();
	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
	{
		for (int i = 0; i < m_nBufTot[0]; i++)
			m_mgrStatus->ListBuf[0].Push(m_pBufSerial[0][i]);
	}

	m_mgrStatus->ListBuf[1].Clear();
	if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
	{
		for (int i = 0; i < m_nBufTot[1]; i++)
			m_mgrStatus->ListBuf[1].Push(m_pBufSerial[1][i]);
	}
}

BOOL CGvisRTR_PunchView::IsBuffer(int nNum)
{
	if (!m_mgrStatus)
		return FALSE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	stGeneral& General = (m_mgrStatus->General);

	if (pDoc->GetAoiUpVsStatus())
	{
		if (!m_mgrProcedure->IsLotEnd()) // m_bLotEnd
			nNum = 1;

		if (General.bLastProc) // m_bLastProc
		{
			int nLastShot = GetLotEndSerial();
			if (bDualTest)
			{
				if (m_mgrStatus->ListBuf[0].GetLast() == nLastShot && m_mgrStatus->ListBuf[1].GetLast() == nLastShot)
				{
					nNum = 0;
					if (!m_mgrProcedure->IsLotEnd())
						General.bLotEndF = TRUE;
				}
				else
					nNum = 1;
			}
			else
			{
				if (m_mgrStatus->ListBuf[0].GetLast() == nLastShot)
				{
					nNum = 0;
					if (!m_mgrProcedure->IsLotEnd())
						General.bLotEndF = TRUE;
				}
				else
					nNum = 1;
			}
		}
	}

	if (bDualTest)
	{
		if (m_nBufTot[0] > nNum && m_nBufTot[1] > nNum) // [0]: AOI-Up , [1]: AOI-Dn
			return TRUE;

		if (General.bLastProc)
		{
			if ((m_nBufTot[0] > nNum || General.bBufEmpty[0]) && (m_nBufTot[1] > nNum || General.bBufEmpty[1])) // [0]: AOI-Up , [1]: AOI-Dn
				return TRUE;
		}
	}
	else
	{
		if (m_nBufTot[0] > nNum) // [0]: AOI-Up
			return TRUE;
	}

	return FALSE;
}

BOOL CGvisRTR_PunchView::IsBufferUp()
{
	if (m_nBufTot[0] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisRTR_PunchView::IsBufferDn()
{
	if (m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

int CGvisRTR_PunchView::GetBuffer(int *pPrevSerial)
{
	int nS0 = GetBufferUp(pPrevSerial);
	int nS1 = GetBufferDn(pPrevSerial);
	if (nS0 != nS1)
		return 0;
	return nS0;
}

int CGvisRTR_PunchView::GetBufferUp(int *pPrevSerial)
{
	if (IsBufferUp())
		return m_pBufSerial[0][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][0];
	return 0;
}

int CGvisRTR_PunchView::GetBufferDn(int *pPrevSerial)
{
	if (IsBufferDn())
		return m_pBufSerial[1][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][0];
	return 0;
}

BOOL CGvisRTR_PunchView::IsBuffer0()
{
	if (m_nBufTot[0] > 0 && m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisRTR_PunchView::IsBufferUp0()
{
	if (m_nBufTot[0] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisRTR_PunchView::IsBufferDn0()
{
	if (m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

int CGvisRTR_PunchView::GetBuffer0(int *pPrevSerial)
{
	int nS0 = GetBufferUp0(pPrevSerial);
	int nS1 = GetBufferDn0(pPrevSerial);
	if (nS0 != nS1)
		return 0;
	return nS0;
}

int CGvisRTR_PunchView::GetBufferUp0(int *pPrevSerial)
{
	if (IsBufferUp0())
		return m_pBufSerial[0][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][0];
	return 0;
}

int CGvisRTR_PunchView::GetBufferDn0(int *pPrevSerial)
{
	if (IsBufferDn0())
		return m_pBufSerial[1][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][0];
	return 0;
}

BOOL CGvisRTR_PunchView::IsBuffer1()
{
	if (m_nBufTot[0] > 1 && m_nBufTot[1] > 1)
		return TRUE;
	return FALSE;
}

BOOL CGvisRTR_PunchView::IsBufferUp1()
{
	if (m_nBufTot[0] > 1)
		return TRUE;
	return FALSE;
}

BOOL CGvisRTR_PunchView::IsBufferDn1()
{
	if (m_nBufTot[1] > 1)
		return TRUE;
	return FALSE;
}

int CGvisRTR_PunchView::GetBuffer1(int *pPrevSerial)
{
	int nS0 = GetBufferUp1(pPrevSerial);
	int nS1 = GetBufferDn1(pPrevSerial);
	if (nS0 != nS1)
		return 0;
	return nS0;
}

int CGvisRTR_PunchView::GetBufferUp1(int *pPrevSerial)
{
	if (IsBufferUp1())
		return m_pBufSerial[0][1];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][1];
	return 0;
}

int CGvisRTR_PunchView::GetBufferDn1(int *pPrevSerial)
{
	if (IsBufferDn1())
		return m_pBufSerial[1][1];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][1];
	return 0;
}

BOOL CGvisRTR_PunchView::IsShare()
{
	stGeneral& General = (m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if(General.bWaitPcr[0] && General.bWaitPcr[1])
		{
			if (IsShareUp() && IsShareDn())
			{
				General.bWaitPcr[0] = FALSE;
				General.bWaitPcr[1] = FALSE;
				return TRUE;
			}
		}
		else if (General.bWaitPcr[0] && !General.bWaitPcr[1])
		{
			if (IsShareUp())
			{
				General.bWaitPcr[0] = FALSE;
				return TRUE;
			}
		}
		else if (!General.bWaitPcr[0] && General.bWaitPcr[1])
		{
			if (IsShareDn())
			{
				General.bWaitPcr[1] = FALSE;
				return TRUE;
			}
		}
		else
		{
			if (IsShareUp() || IsShareDn())
				return TRUE;
		}
	}
	else
	{
		if (General.bWaitPcr[0])
		{
			if (IsShareUp())
			{
				General.bWaitPcr[0] = FALSE;
				return TRUE;
			}
		}
		else
		{
			if (IsShareUp())
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CGvisRTR_PunchView::IsShareUp()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShare) return FALSE;
	stPcrShare* PcrShare = (m_mgrStatus->PcrShare);
	return PcrShare[0].bExist;
}

BOOL CGvisRTR_PunchView::IsShareDn()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShare) return FALSE;
	stPcrShare* PcrShare = (m_mgrStatus->PcrShare);
	return PcrShare[1].bExist;
}

int CGvisRTR_PunchView::GetShareUp()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShare) return 0;
	stPcrShare* PcrShare = (m_mgrStatus->PcrShare);
	return PcrShare[0].nSerial;
}

int CGvisRTR_PunchView::GetShareDn()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShare) return 0;
	stPcrShare* PcrShare = (m_mgrStatus->PcrShare);
	return PcrShare[1].nSerial;
}

BOOL CGvisRTR_PunchView::IsShareVs()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (IsShareVsUp() || IsShareVsDn())
			return TRUE;
	}
	else
	{
		if (IsShareVsUp())
			return TRUE;
	}
	return FALSE;
}

BOOL CGvisRTR_PunchView::IsShareVsUp()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShareVs) return FALSE;
	stPcrShare* PcrShareVs = (m_mgrStatus->PcrShareVs);
	return PcrShareVs[0].bExist;
}

BOOL CGvisRTR_PunchView::IsShareVsDn()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShareVs) return FALSE;
	stPcrShare* PcrShareVs = (m_mgrStatus->PcrShareVs);
	return PcrShareVs[1].bExist;
}

int CGvisRTR_PunchView::GetShareVsUp()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShareVs) return 0;
	stPcrShare* PcrShareVs = (m_mgrStatus->PcrShareVs);
	return PcrShareVs[0].nSerial;
}

int CGvisRTR_PunchView::GetShareVsDn()
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShareVs) return 0;
	stPcrShare* PcrShareVs = (m_mgrStatus->PcrShareVs);
	return PcrShareVs[1].nSerial;
}

void CGvisRTR_PunchView::DelOverLotEndSerialUp(int nSerial)
{
	CString sSrc;

	if (nSerial > 0)
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);

		if (m_mgrStatus->General.bSerialDecrese)
		{
			if (m_mgrStatus->General.nLotEndSerial > 0 && nSerial < m_mgrStatus->General.nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
		else
		{
			if (m_mgrStatus->General.nLotEndSerial > 0 && nSerial > m_mgrStatus->General.nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
	}

}

void CGvisRTR_PunchView::DelOverLotEndSerialDn(int nSerial)
{
	CString sSrc;

	if (nSerial > 0)
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);

		if (m_mgrStatus->General.bSerialDecrese)
		{
			if (m_mgrStatus->General.nLotEndSerial > 0 && nSerial < m_mgrStatus->General.nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
		else
		{
			if (m_mgrStatus->General.nLotEndSerial > 0 && nSerial > m_mgrStatus->General.nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
	}

}

void CGvisRTR_PunchView::ChkReTestAlarmOnAoiUp()
{
	CString sMsg;
	sMsg.Format(_T("U%03d"), pDoc->GetAoiUpAutoSerial());
	DispStsBar(sMsg, 0);

	int nSerial = m_pBufSerial[0][m_nBufTot[0] - 1];
	stGeneral& General = (m_mgrStatus->General);

	if (General.bSerialDecrese)
	{
		if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
		{
			pDoc->SetAoiUpAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
			MpeWrite(_T("MB44013B"), 1); // 검사부 상부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
		else if (General.nLotEndSerial > 0 && nSerial <= General.nLotEndSerial)
		{
			MpeWrite(_T("MB44012B"), 1); // AOI 상 : PCR파일 Received
		}
	}
	else
	{
		if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
		{
			pDoc->SetAoiUpAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
			MpeWrite(_T("MB44013B"), 1); // 검사부 상부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
		else if (General.nLotEndSerial > 0 && nSerial >= General.nLotEndSerial)
		{
			MpeWrite(_T("MB44012B"), 1); // AOI 상 : PCR파일 Received
		}
	}

}

void CGvisRTR_PunchView::ChkReTestAlarmOnAoiDn()
{
	CString sMsg;
	sMsg.Format(_T("D%03d"), pDoc->GetAoiDnAutoSerial());
	pView->DispStsBar(sMsg, 0);

	stGeneral& General = (m_mgrStatus->General);
	int nSerial = m_pBufSerial[1][m_nBufTot[1] - 1];

	if (General.bSerialDecrese)
	{
		if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
		{
			pDoc->SetAoiDnAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
			MpeWrite(_T("MB44013C"), 1); // 검사부 하부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
		else if (General.nLotEndSerial > 0 && nSerial <= General.nLotEndSerial)
		{
			;
		}
	}
	else
	{
		if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
		{
			pDoc->SetAoiDnAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
			MpeWrite(_T("MB44013C"), 1); // 검사부 하부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
		else if (General.nLotEndSerial > 0 && nSerial >= General.nLotEndSerial)
		{
			;
		}
	}

}

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
}

void CGvisRTR_PunchView::CreateMgrProcedure()
{
	if (m_mgrProcedure)
	{
		delete m_mgrProcedure;
		m_mgrProcedure = NULL;
	}
	m_mgrProcedure = new CManagerProcedure(this);
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

	if (bEngave)
	{
		if (m_mgrProcedure && m_mgrProcedure->m_pEngrave)
			m_mgrProcedure->m_pEngrave->SetMsgBox(sMsg, nType);
	}

	if (m_pDlgMyMsg)
		nRtnVal = m_pDlgMyMsg->SyncMsgBox(sMsg, nThreadIdx, nType, nTimOut);

	return nRtnVal;
}

void CGvisRTR_PunchView::SetMyMsgYes()
{
	if (m_pDlgMyMsg)
	{
		if (m_pDlgMyMsg->m_pDlgMyMsgSub01)
		{
			((CDlgMyMsgSub01*)(m_pDlgMyMsg->m_pDlgMyMsgSub01))->ClickYes();
		}
	}
}

void CGvisRTR_PunchView::SetMyMsgNo()
{
	if (m_pDlgMyMsg)
	{
		if (m_pDlgMyMsg->m_pDlgMyMsgSub01)
		{
			((CDlgMyMsgSub01*)(m_pDlgMyMsg->m_pDlgMyMsgSub01))->ClickNo();
		}
	}
}

void CGvisRTR_PunchView::SetMyMsgOk()
{
	if (m_pDlgMyMsg)
	{
		if (m_pDlgMyMsg->m_pDlgMyMsgSub02)
		{
			((CDlgMyMsgSub02*)(m_pDlgMyMsg->m_pDlgMyMsgSub02))->ClickOk();
		}
	}
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
	m_pDlgMenu07 = NULL;
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

	case IDD_DLG_MENU_07:
		if (!m_pDlgMenu07)
			m_pDlgMenu07 = new CDlgMenu07(this);
		if (m_pDlgMenu07->GetSafeHwnd())
			m_pDlgMenu07->ShowWindow(SW_SHOW);
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
	if (m_pDlgMenu07 && m_pDlgMenu07->GetSafeHwnd())
	{
		if (m_pDlgMenu07->IsWindowVisible())
			m_pDlgMenu07->ShowWindow(SW_HIDE);
	}
}

void CGvisRTR_PunchView::DelAllDlg()
{
	if (m_pDlgMenu07 != NULL)
	{
		delete m_pDlgMenu07;
		m_pDlgMenu07 = NULL;
	}
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

BOOL CGvisRTR_PunchView::IsRun()
{
	if (m_mgrFeeding)
		return m_mgrFeeding->IsRun();
	return FALSE;
	//if (m_sDispMain == _T("운전중") || m_sDispMain == _T("초기운전") || m_sDispMain == _T("단면샘플")
	//	|| m_sDispMain == _T("단면검사") || m_sDispMain == _T("내층검사") || m_sDispMain == _T("외층검사")
	//	|| m_sDispMain == _T("중층검사") || m_sDispMain == _T("양면검사") || m_sDispMain == _T("양면샘플"))
	//	return TRUE;
	//return FALSE;
}

CString CGvisRTR_PunchView::GetDispMain()
{
	//return m_stDispMain.sMsg;
	return m_sDispMain;
}

void CGvisRTR_PunchView::DispMain(CString sMsg, COLORREF rgb)
{
	pDoc->SetMonDispMain(sMsg);

	m_stDispMain.sMsg = sMsg;
	m_stDispMain.rgb = rgb;

	if (sMsg == _T("정 지"))
	{
		pView->SetMkMenu03(_T("Main"), _T("Stop"), TRUE);
		pView->SetMkMenu03(_T("Main"), _T("Run"), FALSE);
	}
	else if (sMsg == _T("운전준비"))
	{
		pView->SetMkMenu03(_T("Main"), _T("Ready"), TRUE);
	}
	else
	{
		pView->SetMkMenu03(_T("Main"), _T("Run"), TRUE);
		pView->SetMkMenu03(_T("Main"), _T("Stop"), FALSE);
	}

	sMsg.Empty();
}

int CGvisRTR_PunchView::DoDispMain()
{
	int nRtn = -1;

	if (m_stDispMain.sMsg.IsEmpty())
		return nRtn;

	if (m_pDlgMenu01)
	{
		CString sMsg = m_stDispMain.sMsg;
		COLORREF rgb = m_stDispMain.rgb;
		m_sDispMain = sMsg;
		m_pDlgMenu01->DispMain(sMsg, rgb);
		m_stDispMain.sMsg = _T("");
		m_stDispMain.rgb = RGB_WHITE;

		return 0;
	}

	return nRtn;
}

void CGvisRTR_PunchView::DispContRun(BOOL bOn)
{
	if (pDoc->WorkingInfo.LastJob.bDispContRun != bOn)
	{
		pDoc->WorkingInfo.LastJob.bDispContRun = bOn;
		pDoc->SetMkInfo(_T("Signal"), _T("DispContRun"), bOn);

#ifdef USE_ENGRAVE
		if (m_mgrProcedure->m_pEngrave)
			m_mgrProcedure->m_pEngrave->SetDispContRun();	//_stSigInx::_DispContRun
#endif
	}
}

int CGvisRTR_PunchView::MyPassword(CString strMsg, int nCtrlId)
{
	CDlgMyPassword dlg1(this);
	dlg1.SetMsg(strMsg, nCtrlId);
	dlg1.DoModal();
	return (dlg1.m_nRtnVal);

}

void CGvisRTR_PunchView::EnableItsMode(BOOL bEnable)
{
	if(m_pDlgMenu01)
		m_pDlgMenu01->EnableItsMode(bEnable);
}

void CGvisRTR_PunchView::SetDualTest(BOOL bOn)
{
	MpeWrite(_T("MB44017A"), (long)(bOn ? 0 : 1));		// 단면 검사 On

	if (m_pDlgFrameHigh)
		m_pDlgFrameHigh->SetDualTest(bOn);
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetDualTest(bOn);
	if (m_pDlgMenu03)
		m_pDlgMenu03->SetDualTest(bOn);
	if (m_pDlgMenu06)
		m_pDlgMenu06->SetDualTest(bOn);
}

void CGvisRTR_PunchView::SetTestMode(int nMode)
{
	pDoc->SetTestMode(nMode); // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2 .

	if (pDoc->GetTestMode() == MODE_INNER)
	{
		MpeWrite(_T("MB440172"), 1);// 내층 검사 사용/미사용 
		MpeWrite(_T("MB440176"), 0);// 외층 검사 사용/미사용
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->EnableItsMode(FALSE);
	}
	else if (pDoc->GetTestMode() == MODE_OUTER)
	{
		MpeWrite(_T("MB440172"), 0);// 내층 검사 사용/미사용
		MpeWrite(_T("MB440176"), 1);// 외층 검사 사용/미사용
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), TRUE);
		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->EnableItsMode();
	}
	else
	{
		MpeWrite(_T("MB440172"), 0);// 내층 검사 사용/미사용
		MpeWrite(_T("MB440176"), 0);// 외층 검사 사용/미사용
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->EnableItsMode(FALSE);
	}
}

void CGvisRTR_PunchView::SetAlarm(CString sMsg)
{
	if(m_mgrProcedure->m_pEngrave)
		m_mgrProcedure->m_pEngrave->SetAlarm(sMsg);
}

void CGvisRTR_PunchView::ClrAlarm()
{
	m_sAlmMsg = _T("");
	m_sIsAlmMsg = _T("");
	m_sPrevAlmMsg = _T("");
}

int CGvisRTR_PunchView::GetLastShotMk()
{
	int nLastShot = 0;
	if (m_pDlgFrameHigh)
		nLastShot = m_pDlgFrameHigh->GetLastShotMk();
	return nLastShot;
}

int CGvisRTR_PunchView::GetLastShotUp()
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nAoiLastShot[0];

	return (nLastShot);
}

int CGvisRTR_PunchView::GetLastShotDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nAoiLastShot[1];

	return (nLastShot);
}

BOOL CGvisRTR_PunchView::GetMkStSignal()
{
	return GetMpeSignal(1, 0);
}

void CGvisRTR_PunchView::ResetMkStSignal()
{
	MpeWrite(_T("MB440110"), 0); // 마킹시작(PC가 확인하고 Reset시킴.)
}

BOOL CGvisRTR_PunchView::GetMpeSignal(int nSection, int nName)
{
	BOOL bVal = FALSE;
	if (m_mgrFeeding)
		bVal = m_mgrFeeding->GetMpeSignal(nSection, nName);
	return bVal;
}

long CGvisRTR_PunchView::GetMpeData(int nSection, int nName)
{
	long nVal = 0;
	if (m_mgrFeeding)
		nVal = m_mgrFeeding->GetMpeData(nSection, nName);
	return nVal;
}

BOOL CGvisRTR_PunchView::MpeWrite(CString strRegAddr, long lData, BOOL bCheck)
{
	return m_mgrFeeding->MpeWrite(strRegAddr, lData, bCheck);
}

void CGvisRTR_PunchView::Auto()
{
	if (m_mgrProcedure)
		m_mgrProcedure->Auto();
}

BOOL CGvisRTR_PunchView::IsAuto()
{
	BOOL bVal = 0;
	if (m_mgrFeeding)
		bVal = m_mgrFeeding->IsAuto();
	return bVal;
}

void CGvisRTR_PunchView::ResetMotion()
{
	if (m_mgrPunch)
		m_mgrPunch->ResetMotion();
}

void CGvisRTR_PunchView::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

	//3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}

DWORD CGvisRTR_PunchView::GetLotSt()
{
	return m_dwLotSt;
}

DWORD CGvisRTR_PunchView::GetLotEd()
{
	return m_dwLotEd;
}

void CGvisRTR_PunchView::SetLotSt()
{
	stLotTime LotTime;
	GetTime(LotTime);

	pDoc->WorkingInfo.Lot.StTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.StTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.StTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.StTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.StTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.StTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.EdTime.nYear = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMonth = 0;
	pDoc->WorkingInfo.Lot.EdTime.nDay = 0;
	pDoc->WorkingInfo.Lot.EdTime.nHour = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMin = 0;
	pDoc->WorkingInfo.Lot.EdTime.nSec = 0;

	m_dwLotSt = GetTickCount();
	pDoc->SaveLotTime(m_dwLotSt);
	DispLotTime();

	//BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	//if (pDoc->m_pReelMap)
	//	pDoc->m_pReelMap->SetLotSt();
	//if (pDoc->m_pReelMapUp)
	//	pDoc->m_pReelMapUp->SetLotSt();
	//if (bDualTest)
	//{
	//	if (pDoc->m_pReelMapDn)
	//		pDoc->m_pReelMapDn->SetLotSt();
	//	if (pDoc->m_pReelMapAllUp)
	//		pDoc->m_pReelMapAllUp->SetLotSt();
	//	if (pDoc->m_pReelMapAllDn)
	//		pDoc->m_pReelMapAllDn->SetLotSt();
	//}
}

void CGvisRTR_PunchView::SetLotEd()
{
	stLotTime LotTime;
	GetTime(LotTime);

	pDoc->WorkingInfo.Lot.EdTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.EdTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.EdTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.EdTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.EdTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.EdTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

	m_dwLotEd = GetTickCount();

	pDoc->SaveLotTime(pDoc->WorkingInfo.Lot.dwStTick);
	DispLotTime();

	//BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	//if (pDoc->m_pReelMap)
	//	pDoc->m_pReelMap->SetLotEd();
	//if (pDoc->m_pReelMapUp)
	//	pDoc->m_pReelMapUp->SetLotEd();
	//if (bDualTest)
	//{
	//	if (pDoc->m_pReelMapDn)
	//		pDoc->m_pReelMapDn->SetLotEd();
	//	if (pDoc->m_pReelMapAllUp)
	//		pDoc->m_pReelMapAllUp->SetLotEd();
	//	if (pDoc->m_pReelMapAllDn)
	//		pDoc->m_pReelMapAllDn->SetLotEd();
	//}
}

void CGvisRTR_PunchView::DispLotTime()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->DispLotTime();
}

void CGvisRTR_PunchView::SetMkMenu01(CString sMenu, CString sItem, CString sData)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkMenu01;

	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CGvisRTR_PunchView::SetMkMenu03(CString sMenu, CString sItem, BOOL bOn)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkMenu03;
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), bOn > 0 ? 1 : 0);
	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

BOOL CGvisRTR_PunchView::IsEnableBtn(int nId)
{
	BOOL bRtn = FALSE;

	switch (nId)
	{
	case IDD_DLG_INFO:
		break;

	case IDD_DLG_FRAME_HIGH:
		break;

	case IDD_DLG_MENU_01:
		if (m_pDlgMenu01)
			bRtn = m_pDlgMenu01->IsEnableBtn();
		break;

	case IDD_DLG_MENU_02:
		break;

	case IDD_DLG_MENU_03:
		break;

	case IDD_DLG_MENU_04:
		break;

	case IDD_DLG_MENU_05:
		break;

	case IDD_DLG_MENU_06:
		break;
	}

	return bRtn;
}

void CGvisRTR_PunchView::EnableBtn(int nId, BOOL bEnable)
{
	switch (nId)
	{
	case IDD_DLG_INFO:
		break;

	case IDD_DLG_FRAME_HIGH:
		break;

	case IDD_DLG_MENU_01:
		if (m_pDlgMenu01)
		{
			if (m_pDlgMenu01->IsEnableBtn() != bEnable)
				m_pDlgMenu01->EnableBtn(bEnable);
		}
		break;

	case IDD_DLG_MENU_02:
		break;

	case IDD_DLG_MENU_03:
		break;

	case IDD_DLG_MENU_04:
		break;

	case IDD_DLG_MENU_05:
		break;

	case IDD_DLG_MENU_06:
		break;
	}
}

void CGvisRTR_PunchView::SetLed(int nIdx, BOOL bOn)
{
	if (m_pDlgMenu03)
		m_pDlgMenu03->SetLed(nIdx, bOn);
}

int CGvisRTR_PunchView::GetLotEndSerial()
{
	if(!m_mgrStatus)
		return 0;

	stGeneral& General = (m_mgrStatus->General);
	return General.nLotEndSerial; //General.nLotEndSerial; // 테이블상에 정지하는 Serial.
}

void CGvisRTR_PunchView::SetLotEnd(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.61"));
		return;
	}

	if (!m_mgrStatus) return;
	stGeneral& General = (m_mgrStatus->General);

	General.nLotEndSerial = nSerial;
	//m_nAoiLastSerial[0] = nSerial;

	CString str;
	str.Format(_T("%d"), General.nLotEndSerial);
	DispStsBar(str, 0);
	if (m_pDlgMenu01)
		m_pDlgMenu01->DispLotEndSerial(General.nLotEndSerial);
}

void CGvisRTR_PunchView::UpdateRst()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateRst();
}

void CGvisRTR_PunchView::Buzzer(BOOL bOn, int nCh)
{
	if (m_mgrFeeding)
		m_mgrFeeding->Buzzer(bOn, nCh);
}

void CGvisRTR_PunchView::Stop()
{
	DispMain(_T("정 지"), RGB_RED);
	MpeWrite(_T("MB440162"), 1);
}

void CGvisRTR_PunchView::SetAoiUpOffset(CfPoint &OfSt)
{
	if (m_pDlgMenu02)
		m_pDlgMenu02->SetAoiUpOffset(OfSt);
}

void CGvisRTR_PunchView::SetAoiDnOffset(CfPoint &OfSt)
{
	if (m_pDlgMenu02)
		m_pDlgMenu02->SetAoiDnOffset(OfSt);
}

void CGvisRTR_PunchView::SetEngOffset(CfPoint &OfSt)
{
	if (m_pDlgMenu02)
		m_pDlgMenu02->SetEngOffset(OfSt);
}

void CGvisRTR_PunchView::DispLotEndSerial(int nSerial)
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->DispLotEndSerial(nSerial);
}

void CGvisRTR_PunchView::DispLotEnd(BOOL bOn)
{
	if (pDoc->WorkingInfo.LastJob.bDispLotEnd != bOn)
	{
		pDoc->WorkingInfo.LastJob.bDispLotEnd = bOn;
		pDoc->SetMkInfo(_T("Signal"), _T("DispLotEnd"), bOn);

#ifdef USE_ENGRAVE
		if (pView && pView->m_pEngrave)
			pView->m_pEngrave->SetDispLotEnd();	//_stSigInx::_DispLotEnd
#endif

	}
}

void CGvisRTR_PunchView::DispLotStTime()
{
	//char szData[MAX_PATH];
	TCHAR szData[MAX_PATH];
	CString sPath = PATH_WORKING_INFO;
	// [Lot]
	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Tick"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.dwStTick = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.dwStTick = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Year"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nYear = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Month"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nMonth = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Day"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nDay = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Hour"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nHour = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Minute"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nMin = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Second"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nSec = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nSec = 0;

	m_dwLotSt = (DWORD)pDoc->WorkingInfo.Lot.dwStTick;
	DispLotTime();
}

BOOL CGvisRTR_PunchView::IsReady()
{
	if (!m_mgrStatus) return FALSE;
	return m_mgrStatus->General.bReadyDone;
}

BOOL CGvisRTR_PunchView::IsAuto()
{
	if (!m_mgrFeeding) return FALSE;
	return m_mgrFeeding->Status.bAuto;
}

BOOL CGvisRTR_PunchView::IsAoiLdRun()
{
	if (!m_mgrFeeding) return FALSE;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (!m_mgrFeeding->IsLoaderOnAoiUp() || !m_mgrFeeding->IsLoaderOnAoiDn())
			return FALSE;
	}
	else
	{
		if (!m_mgrFeeding->IsLoaderOnAoiUp())
			return FALSE;
	}

	return TRUE;
}

void CGvisRTR_PunchView::DispInfo()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateData();

	//if (m_pDlgMenu05)
	//{
	//	m_pDlgMenu05->InitModel();
	//	if (m_pDlgMenu05->IsWindowVisible())
	//		m_pDlgMenu05->AtDlgShow();
	//}
}

void CGvisRTR_PunchView::ResetMkInfo(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
{
	if (!m_mgrReelmap) return;
	CCamMaster* pMaster = m_mgrReelmap->m_Master;
	CCamMaster* pMasterInner = m_mgrReelmap->m_MasterInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bDualTestInner, bGetCurrentInfoEng;
	CString sLot, sLayerUp, sLayerDn;
	bGetCurrentInfoEng = pDoc->GetCurrentInfoEng();

	// CamMst Info...
	pDoc->GetCamPxlRes();

	if (nAoi == 0 || nAoi == 2)
	{
		if (!bDualTest)
		{
			m_bDrawGL_Menu01 = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->ResetMkInfo();
		}

		if (bGetCurrentInfoEng)
		{
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
				{
					if (pMasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
					{
						if (!bDualTestInner)
						{
							m_bDrawGL_Menu06 = FALSE;
							if (m_pDlgMenu06)
								m_pDlgMenu06->ResetMkInfo();
						}
					}
				}
			}
		}

		if (IsLastJob(0)) // Up
		{
			pMaster[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLayerUp);
			pMaster[0].LoadMstInfo();

			if (m_pEngrave)
				m_pEngrave->SwMenu01UpdateWorking(TRUE);

			if (bGetCurrentInfoEng)
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
					{
						if (pMasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
						{
							pMasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModelUp,
								sLayerUp);
							pMasterInner[0].LoadMstInfo();

							if (bDualTestInner)
							{
								pMasterInner[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
									pDoc->WorkingInfo.LastJob.sModelUp,
									sLayerDn);
								pMasterInner[1].LoadMstInfo();

								m_bDrawGL_Menu06 = FALSE;
								if (m_pDlgMenu06)
									m_pDlgMenu06->ResetMkInfo();
							}
						}
					}
				}
			}
		}
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - IsLastJob(0)..."));
		}

		m_mgrReelmap->InitReelmapUp();

		if (bGetCurrentInfoEng)
		{
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
				{
					if (pMasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
					{
						m_mgrReelmap->InitReelmapInnerUp();
						if (bDualTestInner)
							m_mgrReelmap->InitReelmapInnerDn();
					}
				}
			}
		}

		m_mgrReelmap->OpenReelmap();
		SetAlignPosUp();

		InitReelmapDisp();
		InitCamImgDisp();
	}


	if (bDualTest)
	{
		if (nAoi == 1 || nAoi == 2)
		{
			m_bDrawGL_Menu01 = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->ResetMkInfo();

			if (IsLastJob(1)) // Dn
			{
				pMaster[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
					pDoc->WorkingInfo.LastJob.sModelUp,
					pDoc->WorkingInfo.LastJob.sLayerDn,
					pDoc->WorkingInfo.LastJob.sLayerUp);

				pMaster[1].LoadMstInfo();
			}
			else
			{
				AfxMessageBox(_T("Error - IsLastJob(1)..."));
			}

			m_mgrReelmap->InitReelmapDn();
			SetAlignPosDn();

			InitReelmapDisp();
			InitCamImgDisp();
		}
	}
}

void CGvisRTR_PunchView::SetAlignPos()
{
	if (m_mgrPunch)
		m_mgrPunch->SetAlignPos();
}

void CGvisRTR_PunchView::SetAlignPosUp()
{
	if (m_mgrPunch)
		m_mgrPunch->SetAlignPosUp();
}

void CGvisRTR_PunchView::SetAlignPosDn()
{
	if (m_mgrPunch)
		m_mgrPunch->SetAlignPosDn();
}

void CGvisRTR_PunchView::ModelChange(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn 
{
	if (!m_mgrStatus || !m_mgrStatus->PcrShare) return;
	stPcrShare* PcrShare = (m_mgrStatus->PcrShare);

	if (nAoi == 0)
	{
		m_mgrProcedure->SetModelInfoUp();
		m_mgrReelmap->OpenReelmapUp(); // At Start...
		m_mgrReelmap->SetPathAtBufUp();
		if (m_pDlgMenu01)
		{
			m_pDlgMenu01->UpdateData();
		}

		MakeDir(PcrShare[0].sModel, PcrShare[0].sLayer);

		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			m_mgrReelmap->OpenReelmapInner();
		}
	}
	else if (nAoi == 1)
	{
		m_mgrProcedure->SetModelInfoDn();
		m_mgrReelmap->OpenReelmapDn(); // At Start...
		m_mgrReelmap->SetPathAtBufDn();
		if (m_pDlgMenu01)
		{
			m_pDlgMenu01->UpdateData();
		}
		MakeDir(PcrShare[1].sModel, PcrShare[1].sLayer);
	}
}

void CGvisRTR_PunchView::InitReelmapDisp()
{
	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->InitGL();
		m_bDrawGL_Menu01 = TRUE;
		m_pDlgMenu01->RefreshRmap();
		m_pDlgMenu01->InitCadImg();
		m_pDlgMenu01->SetPnlNum();
		m_pDlgMenu01->SetPnlDefNum();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pDlgMenu06)
		{
			m_pDlgMenu06->InitGL();
			m_bDrawGL_Menu06 = TRUE;
			m_pDlgMenu06->RefreshRmap();
			m_pDlgMenu06->InitCadImg();
			m_pDlgMenu06->SetPnlNum();
			m_pDlgMenu06->SetPnlDefNum();
		}
	}
}

void CGvisRTR_PunchView::InitCamImgDisp()
{
	if (m_pDlgMenu02)
	{
		m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
		m_pDlgMenu02->InitCadImg();
	}
}

void CGvisRTR_PunchView::UpdateProcessNum(CString sProcessNum)
{
	if (m_mgrReelmap)
		m_mgrReelmap->UpdateProcessNum(sProcessNum);
}

void CGvisRTR_PunchView::ClrMkInfo()
{
	if (!m_mgrReelmap) return;
	if (m_mgrReelmap)
		m_mgrReelmap->ClrMkInfo();

	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->ResetMkInfo();
		m_pDlgMenu01->SetPnlNum();
		m_pDlgMenu01->SetPnlDefNum();
		m_pDlgMenu01->RefreshRmap();
		m_pDlgMenu01->UpdateRst();
	}

	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;
	if (pDoc->GetCurrentInfoEng())
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			CCamMaster* pMasterInner = m_mgrReelmap->m_MasterInner;
			if (!pMasterInner) return;
			if (GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
			{
				if (pMasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, sLayerUp))
				{
					if (m_pDlgMenu06)
					{
						m_pDlgMenu06->ResetMkInfo();
						m_pDlgMenu06->RefreshRmap();
						m_pDlgMenu06->UpdateRst();
					}
				}
			}
		}
	}

	UpdateWorking();
}

void CGvisRTR_PunchView::DoLoadMstInfo()
{
	m_bLoadMstInfo = TRUE;
}

BOOL CGvisRTR_PunchView::LoadMstInfo()
{
	CCamMaster* pMaster = m_mgrReelmap->m_Master;
	CCamMaster* pMasterInner = m_mgrReelmap->m_MasterInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner, bGetCurrentInfoEng;

	bGetCurrentInfoEng = pDoc->GetCurrentInfoEng(); // TRUE: MODE_INNER or MODE_OUTER
	pDoc->GetCamPxlRes();

	if (IsLastJob(0)) // Up : Check Empty (sPathCamSpecDir, sModelUp, sLayerUp)
	{
		if (bGetCurrentInfoEng)
		{
			pMaster[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, pDoc->m_sEngLayerUp);
			pMaster[0].LoadMstInfo();

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
				{
					if (pMasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerUp))
					{
						pMasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerUp);
						pMasterInner[0].LoadMstInfo();
					}
				}
			}
		}
		else
		{
			pMaster[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerUp);
			pMaster[0].LoadMstInfo();
		}
	}

	if (IsLastJob(1)) // Dn : Check Empty (sPathCamSpecDir, sModelUp, sLayerUp)
	{
		if (bGetCurrentInfoEng)
		{
			pMaster[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->m_sEngModel,
				pDoc->m_sEngLayerDn,
				pDoc->m_sEngLayerUp);

			pMaster[1].LoadMstInfo();

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
				{
					if (pMasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerDn))
					{
						pMasterInner[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->m_sEngModel,
							sLayerDn,
							sLayerUp);
						pMasterInner[1].LoadMstInfo();
					}
				}
			}
		}
		else
		{
			pMaster[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->WorkingInfo.LastJob.sModelUp,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				pDoc->WorkingInfo.LastJob.sLayerUp);

			pMaster[1].LoadMstInfo();
		}
	}

	//if (m_pDts)
	//{
	//	if (m_pDts->IsUseDts())
	//	{
	//		pMasterDB.WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->WorkingInfo.LastJob.sLotUp);
	//	}
	//}

	SetAlignPos();

	// Reelmap 정보 Loading.....
	if(m_mgrReelmap)
		m_mgrReelmap->InitReelmap(); // Delete & New

	if (bGetCurrentInfoEng)
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
			{
				if (pMasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerUp))
				{
					// Reelmap 정보 Loading.....
					m_mgrReelmap->InitReelmapInner(); // Delete & New

					if (m_pDlgMenu06)
					{
						m_pDlgMenu06->InitGL();
						m_pDlgMenu06->RefreshRmap();
						m_pDlgMenu06->InitCadImg();
						m_pDlgMenu06->SetPnlNum();
						m_pDlgMenu06->SetPnlDefNum();

						if (bDualTestInner)
							m_pDlgMenu06->SelMap(ALL);
						else
							m_pDlgMenu06->SelMap(UP);
					}
				}
				else
				{
					MsgBox(_T("InitReelmapInner()를 위한 GetItsSerialInfo의 정보가 없습니다.")); // syd-20231127
					return FALSE;
				}
			}
		}
	}

	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->InitGL();
		m_bDrawGL_Menu01 = TRUE;
		m_pDlgMenu01->RefreshRmap();
		m_pDlgMenu01->InitCadImg();
		m_pDlgMenu01->SetPnlNum();
		m_pDlgMenu01->SetPnlDefNum();
	}

	if (m_pDlgMenu02)
	{
		m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
		m_pDlgMenu02->InitCadImg();
	}
#ifndef TEST_MODE
	if (m_pDlgMenu01)
		m_pDlgMenu01->RedrawWindow();

	DispMsg(_T("릴맵을 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
	OpenReelmap();
#endif
	SetPathAtBuf(); // Reelmap path를 설정함.
					//LoadPcrFromBuf();

	int nSrl = pDoc->GetLastShotMk();
	SetMkFdLen();
	if (nSrl >= 0)
	{
		if (bDualTest)
			m_pDlgMenu01->SelMap(ALL);
		else
			m_pDlgMenu01->SelMap(UP);
	}

	return TRUE;
}

void CGvisRTR_PunchView::UpdateWorking()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateWorking();
}

BOOL CGvisRTR_PunchView::GetEngInfo()
{
	if (!m_mgrProcedure || !m_mgrProcedure->m_pEngrave)	return FALSE;	
	return m_mgrProcedure->m_pEngrave->GetEngInfo();
}

void CGvisRTR_PunchView::SetEngInfo(BOOL bOn)
{
	if (!m_mgrProcedure || !m_mgrProcedure->m_pEngrave)	return;
	return m_mgrProcedure->m_pEngrave->SetEngInfo(bOn);
}

BOOL CGvisRTR_PunchView::GetEngOpInfo()
{
	if (!m_mgrProcedure || !m_mgrProcedure->m_pEngrave)	return FALSE;
	return m_mgrProcedure->m_pEngrave->GetEngOpInfo();
}

void CGvisRTR_PunchView::SetEngOpInfo(BOOL bOn)
{
	if (!m_mgrProcedure || !m_mgrProcedure->m_pEngrave)	return;
	return m_mgrProcedure->m_pEngrave->SetEngOpInfo(bOn);
}

BOOL CGvisRTR_PunchView::IsLastJob(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
{
	switch (nAoi)
	{
	case 0: // AOI-Up
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			return FALSE;
		break;
	case 1: // AOI-Dn
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerDn.IsEmpty())
			return FALSE;
		break;
	case 2: // AOI-UpDn
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			return FALSE;
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModelUp.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerDn.IsEmpty())
			return FALSE;
		break;
	}

	return TRUE;
}

BOOL CGvisRTR_PunchView::GetItsSerialInfo(int nItsSerial, BOOL &bDualTest, CString &sLot, CString &sLayerUp, CString &sLayerDn, int nOption)		// 내층에서의 ITS 시리얼의 정보
{
	if (!m_mgrProcedure) return FALSE;
	return m_mgrProcedure->GetItsSerialInfo(nItsSerial, bDualTest, sLot, sLayerUp, sLayerDn, nOption);
}

BOOL CGvisRTR_PunchView::MakeDir(CString sModel, CString sLayer)
{
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;
#ifdef TEST_MODE
	//if(!finder.FindFile(PATH_LOCAL_SPEC))
	if (!pDoc->DirectoryExists(PATH_LOCAL_SPEC))
		CreateDirectory(PATH_LOCAL_SPEC, NULL);

	if (sModel.IsEmpty() || sLayer.IsEmpty())
	{
		sMsg.Format(_T("모델이나 레이어명이 없습니다."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s\\%s"), PATH_LOCAL_SPEC, sModel);
	//if(!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s\\%s\\%s"), PATH_LOCAL_SPEC, sModel, sLayer);
	//if(!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
#else
	sPath.Format(_T("%s"), PATH_LOCAL_SPEC);
	//if(!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (sModel.IsEmpty() || sLayer.IsEmpty())
	{
		sMsg.Format(_T("모델이나 레이어명이 없습니다."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s\\%s"), PATH_LOCAL_SPEC, sModel);
	//if(!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s\\%s\\%s"), PATH_LOCAL_SPEC, sModel, sLayer);
	//if(!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
#endif
	return TRUE;
}

double CGvisRTR_PunchView::GetEnc(int nAxis)
{
	if (!m_mgrPunch) return 0.0;
	return m_mgrPunch->GetEnc(nAxis);
}
void CGvisRTR_PunchView::SetCompletedSerial(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.49"));
		return;
	}
	if (nSerial > 0)
	{
		CString str, sPath = PATH_WORKING_INFO;
		str.Format(_T("%d"), nSerial);
		pDoc->WorkingInfo.LastJob.sCompletedSerialUp = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), str, sPath);
		pDoc->WorkingInfo.LastJob.sCompletedSerialDn = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), str, sPath);

		if (m_mgrReelmap)
			m_mgrReelmap->SetCompletedSerial(nSerial);
	}
}

BOOL CGvisRTR_PunchView::IsShowLive()
{
	if (m_pDlgMenu02)
	{
		if (m_pDlgMenu02->IsWindowVisible())
			return TRUE;
	}
	return FALSE;
}

void CGvisRTR_PunchView::ShowLive(BOOL bShow)
{
	if (bShow)
	{
		if (!IsShowLive())
			SetTimer(TIM_SHOW_MENU02, 30, NULL);
	}
	else
	{
		if (IsShowLive())
			SetTimer(TIM_SHOW_MENU01, 30, NULL);
	}
}

void CGvisRTR_PunchView::SetMkFdLen()
{
	if (m_mgrFeeding)
		m_mgrFeeding->SetMkFdLen();
}

BOOL CGvisRTR_PunchView::ChkLotCutPos()
{
	if (pDoc->WorkingInfo.LastJob.bLotSep && pDoc->m_bDoneChgLot)
	{
		double dFdTotLen = m_mgrFeeding->GetMkFdLen();
		double dLotCutPos = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen)*1000.0;
		if (dFdTotLen >= dLotCutPos)
		{
			pDoc->WorkingInfo.LastJob.bLotSep = FALSE;
			m_mgrReelmap->UpdateData();
			MpeWrite(_T("MB440184"), 0);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)

			::WritePrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), _T("0"), PATH_WORKING_INFO);

			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();

			return TRUE;
		}
	}

	return FALSE;
}
