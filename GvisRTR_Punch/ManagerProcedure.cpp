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
	m_dTotVel = 0.0; 
	m_dPartVel = 0.0;
	m_dwCycSt = 0;
	m_dwCycTim = 0;

	m_bEngSt = FALSE;
	m_bEngStSw = FALSE;
	m_nEngStAuto = 0;

	m_bEng2dSt = FALSE;
	m_bEng2dStSw = FALSE;
	m_nEng2dStAuto = 0;

	m_bPcrInShare[0] = FALSE;
	m_bPcrInShare[1] = FALSE;
	m_bPcrInShareVs[0] = FALSE;
	m_bPcrInShareVs[1] = FALSE;

	for (i = 0; i < MAX_DISP; i++)
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
	//if (GetDispMain() == _T("운전중") || GetDispMain() == _T("초기운전") || GetDispMain() == _T("단면샘플")
	//	|| GetDispMain() == _T("단면검사") || GetDispMain() == _T("내층검사") || GetDispMain() == _T("외층검사")
	//	|| GetDispMain() == _T("양면검사") || GetDispMain() == _T("양면샘플"))
	//	return TRUE;
	//return FALSE;
	//return m_bSwRun;
}

BOOL CManagerProcedure::IsReady()
{
	return pView->IsReady();
}

BOOL CManagerProcedure::IsAuto()
{
	return pView->IsAuto();
}

BOOL CManagerProcedure::IsAoiLdRun()
{
	return pView->IsAoiLdRun();
}

