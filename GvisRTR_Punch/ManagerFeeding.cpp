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
	m_bTIM_SCAN_MPE = FALSE;
	m_sAoiUpAlarmReStartMsg = _T(""); m_sAoiDnAlarmReStartMsg = _T("");
	m_sAoiUpAlarmReTestMsg = _T(""); m_sAoiDnAlarmReTestMsg = _T("");

	Reset();

	if (!Create())
	{
		pView->ClrDispMsg(); 
		AfxMessageBox(_T("CManagerFeeding::Create() Failed!!!"));
	}
}

CManagerFeeding::~CManagerFeeding()
{
	m_bTIM_SCAN_MPE = FALSE;
	//Sleep(10);

	Free();
}

BEGIN_MESSAGE_MAP(CManagerFeeding, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerFeeding::Create()
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


void CManagerFeeding::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIM_SCAN_MPE)
	{
		KillTimer(TIM_SCAN_MPE);

		if (m_pMpe)
		{
			// Read
			GetMpeIO();
			GetMpeSignal();

			ChkEmg();
			ChkSaftySen();
			ChkDoor();
			ChkRcvSig();
			ChkReadyDone();

			DoIO();

			// Write
			CntMk();
		}

		if (m_bTIM_SCAN_MPE)
			SetTimer(TIM_SCAN_MPE, 100, NULL);
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CManagerFeeding::Init()
{
#ifdef USE_MPE
	if (!m_pMpe)
		m_pMpe = new CMpDevice(this);
	if (!m_pMpe->Init(1, 1))
	{
		pView->MsgBox(_T("메카트로링크(MC0)의 통신프로그램을 실행 후, 다시 시작하세요.!!!"));
		return FALSE;
	}
#endif

	return TRUE;
}

void CManagerFeeding::Reset()
{
	m_bCycleStopF = FALSE;
	m_nMonAlmF = 0;
	m_nClrAlmF = 0;
	m_bEngStSw = FALSE;
	m_bEng2dStSw = FALSE;

	m_sAoiUpAlarmReStartMsg = pDoc->GetAoiUpAlarmRestartMsg();
	m_sAoiDnAlarmReStartMsg = pDoc->GetAoiDnAlarmRestartMsg();
	m_sAoiUpAlarmReTestMsg = pDoc->GetAoiUpAlarmReTestMsg();
	m_sAoiDnAlarmReTestMsg = pDoc->GetAoiDnAlarmReTestMsg();
}

void CManagerFeeding::Free()
{
	if (m_pMpe)
	{
		delete m_pMpe;
		m_pMpe = NULL;
	}
}

BOOL CManagerFeeding::InitAct()
{
	if (!Init())
	{
		return FALSE;
	}

	m_bTIM_SCAN_MPE = TRUE;
	SetTimer(TIM_SCAN_MPE, 100, NULL);

	double dPos = _tstof(pDoc->WorkingInfo.Motion.sStBufPos);
	SetBufInitPos(dPos);
	double dVel = _tstof(pDoc->WorkingInfo.Motion.sBufHomeSpd);
	double dAcc = _tstof(pDoc->WorkingInfo.Motion.sBufHomeAcc);
	SetBufHomeParam(dVel, dAcc);

	return TRUE;
}

void CManagerFeeding::CntMk()
{
	if (!pView->m_mgrStatus)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.nPrevTotMk[0] != General.nTotMk[0])
	{
		General.nPrevTotMk[0] = General.nTotMk[0];
		MpeWrite(_T("ML45096"), (long)General.nTotMk[0]);	// 마킹부 (좌) 총 마킹수 
	}
	if (General.nPrevMkPcs[0] != General.nMkPcs[0])//m_nCurMk[0])
	{
		General.nPrevMkPcs[0] = General.nMkPcs[0];//m_nCurMk[0];
		MpeWrite(_T("ML45098"), (long)General.nMkPcs[0]);	// 마킹부 (좌) 현재 마킹한 수
	}

	if (General.nPrevTotMk[1] != General.nTotMk[1])
	{
		General.nPrevTotMk[1] = General.nTotMk[1];
		MpeWrite(_T("ML45100"), (long)General.nTotMk[1]);	// 마킹부 (우) 총 마킹수 
	}
	if (General.nPrevMkPcs[1] != General.nMkPcs[1])//m_nCurMk[1])
	{
		General.nPrevMkPcs[1] = General.nMkPcs[1];//m_nCurMk[1];
		MpeWrite(_T("ML45102"), (long)General.nMkPcs[1]);	// 마킹부 (우) 현재 마킹한 수
	}
}

long CManagerFeeding::GetMpeData(int nSection, int nName)
{
	if (!m_pMpe || !m_pMpe->m_pMpeData)
		return 0;
	return m_pMpe->m_pMpeData[nSection][nName];
}

BOOL CManagerFeeding::GetMpeSignal(int nSection, int nName)
{
	if (!m_pMpe || !m_pMpe->m_pMpeData)
		return 0;
	return (m_pMpe->m_pMpeSignal[nSection] & (0x01 << nName));
}

BOOL CManagerFeeding::MpeWrite(CString strRegAddr, long lData, BOOL bCheck)
{
	if (!m_pMpe)
		return FALSE;
	return MpeWrite(strRegAddr, lData, bCheck);
}

BOOL CManagerFeeding::IsAuto()
{
	return Status.bAuto;
}

void CManagerFeeding::SetBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	pDoc->WorkingInfo.Motion.sStBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), sData, sPath);
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(_T("ML45016"), lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
}

void CManagerFeeding::SetBufHomeParam(double dVel, double dAcc)
{
	long lVel = long(dVel*1000.0);
	long lAcc = long(dAcc*1000.0);
	//	MpeWrite(_T("ML00000"), lVel); // 마킹부 버퍼 홈 속도
	//	MpeWrite(_T("ML00000"), lAcc); // 마킹부 버퍼 홈 가속도
	//	pDoc->SetBufInitPos(dVel, dAcc);
}

void CManagerFeeding::GetMpeIO()
{
	if (!m_pMpe)	return;	
	m_pMpe->GetMpeIO();
}

void CManagerFeeding::GetMpeSignal()
{
	if (!m_pMpe)	return;	
	m_pMpe->GetMpeSignal();
}

void CManagerFeeding::ChkEmg()
{
	if (Status.bEmgAoi[EMG_F_AOI_UP] && !Status.bEmgAoiF[EMG_F_AOI_UP])
	{
		Status.bEmgAoiF[EMG_F_AOI_UP] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->MsgBox(_T("비상정지 - 검사부 상 전면 스위치"));
	}
	else if (!Status.bEmgAoi[EMG_F_AOI_UP] && Status.bEmgAoiF[EMG_F_AOI_UP])
	{
		Status.bEmgAoiF[EMG_F_AOI_UP] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgAoi[EMG_B_AOI_UP] && !Status.bEmgAoiF[EMG_B_AOI_UP])
	{
		Status.bEmgAoiF[EMG_B_AOI_UP] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 검사부 상 후면 스위치"));
	}
	else if (!Status.bEmgAoi[EMG_B_AOI_UP] && Status.bEmgAoiF[EMG_B_AOI_UP])
	{
		Status.bEmgAoiF[EMG_B_AOI_UP] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgAoi[EMG_F_AOI_DN] && !Status.bEmgAoiF[EMG_F_AOI_DN])
	{
		Status.bEmgAoiF[EMG_F_AOI_DN] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 검사부 하 전면 스위치"));
	}
	else if (!Status.bEmgAoi[EMG_F_AOI_DN] && Status.bEmgAoiF[EMG_F_AOI_DN])
	{
		Status.bEmgAoiF[EMG_F_AOI_DN] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgAoi[EMG_B_AOI_DN] && !Status.bEmgAoiF[EMG_B_AOI_DN])
	{
		Status.bEmgAoiF[EMG_B_AOI_DN] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 검사부 하 후면 스위치"));
	}
	else if (!Status.bEmgAoi[EMG_B_AOI_DN] && Status.bEmgAoiF[EMG_B_AOI_DN])
	{
		Status.bEmgAoiF[EMG_B_AOI_DN] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgMk[EMG_M_MK] && !Status.bEmgMkF[EMG_M_MK])
	{
		//SwAoiEmg(TRUE);
		Status.bEmgMkF[EMG_M_MK] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 마킹부 메인 스위치"));
	}
	else if (!Status.bEmgMk[EMG_M_MK] && Status.bEmgMkF[EMG_M_MK])
	{
		Status.bEmgMkF[EMG_M_MK] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgMk[EMG_B_MK] && !Status.bEmgMkF[EMG_B_MK])
	{
		Status.bEmgMkF[EMG_B_MK] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 마킹부 스위치"));
	}
	else if (!Status.bEmgMk[EMG_B_MK] && Status.bEmgMkF[EMG_B_MK])
	{
		Status.bEmgMkF[EMG_B_MK] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgUc && !Status.bEmgUcF)
	{
		Status.bEmgUcF = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 언코일러부 스위치"));
	}
	else if (!Status.bEmgUc && Status.bEmgUcF)
	{
		Status.bEmgUcF = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgRc && !Status.bEmgRcF)
	{
		Status.bEmgRcF = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 리코일러부 스위치"));
	}
	else if (!Status.bEmgRc && Status.bEmgRcF)
	{
		Status.bEmgRcF = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgEngv[0] && !Status.bEmgEngvF[0])
	{
		Status.bEmgEngvF[0] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 각인부 모니터"));
	}
	else if (!Status.bEmgEngv[0] && Status.bEmgEngvF[0])
	{
		Status.bEmgEngvF[0] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}

	if (Status.bEmgEngv[1] && !Status.bEmgEngvF[1])
	{
		Status.bEmgEngvF[1] = TRUE;
		Buzzer(TRUE, 0);
		Stop();
		pView->DispMain(_T("정 지"), RGB_RED);
		pView->MsgBox(_T("비상정지 - 각인부 스위치"));
	}
	else if (!Status.bEmgEngv[1] && Status.bEmgEngvF[1])
	{
		Status.bEmgEngvF[1] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		pView->ResetMotion();
	}
}

