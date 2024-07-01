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

	Reset();
	Init();

	if (!Create())
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("CManagerProcedure::Create() Failed!!!"));
	}
}

CManagerProcedure::~CManagerProcedure()
{
	m_bTIM_INIT_PROCEDURE = FALSE;

	if (m_pEngrave)
	{
		m_pEngrave->Close();
		delete m_pEngrave;
		m_pEngrave = NULL;
	}
}

BEGIN_MESSAGE_MAP(CManagerProcedure, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerProcedure::Create()
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

void CManagerProcedure::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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

BOOL CManagerProcedure::Init()
{
	return TRUE;
}

void CManagerProcedure::Reset()
{
	int i = 0;

	m_bMkSt = FALSE;
	m_bMkStSw = FALSE;
	m_bLotEnd = FALSE;
	m_bLastProc = FALSE;
	m_nMkStAuto = 0;
	m_nLotEndAuto = 0;
	m_nLastProcAuto = 0;
	m_Flag = 0L;
	m_bAoiTestF[0] = FALSE;
	m_bAoiTestF[1] = FALSE;
	m_bAoiFdWriteF[0] = FALSE;
	m_bAoiFdWriteF[1] = FALSE;
	m_bEngTestF = FALSE;
	m_bEngFdWriteF = FALSE;
	m_bCycleStopF = FALSE;
	m_sFixMsg[0] = _T("");
	m_sFixMsg[1] = _T("");

	for (i = 0; i < 10; i++)
	{
		m_bDispMsgDoAuto[i] = FALSE;
		m_nStepDispMsg[i] = 0;
	}
}

BOOL CManagerProcedure::InitAct()
{
#ifdef USE_TCPIP
	if (!m_pEngrave)
	{
		m_pEngrave = new CEngrave(pDoc->WorkingInfo.System.sIpClient[ID_PUNCH], pDoc->WorkingInfo.System.sIpServer[ID_ENGRAVE], pDoc->WorkingInfo.System.sPort[ID_ENGRAVE], this);
		m_pEngrave->SetHwnd(this->GetSafeHwnd());
	}
#endif
	return TRUE;
}

void CManagerProcedure::SetCurrentInfoBufUpTot(int nTotal)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), nTotal);
	::WritePrivateProfileString(_T("Up"), _T("Total"), sData, sPath);
}

void CManagerProcedure::SetCurrentInfoBufUp(int nIdx, int nData)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sIdx, sData;

	if (sPath.IsEmpty())
		return;

	sIdx.Format(_T("%d"), nIdx);
	sData.Format(_T("%d"), nData);

	::WritePrivateProfileString(_T("Up"), sIdx, sData, sPath);
}

void CManagerProcedure::SetCurrentInfoBufDnTot(int nTotal)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), nTotal);
	::WritePrivateProfileString(_T("Dn"), _T("Total"), sData, sPath);
}

void CManagerProcedure::SetCurrentInfoBufDn(int nIdx, int nData)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sIdx, sData;

	if (sPath.IsEmpty())
		return;

	sIdx.Format(_T("%d"), nIdx);
	sData.Format(_T("%d"), nData);

	::WritePrivateProfileString(_T("Dn"), sIdx, sData, sPath);
}

int CManagerProcedure::GetAoiUpCamMstInfo() // AOI상 strpcs.bin 연결
{
	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[0].MasterInfo;

	TCHAR szData[200];
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiUpVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerUp, pDoc->WorkingInfo.LastJob.sLotUp);

	if (0 < ::GetPrivateProfileString(_T("Region"), _T("Piece Region Type"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.nOutFileOnAoi = _ttoi(szData);
	else
		MasterInfo.nOutFileOnAoi = -1;

	return MasterInfo.nOutFileOnAoi;
}

int CManagerProcedure::GetAoiDnCamMstInfo() // AOI하 strpcs.bin 연결
{
	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[1].MasterInfo;

	TCHAR szData[200];
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiDnVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerDn, pDoc->WorkingInfo.LastJob.sLotUp);

	if (0 < ::GetPrivateProfileString(_T("Region"), _T("Piece Region Type"), NULL, szData, sizeof(szData), sPath))
		MasterInfo.nOutFileOnAoi = _ttoi(szData);
	else
		MasterInfo.nOutFileOnAoi = -1;

	return MasterInfo.nOutFileOnAoi;
}

void CManagerProcedure::SetMkMenu01(CString sMenu, CString sItem, CString sData)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkMenu01;

	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CManagerProcedure::SetMkMenu03(CString sMenu, CString sItem, BOOL bOn)
{
	CString sPath = pDoc->WorkingInfo.System.sPathMkMenu03;
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), bOn > 0 ? 1 : 0);
	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

BOOL CManagerProcedure::IsRun()
{
	return pView->IsRun();
	//return TRUE; // AlignTest
	//if (m_sDispMain == _T("운전중") || m_sDispMain == _T("초기운전") || m_sDispMain == _T("단면샘플")
	//	|| m_sDispMain == _T("단면검사") || m_sDispMain == _T("내층검사") || m_sDispMain == _T("외층검사")
	//	|| m_sDispMain == _T("양면검사") || m_sDispMain == _T("양면샘플"))
	//	return TRUE;
	//return FALSE;
	//return m_bSwRun;
}