void CManagerProcedure::InitAuto(BOOL bInit)
{
	stGeneral& General = (pView->m_mgrStatus->General);

	if (!pDoc->WorkingInfo.LastJob.bSampleTest)
	{
		::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
		::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	}

	SetAoiUpAlarmRestartMsg(ReadAoiUpAlarmRestartMsg());		// m_sAoiUpAlarmReStartMsg
	SetAoiDnAlarmRestartMsg(ReadAoiDnAlarmRestartMsg());		// m_sAoiDnAlarmReStartMsg
	SetAoiUpAlarmReTestMsg(ReadAoiUpAlarmReTestMsg());			// m_sAoiUpAlarmReTestMsg
	SetAoiDnAlarmReTestMsg(ReadAoiDnAlarmReTestMsg());			// m_sAoiDnAlarmReTestMsg

	pDoc->SetAoiUpAutoStep(0);
	pDoc->SetAoiDnAutoStep(0);

	int nCam, nPoint, kk, a, b;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	for (kk = 0; kk < 10; kk++)
	{
		m_bDispMsgDoAuto[kk] = FALSE;
		m_nStepDispMsg[kk] = 0;
	}
	m_sFixMsg[0] = _T("");
	m_sFixMsg[1] = _T("");

	General.bReadyDone = FALSE;
	General.nStepAuto = 0;
	General.nPrevMkStAuto = 0;
	General.nLotEndSerial = 0;
	General.bCam = FALSE;
	General.nStepMk[0] = 0;
	General.nStepMk[1] = 0;
	General.nStepMk[2] = 0;
	General.nStepMk[3] = 0;
	General.nMkPcs[0] = 0;
	General.nMkPcs[1] = 0;
	General.nMkPcs[2] = 0;
	General.nMkPcs[3] = 0;
	General.bWaitPcr[0] = FALSE;
	General.bWaitPcr[1] = FALSE;
	for (a = 0; a < 2; a++)
	{
		for (b = 0; b < MAX_STRIP; b++)
		{
			General.nMkStrip[a][b] = 0;
			General.bRejectDone[a][b] = FALSE;
		}
	}
	pView->DispLotEndSerial(0);

	//m_bChkLastProcVs = FALSE;
	//m_nDummy[0] = 0;
	//m_nDummy[1] = 0;
	//m_nAoiLastSerial[0] = 0;
	//m_nAoiLastSerial[1] = 0;
	//m_nPrevStepAuto = 0;
	//m_bAoiLdRun = TRUE;
	//m_bAoiLdRunF = FALSE;
	//m_bNewModel = FALSE;
	//m_bReview = FALSE;
	//m_bChkBufIdx[0] = TRUE;
	//m_bChkBufIdx[0] = TRUE;
	//m_nErrCnt = 0;

	//Thread.bTHREAD_MK[0] = FALSE;
	//Thread.bTHREAD_MK[1] = FALSE;
	//Thread.bTHREAD_MK[2] = FALSE;
	//Thread.bTHREAD_MK[3] = FALSE;

	//m_bMkTmpStop = FALSE;



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

	//pView->m_mgrPunch->ResetMkFdOffset();
	//for (nCam = 0; nCam < 2; nCam++)
	//{
	//	for (nPoint = 0; nPoint < 4; nPoint++)
	//	{
	//		m_pDlgMenu02->m_dMkFdOffsetX[nCam][nPoint] = 0.0;
	//		m_pDlgMenu02->m_dMkFdOffsetY[nCam][nPoint] = 0.0;
	//	}
	//}

	//pView->m_mgrPunch->ResetAoiUpFdOffset();
	//pView->m_mgrPunch->ResetAoiDnFdOffset();
	//m_pDlgMenu02->m_dAoiUpFdOffsetX = 0.0;
	//m_pDlgMenu02->m_dAoiUpFdOffsetY = 0.0;
	//m_pDlgMenu02->m_dAoiDnFdOffsetX = 0.0;
	//m_pDlgMenu02->m_dAoiDnFdOffsetY = 0.0;

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
		General.bLastProcFromUp = TRUE;
	else
		General.bLastProcFromEng = TRUE;

	m_nLastProcAuto = 0;

	pView->ClrAlarm();

	m_dwCycSt = 0;
	//m_sNewLotUp = _T("");
	//m_sNewLotDn = _T("");
	//m_nStop = 0;
	
	SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("0"));

	pView->MpeWrite(_T("MB440100"), 0); // PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
	pView->MpeWrite(_T("MB440110"), 0); // 마킹시작(PC가 확인하고 Reset시킴.)-20141029
	pView->MpeWrite(_T("MB440150"), 0); // 마킹부 마킹중 ON (PC가 ON, OFF)
	pView->MpeWrite(_T("MB440170"), 0); // 마킹완료(PLC가 확인하고 Reset시킴.)-20141029

	InitAutoEngSignal();

	MoveInitPos1();
	Sleep(30);
	MoveInitPos0();

	InitIoWrite();
	SetTest(FALSE);	// 검사부 상/하 검사 시작 (Off)

	if (pView->m_pDlgMenu01)
	{
		pView->m_pDlgMenu01->Reset();
	}

	//Thread.nStepTHREAD_DISP_DEF = 0;
	//Thread.bTHREAD_DISP_DEF = FALSE;				// CopyDefImg Stop
	//Thread.nStepTHREAD_DISP_DEF_INNER = 0;
	//Thread.bTHREAD_DISP_DEF_INNER = FALSE;		// DispDefImg Stop

	//Thread.bTHREAD_UPDATAE_YIELD[0] = FALSE;
	//Thread.bTHREAD_UPDATAE_YIELD[1] = FALSE;
	//m_nSerialTHREAD_UPDATAE_YIELD[0] = 0;
	//m_nSerialTHREAD_UPDATAE_YIELD[1] = 0;

	//Thread.bTHREAD_UPDATE_REELMAP_UP = FALSE;
	//Thread.bTHREAD_UPDATE_REELMAP_ALLUP = FALSE;
	//Thread.bTHREAD_UPDATE_REELMAP_DN = FALSE;
	//Thread.bTHREAD_UPDATE_REELMAP_ALLDN = FALSE;
	//Thread.bTHREAD_UPDATE_REELMAP_ITS = FALSE;
	//Thread.bTHREAD_MAKE_ITS_FILE_UP = FALSE;
	//Thread.bTHREAD_MAKE_ITS_FILE_DN = FALSE;

	//Thread.bTHREAD_REELMAP_YIELD_UP = FALSE;
	//Thread.bTHREAD_REELMAP_YIELD_ALLUP = FALSE;
	//Thread.bTHREAD_REELMAP_YIELD_DN = FALSE;
	//Thread.bTHREAD_REELMAP_YIELD_ALLDN = FALSE;
	//Thread.bTHREAD_REELMAP_YIELD_ITS = FALSE;

	//m_nSnTHREAD_UPDATAE_YIELD = 0;

	General.nEjectBufferLastShot = -1;
	General.bSerialDecrese = FALSE;
	General.bStopF_Verify = TRUE;
	General.bInitAuto = TRUE;
	General.bInitAutoLoadMstInfo = TRUE;

	if (bInit) // 이어가기가 아닌경우.
	{
		pView->MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
		pView->DispLotEnd(FALSE);

		//m_nRstNum = 0;
		General.bCont = FALSE;
		m_dTotVel = 0.0;
		m_dPartVel = 0.0;
		m_dwCycSt = 0;
		m_dwCycTim = 0;

		//pDoc->m_nPrevSerial = 0;
		//pDoc->m_bNewLotShare[0] = FALSE;
		//pDoc->m_bNewLotShare[1] = FALSE;
		//pDoc->m_bNewLotBuf[0] = FALSE;
		//pDoc->m_bNewLotBuf[1] = FALSE;
		//pDoc->m_bDoneChgLot = FALSE;

		//m_pDlgFrameHigh->m_nMkLastShot = 0;
		//m_pDlgFrameHigh->m_nAoiLastShot[0] = 0;
		//m_pDlgFrameHigh->m_nAoiLastShot[1] = 0;

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->ResetLotTime();

		ClrMkInfo();

		if (pView->m_pDlgFrameHigh)
		{
			pView->m_pDlgFrameHigh->Reset();
			//m_pDlgFrameHigh->SetMkLastShot(0);
			//m_pDlgFrameHigh->SetAoiLastShot(0, 0);
			//m_pDlgFrameHigh->SetAoiLastShot(1, 0);
			//m_pDlgFrameHigh->SetEngraveLastShot(0);
		}
	}
	else
	{
		if (pDoc->GetTestMode() == MODE_INNER && pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == pView->MsgBox(_T("각인부에서 레이저 각인부터 시작하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				pView->MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					pView->MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
				else
				{
					pView->MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
			}
		}
		else if (pDoc->GetTestMode() == MODE_INNER && !pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == pView->MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				pView->MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("각인부에서 레이저 각인부터 시작하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					pView->MpeWrite(_T("MB440187"), 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
				else
				{
					pView->MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
			}
		}
		else
		{
			pView->MpeWrite(_T("MB440187"), 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
		}

		if(pView->m_mgrReelmap)
			pView->m_mgrReelmap->ClrFixPcs();
		//if (pDoc->m_pReelMap)
		//	pDoc->m_pReelMap->ClrFixPcs();
		//if (pDoc->m_pReelMapUp)
		//	pDoc->m_pReelMapUp->ClrFixPcs();
		//
		//if (bDualTest)
		//{
		//	if (pDoc->m_pReelMapDn)
		//		pDoc->m_pReelMapDn->ClrFixPcs();
		//	if (pDoc->m_pReelMapAllUp)
		//		pDoc->m_pReelMapAllUp->ClrFixPcs();
		//	if (pDoc->m_pReelMapAllDn)
		//		pDoc->m_pReelMapAllDn->ClrFixPcs();
		//}
		//
		//if (pDoc->GetTestMode() == MODE_OUTER)
		//{
		//	if (pDoc->m_pReelMapInnerUp)
		//		pDoc->m_pReelMapInnerUp->ClrFixPcs();
		//
		//	if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		//	{
		//		if (pDoc->m_pReelMapInnerDn)
		//			pDoc->m_pReelMapInnerDn->ClrFixPcs();
		//		if (pDoc->m_pReelMapInnerAllUp)
		//			pDoc->m_pReelMapInnerAllUp->ClrFixPcs();
		//		if (pDoc->m_pReelMapInnerAllDn)
		//			pDoc->m_pReelMapInnerAllDn->ClrFixPcs();
		//	}
		//}

#ifndef TEST_MODE
		if (pView->m_mgrReelmap)
			pView->m_mgrReelmap->ReloadReelmap();
		UpdateRst();
#endif
		pView->DispLotStTime();
		if (pView->m_mgrReelmap)
			pView->m_mgrReelmap->RestoreReelmap();
	}
}

void CManagerProcedure::Auto()
{
	DoAuto();
}

void CManagerProcedure::DoAuto()
{
	if (!pView->m_mgrStatus)	return;
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
			DispMain(_T("작업종료"), RGB_RED);
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
			DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DOMARK1:
			m_bDispMsgDoAuto[9] = FALSE;
			m_nStepDispMsg[9] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			DispMain(_T("정 지"), RGB_RED);
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
			DispMain(_T("정 지"), RGB_RED);
			pView->MsgBox(m_sFixMsg[0]);
			m_sFixMsg[0] = _T("");
			break;
		case FROM_DISPDEFIMG + 3: // IsFixDn
			m_bDispMsgDoAuto[3] = FALSE;
			m_nStepDispMsg[3] = 0;
			pView->Buzzer(TRUE, 0);
			//TowerLamp(RGB_RED, TRUE);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			DispMain(_T("정 지"), RGB_RED);
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
			DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DISPDEFIMG + 7:
			m_bDispMsgDoAuto[7] = FALSE;
			m_nStepDispMsg[7] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			DispMain(_T("정 지"), RGB_RED);
			break;
		}
	}
}

void CManagerProcedure::DoAutoChkShareVsFolder()	// 잔량처리 시 계속적으로 반복해서 이함수가 호출됨으로 좌우 마킹 인덱스 동일 현상 발생.(case AT_LP + 8:)
{
	if (!pView->m_mgrStatus || pView->m_mgrFeeding)		return;
	if (!pView->m_mgrReelmap)							return;

	CCamMaster* pMaster = (pView->m_mgrReelmap->m_Master);
	CCamMaster* pMasterInner = (pView->m_mgrReelmap->m_MasterInner);
	stGeneral& General = (pView->m_mgrStatus->General);
	stBtnPush& BtnPush = (pView->m_mgrFeeding->Btn);
	stListBuf* ListBuf = pView->m_mgrStatus->ListBuf;

	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = 0;
	//CString sNewLot;
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
			General.nStepAuto++;
		}
		break;
	case 2:
		if (IsRun())
			General.nStepAuto++;
		break;
	case 3:
		pView->ClrDispMsg();
		General.nStepAuto++;
		break;
	case 4:
		if (IsRun())		// 초기운전
		{
			if (!IsAoiLdRun())
			{
				Stop();
			}
			else
			{
				ResetWinker();
				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (pDoc->WorkingInfo.LastJob.bDualTest)
					{
						if (GetDispMain() != _T("양면샘플"))
							DispMain(_T("양면샘플"), RGB_GREEN);
					}
					else
					{
						if (GetDispMain() != _T("단면샘플"))
							DispMain(_T("단면샘플"), RGB_GREEN);
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (GetDispMain() != _T("내층검사"))
						DispMain(_T("내층검사"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (GetDispMain() != _T("외층검사"))
						DispMain(_T("외층검사"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (GetDispMain() != _T("양면검사"))
						DispMain(_T("양면검사"), RGB_GREEN);
				}
				else
				{
					if (GetDispMain() != _T("단면검사"))
						DispMain(_T("단면검사"), RGB_GREEN);
				}

				SetListBuf();

				if (MODE_INNER == pDoc->GetTestMode() || MODE_OUTER == pDoc->GetTestMode()) // Please modify for outer mode.-20221226
				{
					pDoc->GetCurrentInfoEng();
					if (pView->m_pDlgMenu01)
						pView->m_pDlgMenu01->UpdateData();
				}

				General.nStepAuto = AT_LP;
			}
		}
		else
			Winker(MN_RUN);
		break;

	case AT_LP + (AtLpVsIdx::ChkShare) :
		if (IsShare()) // ChkShare()
		{
			m_bPcrInShare[0] = FALSE;
			m_bPcrInShare[1] = FALSE;

			if (IsShareUp()) // 검사부(상) 검사중
			{
				nSerial = GetShareUp();
				if (nSerial > 0)
				{
					if (General.bSerialDecrese)
					{
						if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
						{
							pDoc->DelSharePcrUp();	// Delete PCR File
						}
						else
						{
							m_bPcrInShare[0] = TRUE;
						}
					}
					else
					{
						if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
						{							
							pDoc->DelSharePcrUp();	// Delete PCR File
						}
						else
						{
							m_bPcrInShare[0] = TRUE;
						}
					}
				}
			}

			if (bDualTest)
			{
				if (IsShareDn()) // 검사부(하) 검사중
				{
					nSerial = GetShareDn();
					if (nSerial > 0)
					{
						if (General.bSerialDecrese)
						{
							if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
							{
								
								pDoc->DelSharePcrDn();	// Delete PCR File
							}
							else
							{
								m_bPcrInShare[1] = TRUE;
							}
						}
						else
						{
							if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
							{								
								pDoc->DelSharePcrDn();	// Delete PCR File
							}
							else
							{
								m_bPcrInShare[1] = TRUE;
							}
						}
					}
				}

				if (m_bPcrInShare[0] || m_bPcrInShare[1])
					General.nStepAuto++;
			}
			else
			{
				if (m_bPcrInShare[0])
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
		General.nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ChkShare) + 2:
		General.nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ChkShare) + 3:
		Shift2DummyBuf();			// PCR 이동(Share->Buffer)
		General.nStepAuto = AT_LP + (AtLpVsIdx::ChkShare);
		break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) :
		if (IsShareVs()) // ChkShareVs()
		{
			m_bPcrInShareVs[0] = FALSE;
			m_bPcrInShareVs[1] = FALSE;

			if (IsShareVsUp()) // 검사부(상) 검사중
			{
				nSerial = GetShareVsUp();
				if (nSerial > 0)
				{
					if (General.bSerialDecrese)
					{
						if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
						{						
							pDoc->DelShareVsPcrUp();	// Delete PCR File
						}
						else
						{
							m_nShareUpS = nSerial;
							m_bPcrInShareVs[0] = TRUE;
						}
					}
					else
					{
						if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
						{						
							pDoc->DelShareVsPcrUp();	// Delete PCR File
						}
						else
						{
							m_nShareUpS = nSerial;
							m_bPcrInShareVs[0] = TRUE;
						}
					}
				}
			}

			if (bDualTest)
			{
				if (IsShareVsDn()) // 검사부(하) 검사중
				{
					nSerial = GetShareVsDn();
					if (nSerial > 0)
					{
						if (General.bSerialDecrese)
						{
							if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
							{							
								pDoc->DelShareVsPcrDn();	// Delete PCR File
							}
							else
							{
								m_nShareDnS = nSerial;
								m_bPcrInShareVs[1] = TRUE;
							}
						}
						else
						{
							if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
							{							
								pDoc->DelShareVsPcrDn();	// Delete PCR File
							}
							else
							{
								m_nShareDnS = nSerial;
								m_bPcrInShareVs[1] = TRUE;
							}
						}
					}
				}

				if (m_bPcrInShareVs[0] || m_bPcrInShareVs[1])
					General.nStepAuto++;
			}
			else
			{
				if (m_bPcrInShareVs[0])
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
								nSerial = ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = ListBuf[1].GetLast();
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
								nSerial = ListBuf[0].GetLast();
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

				General.bWaitPcr[0] = FALSE; // 향후 제거해야 할 flag
				General.bWaitPcr[1] = FALSE;
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
		if (!General.bCont) // 이어가기 아닌 경우.
		{
			if (!ChkStShotNum())
			{
				Stop();
			}
		}
		General.nStepAuto++;
		break;
	case AT_LP + (AtLpVsIdx::ChkShareVs) + 2:
		//m_bBufEmpty[0] = pDoc->m_bBufEmpty[0]; // Up
		General.nStepAuto++;
	break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) + 3:
		if (Thread.bTHREAD_UPDATE_REELMAP_UP || Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (Thread.bTHREAD_UPDATE_YIELD_UP || Thread.bTHREAD_UPDATE_YIELD_DN || Thread.bTHREAD_UPDATE_YIELD_ALLUP || Thread.bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		Shift2Buf();			// PCR 이동(Share->Buffer)
		General.nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) :
		if (Thread.bTHREAD_UPDATE_REELMAP_UP || Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (Thread.bTHREAD_UPDATE_YIELD_UP || Thread.bTHREAD_UPDATE_YIELD_DN || Thread.bTHREAD_UPDATE_YIELD_ALLUP || Thread.bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (Thread.bTHREAD_MAKE_ITS_FILE_UP) // Write Reelmap
				break;

			if (bDualTest)
			{
				if (Thread.bTHREAD_MAKE_ITS_FILE_DN)
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
					GetItsSerialInfo(m_nShareUpS, bDualTestInner, sLot, sLayerUp, sLayerDn, 0);
			}

			bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer에서 PCR파일의 헤드 정보를 얻음.

			if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//m_bNewModel = TRUE;
				DispInfo();
				ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	

				pDoc->GetCamPxlRes();

				if (IsLastJob(0)) // Up
				{
					pMaster[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
						pDoc->WorkingInfo.LastJob.sModelUp,
						pDoc->WorkingInfo.LastJob.sLayerUp);
					pMaster[0].LoadMstInfo();

					if (m_pEngrave)
						m_pEngrave->SwMenu01UpdateWorking(TRUE);


					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						pMasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModelUp,
							pDoc->WorkingInfo.LastJob.sInnerLayerUp);
						pMasterInner[0].LoadMstInfo();
					}
				}

				if (IsLastJob(1)) // Dn
				{
					pMaster[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
						pDoc->WorkingInfo.LastJob.sModelDn,
						pDoc->WorkingInfo.LastJob.sLayerDn,
						pDoc->WorkingInfo.LastJob.sLayerUp);
					pMaster[1].LoadMstInfo();

					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						pMasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModelUp,
							pDoc->WorkingInfo.LastJob.sInnerLayerDn,
							pDoc->WorkingInfo.LastJob.sInnerLayerUp);
						pMasterInner[0].LoadMstInfo();
					}
				}

				SetAlignPos();
				InitReelmap();

				if (pMaster[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp))
					InitReelmapInner();

				ModelChange(0); // 0 : AOI-Up , 1 : AOI-Dn
				InitReelmapDisp();
				//if (m_pDlgMenu01)
				//{
				//	m_pDlgMenu01->InitGL();
				//	m_bDrawGL_Menu01 = TRUE;
				//	m_pDlgMenu01->RefreshRmap();
				//	m_pDlgMenu01->InitCadImg();
				//	m_pDlgMenu01->SetPnlNum();
				//	m_pDlgMenu01->SetPnlDefNum();
				//}

				InitCamImgDisp();
				//if (m_pDlgMenu02)
				//{
				//	m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
				//	m_pDlgMenu02->InitCadImg();
				//}

				//if (pDoc->GetTestMode() == MODE_OUTER)
				//{
				//	if (m_pDlgMenu06)
				//	{
				//		m_pDlgMenu06->InitGL();
				//		m_bDrawGL_Menu06 = TRUE;
				//		m_pDlgMenu06->RefreshRmap();
				//		m_pDlgMenu06->InitCadImg();
				//		m_pDlgMenu06->SetPnlNum();
				//		m_pDlgMenu06->SetPnlDefNum();
				//	}
				//}
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
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (!bDualTest)
					OpenReelmapFromBuf(m_nShareUpS);
				//if (!pDoc->m_bNewLotShare[0])
				//{
				//	pDoc->m_bNewLotShare[0] = TRUE;// Lot Change.
				//	if (!bDualTest)
				//		OpenReelmapFromBuf(m_nShareUpS);
				//}
			}

			LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share

			if (ListBuf[0].nTot <= ListBuf[1].nTot)
			{
				UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬
			}

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
								nSerial = ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = ListBuf[1].GetLast();
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
								nSerial = ListBuf[0].GetLast();
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
		if (!bDualTest)
		{
			General.nStepAuto++;
			break;
		}

		if (Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (Thread.bTHREAD_REELMAP_YIELD_DN || Thread.bTHREAD_REELMAP_YIELD_ALLUP || Thread.bTHREAD_REELMAP_YIELD_ALLDN) // Write Reelmap
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


			bNewModel = pDoc->GetAoiDnInfo(m_nShareDnS, &nNewLot);

			if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//MsgBox(_T("신규 모델에 의한 AOI(하)에서 로트 분리가 되었습니다.\r\n이전 로트를 잔량처리 합니다.");
				DispInfo();
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
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (bDualTest)
					OpenReelmapFromBuf(m_nShareDnS);
				//if (!pDoc->m_bNewLotShare[1])
				//{
				//	pDoc->m_bNewLotShare[1] = TRUE;// Lot Change.				
				//	if (bDualTest)
				//		OpenReelmapFromBuf(m_nShareDnS);
				//}
			}

			LoadPcrDn(m_nShareDnS);

			if (ListBuf[1].nTot <= ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // 시리얼파일의 정보로 릴맵을 만듬
			}

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
							nSerial = ListBuf[0].GetLast();
					}
					else
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = ListBuf[1].GetLast();
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
		if (bDualTest)
		{
			if (Thread.bTHREAD_UPDATE_REELMAP_UP || Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				break;
		}
		else
		{
			if (Thread.bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
				break;
		}
		General.nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::MakeItsFile) :
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
		break;

	case AT_LP + (AtLpVsIdx::MakeItsFile) + 1:
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (Thread.bTHREAD_MAKE_ITS_FILE_UP) // makeIts
				break;

			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (Thread.bTHREAD_MAKE_ITS_FILE_DN) // makeIts
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
		General.nStepAuto = AT_LP + (AtLpVsIdx::ChkShare);
		break;
	}
}

void CManagerProcedure::DoAutoChkShareFolder()
{
	if (!pView->m_mgrStatus || pView->m_mgrFeeding)		return;
	if (!pView->m_mgrReelmap)							return;

	CCamMaster* pMaster = (pView->m_mgrReelmap->m_Master);
	CCamMaster* pMasterInner = (pView->m_mgrReelmap->m_MasterInner);
	stGeneral& General = (pView->m_mgrStatus->General);
	stBtnPush& BtnPush = (pView->m_mgrFeeding->Btn);
	stListBuf* ListBuf = pView->m_mgrStatus->ListBuf;

	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = 0;
	//CString sNewLot;
	int nNewLot = 0;
	//BOOL m_bPcrInShare[2];
	BOOL bNewModel = FALSE;

	switch (General.nStepAuto)
	{
	case 0:
		BtnPush.bSwRun = FALSE;
		BtnPush.bSwStop = TRUE;
		General.nStepAuto++;
		break;
	case 1:
		if (pView->IsReady() || pView->IsAuto())		// 운전준비
		{
			General.nStepAuto++;
		}
		break;
	case 2:
		if (IsRun())
			General.nStepAuto++;
		break;
	case 3:
		pView->ClrDispMsg();
		General.nStepAuto++;
		break;
	case 4:
		if (IsRun())		// 초기운전
		{
			if (!IsAoiLdRun())
			{
				Stop();
			}
			else
			{
				ResetWinker(); // 20151126 : 운전스위치 램프 동작 문제로 수정.

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (pDoc->WorkingInfo.LastJob.bDualTest)
					{
						if (GetDispMain() != _T("양면샘플"))
							DispMain(_T("양면샘플"), RGB_GREEN);
					}
					else
					{
						if (GetDispMain() != _T("단면샘플"))
							DispMain(_T("단면샘플"), RGB_GREEN);
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (GetDispMain() != _T("내층검사"))
						DispMain(_T("내층검사"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (GetDispMain() != _T("외층검사"))
						DispMain(_T("외층검사"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (GetDispMain() != _T("양면검사"))
						DispMain(_T("양면검사"), RGB_GREEN);
				}
				else
				{
					if (GetDispMain() != _T("단면검사"))
						DispMain(_T("단면검사"), RGB_GREEN);
				}

				SetListBuf();

				if (MODE_INNER == pDoc->GetTestMode() || MODE_OUTER == pDoc->GetTestMode()) // Please modify for outer mode.-20221226
				{
					pDoc->GetCurrentInfoEng();
					if (pView->m_pDlgMenu01)
						pView->m_pDlgMenu01->UpdateData();
				}

				General.nStepAuto = AT_LP;
			}
		}
		else
			Winker(MN_RUN);
		break;

	case AT_LP:
		if (IsShare()) // ChkShare()
		{
			m_bPcrInShare[0] = FALSE;
			m_bPcrInShare[1] = FALSE;

			if (IsShareUp())
			{
				nSerial = GetShareUp();
				if (nSerial > 0)
				{
					if (General.bSerialDecrese)
					{
						if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
						{						
							pDoc->DelSharePcrUp();	// Delete PCR File
						}
						else
						{
							m_nShareUpS = nSerial;
							m_bPcrInShare[0] = TRUE;
						}
					}
					else
					{
						if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
						{						
							pDoc->DelSharePcrUp();	// Delete PCR File
						}
						else
						{
							m_nShareUpS = nSerial;
							m_bPcrInShare[0] = TRUE;
						}
					}
				}
			}


			if (bDualTest)
			{
				if (IsShareDn())
				{
					nSerial = GetShareDn();
					if (nSerial > 0)
					{
						if (General.bSerialDecrese)
						{
							if (General.nLotEndSerial > 0 && nSerial < General.nLotEndSerial)
							{							
								pDoc->DelSharePcrDn();	// Delete PCR File
							}
							else
							{
								m_nShareDnS = nSerial;
								m_bPcrInShare[1] = TRUE;
							}
						}
						else
						{
							if (General.nLotEndSerial > 0 && nSerial > General.nLotEndSerial)
							{							
								pDoc->DelSharePcrDn();	// Delete PCR File
							}
							else
							{
								m_nShareDnS = nSerial;
								m_bPcrInShare[1] = TRUE;
							}
						}
					}
				}

				if (m_bPcrInShare[0] || m_bPcrInShare[1])
					General.nStepAuto++;
			}
			else
			{
				if (m_bPcrInShare[0])
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
								nSerial = ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = ListBuf[1].GetLast();
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
								nSerial = ListBuf[0].GetLast();
						}
					}

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}

					General.nStepAuto++;					
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

				General.bWaitPcr[0] = FALSE;
				General.bWaitPcr[1] = FALSE;
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

	case AT_LP + 1:
		if (!General.bCont) // 이어가기 아닌 경우.
		{
			if (!ChkStShotNum())
			{
				Stop();
			}
		}
		General.nStepAuto++;
		break;
	case AT_LP + 2:
		if (IsRun())
		{
			//m_bBufEmpty[0] = pDoc->m_bBufEmpty[0]; // Up
			General.nStepAuto++;
		}
		break;

	case AT_LP + 3:
		if (Thread.bTHREAD_UPDATE_REELMAP_UP || Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (Thread.bTHREAD_UPDATE_YIELD_UP || Thread.bTHREAD_UPDATE_YIELD_DN || Thread.bTHREAD_UPDATE_YIELD_ALLUP || Thread.bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
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

		if (Thread.bTHREAD_UPDATE_REELMAP_UP || Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (Thread.bTHREAD_UPDATE_YIELD_UP || Thread.bTHREAD_UPDATE_YIELD_DN || Thread.bTHREAD_UPDATE_YIELD_ALLUP || Thread.bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (Thread.bTHREAD_MAKE_ITS_FILE_UP)
				break;

			if (bDualTest)
			{
				if (Thread.bTHREAD_MAKE_ITS_FILE_DN)
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
					GetItsSerialInfo(m_nShareUpS, bDualTestInner, sLot, sLayerUp, sLayerDn, 0);
			}

			bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer에서 PCR파일의 헤드 정보를 얻음.

			if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//m_bNewModel = TRUE;
				DispInfo();
				ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	

				pDoc->GetCamPxlRes();

				if (IsLastJob(0)) // Up
				{
					pMaster[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
						pDoc->WorkingInfo.LastJob.sModelUp,
						pDoc->WorkingInfo.LastJob.sLayerUp);
					pMaster[0].LoadMstInfo();

					if (m_pEngrave)
						m_pEngrave->SwMenu01UpdateWorking(TRUE);

					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						pMasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModelUp,
							pDoc->WorkingInfo.LastJob.sInnerLayerUp);
						pMasterInner[0].LoadMstInfo();
					}
				}

				if (IsLastJob(1)) // Dn
				{
					pMaster[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
						pDoc->WorkingInfo.LastJob.sModelDn,
						pDoc->WorkingInfo.LastJob.sLayerDn,
						pDoc->WorkingInfo.LastJob.sLayerUp);
					pMaster[1].LoadMstInfo();

					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						pMasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModelUp,
							pDoc->WorkingInfo.LastJob.sInnerLayerDn,
							pDoc->WorkingInfo.LastJob.sInnerLayerUp);
						pMasterInner[0].LoadMstInfo();
					}
				}

				SetAlignPos();
				InitReelmap();

				if (pMaster[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp))
					InitReelmapInner();

				ModelChange(0); // 0 : AOI-Up , 1 : AOI-Dn
				InitReelmapDisp();
				//if (m_pDlgMenu01)
				//{
				//	m_pDlgMenu01->InitGL();
				//	m_bDrawGL_Menu01 = TRUE;
				//	m_pDlgMenu01->RefreshRmap();
				//	m_pDlgMenu01->InitCadImg();
				//	m_pDlgMenu01->SetPnlNum();
				//	m_pDlgMenu01->SetPnlDefNum();
				//}

				InitCamImgDisp();
				//if (m_pDlgMenu02)
				//{
				//	m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
				//	m_pDlgMenu02->InitCadImg();
				//}

				//if (pDoc->GetTestMode() == MODE_OUTER) // syd-20231121
				//{
				//	if (m_pDlgMenu06)
				//	{
				//		m_pDlgMenu06->InitGL();
				//		m_bDrawGL_Menu06 = TRUE;
				//		m_pDlgMenu06->RefreshRmap();
				//		m_pDlgMenu06->InitCadImg();
				//		m_pDlgMenu06->SetPnlNum();
				//		m_pDlgMenu06->SetPnlDefNum();
				//	}
				//}

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
						break;

					}
				}
			}

			if (nNewLot)
			{
				if (!bDualTest)
					OpenReelmapFromBuf(m_nShareUpS);
				//if (!pDoc->m_bNewLotShare[0])
				//{
				//	pDoc->m_bNewLotShare[0] = TRUE;// Lot Change.
				//	if (!bDualTest)
				//		OpenReelmapFromBuf(m_nShareUpS);
				//}
			}

			LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share

			if (ListBuf[0].nTot <= ListBuf[1].nTot)
			{
				UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬
			}

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
								nSerial = ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = ListBuf[1].GetLast();
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
								nSerial = ListBuf[0].GetLast();
						}
					}

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
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

	case AT_LP + 5:
		if (!IsRun())
			break;

		if (!bDualTest)
		{
			General.nStepAuto++;
			break;
		}

		if (Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (Thread.bTHREAD_REELMAP_YIELD_DN || Thread.bTHREAD_REELMAP_YIELD_ALLUP || Thread.bTHREAD_REELMAP_YIELD_ALLDN) // Write Reelmap
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


			bNewModel = pDoc->GetAoiDnInfo(m_nShareDnS, &nNewLot);

			if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//MsgBox(_T("신규 모델에 의한 AOI(하)에서 로트 분리가 되었습니다.\r\n이전 로트를 잔량처리 합니다.");
				DispInfo();
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
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (bDualTest)
					OpenReelmapFromBuf(m_nShareDnS);
				//if (!pDoc->m_bNewLotShare[1])
				//{
				//	pDoc->m_bNewLotShare[1] = TRUE;// Lot Change.				
				//	if (bDualTest)
				//		OpenReelmapFromBuf(m_nShareDnS);
				//}
			}

			LoadPcrDn(m_nShareDnS);

			if (ListBuf[1].nTot <= ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // 시리얼파일의 정보로 릴맵을 만듬
			}

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
							nSerial = ListBuf[0].GetLast();
					}
					else
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = ListBuf[1].GetLast();
					}

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
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

	case AT_LP + 7:
		if (IsRun())
		{
			if (bDualTest)
			{
				if (Thread.bTHREAD_UPDATE_REELMAP_UP || Thread.bTHREAD_UPDATE_REELMAP_DN || Thread.bTHREAD_UPDATE_REELMAP_ALLUP || Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
					break;
			}
			else
			{
				if (Thread.bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
					break;
			}
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
				}
				else
				{
					if (m_nShareDnS > 0)
						MakeItsFile(m_nShareDnS);
				}
			}

			General.nStepAuto++;
		}
		break;

	case AT_LP + 9:
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (Thread.bTHREAD_MAKE_ITS_FILE_UP) // makeIts
				break;

			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (Thread.bTHREAD_MAKE_ITS_FILE_DN) // makeIts
					break;
			}
		}
		General.nStepAuto++;
		break;

	case AT_LP + 10:
		m_nShareUpS = 0;
		m_nShareDnS = 0;
		General.nStepAuto++;
		break;

	case AT_LP + 11:
		General.nStepAuto = AT_LP;
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

	//if (BtnStatus.EngAuto.IsOnMking && !(m_pMpe->m_pMpeSignal[6] & (0x01 << 3))) // 각인부 마킹중 ON (PC가 ON, OFF)
	if (BtnStatus.EngAuto.IsOnMking && !(pView->m_mgrFeeding->GetMpeSignal(6, 3))) // 각인부 마킹중 ON (PC가 ON, OFF)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, TRUE);
		pDoc->LogAuto(_T("PC: 2D(GUI) 각인 동작Running신호 ON (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440173"), 1); // 2D(GUI) 각인 동작Running신호(PC On->PC Off)
	}
	else if (!BtnStatus.EngAuto.IsOnMking && (pView->m_mgrFeeding->GetMpeSignal(6, 3)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
		pDoc->LogAuto(_T("PC: 2D(GUI) 각인 동작Running신호 OFF (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440173"), 0); // 2D(GUI) 각인 동작Running신호(PC On->PC Off)
	}

	// 각인부 마킹완료 ON (PC가 ON, OFF)
	if (BtnStatus.EngAuto.IsMkDone && !(pView->m_mgrFeeding->GetMpeSignal(6, 4))) // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, TRUE);
		pDoc->LogAuto(_T("PC: 각인부 작업완료 ON (PC가 On, PLC가 확인 후 Off)"));
		pView->MpeWrite(_T("MB440174"), 1); // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
	}
	else if (!BtnStatus.EngAuto.IsMkDone && (pView->m_mgrFeeding->GetMpeSignal(6, 4))) // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, FALSE);
	}

	// 각인부 2D 리더 작업중 신호
	if (BtnStatus.EngAuto.IsOnRead2d && !(pView->m_mgrFeeding->GetMpeSignal(6, 8))) // 각인부 2D 리더 작업중 신호(PC On->PC Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, TRUE);
		pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업중 신호 ON (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440178"), 1); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
	}
	else if (!BtnStatus.EngAuto.IsOnRead2d && (pView->m_mgrFeeding->GetMpeSignal(6, 8)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
		pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업중 신호 OFF (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440178"), 0); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
	}

	// 각인부 2D 리더 작업완료 신호
	if (BtnStatus.EngAuto.IsRead2dDone && !(pView->m_mgrFeeding->GetMpeSignal(6, 9))) // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, TRUE);
		pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업완료 신호 ON (PC가 On, PLC가 확인 후 Off)"));
		pView->MpeWrite(_T("MB440179"), 1); // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
	}
	else if (!BtnStatus.EngAuto.IsRead2dDone && (pView->m_mgrFeeding->GetMpeSignal(6, 9)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, FALSE);
	}

}

