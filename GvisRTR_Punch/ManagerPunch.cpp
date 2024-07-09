#include "stdafx.h"
#include "ManagerPunch.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerPunch::CManagerPunch(CWnd* pParent)
{
	m_pParent = pParent;
	m_bCreated = FALSE;
	m_bTIM_INIT_PUNCH = FALSE;
	m_bTIM_SCAN_STATUS = FALSE;

	m_pMotion = NULL;
	m_pVoiceCoil[0] = NULL;
	m_pVoiceCoil[1] = NULL;
	m_pLight = NULL;
	m_pVision[0] = NULL;			// Camera & MIL
	m_pVision[1] = NULL;			// Camera & MIL
	m_pVisionInner[0] = NULL;		// Camera & MIL
	m_pVisionInner[1] = NULL;		// Camera & MIL						
	m_pSr1000w = NULL;				// client for SR-1000W

	Reset();

	if (!Create())
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("CManagerPunch::Create() Failed!!!"));
	}
}

CManagerPunch::~CManagerPunch()
{
	m_bTIM_INIT_PUNCH = FALSE;
	m_bTIM_SCAN_STATUS = FALSE;
}

BOOL CManagerPunch::Create()
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

BEGIN_MESSAGE_MAP(CManagerPunch, CWnd)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

void CManagerPunch::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bCreated)
	{
		m_bCreated = TRUE;
	}
}

void CManagerPunch::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIM_SCAN_STATUS)
	{
		KillTimer(TIM_SCAN_STATUS);
		DoInterlock();
		if (m_bTIM_SCAN_STATUS)
			SetTimer(TIM_SCAN_STATUS, 100, NULL);
	}

	CWnd::OnTimer(nIDEvent);
}

int CManagerPunch::MsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut, BOOL bEngave)
{
	return pView->MsgBox(sMsg, nThreadIdx, nType, nTimOut, bEngave);
}

BOOL CManagerPunch::CreateDevices()
{
	if (m_pMotion)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

	m_pMotion = new CMotion(this);

	if (!m_pMotion->InitBoard())
	{
		MsgBox(_T("XMP 보드 초기화 실패, 다시 시작하세요.!!!"));
		return FALSE;
	}

	if (m_pVoiceCoil[0])
	{
		delete m_pVoiceCoil[0];
		m_pVoiceCoil[0] = NULL;
	}

	m_pVoiceCoil[0] = new CSmac(this);
	m_pVoiceCoil[0]->Init(VOICE_COIL_FIRST_CAM);

	if (m_pVoiceCoil[1])
	{
		delete m_pVoiceCoil[1];
		m_pVoiceCoil[1] = NULL;
	}

	m_pVoiceCoil[1] = new CSmac(this);
	m_pVoiceCoil[1]->Init(VOICE_COIL_SECOND_CAM);

	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = NULL;
	}
	m_pLight = new CLight(this);
	m_pLight->Init();

	return TRUE;
}

BOOL CManagerPunch::Init()
{
	if (!CreateDevices())
		return FALSE;

	int nAxis;
	if (m_pMotion)
	{
		for (nAxis = 0; nAxis < m_pMotion->m_ParamCtrl.nTotAxis; nAxis++)
		{
			m_pMotion->AmpReset(nAxis);
			Sleep(30);
		}
	}

	return TRUE;
}

void CManagerPunch::Reset()
{
	m_bDoneMk[0] = FALSE;		// [nCam]
	m_bDoneMk[1] = FALSE;		// [nCam]
	m_bCollision[0] = FALSE;
	m_bCollision[1] = FALSE;

	ResetPriority();
}

BOOL CManagerPunch::InitAct()
{
	if (!Init())
		return FALSE;

#ifdef USE_XMP
	if (!m_pMotion)
		return FALSE;
#endif

#ifdef USE_LIGHT
	if (!m_pLight)
		return FALSE;
#endif

#ifdef USE_FLUCK
	if (!m_pFluck)
		return FALSE;
#endif

	int nAxis;

	if (m_pMotion)
	{
		// Motor On
		for (nAxis = 0; nAxis < m_pMotion->m_ParamCtrl.nTotAxis; nAxis++)
		{
			m_pMotion->ServoOnOff(nAxis, TRUE);
			Sleep(100);
		}
	}

	// Light On
	SetLight(_tstoi(pDoc->WorkingInfo.Light.sVal[0]));
	SetLight2(_tstoi(pDoc->WorkingInfo.Light.sVal[1]));

	// Homming
	if (m_pVoiceCoil[0])
		m_pVoiceCoil[0]->SearchHomeSmac();
	if (m_pVoiceCoil[1])
		m_pVoiceCoil[1]->SearchHomeSmac();

	m_bTIM_SCAN_STATUS = TRUE;
	SetTimer(TIM_SCAN_STATUS, 100, NULL);

	return TRUE;
}

//===> Light
int CManagerPunch::GetLight()
{
	int nVal = 0;
	if(m_pLight)
		nVal = m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
	return nVal;
}

void CManagerPunch::SetLight(int nVal)
{
	if (m_pLight)
	{
		m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[0]), nVal);
		nVal = m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));

		int nPos;
		nPos = int(100.0*(1.0 - (nVal - 0.5) / 255.0));
		CString str;
		str.Format(_T("%d"), nVal);

		pDoc->WorkingInfo.Light.sVal[0] = str;
		::WritePrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);
	}
}

int CManagerPunch::GetLight2()
{
	int nVal = 0;
	if (m_pLight)
		nVal = m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));
	return nVal;
}

void CManagerPunch::SetLight2(int nVal)
{
	if (m_pLight)
	{
		m_pLight->Set(_tstoi(pDoc->WorkingInfo.Light.sCh[1]), nVal);
		nVal = m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));

		int nPos;
		nPos = int(100.0*(1.0 - (nVal - 0.5) / 255.0));
		CString str;
		str.Format(_T("%d"), nVal);

		pDoc->WorkingInfo.Light.sVal[1] = str;
		::WritePrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);
	}
}

void CManagerPunch::ResetLight()
{
	if (m_pLight)
	{
		m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
	}
}

void CManagerPunch::ResetLight2()
{
	if (m_pLight)
	{
		m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));
	}
}

//===> Motion

void CManagerPunch::ResetMotion()
{
	int nAxis;
	for (nAxis = 0; nAxis < MAX_MS; nAxis++)
	{
		ResetMotion(nAxis);

		if (nAxis < MAX_AXIS)
		{
			while (!m_pMotion->IsServoOn(nAxis))
			{
				if (nAxis == MS_X0 || nAxis == MS_Y0)
					m_pMotion->Clear(MS_X0Y0);
				else if (nAxis == MS_X1 || nAxis == MS_Y1)
					m_pMotion->Clear(MS_X1Y1);
				else
					m_pMotion->Clear(nAxis);
				Sleep(30);
				m_pMotion->ServoOnOff(nAxis, TRUE);
				Sleep(30);
			}
		}
	}
}

void CManagerPunch::ResetMotion(int nMsId)
{
	if (!m_pMotion)
		return;

	long lRtn = m_pMotion->GetState(nMsId);  // -1 : MPIStateERROR, 0 : MPIStateIDLE, 1 : MPIStateSTOPPING, 2 : MPIStateMOVING
	if (lRtn == 2)
	{
		if (nMsId == MS_X0 || nMsId == MS_Y0)
			m_pMotion->Abort(MS_X0Y0);
		else if (nMsId == MS_X1 || nMsId == MS_Y1)
			m_pMotion->Abort(MS_X1Y1);
		else
			m_pMotion->Abort(nMsId);
		Sleep(30);
	}

	if (nMsId == MS_X0 || nMsId == MS_Y0)
		m_pMotion->Clear(MS_X0Y0);
	else if (nMsId == MS_X1 || nMsId == MS_Y1)
		m_pMotion->Clear(MS_X1Y1);
	else
		m_pMotion->Clear(nMsId);

	Sleep(30);

	if (!m_pMotion->IsEnable(nMsId))
	{
		if (nMsId == MS_X0Y0 || nMsId == MS_X0 || nMsId == MS_Y0)
		{
			m_pMotion->ServoOnOff(AXIS_X0, TRUE);
			Sleep(30);
			m_pMotion->ServoOnOff(AXIS_Y0, TRUE);
		}
		else if (nMsId == MS_X1Y1 || nMsId == MS_X1 || nMsId == MS_Y1)
		{
			m_pMotion->ServoOnOff(AXIS_X1, TRUE);
			Sleep(30);
			m_pMotion->ServoOnOff(AXIS_Y1, TRUE);
		}
		else
			m_pMotion->ServoOnOff(nMsId, TRUE);

		Sleep(30);
	}
}