void CManagerProcedure::InitAuto(BOOL bInit)
{
	if (!pDoc->WorkingInfo.LastJob.bSampleTest)
	{
		::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
		::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	}

	SetAoiUpAlarmRestartMsg(GetAoiUpAlarmRestartMsg());		// m_sAoiUpAlarmReStartMsg
	SetAoiDnAlarmRestartMsg(GetAoiDnAlarmRestartMsg());		// m_sAoiDnAlarmReStartMsg
	SetAoiUpAlarmReTestMsg(GetAoiUpAlarmReTestMsg());		// m_sAoiUpAlarmReTestMsg
	SetAoiDnAlarmReTestMsg(GetAoiDnAlarmReTestMsg());		// m_sAoiDnAlarmReTestMsg

	SetAoiUpAutoStep(0);
	SetAoiDnAutoStep(0);

	m_nAoiUpAutoSerial = 0;
	m_nAoiUpAutoSerialPrev = 0;
	m_nAoiDnAutoSerial = 0;
	m_nAoiDnAutoSerialPrev = 0;

	pView->m_nDebugStep = 10; pView->DispThreadTick();
	int nCam, nPoint, kk, a, b;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	for (kk = 0; kk < 10; kk++)
	{
		m_bDispMsgDoAuto[kk] = FALSE;
		m_nStepDispMsg[kk] = 0;
	}
	m_sFixMsg[0] = _T("");
	m_sFixMsg[1] = _T("");

	m_bReadyDone = FALSE;
	m_bChkLastProcVs = FALSE;
	m_nDummy[0] = 0;
	m_nDummy[1] = 0;
	m_nAoiLastSerial[0] = 0;
	m_nAoiLastSerial[1] = 0;
	General.nStepAuto = 0;
	m_nPrevStepAuto = 0;
	m_nPrevMkStAuto = 0;
	m_bAoiLdRun = TRUE;
	m_bAoiLdRunF = FALSE;
	m_bNewModel = FALSE;
	m_nLotEndSerial = 0;
	if (m_pDlgMenu01)
		m_pDlgMenu01->DispLotEndSerial(0);
	m_bCam = FALSE;
	m_bReview = FALSE;
	m_bChkBufIdx[0] = TRUE;
	m_bChkBufIdx[0] = TRUE;

	m_nErrCnt = 0;

	m_nStepMk[0] = 0;
	m_nStepMk[1] = 0;
	m_nStepMk[2] = 0;
	m_nStepMk[3] = 0;
	m_bTHREAD_MK[0] = FALSE;
	m_bTHREAD_MK[1] = FALSE;
	m_bTHREAD_MK[2] = FALSE;
	m_bTHREAD_MK[3] = FALSE;
	m_nMkPcs[0] = 0;
	m_nMkPcs[1] = 0;
	m_nMkPcs[2] = 0;
	m_nMkPcs[3] = 0;

	m_bMkTmpStop = FALSE;

	m_bWaitPcr[0] = FALSE;
	m_bWaitPcr[1] = FALSE;


	m_nShareUpS = 0;
	m_nShareUpSerial[0] = 0;
	m_nShareUpSerial[1] = 0;
	m_nShareUpCnt = 0;

	m_nShareDnS = 0;
	m_nShareDnSerial[0] = 0;
	m_nShareDnSerial[1] = 0;
	m_nShareDnCnt = 0;

	m_nBufUpSerial[0] = 0;
	m_nBufUpSerial[1] = 0;
	m_nBufUpCnt = 0;

	m_nBufDnSerial[0] = 0;
	m_nBufDnSerial[1] = 0;
	m_nBufDnCnt = 0;

	for (nCam = 0; nCam < 2; nCam++)
	{
		for (nPoint = 0; nPoint < 4; nPoint++)
		{
			m_pDlgMenu02->m_dMkFdOffsetX[nCam][nPoint] = 0.0;
			m_pDlgMenu02->m_dMkFdOffsetY[nCam][nPoint] = 0.0;
		}
	}


	m_pDlgMenu02->m_dAoiUpFdOffsetX = 0.0;
	m_pDlgMenu02->m_dAoiUpFdOffsetY = 0.0;
	m_pDlgMenu02->m_dAoiDnFdOffsetX = 0.0;
	m_pDlgMenu02->m_dAoiDnFdOffsetY = 0.0;

	m_bReAlign[0][0] = FALSE;	// [nCam][nPos]
	m_bReAlign[0][1] = FALSE;	// [nCam][nPos]
	m_bReAlign[0][2] = FALSE;	// [nCam][nPos]
	m_bReAlign[0][3] = FALSE;	// [nCam][nPos]
	m_bReAlign[1][0] = FALSE;	// [nCam][nPos]
	m_bReAlign[1][1] = FALSE;	// [nCam][nPos]
	m_bReAlign[1][2] = FALSE;	// [nCam][nPos]
	m_bReAlign[1][3] = FALSE;	// [nCam][nPos]

	m_bSkipAlign[0][0] = FALSE;	// [nCam][nPos]
	m_bSkipAlign[0][1] = FALSE;	// [nCam][nPos]
	m_bSkipAlign[0][2] = FALSE;	// [nCam][nPos]
	m_bSkipAlign[0][3] = FALSE;	// [nCam][nPos]
	m_bSkipAlign[1][0] = FALSE;	// [nCam][nPos]
	m_bSkipAlign[1][1] = FALSE;	// [nCam][nPos]
	m_bSkipAlign[1][2] = FALSE;	// [nCam][nPos]
	m_bSkipAlign[1][3] = FALSE;	// [nCam][nPos]

	m_bFailAlign[0][0] = FALSE;	// [nCam][nPos]
	m_bFailAlign[0][1] = FALSE;	// [nCam][nPos]
	m_bFailAlign[0][2] = FALSE;	// [nCam][nPos]
	m_bFailAlign[0][3] = FALSE;	// [nCam][nPos]
	m_bFailAlign[1][0] = FALSE;	// [nCam][nPos]
	m_bFailAlign[1][1] = FALSE;	// [nCam][nPos]
	m_bFailAlign[1][2] = FALSE;	// [nCam][nPos]
	m_bFailAlign[1][3] = FALSE;	// [nCam][nPos]

	m_bDoMk[0] = TRUE;			// [nCam]
	m_bDoMk[1] = TRUE;			// [nCam]
	m_bDoneMk[0] = FALSE;		// [nCam]
	m_bDoneMk[1] = FALSE;		// [nCam]
	m_bReMark[0] = FALSE;		// [nCam]
	m_bReMark[1] = FALSE;		// [nCam]

	m_nTotMk[0] = 0;
	m_nCurMk[0] = 0;
	m_nTotMk[1] = 0;
	m_nCurMk[1] = 0;
	m_nPrevTotMk[0] = 0;
	m_nPrevCurMk[0] = 0;
	m_nPrevTotMk[1] = 0;
	m_nPrevCurMk[1] = 0;


	m_bMkSt = FALSE;
	::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
	m_bMkStSw = FALSE;
	m_nMkStAuto = 0;

	m_bEngSt = FALSE;
	m_bEngStSw = FALSE;
	m_nEngStAuto = 0;

	m_bEng2dSt = FALSE;
	m_bEng2dStSw = FALSE;
	m_nEng2dStAuto = 0;

	m_bLotEnd = FALSE;
	m_nLotEndAuto = 0;

	m_bLastProc = FALSE;
	if (MODE_INNER != pDoc->GetTestMode())
		m_bLastProcFromUp = TRUE;
	else
		m_bLastProcFromEng = TRUE;

	m_nLastProcAuto = 0;

	pView->ClrAlarm();

	m_dwCycSt = 0;
	m_sNewLotUp = _T("");
	m_sNewLotDn = _T("");

	m_nStop = 0;

	m_nStepTHREAD_DISP_DEF = 0;
	m_bTHREAD_DISP_DEF = FALSE;				// CopyDefImg Stop
	m_nStepTHREAD_DISP_DEF_INNER = 0;
	m_bTHREAD_DISP_DEF_INNER = FALSE;		// DispDefImg Stop

	pDoc->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("0"));

	pView->m_nDebugStep = 11; pView->DispThreadTick();
	for (a = 0; a < 2; a++)
	{
		for (b = 0; b < 4; b++)
		{
			m_nMkStrip[a][b] = 0;
			m_bRejectDone[a][b] = FALSE;
		}
	}

	pView->m_nDebugStep = 12; pView->DispThreadTick();
	MpeWrite(_T("MB440100"), 0); // PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
	MpeWrite(_T("MB440110"), 0); // 마킹시작(PC가 확인하고 Reset시킴.)-20141029
	MpeWrite(_T("MB440150"), 0); // 마킹부 마킹중 ON (PC가 ON, OFF)
	MpeWrite(_T("MB440170"), 0); // 마킹완료(PLC가 확인하고 Reset시킴.)-20141029

	InitAutoEngSignal();

	pView->m_nDebugStep = 13; pView->DispThreadTick();
	MoveInitPos1();
	Sleep(30);
	MoveInitPos0();

	pView->m_nDebugStep = 14; pView->DispThreadTick();
	InitIoWrite();
	//pView->m_nDebugStep = 15; pView->DispThreadTick();
	//OpenShareUp();
	//pView->m_nDebugStep = 16; pView->DispThreadTick();
	//OpenShareDn();
	pView->m_nDebugStep = 17; pView->DispThreadTick();
	SetTest(FALSE);	// 검사부 상/하 검사 시작 (Off)
	pView->m_nDebugStep = 18; pView->DispThreadTick();
	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->m_bLastProc = FALSE;

		if (MODE_INNER != pDoc->GetTestMode())
			m_pDlgMenu01->m_bLastProcFromUp = TRUE;
		else
			m_pDlgMenu01->m_bLastProcFromEng = TRUE;

		m_pDlgMenu01->ResetSerial();
		m_pDlgMenu01->ResetLastProc();
	}


	m_bTHREAD_UPDATAE_YIELD[0] = FALSE;
	m_bTHREAD_UPDATAE_YIELD[1] = FALSE;
	m_nSerialTHREAD_UPDATAE_YIELD[0] = 0;
	m_nSerialTHREAD_UPDATAE_YIELD[1] = 0;

	m_bTHREAD_UPDATE_REELMAP_UP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLUP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_DN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLDN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ITS = FALSE;
	m_bTHREAD_MAKE_ITS_FILE_UP = FALSE;
	m_bTHREAD_MAKE_ITS_FILE_DN = FALSE;
	//m_bTHREAD_UPDATE_REELMAP_INOUTER_UP = FALSE;
	//m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE;
	//m_bTHREAD_UPDATE_REELMAP_INOUTER_DN = FALSE;
	//m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE;

	m_bTHREAD_REELMAP_YIELD_UP = FALSE;
	m_bTHREAD_REELMAP_YIELD_ALLUP = FALSE;
	m_bTHREAD_REELMAP_YIELD_DN = FALSE;
	m_bTHREAD_REELMAP_YIELD_ALLDN = FALSE;
	m_bTHREAD_REELMAP_YIELD_ITS = FALSE;

	m_nSnTHREAD_UPDATAE_YIELD = 0;

	pDoc->m_nEjectBufferLastShot = -1;
	m_bSerialDecrese = FALSE;
	m_bStopF_Verify = TRUE;
	m_bInitAuto = TRUE;
	m_bInitAutoLoadMstInfo = TRUE;

	if (bInit) // 이어가기가 아닌경우.
	{
		MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
		DispLotEnd(FALSE);

		m_nRstNum = 0;
		m_bCont = FALSE;
		m_dTotVel = 0.0;
		m_dPartVel = 0.0;
		m_dwCycSt = 0;
		m_dwCycTim = 0;

		pDoc->m_nPrevSerial = 0;
		pDoc->m_bNewLotShare[0] = FALSE;
		pDoc->m_bNewLotShare[1] = FALSE;
		pDoc->m_bNewLotBuf[0] = FALSE;
		pDoc->m_bNewLotBuf[1] = FALSE;
		pDoc->m_bDoneChgLot = FALSE;

		m_pDlgFrameHigh->m_nMkLastShot = 0;
		m_pDlgFrameHigh->m_nAoiLastShot[0] = 0;
		m_pDlgFrameHigh->m_nAoiLastShot[1] = 0;

		pView->m_nDebugStep = 20; pView->DispThreadTick();
		if (m_pDlgMenu01)
			m_pDlgMenu01->ResetLotTime();

		//ClrMkInfo();
		pView->m_nDebugStep = 21; pView->DispThreadTick();

		//ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
		//pView->m_nDebugStep = 22; pView->DispThreadTick();
		//if (bDualTest)
		//	ResetMkInfo(1);

		pView->m_nDebugStep = 23; pView->DispThreadTick();
		ClrMkInfo(); // 20220420 - Happen Release Trouble

		pView->m_nDebugStep = 24; pView->DispThreadTick();
		if (m_pDlgFrameHigh)
		{
			m_pDlgFrameHigh->SetMkLastShot(0);
			m_pDlgFrameHigh->SetAoiLastShot(0, 0);
			m_pDlgFrameHigh->SetAoiLastShot(1, 0);
			m_pDlgFrameHigh->SetEngraveLastShot(0);
		}
		pView->m_nDebugStep = 25; pView->DispThreadTick();

		//pDoc->m_ListBuf[0].Clear();
		//pDoc->m_ListBuf[1].Clear();
	}
	else
	{
		pView->m_nDebugStep = 26; pView->DispThreadTick();
		//if (pDoc->GetTestMode() == MODE_INNER && pDoc->WorkingInfo.LastJob.bDispLotEnd)
		if (pDoc->GetTestMode() == MODE_INNER && pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == pView->MsgBox(_T("각인부에서 레이저 각인부터 시작하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
				else
				{
					MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
			}
		}
		else if (pDoc->GetTestMode() == MODE_INNER && !pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == pView->MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("각인부에서 레이저 각인부터 시작하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
				else
				{
					MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
			}

		}
		else
		{
			MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
		}
		//DispStsBar("이어가기");

		pView->m_nDebugStep = 27; pView->DispThreadTick();
		if (pDoc->m_pReelMap)
			pDoc->m_pReelMap->ClrFixPcs();
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->ClrFixPcs();

		if (bDualTest)
		{
			if (pDoc->m_pReelMapDn)
				pDoc->m_pReelMapDn->ClrFixPcs();
			if (pDoc->m_pReelMapAllUp)
				pDoc->m_pReelMapAllUp->ClrFixPcs();
			if (pDoc->m_pReelMapAllDn)
				pDoc->m_pReelMapAllDn->ClrFixPcs();
		}

		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (pDoc->m_pReelMapInnerUp)
				pDoc->m_pReelMapInnerUp->ClrFixPcs();

			if (pDoc->WorkingInfo.LastJob.bDualTestInner)
			{
				if (pDoc->m_pReelMapInnerDn)
					pDoc->m_pReelMapInnerDn->ClrFixPcs();
				if (pDoc->m_pReelMapInnerAllUp)
					pDoc->m_pReelMapInnerAllUp->ClrFixPcs();
				if (pDoc->m_pReelMapInnerAllDn)
					pDoc->m_pReelMapInnerAllDn->ClrFixPcs();
			}
		}

#ifndef TEST_MODE
		ReloadReelmap();
		UpdateRst();
#endif
		DispLotStTime();
		pView->m_nDebugStep = 29; pView->DispThreadTick();
		RestoreReelmap();
	}

	pView->m_nDebugStep = 30; pView->DispThreadTick();
}

void CManagerProcedure::Auto()
{
	DoAuto();
}

void CManagerProcedure::DoAuto()
{
	if (!pView->m_mgrStatus)
		return;

	stThread& Thread = (pView->m_mgrStatus->Thread);

	CString str;
	str.Format(_T("%d : %d"), Thread.nStepTHREAD_DISP_DEF, Thread.bTHREAD_DISP_DEF ? 1 : 0);
	pView->DispStsBar(str, 6);

	// LotEnd Start
	if (DoAutoGetLotEndSignal())
	{
		return;
	}

	// 마킹시작 신호를 확인
	DoAtuoGetMkStSignal();

	// LastProc Start
	DoAutoSetLastProcAtPlc();

	// AOI Feeding Offset Start on LastProc
	DoAutoSetFdOffsetLastProc();

	// AOI Feeding Offset Start
	DoAutoSetFdOffset();

	// Engrave Feeding Offset Start
	DoAutoSetFdOffsetEngrave();

	// CycleStop
	DoAutoChkCycleStop();

	// DispMsg
	DoAutoDispMsg();

	// Check Share Folder Start
	if (pDoc->GetAoiUpVsStatus())
		DoAutoChkShareVsFolder();
	else
		DoAutoChkShareFolder();

	// Marking Start
	DoAutoMarking();

	// Engrave Marking Start
	DoAutoMarkingEngrave();
}

BOOL CManagerProcedure::DoAutoGetLotEndSignal()
{
	if (!pView->m_mgrStatus)
		return FALSE;

	stGeneral& General = (pView->m_mgrStatus->General);
	stThread& Thread = (pView->m_mgrStatus->Thread);

	int nSerial;

	if(General.bLotEnd && General.nStepAuto < LOT_END)
	{
		m_bLotEnd = TRUE;
		m_nLotEndAuto = LOT_END;
	}

	if (!pView->IsBuffer(0) && m_bLastProc && m_nLotEndAuto < LOT_END)
	{
		m_bLotEnd = TRUE;
		m_nLotEndAuto = LOT_END;
	}
	else if (!pView->IsBuffer(0) && m_nMkStAuto > MK_ST + (Mk2PtIdx::DoneMk) + 4)
	{
		m_nMkStAuto = 0;
		m_bLotEnd = TRUE;
		m_nLotEndAuto = LOT_END;
	}


	if (m_bLotEnd)
	{
		nSerial = pDoc->GetLastShotMk();

		switch (m_nLotEndAuto)
		{
		case LOT_END:
			if (!Thread.bTHREAD_REELMAP_YIELD_UP && !Thread.bTHREAD_REELMAP_YIELD_DN && !Thread.bTHREAD_REELMAP_YIELD_ALLUP && !Thread.bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				ReloadReelmap(nSerial);
				UpdateRst();
				m_nLotEndAuto++;
			}
			break;
		case LOT_END + 1:
			pView->MpeWrite(_T("MB440180"), 1);			// 작업종료(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
			pView->DispMain(_T("작업종료"), RGB_RED);
			m_nLotEndAuto++;
			break;
		case LOT_END + 2:
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			LotEnd();									// MakeResultMDS
			m_nLotEndAuto++;
			break;

		case LOT_END + 3:
			pView->MsgBox(_T("작업이 종료되었습니다."));
			m_nLotEndAuto++;
			m_bLastProc = FALSE;
			m_bMkSt = FALSE;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case LOT_END + 4:
			break;
		}
	}

	return m_bLotEnd;
}

BOOL CManagerProcedure::DoAtuoGetMkStSignal()
{
	if (!m_bMkSt)
	{
		if (IsRun())
		{
			if (pView->GetMkStSignal() || m_bMkStSw) // AlignTest		// 마킹시작(PC가 확인하고 Reset시킴.)-20141029
			{
				pDoc->LogAuto(_T("PLC: 마킹시작(PC가 확인하고 Reset시킴.)"));
				m_bMkStSw = FALSE;
				pView->ResetMkStSignal();

				if (pView->GetMpeSignal(0, 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
				{
					pDoc->LogAuto(_T("PLC: Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
					pView->MpeWrite(_T("MB440101"), 0);		// 마킹부 Feeding완료
				}

				m_bMkSt = TRUE;
				m_nMkStAuto = MK_ST;
				::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("1"), PATH_WORKING_INFO);

				if (!pView->GetLastShotMk())
					pView->SetLotSt();		// 새로운 로트의 시작시간을 설정함. // 전체속도의 처음 시작되는 시간 설정.

				return TRUE;
			}
		}
	}
	return FALSE;
}

void CManagerProcedure::DoAutoSetLastProcAtPlc()
{
	if (m_bLastProc)
	{
		switch (m_nLastProcAuto)
		{
		case LAST_PROC:	// 잔량처리 1
			if (IsRun())
			{
				if (MODE_INNER != pDoc->GetTestMode())
				{
					if (ChkLastProcFromUp())
					{
						pDoc->LogAuto(_T("PC: 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
						pView->MpeWrite(_T("MB440185"), 1);			// 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
						Sleep(300);
						pView->MpeWrite(_T("MB44012B"), 1);			// AOI 상 : PCR파일 Received
					}
					else
					{
						pDoc->LogAuto(_T("PC: 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
						pView->MpeWrite(_T("MB440186"), 1);			// 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
					}
				}
				else
				{
					if (ChkLastProcFromEng())
					{
						pDoc->LogAuto(_T("PC: 잔량처리 각인부 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
						pView->MpeWrite(_T("MB44019D"), 1);			// 잔량처리 각인부 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
					}
					else if (ChkLastProcFromUp())
					{
						pDoc->LogAuto(_T("PC: 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
						pView->MpeWrite(_T("MB440185"), 1);			// 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
						Sleep(300);
						pView->MpeWrite(_T("MB44012B"), 1);			// AOI 상 : PCR파일 Received
					}
					else
					{
						pDoc->LogAuto(_T("PC: 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
						pView->MpeWrite(_T("MB440186"), 1);			// 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
					}
				}

				m_nLastProcAuto++;
			}
			break;
		case LAST_PROC + 1:
			pDoc->LogAuto(_T("PC: 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)"));
			pView->MpeWrite(_T("MB440181"), 1);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
			m_nLastProcAuto++;
			break;
		case LAST_PROC + 2:
			;
			break;
		}
	}
}

void CManagerProcedure::DoAutoSetFdOffsetLastProc()
{
	if (!pView->m_mgrFeeding || !pView->m_mgrStatus)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bOn0 = pView->m_mgrFeeding->IsLoaderOnAoiUp();		// 검사부 상 자동 운전 <-> X432B I/F
	BOOL bOn1 = pView->m_mgrFeeding->IsLoaderOnAoiDn();		// 검사부 하 자동 운전 <-> X442B I/F

	if (bOn0 && !(m_Flag & (0x01 << 2)))
	{
		m_Flag |= (0x01 << 2);
	}
	else if (!bOn0 && (m_Flag & (0x01 << 2)))
	{
		m_Flag &= ~(0x01 << 2);

		General.bAoiTest[0] = FALSE;
		General.bWaitPcr[0] = FALSE;
		m_bAoiTestF[0] = FALSE;
		m_bAoiFdWriteF[0] = FALSE;
		pView->MpeWrite(_T("MB440111"), 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
		pDoc->LogAuto(_T("PLC: 검사부(상) Feeding Offset Write 완료(PC가 확인하고 MB440111 Reset시킴.)"));
	}

	if (bOn1 && !(m_Flag & (0x01 << 3)))
	{
		m_Flag |= (0x01 << 3);
	}
	else if (!bOn1 && (m_Flag & (0x01 << 3)))
	{
		m_Flag &= ~(0x01 << 3);

		General.bAoiTest[1] = FALSE;
		General.bWaitPcr[1] = FALSE;
		m_bAoiTestF[1] = FALSE;
		m_bAoiFdWriteF[1] = FALSE;
		pView->MpeWrite(_T("MB440112"), 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
		pDoc->LogAuto(_T("PLC: 검사부(하) Feeding Offset Write 완료(PC가 확인하고 MB440112 Reset시킴.)"));
	}
}

void CManagerProcedure::DoAutoSetFdOffset()
{
	if (!pView->m_mgrFeeding || !pView->m_mgrStatus)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	double dAveX, dAveY;
	CfPoint OfStUp, OfStDn;

	BOOL bOn0 = pView->m_mgrFeeding->IsTestingAoiUp();
	BOOL bOn1 = pView->m_mgrFeeding->IsTestingAoiDn();

	if (bOn0 && !m_bAoiTestF[0])		// 검사부(상) 검사중
	{
		m_bAoiTestF[0] = TRUE;
		General.bAoiTest[0] = TRUE;
		General.bWaitPcr[0] = TRUE;
	}
	else if (!bOn0 && m_bAoiTestF[0])
	{
		m_bAoiTestF[0] = FALSE;
		m_bAoiFdWriteF[0] = FALSE;
		General.bAoiTest[0] = FALSE;

		pView->MpeWrite(_T("MB440111"), 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
		pDoc->LogAuto(_T("PLC: 검사부(상) Feeding Offset Write 완료(PC가 확인하고 MB440111 Reset시킴.)"));
	}

	if (bOn1 && !m_bAoiTestF[1])		// 검사부(하) 검사중
	{
		m_bAoiTestF[1] = TRUE;
		General.bAoiTest[1] = TRUE;
		General.bWaitPcr[1] = TRUE;
	}
	else if (!bOn1 && m_bAoiTestF[1])
	{
		m_bAoiTestF[1] = FALSE;
		m_bAoiFdWriteF[1] = FALSE;
		General.bAoiTest[1] = FALSE;
		pView->MpeWrite(_T("MB440112"), 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
		pDoc->LogAuto(_T("PLC: 검사부(하) Feeding Offset Write 완료(PC가 확인하고 MB440112 Reset시킴.)"));
	}


	BOOL bOn2 = pView->m_mgrFeeding->IsDoneWriteFdOffsetAoiUp();
	BOOL bOn3 = pView->m_mgrFeeding->IsDoneWriteFdOffsetAoiDn();

	if (bOn2 && !General.bAoiFdWrite[0])		// 검사부(상) Feeding Offset Write 완료
		General.bAoiFdWrite[0] = TRUE;
	else if (!bOn2 && General.bAoiFdWrite[0])
		General.bAoiFdWrite[0] = FALSE;

	if (bOn3 && !General.bAoiFdWrite[1])		// 검사부(하) Feeding Offset Write 완료
		General.bAoiFdWrite[1] = TRUE;
	else if (!bOn3 && General.bAoiFdWrite[1])
		General.bAoiFdWrite[1] = FALSE;


	if (bDualTest)
	{
		if ((General.bAoiFdWrite[0] && General.bAoiFdWrite[1]) && (!m_bAoiFdWriteF[0] && !m_bAoiFdWriteF[1]))
		{
			m_bAoiFdWriteF[0] = TRUE;
			m_bAoiFdWriteF[1] = TRUE;

			pDoc->GetAoiUpOffset(OfStUp);
			pDoc->GetAoiDnOffset(OfStDn);

			dAveX = OfStUp.x;
			dAveY = OfStUp.y;

			pView->SetAoiUpOffset(OfStUp);
			pView->SetAoiDnOffset(OfStDn);

			pView->MpeWrite(_T("ML45064"), (long)(-1.0*dAveX*1000.0));
			pView->MpeWrite(_T("MB440111"), 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
			pView->MpeWrite(_T("MB440112"), 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
			pDoc->LogAuto(_T("PLC: 검사부(상MB440111,하MB440112) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
		}
		else if ((!General.bAoiFdWrite[0] && !General.bAoiFdWrite[1]) && (m_bAoiFdWriteF[0] && m_bAoiFdWriteF[1]))
		{
			m_bAoiFdWriteF[0] = FALSE;
			m_bAoiFdWriteF[1] = FALSE;
			General.bAoiTest[0] = FALSE;
			General.bAoiTest[1] = FALSE;
		}

		if (General.bAoiTest[0] && !General.bAoiTest[1])
		{
			if (General.bAoiFdWrite[0] && !m_bAoiFdWriteF[0])
			{
				m_bAoiFdWriteF[0] = TRUE;

				OfStDn.x = 0.0; OfStDn.y = 0.0;
				pDoc->GetAoiUpOffset(OfStUp);
				pView->SetAoiUpOffset(OfStUp);
				pView->SetAoiDnOffset(OfStDn);

				pView->MpeWrite(_T("ML45064"), (long)(-1.0*OfStUp.x*1000.0));
				pView->MpeWrite(_T("MB440111"), 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
				pDoc->LogAuto(_T("PLC: 검사부(상MB440111) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
			}
			else if (!General.bAoiFdWrite[0] && m_bAoiFdWriteF[0])
			{
				m_bAoiFdWriteF[0] = FALSE;
				General.bAoiTest[0] = FALSE;
			}
		}

		if (!General.bAoiTest[0] && General.bAoiTest[1])
		{
			if (General.bAoiFdWrite[1] && !m_bAoiFdWriteF[1])
			{
				m_bAoiFdWriteF[1] = TRUE;

				OfStUp.x = 0.0; OfStUp.y = 0.0;
				pDoc->GetAoiDnOffset(OfStDn);
				pView->SetAoiUpOffset(OfStUp);
				pView->SetAoiDnOffset(OfStDn);

				pView->MpeWrite(_T("ML45064"), (long)(-1.0*OfStDn.x*1000.0));
				pView->MpeWrite(_T("MB440112"), 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
				pDoc->LogAuto(_T("PLC: 검사부(하MB440112) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
			}
			else if (!General.bAoiFdWrite[1] && m_bAoiFdWriteF[1])
			{
				m_bAoiFdWriteF[1] = FALSE;
				General.bAoiTest[1] = FALSE;
			}
		}
	}
	else
	{
		if (General.bAoiFdWrite[0] && !m_bAoiFdWriteF[0])
		{
			m_bAoiFdWriteF[0] = TRUE;

			pDoc->GetAoiUpOffset(OfStUp);
			pView->SetAoiUpOffset(OfStUp);

			dAveX = (OfStUp.x);
			dAveY = (OfStUp.y);

			pView->MpeWrite(_T("ML45064"), (long)(-1.0*dAveX*1000.0));	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
			pView->MpeWrite(_T("MB440111"), 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
			pView->MpeWrite(_T("MB440112"), 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103  // 20160721-syd-temp
			pDoc->LogAuto(_T("PLC: 검사부(상MB440111,하MB440112) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
		}
		else if (!General.bAoiFdWrite[0] && m_bAoiFdWriteF[0])
		{
			m_bAoiFdWriteF[0] = FALSE;
			General.bAoiTest[0] = FALSE;
		}
	}
}

void CManagerProcedure::DoAutoSetFdOffsetEngrave()
{
	if (!pView->m_mgrStatus || !m_pEngrave)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);
	stBtnStatus& BtnStatus = (m_pEngrave->BtnStatus);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	double dAveX, dAveY;
	CfPoint OfSt;

	if ((BtnStatus.EngAuto.IsOnMking && !m_bEngTestF) || (BtnStatus.EngAuto.IsOnRead2d && !m_bEngTestF)) // 각인부 각인중
	{
		m_bEngTestF = TRUE;
		General.bEngTest = TRUE;
	}
	else if ((!BtnStatus.EngAuto.IsOnMking && m_bEngTestF) || (!BtnStatus.EngAuto.IsOnRead2d && m_bEngTestF))
	{
		m_bEngTestF = FALSE;
		General.bEngTest = FALSE;
		m_bEngFdWriteF = FALSE;
		pView->MpeWrite(_T("MB44011A"), 0); // 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
		pDoc->LogAuto(_T("PC: 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
	}

	BOOL bOn = pView->m_mgrFeeding->IsDoneWriteFdOffsetEng();

	if (bOn && !General.bEngFdWrite)		// 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
	{
		General.bEngFdWrite = TRUE;
	}
	else if (!bOn && General.bEngFdWrite)
	{
		General.bEngFdWrite = FALSE;
	}

	if (General.bEngFdWrite && !m_bEngFdWriteF)
	{
		m_bEngFdWriteF = TRUE;

		pDoc->GetEngOffset(OfSt);

		dAveX = OfSt.x;
		dAveY = OfSt.y;

		pView->SetEngOffset(OfSt);

		pDoc->LogAuto(_T("PC: 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
		pView->MpeWrite(_T("ML45078"), (long)(dAveX*1000.0));	// 각인부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
		pView->MpeWrite(_T("MB44011A"), 0);						// 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
		Sleep(10);
	}
	else if (!General.bEngFdWrite && m_bEngFdWriteF)
	{
		m_bEngFdWriteF = FALSE;
		General.bEngTest = FALSE;
	}
}

void CManagerProcedure::DoAutoChkCycleStop()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	CManagerFeeding* pMgrFd = (pView->m_mgrFeeding);

	if (General.bCycleStop && !m_bCycleStopF)
	{
		m_bCycleStopF = TRUE;
		pView->Buzzer(TRUE);
		if (!pView->m_sAlmMsg.IsEmpty())
		{
			pDoc->LogAuto(pView->m_sAlmMsg);
			pView->MsgBox(pView->m_sAlmMsg, 0, 0, DEFAULT_TIME_OUT, FALSE);

			if (pView->m_sAlmMsg == pMgrFd->GetAoiAlarmReStartMsg(0) || pView->m_sAlmMsg == pMgrFd->GetAoiAlarmReTestMsg(0))
			{
				pView->ChkReTestAlarmOnAoiUp();
			}
			else if (pView->m_sAlmMsg == pMgrFd->GetAoiAlarmReStartMsg(1) || pView->m_sAlmMsg == pMgrFd->GetAoiAlarmReTestMsg(1))
			{
				pView->ChkReTestAlarmOnAoiDn();
			}
		}
		pView->m_sAlmMsg = _T("");
		pView->m_sIsAlmMsg = _T("");
		pView->m_sPrevAlmMsg = _T("");
	}
	else if (!General.bCycleStop && m_bCycleStopF)
	{
		m_bCycleStopF = FALSE;
	}
}

void CManagerProcedure::DoAutoDispMsg()
{
	BOOL bDispMsg = FALSE;
	int idx, nStepDispMsg;

	// [2] : 고정불량-상, [3] : 고정불량-하, [5] : nSerialL <= 0, [6] : CopyDefImg 우측 Camera,  
	// [7] : CopyDefImg 좌측 Camera, [8] : 보이스코일(좌) 초기위치 이동, [9] : 보이스코일(우) 초기위치 이동
	for (idx = 0; idx < 10; idx++)
	{
		if (m_bDispMsgDoAuto[idx])
		{
			bDispMsg = TRUE;
			nStepDispMsg = m_nStepDispMsg[idx];
			break;
		}
	}
	if (bDispMsg && IsRun())
	{
		switch (nStepDispMsg)
		{
		case 0:
			break;
		case FROM_DOMARK0:
			m_bDispMsgDoAuto[8] = FALSE;
			m_nStepDispMsg[8] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DOMARK1:
			m_bDispMsgDoAuto[9] = FALSE;
			m_nStepDispMsg[9] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			pView->DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DISPDEFIMG:
			m_bDispMsgDoAuto[0] = FALSE;
			m_nStepDispMsg[0] = 0;
			pView->Stop();
			pView->MsgBox(_T("버퍼(우) Serial이 맞지않습니다."));
			break;
		case FROM_DISPDEFIMG + 1:
			m_bDispMsgDoAuto[1] = FALSE;
			m_nStepDispMsg[1] = 0;
			pView->Stop();
			pView->MsgBox(_T("버퍼(우) Serial이 맞지않습니다."));
			break;
		case FROM_DISPDEFIMG + 2: // IsFixUp
			m_bDispMsgDoAuto[2] = FALSE;
			m_nStepDispMsg[2] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(m_sFixMsg[0]);
			m_sFixMsg[0] = _T("");
			break;
		case FROM_DISPDEFIMG + 3: // IsFixDn
			m_bDispMsgDoAuto[3] = FALSE;
			m_nStepDispMsg[3] = 0;
			pView->Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			pView->DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(m_sFixMsg[1]);
			m_sFixMsg[1] = _T("");
			break;
		case FROM_DISPDEFIMG + 4:
			//m_bDispMsgDoAuto[4] = FALSE;
			//m_nStepDispMsg[4] = 0;
			//Stop();
			//TowerLamp(RGB_RED, TRUE);
			//Buzzer(TRUE, 0);
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			//pView->DispStsBar(_T("정지-41"), 0);
			//DispMain(_T("정 지"), RGB_RED);	
			//MsgBox(m_sFixMsg);
			//m_sFixMsg = _T("");
			break;
		case FROM_DISPDEFIMG + 5:
			m_bDispMsgDoAuto[5] = FALSE;
			m_nStepDispMsg[5] = 0;
			pView->Stop();
			pView->MsgBox(_T("버퍼(좌) Serial이 맞지않습니다."));
			break;
		case FROM_DISPDEFIMG + 6:
			m_bDispMsgDoAuto[6] = FALSE;
			m_nStepDispMsg[6] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			pView->DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DISPDEFIMG + 7:
			m_bDispMsgDoAuto[7] = FALSE;
			m_nStepDispMsg[7] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			pView->DispMain(_T("정 지"), RGB_RED);
			break;
		}
	}
}

void CManagerProcedure::DoAutoChkShareVsFolder()	// 잔량처리 시 계속적으로 반복해서 이함수가 호출됨으로 좌우 마킹 인덱스 동일 현상 발생.(case AT_LP + 8:)
{
	if (!pView->m_mgrStatus)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);
	stBtnPush& BtnPush = (pView->m_mgrFeeding->Btn);

	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = 0;
	CString sNewLot;
	int nNewLot = 0;
	BOOL bNewModel = FALSE;

	switch (General.nStepAuto)
	{
	case 0:
		BtnPush.bSwRun = FALSE;
		BtnPush.bSwStop = TRUE;
		General.nStepAuto++;
		break;
	case 1:
		if (IsReady() || IsAuto())		// 운전준비
		{
			TowerLamp(RGB_YELLOW, TRUE, TRUE);
			General.nStepAuto++;
		}
		break;
	case 2:
		if (IsRun())
			General.nStepAuto++;
		break;
	case 3:
		ClrDispMsg();
		TowerLamp(RGB_YELLOW, TRUE, FALSE);
		General.nStepAuto++;
		break;
	case 4:
		if (IsRun())		// 초기운전
		{
			if (!IsAoiLdRun())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
			else
			{
				ResetWinker(); // 20151126 : 운전스위치 램프 동작 문제로 수정.

				TowerLamp(RGB_GREEN, TRUE, TRUE);
				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (pDoc->WorkingInfo.LastJob.bDualTest)
					{
						if (m_sDispMain != _T("양면샘플"))
							DispMain(_T("양면샘플"), RGB_GREEN);
					}
					else
					{
						if (m_sDispMain != _T("단면샘플"))
							DispMain(_T("단면샘플"), RGB_GREEN);
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (m_sDispMain != _T("내층검사"))
						DispMain(_T("내층검사"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_sDispMain != _T("외층검사"))
						DispMain(_T("외층검사"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (m_sDispMain != _T("양면검사"))
						DispMain(_T("양면검사"), RGB_GREEN);
				}
				else
				{
					if (m_sDispMain != _T("단면검사"))
						DispMain(_T("단면검사"), RGB_GREEN);
					//if(m_sDispMain != _T("초기운전")
					//	DispMain(_T("초기운전", RGB_GREEN);
				}
				m_nVsBufLastSerial[0] = GetVsUpBufLastSerial();
				if (bDualTest)
					m_nVsBufLastSerial[1] = GetVsDnBufLastSerial();

				SetListBuf();

				if (MODE_INNER == pDoc->GetTestMode() || MODE_OUTER == pDoc->GetTestMode()) // Please modify for outer mode.-20221226
				{
					GetCurrentInfoEng();
					if (m_pDlgMenu01)
						m_pDlgMenu01->UpdateData();
				}


				General.nStepAuto = AT_LP;
			}
		}
		else
			Winker(MN_RUN); // Run Button - 20151126 : 운전스위치 램프 동작 문제로 수정.
		break;

	case AT_LP + (AtLpVsIdx::ChkShare) :
		if (IsShare()) // ChkShare()
		{
			bPcrInShare[0] = FALSE;
			bPcrInShare[1] = FALSE;

			if (IsShareUp()) // 검사부(상) 검사중
			{
				nSerial = GetShareUp();
				if (nSerial > 0)
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
						{
							// Delete PCR File
							pDoc->DelSharePcrUp();
						}
						else
						{
							//m_nShareUpS = nSerial;
							bPcrInShare[0] = TRUE;
						}
					}
					else
					{
						if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
						{
							// Delete PCR File
							pDoc->DelSharePcrUp();
						}
						else
						{
							//m_nShareUpS = nSerial;
							bPcrInShare[0] = TRUE;
						}
					}
				}
				//else
				//{
				//	m_bLoadShare[0] = FALSE;
				//}
			}
			//else
			//	m_bLoadShare[0] = FALSE;


			if (bDualTest)
			{
				if (IsShareDn()) // 검사부(하) 검사중
				{
					nSerial = GetShareDn();
					if (nSerial > 0)
					{
						if (pView->m_bSerialDecrese)
						{
							if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
							{
								// Delete PCR File
								pDoc->DelSharePcrDn();
							}
							else
							{
								//m_nShareDnS = nSerial;
								bPcrInShare[1] = TRUE;
							}
						}
						else
						{
							if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
							{
								// Delete PCR File
								pDoc->DelSharePcrDn();
							}
							else
							{
								//m_nShareDnS = nSerial;
								bPcrInShare[1] = TRUE;
							}
						}
					}
					//else
					//{
					//	m_bLoadShare[1] = FALSE;
					//}
				}
				//else
				//	m_bLoadShare[1] = FALSE;

				if (bPcrInShare[0] || bPcrInShare[1])
					General.nStepAuto++;
			}
			else
			{
				if (bPcrInShare[0])
					General.nStepAuto++;
			}
		}
		else // if (IsShare())
		{
			if (IsShareVs())
				General.nStepAuto = AT_LP + (AtLpVsIdx::ChkShareVs);
		}
									   break;

	case AT_LP + (AtLpVsIdx::ChkShare) + 1:
		//if (!m_bCont) // 이어가기 아닌 경우.
		//{
		//	if (!ChkStShotNum())
		//	{
		//		Stop();
		//		TowerLamp(RGB_YELLOW, TRUE);
		//	}
		//}
		General.nStepAuto++;
		break;
	case AT_LP + (AtLpVsIdx::ChkShare) + 2:
		//if (IsRun())
	{
		//m_bBufEmpty[0] = pDoc->m_bBufEmpty[0]; // Up
		General.nStepAuto++;
	}
	break;

	case AT_LP + (AtLpVsIdx::ChkShare) + 3:
		Shift2DummyBuf();			// PCR 이동(Share->Buffer)
		General.nStepAuto = AT_LP + (AtLpVsIdx::ChkShare);
		break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) :
		if (IsShareVs()) // ChkShareVs()
		{
			bPcrInShareVs[0] = FALSE;
			bPcrInShareVs[1] = FALSE;

			if (IsShareVsUp()) // 검사부(상) 검사중
			{
				nSerial = GetShareVsUp();
				if (nSerial > 0)
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
						{
							// Delete PCR File
							pDoc->DelShareVsPcrUp();
						}
						else
						{
							m_nShareUpS = nSerial;
							bPcrInShareVs[0] = TRUE;
						}
					}
					else
					{
						if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
						{
							// Delete PCR File
							pDoc->DelShareVsPcrUp();
						}
						else
						{
							m_nShareUpS = nSerial;
							bPcrInShareVs[0] = TRUE;
						}
					}
				}
				//else
				//{
				//	m_bLoadShareVs[0] = FALSE;
				//}
			}
			//else
			//	m_bLoadShareVs[0] = FALSE;


			if (bDualTest)
			{
				if (IsShareVsDn()) // 검사부(하) 검사중
				{
					nSerial = GetShareVsDn();
					if (nSerial > 0)
					{
						if (pView->m_bSerialDecrese)
						{
							if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
							{
								// Delete PCR File
								pDoc->DelShareVsPcrDn();
							}
							else
							{
								m_nShareDnS = nSerial;
								bPcrInShareVs[1] = TRUE;
							}
						}
						else
						{
							if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
							{
								// Delete PCR File
								pDoc->DelShareVsPcrDn();
							}
							else
							{
								m_nShareDnS = nSerial;
								bPcrInShareVs[1] = TRUE;
							}
						}
					}
					//else
					//{
					//	m_bLoadShareVs[1] = FALSE;
					//}
				}
				//else
				//	m_bLoadShareVs[1] = FALSE;

				if (bPcrInShareVs[0] || bPcrInShareVs[1])
					General.nStepAuto++;
			}
			else
			{
				if (bPcrInShareVs[0])
					General.nStepAuto++;
			}
		}
		else // if (IsShare())
		{
			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC;
					m_bLastProc = TRUE;
					nSerial = GetShareVsUp();

					if (bDualTest)
					{
						if (ChkLastProcFromEng())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->GetCurrentInfoEngShotNum();
						}
						else if (ChkLastProcFromUp())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[1].GetLast();
						}
					}
					else
					{
						if (ChkLastProcFromEng())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->GetCurrentInfoEngShotNum();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[0].GetLast();
						}
					}

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
					}

					General.nStepAuto++;
				}
			}
			else // if (!m_bLastProc)
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}

				m_bWaitPcr[0] = FALSE; // 향후 제거해야 할 flag
				m_bWaitPcr[1] = FALSE;
				General.nStepAuto++;
			}

			if (MODE_INNER == pDoc->GetTestMode())
			{
				if (IsSetLotEnd())
					nSerial = GetLotEndSerial();
				else
					nSerial = pDoc->GetCurrentInfoEngShotNum();

				if (ChkLastProc())
				{
					if (ChkLastProcFromEng())
					{
						if (!IsSetLotEnd())
							SetLotEnd(nSerial);
					}
				}
			}
		}
										 break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) + 1:
		if (!m_bCont) // 이어가기 아닌 경우.
		{
			if (!ChkStShotNum())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
		}
		General.nStepAuto++;
		break;
	case AT_LP + (AtLpVsIdx::ChkShareVs) + 2:
		//if (IsRun())
	{
		m_bBufEmpty[0] = pDoc->m_bBufEmpty[0]; // Up
		General.nStepAuto++;
	}
	break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) + 3:
		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		Shift2Buf();			// PCR 이동(Share->Buffer)
		General.nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) :
		//if (!IsRun())
		//	break;

		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

											if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
											{
												Sleep(100);
												break;
											}

											if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
											{
												if (m_bTHREAD_MAKE_ITS_FILE_UP) // Write Reelmap
													break;

												if (bDualTest)
												{
													if (m_bTHREAD_MAKE_ITS_FILE_DN)
														break;
												}
											}

											General.nStepAuto++;

											if (m_nShareUpS > 0)
											{
												m_nShareUpCnt++;

												if (pDoc->GetCurrentInfoEng())
												{
													if (pDoc->GetTestMode() == MODE_OUTER)
														pDoc->GetItsSerialInfo(m_nShareUpS, bDualTestInner, sLot, sLayerUp, sLayerDn, 0);
												}

												bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer에서 PCR파일의 헤드 정보를 얻음.

												if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
												{
													m_bNewModel = TRUE;
													InitInfo();
													ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	

													pDoc->GetCamPxlRes();

													if (IsLastJob(0)) // Up
													{
														pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
															pDoc->WorkingInfo.LastJob.sModelUp,
															pDoc->WorkingInfo.LastJob.sLayerUp);
														pDoc->m_Master[0].LoadMstInfo();

														if (m_pEngrave)
															m_pEngrave->SwMenu01UpdateWorking(TRUE);


														if (pDoc->GetTestMode() == MODE_OUTER)
														{
															pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
																pDoc->WorkingInfo.LastJob.sModelUp,
																pDoc->WorkingInfo.LastJob.sInnerLayerUp);
															pDoc->m_MasterInner[0].LoadMstInfo();
														}
													}

													if (IsLastJob(1)) // Dn
													{
														pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
															pDoc->WorkingInfo.LastJob.sModelDn,
															pDoc->WorkingInfo.LastJob.sLayerDn,
															pDoc->WorkingInfo.LastJob.sLayerUp);
														pDoc->m_Master[1].LoadMstInfo();

														if (pDoc->GetTestMode() == MODE_OUTER)
														{
															pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
																pDoc->WorkingInfo.LastJob.sModelUp,
																pDoc->WorkingInfo.LastJob.sInnerLayerDn,
																pDoc->WorkingInfo.LastJob.sInnerLayerUp);
															pDoc->m_MasterInner[0].LoadMstInfo();
														}
													}

													SetAlignPos();

													InitReelmap();

													if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp))
														InitReelmapInner();

													ModelChange(0); // 0 : AOI-Up , 1 : AOI-Dn

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
												else
												{
													if (m_nShareUpS == 1)
													{
														pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo();
														if ((pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
														{
															if (pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE)
																pView->MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n상면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
															else
																pView->MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n상면 AOI는 일반 모드에서 검사를 진행하였습니다."));

															Stop();
															TowerLamp(RGB_RED, TRUE);
															break;
														}
													}
												}

												if (nNewLot)
												{
													if (!pDoc->m_bNewLotShare[0])
													{
														pDoc->m_bNewLotShare[0] = TRUE;// Lot Change.
														if (!bDualTest)
															OpenReelmapFromBuf(m_nShareUpS);
													}
												}

												LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share

												if (pDoc->m_ListBuf[0].nTot <= pDoc->m_ListBuf[1].nTot)
												{
													UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬
												}

												//if (!bDualTest)
												//{
												//	if (m_nShareUpS != m_nShareUpSprev)
												//	{
												//		m_nShareUpSprev = m_nShareUpS;
												//		UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬 
												//	}
												//}

												if (!m_bLastProc)
												{
													if (ChkLastProc())
													{
														m_nLastProcAuto = LAST_PROC;
														m_bLastProc = TRUE;

														if (bDualTest)
														{
															if (ChkLastProcFromEng())
															{
																if (IsSetLotEnd())
																	nSerial = GetLotEndSerial();
																else
																	nSerial = pDoc->GetCurrentInfoEngShotNum();
															}
															else if (ChkLastProcFromUp())
															{
																if (IsSetLotEnd())
																	nSerial = GetLotEndSerial();
																else
																	nSerial = pDoc->m_ListBuf[0].GetLast();
															}
															else
															{
																if (IsSetLotEnd())
																	nSerial = GetLotEndSerial();
																else
																	nSerial = pDoc->m_ListBuf[1].GetLast();
															}
														}
														else
														{
															if (ChkLastProcFromEng())
															{
																if (IsSetLotEnd())
																	nSerial = GetLotEndSerial();
																else
																	nSerial = pDoc->GetCurrentInfoEngShotNum();
															}
															else
															{
																if (IsSetLotEnd())
																	nSerial = GetLotEndSerial();
																else
																	nSerial = pDoc->m_ListBuf[0].GetLast();
															}
														}

														if (!IsSetLotEnd())
														{
															SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
														}
													}
												}
												else
												{
													if (ChkLastProcFromEng())
													{
														if (IsSetLotEnd())
															nSerial = GetLotEndSerial();
														else
															nSerial = pDoc->GetCurrentInfoEngShotNum();

														if (!IsSetLotEnd())
														{
															SetLotEnd(nSerial);
														}
													}
												}
											}
											break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) + 1:
		//if (!IsRun())
		//	break;

		if (!bDualTest)
		{
			General.nStepAuto++;
			break;
		}

		if (m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_REELMAP_YIELD_DN || m_bTHREAD_REELMAP_YIELD_ALLUP || m_bTHREAD_REELMAP_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		General.nStepAuto++;

		if (IsSetLotEnd())
		{
			if (m_nShareDnS > GetLotEndSerial())
				break;
		}

		if (m_nShareDnS > 0)
		{
			if (m_nShareDnS % 2)
				m_nShareDnSerial[0] = m_nShareDnS; // 홀수
			else
				m_nShareDnSerial[1] = m_nShareDnS; // 짝수
			m_nShareDnCnt++;


			bNewModel = GetAoiDnInfo(m_nShareDnS, &nNewLot);

			if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//MsgBox(_T("신규 모델에 의한 AOI(하)에서 로트 분리가 되었습니다.\r\n이전 로트를 잔량처리 합니다.");
				InitInfo();
				ResetMkInfo(1); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	
				ModelChange(1); // 0 : AOI-Up , 1 : AOI-Dn

			}
			else
			{
				if (m_nShareDnS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo();
					if ((pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE)
							pView->MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n하면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
						else
							pView->MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n하면 AOI는 일반 모드에서 검사를 진행하였습니다."));

						Stop();
						TowerLamp(RGB_RED, TRUE);
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (!pDoc->m_bNewLotShare[1])
				{
					pDoc->m_bNewLotShare[1] = TRUE;// Lot Change.				
					if (bDualTest)
						OpenReelmapFromBuf(m_nShareDnS);
				}
			}

			LoadPcrDn(m_nShareDnS);

			if (pDoc->m_ListBuf[1].nTot <= pDoc->m_ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // 시리얼파일의 정보로 릴맵을 만듬
			}

			//if (bDualTest)
			//{
			//	if (m_nShareDnS != m_nShareDnSprev)
			//	{
			//		m_nShareDnSprev = m_nShareDnS;
			//		UpdateReelmap(m_nShareDnS);  // 시리얼파일의 정보로 릴맵을 만듬  // After inspect bottom side.
			//	}
			//}


			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC;
					m_bLastProc = TRUE;

					if (ChkLastProcFromEng())
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->GetCurrentInfoEngShotNum();
					}
					else if (ChkLastProcFromUp())
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->m_ListBuf[0].GetLast();
					}
					else
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->m_ListBuf[1].GetLast();
					}

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
					}
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}
			}
		}
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) + 2:
		General.nStepAuto++;
		if (m_nShareUpS > 0)
		{
			if (pView->m_pDlgFrameHigh)
				pView->m_pDlgFrameHigh->SetAoiLastShot(0, m_nShareUpS);
		}
		if (bDualTest)
		{
			if (m_nShareDnS > 0)
			{
				if (IsSetLotEnd())
				{
					if (m_nShareDnS > GetLotEndSerial())
						break;
				}

				if (pView->m_pDlgFrameHigh)
					pView->m_pDlgFrameHigh->SetAoiLastShot(1, m_nShareDnS);
			}
		}
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) + 3:
		//if (IsRun())
	{
		if (bDualTest)
		{
			if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				break;
		}
		else
		{
			if (m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
				break;
		}
		General.nStepAuto++;
	}
	break;

	case AT_LP + (AtLpVsIdx::MakeItsFile) :
		//if (IsRun())
	{
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (!bDualTest)
			{
				if (m_nShareUpS > 0)
					MakeItsFile(m_nShareUpS); // BOOL CReelMap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
			}
			else
			{
				if (m_nShareDnS > 0)
					MakeItsFile(m_nShareDnS); // BOOL CReelMap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
			}
		}

		General.nStepAuto++;
	}
										  break;

	case AT_LP + (AtLpVsIdx::MakeItsFile) + 1:
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (m_bTHREAD_MAKE_ITS_FILE_UP) // makeIts
				break;

			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (m_bTHREAD_MAKE_ITS_FILE_DN) // makeIts
					break;
			}
		}
		General.nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ResetShare) :
		m_nShareUpS = 0;
		m_nShareDnS = 0;
		General.nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ResetShare) + 1:
		m_bLoadShare[0] = FALSE;
		m_bLoadShare[1] = FALSE;
		General.nStepAuto = AT_LP + (AtLpVsIdx::ChkShare);
		break;


	case LAST_PROC_VS_ALL:			 // 잔량처리 3
		m_nDummy[0] = 0;
		m_nDummy[1] = 0;
		m_bChkLastProcVs = TRUE;
		TowerLamp(RGB_GREEN, TRUE);
		DispMain(_T("상면VS잔량"), RGB_GREEN);
		if (m_nAoiLastSerial[0] < 1)
			m_nAoiLastSerial[0] = GetAoiUpSerial();
		if (!IsSetLotEnd())
			SetLotEnd(m_nAoiLastSerial[0]);
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 1:
		if (IsVsUp())
			General.nStepAuto++;
		else
			General.nStepAuto = m_nPrevStepAuto;
		break;

	case LAST_PROC_VS_ALL + 2:
		//SetDummyUp();
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 3:
		//SetDummyUp();
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 4:
		//SetDummyUp();
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 5:
		General.nStepAuto = m_nPrevStepAuto;
		break;
	}
}

void CManagerProcedure::DoAutoChkShareFolder()	// 20170727-잔량처리 시 계속적으로 반복해서 이함수가 호출됨으로 좌우 마킹 인덱스 동일 현상 발생.(case AT_LP + 8:)
{
	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = 0;
	CString sNewLot;
	int nNewLot = 0;
	//BOOL bPcrInShare[2];
	BOOL bNewModel = FALSE;

	switch (General.nStepAuto)
	{
	case 0:
		m_bSwRun = FALSE;
		m_bSwStop = TRUE;
		General.nStepAuto++;
		break;
	case 1:
		if (IsReady() || IsAuto())		// 운전준비
		{
			TowerLamp(RGB_YELLOW, TRUE, TRUE);
			General.nStepAuto++;
		}
		break;
	case 2:
		if (IsRun())
			General.nStepAuto++;
		break;
	case 3:
		ClrDispMsg();
		TowerLamp(RGB_YELLOW, TRUE, FALSE);
		General.nStepAuto++;
		break;
	case 4:
		if (IsRun())		// 초기운전
		{
			if (!IsAoiLdRun())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
			else
			{
				ResetWinker(); // 20151126 : 운전스위치 램프 동작 문제로 수정.

				TowerLamp(RGB_GREEN, TRUE, TRUE);
				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (pDoc->WorkingInfo.LastJob.bDualTest)
					{
						if (m_sDispMain != _T("양면샘플"))
							DispMain(_T("양면샘플"), RGB_GREEN);
					}
					else
					{
						if (m_sDispMain != _T("단면샘플"))
							DispMain(_T("단면샘플"), RGB_GREEN);
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (m_sDispMain != _T("내층검사"))
						DispMain(_T("내층검사"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_sDispMain != _T("외층검사"))
						DispMain(_T("외층검사"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (m_sDispMain != _T("양면검사"))
						DispMain(_T("양면검사"), RGB_GREEN);
				}
				else
				{
					if (m_sDispMain != _T("단면검사"))
						DispMain(_T("단면검사"), RGB_GREEN);
					//if(m_sDispMain != _T("초기운전")
					//	DispMain(_T("초기운전", RGB_GREEN);
				}
				m_nVsBufLastSerial[0] = GetVsUpBufLastSerial();
				if (bDualTest)
					m_nVsBufLastSerial[1] = GetVsDnBufLastSerial();

				SetListBuf();

				if (MODE_INNER == pDoc->GetTestMode() || MODE_OUTER == pDoc->GetTestMode()) // Please modify for outer mode.-20221226
				{
					GetCurrentInfoEng();
					if (m_pDlgMenu01)
						m_pDlgMenu01->UpdateData();
				}


				General.nStepAuto = AT_LP;
			}
		}
		else
			Winker(MN_RUN); // Run Button - 20151126 : 운전스위치 램프 동작 문제로 수정.
		break;

	case AT_LP:
		if (IsShare()) // ChkShare()
		{
			bPcrInShare[0] = FALSE;
			bPcrInShare[1] = FALSE;

			//if (IsShareUp() && IsTestDoneUp() && !m_bAoiTestF[0]) // 검사부(상) 검사중
			if (IsShareUp())
			{
				nSerial = GetShareUp();
				if (nSerial > 0)
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
						{
							// Delete PCR File
							pDoc->DelSharePcrUp();
						}
						else
						{
							m_nShareUpS = nSerial;
							bPcrInShare[0] = TRUE;
						}
					}
					else
					{
						if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
						{
							// Delete PCR File
							pDoc->DelSharePcrUp();
						}
						else
						{
							m_nShareUpS = nSerial;
							bPcrInShare[0] = TRUE;
						}
					}
				}
				else
				{
					m_bLoadShare[0] = FALSE;
				}
			}
			else
				m_bLoadShare[0] = FALSE;


			if (bDualTest)
			{
				//if (IsShareDn() && IsTestDoneDn() && !m_bAoiTestF[1]) // 검사부(하) 검사중
				if (IsShareDn())
				{
					nSerial = GetShareDn();
					if (nSerial > 0)
					{
						if (pView->m_bSerialDecrese)
						{
							if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
							{
								// Delete PCR File
								pDoc->DelSharePcrDn();
							}
							else
							{
								m_nShareDnS = nSerial;
								bPcrInShare[1] = TRUE;
							}
						}
						else
						{
							if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
							{
								// Delete PCR File
								pDoc->DelSharePcrDn();
							}
							else
							{
								m_nShareDnS = nSerial;
								bPcrInShare[1] = TRUE;
							}
						}
					}
					else
					{
						m_bLoadShare[1] = FALSE;
					}
				}
				else
					m_bLoadShare[1] = FALSE;

				if (bPcrInShare[0] || bPcrInShare[1])
					General.nStepAuto++;
			}
			else
			{
				if (bPcrInShare[0])
					General.nStepAuto++;
			}
		}
		else // if (IsShare())
		{
			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC;
					m_bLastProc = TRUE;
					nSerial = GetShareUp();

					//if (IsVs())
					//{
					//	if (m_nAoiLastSerial[0] < 1)
					//		m_nAoiLastSerial[0] = nSerial;

					//	m_nPrevStepAuto = General.nStepAuto;
					//	General.nStepAuto = LAST_PROC_VS_ALL;		 // 잔량처리 3
					//	break;
					//}
					//else
					{
						if (bDualTest)
						{
							if (ChkLastProcFromEng())
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->GetCurrentInfoEngShotNum();
							}
							else if (ChkLastProcFromUp())
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->m_ListBuf[0].GetLast();
							}
							else
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->m_ListBuf[1].GetLast();
							}
						}
						else
						{
							if (ChkLastProcFromEng())
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->GetCurrentInfoEngShotNum();
							}
							else
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->m_ListBuf[0].GetLast();
							}
						}

						if (!IsSetLotEnd()) // 20160810
						{
							SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
											   //if (m_nAoiLastSerial[0] < 1)
											   //m_nAoiLastSerial[0] = nSerial;
						}

						General.nStepAuto++;
					}
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					//if (m_nLotEndSerial != nSerial)
					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
						//if (m_nAoiLastSerial[0] < 1)
						//m_nAoiLastSerial[0] = nSerial;
					}
				}

				m_bWaitPcr[0] = FALSE;
				m_bWaitPcr[1] = FALSE;
				General.nStepAuto++;
			}

			if (MODE_INNER == pDoc->GetTestMode())
			{
				if (IsSetLotEnd())
					nSerial = GetLotEndSerial();
				else
					nSerial = pDoc->GetCurrentInfoEngShotNum();
				//SetLastSerialEng(nSerial);

				if (ChkLastProc())
				{
					if (ChkLastProcFromEng())
					{
						//if (m_nLotEndSerial != nSerial)
						if (!IsSetLotEnd())
							SetLotEnd(nSerial);
					}
				}
			}

		}
		break;

	case AT_LP + 1:
		if (!m_bCont) // 이어가기 아닌 경우.
		{
			if (!ChkStShotNum())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
		}
		else
		{
			//if (!ChkContShotNum())
			//{
			//	Stop();
			//	TowerLamp(RGB_YELLOW, TRUE);
			//}
		}
		General.nStepAuto++;
		break;
	case AT_LP + 2:
		if (IsRun())
		{
			m_bBufEmpty[0] = pDoc->m_bBufEmpty[0]; // Up
			General.nStepAuto++;
		}
		break;

	case AT_LP + 3:
		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		Shift2Buf();			// PCR 이동(Share->Buffer)
		General.nStepAuto++;
		break;

	case AT_LP + 4:
		if (!IsRun())
			break;

		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			//if (m_bTHREAD_UPDATE_REELMAP_INOUTER_UP) // Write Reelmap
			if (m_bTHREAD_MAKE_ITS_FILE_UP)
				break;

			if (bDualTest)
			{
				if (m_bTHREAD_MAKE_ITS_FILE_DN)
					break;
			}
		}

		//if (!bDualTest)
		//{
		//	if (pDoc->GetTestMode() == MODE_OUTER)
		//	{
		//		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_INNER_UP) // Write Reelmap
		//			break;

		//		if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		//		{
		//			if (m_bTHREAD_UPDATE_REELMAP_INNER_DN || m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP || m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
		//				break;
		//		}
		//	}
		//}

		General.nStepAuto++;

		if (m_nShareUpS > 0)
		{
			//if (m_nShareUpS % 2)
			//	m_nShareUpSerial[0] = m_nShareUpS; // 홀수
			//else
			//	m_nShareUpSerial[1] = m_nShareUpS; // 짝수
			m_nShareUpCnt++;

			if (pDoc->GetCurrentInfoEng())
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
					pDoc->GetItsSerialInfo(m_nShareUpS, bDualTestInner, sLot, sLayerUp, sLayerDn, 0);
			}

			bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer에서 PCR파일의 헤드 정보를 얻음.

			if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				m_bNewModel = TRUE;
				InitInfo();
				ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	

				pDoc->GetCamPxlRes();

				if (IsLastJob(0)) // Up
				{
					pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
						pDoc->WorkingInfo.LastJob.sModelUp,
						pDoc->WorkingInfo.LastJob.sLayerUp);
					pDoc->m_Master[0].LoadMstInfo();
					//pDoc->m_Master[0].WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->WorkingInfo.LastJob.sLotUp);


					if (m_pEngrave)
						m_pEngrave->SwMenu01UpdateWorking(TRUE);


					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						//GetCurrentInfoEng();
						pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModelUp,
							pDoc->WorkingInfo.LastJob.sInnerLayerUp);
						pDoc->m_MasterInner[0].LoadMstInfo();
					}
				}

				if (IsLastJob(1)) // Dn
				{
					pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
						pDoc->WorkingInfo.LastJob.sModelDn,
						pDoc->WorkingInfo.LastJob.sLayerDn,
						pDoc->WorkingInfo.LastJob.sLayerUp);
					pDoc->m_Master[1].LoadMstInfo();
					//pDoc->m_Master[1].WriteStripPieceRegion_Text(pDoc->WorkingInfo.System.sPathOldFile, pDoc->WorkingInfo.LastJob.sLotDn);

					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						//GetCurrentInfoEng();
						pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModelUp,
							pDoc->WorkingInfo.LastJob.sInnerLayerDn,
							pDoc->WorkingInfo.LastJob.sInnerLayerUp);
						pDoc->m_MasterInner[0].LoadMstInfo();
					}
				}

				SetAlignPos();

				// 20220502 - end
				InitReelmap();	// 20220421


				if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp))
					InitReelmapInner();	// 20220421


				ModelChange(0); // 0 : AOI-Up , 1 : AOI-Dn

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

				if (pDoc->GetTestMode() == MODE_OUTER) // syd-20231121
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
			else
			{
				if (m_nShareUpS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo();
					if ((pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE)
							pView->MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n상면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
						else
							pView->MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n상면 AOI는 일반 모드에서 검사를 진행하였습니다."));

						Stop();
						TowerLamp(RGB_RED, TRUE);
						break;

					}
				}
			}

			if (nNewLot)
			{
				if (!pDoc->m_bNewLotShare[0])
				{
					pDoc->m_bNewLotShare[0] = TRUE;// Lot Change.
					if (!bDualTest)
						OpenReelmapFromBuf(m_nShareUpS);
				}
			}

			LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share

			if (pDoc->m_ListBuf[0].nTot <= pDoc->m_ListBuf[1].nTot)
			{
				UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬
			}

			//if (!bDualTest)
			//{
			//	if (m_nShareUpS != m_nShareUpSprev)
			//	{
			//		m_nShareUpSprev = m_nShareUpS;
			//		UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬 
			//	}
			//}

			if (!m_bLastProc)
			{
				//if (!IsSetLotEnd())
				//{
				//	if (ChkLotEndUp(m_nShareUpS))// 파일의 내용 중에 Lot End (-2) 잔량처리를 체크함. (연속 3Pnl:-2) -> 로트완료 
				//	{
				//		SetLotEnd(m_nShareUpS - pDoc->AoiDummyShot[0]);
				//		if (m_nAoiLastSerial[0] < 1)
				//			m_nAoiLastSerial[0] = m_nShareUpS;

				//		if (!bDualTest)
				//		{
				//			m_bLastProc = TRUE;
				//			m_nLastProcAuto = LAST_PROC;
				//		}
				//	}
				//}

				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC;
					m_bLastProc = TRUE;

					//if (IsVs())
					//{
					//	if (m_nAoiLastSerial[0] < 1)
					//		m_nAoiLastSerial[0] = m_nShareUpS;

					//	m_nPrevStepAuto = General.nStepAuto;
					//	General.nStepAuto = LAST_PROC_VS_ALL;		 // 잔량처리 3
					//	break;
					//}
					//else
					{
						if (bDualTest)
						{
							if (ChkLastProcFromEng())
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->GetCurrentInfoEngShotNum();
							}
							else if (ChkLastProcFromUp())
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->m_ListBuf[0].GetLast();
							}
							else
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->m_ListBuf[1].GetLast();
							}
						}
						else
						{
							if (ChkLastProcFromEng())
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->GetCurrentInfoEngShotNum();
							}
							else
							{
								if (IsSetLotEnd())
									nSerial = GetLotEndSerial();
								else
									nSerial = pDoc->m_ListBuf[0].GetLast();
							}
						}

						if (!IsSetLotEnd()) // 20160810
						{
							SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
											   //if (m_nAoiLastSerial[0] < 1)
											   //m_nAoiLastSerial[0] = nSerial;
						}
					}
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					//if (m_nLotEndSerial != nSerial)
					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
						//if (m_nAoiLastSerial[0] < 1)
						//m_nAoiLastSerial[0] = nSerial;
					}
				}
			}
		}
		else
		{
			//ClrDispMsg();
			//AfxMessageBox(_T("Error : m_nShareUpS <= 0"));
		}
		break;

	case AT_LP + 5:
		if (!IsRun())
			break;

		if (!bDualTest)
		{
			General.nStepAuto++;
			break;
		}

		if (m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_REELMAP_YIELD_DN || m_bTHREAD_REELMAP_YIELD_ALLUP || m_bTHREAD_REELMAP_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		General.nStepAuto++;

		//if (m_bChkLastProcVs)
		//{
		//	if (m_nShareDnS > m_nAoiLastSerial[0] && m_nAoiLastSerial[0] > 0)
		//		break;
		//}
		//else
		{
			if (IsSetLotEnd())
			{
				//if (m_nShareDnS > m_nAoiLastSerial[0] && m_nAoiLastSerial[0] > 0)
				if (m_nShareDnS > GetLotEndSerial())
					break;
			}
		}


		if (m_nShareDnS > 0)
		{
			if (m_nShareDnS % 2)
				m_nShareDnSerial[0] = m_nShareDnS; // 홀수
			else
				m_nShareDnSerial[1] = m_nShareDnS; // 짝수
			m_nShareDnCnt++;


			bNewModel = GetAoiDnInfo(m_nShareDnS, &nNewLot);

			if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//MsgBox(_T("신규 모델에 의한 AOI(하)에서 로트 분리가 되었습니다.\r\n이전 로트를 잔량처리 합니다.");
				InitInfo();
				ResetMkInfo(1); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	
				ModelChange(1); // 0 : AOI-Up , 1 : AOI-Dn

			}
			else
			{
				if (m_nShareDnS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo();
					if ((pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE)
							pView->MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n하면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
						else
							pView->MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n하면 AOI는 일반 모드에서 검사를 진행하였습니다."));

						Stop();
						TowerLamp(RGB_RED, TRUE);
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (!pDoc->m_bNewLotShare[1])
				{
					pDoc->m_bNewLotShare[1] = TRUE;// Lot Change.				
					if (bDualTest)
						OpenReelmapFromBuf(m_nShareDnS);
				}
			}

			LoadPcrDn(m_nShareDnS);

			if (pDoc->m_ListBuf[1].nTot <= pDoc->m_ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // 시리얼파일의 정보로 릴맵을 만듬
			}

			//if (bDualTest)
			//{
			//	if (m_nShareDnS != m_nShareDnSprev)
			//	{
			//		m_nShareDnSprev = m_nShareDnS;
			//		UpdateReelmap(m_nShareDnS);  // 시리얼파일의 정보로 릴맵을 만듬  // After inspect bottom side.
			//	}
			//}


			if (!m_bLastProc)
			{
				//if (!IsSetLotEnd())
				//{
				//	if (ChkLotEndDn(m_nShareDnS))// 파일의 내용 중에 Lot End (-2) 잔량처리를 체크함. (연속 3Pnl:-2) -> 로트완료 
				//	{
				//		if (!IsSetLotEnd())
				//			SetLotEnd(m_nShareDnS - pDoc->AoiDummyShot[1]);
				//		if (m_nAoiLastSerial[0] < 1)
				//			m_nAoiLastSerial[0] = m_nShareDnS;
				//		if (bDualTest)
				//		{
				//			m_bLastProc = TRUE;
				//			m_nLastProcAuto = LAST_PROC;
				//		}
				//	}
				//}

				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC;
					m_bLastProc = TRUE;

					//if (IsVs())
					//{
					//	if (m_nAoiLastSerial[0] < 1)
					//		m_nAoiLastSerial[0] = m_nShareDnS;

					//	m_nPrevStepAuto = General.nStepAuto;
					//	General.nStepAuto = LAST_PROC_VS_ALL;		 // 잔량처리 3
					//	break;
					//}
					//else
					{
						if (ChkLastProcFromEng())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->GetCurrentInfoEngShotNum();
						}
						else if (ChkLastProcFromUp())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[1].GetLast();
						}

						if (!IsSetLotEnd()) // 20160810
						{
							SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
											   //if (m_nAoiLastSerial[0] < 1)
											   //m_nAoiLastSerial[0] = nSerial;
						}
					}
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					//if (m_nLotEndSerial != nSerial)
					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
						//if (m_nAoiLastSerial[0] < 1)
						//m_nAoiLastSerial[0] = nSerial;
					}
				}
			}
		}
		else
		{
			//ClrDispMsg();
			//AfxMessageBox(_T("Error : m_nShareDnS <= 0"));
		}

		break;

	case AT_LP + 6:
		General.nStepAuto++;
		if (m_nShareUpS > 0)
		{
			if (pView->m_pDlgFrameHigh)
				pView->m_pDlgFrameHigh->SetAoiLastShot(0, m_nShareUpS);
		}
		if (bDualTest)
		{
			if (m_nShareDnS > 0)
			{
				//if (m_bChkLastProcVs)
				//{
				//	if (m_nShareDnS > m_nAoiLastSerial[0] && m_nAoiLastSerial[0] > 0)
				//		break;
				//}
				//else
				{
					if (IsSetLotEnd())
					{
						//if (m_nShareDnS > m_nAoiLastSerial[0] && m_nAoiLastSerial[0] > 0)
						if (m_nShareDnS > GetLotEndSerial())
							break;
					}
				}

				if (pView->m_pDlgFrameHigh)
					pView->m_pDlgFrameHigh->SetAoiLastShot(1, m_nShareDnS);
			}
		}
		break;

	case AT_LP + 7:
		if (IsRun())
		{
			//if (pDoc->GetTestMode() == MODE_OUTER)
			//{
			//	if (m_bTHREAD_UPDATE_REELMAP_INNER_UP) // Write Reelmap
			//		break;

			//	if (pDoc->WorkingInfo.LastJob.bDualTestInner)
			//	{
			//		if (m_bTHREAD_UPDATE_REELMAP_INNER_DN || m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP || m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
			//			break;
			//	}
			//}

			if (bDualTest)
			{
				if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
					break;
			}
			else
			{
				if (m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
					break;
			}
			//SetListBuf(); // 20170727-잔량처리 시 계속적으로 반복해서 이함수가 호출됨으로 좌우 마킹 인덱스 동일 현상 발생.(case AT_LP + 8:)
			General.nStepAuto++;
		}
		break;


	case AT_LP + 8:
		if (IsRun())
		{
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!bDualTest)
				{
					if (m_nShareUpS > 0)
						MakeItsFile(m_nShareUpS);
					//UpdateReelmapInner(m_nShareUpS);
				}
				else
				{
					if (m_nShareDnS > 0)
						MakeItsFile(m_nShareDnS);
					//UpdateReelmapInner(m_nShareDnS);
				}
			}

			General.nStepAuto++;
		}
		break;

	case AT_LP + 9:
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (m_bTHREAD_MAKE_ITS_FILE_UP) // makeIts
				break;

			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (m_bTHREAD_MAKE_ITS_FILE_DN) // makeIts
					break;
			}

			//if (pDoc->GetTestMode() == MODE_OUTER)
			//{
			//	if (pDoc->WorkingInfo.LastJob.bDualTestInner)
			//	{
			//		if (m_bTHREAD_UPDATE_REELMAP_INNER_DN || m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP || m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
			//			break;
			//	}
			//}
			//else
			//{
			//	if (pDoc->WorkingInfo.LastJob.bDualTest)
			//	{
			//		if (m_bTHREAD_UPDATE_REELMAP_INNER_DN || m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP || m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
			//			break;
			//	}
			//}
		}
		General.nStepAuto++;
		break;

	case AT_LP + 10:
		m_nShareUpS = 0;
		m_nShareDnS = 0;
		General.nStepAuto++;
		break;

	case AT_LP + 11:
		m_bLoadShare[0] = FALSE;
		m_bLoadShare[1] = FALSE;
		General.nStepAuto = AT_LP;
		break;


	case LAST_PROC_VS_ALL:			 // 잔량처리 3
		m_nDummy[0] = 0;
		m_nDummy[1] = 0;
		m_bChkLastProcVs = TRUE;
		TowerLamp(RGB_GREEN, TRUE);
		DispMain(_T("상면VS잔량"), RGB_GREEN);
		if (m_nAoiLastSerial[0] < 1)
			m_nAoiLastSerial[0] = GetAoiUpSerial();
		if (!IsSetLotEnd())
			SetLotEnd(m_nAoiLastSerial[0]);
		//m_nAoiLastSerial[1] = GetAoiDnSerial();
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 1:
		if (IsVsUp())
			General.nStepAuto++;
		else
			General.nStepAuto = m_nPrevStepAuto;
		break;

	case LAST_PROC_VS_ALL + 2:
		//SetDummyUp();
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 3:
		//SetDummyUp();
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 4:
		//SetDummyUp();
		General.nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 5:
		General.nStepAuto = m_nPrevStepAuto;
		break;
	}
}