BOOL CManagerProcedure::IsLotEnd()
{
	return m_bLotEnd;
}

BOOL CManagerProcedure::ReloadReelmap(int nSerial)
{
	if (!pView->m_mgrReelmap)	return FALSE;
	return pView->m_mgrReelmap->ReloadReelmap(nSerial);
}

void CManagerProcedure::UpdateRst()
{
	pView->UpdateRst();
}

void CManagerProcedure::LotEnd()
{
	stGeneral& General = (pView->m_mgrStatus->General);

	General.bCont = FALSE;
	pView->SetLotEd();
	MakeResultMDS();

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pView->m_mgrReelmap)
			pView->m_mgrReelmap->StartThreadFinalCopyItsFiles();
	}

	//if (pDoc->m_pReelMap == pDoc->m_pReelMapUp)
	//{
	//	DuplicateRmap(RMAP_UP);
	//}
	//else if (pDoc->m_pReelMap == pDoc->m_pReelMapAllUp)
	//{
	//	DuplicateRmap(RMAP_ALLUP);
	//}
	//else if (pDoc->m_pReelMap == pDoc->m_pReelMapIts)
	//{
	//	DuplicateRmap(RMAP_ITS);
	//}

	//if (m_pEngrave)
	//	m_pEngrave->SwMenu01DispDefImg(TRUE);
}

BOOL CManagerProcedure::ChkLastProc()
{
	if (!pView->m_mgrStatus)
		return FALSE;

	stGeneral& General = (pView->m_mgrStatus->General);

	return General.bLastProc;
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

CString CManagerProcedure::GetAoiUpAlarmRestartMsg()									// m_sAoiUpAlarmReStartMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->GetAoiUpAlarmRestartMsg();
}

CString CManagerProcedure::GetAoiDnAlarmRestartMsg()									// m_sAoiDnAlarmReStartMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->GetAoiDnAlarmRestartMsg();
}

CString CManagerProcedure::GetAoiUpAlarmReTestMsg()									// m_sAoiUpAlarmReTestMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->GetAoiUpAlarmReTestMsg();
}

CString CManagerProcedure::GetAoiDnAlarmReTestMsg()									// m_sAoiDnAlarmReTestMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->GetAoiDnAlarmReTestMsg();
}

CString CManagerProcedure::ReadAoiUpAlarmRestartMsg()									// m_sAoiUpAlarmReStartMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->ReadAoiUpAlarmRestartMsg();
}

CString CManagerProcedure::ReadAoiDnAlarmRestartMsg()									// m_sAoiDnAlarmReStartMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->ReadAoiDnAlarmRestartMsg();
}

CString CManagerProcedure::ReadAoiUpAlarmReTestMsg()									// m_sAoiUpAlarmReTestMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->ReadAoiUpAlarmReTestMsg();
}

CString CManagerProcedure::ReadAoiDnAlarmReTestMsg()									// m_sAoiDnAlarmReTestMsg
{
	if (!pView->m_mgrFeeding)
		return _T("");
	return pView->m_mgrFeeding->ReadAoiDnAlarmReTestMsg();
}

void CManagerProcedure::SetCycTime()
{
	DWORD dCur = GetTickCount();
	if (m_dwCycSt > 0)
	{
		m_dwCycTim = (double)(dCur - m_dwCycSt);
		if (m_dwCycTim < 0.0)
			m_dwCycTim *= (-1.0);
	}
	else
		m_dwCycTim = 0.0;
}

int CManagerProcedure::GetCycTime()
{
	if (m_dwCycTim < 0)
		m_dwCycTim = 0;

	int nTim = int(m_dwCycTim);
	return nTim;
}

void CManagerProcedure::InitAutoEngSignal()
{
	stBtnStatus& BtnStatus = (m_pEngrave->BtnStatus);
	BtnStatus.EngAuto._Init();

	//m_bEngFdWrite = FALSE;
	m_bEngFdWriteF = FALSE;
	//m_bEngTest = FALSE;
	m_bEngTestF = FALSE;

	pView->MpeWrite(_T("MB440103"), 0); // 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
	pView->MpeWrite(_T("MB440173"), 0); // 2D(GUI) 각인 동작Running신호(PC On->PC Off)
	pView->MpeWrite(_T("MB440174"), 0); // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)

	pView->MpeWrite(_T("MB440105"), 0); // 각인부 2D 리더 시작신호(PLC On->PC Off)
	pView->MpeWrite(_T("MB440178"), 0); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
	pView->MpeWrite(_T("MB440179"), 0); // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)

	pView->MpeWrite(_T("MB440102"), 0); // 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)

	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, FALSE);
}

void CManagerProcedure::MoveInitPos0(BOOL bWait)
{
	if (pView->m_mgrPunch)
		pView->m_mgrPunch->MoveInitPos0(bWait);
}

void CManagerProcedure::MoveInitPos1(BOOL bWait)
{
	if (pView->m_mgrPunch)
		pView->m_mgrPunch->MoveInitPos1(bWait);
}

void CManagerProcedure::InitIoWrite()
{
	pView->MpeWrite(_T("MB44015E"), 0); // 부저1 On  (PC가 ON, OFF) - 20141020
	pView->MpeWrite(_T("MB44015F"), 0); // 부저2 On  (PC가 ON, OFF) - 20141020

	pView->MpeWrite(_T("MB003828"), 0); // 검사부 상 검사 시작 <-> Y4368 I/F
	pView->MpeWrite(_T("MB003829"), 0); // 검사부 상 검사 테이블 진공 SOL <-> Y4369 I/F
	pView->MpeWrite(_T("MB00382A"), 0); // 검사부 상 Reset <-> Y436A I/F
	pView->MpeWrite(_T("MB00382B"), 0); // 마킹부 Lot End <-> Y436B I/F

	pView->MpeWrite(_T("MB003928"), 0); // 검사부 하 검사 시작 <-> Y4468 I/F
	pView->MpeWrite(_T("MB003929"), 0); // 검사부 하 검사 테이블 진공 SOL <-> Y4369 I/F
	pView->MpeWrite(_T("MB00392A"), 0); // 검사부 하 Reset <-> Y436A I/F
	pView->MpeWrite(_T("MB00392B"), 0); // 마킹부 Lot End <-> Y436B I/F

	pView->MpeWrite(_T("MB44015D"), 0); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off)
	pView->MpeWrite(_T("ML45064"), 0); // 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
	pView->MpeWrite(_T("ML45066"), 0); // 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)

	pView->MpeWrite(_T("MB600000"), 0); // PC가 PLC의 Alarm 발생여부를 확인
	pView->MpeWrite(_T("MB600008"), 0); // PC가 PLC의 Alarm 발생여부를 확인
	pView->MpeWrite(_T("ML60002"), 0); // 알람이 발생한 페이지 지정(PLC가 표시 할 알람의 페이지를 가리킴).
}

void CManagerProcedure::SetTest(BOOL bOn)
{
	if (bOn)
	{
		pView->MpeWrite(_T("MB003828"), 1); // 검사부 상 검사 시작 <-> Y4368 I/F
		pView->MpeWrite(_T("MB003928"), 1); // 검사부 하 검사 시작 <-> Y4468 I/F
	}
	else
	{
		pView->MpeWrite(_T("MB003828"), 0); // 검사부 상 검사 시작 <-> Y4368 I/F
		pView->MpeWrite(_T("MB003928"), 0); // 검사부 하 검사 시작 <-> Y4468 I/F
	}
}


void CManagerProcedure::ClrMkInfo()
{
	pView->ClrMkInfo();
}

void CManagerProcedure::Stop()
{
	pView->Stop();
}

void CManagerProcedure::Winker(int nId, int nDly) // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	if (nId == MN_RUN)
		pView->MpeWrite(_T("MB44015D"), 1); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off)
}

void CManagerProcedure::ResetWinker() // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	pView->MpeWrite(_T("MB44015D"), 0);	// 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off)
}

CString CManagerProcedure::GetDispMain()
{
	return pView->GetDispMain();
}

