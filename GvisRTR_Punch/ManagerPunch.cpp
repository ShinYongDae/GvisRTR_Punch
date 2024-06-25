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