void CManagerPunch::DoInterlock()
{
	if (m_dEnc[AXIS_Y0] < 20.0 && m_dEnc[AXIS_Y1] < 20.0)
	{
		if (Status.bStopFeeding)
			StopFeeding(FALSE);
	}
	else
	{
		if (!Status.bStopFeeding)
			StopFeeding(TRUE);
	}
}

void CManagerPunch::StopFeeding(BOOL bStop)
{
	Status.bStopFeeding = bStop;
	if(bStop)
		pView->MpeWrite(_T("MB440115"), 1); // 마킹부Feeding금지
	else
		pView->MpeWrite(_T("MB440115"), 0); // 마킹부Feeding금지 해제
}

void CManagerPunch::MoveInitPos0(BOOL bWait)
{
	if (!m_pMotion)
		return;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	double dCurrX = m_dEnc[AXIS_X0];
	double dCurrY = m_dEnc[AXIS_Y0];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile0(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if (bWait)
			m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc, ABS, WAIT);
		else
			m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

void CManagerPunch::MoveInitPos1(BOOL bWait)
{
	if (!m_pMotion)	return;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
	double dCurrX = m_dEnc[AXIS_X1];
	double dCurrY = m_dEnc[AXIS_Y1];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		if (bWait)
			m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc, ABS, WAIT);
		else
			m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

void CManagerPunch::GetEnc()
{
	if (!m_pMotion)
		return;

	m_dEnc[AXIS_X0] = m_pMotion->GetActualPosition(AXIS_X0);
	m_dEnc[AXIS_Y0] = m_pMotion->GetActualPosition(AXIS_Y0);
	m_dEnc[AXIS_X1] = m_pMotion->GetActualPosition(AXIS_X1);
	m_dEnc[AXIS_Y1] = m_pMotion->GetActualPosition(AXIS_Y1);
}

BOOL CManagerPunch::ChkCollision()
{
	BOOL* bTHREAD_MK = pView->m_mgrThread->m_bTHREAD_MK;

	double dMg = _tstof(pDoc->WorkingInfo.Motion.sCollisionLength) - _tstof(pDoc->WorkingInfo.Motion.sCollisionMargin);

	if (bTHREAD_MK[0] || bTHREAD_MK[1] || bTHREAD_MK[2] || bTHREAD_MK[3])	// [0] Auto-Left, [1] Auto-Right, [2] Manual-Left, [3] Manual-Right
	{
		if (pDoc->WorkingInfo.System.bNoMk || m_bCam)
			dMg += (_tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX) - _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
	}

	if ((m_dEnc[AXIS_X0] - dMg) > m_dEnc[AXIS_X1])
		return TRUE;

	if (m_dEnc[AXIS_X0] < -1.0 || m_dEnc[AXIS_X1] < -1.0)
		return TRUE;

	return FALSE;
}

BOOL CManagerPunch::GetCollision(int nCam)
{
	return m_bCollision[nCam];
}

void CManagerPunch::ResetPriority()
{
	m_bPriority[0] = FALSE;
	m_bPriority[1] = FALSE;
	m_bPriority[2] = FALSE;
	m_bPriority[3] = FALSE;
}

void CManagerPunch::SetPriority()
{
	int nDir[2];
	nDir[0] = (m_dTarget[AXIS_X0] - m_dEnc[AXIS_X0]) >= 0.0 ? 1 : -1;
	nDir[1] = (m_dTarget[AXIS_X1] - m_dEnc[AXIS_X1]) >= 0.0 ? 1 : -1;

	// Cam0 : m_bPriority[0], m_bPriority[3]
	// Cam1 : m_bPriority[1], m_bPriority[2]
	if (nDir[1] > 0) // Cam1 ->
	{
		m_bPriority[0] = FALSE;
		m_bPriority[1] = TRUE;
		m_bPriority[2] = FALSE;
		m_bPriority[3] = FALSE;
	}
	else if (nDir[0] < 0) // Cam0 <-
	{
		m_bPriority[0] = TRUE;
		m_bPriority[1] = FALSE;
		m_bPriority[2] = FALSE;
		m_bPriority[3] = FALSE;
	}
	else if (nDir[1] < 0) // Cam1 <-
	{
		m_bPriority[0] = FALSE;
		m_bPriority[1] = FALSE;
		m_bPriority[2] = TRUE;
		m_bPriority[3] = FALSE;
	}
	else // Cam0 ->
	{
		m_bPriority[0] = FALSE;
		m_bPriority[1] = FALSE;
		m_bPriority[2] = FALSE;
		m_bPriority[3] = TRUE;
	}
}

BOOL CManagerPunch::IsRunAxisX()
{
	if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_X1))
		return FALSE;
	return TRUE;
}