void CManagerProcedure::DispMain(CString sMsg, COLORREF rgb = RGB(0, 255, 0))
{
	pView->DispMain(sMsg, rgb);
}

void CManagerProcedure::SetListBuf()
{
	pView->SetListBuf();
}

BOOL CManagerProcedure::IsShare()
{
	return pView->IsShare();
}

BOOL CManagerProcedure::IsShareUp()
{
	return pView->IsShareUp();
}

BOOL CManagerProcedure::IsShareDn()
{
	return pView->IsShareDn();
}

int CManagerProcedure::GetShareUp()
{
	return pView->GetShareUp();
}

int CManagerProcedure::GetShareDn()
{
	return pView->GetShareDn();
}

BOOL CManagerProcedure::IsShareVs()
{
	return pView->IsShareVs();
}

BOOL CManagerProcedure::IsShareVsUp()
{
	return pView->IsShareVsUp();
}

BOOL CManagerProcedure::IsShareVsDn()
{
	return pView->IsShareVsDn();
}

int CManagerProcedure::GetShareVsUp()
{
	return pView->GetShareVsUp();
}

int CManagerProcedure::GetShareVsDn()
{
	return pView->GetShareVsDn();
}

void CManagerProcedure::Shift2DummyBuf()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sSrc, sDest;
	int nSerial;

	sSrc = pDoc->WorkingInfo.System.sPathVrsShareUp;
	sDest = pDoc->WorkingInfo.System.sPathVsDummyBufUp;

	if (pDoc->m_pFile)
	{
		nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
		pDoc->m_pFile->DelPcr(sSrc, nSerial);
		if (nSerial > 0)
		{
			pView->MpeWrite(_T("ML45112"), (long)nSerial);	// 검사한 Panel의 AOI 상 Serial
			Sleep(300);
			pView->MpeWrite(_T("MB44012B"), 1); // AOI 상 : PCR파일 Received
		}
	}

	if (bDualTest)
	{
		sSrc = pDoc->WorkingInfo.System.sPathVrsShareDn;
		sDest = pDoc->WorkingInfo.System.sPathVsDummyBufDn;

		if (pDoc->m_pFile)
		{
			nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
			pDoc->m_pFile->DelPcr(sSrc, nSerial);
			if (nSerial > 0)
			{
				pView->MpeWrite(_T("ML45114"), (long)pDoc->GetAoiDnAutoSerial() - 1);	// 검사한 Panel의 AOI 하 Serial
				Sleep(300);
				pView->MpeWrite(_T("MB44012C"), 1); // AOI 하 : PCR파일 Received
			}
		}
	}
}

void CManagerProcedure::Shift2Buf()	// 버퍼폴더의 마지막 시리얼과 Share폴더의 시리얼이 연속인지 확인 후 옮김.
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sSrc, sDest;
	int nSerial;

	if (pDoc->GetAoiUpVsStatus())
	{
		sSrc = pDoc->WorkingInfo.System.sPathVsShareUp;
		sDest = pDoc->WorkingInfo.System.sPathVrsBufUp;
		if (pDoc->m_pFile)
		{
			nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
			if (nSerial > 0)
				m_nShareUpS = nSerial;
			pDoc->m_pFile->DelPcr(sSrc, nSerial);
		}
		if (bDualTest)
		{
			sSrc = pDoc->WorkingInfo.System.sPathVsShareDn;
			sDest = pDoc->WorkingInfo.System.sPathVrsBufDn;
			if (pDoc->m_pFile)
			{
				nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
				if (nSerial > 0)
					m_nShareDnS = nSerial;
				pDoc->m_pFile->DelPcr(sSrc, nSerial);
			}
		}

		SetListBuf();
	}
	else
	{
		sSrc = pDoc->WorkingInfo.System.sPathVrsShareUp;
		sDest = pDoc->WorkingInfo.System.sPathVrsBufUp;

		if (pDoc->m_pFile)
		{
			nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
			pDoc->m_pFile->DelPcr(sSrc, nSerial);
			if (nSerial > 0)
			{
				m_nShareUpS = nSerial;
				pView->MpeWrite(_T("ML45112"), (long)nSerial);	// 검사한 Panel의 AOI 상 Serial
				Sleep(300);
				pView->MpeWrite(_T("MB44012B"), 1); // AOI 상 : PCR파일 Received
			}
		}

		if (bDualTest)
		{
			sSrc = pDoc->WorkingInfo.System.sPathVrsShareDn;
			sDest = pDoc->WorkingInfo.System.sPathVrsBufDn;

			if (pDoc->m_pFile)
			{
				nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
				pDoc->m_pFile->DelPcr(sSrc, nSerial);
				if (nSerial > 0)
				{
					m_nShareDnS = nSerial;
					pView->MpeWrite(_T("ML45114"), (long)pDoc->GetAoiDnAutoSerial() - 1);	// 검사한 Panel의 AOI 하 Serial
					Sleep(300);
					pView->MpeWrite(_T("MB44012C"), 1); // AOI 하 : PCR파일 Received
				}
			}
		}

		SetListBuf();
	}
}

void CManagerProcedure::SetLotEnd(int nSerial)
{
	pView->SetLotEnd(nSerial);
}

BOOL CManagerProcedure::IsSetLotEnd()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	if (General.nLotEndSerial > 0)
		return TRUE;
	return FALSE;
}

int CManagerProcedure::GetLotEndSerial()
{
	return pView->GetLotEndSerial();
}

BOOL CManagerProcedure::ChkStShotNum()
{
	CString sMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	stListBuf* ListBuf = pView->m_mgrStatus->ListBuf;


	if (ListBuf[0].nTot == 0)
	{
		if (m_nShareUpS > 0 && !(m_nShareUpS % 2))
		{
			sMsg.Format(_T("AOI 상면의 시리얼이 짝수로 시작하였습니다.\r\n- 시리얼 번호: %d"), m_nShareUpS);
			pView->MsgBox(sMsg);
			return FALSE;
		}
	}

	if (bDualTest)
	{
		if (ListBuf[1].nTot == 0)
		{
			if (m_nShareDnS > 0 && !(m_nShareDnS % 2))
			{
				sMsg.Format(_T("AOI 하면의 시리얼이 짝수로 시작하였습니다.\r\n- 시리얼 번호: %d"), m_nShareDnS);
				pView->MsgBox(sMsg);
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CManagerProcedure::DispInfo()
{
	pView->DispInfo();
}

void CManagerProcedure::ResetMkInfo(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
{
	pView->ResetMkInfo(nAoi);
}

BOOL CManagerProcedure::IsLastJob(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
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

void CManagerProcedure::SetAlignPos()
{
	pView->SetAlignPos();
}

void CManagerProcedure::InitReelmap()
{
	if(pView->m_mgrReelmap)
		pView->m_mgrReelmap->InitReelmap();
}

void CManagerProcedure::InitReelmapInner()
{
	if (pView->m_mgrReelmap)
		pView->m_mgrReelmap->InitReelmapInner();
}

void CManagerProcedure::ModelChange(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn 
{
	pView->ModelChange(nAoi);
}

void CManagerProcedure::InitReelmapDisp()
{
	pView->InitReelmapDisp();
}

void CManagerProcedure::InitCamImgDisp()
{
	pView->InitCamImgDisp();
}

BOOL CManagerProcedure::OpenReelmapFromBuf(int nSerial)
{
	if (!pView->m_mgrReelmap)	return FALSE;
	return pView->m_mgrReelmap->OpenReelmapFromBuf(nSerial);
}

int CManagerProcedure::LoadPcrUp(int nSerial)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (!pView->m_mgrReelmap)	return FALSE;
	return pView->m_mgrReelmap->LoadPcrUp(nSerial);
}

int CManagerProcedure::LoadPcrDn(int nSerial)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (!pView->m_mgrReelmap)	return FALSE;
	return pView->m_mgrReelmap->LoadPcrDn(nSerial);
}

BOOL CManagerProcedure::UpdateReelmap(int nSerial)
{
	if (!pView->m_mgrReelmap)	return FALSE;
	return pView->m_mgrReelmap->UpdateReelmap(nSerial);
}

BOOL CManagerProcedure::MakeItsFile(int nSerial)
{
	if (!pView->m_mgrReelmap)	return FALSE;
	return pView->m_mgrReelmap->MakeItsFile(nSerial);
}

void CManagerProcedure::Mk2PtReady()
{
	if (!pView->m_mgrPunch) return;
	CSr1000w*  pSr1000w = pView->m_mgrPunch->m_pSr1000w;
	stListBuf* ListBuf = pView->m_mgrStatus->ListBuf;
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sMsg;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST:	// PLC MK 신호 확인	
			if (IsRun())
			{
				pDoc->LogAuto(_T("PC: 마킹부 마킹중 ON (PC가 ON, OFF)"));
				pView->MpeWrite(_T("MB440150"), 1);// 마킹부 마킹중 ON (PC가 ON, OFF)
#ifdef USE_SR1000W
				if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
				{
					if (pSr1000w && pSr1000w->IsConnected())
					{
						m_sGet2dCodeLot = _T("");
						m_nGet2dCodeSerial = 0;
						//Set2dRead(TRUE);
					}
					else
					{
						Stop();
						pView->ClrDispMsg();
						sMsg.Format(_T("2D 리더기가 연결이 되지 않았습니다."));
						AfxMessageBox(sMsg);
						m_nMkStAuto = MK_ST;
						break;
					}
				}
#endif
				m_nMkStAuto++;
			}
			break;
		case MK_ST + 1:
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Start);
			}
			break;
		case MK_ST + (Mk2PtIdx::Start) :	// 2
			if (bDualTest)
			{
				if (ListBuf[1].nTot > 0) // AOI-Dn
				{
					m_nMkStAuto++;

					m_nBufDnSerial[0] = ListBuf[1].Pop();
					m_nBufUpSerial[0] = m_nBufDnSerial[0];

					if (ListBuf[1].nTot > 0) // AOI-Dn
					{
						m_nBufDnSerial[1] = ListBuf[1].Pop();
						m_nBufUpSerial[1] = m_nBufDnSerial[1];
					}
					else
					{
						m_nBufDnSerial[1] = 0;
						m_nBufUpSerial[1] = 0;
					}
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						m_bLotEnd = TRUE;
						m_nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (m_nBufUpSerial[0] == 1)
					{
						SetLastProc(ID_AOIDN);
					}
				}
			}
			else
			{
				if (ListBuf[0].nTot > 0) // AOI-Up
				{
					m_nMkStAuto++;
					m_nBufUpSerial[0] = ListBuf[0].Pop();

					if (ListBuf[0].nTot > 0) // AOI-Up
					{
						m_nBufUpSerial[1] = ListBuf[0].Pop();
					}
					else
						m_nBufUpSerial[1] = 0;
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						m_bLotEnd = TRUE;
						m_nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (m_nBufUpSerial[0] == 1)
					{
						SetLastProc(ID_AOIDN);
					}
				}
			}
			break;
		case MK_ST + (Mk2PtIdx::Start) + 1:
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN && !Thread.bTHREAD_MAKE_ITS_FILE_UP && !Thread.bTHREAD_MAKE_ITS_FILE_DN)
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn);
			}
			else
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn);
			}
			break;
		}
	}
}

void CManagerProcedure::Mk2PtChkSerial()
{
	if (!pView->m_mgrStatus)	return;
	stThread& Thread = (pView->m_mgrStatus->Thread);
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	//CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	int nLastShot = 0;
	double dFdEnc;
	CString sLot, sMsg;

	if (Thread.bTHREAD_SHIFT2MK)
		return;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::ChkSn) :
			if (m_nBufUpSerial[0] == m_nBufUpSerial[1])
			{
				Stop();
				pView->ClrDispMsg();
				AfxMessageBox(_T("좌/우 마킹 시리얼이 같습니다."));
				SetListBuf();
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Start);
				break;
			}

			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN && !Thread.bTHREAD_MAKE_ITS_FILE_UP && !Thread.bTHREAD_MAKE_ITS_FILE_DN)
				{
					if (!Thread.bTHREAD_DISP_DEF && !Thread.bTHREAD_DISP_DEF_INNER)
					{
						m_nMkStAuto++;
						Thread.nStepTHREAD_DISP_DEF = 0;
						Thread.nStepTHREAD_DISP_DEF_INNER = 0;

						Thread.bTHREAD_DISP_DEF = TRUE;				// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						Thread.bTHREAD_DISP_DEF_INNER = TRUE;		// DispDefImgInner() : Disp Reelmap Start

						SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}
			else
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				{
					if (!Thread.bTHREAD_DISP_DEF)
					{
						m_nMkStAuto++;
						Thread.nStepTHREAD_DISP_DEF = 0;

						Thread.bTHREAD_DISP_DEF = TRUE;		// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}
			break;

		case MK_ST + (Mk2PtIdx::ChkSn) + 1:
			m_nMkStAuto++;
			nSerial = m_nBufUpSerial[0];
			//sNewLot = m_sNewLotUp;

			if (nSerial > 0)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[1] < General.nLotEndSerial)
						nLastShot = m_nBufUpSerial[0];
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[1] > General.nLotEndSerial)
					{
						nLastShot = m_nBufUpSerial[0];
					}
				}

				bNewModel = GetAoiUpInfo(nSerial, &nNewLot, TRUE);
				if (bDualTest)
				{
					bNewModel = GetAoiDnInfo(nSerial, &nNewLot, TRUE);

					if (!IsSameUpDnLot() && !General.bContDiffLot)
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::LotDiff);
						break;
					}
				}

				if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
				{
					;
				}
				if (nNewLot)
				{					
					ChgLot(); // Lot Change.
#ifdef USE_MPE
					dFdEnc = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
					if ((pDoc->WorkingInfo.LastJob.bLotSep || pDoc->m_bDoneChgLot) && (dFdEnc + _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen)*2.0) > _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0)
					{
						pDoc->m_bDoneChgLot = TRUE;
						SetPathAtBuf();
					}
#endif
				}
			}
			else
			{
				Stop();
				MsgBox(_T("버퍼(좌) Serial이 맞지않습니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
			break;

		case MK_ST + (Mk2PtIdx::ChkSn) + 2:
			//pDoc->UpdateYieldOnRmap(); // 20230614

#ifdef USE_SR1000W
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pSr1000w && pSr1000w->IsConnected())
				{
					//Set2dRead(TRUE);
					//Get2dCode(sLot, nSerial);
					if (m_sGet2dCodeLot != _T("") && m_nGet2dCodeSerial != 0)
					{
						if (m_nBufUpSerial[0] == m_nGet2dCodeSerial)
						{
							m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);			// InitMk()
						}
						else
						{
							Stop();
							pView->ClrDispMsg();
							sMsg.Format(_T("2D각인 시리얼(%d)과 검사파일 시리얼(%d)이 다릅니다."), m_nGet2dCodeSerial, m_nBufUpSerial[0]);
							AfxMessageBox(sMsg);
							m_nMkStAuto = MK_ST + (Mk2PtIdx::Start);
							break;
						}
					}
					else
						m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);			// InitMk()

				}
				else
				{
					Stop();
					pView->ClrDispMsg();
					sMsg.Format(_T("2D 리더기가 연결이 되지 않았습니다."));
					AfxMessageBox(sMsg);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Start);
					break;
				}
			}
			else
				m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);					// InitMk()
#else
			m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);					// InitMk()
#endif
			break;
		}
	}
}