BOOL CManagerFeeding::ChkSaftySen() // 감지 : TRUE , 비감지 : FALSE
{
	if (pDoc->WorkingInfo.LastJob.bMkSftySen)
	{
		if (Status.bSensSaftyMk && !Status.bSensSaftyMkF)
		{
			Status.bSensSaftyMkF = TRUE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 마킹부 안전센서가 감지되었습니다."));
		}
		else if (!Status.bSensSaftyMk && Status.bSensSaftyMkF)
		{
			Status.bSensSaftyMkF = FALSE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
	}

	return (Status.bSensSaftyMk);
}

unsigned long CManagerFeeding::ChkDoor() // 0: All Closed , Open Door Index : Doesn't all closed. (Bit3: F, Bit2: L, Bit1: R, Bit0; B)
{
	unsigned long ulOpenDoor = 0;

	if (pDoc->WorkingInfo.LastJob.bAoiUpDrSen)
	{
		if (Status.bDoorAoi[DOOR_FM_AOI_UP] && !Status.bDoorAoiF[DOOR_FM_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 0);
			Status.bDoorAoiF[DOOR_FM_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_FM_AOI_UP] && Status.bDoorAoiF[DOOR_FM_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 0);
			Status.bDoorAoiF[DOOR_FM_AOI_UP] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			//pView->DispStsBar(_T("정지-5"), 0);
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 상 전면 중앙 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_FL_AOI_UP] && !Status.bDoorAoiF[DOOR_FL_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 1);
			Status.bDoorAoiF[DOOR_FL_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_FL_AOI_UP] && Status.bDoorAoiF[DOOR_FL_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 1);
			Status.bDoorAoiF[DOOR_FL_AOI_UP] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 상 전면 좌측 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_FR_AOI_UP] && !Status.bDoorAoiF[DOOR_FR_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 2);
			Status.bDoorAoiF[DOOR_FR_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_FR_AOI_UP] && Status.bDoorAoiF[DOOR_FR_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 2);
			Status.bDoorAoiF[DOOR_FR_AOI_UP] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 상 전면 우측 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_BM_AOI_UP] && !Status.bDoorAoiF[DOOR_BM_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 3);
			Status.bDoorAoiF[DOOR_BM_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_BM_AOI_UP] && Status.bDoorAoiF[DOOR_BM_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 3);
			Status.bDoorAoiF[DOOR_BM_AOI_UP] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 상 후면 중앙 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_BL_AOI_UP] && !Status.bDoorAoiF[DOOR_BL_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 4);
			Status.bDoorAoiF[DOOR_BL_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_BL_AOI_UP] && Status.bDoorAoiF[DOOR_BL_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 4);
			Status.bDoorAoiF[DOOR_BL_AOI_UP] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 상 후면 좌측 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_BR_AOI_UP] && !Status.bDoorAoiF[DOOR_BR_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 5);
			Status.bDoorAoiF[DOOR_BR_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_BR_AOI_UP] && Status.bDoorAoiF[DOOR_BR_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 5);
			Status.bDoorAoiF[DOOR_BR_AOI_UP] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 상 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen)
	{
		if (Status.bDoorAoi[DOOR_FM_AOI_DN] && !Status.bDoorAoiF[DOOR_FM_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 0);
			Status.bDoorAoiF[DOOR_FM_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_FM_AOI_DN] && Status.bDoorAoiF[DOOR_FM_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 0);
			Status.bDoorAoiF[DOOR_FM_AOI_DN] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 하 전면 중앙 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_FL_AOI_DN] && !Status.bDoorAoiF[DOOR_FL_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 1);
			Status.bDoorAoiF[DOOR_FL_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_FL_AOI_DN] && Status.bDoorAoiF[DOOR_FL_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 1);
			Status.bDoorAoiF[DOOR_FL_AOI_DN] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 하 전면 좌측 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_FR_AOI_DN] && !Status.bDoorAoiF[DOOR_FR_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 2);
			Status.bDoorAoiF[DOOR_FR_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_FR_AOI_DN] && Status.bDoorAoiF[DOOR_FR_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 2);
			Status.bDoorAoiF[DOOR_FR_AOI_DN] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 하 전면 우측 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_BM_AOI_DN] && !Status.bDoorAoiF[DOOR_BM_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 3);
			Status.bDoorAoiF[DOOR_BM_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_BM_AOI_DN] && Status.bDoorAoiF[DOOR_BM_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 3);
			Status.bDoorAoiF[DOOR_BM_AOI_DN] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 하 후면 중앙 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_BL_AOI_DN] && !Status.bDoorAoiF[DOOR_BL_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 4);
			Status.bDoorAoiF[DOOR_BL_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_BL_AOI_DN] && Status.bDoorAoiF[DOOR_BL_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 4);
			Status.bDoorAoiF[DOOR_BL_AOI_DN] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 하 후면 좌측 도어 Open"));
		}

		if (Status.bDoorAoi[DOOR_BR_AOI_DN] && !Status.bDoorAoiF[DOOR_BR_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 5);
			Status.bDoorAoiF[DOOR_BR_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorAoi[DOOR_BR_AOI_DN] && Status.bDoorAoiF[DOOR_BR_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 5);
			Status.bDoorAoiF[DOOR_BR_AOI_DN] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 검사부 하 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bMkDrSen)
	{
		if (Status.bDoorMk[DOOR_FL_MK] && !Status.bDoorMkF[DOOR_FL_MK])
		{
			ulOpenDoor |= (0x01 << 6);
			Status.bDoorMkF[DOOR_FL_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorMk[DOOR_FL_MK] && Status.bDoorMkF[DOOR_FL_MK])
		{
			ulOpenDoor &= ~(0x01 << 6);
			Status.bDoorMkF[DOOR_FL_MK] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 마킹부 전면 좌측 도어 Open"));
		}

		if (Status.bDoorMk[DOOR_FR_MK] && !Status.bDoorMkF[DOOR_FR_MK])
		{
			ulOpenDoor |= (0x01 << 7);
			Status.bDoorMkF[DOOR_FR_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorMk[DOOR_FR_MK] && Status.bDoorMkF[DOOR_FR_MK])
		{
			ulOpenDoor &= ~(0x01 << 7);
			Status.bDoorMkF[DOOR_FR_MK] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 마킹부 전면 우측 도어 Open"));
		}

		if (Status.bDoorMk[DOOR_BL_MK] && !Status.bDoorMkF[DOOR_BL_MK])
		{
			ulOpenDoor |= (0x01 << 8);
			Status.bDoorMkF[DOOR_BL_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorMk[DOOR_BL_MK] && Status.bDoorMkF[DOOR_BL_MK])
		{
			ulOpenDoor &= ~(0x01 << 8);
			Status.bDoorMkF[DOOR_BL_MK] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 마킹부 후면 좌측 도어 Open"));
		}

		if (Status.bDoorMk[DOOR_BR_MK] && !Status.bDoorMkF[DOOR_BR_MK])
		{
			ulOpenDoor |= (0x01 << 9);
			Status.bDoorMkF[DOOR_BR_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorMk[DOOR_BR_MK] && Status.bDoorMkF[DOOR_BR_MK])
		{
			ulOpenDoor &= ~(0x01 << 9);
			Status.bDoorMkF[DOOR_BR_MK] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 마킹부 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bEngvDrSen)
	{
		if (Status.bDoorEngv[DOOR_FL_ENGV] && !Status.bDoorEngvF[DOOR_FL_ENGV])
		{
			ulOpenDoor |= (0x01 << 6);
			Status.bDoorEngvF[DOOR_FL_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorEngv[DOOR_FL_ENGV] && Status.bDoorEngvF[DOOR_FL_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 6);
			Status.bDoorEngvF[DOOR_FL_ENGV] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 각인부 전면 좌측 도어 Open"));
		}

		if (Status.bDoorEngv[DOOR_FR_ENGV] && !Status.bDoorEngvF[DOOR_FR_ENGV])
		{
			ulOpenDoor |= (0x01 << 7);
			Status.bDoorEngvF[DOOR_FR_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorEngv[DOOR_FR_ENGV] && Status.bDoorEngvF[DOOR_FR_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 7);
			Status.bDoorEngvF[DOOR_FR_ENGV] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 각인부 전면 우측 도어 Open"));
		}

		if (Status.bDoorEngv[DOOR_BL_ENGV] && !Status.bDoorEngvF[DOOR_BL_ENGV])
		{
			ulOpenDoor |= (0x01 << 8);
			Status.bDoorEngvF[DOOR_BL_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorEngv[DOOR_BL_ENGV] && Status.bDoorEngvF[DOOR_BL_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 8);
			Status.bDoorEngvF[DOOR_BL_ENGV] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 각인부 후면 좌측 도어 Open"));
		}

		if (Status.bDoorEngv[DOOR_BR_ENGV] && !Status.bDoorEngvF[DOOR_BR_ENGV])
		{
			ulOpenDoor |= (0x01 << 9);
			Status.bDoorEngvF[DOOR_BR_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorEngv[DOOR_BR_ENGV] && Status.bDoorEngvF[DOOR_BR_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 9);
			Status.bDoorEngvF[DOOR_BR_ENGV] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 각인부 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bUclDrSen)
	{
		if (Status.bDoorUc[DOOR_FL_UC] && !Status.bDoorUcF[DOOR_FL_UC])
		{
			ulOpenDoor |= (0x01 << 10);
			Status.bDoorUcF[DOOR_FL_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorUc[DOOR_FL_UC] && Status.bDoorUcF[DOOR_FL_UC])
		{
			ulOpenDoor &= ~(0x01 << 10);
			Status.bDoorUcF[DOOR_FL_UC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 언코일러부 전면 좌측 도어 Open"));
		}

		if (Status.bDoorUc[DOOR_FR_UC] && !Status.bDoorUcF[DOOR_FR_UC])
		{
			ulOpenDoor |= (0x01 << 11);
			Status.bDoorUcF[DOOR_FR_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorUc[DOOR_FR_UC] && Status.bDoorUcF[DOOR_FR_UC])
		{
			ulOpenDoor &= ~(0x01 << 11);
			Status.bDoorUcF[DOOR_FR_UC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 언코일러부 측면 도어 Open"));
		}

		if (Status.bDoorUc[DOOR_BL_UC] && !Status.bDoorUcF[DOOR_BL_UC])
		{
			ulOpenDoor |= (0x01 << 12);
			Status.bDoorUcF[DOOR_BL_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorUc[DOOR_BL_UC] && Status.bDoorUcF[DOOR_BL_UC])
		{
			ulOpenDoor &= ~(0x01 << 12);
			Status.bDoorUcF[DOOR_BL_UC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 언코일러부 후면 좌측 도어 Open"));
		}

		if (Status.bDoorUc[DOOR_BR_UC] && !Status.bDoorUcF[DOOR_BR_UC])
		{
			ulOpenDoor |= (0x01 << 13);
			Status.bDoorUcF[DOOR_BR_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorUc[DOOR_BR_UC] && Status.bDoorUcF[DOOR_BR_UC])
		{
			ulOpenDoor &= ~(0x01 << 13);
			Status.bDoorUcF[DOOR_BR_UC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 언코일러부 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bRclDrSen)
	{
		if (Status.bDoorRe[DOOR_FR_RC] && !Status.bDoorReF[DOOR_FR_RC])
		{
			ulOpenDoor |= (0x01 << 15);
			Status.bDoorReF[DOOR_FR_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorRe[DOOR_FR_RC] && Status.bDoorReF[DOOR_FR_RC])
		{
			ulOpenDoor &= ~(0x01 << 15);
			Status.bDoorReF[DOOR_FR_RC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 리코일러부 전면 우측 도어 Open"));
		}

		if (Status.bDoorRe[DOOR_S_RC] && !Status.bDoorReF[DOOR_S_RC])
		{
			ulOpenDoor |= (0x01 << 16);
			Status.bDoorReF[DOOR_S_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorRe[DOOR_S_RC] && Status.bDoorReF[DOOR_S_RC])
		{
			ulOpenDoor &= ~(0x01 << 16);
			Status.bDoorReF[DOOR_S_RC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 리코일러부 측면 도어 Open"));
		}

		if (Status.bDoorRe[DOOR_BL_RC] && !Status.bDoorReF[DOOR_BL_RC])
		{
			ulOpenDoor |= (0x01 << 17);
			Status.bDoorReF[DOOR_BL_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorRe[DOOR_BL_RC] && Status.bDoorReF[DOOR_BL_RC])
		{
			ulOpenDoor &= ~(0x01 << 17);
			Status.bDoorReF[DOOR_BL_RC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 리코일러부 후면 좌측 도어 Open"));
		}

		if (Status.bDoorRe[DOOR_BR_RC] && !Status.bDoorReF[DOOR_BR_RC])
		{
			ulOpenDoor |= (0x01 << 18);
			Status.bDoorReF[DOOR_BR_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!Status.bDoorRe[DOOR_BR_RC] && Status.bDoorReF[DOOR_BR_RC])
		{
			ulOpenDoor &= ~(0x01 << 18);
			Status.bDoorReF[DOOR_BR_RC] = FALSE;
			Buzzer(TRUE, 0);
			Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(_T("일시정지 - 리코일러부 후면 우측 도어 Open"));
		}
	}

	return ulOpenDoor;
}

void CManagerFeeding::ChkRcvSig()
{
	if (!pView || !pView->m_mgrProcedure || !pView->m_mgrProcedure->m_pEngrave)
		return;

	stBtnStatus &BtnStatus = pView->m_mgrProcedure->m_pEngrave->BtnStatus;

	long lData = 0;
	int i = 0;

	for (i = 0; i < _SigInx::_EndIdx; i++)
	{
		if (pView->m_bRcvSig[i])
		{
			pView->m_bRcvSig[i] = FALSE;
			switch (i)
			{
			case _SigInx::_Ready:
				MpeWrite(_T("MB440162"), (long)0); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
				break;
			case _SigInx::_Run:
				MpeWrite(_T("MB440162"), (long)0); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
				break;
			case _SigInx::_Reset:
				MpeWrite(_T("MB440162"), (long)0); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
				break;
			case _SigInx::_Stop:
				MpeWrite(_T("MB440162"), (long)(BtnStatus.Main.Stop ? 1 : 0)); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
				break;
			case _SigInx::_MkTq:
				MpeWrite(_T("MB440155"), (long)(BtnStatus.Tq.Mk ? 1 : 0)); // 마킹부 텐션 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_AoiTq:
				MpeWrite(_T("MB440156"), (long)(BtnStatus.Tq.Aoi ? 1 : 0)); // 검사부/각인부 텐션 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_EngTq:
				MpeWrite(_T("MB440156"), (long)(BtnStatus.Tq.Eng ? 1 : 0)); // 검사부/각인부 텐션 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_CcwModRe:
				MpeWrite(_T("MB44017D"), (long)(BtnStatus.Induct.Rc ? 1 : 0)); // 마킹(GUI) 리코일러 정방향(Off->CW/On->CCW)
				break;
			case _SigInx::_CcwModUn:
				MpeWrite(_T("MB44017C"), (long)(BtnStatus.Induct.Uc ? 1 : 0)); // 마킹(GUI) 언코일러 정방향(Off->CW/On->CCW)
				break;
			case _SigInx::_Core150Re:
				MpeWrite(_T("MB44017E"), (long)(BtnStatus.Core150.Rc ? 1 : 0)); // Marking GUI Recoiler Core 150[mm](PC On/Off)
				break;
			case _SigInx::_Core150Un:
				MpeWrite(_T("MB44017F"), (long)(BtnStatus.Core150.Uc ? 1 : 0)); // Marking GUI Uncoiler Core 150[mm](PC On/Off)
				break;
			case _SigInx::_MvCwRe:	// 리코일러 제품휠 정회전 스위치 - IDC_CHK_5
				MpeWrite(_T("MB00580C"), 1);
				Sleep(300);
				MpeWrite(_T("MB00580C"), 0);
				break;
			case _SigInx::_MvCcwRe:	// 리코일러 제품휠 역회전 스위치 - IDC_CHK_6
				MpeWrite(_T("MB00580D"), 1);
				Sleep(300);
				MpeWrite(_T("MB00580D"), 0);
				break;
			case _SigInx::_PrdChuckRe:	// 리코일러 제품척 클램프 스위치 - IDC_CHK_41
				MpeWrite(_T("MB00580B"), 1);
				Sleep(300);
				MpeWrite(_T("MB00580B"), 0);
				break;
			case _SigInx::_DancerUpRe:	// 리코일러 댄서롤 상승/하강 스위치 - IDC_CHK_42
				MpeWrite(_T("MB005802"), 1);
				Sleep(300);
				MpeWrite(_T("MB005802"), 0);
				break;
			case _SigInx::_PasteUpLfRe:	// 리코일러 제품 이음매(상/좌) 스위치 - IDC_CHK_43
				MpeWrite(_T("MB005805"), 1);
				Sleep(300);
				MpeWrite(_T("MB005805"), 0);
				break;
			case _SigInx::_PasteUpRtRe:	// 리코일러 제품 이음매(하/우) 스위치 - IDC_CHK_7
				MpeWrite(_T("MB005806"), 1);
				Sleep(300);
				MpeWrite(_T("MB005806"), 0);
				break;
			case _SigInx::_PasteVacRe:	// 리코일러 제품 이음매 진공 스위치 - IDC_CHK_8
				MpeWrite(_T("MB00580F"), 1);
				Sleep(300);
				MpeWrite(_T("MB00580F"), 0);
				break;
			case _SigInx::_PprChuckRe:	// 리코일러 간지척 클램프 스위치 - IDC_CHK_44
				MpeWrite(_T("MB005808"), 1);
				Sleep(300);
				MpeWrite(_T("MB005808"), 0);
				break;
			case _SigInx::_PprCwRe:	// 리코일러 간지휠 정회전 스위치 - IDC_CHK_45
				MpeWrite(_T("MB005809"), 1);
				Sleep(300);
				MpeWrite(_T("MB005809"), 0);
				break;
			case _SigInx::_PprCcwRe:	// 리코일러 간지휠 역회전 스위치 - IDC_CHK_46
				MpeWrite(_T("MB00580A"), 1);
				Sleep(300);
				MpeWrite(_T("MB00580A"), 0);
				break;
			case _SigInx::_DoRe:	// 리코일러 Rewinder 동작 스위치 - IDC_CHK_66
				MpeWrite(_T("MB005803"), 1);
				Sleep(300);
				MpeWrite(_T("MB005803"), 0);
				break;
			case _SigInx::_PrdPprRe:	// 리코일러 Rewinder 제품 & 간지 스위치 - IDC_CHK_67
				MpeWrite(_T("MB005804"), 1);
				Sleep(300);
				MpeWrite(_T("MB005804"), 0);
				break;
			case _SigInx::_Relation:	// 마킹부 연동 온/오프 스위치 - IDC_CHK_9
				MpeWrite(_T("MB005801"), 1);		// 리코일러 연동 온/오프 스위치
				//MpeWrite(_T("MB005511"), 1);		// 마킹부 연동 온/오프 스위치
				//MpeWrite(_T("MB005701"), 1);		// 검사부 하 연동 온/오프 스위치
				//MpeWrite(_T("MB005601"), 1);		// 검사부 상 연동 온/오프 스위치
				//MpeWrite(_T("MB005401"), 1);		// 언코일러 연동 온/오프 스위치
				Sleep(300);
				MpeWrite(_T("MB005801"), 0);		// 리코일러 연동 온/오프 스위치
				//MpeWrite(_T("MB005511"), 0);		// 마킹부 연동 온/오프 스위치
				//MpeWrite(_T("MB005701"), 0);		// 검사부 하 연동 온/오프 스위치
				//MpeWrite(_T("MB005601"), 0);		// 검사부 상 연동 온/오프 스위치
				//MpeWrite(_T("MB005401"), 0);		// 언코일러 연동 온/오프 스위치
				break;
			case _SigInx::_MvCwMk:	// 마킹부 피딩 정회전 스위치 - IDC_CHK_10
				MpeWrite(_T("MB005513"), 1);
				Sleep(300);
				MpeWrite(_T("MB005513"), 0);
				break;
			case _SigInx::_MvCcwMk:	// 마킹부 피딩 역회전 스위치 - IDC_CHK_11
				MpeWrite(_T("MB005514"), 1);
				Sleep(300);
				MpeWrite(_T("MB005514"), 0);
				break;
			case _SigInx::_FdVacMk:	// 마킹부 피딩 진공 스위치 - IDC_CHK_12
				MpeWrite(_T("MB005515"), 1);
				Sleep(300);
				MpeWrite(_T("MB005515"), 0);
				break;
			case _SigInx::_PushUpMk:	// 마킹부 제품푸쉬 스위치 - IDC_CHK_13
				MpeWrite(_T("MB005516"), 1);
				Sleep(300);
				MpeWrite(_T("MB005516"), 0);
				break;
			case _SigInx::_TblBlwMk:	// 마킹부 테이블 브로워 스위치 - IDC_CHK_14
				MpeWrite(_T("MB005512"), 1);
				Sleep(300);
				MpeWrite(_T("MB005512"), 0);
				break;
			case _SigInx::_TblVacMk:	// 마킹부 테이블 진공 스위치 - IDC_CHK_15
				MpeWrite(_T("MB005517"), 1);
				Sleep(300);
				MpeWrite(_T("MB005517"), 0);
				break;
			case _SigInx::_FdClampMk:	// 마킹부 피딩 클램프 스위치 - IDC_CHK_51
				MpeWrite(_T("MB005519"), 1);
				Sleep(300);
				MpeWrite(_T("MB005519"), 0);
				break;
			case _SigInx::_TensClampMk:	// 마킹부 텐션 클램프 스위치 - IDC_CHK_52
				MpeWrite(_T("MB00551A"), 1);
				Sleep(300);
				MpeWrite(_T("MB00551A"), 0);
				break;
			case _SigInx::_OnePnlMk:
				MpeWrite(_T("MB440151"), (long)(BtnStatus.Mk.MvOne ? 1 : 0));
				break;
			case _SigInx::_DancerUpMk:	// 마킹부 댄서롤 상승/하강 스위치 - IDC_CHK_48
				break;
			case _SigInx::_MvCwAoiDn:	// 검사부 하 피딩 정회전 스위치 - IDC_CHK_56
				MpeWrite(_T("MB005703"), 1);
				Sleep(300);
				MpeWrite(_T("MB005703"), 0);
				break;
			case _SigInx::_MvCcwAoiDn:	// 검사부 하 피딩 역회전 스위치 - IDC_CHK_57
				MpeWrite(_T("MB005704"), 1);
				Sleep(300);
				MpeWrite(_T("MB005704"), 0);
				break;
			case _SigInx::_FdVacAoiDn:	// 검사부 하 피딩 진공 스위치 - IDC_CHK_58
				MpeWrite(_T("MB005705"), 1);
				Sleep(300);
				MpeWrite(_T("MB005705"), 0);
				break;
			case _SigInx::_PushUpAoiDn:	// 검사부 하 제품푸쉬 스위치 - IDC_CHK_59
				MpeWrite(_T("MB005706"), 1);
				Sleep(300);
				MpeWrite(_T("MB005706"), 0);
				break;
			case _SigInx::_TblBlwAoiDn:	// 검사부 하 테이블 브로워 스위치 - IDC_CHK_60
				MpeWrite(_T("MB005702"), 1);
				Sleep(300);
				MpeWrite(_T("MB005702"), 0);
				break;
			case _SigInx::_TblVacAoiDn:	// 검사부 하 테이블 진공 스위치 - IDC_CHK_61
				MpeWrite(_T("MB005707"), 1);
				Sleep(300);
				MpeWrite(_T("MB005707"), 0);
				break;
			case _SigInx::_FdClampAoiDn:	// 검사부 하 피딩 클램프 스위치 - IDC_CHK_64
				MpeWrite(_T("MB005709"), 1);
				Sleep(300);
				MpeWrite(_T("MB005709"), 0);
				break;
			case _SigInx::_TensClampAoiDn:	// 검사부 하 텐션 클램프 스위치 - IDC_CHK_65
				MpeWrite(_T("MB00570A"), 1);
				Sleep(300);
				MpeWrite(_T("MB00570A"), 0);
				break;
			case _SigInx::_OnePnlAoiDn: // 한판넬 이송상태 ON (PC가 ON, OFF) 
				MpeWrite(_T("MB440151"), (long)(BtnStatus.AoiDn.MvOne ? 1 : 0));
				break;
			case _SigInx::_VelClrSonicAoiDn:	// AOI(하) 초음파 세정기 속도 ON (PC가 ON/OFF시킴) - IDC_CHK_88
				if (!(pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic))
				{
					pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic = TRUE;
					MpeWrite(_T("MB44014F"), 1);
				}
				else
				{
					pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic = FALSE;
					MpeWrite(_T("MB44014F"), 0);
				}
				break;
			case _SigInx::_OnePnlAoiUp: // 한판넬 이송상태 ON (PC가 ON, OFF)
				MpeWrite(_T("MB440151"), (long)(BtnStatus.AoiUp.MvOne ? 1 : 0));
				break;
			case _SigInx::_OnePnlEng: // 한판넬 이송상태 ON (PC가 ON, OFF)
				MpeWrite(_T("MB440151"), (long)(BtnStatus.Eng.MvOne ? 1 : 0));
				break;
			case _SigInx::_VelClrSonicEng:	// 각인부 초음파 세정기 속도 ON (PC가 ON/OFF시킴) - IDC_CHK_87
				if (!(pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic))
				{
					pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic = TRUE;
					MpeWrite(_T("MB44014E"), 1);
				}
				else
				{
					pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic = FALSE;
					MpeWrite(_T("MB44014E"), 0);
				}
				break;
			case _SigInx::_MyMsgYes:
				pView->SetMyMsgYes();
				break;
			case _SigInx::_MyMsgNo:
				pView->SetMyMsgNo();
				break;
			case _SigInx::_MyMsgOk:
				pView->SetMyMsgOk();
				break;
			case _SigInx::_DualTest:
				pView->SetDualTest(pDoc->WorkingInfo.LastJob.bDualTest);
				break;
			case _SigInx::_SampleTest:		// Sample 검사 On
				MpeWrite(_T("MB44017B"), (pDoc->WorkingInfo.LastJob.bSampleTest) ? 1 : 0);
				break;
			case _SigInx::_TestMode: // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
				pDoc->SetTestMode(pDoc->WorkingInfo.LastJob.nTestMode);
				break;
			case _SigInx::_RecoilerCcw:
				MpeWrite(_T("MB44017D"), (long)(BtnStatus.Induct.Rc ? 1 : 0));
				::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), BtnStatus.Induct.Rc ? _T("1") : _T("0"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
				break;
			case _SigInx::_UncoilerCcw:
				MpeWrite(_T("MB44017C"), (long)(BtnStatus.Induct.Uc ? 1 : 0));
				::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), BtnStatus.Induct.Uc ? _T("1") : _T("0"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
				break;
			case _SigInx::_DoorRecoiler:
				MpeWrite(_T("MB440163"), (long)(pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0));	// 리코일러Door센서 사용
				break;
			case _SigInx::_DoorAoiUp:
				MpeWrite(_T("MB440166"), (long)(pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0));	// AOI(상) Door센서 사용
				break;
			case _SigInx::_DoorAoiDn:
				MpeWrite(_T("MB440167"), (long)(pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0));	// AOI(하) Door센서 사용
				break;
			case _SigInx::_DoorMk:
				MpeWrite(_T("MB440164"), (long)(pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0));	// 마킹Door센서 사용
				break;
			case _SigInx::_DoorEngrave:
				MpeWrite(_T("MB44019B"), (long)(pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0));	// 각인부 Door센서 사용
				break;
			case _SigInx::_DoorUncoiler:
				MpeWrite(_T("MB440168"), (long)(pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0));	// 언코일러Door센서 사용
				break;
			case _SigInx::_CleannerAoiUp:
				MpeWrite(_T("MB44010E"), (long)(pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0));
				break;
			case _SigInx::_CleannerAoiDn:
				MpeWrite(_T("MB44010F"), (long)(pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0));
				break;
			case _SigInx::_UltraSonicAoiDn:
				MpeWrite(_T("MB44016F"), (long)(pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic ? 1 : 0));
				break;
			case _SigInx::_UltraSonicEngrave:
				MpeWrite(_T("MB44016E"), (long)(pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic ? 1 : 0));
				break;
			case _SigInx::_Use380mm:
				MpeWrite(_T("MB440177"), (long)(pDoc->WorkingInfo.LastJob.bUse380mm ? 1 : 0));	// EPC실린더(제품소->OFF/제품대->ON)
				break;
			case _SigInx::_TempPause:
				MpeWrite(_T("MB440183"), (long)(pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0));	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)
				::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), pDoc->WorkingInfo.LastJob.bTempPause ? _T("1") : _T("0"), PATH_WORKING_INFO);
				break;
			case _SigInx::_2DOffsetInitPos:
				lData = (long)(pDoc->GetOffsetInitPos() * 1000.0); // WorkingInfo.Motion.sOffsetInitPos
				MpeWrite(_T("ML44046"), lData);	// 각인부, 검사부, 마킹부 offset 이송 값 (단위 mm * 1000)
				break;
			case _SigInx::_2DOffsetInitPosMove:
				MpeWrite(_T("MB44013F"), (long)1); // 각인부, 검사부, 마킹부 offset 이송 ON(PC가 On시키고, PLC가 확인하고 Off시킴)
				break;
			default:
				break;
			}
		}
	}
}

void CManagerFeeding::ChkReadyDone()
{
	if (!pView->m_mgrStatus)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);
	CString strMsg, strTitle;

	if (m_pMpe->m_pMpeSignal[0] & (0x01 << 0) && !General.bReadyDone)	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
	{
		General.bReadyDone = TRUE;
		MpeWrite(_T("MB440100"), 0);	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
	}
	else
	{
		pView->GetDispMsg(strMsg, strTitle);
		if (strMsg != _T("Searching Buffer Home Position...") || strTitle != _T("Homming"))
		{
			General.bReadyDone = FALSE;
		}
	}
}

void CManagerFeeding::DoIO()
{
	if (!pView->m_mgrStatus)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);

	DoEmgSens();
	DoSaftySens();
	DoDoorSens();

	DoModeSel();
	DoMainSw();
	DoEngraveSens();

	MonPlcAlm();
	MonDispMain();
	MonPlcSignal();

	if (General.bCycleStop && !m_bCycleStopF)
	{
		m_bCycleStopF = TRUE;
		Buzzer(TRUE);

		if (!pView->m_sAlmMsg.IsEmpty())
		{
			pDoc->LogAuto(pView->m_sAlmMsg);
			pView->MsgBox(pView->m_sAlmMsg, 0, 0, DEFAULT_TIME_OUT, FALSE);

			if (pView->m_sAlmMsg == m_sAoiUpAlarmReStartMsg || pView->m_sAlmMsg == m_sAoiUpAlarmReTestMsg)
			{ // Wait for AOI 검사시작 신호.
				ChkReTestAlarmOnAoiUp(); // 검사부 상부 재작업 (시작신호) : PC가 On시키고 PLC가 Off
			}
			else if (pView->m_sAlmMsg == m_sAoiDnAlarmReStartMsg || pView->m_sAlmMsg == m_sAoiDnAlarmReTestMsg)
			{ // Wait for AOI 검사시작 신호.
				ChkReTestAlarmOnAoiDn(); // 검사부 하부 재작업 (시작신호) : PC가 On시키고 PLC가 Off
			}
		}
		ClrAlarm();
	}
	else if (!General.bCycleStop && m_bCycleStopF)
	{
		m_bCycleStopF = FALSE;
	}

	if (Status.bAuto)
	{
		DoAutoEng();
		pView->Auto();
	}

	if (IsRun())
		pView->EnableBtn(IDD_DLG_MENU_01, FALSE);
	else
		pView->EnableBtn(IDD_DLG_MENU_01, TRUE);
}

void CManagerFeeding::DoAutoEng()
{
	if (!IsAuto() || (MODE_INNER != pDoc->GetTestMode()))
		return;

	// 각인부 마킹시작 신호를 확인
	DoAtuoGetEngStSignal();

	// 각인부 2D 코드 Reading신호를 확인
	DoAtuoGet2dReadStSignal();
}

void CManagerFeeding::DoEmgSens()
{
#ifdef USE_MPE
	unsigned short usIn, *usInF;

	if (!m_pMpe->m_pMpeIb || !m_pMpe->m_pMpeIbF)
		return;

	usIn = m_pMpe->m_pMpeIb[0];
	usInF = &m_pMpe->m_pMpeIbF[0];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 언코일러 비상정지 스위치
	{
		*usInF |= (0x01 << 0);
		Status.bEmgUc = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		Status.bEmgUc = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[4];
	usInF = &m_pMpe->m_pMpeIbF[4];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 마킹부 비상정지 스위치(모니터부)
	{
		*usInF |= (0x01 << 0);
		Status.bEmgMk[EMG_M_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		Status.bEmgMk[EMG_M_MK] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[5];
	usInF = &m_pMpe->m_pMpeIbF[5];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 마킹부 비상정지 스위치(스위치부)	
	{
		*usInF |= (0x01 << 0);
		Status.bEmgMk[EMG_B_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		Status.bEmgMk[EMG_B_MK] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[8];
	usInF = &m_pMpe->m_pMpeIbF[8];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 검사부 상 비상정지 스위치(후면) 
	{
		*usInF |= (0x01 << 0);
		Status.bEmgAoi[EMG_B_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		Status.bEmgAoi[EMG_B_AOI_UP] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[12];
	usInF = &m_pMpe->m_pMpeIbF[12];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 검사부 하 비상정지 스위치(후면) 
	{
		*usInF |= (0x01 << 0);
		Status.bEmgAoi[EMG_B_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		Status.bEmgAoi[EMG_B_AOI_DN] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[16];
	usInF = &m_pMpe->m_pMpeIbF[16];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 리코일러 비상정지 스위치
	{
		*usInF |= (0x01 << 0);
		Status.bEmgRc = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		Status.bEmgRc = FALSE;
	}
#endif
}

void CManagerFeeding::DoSaftySens()
{
#ifdef USE_MPE
	if (!m_pMpe->m_pMpeIb || !m_pMpe->m_pMpeIbF)
		return;

	if (!IsRun())
		return;

	unsigned short usIn = m_pMpe->m_pMpeIb[7];
	unsigned short *usInF = &m_pMpe->m_pMpeIbF[7];


	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))		// 마킹부 안전 센서
	{
		*usInF |= (0x01 << 8);
		Status.bSensSaftyMk = TRUE;
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))	// 마킹부 안전 센서
	{
		*usInF &= ~(0x01 << 8);
		Status.bSensSaftyMk = FALSE;
	}
#endif
}

void CManagerFeeding::DoDoorSens()
{
#ifdef USE_MPE
	unsigned short usIn;
	unsigned short *usInF;

	if (!m_pMpe->m_pMpeIb || !m_pMpe->m_pMpeIbF)
		return;

	if (!IsRun())
		return;

	usIn = m_pMpe->m_pMpeIb[1];
	usInF = &m_pMpe->m_pMpeIbF[1];

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 언코일러 전면 도어 센서
	{
		*usInF |= (0x01 << 12);
		Status.bDoorUc[DOOR_FL_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 언코일러 전면 도어 센서
	{
		*usInF &= ~(0x01 << 12);
		Status.bDoorUc[DOOR_FL_UC] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 언코일러 측면 도어 센서
	{
		*usInF |= (0x01 << 13);
		Status.bDoorUc[DOOR_FR_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 언코일러 측면 도어 센서
	{
		*usInF &= ~(0x01 << 13);
		Status.bDoorUc[DOOR_FR_UC] = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))		// 언코일러 후면 도어 센서(좌)
	{
		*usInF |= (0x01 << 14);
		Status.bDoorUc[DOOR_BL_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))	// 언코일러 후면 도어 센서(좌)
	{
		*usInF &= ~(0x01 << 14);
		Status.bDoorUc[DOOR_BL_UC] = FALSE;
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))		// 언코일러 후면 도어 센서(우)
	{
		*usInF |= (0x01 << 15);
		Status.bDoorUc[DOOR_BR_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))	// 언코일러 후면 도어 센서(우)
	{
		*usInF &= ~(0x01 << 15);
		Status.bDoorUc[DOOR_BR_UC] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[7];
	usInF = &m_pMpe->m_pMpeIbF[7];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 마킹부 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		Status.bDoorMk[DOOR_FL_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 마킹부 도어 센서 1 
	{
		*usInF &= ~(0x01 << 10);
		Status.bDoorMk[DOOR_FL_MK] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 마킹부 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		Status.bDoorMk[DOOR_FR_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 마킹부 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		Status.bDoorMk[DOOR_FR_MK] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 마킹부 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		Status.bDoorMk[DOOR_BL_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 마킹부 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		Status.bDoorMk[DOOR_BL_MK] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 마킹부 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		Status.bDoorMk[DOOR_BR_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 마킹부 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		Status.bDoorMk[DOOR_BR_MK] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[11];
	usInF = &m_pMpe->m_pMpeIbF[11];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 상 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		Status.bDoorMk[DOOR_FL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 검사부 상 도어 센서 1
	{
		*usInF &= ~(0x01 << 10);
		Status.bDoorMk[DOOR_FL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 검사부 상 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		Status.bDoorMk[DOOR_FR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 상 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		Status.bDoorMk[DOOR_FR_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 검사부 상 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		Status.bDoorMk[DOOR_BL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 상 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		Status.bDoorMk[DOOR_BL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 검사부 상 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		Status.bDoorMk[DOOR_BR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 상 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		Status.bDoorMk[DOOR_BR_AOI_UP] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[15];
	usInF = &m_pMpe->m_pMpeIbF[15];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 상 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		Status.bDoorAoi[DOOR_FL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 검사부 상 도어 센서 1
	{
		*usInF &= ~(0x01 << 11);
		Status.bDoorAoi[DOOR_FL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 검사부 상 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		Status.bDoorAoi[DOOR_FR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 상 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		Status.bDoorAoi[DOOR_FR_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 검사부 상 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		Status.bDoorAoi[DOOR_BL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 상 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		Status.bDoorAoi[DOOR_BL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 검사부 상 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		Status.bDoorAoi[DOOR_BR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 상 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		Status.bDoorAoi[DOOR_BR_AOI_UP] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[15];
	usInF = &m_pMpe->m_pMpeIbF[15];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 하 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		Status.bDoorAoi[DOOR_FL_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 검사부 하 도어 센서 1
	{
		*usInF &= ~(0x01 << 10);
		Status.bDoorAoi[DOOR_FL_AOI_DN] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 검사부 하 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		Status.bDoorAoi[DOOR_FR_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 하 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		Status.bDoorAoi[DOOR_FR_AOI_DN] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 검사부 하 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		Status.bDoorAoi[DOOR_BL_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 하 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		Status.bDoorAoi[DOOR_BL_AOI_DN] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 검사부 하 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		Status.bDoorAoi[DOOR_BR_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 하 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		Status.bDoorAoi[DOOR_BR_AOI_DN] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[17];
	usInF = &m_pMpe->m_pMpeIbF[17];

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 리코일러 전면 도어 센서
	{
		*usInF |= (0x01 << 12);
		Status.bDoorRe[DOOR_FL_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 리코일러 전면 도어 센서
	{
		*usInF &= ~(0x01 << 12);
		Status.bDoorRe[DOOR_FL_RC] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 리코일러 측면 도어 센서
	{
		*usInF |= (0x01 << 13);
		Status.bDoorRe[DOOR_FR_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 리코일러 측면 도어 센서
	{
		*usInF &= ~(0x01 << 13);
		Status.bDoorRe[DOOR_FR_RC] = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))		// 리코일러 후면 도어 센서
	{
		*usInF |= (0x01 << 14);
		Status.bDoorRe[DOOR_BL_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))	// 리코일러 후면 도어 센서(좌)
	{
		*usInF &= ~(0x01 << 14);
		Status.bDoorRe[DOOR_BL_RC] = FALSE;
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))		// 리코일러 후면 도어 센서(우)
	{
		*usInF |= (0x01 << 15);
		Status.bDoorRe[DOOR_BR_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))	// 리코일러 후면 도어 센서(우)
	{
		*usInF &= ~(0x01 << 15);
		Status.bDoorRe[DOOR_BR_RC] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[27];
	usInF = &m_pMpe->m_pMpeIbF[27];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 각인부 도어 센서 1
	{
		*usInF |= (0x01 << 10);
		Status.bDoorEngv[DOOR_FL_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 각인부 도어 센서 1
	{
		*usInF &= ~(0x01 << 10);
		Status.bDoorEngv[DOOR_FL_ENGV] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 각인부 도어 센서 2
	{
		*usInF |= (0x01 << 11);
		Status.bDoorEngv[DOOR_FR_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 각인부 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		Status.bDoorEngv[DOOR_FR_ENGV] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 각인부 도어 센서 3
	{
		*usInF |= (0x01 << 12);
		Status.bDoorEngv[DOOR_BL_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 각인부 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		Status.bDoorEngv[DOOR_BL_ENGV] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 각인부 도어 센서 4
	{
		*usInF |= (0x01 << 13);
		Status.bDoorEngv[DOOR_BR_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 각인부 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		Status.bDoorEngv[DOOR_BR_ENGV] = FALSE;
	}

	usIn = m_pMpe->m_pMpeIb[28];
	usInF = &m_pMpe->m_pMpeIbF[28];


	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 2D 리셋 스위치
		//if (pView->m_pDlgMenu03)
		//	pView->m_pDlgMenu03->SwReset();
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

#endif
}

void CManagerFeeding::DoModeSel()
{
#ifdef USE_MPE
	if (!m_pMpe->m_pMpeIo)
		return;

	BOOL bMode;// [2];
	bMode = m_pMpe->m_pMpeIo[36] & (0x01 << 5) ? TRUE : FALSE;	// 마킹부 자동 상태 스위치 램프
	if (bMode)
	{
		Status.bAuto = TRUE;
		Status.bManual = FALSE;
		Status.bOneCycle = FALSE;
	}
	else
	{
		Status.bManual = TRUE;
		Status.bAuto = FALSE;
		Status.bOneCycle = FALSE;
	}
#else
	Status.bAuto = FALSE;
	Status.bManual = TRUE;
	Status.bOneCycle = FALSE;
#endif
}

void CManagerFeeding::DoMainSw()
{
#ifdef USE_MPE
	if (!m_pMpe->m_pMpeIb || !m_pMpe->m_pMpeIbF)
		return;

	unsigned short usIn = m_pMpe->m_pMpeIb[4];
	unsigned short *usInF = &m_pMpe->m_pMpeIbF[4];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 마킹부 비상정지 스위치(모니터부)
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 마킹부 운전 스위치
		//if (pView->m_pDlgMenu03)
		//	pView->m_pDlgMenu03->SwRun();
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 마킹부 정지 스위치
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 마킹부 운전준비 스위치
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 마킹부 리셋 스위치
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 마킹부 JOG 버튼(상)
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 마킹부 JOG 버튼(하)
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 마킹부 JOG 버튼(좌)
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 마킹부 JOG 버튼(우)
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 마킹부 모션 선택(LEFT)
		Status.bSwJogLeft = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
		Status.bSwJogLeft = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 마킹부 속도 선택
		Status.bSwJogFast = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
		Status.bSwJogFast = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 마킹부 운전 선택(INDEX)
		Status.bSwJogStep = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
		Status.bSwJogStep = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// SPARE	
		// No Use....
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
		// No Use....
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// SPARE	
		// No Use....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
		// No Use....
	}

	if (!m_pMpe->m_pMpeIo)
		return;
#endif
}

void CManagerFeeding::DoEngraveSens()
{
#ifdef USE_MPE
	unsigned short usIn;
	unsigned short *usInF;

	if (!m_pMpe->m_pMpeIb || !m_pMpe->m_pMpeIbF)
		return;

	usIn = m_pMpe->m_pMpeIb[28];
	usInF = &m_pMpe->m_pMpeIbF[28];


	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 2D 리셋 스위치
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}
#endif
}

void CManagerFeeding::MonPlcAlm()
{
	BOOL bMon, bClr;
	long lOn = m_pMpe->Read(_T("ML60000"));

	bMon = lOn & (0x01 << 0);
	bClr = lOn & (0x01 << 1);
	if (bMon)
		PlcAlm(bMon, 0);
	else if (bClr)
		PlcAlm(0, bClr);
	else
		PlcAlm(bMon, bClr);

	if (!pView->m_sAlmMsg.IsEmpty())
	{
		if (pView->m_sAlmMsg != pView->m_sPrevAlmMsg)
		{
			pView->m_sPrevAlmMsg = pView->m_sAlmMsg;
			CycleStop();
		}
	}
}

void CManagerFeeding::MonDispMain()
{
	BOOL bDispStop = TRUE;
	CString sDispMain = pView->GetDispMain();

#ifdef USE_MPE
	if (m_pMpe->m_pMpeSignal[2] & (0x01 << 0))		// 운전중(PLC가 PC에 알려주는 설비 상태) - 20141031
	{
		if (sDispMain != _T("운전중"))
		{
			pView->DispMain(_T("운전중"), RGB_GREEN);
		}
	}

	if (m_pMpe->m_pMpeSignal[2] & (0x01 << 2))		// 운전준비(PLC가 PC에 알려주는 설비 상태) - 20141031
	{
		bDispStop = FALSE;
		if (sDispMain != _T("운전준비"))
		{
			pView->DispMain(_T("운전준비"), RGB_GREEN);
		}
	}
	else
	{
		if (m_pMpe->m_pMpeSignal[2] & (0x01 << 3))		// 초기운전(PLC가 PC에 알려주는 설비 상태) - 20141031
		{
			bDispStop = FALSE;
			{
				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (pDoc->WorkingInfo.LastJob.bDualTest)
					{
						if (sDispMain != _T("양면샘플"))
						{
							pView->DispMain(_T("양면샘플"), RGB_GREEN);
						}
					}
					else
					{
						if (sDispMain != _T("단면샘플"))
						{
							pView->DispMain(_T("단면샘플"), RGB_GREEN);
						}
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (sDispMain != _T("내층검사"))
						pView->DispMain(_T("내층검사"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (sDispMain != _T("외층검사"))
						pView->DispMain(_T("외층검사"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (sDispMain != _T("양면검사"))
					{
						pView->DispMain(_T("양면검사"), RGB_GREEN);
					}
				}
				else
				{
					if (sDispMain != _T("단면검사"))
					{
						pView->DispMain(_T("단면검사"), RGB_GREEN);
					}
				}
			}
		}
		else
		{
			if (sDispMain != _T("운전준비"))
			{
				bDispStop = TRUE;
			}
			else
			{
				bDispStop = FALSE;
			}
		}
	}

	if (m_pMpe->m_pMpeSignal[2] & (0x01 << 1))		// 정지(PLC가 PC에 알려주는 설비 상태) - 20141031
	{
		if (bDispStop)
		{
			if (sDispMain != _T("정 지"))
			{
				pView->DispMain(_T("정 지"), RGB_RED);
			}
		}
	}
#endif
}

void CManagerFeeding::MonPlcSignal()
{
#ifdef USE_MPE
	if (m_pMpe->m_pMpeSignal[3] & (0x01 << 0))		// 각인부 2D Leading 작업완료(PLC가 ON/OFF) - MB440130
	{
			SetLed(0, TRUE);
	}
	else
	{
			SetLed(0, FALSE);
	}

	if (m_pMpe->m_pMpeSignal[3] & (0x01 << 2))		// 각인부 Laser 작업완료(PLC가 ON/OFF) - MB440132
	{
			SetLed(1, TRUE);
	}
	else
	{
			SetLed(1, FALSE);
	}

	if (m_pMpe->m_pMpeSignal[3] & (0x01 << 3))		// 검사부 상면 검사 작업완료(PLC가 ON/OFF) - MB440133
	{
			SetLed(2, TRUE);
	}
	else
	{
			SetLed(2, FALSE);
	}

	if (m_pMpe->m_pMpeSignal[3] & (0x01 << 4))		// 검사부 하면 검사 작업완료(PLC가 ON/OFF) - MB440134
	{
			SetLed(3, TRUE);
	}
	else
	{
			SetLed(3, FALSE);
	}

	if (m_pMpe->m_pMpeSignal[3] & (0x01 << 5))		// 마킹부 마킹 작업완료(PLC가 ON/OFF) - MB440135
	{
			SetLed(4, TRUE);
	}
	else
	{
			SetLed(4, FALSE);
	}

	if (m_pMpe->m_pMpeSignal[2] & (0x01 << 5))		// 내층 제품시 이어가기 상태 표시 - MB440125
	{
		pView->DispContRun(TRUE);
	}
	else
	{
		pView->DispContRun(FALSE);
	}

#endif
}

void CManagerFeeding::PlcAlm(BOOL bMon, BOOL bClr)
{
	if (bMon && !m_nMonAlmF)
	{
		m_nMonAlmF = 1;
		FindAlarm();
		pView->m_sIsAlmMsg = _T("");
		pView->SetAlarm(pView->m_sAlmMsg);
		Sleep(300);
		MpeWrite(_T("MB600008"), 1);
	}
	else if (!bMon && m_nMonAlmF)
	{
		m_nMonAlmF = 0;
		ResetMonAlm();
	}
	else
	{
		if (pView->m_sIsAlmMsg != pView->m_sAlmMsg)
		{
			pView->SetAlarm(pView->m_sAlmMsg);
		}
	}


	if (bClr && !m_nClrAlmF)
	{
		m_nClrAlmF = 1;
		ClrAlarm();
		pView->SetAlarm(pView->m_sAlmMsg);
		Sleep(300);
		MpeWrite(_T("MB600009"), 1); // ResetAlarm

	}
	else if (!bClr && m_nClrAlmF)
	{
		m_nClrAlmF = 0;
		MpeWrite(_T("MB600009"), 0); // ResetAlarm
	}
	else
	{
		if (pView->m_sIsAlmMsg != pView->m_sAlmMsg)
		{
			pView->SetAlarm(pView->m_sAlmMsg);
		}
	}
}

void CManagerFeeding::FindAlarm()
{
	// 알람이 발생한 페이지
	long lAlmPage = -1;
	lAlmPage = m_pMpe->Read(_T("ML60002"));

	TCHAR szData[200];
	CString str1, str2, str3, strM, str, strH = _T("");

	str1.Format(_T("%d"), lAlmPage);
	str2 = _T("Address");
	if (0 < ::GetPrivateProfileString(str1, str2, NULL, szData, sizeof(szData), PATH_ALARM))
		strM = CString(szData);
	else
		strM = _T("");

	if (strM.IsEmpty())
		return;

	long lAlm = m_pMpe->Read(strM); // lAlm : (32Bits << Row)
	for (int i = 0; i < 32; i++)
	{
		if (lAlm & (0x01 << i))
		{
			str3.Format(_T("%d"), i);
			if (0 < ::GetPrivateProfileString(str1, str3, NULL, szData, sizeof(szData), PATH_ALARM))
				strH = CString(szData);
			else
				strH.Format(_T("%s = %d"), strM, lAlm);
			//strH = _T("");

			if (str.IsEmpty())
				str = strH;
			else
			{
				str += _T("\r\n");
				str += strH;
			}
		}
	}

	pView->m_sAlmMsg = strH;
}

void CManagerFeeding::ResetMonAlm()
{
	MpeWrite(_T("MB600008"), 0);
}

void CManagerFeeding::ClrAlarm()
{
	pView->ClrAlarm();
}

void CManagerFeeding::ChkReTestAlarmOnAoiUp()
{
	pView->ChkReTestAlarmOnAoiUp();
}

void CManagerFeeding::ChkReTestAlarmOnAoiDn()
{
	pView->ChkReTestAlarmOnAoiDn();
}

void CManagerFeeding::Buzzer(BOOL bOn, int nCh)
{
	if (!bOn)
	{
		switch (nCh)
		{
		case 0:
			MpeWrite(_T("MB44015E"), 0); // 부저1 OFF  (PC가 ON, OFF)
			break;
		case 1:
			MpeWrite(_T("MB44015F"), 0); // 부저2 OFF  (PC가 ON, OFF) 
			break;
		}
	}
	else
	{
		switch (nCh)
		{
		case 0:
			MpeWrite(_T("MB44015E"), 0); // 부저1 On  (PC가 ON, OFF)
			Sleep(300);
			MpeWrite(_T("MB44015E"), 1);
			break;
		case 1:
			MpeWrite(_T("MB44015E"), 0); // 부저2 On  (PC가 ON, OFF)
			Sleep(300);
			MpeWrite(_T("MB44015F"), 1);
			break;
		}
	}
}

void CManagerFeeding::SetLed(int nIdx, BOOL bOn)
{
	pView->SetLed(nIdx, bOn);
}

void CManagerFeeding::Stop()
{
	pView->DispMain(_T("정 지"), RGB_RED);
	MpeWrite(_T("MB440162"), 1);
}

void CManagerFeeding::CycleStop()
{
	if (!pView->m_mgrStatus)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);

	General.bCycleStop = TRUE;
}

BOOL CManagerFeeding::IsRun()
{
	if (!m_pMpe)
		return FALSE;

	return (m_pMpe->m_pMpeSignal[2] & (0x01 << 0)); // 운전중(PLC가 PC에 알려주는 설비 상태)
}

BOOL CManagerFeeding::IsStop()
{
	if (!m_pMpe)
		return FALSE;

	return (m_pMpe->m_pMpeSignal[2] & (0x01 << 1)); // 정지(PLC가 PC에 알려주는 설비 상태)
}

BOOL CManagerFeeding::IsReady()
{
	if (!m_pMpe)
		return FALSE;

	return (m_pMpe->m_pMpeSignal[2] & (0x01 << 2)); // PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
}

BOOL CManagerFeeding::IsInitRun()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeSignal[2] & (0x01 << 3)); // 초기운전(PLC가 PC에 알려주는 설비 상태)
}

void CManagerFeeding::ResetReady()
{
	MpeWrite(_T("MB440100"), 0);	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
}

void CManagerFeeding::DoAtuoGetEngStSignal()
{
	if (!m_pMpe || !pView->m_mgrProcedure) return;
	stBtnStatus& BtnStatus = (pView->m_mgrProcedure->m_pEngrave->BtnStatus);

	if ((m_pMpe->m_pMpeSignal[0] & (0x01 << 3) || m_bEngStSw) && !BtnStatus.EngAuto.MkStF)// 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
	{
		BtnStatus.EngAuto.MkStF = TRUE;
		m_bEngStSw = FALSE;

		BtnStatus.EngAuto.IsMkSt = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, TRUE);
	}
	else if (BtnStatus.EngAuto.IsMkSt && BtnStatus.EngAuto.MkStF)
	{
		BtnStatus.EngAuto.MkStF = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, FALSE);
		MpeWrite(_T("MB440103"), 0);			// 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
	}

	if (m_pMpe->m_pMpeSignal[0] & (0x01 << 2) && !BtnStatus.EngAuto.FdDoneF)	// 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
	{
		BtnStatus.EngAuto.FdDoneF = TRUE;

		BtnStatus.EngAuto.IsFdDone = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, TRUE);
	}
	else if (BtnStatus.EngAuto.IsFdDone && BtnStatus.EngAuto.FdDoneF)
	{
		BtnStatus.EngAuto.FdDoneF = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, FALSE);
		MpeWrite(_T("MB440102"), 0);		// 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
	}
}

void CManagerFeeding::DoAtuoGet2dReadStSignal()
{
	if (!m_pMpe || !pView->m_mgrProcedure) return;
	stBtnStatus& BtnStatus = (pView->m_mgrProcedure->m_pEngrave->BtnStatus);

	if ((m_pMpe->m_pMpeSignal[0] & (0x01 << 5) || m_bEng2dStSw) && !BtnStatus.EngAuto.Read2dStF)// 각인부 2D 리더 시작신호(PLC On->PC Off)
	{
		BtnStatus.EngAuto.Read2dStF = TRUE;
		m_bEng2dStSw = FALSE;

		BtnStatus.EngAuto.IsRead2dSt = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, TRUE);
	}
	else if (BtnStatus.EngAuto.IsRead2dSt && BtnStatus.EngAuto.Read2dStF)
	{
		BtnStatus.EngAuto.Read2dStF = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, FALSE);
		MpeWrite(_T("MB440105"), 0);			// 각인부 2D 리더 시작신호(PLC On->PC Off)
	}
}

BOOL CManagerFeeding::IsLoaderOnAoiUp()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeIb[10] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 상 자동 운전 <-> X432B I/F
}

BOOL CManagerFeeding::IsLoaderOnAoiDn()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeIb[14] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 하 자동 운전 <-> X442B I/F
}

BOOL CManagerFeeding::IsTestingAoiUp()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeSignal[1] & (0x01 << 3)) ? TRUE : FALSE;		// 검사부(상) 검사중
}

BOOL CManagerFeeding::IsTestingAoiDn()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeSignal[1] & (0x01 << 4)) ? TRUE : FALSE;		// 검사부(하) 검사중
}

BOOL CManagerFeeding::IsDoneWriteFdOffsetAoiUp()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeSignal[1] & (0x01 << 1)) ? TRUE : FALSE;		// 검사부(상) Feeding Offset Write 완료
}

BOOL CManagerFeeding::IsDoneWriteFdOffsetAoiDn()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeSignal[1] & (0x01 << 2)) ? TRUE : FALSE;		// 검사부(하) Feeding Offset Write 완료
}

BOOL CManagerFeeding::IsDoneWriteFdOffsetEng()
{
	if (!m_pMpe) return FALSE;
	return (m_pMpe->m_pMpeSignal[1] & (0x01 << 10)) ? TRUE : FALSE;		// 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
}

CString CManagerFeeding::GetAoiAlarmReStartMsg(int nId)
{
	CString str = _T("");

	switch (nId)
	{
	case 0:
		str = m_sAoiUpAlarmReStartMsg;
		break;
	case 1:
		str = m_sAoiDnAlarmReStartMsg;
		break;
	}

	return str;
}

CString CManagerFeeding::GetAoiAlarmReTestMsg(int nId)
{
	CString str = _T("");

	switch (nId)
	{
	case 0:
		str = m_sAoiUpAlarmReTestMsg;
		break;
	case 1:
		str = m_sAoiDnAlarmReTestMsg;
		break;
	}

	return str;
}

void CManagerFeeding::SetAoiUpAlarmRestartMsg(CString sAoiUpAlarmReStartMsg)		// m_sAoiUpAlarmReStartMsg
{
	m_sAoiUpAlarmReStartMsg = sAoiUpAlarmReStartMsg;
}

void CManagerFeeding::SetAoiDnAlarmRestartMsg(CString sAoiDnAlarmReStartMsg)		// m_sAoiDnAlarmReStartMsg
{
	m_sAoiDnAlarmReStartMsg = sAoiDnAlarmReStartMsg;
}

void CManagerFeeding::SetAoiUpAlarmReTestMsg(CString sAoiUpAlarmReTestMsg)			// m_sAoiUpAlarmReTestMsg
{
	m_sAoiUpAlarmReTestMsg = sAoiUpAlarmReTestMsg;
}

void CManagerFeeding::SetAoiDnAlarmReTestMsg(CString sAoiDnAlarmReTestMsg)			// m_sAoiDnAlarmReTestMsg
{
	m_sAoiDnAlarmReTestMsg = sAoiDnAlarmReTestMsg;
}