void CManagerPunch::EStop()
{
	if (m_pMotion)
	{
		m_pMotion->EStop(MS_X0Y0);
		m_pMotion->EStop(MS_X1Y1);
		Sleep(30);
		ResetMotion(MS_X0Y0);
		Sleep(30);
		ResetMotion(MS_X1Y1);
		Sleep(30);
		//DispMsg(_T("X축 충돌 범위에 의한 정지입니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
		pView->ClrDispMsg();
		AfxMessageBox(_T("X축 충돌 범위에 의한 정지입니다."));

		double dCurrX = pView->m_dEnc[AXIS_X1];
		double dCurrY = pView->m_dEnc[AXIS_Y1];
		double pPos[2], fVel, fAcc, fJerk;
		double fLen = 2.0;
		pPos[0] = dCurrX + fLen;
		pPos[1] = dCurrY;
		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		m_pMotion->Move1(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

//===> Punching

BOOL CManagerPunch::GetDoneMk(int nCam)
{
	return m_bDoneMk[nCam];
}

void CManagerPunch::SetDoneMk(int nCam)
{
	m_bDoneMk[nCam] = TRUE;
}

void CManagerPunch::DoMark0()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto)
		return;


	//BOOL bOn;
	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;			// 스트립 양폐 비율

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!IsRun())																		// 정지상태에서
	{
		if (IsOnMarking0())																// 마킹중에
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))				// 모션이 Enable상태이고
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))	// 모션 Done상태이면,
				{
					if (!IsInitPos0() && !IsPinPos0())									// 초기위치가 아니거나, 핀위치가 아닐때
						MoveInitPos0();													// 초기위치로 이동
				}
			}

			if (m_nStepMk[0] < 13 && m_nStepMk[0] > 8) // Mk0();
			{
				m_nStepMk[0] = 8;	// 마킹완료Check
			}
		}
		return;
	}

	m_sDispSts[0].Format(_T("%d"), m_nStepMk[0]);

	switch (m_nStepMk[0])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[0]++;
		break;
	case 1:
		if (!IsInitPos0())
			MoveInitPos0();
		m_nStepMk[0]++;
		break;
	case 2:

		nSerial = m_nBufUpSerial[0]; // Cam0

									 //nSerial = 1;

		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode0(nSerial)) != 1)
			{
				m_nMkPcs[0] = GetTotDefPcs0(nSerial);
				m_nStepMk[0] = MK_END;
			}
			else
			{
				m_nStepMk[0]++;
			}
		}
		else
		{
			StopFromThread();
			//AsyncMsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 1);
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 1);
			BuzzerFromThread(TRUE, 0);
			//pView->DispStsBar(_T("정지-31"), 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[0]++;
		break;
	case 4:
		m_nStepMk[0]++;
		break;
	case 5:
		m_nStepMk[0]++;
		break;
	case 6:

		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))
		{
			if (!IsNoMk0())
			{
				;
			}
			else
			{
				if (!IsReview0())
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0(nSerial);
						m_nStepMk[0] = MK_END;
						break;
					}
				}
			}
			// Punching On이거나 Review이면 다음으로 진행
			SetDelay0(100, 1);		// [mSec]
			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END;
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (!WaitDelay0(1))		// F:Done, T:On Waiting....		// Delay후에
		{
			m_nMkPcs[0] = 0;

			if (!IsNoMk0())										// Punching On이면
			{
				m_nStepMk[0]++;
			}
			else												// Punching이 Off이고
			{
				if (IsReview0())								// Review이면 다음으로
				{
					m_nStepMk[0]++;
				}
				else											// Review가 아니면
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0(nSerial);
						m_nStepMk[0] = MK_END;
					}
					else
						m_nStepMk[0]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[0] + 1 < GetTotDefPcs0(nSerial))		// 다음 마킹위치가 있으면
			{
				ptPnt = GetMkPnt0(nSerial, m_nMkPcs[0] + 1);	// 다음 마킹위치
				m_dNextTarget[AXIS_X0] = ptPnt.x;
				m_dNextTarget[AXIS_Y0] = ptPnt.y;
			}
			else												// 다음 마킹위치가 없으면
			{
				m_dNextTarget[AXIS_X0] = -1.0;
				m_dNextTarget[AXIS_Y0] = -1.0;
			}

			ptPnt = GetMkPnt0(nSerial, m_nMkPcs[0]);			// 이번 마킹위치
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화. (마킹하지 않음)
			{
				m_nMkPcs[0]++;
				m_nStepMk[0] = MK_DONE_CHECK;
				break;
			}

			nIdx = GetMkStripIdx0(nSerial, m_nMkPcs[0]);		// 1 ~ 4 : strip index
			if (nIdx > 0)										// Strip index가 정상이면,
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[0]++;
					m_nStepMk[0] = MK_DONE_CHECK;
					break;
				}
				else
				{
					if (m_nMkStrip[0][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[0]++;
						m_nStepMk[0] = MK_DONE_CHECK;
						break;
					}
					else
						m_nMkStrip[0][nIdx - 1]++;
				}
			}
			else
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;
			m_dTarget[AXIS_Y0] = ptPnt.y;

			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END;
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X0];
		ptPnt.y = m_dTarget[AXIS_Y0];

		if (m_dNextTarget[AXIS_X0] < 0)
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0]);
		else
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0], m_dNextTarget[AXIS_X0]);

		if (!m_bCollision[0])
		{
			if (IsMoveDone0())
			{
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[0])
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[0] = FALSE;
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[2])
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[2] = FALSE;
				ptPnt.x = 0.0;
				ptPnt.y = 0.0;//m_dEnc[AXIS_Y0];
				Move0(ptPnt, FALSE);
			}
		}
		break;
	case 10:
		m_nStepMk[0]++;
		break;
	case 11:
		m_nStepMk[0]++;
		break;
	case 12:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 13:
		if (!IsNoMk0())
		{
			m_dwStMkDn[0] = GetTickCount();
			Mk0();
		}
		else
		{
			// Verify - Mk0
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]
			if (!SaveMk0Img(m_nMkPcs[0]))
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error-SaveMk0Img()"));
			}
			//m_nDebugStep = m_nMkPcs[0]; DispThreadTick();
		}
		m_nStepMk[0]++;
		break;
	case 14:
		m_nStepMk[0]++;
		break;
	case 15:
		m_nStepMk[0]++;
		break;
	case 16:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[0]++;
		}
		else
			m_nStepMk[0]++;
		break;
	case 17:
		if (!IsNoMk0())
		{
			if (IsMk0Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[0] /*&& pDoc->WorkingInfo.Probing[0].bUse*/ && !m_bAnswer[2])
				{
					m_bAnswer[2] = TRUE;
					Mk0();
				}
				else
				{
					m_bAnswer[2] = FALSE;
					m_nMkPcs[0]++;
					m_nStepMk[0]++;
					m_nStepMk[0]++;
				}
			}
			else
			{
				if (m_dwStMkDn[0] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					//pView->DispStsBar(_T("정지-32"), 0);
					DispMain(_T("정 지"), RGB_RED);
					m_pVoiceCoil[0]->SearchHomeSmac0();

					//nRtn = AsyncMsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[0] = 13;
					}
					else if (nRtn < 0)
						m_nStepMk[0]++; // Wait...
					else
					{
						m_bAnswer[2] = FALSE;
						m_nMkPcs[0]++;
						m_nStepMk[0]++;
						m_nStepMk[0]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[0]++;
			m_nStepMk[0]++;
			m_nStepMk[0]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[0] = 13;
			}
			else
			{
				m_bAnswer[2] = FALSE;
				m_nMkPcs[0]++;
				m_nStepMk[0]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[0] = MK_DONE_CHECK;
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))
		{
			if (IsNoMk0())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview0())
					{
						if (IsJogRtDn0())
							m_nStepMk[0]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[0] = GetTotDefPcs0(nSerial);
						m_nStepMk[0] = MK_END;
					}
					else
						m_nStepMk[0] = 8;	// 마킹완료Check
				}
				else
					m_nStepMk[0] = 8;	// 마킹완료Check
			}
			else
				m_nStepMk[0] = 8;	// 마킹완료Check
		}
		else
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtDn0())
						m_nStepMk[0] = MK_END;
				}
				else
				{
					m_nStepMk[0] = MK_END;
				}
			}
			else
				m_nStepMk[0] = MK_END;
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtUp0())
						m_nStepMk[0] = 8;	// 마킹완료Check
				}
				else
				{
					m_nMkPcs[0] = GetTotDefPcs0(nSerial);
					m_nStepMk[0] = MK_END;
				}
			}
			else
				m_nStepMk[0] = 8;	// 마킹완료Check
		}
		else
		{
			m_nStepMk[0] = MK_END;
		}
		break;

	case MK_END:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 101:
		SetDelay0(100, 1);		// [mSec]
		m_nStepMk[0]++;
		break;
	case 102:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			m_nStepMk[0]++;
		}
		break;
	case 103:
		if (!IsInitPos0())
		{
			m_dTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
			m_dNextTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dNextTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);

			MoveInitPos0();
		}

		pDoc->SaveMkCntL();
		m_nStepMk[0]++;
		break;
	case 104:
		if (IsMoveDone0())
		{
			m_nStepMk[0]++;
			SetDelay0(10000, 1);		// [mSec]
		}
		break;
	case 105:
		if (IsInitPos0())
		{
			m_nStepMk[0]++;
		}
		else
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
			{
				m_nStepMk[0] = ERR_PROC;
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[0] = TRUE;
		m_bTHREAD_MK[0] = FALSE;
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		m_pVoiceCoil[0]->SearchHomeSmac0();
		//AsyncMsgBox(_T("보이스코일(좌) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 1);
		MsgBox(_T("보이스코일(좌) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 1);
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 0;
		m_bRtnMyMsgBox[0] = FALSE;
		m_nRtnMyMsgBox[0] = -1;
		//pView->AsyncMsgBox(sMsg, 1, MB_YESNO);
		pView->MsgBox(sMsg, 1, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 10;
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 0;
				m_bRtnMyMsgBox[0] = FALSE;
				m_nRtnMyMsgBox[0] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				//pView->AsyncMsgBox(sMsg, 1, MB_YESNO);
				pView->MsgBox(sMsg, 1, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[0]++;
			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 20;
			}
			else
			{
				m_bDispMsgDoAuto[8] = TRUE;
				m_nStepDispMsg[8] = FROM_DOMARK0;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[0] = TRUE;
		m_bTHREAD_MK[0] = FALSE;
		m_nStepMk[0] = 0;
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[0] = TRUE;
		m_bTHREAD_MK[0] = FALSE;
		break;
	}
}

void CManagerPunch::DoMark1()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto)
		return;

	//BOOL bOn;
	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!IsRun())
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}

			if (m_nStepMk[1] < 13 && m_nStepMk[1] > 8) // Mk1();
			{
				m_nStepMk[1] = 8;	// 마킹완료Check
			}
		}
		return;
	}


	m_sDispSts[1].Format(_T("%d"), m_nStepMk[1]);

	switch (m_nStepMk[1])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[1]++;
		break;
	case 1:
		if (!IsInitPos1())
			MoveInitPos1();
		m_nStepMk[1]++;
		break;
	case 2:

		nSerial = m_nBufUpSerial[1]; // Cam1

		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode1(nSerial)) != 1)
			{
				m_nMkPcs[1] = GetTotDefPcs1(nSerial);
				m_nStepMk[1] = MK_END;
			}
			else
			{
				m_nStepMk[1]++;
			}
		}
		else
		{
			StopFromThread();
			//AsyncMsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 2);
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 2);
			BuzzerFromThread(TRUE, 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[1]++;
		break;
	case 4:
		m_nStepMk[1]++;
		break;
	case 5:
		m_nStepMk[1]++;
		break;
	case 6:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))
		{
			if (!IsNoMk1())
			{
				;
			}
			else
			{
				if (!IsReview1())
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1(nSerial);
						m_nStepMk[1] = MK_END;
						break;
					}
				}
			}
			SetDelay1(100, 6);		// [mSec]
			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END;
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
		{
			m_nMkPcs[1] = 0;

			if (!IsNoMk1())
			{
				m_nStepMk[1]++;
			}
			else
			{
				if (IsReview1())
				{
					m_nStepMk[1]++;
				}
				else
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1(nSerial);
						m_nStepMk[1] = MK_END;
					}
					else
						m_nStepMk[1]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[1] + 1 < GetTotDefPcs1(nSerial))
			{
				ptPnt = GetMkPnt1(nSerial, m_nMkPcs[1] + 1);
				m_dNextTarget[AXIS_X1] = ptPnt.x;
				m_dNextTarget[AXIS_Y1] = ptPnt.y;
			}
			else
			{
				m_dNextTarget[AXIS_X1] = -1.0;
				m_dNextTarget[AXIS_Y1] = -1.0;
			}

			ptPnt = GetMkPnt1(nSerial, m_nMkPcs[1]);
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[1]++;
				m_nStepMk[1] = MK_DONE_CHECK;
				break;
			}

			nIdx = GetMkStripIdx1(nSerial, m_nMkPcs[1]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[1]++;
					m_nStepMk[1] = MK_DONE_CHECK;
					break;
				}
				else
				{
					if (m_nMkStrip[1][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[1]++;
						m_nStepMk[1] = MK_DONE_CHECK;
						break;
					}
					else
						m_nMkStrip[1][nIdx - 1]++;
				}
			}
			else
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;
			m_dTarget[AXIS_Y1] = ptPnt.y;

			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END;
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X1];
		ptPnt.y = m_dTarget[AXIS_Y1];

		if (m_dNextTarget[AXIS_X1] < 0)
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1]);
		else
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1], m_dNextTarget[AXIS_X1]);

		if (!m_bCollision[1])
		{
			if (IsMoveDone1())
			{
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[1])
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[1] = FALSE;
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[3])
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = 0.0;//m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		break;
	case 10:
		m_nStepMk[1]++;
		break;
	case 11:
		m_nStepMk[1]++;
		break;
	case 12:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 13:
		if (!IsNoMk1())
		{
			m_dwStMkDn[1] = GetTickCount();
			Mk1();
		}
		else
		{
			// Verify - Mk1
			SetDelay1(pDoc->m_nDelayShow, 6);		// [mSec]
			if (!SaveMk1Img(m_nMkPcs[1]))
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error-SaveMk1Img()"));
			}
		}
		m_nStepMk[1]++;
		break;
	case 14:
		m_nStepMk[1]++;
		break;
	case 15:
		m_nStepMk[1]++;
		break;
	case 16:
		if (IsNoMk1())
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
				m_nStepMk[1]++;
		}
		else
			m_nStepMk[1]++;
		break;
	case 17:
		if (!IsNoMk1())
		{
			if (IsMk1Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[1] && !m_bAnswer[3])
				{
					m_bAnswer[3] = TRUE;
					Mk1();
				}
				else
				{
					m_bAnswer[3] = FALSE;
					m_nMkPcs[1]++;
					m_nStepMk[1]++;
					m_nStepMk[1]++;
				}

			}
			else
			{
				if (m_dwStMkDn[1] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					m_pVoiceCoil[1]->SearchHomeSmac1();

					//nRtn = AsyncMsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[1] = 13;
					}
					else if (nRtn < 0)
						m_nStepMk[1]++; // Wait...
					else
					{
						m_bAnswer[3] = FALSE;
						m_nMkPcs[1]++;
						m_nStepMk[1]++;
						m_nStepMk[1]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[1]++;
			m_nStepMk[1]++;
			m_nStepMk[1]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[1] = 13;
			}
			else
			{
				m_bAnswer[3] = FALSE;
				m_nMkPcs[1]++;
				m_nStepMk[1]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[1] = MK_DONE_CHECK;
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))
		{
			if (IsNoMk1())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview1())
					{
						if (IsJogRtDn1())
							m_nStepMk[1]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[1] = GetTotDefPcs1(nSerial);
						m_nStepMk[1] = MK_END;
					}
					else
						m_nStepMk[1] = 8;	// 마킹완료Check
				}
				else
					m_nStepMk[1] = 8;	// 마킹완료Check
			}
			else
				m_nStepMk[1] = 8;	// 마킹완료Check
		}
		else
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtDn1())
						m_nStepMk[1] = MK_END;
				}
				else
				{
					m_nStepMk[1] = MK_END;
				}
			}
			else
				m_nStepMk[1] = MK_END;
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtUp1())
						m_nStepMk[1] = 8;	// 마킹완료Check
				}
				else
				{
					m_nMkPcs[1] = GetTotDefPcs1(nSerial);
					m_nStepMk[1] = MK_END;
				}
			}
			else
				m_nStepMk[1] = 8;	// 마킹완료Check
		}
		else
		{
			m_nStepMk[1] = MK_END;
		}
		break;


	case MK_END:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 101:
		SetDelay1(100, 6);		// [mSec]
		m_nStepMk[1]++;
		break;
	case 102:
		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			m_nStepMk[1]++;
		break;
	case 103:
		if (!IsMkEdPos1())
		{
			m_dTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
			m_dNextTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dNextTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);

			MoveMkEdPos1();
		}

		pDoc->SaveMkCntR();
		m_nStepMk[1]++;
		break;
	case 104:
		if (IsMoveDone1())
		{
			m_nStepMk[1]++;
			SetDelay1(10000, 6);		// [mSec]
		}
		break;
	case 105:
		if (IsMkEdPos1())
		{
			m_nStepMk[1]++;
		}
		else
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			{
				m_nStepMk[1] = ERR_PROC;
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[1] = TRUE;
		m_bTHREAD_MK[1] = FALSE;
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		m_pVoiceCoil[1]->SearchHomeSmac1();
		//AsyncMsgBox(_T("보이스코일(우) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 2);
		MsgBox(_T("보이스코일(우) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 2);
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 1;
		m_bRtnMyMsgBox[1] = FALSE;
		m_nRtnMyMsgBox[1] = -1;
		//pView->AsyncMsgBox(sMsg, 2, MB_YESNO);
		pView->MsgBox(sMsg, 2, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 10;
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 1;
				m_bRtnMyMsgBox[1] = FALSE;
				m_nRtnMyMsgBox[1] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				//pView->AsyncMsgBox(sMsg, 2, MB_YESNO);
				pView->MsgBox(sMsg, 2, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[1]++;

			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 20;
			}
			else
			{
				m_bDispMsgDoAuto[9] = TRUE;
				m_nStepDispMsg[9] = FROM_DOMARK1;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[1] = TRUE;
		m_bTHREAD_MK[1] = FALSE;
		m_nStepMk[1] = 0;
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[1] = TRUE;
		m_bTHREAD_MK[1] = FALSE;
		break;
	}
}

void CManagerPunch::DoMark0Its()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto)
		return;

	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;			// 스트립 양폐 비율

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!IsRun())																		// 정지상태에서
	{
		if (IsOnMarking0())																// 마킹중에
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))				// 모션이 Enable상태이고
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))	// 모션 Done상태이면,
				{
					if (!IsInitPos0() && !IsPinPos0())									// 초기위치가 아니거나, 핀위치가 아닐때
						MoveInitPos0();													// 초기위치로 이동
				}
			}

			if (m_nStepMk[0] < 13 && m_nStepMk[0] > 8) // Mk0();
			{
				m_nStepMk[0] = 8;	// 마킹완료Check
			}
		}
		return;
	}

	m_sDispSts[0].Format(_T("%d"), m_nStepMk[0]);

	switch (m_nStepMk[0])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[0]++;
		break;
	case 1:
		if (!IsInitPos0())
			MoveInitPos0();
		m_nStepMk[0]++;
		break;
	case 2:

		nSerial = m_nBufUpSerial[0]; // Cam0

		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode0Its(nSerial)) != 1)
			{
				m_nMkPcs[0] = GetTotDefPcs0Its(nSerial);
				m_nStepMk[0] = MK_END;
			}
			else
			{
				m_nStepMk[0]++;
			}
		}
		else
		{
			StopFromThread();
			//AsyncMsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 1);
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 1);
			BuzzerFromThread(TRUE, 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[0]++;
		break;
	case 4:
		m_nStepMk[0]++;
		break;
	case 5:
		m_nStepMk[0]++;
		break;
	case 6:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))
		{
			if (!IsNoMk0())
			{
				;
			}
			else
			{
				if (!IsReview0())
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0Its(nSerial);
						m_nStepMk[0] = MK_END;
						break;
					}
				}
			}
			// Punching On이거나 Review이면 다음으로 진행
			SetDelay0(100, 1);		// [mSec]
			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END;
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (!WaitDelay0(1))		// F:Done, T:On Waiting....		// Delay후에
		{
			m_nMkPcs[0] = 0;

			if (!IsNoMk0())										// Punching On이면
			{
				m_nStepMk[0]++;
			}
			else												// Punching이 Off이고
			{
				if (IsReview0())								// Review이면 다음으로
				{
					m_nStepMk[0]++;
				}
				else											// Review가 아니면
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0Its(nSerial);
						m_nStepMk[0] = MK_END;
					}
					else
						m_nStepMk[0]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[0] + 1 < GetTotDefPcs0Its(nSerial))		// 다음 마킹위치가 있으면
			{
				ptPnt = GetMkPnt0Its(nSerial, m_nMkPcs[0] + 1);	// 다음 마킹위치
				m_dNextTarget[AXIS_X0] = ptPnt.x;
				m_dNextTarget[AXIS_Y0] = ptPnt.y;
			}
			else												// 다음 마킹위치가 없으면
			{
				m_dNextTarget[AXIS_X0] = -1.0;
				m_dNextTarget[AXIS_Y0] = -1.0;
			}

			ptPnt = GetMkPnt0Its(nSerial, m_nMkPcs[0]);			// 이번 마킹위치
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화. (마킹하지 않음)
			{
				m_nMkPcs[0]++;
				m_nStepMk[0] = MK_DONE_CHECK;
				break;
			}

			nIdx = GetMkStripIdx0Its(nSerial, m_nMkPcs[0]);		// 1 ~ 4 : strip index
			if (nIdx > 0)										// Strip index가 정상이면,
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[0]++;
					m_nStepMk[0] = MK_DONE_CHECK;
					break;
				}
				else
				{
					if (m_nMkStrip[0][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[0]++;
						m_nStepMk[0] = MK_DONE_CHECK;
						break;
					}
					else
						m_nMkStrip[0][nIdx - 1]++;
				}
			}
			else
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;
			m_dTarget[AXIS_Y0] = ptPnt.y;

			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END;
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X0];
		ptPnt.y = m_dTarget[AXIS_Y0];

		if (m_dNextTarget[AXIS_X0] < 0)
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0]);
		else
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0], m_dNextTarget[AXIS_X0]);

		if (!m_bCollision[0])
		{
			if (IsMoveDone0())
			{
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[0])
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[0] = FALSE;
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[2])
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[2] = FALSE;
				ptPnt.x = 0.0;
				ptPnt.y = 0.0;//m_dEnc[AXIS_Y0];
				Move0(ptPnt, FALSE);
			}
		}
		break;
	case 10:
		m_nStepMk[0]++;
		break;
	case 11:
		m_nStepMk[0]++;
		break;
	case 12:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 13:
		if (!IsNoMk0())
		{
			m_dwStMkDn[0] = GetTickCount();
			Mk0();
		}
		else
		{
			// Verify - Mk0
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]
			if (!SaveMk0Img(m_nMkPcs[0]))
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error-SaveMk0Img()"));
			}
		}
		m_nStepMk[0]++;
		break;
	case 14:
		m_nStepMk[0]++;
		break;
	case 15:
		m_nStepMk[0]++;
		break;
	case 16:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[0]++;
		}
		else
			m_nStepMk[0]++;
		break;
	case 17:
		if (!IsNoMk0())
		{
			if (IsMk0Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[0] && !m_bAnswer[2])
				{
					m_bAnswer[2] = TRUE;
					Mk0();
				}
				else
				{
					m_bAnswer[2] = FALSE;
					m_nMkPcs[0]++;
					m_nStepMk[0]++;
					m_nStepMk[0]++;
				}
			}
			else
			{
				if (m_dwStMkDn[0] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					m_pVoiceCoil[0]->SearchHomeSmac0();

					//nRtn = AsyncMsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[0] = 13;
					}
					else if (nRtn < 0)
						m_nStepMk[0]++; // Wait...
					else
					{
						m_bAnswer[2] = FALSE;
						m_nMkPcs[0]++;
						m_nStepMk[0]++;
						m_nStepMk[0]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[0]++;
			m_nStepMk[0]++;
			m_nStepMk[0]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[0] = 13;
			}
			else
			{
				m_bAnswer[2] = FALSE;
				m_nMkPcs[0]++;
				m_nStepMk[0]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[0] = MK_DONE_CHECK;
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))
		{
			if (IsNoMk0())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview0())
					{
						if (IsJogRtDn0())
							m_nStepMk[0]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[0] = GetTotDefPcs0Its(nSerial);
						m_nStepMk[0] = MK_END;
					}
					else
						m_nStepMk[0] = 8;	// 마킹완료Check
				}
				else
					m_nStepMk[0] = 8;	// 마킹완료Check
			}
			else
				m_nStepMk[0] = 8;	// 마킹완료Check
		}
		else
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtDn0())
						m_nStepMk[0] = MK_END;
				}
				else
				{
					m_nStepMk[0] = MK_END;
				}
			}
			else
				m_nStepMk[0] = MK_END;
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtUp0())
						m_nStepMk[0] = 8;	// 마킹완료Check
				}
				else
				{
					m_nMkPcs[0] = GetTotDefPcs0Its(nSerial);
					m_nStepMk[0] = MK_END;
				}
			}
			else
				m_nStepMk[0] = 8;	// 마킹완료Check
		}
		else
		{
			m_nStepMk[0] = MK_END;
		}
		break;

	case MK_END:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 101:
		SetDelay0(100, 1);		// [mSec]
		m_nStepMk[0]++;
		break;
	case 102:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			m_nStepMk[0]++;
		}
		break;
	case 103:
		if (!IsInitPos0())
		{
			m_dTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
			m_dNextTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dNextTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);

			MoveInitPos0();
		}

		pDoc->SaveMkCntL();
		m_nStepMk[0]++;
		break;
	case 104:
		if (IsMoveDone0())
		{
			m_nStepMk[0]++;
			SetDelay0(10000, 1);		// [mSec]
		}
		break;
	case 105:
		if (IsInitPos0())
		{
			m_nStepMk[0]++;
		}
		else
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
			{
				m_nStepMk[0] = ERR_PROC;
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[0] = TRUE;
		m_bTHREAD_MK[0] = FALSE;
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		m_pVoiceCoil[0]->SearchHomeSmac0();
		//AsyncMsgBox(_T("보이스코일(좌) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 1);
		MsgBox(_T("보이스코일(좌) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 1);
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[0];

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 0;
		m_bRtnMyMsgBox[0] = FALSE;
		m_nRtnMyMsgBox[0] = -1;
		//pView->AsyncMsgBox(sMsg, 1, MB_YESNO);
		pView->MsgBox(sMsg, 1, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 10;
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 0;
				m_bRtnMyMsgBox[0] = FALSE;
				m_nRtnMyMsgBox[0] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				//pView->AsyncMsgBox(sMsg, 1, MB_YESNO);
				pView->MsgBox(sMsg, 1, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[0]++;
			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 20;
			}
			else
			{
				m_bDispMsgDoAuto[8] = TRUE;
				m_nStepDispMsg[8] = FROM_DOMARK0;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[0] = TRUE;
		m_bTHREAD_MK[0] = FALSE;
		m_nStepMk[0] = 0;
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[0] = TRUE;
		m_bTHREAD_MK[0] = FALSE;
		break;
	}
}

void CManagerPunch::DoMark1Its()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto)
		return;

	//BOOL bOn;
	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;


	if (!IsRun())
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}

			if (m_nStepMk[1] < 13 && m_nStepMk[1] > 8) // Mk1();
			{
				m_nStepMk[1] = 8;	// 마킹완료Check
			}
		}
		return;
	}


	m_sDispSts[1].Format(_T("%d"), m_nStepMk[1]);

	switch (m_nStepMk[1])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[1]++;
		break;
	case 1:
		if (!IsInitPos1())
			MoveInitPos1();
		m_nStepMk[1]++;
		break;
	case 2:

		nSerial = m_nBufUpSerial[1]; // Cam1

		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode1Its(nSerial)) != 1)
			{
				m_nMkPcs[1] = GetTotDefPcs1Its(nSerial);
				m_nStepMk[1] = MK_END;
			}
			else
			{
				m_nStepMk[1]++;
			}
		}
		else
		{
			StopFromThread();
			//AsyncMsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 2);
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 2);
			BuzzerFromThread(TRUE, 0);
			//pView->DispStsBar(_T("정지-34"), 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[1]++;
		break;
	case 4:
		m_nStepMk[1]++;
		break;
	case 5:
		m_nStepMk[1]++;
		break;
	case 6:

		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))
		{
			if (!IsNoMk1())
			{
				;
			}
			else
			{
				if (!IsReview1())
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1Its(nSerial);
						m_nStepMk[1] = MK_END;
						break;
					}
				}
			}
			SetDelay1(100, 6);		// [mSec]
			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END;
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
		{
			m_nMkPcs[1] = 0;

			if (!IsNoMk1())
			{
				m_nStepMk[1]++;
			}
			else
			{
				if (IsReview1())
				{
					m_nStepMk[1]++;
				}
				else
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1Its(nSerial);
						m_nStepMk[1] = MK_END;
					}
					else
						m_nStepMk[1]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[1] + 1 < GetTotDefPcs1Its(nSerial))
			{
				ptPnt = GetMkPnt1Its(nSerial, m_nMkPcs[1] + 1);
				m_dNextTarget[AXIS_X1] = ptPnt.x;
				m_dNextTarget[AXIS_Y1] = ptPnt.y;
			}
			else
			{
				m_dNextTarget[AXIS_X1] = -1.0;
				m_dNextTarget[AXIS_Y1] = -1.0;
			}

			ptPnt = GetMkPnt1Its(nSerial, m_nMkPcs[1]);
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[1]++;
				m_nStepMk[1] = MK_DONE_CHECK;
				break;
			}

			nIdx = GetMkStripIdx1Its(nSerial, m_nMkPcs[1]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[1]++;
					m_nStepMk[1] = MK_DONE_CHECK;
					break;
				}
				else
				{
					if (m_nMkStrip[1][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[1]++;
						m_nStepMk[1] = MK_DONE_CHECK;
						break;
					}
					else
						m_nMkStrip[1][nIdx - 1]++;
				}
			}
			else
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;
			m_dTarget[AXIS_Y1] = ptPnt.y;

			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END;
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X1];
		ptPnt.y = m_dTarget[AXIS_Y1];

		if (m_dNextTarget[AXIS_X1] < 0)
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1]);
		else
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1], m_dNextTarget[AXIS_X1]);

		if (!m_bCollision[1])
		{
			if (IsMoveDone1())
			{
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[1])
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[1] = FALSE;
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[3])
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = 0.0;//m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		break;
	case 10:
		m_nStepMk[1]++;
		break;
	case 11:
		m_nStepMk[1]++;
		break;
	case 12:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 13:
		if (!IsNoMk1())
		{
			m_dwStMkDn[1] = GetTickCount();
			Mk1();
		}
		else
		{
			// Verify - Mk1
			SetDelay1(pDoc->m_nDelayShow, 6);		// [mSec]
			if (!SaveMk1Img(m_nMkPcs[1]))
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error-SaveMk1Img()"));
			}
		}
		m_nStepMk[1]++;
		break;
	case 14:
		m_nStepMk[1]++;
		break;
	case 15:
		m_nStepMk[1]++;
		break;
	case 16:
		if (IsNoMk1())
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
				m_nStepMk[1]++;
		}
		else
			m_nStepMk[1]++;
		break;
	case 17:
		if (!IsNoMk1())
		{
			if (IsMk1Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[1] && !m_bAnswer[3])
				{
					m_bAnswer[3] = TRUE;
					Mk1();
				}
				else
				{
					m_bAnswer[3] = FALSE;
					m_nMkPcs[1]++;
					m_nStepMk[1]++;
					m_nStepMk[1]++;
				}

			}
			else
			{
				if (m_dwStMkDn[1] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					m_pVoiceCoil[1]->SearchHomeSmac1();

					//nRtn = AsyncMsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[1] = 13;
					}
					else if (nRtn < 0)
						m_nStepMk[1]++; // Wait...
					else
					{
						m_bAnswer[3] = FALSE;
						m_nMkPcs[1]++;
						m_nStepMk[1]++;
						m_nStepMk[1]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[1]++;
			m_nStepMk[1]++;
			m_nStepMk[1]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[1] = 13;
			}
			else
			{
				m_bAnswer[3] = FALSE;
				m_nMkPcs[1]++;
				m_nStepMk[1]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[1] = MK_DONE_CHECK;
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))
		{
			if (IsNoMk1())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview1())
					{
						if (IsJogRtDn1())
							m_nStepMk[1]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[1] = GetTotDefPcs1Its(nSerial);
						m_nStepMk[1] = MK_END;
					}
					else
						m_nStepMk[1] = 8;	// 마킹완료Check
				}
				else
					m_nStepMk[1] = 8;	// 마킹완료Check
			}
			else
				m_nStepMk[1] = 8;	// 마킹완료Check
		}
		else
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtDn1())
						m_nStepMk[1] = MK_END;
				}
				else
				{
					m_nStepMk[1] = MK_END;
				}
			}
			else
				m_nStepMk[1] = MK_END;
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtUp1())
						m_nStepMk[1] = 8;	// 마킹완료Check
				}
				else
				{
					m_nMkPcs[1] = GetTotDefPcs1Its(nSerial);
					m_nStepMk[1] = MK_END;
				}
			}
			else
				m_nStepMk[1] = 8;	// 마킹완료Check
		}
		else
		{
			m_nStepMk[1] = MK_END;
		}
		break;


	case MK_END:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 101:
		SetDelay1(100, 6);		// [mSec]
		m_nStepMk[1]++;
		break;
	case 102:
		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			m_nStepMk[1]++;
		break;
	case 103:
		if (!IsMkEdPos1())
		{
			m_dTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
			m_dNextTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dNextTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);

			MoveMkEdPos1();
		}

		pDoc->SaveMkCntR();
		m_nStepMk[1]++;
		break;
	case 104:
		if (IsMoveDone1())
		{
			m_nStepMk[1]++;
			SetDelay1(10000, 6);		// [mSec]
		}
		break;
	case 105:
		if (IsMkEdPos1())
		{
			m_nStepMk[1]++;
		}
		else
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			{
				m_nStepMk[1] = ERR_PROC;
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[1] = TRUE;
		m_bTHREAD_MK[1] = FALSE;
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		m_pVoiceCoil[1]->SearchHomeSmac1();
		//AsyncMsgBox(_T("보이스코일(우) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 2);
		MsgBox(_T("보이스코일(우) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 2);
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 1;
		m_bRtnMyMsgBox[1] = FALSE;
		m_nRtnMyMsgBox[1] = -1;
		//pView->AsyncMsgBox(sMsg, 2, MB_YESNO);
		pView->MsgBox(sMsg, 2, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 10;
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 1;
				m_bRtnMyMsgBox[1] = FALSE;
				m_nRtnMyMsgBox[1] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				//pView->AsyncMsgBox(sMsg, 2, MB_YESNO);
				pView->MsgBox(sMsg, 2, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[1]++;

			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 20;
			}
			else
			{
				m_bDispMsgDoAuto[9] = TRUE;
				m_nStepDispMsg[9] = FROM_DOMARK1;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[1] = TRUE;
		m_bTHREAD_MK[1] = FALSE;
		m_nStepMk[1] = 0;
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[1] = TRUE;
		m_bTHREAD_MK[1] = FALSE;
		break;
	}
}

void CManagerPunch::DoMark0All()
{
	CfPoint ptPnt;

	if (!IsRun())
	{
		if (IsOnMarking0())
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))
				{
					if (!IsInitPos0() && !IsPinPos0())
						MoveInitPos0();
				}
			}

			if (m_nStepMk[2] < 5 && m_nStepMk[2] > 3) // Mk0();
			{
				m_nStepMk[2] = 3;	// 마킹완료Check
			}
		}
		return;
	}


	switch (m_nStepMk[2])
	{
	case 0:
		m_nStepMk[2]++;
		break;
	case 1:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)
		{
			m_nStepMk[2]++;
		}
		else
		{
			m_nStepMk[2] = MK_END;
		}
		break;
	case 2:
		m_nStepMk[2]++;
		break;
	case 3:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt0(m_nMkPcs[2]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[2]++;
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;

			// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
			// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-

			m_bCollision[0] = ChkCollision(AXIS_X0, ptPnt.x);
			if (!m_bCollision[0])
			{
				if (IsMoveDone0())
				{
					Move0(ptPnt, m_bCam);
					m_nStepMk[2]++;
				}
			}
			else if (m_bPriority[0])
			{
				if (IsMoveDone0())
				{
					m_bPriority[0] = FALSE;
					Move0(ptPnt, m_bCam);
					m_nStepMk[2]++;
				}
			}
			else if (m_bPriority[2])
			{
				if (IsMoveDone0())
				{
					m_bPriority[2] = FALSE;
					ptPnt.x = 0.0;
					ptPnt.y = 0.0;//m_dEnc[AXIS_Y0];
					Move0(ptPnt, FALSE);
				}
			}
		}
		else
		{
			m_nStepMk[2] = MK_END;
		}
		break;
	case 4:
		if (IsMoveDone0())
			m_nStepMk[2]++;
		break;
	case 5:
		if (!IsNoMk0())
			Mk0();
		else
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 6:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[2]++;
		}
		else
			m_nStepMk[2]++;
		break;
	case 7:
		m_nMkPcs[2]++;
		m_nStepMk[2]++;
		break;
	case 8:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[2] = 3;
		}
		else
		{
			m_nStepMk[2] = MK_END;
		}
		break;
	case MK_END:
		SetDelay0(100, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 101:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			if (IsMoveDone0())
			{
				MoveInitPos0();
				m_nStepMk[2]++;
			}
		}
		break;
	case 102:
		if (IsMoveDone0())
			m_nStepMk[2]++;
		break;
	case 103:
		m_bDoneMk[0] = TRUE;
		m_bTHREAD_MK[2] = FALSE;
		break;
	}
}