void CManagerProcedure::Mk2PtInit()
{
	if (Thread.bTHREAD_SHIFT2MK)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::InitMk) :
			if (InitMk()) // 시리얼이 증가 또는 감소 : TRUE 아니고 같으면 : FALSE
			{
				if (General.bSerialDecrese)
				{
					if ((m_nBufUpSerial[0] <= General.nLotEndSerial || m_nBufUpSerial[1] <= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off) - 20160718
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
				else
				{
					if ((m_nBufUpSerial[0] >= General.nLotEndSerial || m_nBufUpSerial[1] >= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off) - 20160718
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
			}
			else // Same Serial
			{
				Stop();
				MsgBox(_T("Serial 연속 되지않습니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
										m_nMkStAuto++;
										break;

		case MK_ST + (Mk2PtIdx::InitMk) + 1:
			if (IsRun())
			{
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1);	// Move - Cam1 - Pt0
																//if (MODE_INNER != pDoc->GetTestMode())
																//	m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1);	// Move - Cam1 - Pt0
																//else
																//	m_nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt);
			}
			break;
		}
	}
}

void CManagerProcedure::Mk2PtAlignPt0()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Move0Cam1) :	// Move - Cam1 - Pt0
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufDnSerial[1] < General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0))	// Move - Cam1 - Pt0
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufDnSerial[1] > General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0))	// Move - Cam1 - Pt0
							m_nMkStAuto++;
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[1] < General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[1] > General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
							m_nMkStAuto++;
					}
				}
			}
												break;
		case MK_ST + (Mk2PtIdx::Move0Cam1) + 1:
			if (IsRun())
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0);
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) :	// Move - Cam0 - Pt0
			if (MoveAlign0(0))
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Align1_0);
			}
			break;
		case MK_ST + (Mk2PtIdx::Align1_0) :	// 2PtAlign - Cam1 - Pt0
			if (!m_bSkipAlign[1][0])
			{
				if (TwoPointAlign1(0))
					m_bFailAlign[1][0] = FALSE;
				else
					m_bFailAlign[1][0] = TRUE;
			}
											m_nMkStAuto = MK_ST + (Mk2PtIdx::Align0_0);
											break;
		case MK_ST + (Mk2PtIdx::Align0_0) :	// 2PtAlign - Cam0 - Pt0
			if (!m_bSkipAlign[0][0])
			{
				if (TwoPointAlign0(0))
					m_bFailAlign[0][0] = FALSE;
				else
					m_bFailAlign[0][0] = TRUE;
			}
											m_nMkStAuto++;
											break;
		case MK_ST + (Mk2PtIdx::Align0_0) + 1:
			if (m_bFailAlign[0][0])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);
				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][0] = FALSE;
						m_bSkipAlign[0][0] = TRUE;
						m_bSkipAlign[0][1] = TRUE;
						m_bSkipAlign[0][2] = TRUE;
						m_bSkipAlign[0][3] = TRUE;
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE;
							m_bDoneMk[0] = TRUE;
						}
						else
						{
							m_bDoMk[0] = TRUE;
							m_bDoneMk[0] = FALSE;
						}
					}
					else
					{
						m_bReAlign[0][0] = TRUE;
						m_bSkipAlign[0][0] = FALSE;
						m_bSkipAlign[0][1] = FALSE;
						m_bSkipAlign[0][2] = FALSE;
						m_bSkipAlign[0][3] = FALSE;
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0); // TwoPointAlign0(0) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][0] = TRUE;
					m_bSkipAlign[0][0] = FALSE;
					m_bSkipAlign[0][1] = FALSE;
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0); // TwoPointAlign0(0) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}
			if (m_bFailAlign[1][0])
			{
				Buzzer(TRUE, 0);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][0] = FALSE;
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE;
							m_bDoneMk[1] = TRUE;
						}
						else
						{
							m_bDoMk[1] = TRUE;
							m_bDoneMk[1] = FALSE;
						}
					}
					else
					{
						m_bReAlign[1][0] = TRUE;
						m_bSkipAlign[1][0] = FALSE;
						m_bSkipAlign[1][1] = FALSE;
						m_bSkipAlign[1][2] = FALSE;
						m_bSkipAlign[1][3] = FALSE;
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
						Stop();
						TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][0] = TRUE;
					m_bSkipAlign[1][0] = FALSE;
					m_bSkipAlign[1][1] = FALSE;
					m_bSkipAlign[1][2] = FALSE;
					m_bSkipAlign[1][3] = FALSE;
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
					Stop();
					TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][0] || m_bFailAlign[1][0])
			{
				if (!m_bReAlign[0][0] && !m_bReAlign[1][0])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++; //m_nMkStAuto = MK_ST + 27; // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
				}
			}
			else
				m_nMkStAuto++;

			break;
		case MK_ST + (Mk2PtIdx::Align0_0) + 2:
			if (IsRun())
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1);
			break;
		}
	}
}

void CManagerProcedure::Mk2PtAlignPt1()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Move1Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufDnSerial[1] < General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufDnSerial[1] > General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[1] < General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[1] > General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else if (m_nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
			}
										   break;
		case MK_ST + (Mk2PtIdx::Move1Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) :
			if (MoveAlign0(1))	// Move - Cam0 - Pt1
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Align1_1);
			}
			break;
		case MK_ST + (Mk2PtIdx::Align1_1) :	// 2PtAlign - Cam1 - Pt1
			if (!m_bFailAlign[1][0])
			{
				if (!m_bSkipAlign[1][1])
				{
					if (!TwoPointAlign1(1))
						m_bFailAlign[1][1] = TRUE;
					else
						m_bFailAlign[1][1] = FALSE;
				}
				else
					m_bFailAlign[1][1] = FALSE;
			}
			else
				m_bFailAlign[1][1] = FALSE;

			m_nMkStAuto = MK_ST + (Mk2PtIdx::Align0_1);
			break;
		case MK_ST + (Mk2PtIdx::Align0_1) :	// 2PtAlign - Cam0 - Pt1
			if (!m_bFailAlign[0][0])
			{
				if (!m_bSkipAlign[0][1])
				{
					if (!TwoPointAlign0(1))
						m_bFailAlign[0][1] = TRUE;
					else
						m_bFailAlign[0][1] = FALSE;
				}
				else
					m_bFailAlign[0][1] = FALSE;
			}
			else
				m_bFailAlign[0][1] = FALSE;

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Align0_1) + 1:
			if (m_bFailAlign[0][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][1] = FALSE;
						m_bSkipAlign[0][1] = TRUE;
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE;
							m_bDoneMk[0] = TRUE;
						}
						else
						{
							m_bDoMk[0] = TRUE;
							m_bDoneMk[0] = FALSE;
						}
					}
					else
					{
						m_bReAlign[0][1] = TRUE;
						m_bSkipAlign[0][1] = FALSE;
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam0); // TwoPointAlign0(1) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][1] = TRUE;
					m_bSkipAlign[0][1] = FALSE;
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam0); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}
			if (m_bFailAlign[1][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][1] = FALSE;
						m_bSkipAlign[1][1] = TRUE;
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE;
							m_bDoneMk[1] = TRUE;
						}
						else
						{
							m_bDoMk[1] = TRUE;
							m_bDoneMk[1] = FALSE;
						}
					}
					else
					{
						m_bReAlign[1][1] = TRUE;
						m_bSkipAlign[1][1] = FALSE;
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][1] = TRUE;
					m_bSkipAlign[1][1] = FALSE;
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][1] || m_bFailAlign[1][1])
			{
				if (!m_bReAlign[0][1] && !m_bReAlign[1][1])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt); //m_nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
			}
			else
				m_nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt);

			break;
		}
	}
}

void CManagerProcedure::Mk2PtMoveInitPos()
{
	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::MoveInitPt) :
			MoveInitPos0(FALSE);
			MoveInitPos1(FALSE); // 20220526
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::MoveInitPt) + 1:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::MoveInitPt) + 2:
			if (IsMoveDone())
				m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkElec);
			break;
		}
	}
}

void CManagerProcedure::Mk2PtElecChk()
{
	CString sRst;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::ChkElec) : // DoElecChk
			if (DoElecChk(sRst))
			{
				if (pDoc->WorkingInfo.Probing[0].bUse)
				{
					if (sRst == _T("Open"))
					{
						if (pDoc->WorkingInfo.Probing[0].bStopOnOpen)
							m_nMkStAuto = REJECT_ST;
						else
							m_nMkStAuto++;
					}
					else if (sRst == _T("Error"))
					{
						m_nMkStAuto = ERROR_ST;
					}
					else
					{
						m_nMkStAuto++;
					}
				}
				else
					m_nMkStAuto++;
			}
										   break;

		case MK_ST + (Mk2PtIdx::ChkElec) + 1:
			if (ChkLightErr())
			{
				m_bChkLightErr = FALSE;
				m_nMkStAuto++;
			}
			else
			{
				if (MODE_INNER != pDoc->GetTestMode())
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); 	// Mk 마킹 시작
				else
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Shift2Mk);
			}
			break;

		case MK_ST + (Mk2PtIdx::ChkElec) + 2:
			if (IsRun())
			{
				if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0) &&
					m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
				{
					if (MODE_INNER != pDoc->GetTestMode())
						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); 	// Mk 마킹 시작
					else
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Shift2Mk);
				}
				else
				{
					Stop();
					MsgBox(_T("마킹부 모션이 비활성화 되었습니다."));
					TowerLamp(RGB_RED, TRUE);
				}
			}
			else
			{
				if (!m_bChkLightErr)
				{
					m_bChkLightErr = TRUE;
					MsgBox(_T("노광불량 정지 - 기판을 확인하세요.\r\n계속진행하려면 운전스위치를 누르세요."));
				}
			}
			break;
		}
	}
}

void CManagerProcedure::Mk2PtDoMarking()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::DoMk) :				// Mk 마킹 시작]
			if (CheckMkPnt())
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
					SetMkIts(TRUE);						// ITS 마킹 시작
				else
					SetMk(TRUE);						// Mk 마킹 시작

				m_nMkStAuto++;
			}
			else
			{
				MsgBox(_T("캠마스터의 마킹위치좌표가 설정되어 있지않습니다.\r\n확인하세요."));
				Stop();
				TowerLamp(RGB_RED, TRUE);
			}
													break;

		case MK_ST + (Mk2PtIdx::DoMk) + 1:
			if (!m_bUpdateYield)
			{
				if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
				{
					m_bUpdateYield = TRUE;
					UpdateYield(); // Cam[0],  Cam[1]
					m_nMkStAuto++;
				}
			}
			else
			{
				Sleep(100);
				m_nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::DoMk) + 2:
			if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
			{
				if (!m_bUpdateYieldOnRmap)
				{
					if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN)
					{
						if (!Thread.bTHREAD_UPDATE_YIELD_UP && !Thread.bTHREAD_UPDATE_YIELD_DN && !Thread.bTHREAD_UPDATE_YIELD_ALLUP && !Thread.bTHREAD_UPDATE_YIELD_ALLDN)
						{
							m_bUpdateYieldOnRmap = TRUE;
							pDoc->UpdateYieldOnRmap(); // 20230614
							m_nMkStAuto++;
						}
						else
							Sleep(100);
					}
					else
						Sleep(100);
				}
				else
				{
					Sleep(100);
					m_nMkStAuto++; // 마킹 및 verify가 완전히 끝나지 않은 경우.
				}
			}
			break;

		case MK_ST + (Mk2PtIdx::Verify) :
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = MK_ST + (Mk2PtIdx::Verify);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);		// Mk 마킹 시작
					m_bCam = TRUE;
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = MK_ST + (Mk2PtIdx::Verify);
							m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);		// Mk 마킹 시작
							m_bCam = TRUE;
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE;
							m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk);	// Mk 마킹 완료
						}
					}
					else
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk);	// Mk 마킹 완료
					}
				}
			}
			else if (IsReMk())
			{
				m_nPrevMkStAuto = MK_ST + (Mk2PtIdx::Verify);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);		// Mk 재시작
			}
			else
			{
				sMsg = _T("");
				sMsg += m_sDispSts[0];
				sMsg += _T(",");
				sMsg += m_sDispSts[1];
			}
										break;

		case MK_ST + (Mk2PtIdx::DoneMk) :	 // Align변수 초기화
			if (!IsRun())
				break;

			//if (Thread.bTHREAD_UPDATE_YIELD_UP || Thread.bTHREAD_UPDATE_YIELD_DN || Thread.bTHREAD_UPDATE_YIELD_ALLUP || Thread.bTHREAD_UPDATE_YIELD_ALLDN)
			//	break;

			if (m_bInitAuto)
			{
				m_bInitAuto = FALSE;
				MsgBox(_T("마킹위치를 확인하세요."));
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
				break;
			}

			if (m_nBufUpSerial[0] == 0)
			{
				m_bSkipAlign[0][0] = TRUE;
				m_bSkipAlign[0][1] = TRUE;
			}
			if (m_nBufUpSerial[1] == 0)
			{
				m_bSkipAlign[1][0] = TRUE;
				m_bSkipAlign[1][1] = TRUE;
			}

			if ((!m_bSkipAlign[0][0] && !m_bSkipAlign[0][1]) && (!m_bSkipAlign[1][0] && !m_bSkipAlign[1][1]))
				CompletedMk(2); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			else if ((m_bSkipAlign[0][0] || m_bSkipAlign[0][1]) && (!m_bSkipAlign[1][0] && !m_bSkipAlign[1][1]))
				CompletedMk(1); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			else if ((!m_bSkipAlign[0][0] && !m_bSkipAlign[0][1]) && (m_bSkipAlign[1][0] || m_bSkipAlign[1][1]))
				CompletedMk(0); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			else
				CompletedMk(3); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 1:  // Mk변수 초기화
			m_bReAlign[0][0] = FALSE; // [nCam][nPos] 
			m_bReAlign[0][1] = FALSE; // [nCam][nPos] 
			m_bReAlign[1][0] = FALSE; // [nCam][nPos] 
			m_bReAlign[1][1] = FALSE; // [nCam][nPos] 

			m_bSkipAlign[0][0] = FALSE; // [nCam][nPos] 
			m_bSkipAlign[0][1] = FALSE; // [nCam][nPos] 
			m_bSkipAlign[1][0] = FALSE; // [nCam][nPos] 
			m_bSkipAlign[1][1] = FALSE; // [nCam][nPos] 

			m_bFailAlign[0][0] = FALSE; // [nCam][nPos] 
			m_bFailAlign[0][1] = FALSE; // [nCam][nPos] 
			m_bFailAlign[1][0] = FALSE; // [nCam][nPos] 
			m_bFailAlign[1][1] = FALSE; // [nCam][nPos] 

			m_bDoMk[0] = TRUE;
			m_bDoMk[1] = TRUE;
			m_bDoneMk[0] = FALSE;
			m_bDoneMk[1] = FALSE;
			m_bReMark[0] = FALSE;
			m_bReMark[1] = FALSE;
			m_bCam = FALSE;
			m_nPrevMkStAuto = 0;

			m_bUpdateYield = FALSE;
			m_bUpdateYieldOnRmap = FALSE;

			for (a = 0; a < 2; a++)
			{
				for (b = 0; b < MAX_STRIP_NUM; b++)
				{
					m_nMkStrip[a][b] = 0;
					m_bRejectDone[a][b] = FALSE;
				}
			}

			m_nSaveMk0Img = 0;
			m_nSaveMk1Img = 0;

			pView->MpeWrite(_T("MB440150"), 0);	// 마킹부 마킹중 ON (PC가 ON, OFF)
			pView->MpeWrite(_T("MB440170"), 1);	// 마킹완료(PLC가 확인하고 Reset시킴.)-20141029
			pDoc->LogAuto(_T("PC: 마킹완료(PLC가 확인하고 Reset시킴.)"));
			if (IsNoMk() || IsShowLive())
				ShowLive(FALSE);

			m_nMkStAuto++;
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) + 2:
			if (pDoc->m_pMpeSignal[0] & (0x01 << 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
			{
				pDoc->LogAuto(_T("PLC: 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
				m_nMkStAuto++;
				//if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
				//{
				//	UpdateYield(); // Cam[0],  Cam[1]
				//	m_nMkStAuto++;
				//}
			}
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) + 3:
#ifdef USE_MPE
			//if (pDoc->m_pMpeSignal[0] & (0x01 << 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
		{
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				pView->MpeWrite(_T("MB440101"), 0);	// 마킹부 Feeding완료
				pDoc->LogAuto(_T("PC: 마킹부 Feeding완료 OFF"));

				//Shift2Mk();			// PCR 이동(Buffer->Marked) // 기록(WorkingInfo.LastJob.sSerial)
				m_bShift2Mk = TRUE;
				DoShift2Mk();

				SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();

				UpdateRst();

				//UpdateWorking();	// Update Working Info...
				m_nMkStAuto++;
			}
		}
#endif
		break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 4:
			//sMsg.Format(_T("%d%d%d%d%d"), Thread.bTHREAD_SHIFT2MK ? 1 : 0, Thread.bTHREAD_REELMAP_YIELD_UP ? 1 : 0, Thread.bTHREAD_REELMAP_YIELD_DN ? 1 : 0, Thread.bTHREAD_REELMAP_YIELD_ALLUP ? 1 : 0, Thread.bTHREAD_REELMAP_YIELD_ALLDN ? 1 : 0);
			//DispStsBar(sMsg, 0);

			if (!Thread.bTHREAD_SHIFT2MK && !Thread.bTHREAD_REELMAP_YIELD_UP && !Thread.bTHREAD_REELMAP_YIELD_DN && !Thread.bTHREAD_REELMAP_YIELD_ALLUP && !Thread.bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (Thread.bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
						break;
				}

				m_nMkStAuto++;
				//UpdateRst();
				//UpdateWorking();	// Update Working Info...
				ChkYield();
			}
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 5:
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				ChkLotCutPos();
				UpdateWorking();	// Update Working Info...
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 6:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 7:
			m_nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 8:
			m_bMkSt = FALSE;
			break;
		}
	}
}