void CManagerProcedure::DoAutoMarking()
{
	if (pDoc->WorkingInfo.LastJob.nAlignMethode == TWO_POINT)
		MarkingWith2PointAlign();
	else if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
		MarkingWith4PointAlign();
	else
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("마킹을 위한 Align방식이 정해지지 않았습니다."));
	}
}

void CManagerProcedure::DoAutoMarkingEngrave()
{
	stBtnStatus& BtnStatus = (pView->m_mgrProcedure->m_pEngrave->BtnStatus);

	// 각인부 마킹중 ON (PC가 ON, OFF)
	if (BtnStatus.EngAuto.IsOnMking && !(m_pMpe->m_pMpeSignal[6] & (0x01 << 3))) // 각인부 마킹중 ON (PC가 ON, OFF)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, TRUE);
#ifdef USE_MPE
		if (m_pMpe)
		{
			pDoc->LogAuto(_T("PC: 2D(GUI) 각인 동작Running신호 ON (PC On->PC Off)"));
			MpeWrite(_T("MB440173"), 1); // 2D(GUI) 각인 동작Running신호(PC On->PC Off)
		}
#endif
	}
	else if (!BtnStatus.EngAuto.IsOnMking && (m_pMpe->m_pMpeSignal[6] & (0x01 << 3)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
#ifdef USE_MPE
		if (m_pMpe)
		{
			pDoc->LogAuto(_T("PC: 2D(GUI) 각인 동작Running신호 OFF (PC On->PC Off)"));
			MpeWrite(_T("MB440173"), 0); // 2D(GUI) 각인 동작Running신호(PC On->PC Off)
		}
#endif
	}

	// 각인부 마킹완료 ON (PC가 ON, OFF)
	if (BtnStatus.EngAuto.IsMkDone && !(m_pMpe->m_pMpeSignal[6] & (0x01 << 4))) // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, TRUE);
#ifdef USE_MPE
		if (m_pMpe)
		{
			pDoc->LogAuto(_T("PC: 각인부 작업완료 ON (PC가 On, PLC가 확인 후 Off)"));
			MpeWrite(_T("MB440174"), 1); // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
		}
#endif
	}
	else if (!BtnStatus.EngAuto.IsMkDone && (m_pMpe->m_pMpeSignal[6] & (0x01 << 4))) // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, FALSE);
		//#ifdef USE_MPE
		//		if (m_pMpe)
		//			MpeWrite(_T("MB440174"), 0); // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
		//#endif
	}

	// 각인부 2D 리더 작업중 신호
	if (BtnStatus.EngAuto.IsOnRead2d && !(m_pMpe->m_pMpeSignal[6] & (0x01 << 8))) // 각인부 2D 리더 작업중 신호(PC On->PC Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, TRUE);
