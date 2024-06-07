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

void CManagerProcedure::Init()
{
	m_bMkSt = FALSE;
	m_bMkStSw = FALSE;
	m_nMkStAuto = 0;
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
	TCHAR szData[200];
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiUpVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerUp, pDoc->WorkingInfo.LastJob.sLotUp);

	if (0 < ::GetPrivateProfileString(_T("Region"), _T("Piece Region Type"), NULL, szData, sizeof(szData), sPath))
		pView->m_mgrPunch->m_Master[0].MasterInfo.nOutFileOnAoi = _ttoi(szData);
	else
		pView->m_mgrPunch->m_Master[0].MasterInfo.nOutFileOnAoi = -1;

	return pView->m_mgrPunch->m_Master[0].MasterInfo.nOutFileOnAoi;
}

int CManagerProcedure::GetAoiDnCamMstInfo() // AOI하 strpcs.bin 연결
{
	TCHAR szData[200];
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiDnVrsData,
		pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerDn, pDoc->WorkingInfo.LastJob.sLotUp);

	if (0 < ::GetPrivateProfileString(_T("Region"), _T("Piece Region Type"), NULL, szData, sizeof(szData), sPath))
		pView->m_mgrPunch->m_Master[1].MasterInfo.nOutFileOnAoi = _ttoi(szData);
	else
		pView->m_mgrPunch->m_Master[1].MasterInfo.nOutFileOnAoi = -1;

	return pView->m_mgrPunch->m_Master[1].MasterInfo.nOutFileOnAoi;
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

void CManagerProcedure::InitAuto(BOOL bInit)
{/*
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
	m_nStepAuto = 0;
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
	m_pMpe->Write(_T("MB440100"), 0); // PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
	m_pMpe->Write(_T("MB440110"), 0); // 마킹시작(PC가 확인하고 Reset시킴.)-20141029
	m_pMpe->Write(_T("MB440150"), 0); // 마킹부 마킹중 ON (PC가 ON, OFF)
	m_pMpe->Write(_T("MB440170"), 0); // 마킹완료(PLC가 확인하고 Reset시킴.)-20141029

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
		m_pMpe->Write(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
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
				m_pMpe->Write(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					m_pMpe->Write(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
				else
				{
					m_pMpe->Write(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
			}
		}
		else if (pDoc->GetTestMode() == MODE_INNER && !pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == pView->MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				m_pMpe->Write(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("각인부에서 레이저 각인부터 시작하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					m_pMpe->Write(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
				else
				{
					m_pMpe->Write(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
			}

		}
		else
		{
			m_pMpe->Write(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
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
*/
}

void CManagerProcedure::Auto()
{
	DoAuto();
}

void CManagerProcedure::DoAuto()
{
	//if (!pView->IsAuto())
	//	return;

	CString str;
	str.Format(_T("%d : %d"), m_nStepTHREAD_DISP_DEF, m_bTHREAD_DISP_DEF ? 1 : 0);
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
	if (GetAoiUpVsStatus())
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
	int nSerial;

	if (m_pDlgMenu01)
	{
		if (m_pDlgMenu01->m_bLotEnd && m_nStepAuto < LOT_END)
		{
			m_bLotEnd = TRUE;
			m_nLotEndAuto = LOT_END;
		}
	}

	if (!IsBuffer(0) && m_bLastProc && m_nLotEndAuto < LOT_END)
	{
		m_bLotEnd = TRUE;
		m_nLotEndAuto = LOT_END;
	}
	else if (!IsBuffer(0) && m_nMkStAuto > MK_ST + (Mk2PtIdx::DoneMk) + 4)
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
			if (!m_bTHREAD_REELMAP_YIELD_UP && !m_bTHREAD_REELMAP_YIELD_DN && !m_bTHREAD_REELMAP_YIELD_ALLUP && !m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				ReloadReelmap(nSerial);
				UpdateRst();
				//pDoc->UpdateYieldOnRmap(); // 20230614
				m_nLotEndAuto++;
			}
			break;
		case LOT_END + 1:
			m_pMpe->Write(_T("MB440180"), 1);			// 작업종료(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
			DispMain(_T("작업종료"), RGB_RED);
			m_nLotEndAuto++;
			break;
		case LOT_END + 2:
			Buzzer(TRUE, 0);
			TowerLamp(RGB_YELLOW, TRUE);
			Stop();
			LotEnd();									// MakeResultMDS
			m_nLotEndAuto++;
			break;

		case LOT_END + 3:
			MsgBox(_T("작업이 종료되었습니다."));
			//m_nStepAuto = 0; // 자동종료
			m_nLotEndAuto++;
			//m_bLotEnd = FALSE;
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
#ifdef USE_MPE
	if (!m_bMkSt)
	{
		if (pView->IsRun())
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
#endif
	return FALSE;
}