void CManagerPunch::DoMark1All()
{
	CfPoint ptPnt;

	if (!IsRun())
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}

			if (m_nStepMk[3] < 5 && m_nStepMk[3] > 3) // Mk1();
			{
				m_nStepMk[3] = 3;	// 마킹완료Check
			}
		}
		return;
	}


	switch (m_nStepMk[3])
	{
	case 0:
		m_nStepMk[3]++;
		break;
	case 1:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)
		{
			m_nStepMk[3]++;
		}
		else
		{
			m_nStepMk[3] = MK_END;
		}
		break;
	case 2:
		m_nStepMk[3]++;
		break;
	case 3:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt1(m_nMkPcs[3]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[3]++;
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;

			// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
			// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-

			m_bCollision[1] = ChkCollision(AXIS_X1, ptPnt.x);
			if (!m_bCollision[1])
			{
				if (IsMoveDone1())
				{
					Move1(ptPnt, m_bCam);
					m_nStepMk[3]++;
				}
			}
			else if (m_bPriority[1])
			{
				if (IsMoveDone1())
				{
					m_bPriority[1] = FALSE;
					Move1(ptPnt, m_bCam);
					m_nStepMk[3]++;
				}
			}
			else if (m_bPriority[3])
			{
				if (IsMoveDone1())
				{
					m_bPriority[3] = FALSE;
					ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
					ptPnt.y = 0.0;//m_dEnc[AXIS_Y1];
					Move1(ptPnt, FALSE);
				}
			}
		}
		else
		{
			m_nStepMk[3] = MK_END;
		}
		break;
	case 4:
		if (IsMoveDone1())
			m_nStepMk[3]++;
		break;
	case 5:
		if (!IsNoMk1())
			Mk1();
		else
			SetDelay1(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 6:
		if (IsNoMk1())
		{
			if (!WaitDelay1(1))		// F:Done, T:On Waiting....
				m_nStepMk[3]++;
		}
		else
			m_nStepMk[3]++;
		break;
	case 7:
		m_nMkPcs[3]++;
		m_nStepMk[3]++;
		break;
	case 8:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[3] = 3;
		}
		else
		{
			//StopLive1();
			m_nStepMk[3] = MK_END;
		}
		break;
	case MK_END:
		SetDelay1(100, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 101:
		if (!WaitDelay1(1))		// F:Done, T:On Waiting....
		{
			if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)
			{
				if (m_nMkPcs[2] > 0)
				{
					if (!(m_Flag & (0x01 << 0)))
					{
						if (IsMoveDone1())
						{
							m_Flag |= (0x01 << 0);
							ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
							ptPnt.y = 0.0;//m_dEnc[AXIS_Y1];
							Move1(ptPnt, FALSE);
						}
					}
				}
				else
				{
					if (IsMoveDone1())
					{
						m_Flag &= ~(0x01 << 0);
						MoveMkEdPos1();
						m_nStepMk[3]++;
					}
				}
			}
			else
			{
				if (IsMoveDone1())
				{
					m_Flag &= ~(0x01 << 0);
					MoveMkEdPos1();
					m_nStepMk[3]++;
				}
			}
		}
		break;
	case 102:
		if (IsMoveDone0())
			m_nStepMk[3]++;
		break;
	case 103:
		m_bDoneMk[1] = TRUE;
		m_bTHREAD_MK[3] = FALSE;
		break;
	}
}