void CManagerProcedure::Mk2PtShift2Mk() // MODE_INNER
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Shift2Mk) :
			m_nMkStAuto++;
			//if (!m_bUpdateYield)
			//{
			if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
			{
				//m_bUpdateYield = TRUE;
				UpdateYield(); // Cam[0],  Cam[1]
				m_nMkStAuto++;
			}
			//}
			//else
			//{
			//	Sleep(100);
			//	m_nMkStAuto++;
			//}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 1:
			if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
			{
				//if (!m_bUpdateYieldOnRmap)
				//{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN)
				{
					if (!Thread.bTHREAD_UPDATE_YIELD_UP && !Thread.bTHREAD_UPDATE_YIELD_DN && !Thread.bTHREAD_UPDATE_YIELD_ALLUP && !Thread.bTHREAD_UPDATE_YIELD_ALLDN)
					{
						UpdateRst();
						UpdateWorking();	// Update Working Info...

											//m_bUpdateYieldOnRmap = TRUE;
						pDoc->UpdateYieldOnRmap(); // 20230614
						m_nMkStAuto++;
					}
					else
						Sleep(100);
				}
				else
					Sleep(100);
				//}
				//else
				//{
				//	Sleep(100);
				//	m_nMkStAuto++; // 마킹 및 verify가 완전히 끝나지 않은 경우.
				//}
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 2:
			pView->MpeWrite(_T("MB440150"), 0);	// 마킹부 마킹중 ON (PC가 ON, OFF)
			pView->MpeWrite(_T("MB440170"), 1);	// 마킹완료(PLC가 확인하고 Reset시킴.)-20141029
			pDoc->LogAuto(_T("PC: 마킹완료(PLC가 확인하고 Reset시킴.)"));
			m_nMkStAuto++;
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 3:
			if (pDoc->m_pMpeSignal[0] & (0x01 << 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
			{
				pDoc->LogAuto(_T("PLC: 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
				m_nMkStAuto++;
				//if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
				//{
				//	UpdateYield(); // Cam[0],  Cam[1]
				//	m_nMkStAuto++;
				//}
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 4:
#ifdef USE_MPE
			//if (pDoc->m_pMpeSignal[0] & (0x01 << 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
		{
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				pDoc->LogAuto(_T("PC: 마킹부 Feeding완료 OFF"));
				pView->MpeWrite(_T("MB440101"), 0);	// 마킹부 Feeding완료
				m_bShift2Mk = TRUE;
				DoShift2Mk();

				SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();

				m_nMkStAuto++;
			}
		}
#endif
		break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 5:
			if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
			{
				if (!Thread.bTHREAD_SHIFT2MK && !Thread.bTHREAD_REELMAP_YIELD_UP && !Thread.bTHREAD_REELMAP_YIELD_DN && !Thread.bTHREAD_REELMAP_YIELD_ALLUP && !Thread.bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
				{
					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						if (Thread.bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
							break;
					}

					m_nMkStAuto++;
					UpdateRst();
					UpdateWorking();	// Update Working Info...
					ChkYield();
				}
			}
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 6:
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				ChkLotCutPos();
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 7:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 8:
			m_nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 9:
			m_bMkSt = FALSE;
			break;
		}
	}
}

void CManagerProcedure::Mk2PtLotDiff()
{
	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::LotDiff) :
			Stop();
			TowerLamp(RGB_YELLOW, TRUE);
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 1:
			//if(IDYES == DoMyMsgBox(_T("상면과 하면의 Lot가 다릅니다.\r\n계속 작업을 진행하시겠습니까?"), MB_YESNO))
			if (IDYES == MsgBox(_T("상면과 하면의 Lot가 다릅니다.\r\n계속 작업을 진행하시겠습니까?"), 0, MB_YESNO))
			{
				General.bContDiffLot = TRUE;
			}
			else
			{
				General.bContDiffLot = FALSE;
				m_bLotEnd = TRUE;
				m_nLotEndAuto = LOT_END;
			}
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 2:
			if (IsRun())
			{
				if (General.bContDiffLot)
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn);
				else
					m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 3:
			General.bContDiffLot = FALSE;
			m_bLotEnd = TRUE;
			m_nLotEndAuto = LOT_END;
			break;
		}
	}
}

void CManagerProcedure::Mk2PtReject()
{
	int a, b;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case REJECT_ST:
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();

			//if(IDYES == DoMyMsgBox(_T("쇼트 체크 불량입니다.\r\n리젝 처리를 진행하시겠습니까?"), MB_YESNO))
			if (IDYES == MsgBox(_T("쇼트 체크 불량입니다.\r\n리젝 처리를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				m_nMkStAuto++;
			}
			else
			{
				//if(IDYES == DoMyMsgBox(_T("리젝 처리를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), MB_YESNO))
				if (IDYES == MsgBox(_T("리젝 처리를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					m_nMkStAuto++;
				}
			}

			Buzzer(FALSE, 0);
			break;
		case REJECT_ST + 1:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE;
					m_nMkStAuto++;
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);	// Mk 마킹 시작
				}
				else
					m_nMkStAuto = REJECT_ST;
			}
			break;
		case REJECT_ST + 2:
			SetReject();
			m_nMkStAuto++;
			break;
		case REJECT_ST + 3:
			m_nMkStAuto++;
			break;
		case REJECT_ST + 4:
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = REJECT_ST + 4;
					m_nMkStAuto = REJECT_ST + 2;		// Mk 마킹 시작
					m_bCam = TRUE;

					m_bDoneMk[0] = FALSE;
					m_bDoneMk[1] = FALSE;

					for (a = 0; a < 2; a++)
					{
						for (b = 0; b < MAX_STRIP_NUM; b++)
						{
							m_nMkStrip[a][b] = 0;
							m_bRejectDone[a][b] = FALSE;
						}
					}
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = REJECT_ST + 4;
							m_nMkStAuto = REJECT_ST + 2;		// Mk 마킹 시작
							m_bCam = TRUE;
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE;
							m_nMkStAuto++;	// Mk 마킹 완료

											//sMsg = _T("");
											//DispStsBar(sMsg, 0);
						}
					}
					else
					{
						m_nMkStAuto++;	// Mk 마킹 완료

										//sMsg = _T("");
										//DispStsBar(sMsg, 0);
					}
				}
			}
			break;
		case REJECT_ST + 5:
			m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk);				// Align변수 초기화
			break;
		}
	}
}

void CManagerProcedure::Mk2PtErrStop()
{
	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case ERROR_ST:
			Stop();
			TowerLamp(RGB_RED, TRUE);

			//if(IDYES == DoMyMsgBox(_T("쇼트 체크 Error입니다.\r\n다시 쇼트 체크를 진행하시겠습니까?"), MB_YESNO))
			if (IDYES == MsgBox(_T("쇼트 체크 Error입니다.\r\n다시 쇼트 체크를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				m_nMkStAuto++;
			}
			else
			{
				//if(IDYES == DoMyMsgBox(_T("쇼트 체크를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), MB_YESNO))
				if (IDYES == MsgBox(_T("쇼트 체크를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					m_nMkStAuto++;
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 1:
			m_nMkStAuto++;
			break;
		case ERROR_ST + 2:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE;
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkElec); // DoElecChk
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);	// Mk 마킹 시작
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 3:
			m_nMkStAuto = ERROR_ST;
			break;
		}
	}
}

void CManagerProcedure::MarkingWith2PointAlign()
{
	Mk2PtReady();
	Mk2PtChkSerial();
	Mk2PtInit();
	Mk2PtAlignPt0();
	Mk2PtAlignPt1();
	Mk2PtMoveInitPos();
	Mk2PtElecChk();
	if (MODE_INNER != pDoc->GetTestMode())
		Mk2PtDoMarking();
	else
		Mk2PtShift2Mk();
	Mk2PtLotDiff();
	Mk2PtReject();
	Mk2PtErrStop();
}

void CManagerProcedure::Mk4PtReady()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	stListBuf* ListBuf = pView->m_mgrStatus->ListBuf;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST:	// PLC MK 신호 확인	
			if (IsRun())
			{
				SetListBuf();
				m_nMkStAuto++;
			}
			break;
		case MK_ST + 1:
			pDoc->LogAuto(_T("PC: 마킹부 마킹중 ON (PC가 ON, OFF)"));
			pView->MpeWrite(_T("MB440150"), 1);// 마킹부 마킹중 ON (PC가 ON, OFF)
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Start) :	// 2
			if (bDualTest)
			{
				if (ListBuf[1].nTot > 0) // AOI-Dn
				{
					m_nMkStAuto++;

					m_nBufDnSerial[0] = ListBuf[1].Pop();
					m_nBufUpSerial[0] = m_nBufDnSerial[0];
					if (ListBuf[1].nTot > 0) // AOI-Dn
					{
						m_nBufDnSerial[1] = ListBuf[1].Pop();
						m_nBufUpSerial[1] = m_nBufDnSerial[1];
					}
					else
					{
						m_nBufDnSerial[1] = 0;
						m_nBufUpSerial[1] = 0;
					}
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						m_bLotEnd = TRUE;
						m_nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (m_nBufUpSerial[0] == 1)
					{
						SetLastProc(ID_AOIDN);
					}
				}
			}
			else
			{
				if (ListBuf[0].nTot > 0) // AOI-Up
				{
					m_nMkStAuto++;
					m_nBufUpSerial[0] = ListBuf[0].Pop();
					if (ListBuf[0].nTot > 0) // AOI-Up
						m_nBufUpSerial[1] = ListBuf[0].Pop();
					else
						m_nBufUpSerial[1] = 0;
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						m_bLotEnd = TRUE;
						m_nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (m_nBufUpSerial[0] == 1)
					{
						SetLastProc(ID_AOIDN);
					}
				}
			}
											break;
		case MK_ST + (Mk4PtIdx::Start) + 1:
			m_nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtChkSerial()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	double dFdEnc;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::ChkSn) :
			if (m_nBufUpSerial[0] == m_nBufUpSerial[1])
			{
				Stop();
				pView->ClrDispMsg();
				AfxMessageBox(_T("좌/우 마킹 시리얼이 같습니다."));
				SetListBuf();
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Start);
				break;
			}

									   if (!Thread.bTHREAD_DISP_DEF)
									   {
										   m_nMkStAuto++;
										   Thread.nStepTHREAD_DISP_DEF = 0;
										   Thread.bTHREAD_DISP_DEF = TRUE;		// DispDefImg() : CopyDefImg Start
										   SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
									   }
									   break;

		case MK_ST + (Mk4PtIdx::ChkSn) + 1:
			m_nMkStAuto = MK_ST + (Mk4PtIdx::InitMk);			// InitMk()
			nSerial = m_nBufUpSerial[0];
			sNewLot = m_sNewLotUp;

			if (nSerial > 0)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[1] < General.nLotEndSerial)
					{
						nSerial = m_nBufUpSerial[0]; // Test
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[1] > General.nLotEndSerial)
					{
						nSerial = m_nBufUpSerial[0]; // Test
					}
				}

				bNewModel = GetAoiUpInfo(nSerial, &nNewLot, TRUE);
				if (bDualTest)
				{
					bNewModel = GetAoiDnInfo(nSerial, &nNewLot, TRUE);

					if (!IsSameUpDnLot() && !General.bContDiffLot)
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::LotDiff);
						break;
					}
				}

				if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
				{
					;
				}
				if (nNewLot)
				{
					// Lot Change.
					ChgLot();
#ifdef USE_MPE
					dFdEnc = (double)pDoc->m_pMpeData[0][0];	// 마킹부 Feeding 엔코더 값(단위 mm )
					if ((pDoc->WorkingInfo.LastJob.bLotSep || pDoc->m_bDoneChgLot) && (dFdEnc + _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen)*2.0) > _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0)
					{
						pDoc->m_bDoneChgLot = TRUE;
						SetPathAtBuf();
					}
#endif
				}
			}
			else
			{
				Stop();
				MsgBox(_T("버퍼(좌) Serial이 맞지않습니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
			break;
		}
	}
}