#ifdef USE_MPE
		if (m_pMpe)
		{
			pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업중 신호 ON (PC On->PC Off)"));
			MpeWrite(_T("MB440178"), 1); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
		}
#endif
	}
	else if (!BtnStatus.EngAuto.IsOnRead2d && (m_pMpe->m_pMpeSignal[6] & (0x01 << 8)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
#ifdef USE_MPE
		if (m_pMpe)
		{
			pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업중 신호 OFF (PC On->PC Off)"));
			MpeWrite(_T("MB440178"), 0); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
		}
#endif
	}

	// 각인부 2D 리더 작업완료 신호
	if (BtnStatus.EngAuto.IsRead2dDone && !(m_pMpe->m_pMpeSignal[6] & (0x01 << 9))) // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, TRUE);
#ifdef USE_MPE
		if (m_pMpe)
		{
			pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업완료 신호 ON (PC가 On, PLC가 확인 후 Off)"));
			MpeWrite(_T("MB440179"), 1); // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
		}
#endif
	}
	else if (!BtnStatus.EngAuto.IsRead2dDone && (m_pMpe->m_pMpeSignal[6] & (0x01 << 9)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, FALSE);
		//#ifdef USE_MPE
		//		if (m_pMpe)
		//			MpeWrite(_T("MB440179"), 0); // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
		//#endif
	}

}