void CManagerPunch::DoReject0()
{
#ifdef TEST_MODE
	return;
#endif

	//BOOL bOn;
	int nIdx, nRtn;//nSerial, 
	CfPoint ptPnt;

	if (!IsRun() && m_bAuto)
	{
		if (IsOnMarking0())
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))
				{
					if (!IsInitPos0() && !IsPinPos0())
						MoveInitPos0();
				}
			}
		}
		return;
	}


	switch (m_nStepMk[2])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nMkStrip[0][0] = 0;
		m_nMkStrip[0][1] = 0;
		m_nMkStrip[0][2] = 0;
		m_nMkStrip[0][3] = 0;
		m_bRejectDone[0][0] = FALSE;
		m_bRejectDone[0][1] = FALSE;
		m_bRejectDone[0][2] = FALSE;
		m_bRejectDone[0][3] = FALSE;
		m_nStepMk[2]++;
		break;
	case 1:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)
		{
			m_nStepMk[2]++;
		}
		else
		{
			m_nStepMk[2] = MK_END;
		}
		break;
	case 2:
		m_nStepMk[2]++;
		break;
	case 3:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt0(m_nMkPcs[2]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[2]++;
				break;
			}

			nIdx = GetMkStripIdx0(m_nMkPcs[2]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[2]++;
					break;
				}
				else
				{
					if (m_nMkStrip[0][nIdx - 1] > pDoc->GetStripRejectMkNum())
					{
						m_bRejectDone[0][nIdx - 1] = TRUE;
						m_nMkPcs[2]++;
						break;
					}
					else
						m_nMkStrip[0][nIdx - 1]++;
				}
			}
			else
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			if (m_bRejectDone[0][0] && m_bRejectDone[0][1] &&
				m_bRejectDone[0][2] && m_bRejectDone[0][3])
			{
				m_nStepMk[2] = MK_END;
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;

			// Cam0 : m_bPriority[0], m_bPriority[3]
			// Cam1 : m_bPriority[1], m_bPriority[2]

			m_bCollision[0] = ChkCollision(AXIS_X0, ptPnt.x);
			if (!m_bCollision[0])
			{
				Move0(ptPnt, m_bCam);
				m_nStepMk[2]++;
			}
			else if (m_bPriority[0])
			{
				m_bPriority[0] = FALSE;
				Move0(ptPnt, m_bCam);
				m_nStepMk[2]++;
			}
			else if (m_bPriority[2])
			{
				m_bPriority[2] = FALSE;
				ptPnt.x = 0.0;
				ptPnt.y = 0.0;//m_dEnc[AXIS_Y0];
				Move0(ptPnt, m_bCam);
			}
		}
		else
		{
			m_nStepMk[2] = MK_END;
		}
		break;
	case 4:
		m_nStepMk[2]++;
		break;
	case 5:
		m_nStepMk[2]++;
		break;
	case 6:
		if (IsMoveDone0())
			m_nStepMk[2]++;
		break;
	case 7:
		if (!IsNoMk0())
		{
			m_dwStMkDn[0] = GetTickCount();
			Mk0();
		}
		else
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 8:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[2]++;
		}
		else
			m_nStepMk[2]++;
		break;
	case 9:
		m_nStepMk[2]++;
		break;
	case 10:
		if (!IsNoMk0())
		{
			//Mk0(FALSE);
			if (IsMk0Done())
			{
				m_nMkPcs[2]++;
				m_nStepMk[2]++;
				m_nStepMk[2]++;
			}
			else
			{
				if (m_dwStMkDn[0] + 5000 < GetTickCount())
				{
					Buzzer(TRUE, 0);
					//pView->DispStsBar(_T("정지-29"), 0);
					DispMain(_T("정 지"), RGB_RED);
					m_pVoiceCoil[0]->SearchHomeSmac0();

					//nRtn = AsyncMsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[2] = 7;
					}
					else if (nRtn < 0)
						m_nStepMk[2]++;
					else
					{
						m_nMkPcs[2]++;
						m_nStepMk[2]++;
						m_nStepMk[2]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[2]++;
			m_nStepMk[2]++;
			m_nStepMk[2]++;
		}
		break;
	case 11:
		//m_nStepMk[2]++;
		//if(m_bRtnMyMsgBox[2])
		if ((nRtn = WaitRtnVal(3)) > -1)
		{
			//if(IDYES == m_nRtnMyMsgBox[2])
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[2] = 7;
			}
			else
			{
				m_nMkPcs[2]++;
				m_nStepMk[2]++;
				Stop();
			}
		}
		break;
	case 12:
		m_nStepMk[2]++;
		break;
	case 13:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[2] = 3;
		}
		else
		{
			m_nStepMk[2] = MK_END;
		}
		break;
	case MK_END:
		m_pVoiceCoil[0]->SearchHomeSmac0();
		SetDelay0(500, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 101:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			MoveInitPos0();
			m_nStepMk[2]++;
		}
		break;
	case 102:
		if (IsMoveDone0())
		{
			m_bTHREAD_MK[2] = FALSE;
			m_bDoneMk[0] = TRUE;
			m_nStepMk[2] = 0;
			m_nMkStrip[0][0] = 0;
			m_nMkStrip[0][1] = 0;
			m_nMkStrip[0][2] = 0;
			m_nMkStrip[0][3] = 0;
		}
		break;
	}
}

