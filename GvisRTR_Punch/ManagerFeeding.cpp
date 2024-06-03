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

	if (!Create())
	{
		pView->ClrDispMsg(); 
		AfxMessageBox(_T("CManagerFeeding::Create() Failed!!!"));
	}

	Init();
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
			DoIO();

			// Write
			CntMk();
		}

		if (m_bTIM_SCAN_MPE)
			SetTimer(TIM_SCAN_MPE, 100, NULL);
	}

	CWnd::OnTimer(nIDEvent);
}

void CManagerFeeding::Init()
{
#ifdef USE_MPE
	if (!m_pMpe)
		m_pMpe = new CMpDevice(this);
	if (!m_pMpe->Init(1, 1))
	{
		//		DoMyMsgBox(_T("메카트로링크(MC0)의 통신프로그램을 실행 후, 다시 시작하세요.!!!"));
		MsgBox(_T("메카트로링크(MC0)의 통신프로그램을 실행 후, 다시 시작하세요.!!!"));
		PostMessage(WM_CLOSE);
		return FALSE;
	}
#endif
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
	stGeneral* pGeneral = &(pView->m_mgrStatus->General);
	if (pGeneral->nPrevTotMk[0] != pGeneral->nTotMk[0])
	{
		pGeneral->nPrevTotMk[0] = pGeneral->nTotMk[0];
		MpeWrite(_T("ML45096"), (long)pGeneral->nTotMk[0]);	// 마킹부 (좌) 총 마킹수 
	}
	if (pGeneral->nPrevMkPcs[0] != pGeneral->nMkPcs[0])//m_nCurMk[0])
	{
		pGeneral->nPrevMkPcs[0] = pGeneral->nMkPcs[0];//m_nCurMk[0];
		MpeWrite(_T("ML45098"), (long)pGeneral->nMkPcs[0]);	// 마킹부 (좌) 현재 마킹한 수
	}

	if (pGeneral->nPrevTotMk[1] != pGeneral->nTotMk[1])
	{
		pGeneral->nPrevTotMk[1] = pGeneral->nTotMk[1];
		MpeWrite(_T("ML45100"), (long)pGeneral->nTotMk[1]);	// 마킹부 (우) 총 마킹수 
	}
	if (pGeneral->nPrevMkPcs[1] != pGeneral->nMkPcs[1])//m_nCurMk[1])
	{
		pGeneral->nPrevMkPcs[1] = pGeneral->nMkPcs[1];//m_nCurMk[1];
		MpeWrite(_T("ML45102"), (long)pGeneral->nMkPcs[1]);	// 마킹부 (우) 현재 마킹한 수
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
	return m_pMpe->Write(strRegAddr, lData, bCheck);
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
		TowerLamp(RGB_RED, TRUE);
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
			TowerLamp(RGB_RED, TRUE);
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

void CManagerFeeding::Buzzer(BOOL bOn, int nCh)
{
	if (!m_pMpe)
		return;

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

void CManagerFeeding::Stop()
{
	if (!pView)	return;
	pView->DispMain(_T("정 지"), RGB_RED);
	MpeWrite(_T("MB440162"), 1);
}