BOOL CManagerProcedure::IsLotEnd()
{
	return m_bLotEnd;
}

BOOL CManagerProcedure::ReloadReelmap(int nSerial)
{
	m_nReloadReelmapSerial = nSerial;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_bTHREAD_RELOAD_RST_UP = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_RELOAD_RST_DN = TRUE;
		m_bTHREAD_RELOAD_RST_ALLUP = TRUE;
		m_bTHREAD_RELOAD_RST_ALLDN = TRUE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		m_bTHREAD_RELOAD_RST_UP_INNER = TRUE;
		m_bTHREAD_RELOAD_RST_ITS = TRUE;
		if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		{
			m_bTHREAD_RELOAD_RST_DN_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLUP_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLDN_INNER = TRUE;
		}
	}

	Sleep(100);

	return TRUE;
}

void CManagerProcedure::UpdateRst()
{
	pView->UpdateRst();
}

void CManagerProcedure::LotEnd()
{
	//if (m_pDlgMenu01)
	//	m_pDlgMenu01->LotEnd();
	//if (m_pDlgMenu03)
	//	m_pDlgMenu03->SwAoiLotEnd(TRUE);

	m_bCont = FALSE;
	SetLotEd();

	//ReloadReelmap();
	MakeResultMDS();

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	{
		FinalCopyItsFiles();
	}

	if (pDoc->m_pReelMap == pDoc->m_pReelMapUp)
	{
		DuplicateRmap(RMAP_UP);
	}
	else if (pDoc->m_pReelMap == pDoc->m_pReelMapAllUp)
	{
		DuplicateRmap(RMAP_ALLUP);
	}
	else if (pDoc->m_pReelMap == pDoc->m_pReelMapIts)
	{
		DuplicateRmap(RMAP_ITS);
	}

	if (m_pEngrave)
		m_pEngrave->SwMenu01DispDefImg(TRUE);
}