void CManagerPunch::DoReject1()
{
#ifdef TEST_MODE
	return;
#endif

	//BOOL bOn;
	int nIdx, nRtn;//nSerial, 
	CfPoint ptPnt;

	if (!IsRun() && m_bAuto)
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}
		}
		return;
	}


	switch (m_nStepMk[3])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nMkStrip[1][0] = 0;
		m_nMkStrip[1][1] = 0;
		m_nMkStrip[1][2] = 0;
		m_nMkStrip[1][3] = 0;
		m_bRejectDone[1][0] = FALSE;
		m_bRejectDone[1][1] = FALSE;
		m_bRejectDone[1][2] = FALSE;
		m_bRejectDone[1][3] = FALSE;
		m_nStepMk[3]++;
		break;
	case 1:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)
		{
			m_nStepMk[3]++;
		}
		else
		{
			m_nStepMk[3] = MK_END;
		}
		break;
	case 2:
		m_nStepMk[3]++;
		break;
	case 3:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt1(m_nMkPcs[3]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[3]++;
				break;
			}

			nIdx = GetMkStripIdx1(m_nMkPcs[3]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[3]++;
					break;
				}
				else
				{
					if (m_nMkStrip[1][nIdx - 1] > pDoc->GetStripRejectMkNum())
					{
						m_bRejectDone[1][nIdx - 1] = TRUE;
						m_nMkPcs[3]++;
						break;
					}
					else
						m_nMkStrip[1][nIdx - 1]++;
				}
			}
			else
			{
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			if (m_bRejectDone[1][0] && m_bRejectDone[1][1] &&
				m_bRejectDone[1][2] && m_bRejectDone[1][3])
			{
				m_nStepMk[2] = MK_END;
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;

			// Cam0 : m_bPriority[0], m_bPriority[3]
			// Cam1 : m_bPriority[1], m_bPriority[2]

			m_bCollision[1] = ChkCollision(AXIS_X1, ptPnt.x);
			if (!m_bCollision[1])
			{
				Move1(ptPnt, m_bCam);
				m_nStepMk[3]++;
			}
			else if (m_bPriority[1])
			{
				m_bPriority[1] = FALSE;
				Move1(ptPnt, m_bCam);
				m_nStepMk[3]++;
			}
			else if (m_bPriority[3])
			{
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = 0.0;//m_dEnc[AXIS_Y1];
				Move1(ptPnt, m_bCam);
			}
		}
		else
		{
			m_nStepMk[3] = MK_END;
		}
		break;
	case 4:
		m_nStepMk[3]++;
		break;
	case 5:
		m_nStepMk[3]++;
		break;
	case 6:
		if (IsMoveDone1())
			m_nStepMk[3]++;
		break;
	case 7:
		if (!IsNoMk1())
		{
			m_dwStMkDn[1] = GetTickCount();
			Mk1();
		}
		else
			SetDelay1(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 8:
		if (IsNoMk1())
		{
			if (!WaitDelay1(1))		// F:Done, T:On Waiting....
				m_nStepMk[3]++;
		}
		else
			m_nStepMk[3]++;
		break;
	case 9:
		m_nStepMk[3]++;
		break;
	case 10:
		if (!IsNoMk1())
		{
			if (IsMk1Done())
			{
				m_nMkPcs[3]++;
				m_nStepMk[3]++;
				m_nStepMk[3]++;
			}
			else
			{
				if (m_dwStMkDn[1] + 5000 < GetTickCount())
				{
					Buzzer(TRUE, 0);
					//pView->DispStsBar(_T("정지-30"), 0);
					DispMain(_T("정 지"), RGB_RED);
					m_pVoiceCoil[1]->SearchHomeSmac1();

					//nRtn = AsyncMsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[3] = 7;
					}
					else if (nRtn < 0)
						m_nStepMk[3]++;
					else
					{
						m_nMkPcs[3]++;
						m_nStepMk[3]++;
						m_nStepMk[3]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[3]++;
			m_nStepMk[3]++;
			m_nStepMk[3]++;
		}
		break;
	case 11:
		//m_nStepMk[3]++;
		//if(m_bRtnMyMsgBox[3])
		if ((nRtn = WaitRtnVal(4)) > -1)
		{
			//if(IDYES == m_nRtnMyMsgBox[3])
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[3] = 7;
			}
			else
			{
				m_nMkPcs[3]++;
				m_nStepMk[3]++;
				Stop();
			}
		}
		break;
	case 12:
		m_nStepMk[3]++;
		break;
	case 13:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[3] = 3;
		}
		else
		{
			m_nStepMk[3] = MK_END;
		}
		break;
	case MK_END:
		m_pVoiceCoil[1]->SearchHomeSmac1();
		SetDelay1(500, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 101:
		if (!WaitDelay1(1))		// F:Done, T:On Waiting....
		{
			MoveMkEdPos1();
			m_nStepMk[3]++;
		}
		break;
	case 102:
		if (IsMoveDone1())
		{
			m_bTHREAD_MK[3] = FALSE;
			m_bDoneMk[1] = TRUE;
			m_nStepMk[3] = 0;
			m_nMkStrip[1][0] = 0;
			m_nMkStrip[1][1] = 0;
			m_nMkStrip[1][2] = 0;
			m_nMkStrip[1][3] = 0;
		}
		break;
	}
}

BOOL CManagerPunch::SaveMk0Img(int nMkPcsIdx) // Cam0
{
	if (!pDoc->WorkingInfo.System.bSaveMkImg)
		return TRUE;

	int nSerial;
	nSerial = m_nBufUpSerial[0]; // Cam0

	CString sSrc, sDest, sPath;
	stModelInfo stInfo;

	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!pDoc->GetPcrInfo(sSrc, stInfo))
	{
		pView->DispStsBar(_T("E(2)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(2)"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir(stInfo))
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-MakeMkDir()"));
		return FALSE;
	}

	//sDest.Format(_T("%s%s\\%s\\%s\\Punching\\%04d"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
	//	stInfo.sLot, stInfo.sLayer, nSerial);

	//if (!pDoc->DirectoryExists(sDest))
	//	CreateDirectory(sDest, NULL);
	//sPath.Format(_T("%s\\%d.tif"), sDest, ++m_nSaveMk0Img);

	sDest.Format(_T("%s%s\\%s\\%s\\Punching"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
		stInfo.sLot, stInfo.sLayer);

	if (!pDoc->DirectoryExists(sDest))
		CreateDirectory(sDest, NULL);

	if (GetTotDefPcs0(nSerial) > 0)
	{
		sPath.Format(_T("%s\\%s.tif"), sDest, GetMkInfo0(nSerial, nMkPcsIdx));

#ifdef USE_VISION
		if (m_pVision[0])
			return m_pVision[0]->SaveMkImg(sPath);
#endif
	}
	else
		return TRUE;

	return FALSE;
}

BOOL CManagerPunch::SaveMk1Img(int nMkPcsIdx) // Cam1
{
	if (!pDoc->WorkingInfo.System.bSaveMkImg)
		return TRUE;

	int nSerial;
	nSerial = m_nBufUpSerial[1]; // Cam1

	CString sSrc, sDest, sPath;
	stModelInfo stInfo;

	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!pDoc->GetPcrInfo(sSrc, stInfo))
	{
		pView->DispStsBar(_T("E(2)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(2)"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir(stInfo))
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-MakeMkDir()"));
		return FALSE;
	}

	//sDest.Format(_T("%s%s\\%s\\%s\\Punching\\%04d"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
	//	stInfo.sLot, stInfo.sLayer, nSerial);

	//if (!pDoc->DirectoryExists(sDest))
	//	CreateDirectory(sDest, NULL);

	//sPath.Format(_T("%s\\%d.tif"), sDest, ++m_nSaveMk1Img);

	sDest.Format(_T("%s%s\\%s\\%s\\Punching"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
		stInfo.sLot, stInfo.sLayer);

	if (!pDoc->DirectoryExists(sDest))
		CreateDirectory(sDest, NULL);

	if (GetTotDefPcs1(nSerial) > 0) // Cam1
	{
		sPath.Format(_T("%s\\%s.tif"), sDest, GetMkInfo1(nSerial, nMkPcsIdx));

#ifdef USE_VISION
		if (m_pVision[1])
			return m_pVision[1]->SaveMkImg(sPath);
#endif
	}
	else
		return TRUE;

	return FALSE;
}



