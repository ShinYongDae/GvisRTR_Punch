
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

	for (int i = 0; i < 10; i++)
		m_sDispStatusBar[i] = _T("");

	m_sShare[0] = _T("");
	m_sBuf[0] = _T("");
	m_sShare[1] = _T("");
	m_sBuf[1] = _T("");

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
			m_mgrPunch->InitAct();
			m_mgrReelmap->InitAct();
			m_mgrFeeding->InitAct();
			//m_mgrProcedure->m_bStopFeeding = TRUE;
			//MpeWrite(_T("MB440115"), 1); // 마킹부Feeding금지
			//Sleep(300);
			break;
		case 13:
			m_nStepInitView++;
			ClrDispMsg();
			ChkBuf(); // 1, 3
			SetListBuf();
			m_bTIM_DISP_STATUS = TRUE;
			SetTimer(TIM_DISP_STATUS, 100, NULL);
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
	CString str, str2;
	int nSerial, nAoiUpAutoSerial;

	if (ChkShareUp(nSerial))
	{
		str.Format(_T("US: %d"), nSerial);
		m_mgrReelmap->PcrShare[0].bExist = TRUE;
		m_mgrReelmap->PcrShare[0].nSerial = nSerial;
		str2.Format(_T("PCR파일 Received - US: %d"), nSerial);
		pDoc->LogAuto(str2);
		MpeWrite(_T("ML45112"), (long)nSerial);		// 검사한 Panel의 AOI 상 Serial
		MpeWrite(_T("MB44012B"), 1);				// AOI 상 : PCR파일 Received
	}
	else
	{
		m_mgrReelmap->PcrShare[0].bExist = FALSE;
		m_mgrReelmap->PcrShare[0].nSerial = -1;
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
	CString str, str2;
	int nSerial, nAoiDnAutoSerial;

	if (ChkShareDn(nSerial))
	{
		str.Format(_T("DS: %d"), nSerial);
		m_mgrReelmap->PcrShare[1].bExist = TRUE;
		m_mgrReelmap->PcrShare[1].nSerial = nSerial;

		str2.Format(_T("PCR파일 Received - DS: %d"), nSerial);
		pDoc->LogAuto(str2);

		MpeWrite(_T("ML45114"), (long)nSerial);		// 검사한 Panel의 AOI 하 Serial
		MpeWrite(_T("MB44012C"), 1);				// AOI 하 : PCR파일 Received
	}
	else
	{
		m_mgrReelmap->PcrShare[1].bExist = FALSE;
		m_mgrReelmap->PcrShare[1].nSerial = -1;
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

void CGvisRTR_PunchView::DispMain(CString sMsg, COLORREF rgb)
{
	pDoc->SetMonDispMain(sMsg);

	m_stDispMain.sMsg = sMsg;
	m_stDispMain.rgb = rgb;

	if (sMsg == _T("정 지"))
	{
		pDoc->SetMkMenu03(_T("Main"), _T("Stop"), TRUE);
		pDoc->SetMkMenu03(_T("Main"), _T("Run"), FALSE);
	}
	else
	{
		pDoc->SetMkMenu03(_T("Main"), _T("Run"), TRUE);
		pDoc->SetMkMenu03(_T("Main"), _T("Stop"), FALSE);
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