void CManagerProcedure::Mk4PtInit()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::InitMk) :
			if (InitMk())	// 시리얼이 증가 또는 감소 : TRUE 아니고 같으면 : FALSE
			{
				if (General.bSerialDecrese)
				{
					if ((m_nBufUpSerial[0] <= General.nLotEndSerial || m_nBufUpSerial[1] <= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off) - 20160718
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
				else
				{
					if ((m_nBufUpSerial[0] >= General.nLotEndSerial || m_nBufUpSerial[1] >= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off) - 20160718
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
			}
			else
			{
				Stop();
				MsgBox(_T("Serial 연속 되지않습니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
										m_nMkStAuto++;
										break;

		case MK_ST + (Mk4PtIdx::InitMk) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt0()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move0Cam1) :	// Move - Cam1 - Pt0
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0))
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0))
							m_nMkStAuto++;
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0))
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(0))
							m_nMkStAuto++;
					}
				}
			}
												break;
		case MK_ST + (Mk4PtIdx::Move0Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) :	// Move - Cam0 - Pt0
			if (MoveAlign0(0))
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_0) :	// 4PtAlign - Cam1 - Pt0
			if (!m_bSkipAlign[1][0])
			{
				if (FourPointAlign1(0))
					m_bFailAlign[1][0] = FALSE;
				else
					m_bFailAlign[1][0] = TRUE;
			}
											m_nMkStAuto++;
											break;
		case MK_ST + (Mk4PtIdx::Align0_0) :	// 4PtAlign - Cam0 - Pt0
			if (!m_bSkipAlign[0][0])
			{
				if (FourPointAlign0(0))
					m_bFailAlign[0][0] = FALSE;
				else
					m_bFailAlign[0][0] = TRUE;
			}
											m_nMkStAuto++;
											break;
		case MK_ST + (Mk4PtIdx::Align0_0) + 1:
			if (m_bFailAlign[0][0])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][0] = FALSE;
						m_bSkipAlign[0][0] = TRUE;
						m_bSkipAlign[0][1] = TRUE;
						m_bSkipAlign[0][2] = TRUE;
						m_bSkipAlign[0][3] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE;
							m_bDoneMk[0] = TRUE;
						}
						else
						{
							m_bDoMk[0] = TRUE;
							m_bDoneMk[0] = FALSE;
						}
					}
					else
					{
						m_bReAlign[0][0] = TRUE;
						m_bSkipAlign[0][0] = FALSE;
						m_bSkipAlign[0][1] = FALSE;
						m_bSkipAlign[0][2] = FALSE;
						m_bSkipAlign[0][3] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_0); // FourPointAlign0(0) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam0); // FourPointAlign0(0) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][0] = TRUE;
					m_bSkipAlign[0][0] = FALSE;
					m_bSkipAlign[0][1] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_0); // FourPointAlign0(0) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam0); // FourPointAlign0(0) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}
			if (m_bFailAlign[1][0])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][0] = FALSE;
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE;
							m_bDoneMk[1] = TRUE;
						}
						else
						{
							m_bDoMk[1] = TRUE;
							m_bDoneMk[1] = FALSE;
						}
					}
					else
					{
						m_bReAlign[1][0] = TRUE;
						m_bSkipAlign[1][0] = FALSE;
						m_bSkipAlign[1][1] = FALSE;
						m_bSkipAlign[1][2] = FALSE;
						m_bSkipAlign[1][3] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_0); // FourPointAlign1(0) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); // FourPointAlign1(0) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][0] = TRUE;
					m_bSkipAlign[1][0] = FALSE;
					m_bSkipAlign[1][1] = FALSE;
					m_bSkipAlign[1][2] = FALSE;
					m_bSkipAlign[1][3] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_0); // FourPointAlign1(0) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); // FourPointAlign1(0) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][0] || m_bFailAlign[1][0])
			{
				if (!m_bReAlign[0][0] && !m_bReAlign[1][0])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++; //m_nMkStAuto = MK_ST + 27; // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_0); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
				}
			}
			else
				m_nMkStAuto++;

			break;
		case MK_ST + (Mk4PtIdx::Align0_0) + 2:
			if (IsRun())
				m_nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt1()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move1Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(1))	// Move - Cam1 - Pt1
							m_nMkStAuto++;
					}
				}
			}
										   break;
		case MK_ST + (Mk4PtIdx::Move1Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) :
			if (MoveAlign0(1))	// Move - Cam0 - Pt1
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_1) :	// 4PtAlign - Cam1 - Pt1
			if (!m_bFailAlign[1][0])
			{
				if (!m_bSkipAlign[1][1])
				{
					if (!FourPointAlign1(1))
						m_bFailAlign[1][1] = TRUE;
					else
						m_bFailAlign[1][1] = FALSE;
				}
				else
					m_bFailAlign[1][1] = FALSE;
			}
			else
				m_bFailAlign[1][1] = FALSE;

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_1) :	// 4PtAlign - Cam0 - Pt1
			if (!m_bFailAlign[0][0])
			{
				if (!m_bSkipAlign[0][1])
				{
					if (!FourPointAlign0(1))
						m_bFailAlign[0][1] = TRUE;
					else
						m_bFailAlign[0][1] = FALSE;
				}
				else
					m_bFailAlign[0][1] = FALSE;
			}
			else
				m_bFailAlign[0][1] = FALSE;

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_1) + 1:
			if (m_bFailAlign[0][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][1] = FALSE;
						m_bSkipAlign[0][1] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE;
							m_bDoneMk[0] = TRUE;
						}
						else
						{
							m_bDoMk[0] = TRUE;
							m_bDoneMk[0] = FALSE;
						}
					}
					else
					{
						m_bReAlign[0][1] = TRUE;
						m_bSkipAlign[0][1] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_1); // FourPointAlign0(1) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam0); // FourPointAlign0(1) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][1] = TRUE;
					m_bSkipAlign[0][1] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_1); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam0); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}
			if (m_bFailAlign[1][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][1] = FALSE;
						m_bSkipAlign[1][1] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE;
							m_bDoneMk[1] = TRUE;
						}
						else
						{
							m_bDoMk[1] = TRUE;
							m_bDoneMk[1] = FALSE;
						}
					}
					else
					{
						m_bReAlign[1][1] = TRUE;
						m_bSkipAlign[1][1] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_1); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][1] = TRUE;
					m_bSkipAlign[1][1] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_1); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][1] || m_bFailAlign[1][1])
			{
				if (!m_bReAlign[0][1] && !m_bReAlign[1][1])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++;//m_nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
																 //m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_1); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
			}
			else
				m_nMkStAuto++;

			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt2()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move2Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(2))	// Move - Cam1 - Pt2
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(2))	// Move - Cam1 - Pt2
							m_nMkStAuto++;
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(2))	// Move - Cam1 - Pt2
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(2))	// Move - Cam1 - Pt2
							m_nMkStAuto++;
					}
				}
			}
										   break;
		case MK_ST + (Mk4PtIdx::Move2Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) :
			if (MoveAlign0(2))	// Move - Cam0 - Pt2
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_2) :	// 4PtAlign - Cam1 - Pt2
			if (!m_bFailAlign[1][0])
			{
				if (!m_bSkipAlign[1][1])
				{
					if (!m_bSkipAlign[1][2])
					{
						if (!FourPointAlign1(2))
							m_bFailAlign[1][2] = TRUE;
						else
							m_bFailAlign[1][2] = FALSE;
					}
					else
						m_bFailAlign[1][2] = FALSE;
				}
				else
					m_bFailAlign[1][2] = FALSE;
			}
			else
				m_bFailAlign[1][2] = FALSE;

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_2) :	// 4PtAlign - Cam0 - Pt2
			if (!m_bFailAlign[0][0])
			{
				if (!m_bSkipAlign[0][1])
				{
					if (!m_bSkipAlign[0][2])
					{
						if (!FourPointAlign0(2))
							m_bFailAlign[0][2] = TRUE;
						else
							m_bFailAlign[0][2] = FALSE;
					}
					else
						m_bFailAlign[0][2] = FALSE;
				}
				else
					m_bFailAlign[0][2] = FALSE;
			}
			else
				m_bFailAlign[0][2] = FALSE;

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_2) + 1:
			if (m_bFailAlign[0][2])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][2] = FALSE;
						m_bSkipAlign[0][2] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE;
							m_bDoneMk[0] = TRUE;
						}
						else
						{
							m_bDoMk[0] = TRUE;
							m_bDoneMk[0] = FALSE;
						}
					}
					else
					{
						m_bReAlign[0][2] = TRUE;
						m_bSkipAlign[0][2] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_2); // FourPointAlign0(2) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam0); // FourPointAlign0(2) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][2] = TRUE;
					m_bSkipAlign[0][2] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_2); // FourPointAlign0(2) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam0); // FourPointAlign0(2) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}
			if (m_bFailAlign[1][2])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][2] = FALSE;
						m_bSkipAlign[1][2] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE;
							m_bDoneMk[1] = TRUE;
						}
						else
						{
							m_bDoMk[1] = TRUE;
							m_bDoneMk[1] = FALSE;
						}
					}
					else
					{
						m_bReAlign[1][2] = TRUE;
						m_bSkipAlign[1][2] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_2); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
						Stop();
						//TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][2] = TRUE;
					m_bSkipAlign[1][2] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_2); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][2] || m_bFailAlign[1][2])
			{
				if (!m_bReAlign[0][2] && !m_bReAlign[1][2])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++;//m_nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
																 //m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_2); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
			}
			else
				m_nMkStAuto++;

			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt3()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move3Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(3))	// Move - Cam1 - Pt3
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI하면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(3))	// Move - Cam1 - Pt3
							m_nMkStAuto++;
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (m_bLastProc && m_nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(3))	// Move - Cam1 - Pt3
							m_nMkStAuto++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI상면 Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						m_nMkStAuto++;
					}
					else
					{
						if (MoveAlign1(3))	// Move - Cam1 - Pt3
							m_nMkStAuto++;
					}
				}
			}
										   break;
		case MK_ST + (Mk4PtIdx::Move3Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) :
			if (MoveAlign0(3))	// Move - Cam0 - Pt3
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_3) :	// 4PtAlign - Cam1 - Pt3
			if (!m_bFailAlign[1][0])
			{
				if (!m_bSkipAlign[1][1])
				{
					if (!m_bSkipAlign[1][2])
					{
						if (!m_bSkipAlign[1][3])
						{
							if (!FourPointAlign1(3))
								m_bFailAlign[1][3] = TRUE;
							else
								m_bFailAlign[1][3] = FALSE;
						}
						else
							m_bFailAlign[1][3] = FALSE;
					}
					else
						m_bFailAlign[1][3] = FALSE;
				}
				else
					m_bFailAlign[1][3] = FALSE;
			}
			else
				m_bFailAlign[1][3] = FALSE;

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_3) :	// 4PtAlign - Cam0 - Pt3
			if (!m_bFailAlign[0][0])
			{
				if (!m_bSkipAlign[0][1])
				{
					if (!m_bSkipAlign[0][2])
					{
						if (!m_bSkipAlign[0][3])
						{
							if (!FourPointAlign0(3))
								m_bFailAlign[0][3] = TRUE;
							else
								m_bFailAlign[0][3] = FALSE;
						}
						else
							m_bFailAlign[0][3] = FALSE;
					}
					else
						m_bFailAlign[0][3] = FALSE;
				}
				else
					m_bFailAlign[0][3] = FALSE;
			}
			else
				m_bFailAlign[0][3] = FALSE;

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_3) + 1:
			if (m_bFailAlign[0][3])
			{
				Buzzer(TRUE, 0);

				//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][3] = FALSE;
						m_bSkipAlign[0][3] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE;
							m_bDoneMk[0] = TRUE;
						}
						else
						{
							m_bDoMk[0] = TRUE;
							m_bDoneMk[0] = FALSE;
						}
					}
					else
					{
						m_bReAlign[0][3] = TRUE;
						m_bSkipAlign[0][3] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_3); // FourPointAlign0(3) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam0); // FourPointAlign0(3) 으로 진행. - 카메라 재정렬
						Stop();
						TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][3] = TRUE;
					m_bSkipAlign[0][3] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_3); // FourPointAlign0(3) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam0); // FourPointAlign0(3) 으로 진행. - 카메라 재정렬
					Stop();
					TowerLamp(RGB_YELLOW, TRUE);
				}
			}
			if (m_bFailAlign[1][3])
			{
				Buzzer(TRUE, 0);

				//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), MB_YESNO))
				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					//if(IDYES == DoMyMsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), MB_YESNO))
					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][3] = FALSE;
						m_bSkipAlign[1][3] = TRUE;
						//if(IDNO == DoMyMsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), MB_YESNO))
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE;
							m_bDoneMk[1] = TRUE;
						}
						else
						{
							m_bDoMk[1] = TRUE;
							m_bDoneMk[1] = FALSE;
						}
					}
					else
					{
						m_bReAlign[1][3] = TRUE;
						m_bSkipAlign[1][3] = FALSE;
						//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_3); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
						Stop();
						TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][3] = TRUE;
					m_bSkipAlign[1][3] = FALSE;
					//m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_3); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
					Stop();
					TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][3] || m_bFailAlign[1][3])
			{
				if (!m_bReAlign[0][3] && !m_bReAlign[1][3])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++;//m_nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
																 //m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_3); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
			}
			else
				m_nMkStAuto++;

			break;
		}
	}
}

void CManagerProcedure::Mk4PtMoveInitPos()
{
	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::MoveInitPt) :
			MoveInitPos0();
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::MoveInitPt) + 1:
			if (m_bDoMk[1])
				MoveInitPos1();
			else
				MoveMkEdPos1();
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::MoveInitPt) + 2:
			if (IsMoveDone())
				m_nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtElecChk()
{
	CString sRst;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::ChkElec) : // DoElecChk
			if (DoElecChk(sRst))
			{
				if (pDoc->WorkingInfo.Probing[0].bUse)
				{
					if (sRst == _T("Open"))
					{
						if (pDoc->WorkingInfo.Probing[0].bStopOnOpen)
							m_nMkStAuto = REJECT_ST;
						else
							m_nMkStAuto++;
					}
					else if (sRst == _T("Error"))
					{
						m_nMkStAuto = ERROR_ST;
					}
					else
					{
						m_nMkStAuto++;
					}
				}
				else
					m_nMkStAuto++;
			}
										   break;

		case MK_ST + (Mk4PtIdx::ChkElec) + 1:
			if (ChkLightErr())
			{
				m_bChkLightErr = FALSE;
				m_nMkStAuto++;
			}
			else
				m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);	// Mk 마킹 시작
			break;

		case MK_ST + (Mk4PtIdx::ChkElec) + 2:
			if (IsRun())
			{
				if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0) &&
					m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
				{
					if (MODE_INNER != pDoc->GetTestMode())
						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); 	// Mk 마킹 시작
					else
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Shift2Mk);
				}
				else
				{
					Stop();
					MsgBox(_T("마킹부 모션이 비활성화 되었습니다."));
					TowerLamp(RGB_RED, TRUE);
				}
			}
			else
			{
				if (!m_bChkLightErr)
				{
					m_bChkLightErr = TRUE;
					MsgBox(_T("노광불량 정지 - 기판을 확인하세요.\r\n계속진행하려면 운전스위치를 누르세요."));
				}
			}
			break;
		}
	}
}

void CManagerProcedure::Mk4PtDoMarking()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::DoMk) :				// Mk 마킹 시작
			if (pDoc->GetTestMode() == MODE_OUTER)
				SetMkIts(TRUE);						// ITS 마킹 시작
			else
				SetMk(TRUE);						// Mk 마킹 시작

			m_nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::DoMk) + 1:
			Sleep(100);
			m_nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::Verify) :
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = MK_ST + (Mk4PtIdx::Verify);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);		// Mk 마킹 시작
					m_bCam = TRUE;
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = MK_ST + (Mk4PtIdx::Verify);
							m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);		// Mk 마킹 시작
							m_bCam = TRUE;
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE;
							m_nMkStAuto++;	// Mk 마킹 완료

											//sMsg = _T("");
											//DispStsBar(sMsg, 0);
						}
					}
					else
					{
						m_nMkStAuto++;	// Mk 마킹 완료

										//sMsg = _T("");
										//DispStsBar(sMsg, 0);
					}
				}
			}
			else if (IsReMk())
			{
				m_nPrevMkStAuto = MK_ST + (Mk4PtIdx::Verify);
				m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);		// Mk 재시작
			}
			else
			{
				sMsg = _T("");
				sMsg += m_sDispSts[0];
				sMsg += _T(",");
				sMsg += m_sDispSts[1];
				//DispStsBar(sMsg, 0);
			}
										break;

		case MK_ST + (Mk4PtIdx::DoneMk) :	 // Align변수 초기화
			m_bReAlign[0][0] = FALSE; // [nCam][nPos] 
			m_bReAlign[0][1] = FALSE; // [nCam][nPos] 
			m_bReAlign[1][0] = FALSE; // [nCam][nPos] 
			m_bReAlign[1][1] = FALSE; // [nCam][nPos] 

			m_bSkipAlign[0][0] = FALSE; // [nCam][nPos] 
			m_bSkipAlign[0][1] = FALSE; // [nCam][nPos] 
			m_bSkipAlign[1][0] = FALSE; // [nCam][nPos] 
			m_bSkipAlign[1][1] = FALSE; // [nCam][nPos] 

			m_bFailAlign[0][0] = FALSE; // [nCam][nPos] 
			m_bFailAlign[0][1] = FALSE; // [nCam][nPos] 
			m_bFailAlign[1][0] = FALSE; // [nCam][nPos] 
			m_bFailAlign[1][1] = FALSE; // [nCam][nPos] 
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 1:  // Mk변수 초기화
			m_bDoMk[0] = TRUE;
			m_bDoMk[1] = TRUE;
			m_bDoneMk[0] = FALSE;
			m_bDoneMk[1] = FALSE;
			m_bReMark[0] = FALSE;
			m_bReMark[1] = FALSE;
			m_bCam = FALSE;
			m_nPrevMkStAuto = 0;

			for (a = 0; a < 2; a++)
			{
				for (b = 0; b < MAX_STRIP_NUM; b++)
				{
					m_nMkStrip[a][b] = 0;
					m_bRejectDone[a][b] = FALSE;
				}
			}

			m_nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 2:
			if (IsVs() && bDualTest)
			{
				if (m_nBufTot[1] < 4 && m_pBufSerial[1][m_nBufTot[1] - 1] < GetLotEndSerial())
				{
					Sleep(300);
					break;
				}
			}

			pView->MpeWrite(_T("MB440150"), 0);	// 마킹부 마킹중 ON (PC가 ON, OFF)
			pView->MpeWrite(_T("MB440170"), 1);	// 마킹완료(PLC가 확인하고 Reset시킴.)-20141029
			pDoc->LogAuto(_T("PC: 마킹완료(PLC가 확인하고 Reset시킴.)"));
			if (IsNoMk() || IsShowLive())
				ShowLive(FALSE);

			m_nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 3:
#ifdef USE_MPE
			if (pDoc->m_pMpeSignal[0] & (0x01 << 1))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
			{
				pDoc->LogAuto(_T("PLC: 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
				pView->MpeWrite(_T("MB440101"), 0);	// 마킹부 Feeding완료

				Shift2Mk();			// PCR 이동(Buffer->Marked) // 기록(WorkingInfo.LastJob.sSerial)
				UpdateRst();
				SetMkFdLen();

				SetCycTime();
				m_dwCycSt = GetTickCount();

				UpdateWorking();	// Update Working Info...
				ChkYield();
				m_nMkStAuto++;
			}
#endif
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 4:
			ChkLotCutPos();
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 5:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 6:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 7:
			m_nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 8:
			m_bMkSt = FALSE;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtLotDiff()
{
	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::LotDiff) :
			Stop();
			TowerLamp(RGB_YELLOW, TRUE);
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 1:
			//if(IDYES == DoMyMsgBox(_T("상면과 하면의 Lot가 다릅니다.\r\n계속 작업을 진행하시겠습니까?"), MB_YESNO))
			if (IDYES == MsgBox(_T("상면과 하면의 Lot가 다릅니다.\r\n계속 작업을 진행하시겠습니까?"), 0, MB_YESNO))
			{
				General.bContDiffLot = TRUE;
			}
			else
			{
				General.bContDiffLot = FALSE;
				m_bLotEnd = TRUE;
				m_nLotEndAuto = LOT_END;
			}
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 2:
			if (IsRun())
			{
				if (General.bContDiffLot)
					m_nMkStAuto = MK_ST + (Mk4PtIdx::ChkSn);
				else
					m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 3:
			General.bContDiffLot = FALSE;
			m_bLotEnd = TRUE;
			m_nLotEndAuto = LOT_END;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtReject()
{
	int a, b;

	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case REJECT_ST:
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();

			//if(IDYES == DoMyMsgBox(_T("쇼트 체크 불량입니다.\r\n리젝 처리를 진행하시겠습니까?"), MB_YESNO))
			if (IDYES == MsgBox(_T("쇼트 체크 불량입니다.\r\n리젝 처리를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				m_nMkStAuto++;
			}
			else
			{
				//if(IDYES == DoMyMsgBox(_T("리젝 처리를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), MB_YESNO))
				if (IDYES == MsgBox(_T("리젝 처리를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					m_nMkStAuto++;
				}
			}

			Buzzer(FALSE, 0);
			break;
		case REJECT_ST + 1:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE;
					m_nMkStAuto++;
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);	// Mk 마킹 시작
				}
				else
					m_nMkStAuto = REJECT_ST;
			}
			break;
		case REJECT_ST + 2:
			SetReject();
			m_nMkStAuto++;
			break;
		case REJECT_ST + 3:
			m_nMkStAuto++;
			break;
		case REJECT_ST + 4:
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = REJECT_ST + 4;
					m_nMkStAuto = REJECT_ST + 2;		// Mk 마킹 시작
					m_bCam = TRUE;

					m_bDoneMk[0] = FALSE;
					m_bDoneMk[1] = FALSE;

					for (a = 0; a < 2; a++)
					{
						for (b = 0; b < MAX_STRIP_NUM; b++)
						{
							m_nMkStrip[a][b] = 0;
							m_bRejectDone[a][b] = FALSE;
						}
					}
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = REJECT_ST + 4;
							m_nMkStAuto = REJECT_ST + 2;		// Mk 마킹 시작
							m_bCam = TRUE;
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE;
							m_nMkStAuto++;	// Mk 마킹 완료

											//sMsg = _T("");
											//DispStsBar(sMsg, 0);
						}
					}
					else
					{
						m_nMkStAuto++;	// Mk 마킹 완료

										//sMsg = _T("");
										//DispStsBar(sMsg, 0);
					}
				}
			}
			break;
		case REJECT_ST + 5:
			m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk);				// Align변수 초기화
			break;
		}
	}
}

void CManagerProcedure::Mk4PtErrStop()
{
	if (m_bMkSt && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case ERROR_ST:
			Stop();
			TowerLamp(RGB_RED, TRUE);

			//if(IDYES == DoMyMsgBox(_T("쇼트 체크 Error입니다.\r\n다시 쇼트 체크를 진행하시겠습니까?"), MB_YESNO))
			if (IDYES == MsgBox(_T("쇼트 체크 Error입니다.\r\n다시 쇼트 체크를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				m_nMkStAuto++;
			}
			else
			{
				//if(IDYES == DoMyMsgBox(_T("쇼트 체크를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), MB_YESNO))
				if (IDYES == MsgBox(_T("쇼트 체크를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					m_nMkStAuto++;
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 1:
			m_nMkStAuto++;
			break;
		case ERROR_ST + 2:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE;
					m_nMkStAuto = MK_ST + (Mk4PtIdx::ChkElec); // DoElecChk
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);	// Mk 마킹 시작
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 3:
			m_nMkStAuto = ERROR_ST;
			break;
		}
	}
}

void CManagerProcedure::MarkingWith4PointAlign()
{
	Mk4PtReady();
	Mk4PtChkSerial();
	Mk4PtInit();
	Mk4PtAlignPt0();
	Mk4PtAlignPt1();
	Mk4PtAlignPt2();
	Mk4PtAlignPt3();
	Mk4PtMoveInitPos();
	Mk4PtElecChk();
	Mk4PtDoMarking();
	Mk4PtLotDiff();
	Mk4PtReject();
	Mk4PtErrStop();
}

void CManagerProcedure::MakeResultMDS()
{
	if (pView->m_mgrReelmap)
		pView->m_mgrReelmap->MakeResultMDS();
}

BOOL CManagerProcedure::IsBuffer(int nNum)
{
	return pView->IsBuffer(nNum);
}

void CManagerProcedure::SetLastProc(int nFromMachine)
{
	if (!pView->m_mgrStatus)	return;
	stGeneral& General = (pView->m_mgrStatus->General);

	switch (nFromMachine)
	{
	case ID_AOIDN:
		General.nLotEndSerial = _tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
		General.bLastProcFromUp = FALSE;
		General.bLastProcFromEng = FALSE;
		General.bLastProc = TRUE;

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->m_bLastProc = TRUE;

		pView->MpeWrite(_T("MB440186"), 1);			// 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
		pView->MpeWrite(_T("MB440181"), 1);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
		pDoc->LogAuto(_T("PC: 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
		break;
	default:
		break;
	}
}

BOOL CManagerProcedure::IsSameUpDnLot()
{
	if (!pView->m_mgrStatus)	return FALSE;
	stPcrShare* PcrShare = (pView->m_mgrStatus->PcrShare);

	if (PcrShare[0].sLot == PcrShare[1].sLot)
		return TRUE;

	return FALSE;
}

BOOL CManagerProcedure::GetItsSerialInfo(int nItsSerial, BOOL &bDualTest, CString &sLot, CString &sLayerUp, CString &sLayerDn, int nOption)		// 내층에서의 ITS 시리얼의 정보
{
	TCHAR szData[512];
	CString str, sName, sPath, Path[3];

	Path[0] = WorkingInfo.System.sPathItsFile;
	Path[1] = WorkingInfo.LastJob.sModelUp;
	//Path[1] = m_sEngModel;
	Path[2] = m_sItsCode;

	sName.Format(_T("%s.txt"), pDoc->m_sItsCode);
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt

	if (sPath.IsEmpty())
		return FALSE;

	CString strTemp;
	CFileFind finder;
	if (finder.FindFile(sPath) == FALSE)
	{
		Path[1] = m_sEngModel;
		sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt
		if (finder.FindFile(sPath) == FALSE)
		{
			strTemp.Format(_T("GetItsSerialInfo - Didn't find file.\r\n%s"), sPath);
			pView->MsgBox(strTemp);
			return FALSE;
		}
	}

	if (nItsSerial == 0)
		nItsSerial = SearchFirstShotOnIts();

	CString sItsSerail;
	sItsSerail.Format(_T("%d"), nItsSerial);

	// ITS_Code.txt 파일의 정보
	//int nTestMode;
	//CString sProcessCode;


	// Option 1
	if (nOption == 0 || nOption == 1)
	{
		if (0 < ::GetPrivateProfileString(sItsSerail, _T("Dual Test"), NULL, szData, sizeof(szData), sPath))
			bDualTest = (_ttoi(szData) > 0) ? TRUE : FALSE;
		else
			bDualTest = FALSE;

		WorkingInfo.LastJob.bDualTestInner = bDualTest;
	}

	// Option 2
	if (nOption == 0 || nOption == 2)
	{
		if (0 < ::GetPrivateProfileString(sItsSerail, _T("Current Lot"), NULL, szData, sizeof(szData), sPath))
			sLot = CString(szData);
		else
			sLot = _T("");

		WorkingInfo.LastJob.sInnerLotUp = WorkingInfo.LastJob.sInnerLotDn = sLot;
	}

	// Option 3
	if (nOption == 0 || nOption == 3)
	{
		if (0 < ::GetPrivateProfileString(sItsSerail, _T("Current Layer Up"), NULL, szData, sizeof(szData), sPath))
			sLayerUp = CString(szData);
		else
			sLayerUp = _T("");

		WorkingInfo.LastJob.sInnerLayerUp = sLayerUp;
	}

	// Option 4
	if (nOption == 0 || nOption == 4)
	{
		if (bDualTest)
		{
			if (0 < ::GetPrivateProfileString(sItsSerail, _T("Current Layer Dn"), NULL, szData, sizeof(szData), sPath))
				sLayerDn = CString(szData);
			else
				sLayerDn = _T("");
		}
		else
			sLayerDn = _T("");

		WorkingInfo.LastJob.sInnerLayerDn = sLayerDn;
	}

	return TRUE;
}

BOOL CManagerProcedure::GetAoiUpInfo(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.11"));
		return 0;
	}

	BOOL Info0;//, Info1;
	Info0 = GetAoiInfoUp(nSerial, pNewLot, bFromBuf);

	if (Info0)
		return TRUE;

	return FALSE;
}

BOOL CManagerProcedure::GetAoiDnInfo(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.12"));
		return 0;
	}

	BOOL Info1;
	Info1 = GetAoiInfoDn(nSerial, pNewLot, bFromBuf);
	if (Info1)
		return TRUE;

	return FALSE;
}

BOOL CManagerProcedure::GetAoiInfoUp(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (!pView->m_mgrStatus)	return FALSE;
	stPcrShare* PcrShare = (pView->m_mgrStatus->PcrShare);
	stGeneral& General = (pView->m_mgrStatus->General);

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial < 1)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[0].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[0].sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[0].sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[0].sItsCode = sItsCode;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	if (PcrShare[0].sModel.IsEmpty() || PcrShare[0].sLayer.IsEmpty() || PcrShare[0].sLot.IsEmpty())
	{
		pView->MsgBox(_T("Error - Aoi Information."));
		return FALSE;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLotUp != PcrShare[0].sLot || WorkingInfo.LastJob.sEngItsCode != PcrShare[0].sItsCode)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sLotUp = PcrShare[0].sLot;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = PcrShare[0].sItsCode;
	}

	if (WorkingInfo.LastJob.sModelUp != PcrShare[0].sModel || WorkingInfo.LastJob.sLayerUp != PcrShare[0].sLayer || pView->m_bInitAutoLoadMstInfo)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sModelUp = PcrShare[0].sModel;
		WorkingInfo.LastJob.sLayerUp = PcrShare[0].sLayer;

		if (General.bBufEmptyF[0])
		{
			if (!General.bBufEmpty[0])
				General.bBufEmptyF[0] = FALSE;

			m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo();
			if (m_nAoiCamInfoStrPcs[0] > -1)
			{
				if ((m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != WorkingInfo.System.bStripPcsRgnBin)
				{
					return FALSE;
				}
			}

			pView->m_bInitAutoLoadMstInfo = FALSE;
			return TRUE;
		}

		if (pView->m_bInitAutoLoadMstInfo)
		{
			pView->m_bInitAutoLoadMstInfo = FALSE;
			return TRUE;
		}
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();

		if (m_pReelMapUp)
			m_pReelMapUp->ResetReelmapPath();

		if (GetTestMode() == MODE_OUTER)
		{
			BOOL bDualTestInner;
			CString sLot, sLayerUp, sLayerDn, str;
			if (!pDoc->GetItsSerialInfo(nSerial, bDualTestInner, sLot, sLayerUp, sLayerDn))
			{
				str.Format(_T("It is trouble to read GetItsSerialInfo()."));
				pView->MsgBox(str);
				return FALSE; // TRUE: CHANGED, FALSE: NO CHANGED 
			}

			if (m_pReelMapInnerUp)
				m_pReelMapInnerUp->ResetReelmapPath();

			if (bDualTestInner)
			{
				if (m_pReelMapInnerDn)
					m_pReelMapInnerDn->ResetReelmapPath();
				if (m_pReelMapInnerAllUp)
					m_pReelMapInnerAllUp->ResetReelmapPath();
				if (m_pReelMapInnerAllDn)
					m_pReelMapInnerAllDn->ResetReelmapPath();
			}
		}
	}

	return FALSE; // TRUE: CHANGED, FALSE: NO CHANGED 
}

BOOL CManagerProcedure::GetAoiInfoDn(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (!pView->m_mgrStatus)	return FALSE;
	stPcrShare* PcrShare = (pView->m_mgrStatus->PcrShare);
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return TRUE;

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;// , i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial < 1)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufDn, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[1].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[1].sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[1].sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	PcrShare[1].sItsCode = sItsCode;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	if (PcrShare[1].sModel.IsEmpty() || PcrShare[1].sLayer.IsEmpty() || PcrShare[1].sLot.IsEmpty())
	{
		pView->MsgBox(_T("Error - Aoi Information."));
		return FALSE;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLotDn != PcrShare[1].sLot || WorkingInfo.LastJob.sEngItsCode != PcrShare[1].sItsCode)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sLotDn = PcrShare[1].sLot;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = PcrShare[1].sItsCode;
	}

	if (WorkingInfo.LastJob.sModelDn != PcrShare[1].sModel || WorkingInfo.LastJob.sLayerDn != PcrShare[1].sLayer)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sModelDn = PcrShare[1].sModel;
		WorkingInfo.LastJob.sLayerDn = PcrShare[1].sLayer;

		if (General.bBufEmptyF[1])
		{
			if (!General.bBufEmpty[1])
				General.bBufEmptyF[1] = FALSE;

			m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo();
			if (m_nAoiCamInfoStrPcs[1] > -1)
			{
				if ((m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE) != WorkingInfo.System.bStripPcsRgnBin)
				{
					return FALSE;
				}
			}

			return TRUE;
		}
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();

		if (m_pReelMapDn)
			m_pReelMapDn->ResetReelmapPath();

		if (m_pReelMapAllUp)
			m_pReelMapAllUp->ResetReelmapPath();

		if (m_pReelMapAllDn)
			m_pReelMapAllDn->ResetReelmapPath();
	}

	return FALSE;
}