BOOL CManagerProcedure::ChkLastProcFromUp()
{
	if (!pView->m_mgrStatus)
		return FALSE;

	stGeneral& General = (pView->m_mgrStatus->General);

	return General.bLastProcFromUp;
}

BOOL CManagerProcedure::ChkLastProcFromEng()
{
	if (!pView->m_mgrStatus)
		return FALSE;

	stGeneral& General = (pView->m_mgrStatus->General);

	return General.bLastProcFromEng;
}

void CManagerProcedure::SetAoiUpAlarmRestartMsg(CString sAoiUpAlarmReStartMsg)		// m_sAoiUpAlarmReStartMsg
{
	if (!pView->m_mgrFeeding)
		return;
	pView->m_mgrFeeding->SetAoiUpAlarmRestartMsg(sAoiUpAlarmReStartMsg);
}

void CManagerProcedure::SetAoiDnAlarmRestartMsg(CString sAoiDnAlarmReStartMsg)		// m_sAoiDnAlarmReStartMsg
{
	if (!pView->m_mgrFeeding)
		return;
	pView->m_mgrFeeding->SetAoiDnAlarmRestartMsg(sAoiDnAlarmReStartMsg);
}

void CManagerProcedure::SetAoiUpAlarmReTestMsg(CString sAoiUpAlarmReTestMsg)			// m_sAoiUpAlarmReTestMsg
{
	if (!pView->m_mgrFeeding)
		return;
	pView->m_mgrFeeding->SetAoiUpAlarmReTestMsg(sAoiUpAlarmReTestMsg);
}

void CManagerProcedure::SetAoiDnAlarmReTestMsg(CString sAoiDnAlarmReTestMsg)			// m_sAoiDnAlarmReTestMsg
{
	if (!pView->m_mgrFeeding)
		return;
	pView->m_mgrFeeding->SetAoiDnAlarmReTestMsg(sAoiDnAlarmReTestMsg);
}
