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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	stGeneral& General = (pView->m_mgrStatus->General);

	int i = 0;

	General.bMkSt = FALSE;
	General.bLotEnd = FALSE;
	General.bLastProc = FALSE;
	General.nMkStAuto = 0;
	General.nLotEndAuto = 0;
	General.nLastProcAuto = 0;
	General.Flag = 0L;
	General.bAoiTestF[0] = FALSE;
	General.bAoiTestF[1] = FALSE;
	General.bAoiFdWriteF[0] = FALSE;
	General.bAoiFdWriteF[1] = FALSE;
	General.bEngTestF = FALSE;
	General.bEngFdWriteF = FALSE;
	General.bCycleStopF = FALSE;

	//m_bEngSt = FALSE;
	//m_bEngStSw = FALSE;
	//m_nEngStAuto = 0;

	//m_bEng2dSt = FALSE;
	//m_bEng2dStSw = FALSE;
	//m_nEng2dStAuto = 0;

	General.bPcrInShare[0] = FALSE;
	General.bPcrInShare[1] = FALSE;
	General.bPcrInShareVs[0] = FALSE;
	General.bPcrInShareVs[1] = FALSE;

	for (i = 0; i < MAX_DISP; i++)
	{
		m_bDispMsgDoAuto[i] = FALSE;
		m_nStepDispMsg[i] = 0;
	}

	m_bShift2Mk = FALSE;
	m_dTotVel = 0.0;
	m_dPartVel = 0.0;
	m_dwCycSt = 0;
	m_dwCycTim = 0;

	m_sFixMsg[0] = _T("");
	m_sFixMsg[1] = _T("");
	m_nSaveMk0Img = 0;
	m_nSaveMk1Img = 0;
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

int CManagerProcedure::GetAoiUpCamMstInfo() // AOI�� strpcs.bin ����
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

int CManagerProcedure::GetAoiDnCamMstInfo() // AOI�� strpcs.bin ����
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

BOOL CManagerProcedure::IsRun()
{
	return pView->IsRun();
	//return TRUE; // AlignTest
	//if (GetDispMain() == _T("������") || GetDispMain() == _T("�ʱ����") || GetDispMain() == _T("�ܸ����")
	//	|| GetDispMain() == _T("�ܸ�˻�") || GetDispMain() == _T("�����˻�") || GetDispMain() == _T("�����˻�")
	//	|| GetDispMain() == _T("���˻�") || GetDispMain() == _T("������"))
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
	//General.nPrevStepAuto = 0;
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

	//General.nBufUpSerial[0] = 0;
	//General.nBufUpSerial[1] = 0;
	//m_nBufUpCnt = 0;

	//General.nBufDnSerial[0] = 0;
	//General.nBufDnSerial[1] = 0;
	//m_nBufDnCnt = 0;

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
	//m_bDoneMk[0] = FALSE;		// [nCam]
	//m_bDoneMk[1] = FALSE;		// [nCam]
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

	General.bMkSt = FALSE;
	::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
	General.nMkStAuto = 0;

	//m_bEngSt = FALSE;
	//m_bEngStSw = FALSE;
	//m_nEngStAuto = 0;

	//m_bEng2dSt = FALSE;
	//m_bEng2dStSw = FALSE;
	//m_nEng2dStAuto = 0;

	General.bLotEnd = FALSE;
	General.nLotEndAuto = 0;

	General.bLastProc = FALSE;
	if (MODE_INNER != pDoc->GetTestMode())
		General.bLastProcFromUp = TRUE;
	else
		General.bLastProcFromEng = TRUE;

	General.nLastProcAuto = 0;

	pView->ClrAlarm();

	m_dwCycSt = 0;
	//m_sNewLotUp = _T("");
	//m_sNewLotDn = _T("");
	//m_nStop = 0;
	
	pView->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("0"));

	pView->MpeWrite(_T("MB440100"), 0); // PLC �����غ� �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
	pView->MpeWrite(_T("MB440110"), 0); // ��ŷ����(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141029
	pView->MpeWrite(_T("MB440150"), 0); // ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)
	pView->MpeWrite(_T("MB440170"), 0); // ��ŷ�Ϸ�(PLC�� Ȯ���ϰ� Reset��Ŵ.)-20141029

	InitAutoEngSignal();

	MoveInitPos1();
	Sleep(30);
	MoveInitPos0();

	InitIoWrite();
	SetTest(FALSE);	// �˻�� ��/�� �˻� ���� (Off)

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

	if (bInit) // �̾�Ⱑ �ƴѰ��.
	{
		pView->MpeWrite(_T("MB440187"), 0); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141121
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
		}
	}
	else
	{
		if (pDoc->GetTestMode() == MODE_INNER && pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == pView->MsgBox(_T("���κο��� ������ ���κ��� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				pView->MpeWrite(_T("MB440187"), 0); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-������ �������� ����
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("���κο��� 2D �ڵ带 �а� �� �� Last Shot�� Ȯ���Ͻðڽ��ϱ�?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					pView->MpeWrite(_T("MB440187"), 1); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-2D �ڵ� �б���� ����
				}
				else
				{
					pView->MpeWrite(_T("MB440187"), 0); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-������ �������� ����
				}
			}
		}
		else if (pDoc->GetTestMode() == MODE_INNER && !pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == pView->MsgBox(_T("���κο��� 2D �ڵ带 �а� �� �� Last Shot�� Ȯ���Ͻðڽ��ϱ�?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				pView->MpeWrite(_T("MB440187"), 1); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-2D �ڵ� �б���� ����
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("���κο��� ������ ���κ��� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					pView->MpeWrite(_T("MB440187"), 0); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-������ �������� ����
				}
				else
				{
					pView->MpeWrite(_T("MB440187"), 1); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-2D �ڵ� �б���� ����
				}
			}
		}
		else
		{
			pView->MpeWrite(_T("MB440187"), 1); // �̾��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141121
		}

		if(pView->m_mgrReelmap)
			pView->m_mgrReelmap->ClrFixPcs();

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

	// ��ŷ���� ��ȣ�� Ȯ��
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
	if (!pView->m_mgrStatus)	return FALSE;
	stGeneral& General = (pView->m_mgrStatus->General);
	stThread& Thread = (pView->m_mgrStatus->Thread);

	int nSerial;

	if(General.bLotEnd && General.nStepAuto < LOT_END)
	{
		General.bLotEnd = TRUE;
		General.nLotEndAuto = LOT_END;
	}

	if (!pView->IsBuffer(0) && General.bLastProc && General.nLotEndAuto < LOT_END)
	{
		General.bLotEnd = TRUE;
		General.nLotEndAuto = LOT_END;
	}
	else if (!pView->IsBuffer(0) && General.nMkStAuto > MK_ST + (Mk2PtIdx::DoneMk) + 4)
	{
		General.nMkStAuto = 0;
		General.bLotEnd = TRUE;
		General.nLotEndAuto = LOT_END;
	}


	if (General.bLotEnd)
	{
		//nSerial = pDoc->GetLastShotMk();

		switch (General.nLotEndAuto)
		{
		case LOT_END:
			if (!Thread.bTHREAD_REELMAP_YIELD_UP && !Thread.bTHREAD_REELMAP_YIELD_DN && !Thread.bTHREAD_REELMAP_YIELD_ALLUP && !Thread.bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				ReloadReelmap();
				UpdateRst();
				General.nLotEndAuto++;
			}
			break;
		case LOT_END + 1:
			pView->MpeWrite(_T("MB440180"), 1);			// �۾�����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141031
			DispMain(_T("�۾�����"), RGB_RED);
			General.nLotEndAuto++;
			break;
		case LOT_END + 2:
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			LotEnd();									// MakeResultMDS
			General.nLotEndAuto++;
			break;

		case LOT_END + 3:
			pView->MsgBox(_T("�۾��� ����Ǿ����ϴ�."));
			General.nLotEndAuto++;
			General.bLastProc = FALSE;
			General.bMkSt = FALSE;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case LOT_END + 4:
			break;
		}
	}

	return General.bLotEnd;
}

BOOL CManagerProcedure::DoAtuoGetMkStSignal()
{
	if (!pView->m_mgrStatus)	return FALSE;
	stGeneral& General = (pView->m_mgrStatus->General);

	if (!General.bMkSt)
	{
		if (IsRun())
		{
			if (pView->GetMkStSignal()) // AlignTest		// ��ŷ����(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141029
			{
				pDoc->LogAuto(_T("PLC: ��ŷ����(PC�� Ȯ���ϰ� Reset��Ŵ.)"));
				pView->ResetMkStSignal();

				if (pView->GetMpeSignal(0, 1))	// ��ŷ�� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)
				{
					pDoc->LogAuto(_T("PLC: Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)"));
					pView->MpeWrite(_T("MB440101"), 0);		// ��ŷ�� Feeding�Ϸ�
				}

				General.bMkSt = TRUE;
				General.nMkStAuto = MK_ST;
				::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("1"), PATH_WORKING_INFO);

				if (!pView->GetLastShotMk())
					pView->SetLotSt();		// ���ο� ��Ʈ�� ���۽ð��� ������. // ��ü�ӵ��� ó�� ���۵Ǵ� �ð� ����.

				return TRUE;
			}
		}
	}
	return FALSE;
}

void CManagerProcedure::DoAutoSetLastProcAtPlc()
{
	if (!pView->m_mgrStatus)	return;
	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.bLastProc)
	{
		switch (General.nLastProcAuto)
		{
		case LAST_PROC:	// �ܷ�ó�� 1
			if (IsRun())
			{
				if (MODE_INNER != pDoc->GetTestMode())
				{
					if (ChkLastProcFromUp())
					{
						pDoc->LogAuto(_T("PC: �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)"));
						pView->MpeWrite(_T("MB440185"), 1);			// �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141112
						Sleep(300);
						pView->MpeWrite(_T("MB44012B"), 1);			// AOI �� : PCR���� Received
					}
					else
					{
						pDoc->LogAuto(_T("PC: �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)"));
						pView->MpeWrite(_T("MB440186"), 1);			// �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141112
					}
				}
				else
				{
					if (ChkLastProcFromEng())
					{
						pDoc->LogAuto(_T("PC: �ܷ�ó�� ���κ� ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)"));
						pView->MpeWrite(_T("MB44019D"), 1);			// �ܷ�ó�� ���κ� ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141112
					}
					else if (ChkLastProcFromUp())
					{
						pDoc->LogAuto(_T("PC: �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)"));
						pView->MpeWrite(_T("MB440185"), 1);			// �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141112
						Sleep(300);
						pView->MpeWrite(_T("MB44012B"), 1);			// AOI �� : PCR���� Received
					}
					else
					{
						pDoc->LogAuto(_T("PC: �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)"));
						pView->MpeWrite(_T("MB440186"), 1);			// �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141112
					}
				}

				General.nLastProcAuto++;
			}
			break;
		case LAST_PROC + 1:
			pDoc->LogAuto(_T("PC: �ܷ�ó��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)"));
			pView->MpeWrite(_T("MB440181"), 1);			// �ܷ�ó��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141031
			General.nLastProcAuto++;
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

	BOOL bOn0 = pView->m_mgrFeeding->IsLoaderOnAoiUp();		// �˻�� �� �ڵ� ���� <-> X432B I/F
	BOOL bOn1 = pView->m_mgrFeeding->IsLoaderOnAoiDn();		// �˻�� �� �ڵ� ���� <-> X442B I/F

	if (bOn0 && !(General.Flag & (0x01 << 2)))
	{
		General.Flag |= (0x01 << 2);
	}
	else if (!bOn0 && (General.Flag & (0x01 << 2)))
	{
		General.Flag &= ~(0x01 << 2);

		General.bAoiTest[0] = FALSE;
		General.bWaitPcr[0] = FALSE;
		General.bAoiTestF[0] = FALSE;
		General.bAoiFdWriteF[0] = FALSE;
		pView->MpeWrite(_T("MB440111"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141103
		pDoc->LogAuto(_T("PLC: �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� MB440111 Reset��Ŵ.)"));
	}

	if (bOn1 && !(General.Flag & (0x01 << 3)))
	{
		General.Flag |= (0x01 << 3);
	}
	else if (!bOn1 && (General.Flag & (0x01 << 3)))
	{
		General.Flag &= ~(0x01 << 3);

		General.bAoiTest[1] = FALSE;
		General.bWaitPcr[1] = FALSE;
		General.bAoiTestF[1] = FALSE;
		General.bAoiFdWriteF[1] = FALSE;
		pView->MpeWrite(_T("MB440112"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141103
		pDoc->LogAuto(_T("PLC: �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� MB440112 Reset��Ŵ.)"));
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

	if (bOn0 && !General.bAoiTestF[0])		// �˻��(��) �˻���
	{
		General.bAoiTestF[0] = TRUE;
		General.bAoiTest[0] = TRUE;
		General.bWaitPcr[0] = TRUE;
	}
	else if (!bOn0 && General.bAoiTestF[0])
	{
		General.bAoiTestF[0] = FALSE;
		General.bAoiFdWriteF[0] = FALSE;
		General.bAoiTest[0] = FALSE;

		pView->MpeWrite(_T("MB440111"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141103
		pDoc->LogAuto(_T("PLC: �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� MB440111 Reset��Ŵ.)"));
	}

	if (bOn1 && !General.bAoiTestF[1])		// �˻��(��) �˻���
	{
		General.bAoiTestF[1] = TRUE;
		General.bAoiTest[1] = TRUE;
		General.bWaitPcr[1] = TRUE;
	}
	else if (!bOn1 && General.bAoiTestF[1])
	{
		General.bAoiTestF[1] = FALSE;
		General.bAoiFdWriteF[1] = FALSE;
		General.bAoiTest[1] = FALSE;
		pView->MpeWrite(_T("MB440112"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
		pDoc->LogAuto(_T("PLC: �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� MB440112 Reset��Ŵ.)"));
	}


	BOOL bOn2 = pView->m_mgrFeeding->IsDoneWriteFdOffsetAoiUp();
	BOOL bOn3 = pView->m_mgrFeeding->IsDoneWriteFdOffsetAoiDn();

	if (bOn2 && !General.bAoiFdWrite[0])		// �˻��(��) Feeding Offset Write �Ϸ�
		General.bAoiFdWrite[0] = TRUE;
	else if (!bOn2 && General.bAoiFdWrite[0])
		General.bAoiFdWrite[0] = FALSE;

	if (bOn3 && !General.bAoiFdWrite[1])		// �˻��(��) Feeding Offset Write �Ϸ�
		General.bAoiFdWrite[1] = TRUE;
	else if (!bOn3 && General.bAoiFdWrite[1])
		General.bAoiFdWrite[1] = FALSE;


	if (bDualTest)
	{
		if ((General.bAoiFdWrite[0] && General.bAoiFdWrite[1]) && (!General.bAoiFdWriteF[0] && !General.bAoiFdWriteF[1]))
		{
			General.bAoiFdWriteF[0] = TRUE;
			General.bAoiFdWriteF[1] = TRUE;

			pDoc->GetAoiUpOffset(OfStUp);
			pDoc->GetAoiDnOffset(OfStDn);

			dAveX = OfStUp.x;
			dAveY = OfStUp.y;

			pView->SetAoiUpOffset(OfStUp);
			pView->SetAoiDnOffset(OfStDn);

			pView->MpeWrite(_T("ML45064"), (long)(-1.0*dAveX*1000.0));
			pView->MpeWrite(_T("MB440111"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
			pView->MpeWrite(_T("MB440112"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
			pDoc->LogAuto(_T("PLC: �˻��(��MB440111,��MB440112) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)"));
		}
		else if ((!General.bAoiFdWrite[0] && !General.bAoiFdWrite[1]) && (General.bAoiFdWriteF[0] && General.bAoiFdWriteF[1]))
		{
			General.bAoiFdWriteF[0] = FALSE;
			General.bAoiFdWriteF[1] = FALSE;
			General.bAoiTest[0] = FALSE;
			General.bAoiTest[1] = FALSE;
		}

		if (General.bAoiTest[0] && !General.bAoiTest[1])
		{
			if (General.bAoiFdWrite[0] && !General.bAoiFdWriteF[0])
			{
				General.bAoiFdWriteF[0] = TRUE;

				OfStDn.x = 0.0; OfStDn.y = 0.0;
				pDoc->GetAoiUpOffset(OfStUp);
				pView->SetAoiUpOffset(OfStUp);
				pView->SetAoiDnOffset(OfStDn);

				pView->MpeWrite(_T("ML45064"), (long)(-1.0*OfStUp.x*1000.0));
				pView->MpeWrite(_T("MB440111"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
				pDoc->LogAuto(_T("PLC: �˻��(��MB440111) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)"));
			}
			else if (!General.bAoiFdWrite[0] && General.bAoiFdWriteF[0])
			{
				General.bAoiFdWriteF[0] = FALSE;
				General.bAoiTest[0] = FALSE;
			}
		}

		if (!General.bAoiTest[0] && General.bAoiTest[1])
		{
			if (General.bAoiFdWrite[1] && !General.bAoiFdWriteF[1])
			{
				General.bAoiFdWriteF[1] = TRUE;

				OfStUp.x = 0.0; OfStUp.y = 0.0;
				pDoc->GetAoiDnOffset(OfStDn);
				pView->SetAoiUpOffset(OfStUp);
				pView->SetAoiDnOffset(OfStDn);

				pView->MpeWrite(_T("ML45064"), (long)(-1.0*OfStDn.x*1000.0));
				pView->MpeWrite(_T("MB440112"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141103
				pDoc->LogAuto(_T("PLC: �˻��(��MB440112) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)"));
			}
			else if (!General.bAoiFdWrite[1] && General.bAoiFdWriteF[1])
			{
				General.bAoiFdWriteF[1] = FALSE;
				General.bAoiTest[1] = FALSE;
			}
		}
	}
	else
	{
		if (General.bAoiFdWrite[0] && !General.bAoiFdWriteF[0])
		{
			General.bAoiFdWriteF[0] = TRUE;

			pDoc->GetAoiUpOffset(OfStUp);
			pView->SetAoiUpOffset(OfStUp);

			dAveX = (OfStUp.x);
			dAveY = (OfStUp.y);

			pView->MpeWrite(_T("ML45064"), (long)(-1.0*dAveX*1000.0));	// �˻�� Feeding �ѷ� Offset(*1000, +:�� ����, -�� ����)
			pView->MpeWrite(_T("MB440111"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141103
			pView->MpeWrite(_T("MB440112"), 0); // �˻��(��) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)-20141103  // 20160721-syd-temp
			pDoc->LogAuto(_T("PLC: �˻��(��MB440111,��MB440112) Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)"));
		}
		else if (!General.bAoiFdWrite[0] && General.bAoiFdWriteF[0])
		{
			General.bAoiFdWriteF[0] = FALSE;
			General.bAoiTest[0] = FALSE;
		}
	}
}

void CManagerProcedure::DoAutoSetFdOffsetEngrave()
{
	if (!pView->m_mgrStatus || !pView->m_mgrFeeding)
		return;

	stGeneral& General = (pView->m_mgrStatus->General);
	stBtnStatus& BtnStatus = (pView->m_mgrFeeding->BtnStatus);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	double dAveX, dAveY;
	CfPoint OfSt;

	if ((BtnStatus.EngAuto.IsOnMking && !General.bEngTestF) || (BtnStatus.EngAuto.IsOnRead2d && !General.bEngTestF)) // ���κ� ������
	{
		General.bEngTestF = TRUE;
		General.bEngTest = TRUE;
	}
	else if ((!BtnStatus.EngAuto.IsOnMking && General.bEngTestF) || (!BtnStatus.EngAuto.IsOnRead2d && General.bEngTestF))
	{
		General.bEngTestF = FALSE;
		General.bEngTest = FALSE;
		General.bEngFdWriteF = FALSE;
		pView->MpeWrite(_T("MB44011A"), 0); // ���κ� Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
		pDoc->LogAuto(_T("PC: ���κ� Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)"));
	}

	BOOL bOn = pView->m_mgrFeeding->IsDoneWriteFdOffsetEng();

	if (bOn && !General.bEngFdWrite)		// ���κ� Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
	{
		General.bEngFdWrite = TRUE;
	}
	else if (!bOn && General.bEngFdWrite)
	{
		General.bEngFdWrite = FALSE;
	}

	if (General.bEngFdWrite && !General.bEngFdWriteF)
	{
		General.bEngFdWriteF = TRUE;

		pDoc->GetEngOffset(OfSt);

		dAveX = OfSt.x;
		dAveY = OfSt.y;

		pView->SetEngOffset(OfSt);

		pDoc->LogAuto(_T("PC: ���κ� Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)"));
		pView->MpeWrite(_T("ML45078"), (long)(dAveX*1000.0));	// ���κ� Feeding �ѷ� Offset(*1000, +:�� ����, -�� ����, PC�� ���� PLC���� ����)
		pView->MpeWrite(_T("MB44011A"), 0);						// ���κ� Feeding Offset Write �Ϸ�(PC�� Ȯ���ϰ� Reset��Ŵ.)
		Sleep(10);
	}
	else if (!General.bEngFdWrite && General.bEngFdWriteF)
	{
		General.bEngFdWriteF = FALSE;
		General.bEngTest = FALSE;
	}
}

void CManagerProcedure::DoAutoChkCycleStop()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	CManagerFeeding* pMgrFd = (pView->m_mgrFeeding);

	if (General.bCycleStop && !General.bCycleStopF)
	{
		General.bCycleStopF = TRUE;
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
	else if (!General.bCycleStop && General.bCycleStopF)
	{
		General.bCycleStopF = FALSE;
	}
}

void CManagerProcedure::DoAutoDispMsg()
{
	BOOL bDispMsg = FALSE;
	int idx, nStepDispMsg;

	// [2] : �����ҷ�-��, [3] : �����ҷ�-��, [5] : nSerialL <= 0, [6] : CopyDefImg ���� Camera,  
	// [7] : CopyDefImg ���� Camera, [8] : ���̽�����(��) �ʱ���ġ �̵�, [9] : ���̽�����(��) �ʱ���ġ �̵�
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
			DispMain(_T("�� ��"), RGB_RED);
			break;
		case FROM_DOMARK1:
			m_bDispMsgDoAuto[9] = FALSE;
			m_nStepDispMsg[9] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			DispMain(_T("�� ��"), RGB_RED);
			break;
		case FROM_DISPDEFIMG:
			m_bDispMsgDoAuto[0] = FALSE;
			m_nStepDispMsg[0] = 0;
			pView->Stop();
			pView->MsgBox(_T("����(��) Serial�� �����ʽ��ϴ�."));
			break;
		case FROM_DISPDEFIMG + 1:
			m_bDispMsgDoAuto[1] = FALSE;
			m_nStepDispMsg[1] = 0;
			pView->Stop();
			pView->MsgBox(_T("����(��) Serial�� �����ʽ��ϴ�."));
			break;
		case FROM_DISPDEFIMG + 2: // IsFixUp
			m_bDispMsgDoAuto[2] = FALSE;
			m_nStepDispMsg[2] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			DispMain(_T("�� ��"), RGB_RED);
			pView->MsgBox(m_sFixMsg[0]);
			m_sFixMsg[0] = _T("");
			break;
		case FROM_DISPDEFIMG + 3: // IsFixDn
			m_bDispMsgDoAuto[3] = FALSE;
			m_nStepDispMsg[3] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			DispMain(_T("�� ��"), RGB_RED);
			pView->MsgBox(m_sFixMsg[1]);
			m_sFixMsg[1] = _T("");
			break;
		case FROM_DISPDEFIMG + 4:
			//m_bDispMsgDoAuto[4] = FALSE;
			//m_nStepDispMsg[4] = 0;
			//Stop();
			//Buzzer(TRUE, 0);
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			//pView->DispStsBar(_T("����-41"), 0);
			//DispMain(_T("�� ��"), RGB_RED);	
			//MsgBox(m_sFixMsg);
			//m_sFixMsg = _T("");
			break;
		case FROM_DISPDEFIMG + 5:
			m_bDispMsgDoAuto[5] = FALSE;
			m_nStepDispMsg[5] = 0;
			pView->Stop();
			pView->MsgBox(_T("����(��) Serial�� �����ʽ��ϴ�."));
			break;
		case FROM_DISPDEFIMG + 6:
			m_bDispMsgDoAuto[6] = FALSE;
			m_nStepDispMsg[6] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			DispMain(_T("�� ��"), RGB_RED);
			break;
		case FROM_DISPDEFIMG + 7:
			m_bDispMsgDoAuto[7] = FALSE;
			m_nStepDispMsg[7] = 0;
			pView->Buzzer(TRUE, 0);
			pView->Stop();
			//m_bSwStopNow = TRUE;
			//m_bSwRunF = FALSE;
			DispMain(_T("�� ��"), RGB_RED);
			break;
		}
	}
}

void CManagerProcedure::DoAutoChkShareVsFolder()	// �ܷ�ó�� �� ��������� �ݺ��ؼ� ���Լ��� ȣ������� �¿� ��ŷ �ε��� ���� ���� �߻�.(case AT_LP + 8:)
{
	if (!pView->m_mgrStatus || pView->m_mgrFeeding)		return;
	if (!pView->m_mgrReelmap)							return;

	CCamMaster* pMaster = (pView->m_mgrReelmap->m_Master);
	CCamMaster* pMasterInner = (pView->m_mgrReelmap->m_MasterInner);
	stGeneral& General = (pView->m_mgrStatus->General);
	stBtnPush& BtnPush = (pView->m_mgrFeeding->Btn);
	stThread& Thread = (pView->m_mgrStatus->Thread);
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
		if (IsReady() || IsAuto())		// �����غ�
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
		if (IsRun())		// �ʱ����
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
						if (GetDispMain() != _T("������"))
							DispMain(_T("������"), RGB_GREEN);
					}
					else
					{
						if (GetDispMain() != _T("�ܸ����"))
							DispMain(_T("�ܸ����"), RGB_GREEN);
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (GetDispMain() != _T("�����˻�"))
						DispMain(_T("�����˻�"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (GetDispMain() != _T("�����˻�"))
						DispMain(_T("�����˻�"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (GetDispMain() != _T("���˻�"))
						DispMain(_T("���˻�"), RGB_GREEN);
				}
				else
				{
					if (GetDispMain() != _T("�ܸ�˻�"))
						DispMain(_T("�ܸ�˻�"), RGB_GREEN);
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
			General.bPcrInShare[0] = FALSE;
			General.bPcrInShare[1] = FALSE;

			if (IsShareUp()) // �˻��(��) �˻���
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
							General.bPcrInShare[0] = TRUE;
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
							General.bPcrInShare[0] = TRUE;
						}
					}
				}
			}

			if (bDualTest)
			{
				if (IsShareDn()) // �˻��(��) �˻���
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
								General.bPcrInShare[1] = TRUE;
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
								General.bPcrInShare[1] = TRUE;
							}
						}
					}
				}

				if (General.bPcrInShare[0] || General.bPcrInShare[1])
					General.nStepAuto++;
			}
			else
			{
				if (General.bPcrInShare[0])
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
		Shift2DummyBuf();			// PCR �̵�(Share->Buffer)
		General.nStepAuto = AT_LP + (AtLpVsIdx::ChkShare);
		break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) :
		if (IsShareVs()) // ChkShareVs()
		{
			General.bPcrInShareVs[0] = FALSE;
			General.bPcrInShareVs[1] = FALSE;

			if (IsShareVsUp()) // �˻��(��) �˻���
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
							General.bPcrInShareVs[0] = TRUE;
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
							General.bPcrInShareVs[0] = TRUE;
						}
					}
				}
			}

			if (bDualTest)
			{
				if (IsShareVsDn()) // �˻��(��) �˻���
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
								General.bPcrInShareVs[1] = TRUE;
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
								General.bPcrInShareVs[1] = TRUE;
							}
						}
					}
				}

				if (General.bPcrInShareVs[0] || General.bPcrInShareVs[1])
					General.nStepAuto++;
			}
			else
			{
				if (General.bPcrInShareVs[0])
					General.nStepAuto++;
			}
		}
		else // if (IsShare())
		{
			if (!General.bLastProc)
			{
				if (ChkLastProc())
				{
					General.nLastProcAuto = LAST_PROC;
					General.bLastProc = TRUE;
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
			else // if (!General.bLastProc)
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

				General.bWaitPcr[0] = FALSE; // ���� �����ؾ� �� flag
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
		if (!General.bCont) // �̾�� �ƴ� ���.
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

		Shift2Buf();			// PCR �̵�(Share->Buffer)
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

			bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer���� PCR������ ��� ������ ����.

			if (bNewModel)	// AOI ����(AoiCurrentInfoPath) -> AOI Feeding Offset
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
				InitCamImgDisp();
			}
			else
			{
				if (m_nShareUpS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo();
					if ((pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE)
							pView->MsgBox(_T("���� ��ŷ�δ� �Ϲ� ��� �ε�, \r\n��� AOI�� DTS ��忡�� �˻縦 �����Ͽ����ϴ�."));
						else
							pView->MsgBox(_T("���� ��ŷ�δ� DTS ��� �ε�, \r\n��� AOI�� �Ϲ� ��忡�� �˻縦 �����Ͽ����ϴ�."));

						Stop();
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (!bDualTest)
					OpenReelmapFromBuf(m_nShareUpS);
			}

			LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share

			if (ListBuf[0].nTot <= ListBuf[1].nTot)
			{
				UpdateReelmap(m_nShareUpS); // �ø��������� ������ ������ ����
			}

			if (!General.bLastProc)
			{
				if (ChkLastProc())
				{
					General.nLastProcAuto = LAST_PROC;
					General.bLastProc = TRUE;

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
				m_nShareDnSerial[0] = m_nShareDnS; // Ȧ��
			else
				m_nShareDnSerial[1] = m_nShareDnS; // ¦��
			m_nShareDnCnt++;


			bNewModel = GetAoiDnInfo(m_nShareDnS, &nNewLot);

			if (bNewModel)	// AOI ����(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//MsgBox(_T("�ű� �𵨿� ���� AOI(��)���� ��Ʈ �и��� �Ǿ����ϴ�.\r\n���� ��Ʈ�� �ܷ�ó�� �մϴ�.");
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
							pView->MsgBox(_T("���� ��ŷ�δ� �Ϲ� ��� �ε�, \r\n�ϸ� AOI�� DTS ��忡�� �˻縦 �����Ͽ����ϴ�."));
						else
							pView->MsgBox(_T("���� ��ŷ�δ� DTS ��� �ε�, \r\n�ϸ� AOI�� �Ϲ� ��忡�� �˻縦 �����Ͽ����ϴ�."));

						Stop();
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (bDualTest)
					OpenReelmapFromBuf(m_nShareDnS);
			}

			LoadPcrDn(m_nShareDnS);

			if (ListBuf[1].nTot <= ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // �ø��������� ������ ������ ����
			}

			if (!General.bLastProc)
			{
				if (ChkLastProc())
				{
					General.nLastProcAuto = LAST_PROC;
					General.bLastProc = TRUE;

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

	stGeneral& General = (pView->m_mgrStatus->General);
	stListBuf* ListBuf = pView->m_mgrStatus->ListBuf;
	stThread& Thread = (pView->m_mgrStatus->Thread);
	CCamMaster* pMaster = (pView->m_mgrReelmap->m_Master);
	CCamMaster* pMasterInner = (pView->m_mgrReelmap->m_MasterInner);
	stBtnPush& BtnPush = (pView->m_mgrFeeding->Btn);

	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = 0;
	//CString sNewLot;
	int nNewLot = 0;
	//BOOL General.bPcrInShare[2];
	BOOL bNewModel = FALSE;

	switch (General.nStepAuto)
	{
	case 0:
		BtnPush.bSwRun = FALSE;
		BtnPush.bSwStop = TRUE;
		General.nStepAuto++;
		break;
	case 1:
		if (pView->IsReady() || pView->IsAuto())		// �����غ�
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
		if (IsRun())		// �ʱ����
		{
			if (!IsAoiLdRun())
			{
				Stop();
			}
			else
			{
				ResetWinker(); // 20151126 : ��������ġ ���� ���� ������ ����.

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (pDoc->WorkingInfo.LastJob.bDualTest)
					{
						if (GetDispMain() != _T("������"))
							DispMain(_T("������"), RGB_GREEN);
					}
					else
					{
						if (GetDispMain() != _T("�ܸ����"))
							DispMain(_T("�ܸ����"), RGB_GREEN);
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (GetDispMain() != _T("�����˻�"))
						DispMain(_T("�����˻�"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (GetDispMain() != _T("�����˻�"))
						DispMain(_T("�����˻�"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (GetDispMain() != _T("���˻�"))
						DispMain(_T("���˻�"), RGB_GREEN);
				}
				else
				{
					if (GetDispMain() != _T("�ܸ�˻�"))
						DispMain(_T("�ܸ�˻�"), RGB_GREEN);
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
			General.bPcrInShare[0] = FALSE;
			General.bPcrInShare[1] = FALSE;

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
							General.bPcrInShare[0] = TRUE;
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
							General.bPcrInShare[0] = TRUE;
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
								General.bPcrInShare[1] = TRUE;
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
								General.bPcrInShare[1] = TRUE;
							}
						}
					}
				}

				if (General.bPcrInShare[0] || General.bPcrInShare[1])
					General.nStepAuto++;
			}
			else
			{
				if (General.bPcrInShare[0])
					General.nStepAuto++;
			}
		}
		else // if (IsShare())
		{
			if (!General.bLastProc)
			{
				if (ChkLastProc())
				{
					General.nLastProcAuto = LAST_PROC;
					General.bLastProc = TRUE;
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
		if (!General.bCont) // �̾�� �ƴ� ���.
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

		Shift2Buf();			// PCR �̵�(Share->Buffer)
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

			bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer���� PCR������ ��� ������ ����.

			if (bNewModel)	// AOI ����(AoiCurrentInfoPath) -> AOI Feeding Offset
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
				InitCamImgDisp();
			}
			else
			{
				if (m_nShareUpS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo();
					if ((pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE)
							pView->MsgBox(_T("���� ��ŷ�δ� �Ϲ� ��� �ε�, \r\n��� AOI�� DTS ��忡�� �˻縦 �����Ͽ����ϴ�."));
						else
							pView->MsgBox(_T("���� ��ŷ�δ� DTS ��� �ε�, \r\n��� AOI�� �Ϲ� ��忡�� �˻縦 �����Ͽ����ϴ�."));

						Stop();
						break;

					}
				}
			}

			if (nNewLot)
			{
				if (!bDualTest)
					OpenReelmapFromBuf(m_nShareUpS);
			}

			LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share

			if (ListBuf[0].nTot <= ListBuf[1].nTot)
			{
				UpdateReelmap(m_nShareUpS); // �ø��������� ������ ������ ����
			}

			if (!General.bLastProc)
			{
				if (ChkLastProc())
				{
					General.nLastProcAuto = LAST_PROC;
					General.bLastProc = TRUE;

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
				m_nShareDnSerial[0] = m_nShareDnS; // Ȧ��
			else
				m_nShareDnSerial[1] = m_nShareDnS; // ¦��
			m_nShareDnCnt++;


			bNewModel = GetAoiDnInfo(m_nShareDnS, &nNewLot);

			if (bNewModel)	// AOI ����(AoiCurrentInfoPath) -> AOI Feeding Offset
			{
				//MsgBox(_T("�ű� �𵨿� ���� AOI(��)���� ��Ʈ �и��� �Ǿ����ϴ�.\r\n���� ��Ʈ�� �ܷ�ó�� �մϴ�.");
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
							pView->MsgBox(_T("���� ��ŷ�δ� �Ϲ� ��� �ε�, \r\n�ϸ� AOI�� DTS ��忡�� �˻縦 �����Ͽ����ϴ�."));
						else
							pView->MsgBox(_T("���� ��ŷ�δ� DTS ��� �ε�, \r\n�ϸ� AOI�� �Ϲ� ��忡�� �˻縦 �����Ͽ����ϴ�."));

						Stop();
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (bDualTest)
					OpenReelmapFromBuf(m_nShareDnS);
			}

			LoadPcrDn(m_nShareDnS);

			if (ListBuf[1].nTot <= ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // �ø��������� ������ ������ ����
			}

			if (!General.bLastProc)
			{
				if (ChkLastProc())
				{
					General.nLastProcAuto = LAST_PROC;
					General.bLastProc = TRUE;

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
		AfxMessageBox(_T("��ŷ�� ���� Align����� �������� �ʾҽ��ϴ�."));
	}
}

void CManagerProcedure::DoAutoMarkingEngrave()
{
	if (!pView->m_mgrFeeding) return;
	stBtnStatus& BtnStatus = (pView->m_mgrFeeding->BtnStatus);

	//if (BtnStatus.EngAuto.IsOnMking && !(m_pMpe->m_pMpeSignal[6] & (0x01 << 3))) // ���κ� ��ŷ�� ON (PC�� ON, OFF)
	if (BtnStatus.EngAuto.IsOnMking && !(pView->m_mgrFeeding->GetMpeSignal(6, 3))) // ���κ� ��ŷ�� ON (PC�� ON, OFF)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, TRUE);
		pDoc->LogAuto(_T("PC: 2D(GUI) ���� ����Running��ȣ ON (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440173"), 1); // 2D(GUI) ���� ����Running��ȣ(PC On->PC Off)
	}
	else if (!BtnStatus.EngAuto.IsOnMking && (pView->m_mgrFeeding->GetMpeSignal(6, 3)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
		pDoc->LogAuto(_T("PC: 2D(GUI) ���� ����Running��ȣ OFF (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440173"), 0); // 2D(GUI) ���� ����Running��ȣ(PC On->PC Off)
	}

	// ���κ� ��ŷ�Ϸ� ON (PC�� ON, OFF)
	if (BtnStatus.EngAuto.IsMkDone && !(pView->m_mgrFeeding->GetMpeSignal(6, 4))) // ���κ� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, TRUE);
		pDoc->LogAuto(_T("PC: ���κ� �۾��Ϸ� ON (PC�� On, PLC�� Ȯ�� �� Off)"));
		pView->MpeWrite(_T("MB440174"), 1); // ���κ� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)
	}
	else if (!BtnStatus.EngAuto.IsMkDone && (pView->m_mgrFeeding->GetMpeSignal(6, 4))) // ���κ� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, FALSE);
	}

	// ���κ� 2D ���� �۾��� ��ȣ
	if (BtnStatus.EngAuto.IsOnRead2d && !(pView->m_mgrFeeding->GetMpeSignal(6, 8))) // ���κ� 2D ���� �۾��� ��ȣ(PC On->PC Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, TRUE);
		pDoc->LogAuto(_T("PC: ���κ� 2D ���� �۾��� ��ȣ ON (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440178"), 1); // ���κ� 2D ���� �۾��� ��ȣ(PC On->PC Off)
	}
	else if (!BtnStatus.EngAuto.IsOnRead2d && (pView->m_mgrFeeding->GetMpeSignal(6, 8)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
		pDoc->LogAuto(_T("PC: ���κ� 2D ���� �۾��� ��ȣ OFF (PC On->PC Off)"));
		pView->MpeWrite(_T("MB440178"), 0); // ���κ� 2D ���� �۾��� ��ȣ(PC On->PC Off)
	}

	// ���κ� 2D ���� �۾��Ϸ� ��ȣ
	if (BtnStatus.EngAuto.IsRead2dDone && !(pView->m_mgrFeeding->GetMpeSignal(6, 9))) // ���κ� 2D ���� �۾��Ϸ� ��ȣ.(PC�� On, PLC�� Ȯ�� �� Off)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, TRUE);
		pDoc->LogAuto(_T("PC: ���κ� 2D ���� �۾��Ϸ� ��ȣ ON (PC�� On, PLC�� Ȯ�� �� Off)"));
		pView->MpeWrite(_T("MB440179"), 1); // ���κ� 2D ���� �۾��Ϸ� ��ȣ.(PC�� On, PLC�� Ȯ�� �� Off)
	}
	else if (!BtnStatus.EngAuto.IsRead2dDone && (pView->m_mgrFeeding->GetMpeSignal(6, 9)))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, FALSE);
	}

}

BOOL CManagerProcedure::IsLotEnd()
{
	if (!pView->m_mgrStatus)	return FALSE;
	stGeneral& General = (pView->m_mgrStatus->General);

	return General.bLotEnd;
}

BOOL CManagerProcedure::ReloadReelmap()
{
	if (!pView->m_mgrThread)	return FALSE;
	return pView->m_mgrThread->ReloadReelmap();
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

	//if (m_pReelMap == m_pReelMapUp)
	//{
	//	DuplicateRmap(RMAP_UP);
	//}
	//else if (m_pReelMap == m_pReelMapAllUp)
	//{
	//	DuplicateRmap(RMAP_ALLUP);
	//}
	//else if (m_pReelMap == m_pReelMapIts)
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
	if (!pView->m_mgrStatus || !pView->m_mgrFeeding)	return;
	stGeneral& General = (pView->m_mgrStatus->General);
	stBtnStatus& BtnStatus = (pView->m_mgrFeeding->BtnStatus);
	BtnStatus.EngAuto._Init();

	//m_bEngFdWrite = FALSE;
	General.bEngFdWriteF = FALSE;
	//m_bEngTest = FALSE;
	General.bEngTestF = FALSE;

	pView->MpeWrite(_T("MB440103"), 0); // 2D(GUI) ���� ���� Start��ȣ(PLC On->PC Off)
	pView->MpeWrite(_T("MB440173"), 0); // 2D(GUI) ���� ����Running��ȣ(PC On->PC Off)
	pView->MpeWrite(_T("MB440174"), 0); // ���κ� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)

	pView->MpeWrite(_T("MB440105"), 0); // ���κ� 2D ���� ���۽�ȣ(PLC On->PC Off)
	pView->MpeWrite(_T("MB440178"), 0); // ���κ� 2D ���� �۾��� ��ȣ(PC On->PC Off)
	pView->MpeWrite(_T("MB440179"), 0); // ���κ� 2D ���� �۾��Ϸ� ��ȣ.(PC�� On, PLC�� Ȯ�� �� Off)

	pView->MpeWrite(_T("MB440102"), 0); // ���κ� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)

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

void CManagerProcedure::MoveMkEdPos1()
{
	if (pView->m_mgrPunch)
		pView->m_mgrPunch->MoveMkEdPos1();
}

BOOL CManagerProcedure::IsInitPos0()
{
	if (pView->m_mgrPunch)
		return pView->m_mgrPunch->IsInitPos0();
	return FALSE;
}

BOOL CManagerProcedure::IsInitPos1()
{
	if (pView->m_mgrPunch)
		return pView->m_mgrPunch->IsInitPos1();
	return FALSE;
}

BOOL CManagerProcedure::IsMkEdPos1()
{
	if (pView->m_mgrPunch)
		return pView->m_mgrPunch->IsMkEdPos1();
	return FALSE;
}

void CManagerProcedure::InitIoWrite()
{
	pView->MpeWrite(_T("MB44015E"), 0); // ����1 On  (PC�� ON, OFF) - 20141020
	pView->MpeWrite(_T("MB44015F"), 0); // ����2 On  (PC�� ON, OFF) - 20141020

	pView->MpeWrite(_T("MB003828"), 0); // �˻�� �� �˻� ���� <-> Y4368 I/F
	pView->MpeWrite(_T("MB003829"), 0); // �˻�� �� �˻� ���̺� ���� SOL <-> Y4369 I/F
	pView->MpeWrite(_T("MB00382A"), 0); // �˻�� �� Reset <-> Y436A I/F
	pView->MpeWrite(_T("MB00382B"), 0); // ��ŷ�� Lot End <-> Y436B I/F

	pView->MpeWrite(_T("MB003928"), 0); // �˻�� �� �˻� ���� <-> Y4468 I/F
	pView->MpeWrite(_T("MB003929"), 0); // �˻�� �� �˻� ���̺� ���� SOL <-> Y4369 I/F
	pView->MpeWrite(_T("MB00392A"), 0); // �˻�� �� Reset <-> Y436A I/F
	pView->MpeWrite(_T("MB00392B"), 0); // ��ŷ�� Lot End <-> Y436B I/F

	pView->MpeWrite(_T("MB44015D"), 0); // �ڵ� �ʱ� ��������(PC�� On/Off ��Ŵ, PLC�� ���������� ��ũ����, on->off�� �������� on, �ٽ� ��������ġ�� �������� off)
	pView->MpeWrite(_T("ML45064"), 0); // �˻�� Feeding �ѷ� Offset(*1000, +:�� ����, -�� ����, PC�� ���� PLC���� ����)
	pView->MpeWrite(_T("ML45066"), 0); // ��ŷ�� Feeding �ѷ� Offset(*1000, +:�� ����, -�� ����, PC�� ���� PLC���� ����)

	pView->MpeWrite(_T("MB600000"), 0); // PC�� PLC�� Alarm �߻����θ� Ȯ��
	pView->MpeWrite(_T("MB600008"), 0); // PC�� PLC�� Alarm �߻����θ� Ȯ��
	pView->MpeWrite(_T("ML60002"), 0); // �˶��� �߻��� ������ ����(PLC�� ǥ�� �� �˶��� �������� ����Ŵ).
}

void CManagerProcedure::SetTest(BOOL bOn)
{
	if (bOn)
	{
		pView->MpeWrite(_T("MB003828"), 1); // �˻�� �� �˻� ���� <-> Y4368 I/F
		pView->MpeWrite(_T("MB003928"), 1); // �˻�� �� �˻� ���� <-> Y4468 I/F
	}
	else
	{
		pView->MpeWrite(_T("MB003828"), 0); // �˻�� �� �˻� ���� <-> Y4368 I/F
		pView->MpeWrite(_T("MB003928"), 0); // �˻�� �� �˻� ���� <-> Y4468 I/F
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
		pView->MpeWrite(_T("MB44015D"), 1); // �ڵ� �ʱ� ��������(PC�� On/Off ��Ŵ, PLC�� ���������� ��ũ����, on->off�� �������� on, �ٽ� ��������ġ�� �������� off)
}

void CManagerProcedure::ResetWinker() // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	pView->MpeWrite(_T("MB44015D"), 0);	// �ڵ� �ʱ� ��������(PC�� On/Off ��Ŵ, PLC�� ���������� ��ũ����, on->off�� �������� on, �ٽ� ��������ġ�� �������� off)
}

CString CManagerProcedure::GetDispMain()
{
	return pView->GetDispMain();
}

void CManagerProcedure::DispMain(CString sMsg, COLORREF rgb)
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
			pView->MpeWrite(_T("ML45112"), (long)nSerial);	// �˻��� Panel�� AOI �� Serial
			Sleep(300);
			pView->MpeWrite(_T("MB44012B"), 1); // AOI �� : PCR���� Received
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
				pView->MpeWrite(_T("ML45114"), (long)pDoc->GetAoiDnAutoSerial() - 1);	// �˻��� Panel�� AOI �� Serial
				Sleep(300);
				pView->MpeWrite(_T("MB44012C"), 1); // AOI �� : PCR���� Received
			}
		}
	}
}

void CManagerProcedure::Shift2Buf()	// ���������� ������ �ø���� Share������ �ø����� �������� Ȯ�� �� �ű�.
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
				pView->MpeWrite(_T("ML45112"), (long)nSerial);	// �˻��� Panel�� AOI �� Serial
				Sleep(300);
				pView->MpeWrite(_T("MB44012B"), 1); // AOI �� : PCR���� Received
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
					pView->MpeWrite(_T("ML45114"), (long)pDoc->GetAoiDnAutoSerial() - 1);	// �˻��� Panel�� AOI �� Serial
					Sleep(300);
					pView->MpeWrite(_T("MB44012C"), 1); // AOI �� : PCR���� Received
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
			sMsg.Format(_T("AOI ����� �ø����� ¦���� �����Ͽ����ϴ�.\r\n- �ø��� ��ȣ: %d"), m_nShareUpS);
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
				sMsg.Format(_T("AOI �ϸ��� �ø����� ¦���� �����Ͽ����ϴ�.\r\n- �ø��� ��ȣ: %d"), m_nShareDnS);
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

int CManagerProcedure::LoadPcrUp(int nSerial)	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
{
	if (!pView->m_mgrReelmap)	return FALSE;
	return pView->m_mgrReelmap->LoadPcrUp(nSerial);
}

int CManagerProcedure::LoadPcrDn(int nSerial)	// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
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
	stThread& Thread = (pView->m_mgrStatus->Thread);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sMsg;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST:	// PLC MK ��ȣ Ȯ��	
			if (IsRun())
			{
				pDoc->LogAuto(_T("PC: ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)"));
				pView->MpeWrite(_T("MB440150"), 1);// ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)
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
						sMsg.Format(_T("2D �����Ⱑ ������ ���� �ʾҽ��ϴ�."));
						AfxMessageBox(sMsg);
						General.nMkStAuto = MK_ST;
						break;
					}
				}
#endif
				General.nMkStAuto++;
			}
			break;
		case MK_ST + 1:
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				General.nMkStAuto = MK_ST + (Mk2PtIdx::Start);
			}
			break;
		case MK_ST + (Mk2PtIdx::Start) :	// 2
			if (bDualTest)
			{
				if (ListBuf[1].nTot > 0) // AOI-Dn
				{
					General.nMkStAuto++;

					General.nBufDnSerial[0] = ListBuf[1].Pop();
					General.nBufUpSerial[0] = General.nBufDnSerial[0];

					if (ListBuf[1].nTot > 0) // AOI-Dn
					{
						General.nBufDnSerial[1] = ListBuf[1].Pop();
						General.nBufUpSerial[1] = General.nBufDnSerial[1];
					}
					else
					{
						General.nBufDnSerial[1] = 0;
						General.nBufUpSerial[1] = 0;
					}
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						General.bLotEnd = TRUE;
						General.nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (General.nBufUpSerial[0] == 1)
					{
						SetLastProc(ID_AOIDN);
					}
				}
			}
			else
			{
				if (ListBuf[0].nTot > 0) // AOI-Up
				{
					General.nMkStAuto++;
					General.nBufUpSerial[0] = ListBuf[0].Pop();

					if (ListBuf[0].nTot > 0) // AOI-Up
					{
						General.nBufUpSerial[1] = ListBuf[0].Pop();
					}
					else
						General.nBufUpSerial[1] = 0;
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						General.bLotEnd = TRUE;
						General.nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (General.nBufUpSerial[0] == 1)
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
					General.nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn);
			}
			else
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
					General.nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn);
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

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::ChkSn) :
			if (General.nBufUpSerial[0] == General.nBufUpSerial[1])
			{
				Stop();
				pView->ClrDispMsg();
				AfxMessageBox(_T("��/�� ��ŷ �ø����� �����ϴ�."));
				SetListBuf();
				General.nMkStAuto = MK_ST + (Mk2PtIdx::Start);
				break;
			}

			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN && !Thread.bTHREAD_MAKE_ITS_FILE_UP && !Thread.bTHREAD_MAKE_ITS_FILE_DN)
				{
					if (!Thread.bTHREAD_DISP_DEF && !Thread.bTHREAD_DISP_DEF_INNER)
					{
						General.nMkStAuto++;
						Thread.nStepTHREAD_DISP_DEF = 0;
						Thread.nStepTHREAD_DISP_DEF_INNER = 0;

						Thread.bTHREAD_DISP_DEF = TRUE;				// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						Thread.bTHREAD_DISP_DEF_INNER = TRUE;		// DispDefImgInner() : Disp Reelmap Start

						pView->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}
			else
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				{
					if (!Thread.bTHREAD_DISP_DEF)
					{
						General.nMkStAuto++;
						Thread.nStepTHREAD_DISP_DEF = 0;

						Thread.bTHREAD_DISP_DEF = TRUE;		// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						pView->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}
			break;

		case MK_ST + (Mk2PtIdx::ChkSn) + 1:
			General.nMkStAuto++;
			nSerial = General.nBufUpSerial[0];
			//sNewLot = m_sNewLotUp;

			if (nSerial > 0)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[1] < General.nLotEndSerial)
						nLastShot = General.nBufUpSerial[0];
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[1] > General.nLotEndSerial)
					{
						nLastShot = General.nBufUpSerial[0];
					}
				}

				bNewModel = GetAoiUpInfo(nSerial, &nNewLot, TRUE);
				if (bDualTest)
				{
					bNewModel = GetAoiDnInfo(nSerial, &nNewLot, TRUE);

					if (!IsSameUpDnLot() && !General.bContDiffLot)
					{
						General.nMkStAuto = MK_ST + (Mk2PtIdx::LotDiff);
						break;
					}
				}

				if (bNewModel)	// AOI ����(AoiCurrentInfoPath) -> AOI Feeding Offset
				{
					;
				}
				if (nNewLot)
				{					
					ChgLot(); // Lot Change.
					//dFdEnc = (double)pDoc->m_pMpeData[0][0];	// ��ŷ�� Feeding ���ڴ� ��(���� mm )
					//if ((pDoc->WorkingInfo.LastJob.bLotSep || pDoc->m_bDoneChgLot) && (dFdEnc + _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen)*2.0) > _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0)
					//{
					//	pDoc->m_bDoneChgLot = TRUE;
					//	SetPathAtBuf();
					//}
				}
			}
			else
			{
				Stop();
				pView->MsgBox(_T("����(��) Serial�� �����ʽ��ϴ�."));
			}
			break;

		case MK_ST + (Mk2PtIdx::ChkSn) + 2:
#ifdef USE_SR1000W
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pSr1000w && pSr1000w->IsConnected())
				{
					if (m_sGet2dCodeLot != _T("") && m_nGet2dCodeSerial != 0)
					{
						if (General.nBufUpSerial[0] == m_nGet2dCodeSerial)
						{
							General.nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);			// InitMk()
						}
						else
						{
							Stop();
							pView->ClrDispMsg();
							sMsg.Format(_T("2D���� �ø���(%d)�� �˻����� �ø���(%d)�� �ٸ��ϴ�."), m_nGet2dCodeSerial, General.nBufUpSerial[0]);
							AfxMessageBox(sMsg);
							General.nMkStAuto = MK_ST + (Mk2PtIdx::Start);
							break;
						}
					}
					else
						General.nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);			// InitMk()

				}
				else
				{
					Stop();
					pView->ClrDispMsg();
					sMsg.Format(_T("2D �����Ⱑ ������ ���� �ʾҽ��ϴ�."));
					AfxMessageBox(sMsg);
					General.nMkStAuto = MK_ST + (Mk2PtIdx::Start);
					break;
				}
			}
			else
				General.nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);					// InitMk()
#else
			General.nMkStAuto = MK_ST + (Mk2PtIdx::InitMk);					// InitMk()
#endif
			break;
		}
	}
}

void CManagerProcedure::Mk2PtInit()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);
	stGeneral& General = (pView->m_mgrStatus->General);

	if (Thread.bTHREAD_SHIFT2MK)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::InitMk) :
			if (InitMk()) // �ø����� ���� �Ǵ� ���� : TRUE �ƴϰ� ������ : FALSE
			{
				if (General.bSerialDecrese)
				{
					if ((General.nBufUpSerial[0] <= General.nLotEndSerial || General.nBufUpSerial[1] <= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off) - 20160718
						pDoc->LogAuto(_T("PC: ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)"));
					}
				}
				else
				{
					if ((General.nBufUpSerial[0] >= General.nLotEndSerial || General.nBufUpSerial[1] >= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off) - 20160718
						pDoc->LogAuto(_T("PC: ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)"));
					}
				}
			}
			else // Same Serial
			{
				Stop();
				pView->MsgBox(_T("Serial ���� �����ʽ��ϴ�."));
			}
										General.nMkStAuto++;
										break;

		case MK_ST + (Mk2PtIdx::InitMk) + 1:
			if (IsRun())
			{
				General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1);	// Move - Cam1 - Pt0
																//if (MODE_INNER != pDoc->GetTestMode())
																//	General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1);	// Move - Cam1 - Pt0
																//else
																//	General.nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt);
			}
			break;
		}
	}
}

void CManagerProcedure::Mk2PtAlignPt0()
{
	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[0].MasterInfo;
	stGeneral& General = (pView->m_mgrStatus->General);

	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Move0Cam1) :	// Move - Cam1 - Pt0
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufDnSerial[1] < General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0))	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufDnSerial[1] > General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0))	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[1] < General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0)) 	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[1] > General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0)) 	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
								General.nMkStAuto++;
						}
					}
				}
			}
												break;
		case MK_ST + (Mk2PtIdx::Move0Cam1) + 1:
			if (IsRun())
				General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0);
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) :	// Move - Cam0 - Pt0
			if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
			{
				if (MoveAlign1(0))
					General.nMkStAuto++;
			}
			else
			{
				if (MoveAlign0(0))
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				General.nMkStAuto = MK_ST + (Mk2PtIdx::Align1_0);
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
			General.nMkStAuto = MK_ST + (Mk2PtIdx::Align0_0);
			break;
		case MK_ST + (Mk2PtIdx::Align0_0) :	// 2PtAlign - Cam0 - Pt0
			if (!m_bSkipAlign[0][0])
			{
				if (TwoPointAlign0(0))
					m_bFailAlign[0][0] = FALSE;
				else
					m_bFailAlign[0][0] = TRUE;
			}
											General.nMkStAuto++;
											break;
		case MK_ST + (Mk2PtIdx::Align0_0) + 1:
			if (m_bFailAlign[0][0])
			{
				Buzzer(TRUE, 0);
				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[0][0] = FALSE;
						m_bSkipAlign[0][0] = TRUE;
						m_bSkipAlign[0][1] = TRUE;
						m_bSkipAlign[0][2] = TRUE;
						m_bSkipAlign[0][3] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0); // TwoPointAlign0(0) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][0] = TRUE;
					m_bSkipAlign[0][0] = FALSE;
					m_bSkipAlign[0][1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0); // TwoPointAlign0(0) ���� ����. - ī�޶� ������
					Stop();
				}
			}
			if (m_bFailAlign[1][0])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[1][0] = FALSE;
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); // TwoPointAlign1(0) ���� ����. - ī�޶� ������
						Stop();
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
					General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); // TwoPointAlign1(0) ���� ����. - ī�޶� ������
					Stop();
				}
			}

			if (m_bFailAlign[0][0] || m_bFailAlign[1][0])
			{
				if (!m_bReAlign[0][0] && !m_bReAlign[1][0])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						General.nMkStAuto++; //General.nMkStAuto = MK_ST + 27; // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						General.nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); // // �ҷ��̹��� Display, Align���� �ʱ�ȭ (Skip 65 : Mk())
					}
				}
				else
				{
					General.nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); // TwoPointAlign1(0) ���� ����. - ī�޶� ������
				}
			}
			else
				General.nMkStAuto++;

			break;
		case MK_ST + (Mk2PtIdx::Align0_0) + 2:
			if (IsRun())
				General.nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1);
			break;
		}
	}
}

void CManagerProcedure::Mk2PtAlignPt1()
{
	if (!IsRun())
		return;

	stGeneral& General = (pView->m_mgrStatus->General);

	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[1].MasterInfo;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Move1Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufDnSerial[1] < General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufDnSerial[1] > General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufDnSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[1] < General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[1] > General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else if (General.nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
			}
										   break;
		case MK_ST + (Mk2PtIdx::Move1Cam1) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) :
			if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
			{
				if (MoveAlign1(1))	// Move - Cam0 - Pt1
					General.nMkStAuto++;
			}
			else
			{
				if (MoveAlign0(1))	// Move - Cam0 - Pt1
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				General.nMkStAuto = MK_ST + (Mk2PtIdx::Align1_1);
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

			General.nMkStAuto = MK_ST + (Mk2PtIdx::Align0_1);
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

			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Align0_1) + 1:
			if (m_bFailAlign[0][1])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[0][1] = FALSE;
						m_bSkipAlign[0][1] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam0); // TwoPointAlign0(1) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][1] = TRUE;
					m_bSkipAlign[0][1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam0); // TwoPointAlign1(1) ���� ����. - ī�޶� ������
					Stop();
				}
			}
			if (m_bFailAlign[1][1])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[1][1] = FALSE;
						m_bSkipAlign[1][1] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); // TwoPointAlign1(1) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][1] = TRUE;
					m_bSkipAlign[1][1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); // TwoPointAlign1(1) ���� ����. - ī�޶� ������
					Stop();
				}
			}

			if (m_bFailAlign[0][1] || m_bFailAlign[1][1])
			{
				if (!m_bReAlign[0][1] && !m_bReAlign[1][1])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						General.nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt); //General.nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						General.nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); // �ҷ��̹��� Display, Align���� �ʱ�ȭ (Skip 65 : Mk())
					}
				}
				else
					General.nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); // TwoPointAlign1(1) ���� ����. - ī�޶� ������
			}
			else
				General.nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt);

			break;
		}
	}
}

void CManagerProcedure::Mk2PtMoveInitPos()
{
	if (!pView->m_mgrStatus)	return;
	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::MoveInitPt) :
			MoveInitPos0(FALSE);
			MoveInitPos1(FALSE); // 20220526
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::MoveInitPt) + 1:
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::MoveInitPt) + 2:
			if (IsMoveDone())
				General.nMkStAuto = MK_ST + (Mk2PtIdx::ChkElec);
			break;
		}
	}
}

void CManagerProcedure::Mk2PtDoMarking()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::DoMk) :				// Mk ��ŷ ����]
			if (CheckMkPnt())
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
					SetMkIts(TRUE);						// ITS ��ŷ ����
				else
					SetMk(TRUE);						// Mk ��ŷ ����

				General.nMkStAuto++;
			}
			else
			{
				pView->MsgBox(_T("ķ�������� ��ŷ��ġ��ǥ�� �����Ǿ� �����ʽ��ϴ�.\r\nȮ���ϼ���."));
				Stop();
			}
													break;

		case MK_ST + (Mk2PtIdx::DoMk) + 1:
			if (!General.bUpdateYield) // Prevent rerunning on verify.
			{
				if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
				{
					General.bUpdateYield = TRUE;
					UpdateYield(); // Cam[0],  Cam[1]
					General.nMkStAuto++;
				}
			}
			else
			{
				Sleep(100);
				General.nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::DoMk) + 2:
			if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
				General.nMkStAuto++;
			//if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
			//{
			//	if (!m_bUpdateYieldOnRmap)
			//	{
			//		if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN)
			//		{
			//			if (!Thread.bTHREAD_UPDATE_YIELD_UP && !Thread.bTHREAD_UPDATE_YIELD_DN && !Thread.bTHREAD_UPDATE_YIELD_ALLUP && !Thread.bTHREAD_UPDATE_YIELD_ALLDN)
			//			{
			//				m_bUpdateYieldOnRmap = TRUE;
			//				UpdateYieldOnRmap()
			//				General.nMkStAuto++;
			//			}
			//			else
			//				Sleep(100);
			//		}
			//		else
			//			Sleep(100);
			//	}
			//	else
			//	{
			//		Sleep(100);
			//		General.nMkStAuto++; // ��ŷ �� verify�� ������ ������ ���� ���.
			//	}
			//}
			break;

		case MK_ST + (Mk2PtIdx::Verify) :
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !General.nPrevMkStAuto)
				{
					General.nPrevMkStAuto = MK_ST + (Mk2PtIdx::Verify);
					General.nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);		// Mk ��ŷ ����
					General.bCam = TRUE;
				}
				else
				{
					if (IsReview())
					{
						if (!General.bCam)
						{
							General.nPrevStepAuto = MK_ST + (Mk2PtIdx::Verify);
							General.nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);		// Mk ��ŷ ����
							General.bCam = TRUE;
							pView->MsgBox(_T("�� Jog ��ư�� �̿��Ͽ� ��ŷ��ġ�� Ȯ���Ͽ� �ּ���."));
						}
						else
						{
							General.bCam = FALSE;
							General.nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk);	// Mk ��ŷ �Ϸ�
						}
					}
					else
					{
						General.nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk);	// Mk ��ŷ �Ϸ�
					}
				}
			}
			else if (IsReMk())
			{
				General.nPrevMkStAuto = MK_ST + (Mk2PtIdx::Verify);
				General.nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);		// Mk �����
			}
			else
			{
				sMsg = _T("");
				sMsg += pView->m_sDispSts[0];
				sMsg += _T(",");
				sMsg += pView->m_sDispSts[1];
			}
										break;

		case MK_ST + (Mk2PtIdx::DoneMk) :	 // Align���� �ʱ�ȭ
			if (!IsRun())
				break;

			if (General.bInitAuto)
			{
				General.bInitAuto = FALSE;
				pView->MsgBox(_T("��ŷ��ġ�� Ȯ���ϼ���."));
				Stop();
				break;
			}

			if (General.nBufUpSerial[0] == 0)
			{
				m_bSkipAlign[0][0] = TRUE;
				m_bSkipAlign[0][1] = TRUE;
			}
			if (General.nBufUpSerial[1] == 0)
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

			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 1:  // Mk���� �ʱ�ȭ
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
			General.bCam = FALSE;
			General.nPrevMkStAuto = 0;

			General.bUpdateYield = FALSE;

			ResetMkStrip();

			m_nSaveMk0Img = 0;
			m_nSaveMk1Img = 0;

			pView->MpeWrite(_T("MB440150"), 0);	// ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)
			pView->MpeWrite(_T("MB440170"), 1);	// ��ŷ�Ϸ�(PLC�� Ȯ���ϰ� Reset��Ŵ.)-20141029
			pDoc->LogAuto(_T("PC: ��ŷ�Ϸ�(PLC�� Ȯ���ϰ� Reset��Ŵ.)"));
			if (IsNoMk() || pView->IsShowLive())
				pView->ShowLive(FALSE);

			General.nMkStAuto++;
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) + 2:
			if (pView->m_mgrFeeding->GetMpeSignal(0, 1))	// ��ŷ�� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)-20141030
			{
				pDoc->LogAuto(_T("PLC: ��ŷ�� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)"));
				General.nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) + 3:
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				pView->MpeWrite(_T("MB440101"), 0);	// ��ŷ�� Feeding�Ϸ�
				pDoc->LogAuto(_T("PC: ��ŷ�� Feeding�Ϸ� OFF"));

				m_bShift2Mk = TRUE;
				Shift2MkFromThread();			// PCR �̵�(Buffer->Marked) // ���(WorkingInfo.LastJob.sSerial)

				pView->SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();

				UpdateRst();
				General.nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) + 4:
			if (!Thread.bTHREAD_SHIFT2MK && !Thread.bTHREAD_REELMAP_YIELD_UP && !Thread.bTHREAD_REELMAP_YIELD_DN && !Thread.bTHREAD_REELMAP_YIELD_ALLUP && !Thread.bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (Thread.bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
						break;
				}

				General.nMkStAuto++;
				ChkYield();
			}
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 5:
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				ChkLotCutPos();
				UpdateWorking();	// Update Working Info...
				General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 6:
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 7:
			General.nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 8:
			General.bMkSt = FALSE;
			break;
		}
	}
}

void CManagerProcedure::Mk2PtShift2Mk() // MODE_INNER
{
	stThread& Thread = (pView->m_mgrStatus->Thread);
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Shift2Mk) :
			General.nMkStAuto++;
			if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
			{
				UpdateYield(); // Cam[0],  Cam[1]
				General.nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 1:
			if (!Thread.bTHREAD_UPDATAE_YIELD[0] && !Thread.bTHREAD_UPDATAE_YIELD[1])
			{
				if (!Thread.bTHREAD_UPDATE_REELMAP_UP && !Thread.bTHREAD_UPDATE_REELMAP_DN && !Thread.bTHREAD_UPDATE_REELMAP_ALLUP && !Thread.bTHREAD_UPDATE_REELMAP_ALLDN)
				{
					if (!Thread.bTHREAD_UPDATE_YIELD_UP && !Thread.bTHREAD_UPDATE_YIELD_DN && !Thread.bTHREAD_UPDATE_YIELD_ALLUP && !Thread.bTHREAD_UPDATE_YIELD_ALLDN)
					{
						UpdateRst();
						UpdateWorking();	// Update Working Info...
						//UpdateYieldOnRmap();
						General.nMkStAuto++;
					}
					else
						Sleep(100);
				}
				else
					Sleep(100);
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 2:
			pView->MpeWrite(_T("MB440150"), 0);	// ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)
			pView->MpeWrite(_T("MB440170"), 1);	// ��ŷ�Ϸ�(PLC�� Ȯ���ϰ� Reset��Ŵ.)-20141029
			pDoc->LogAuto(_T("PC: ��ŷ�Ϸ�(PLC�� Ȯ���ϰ� Reset��Ŵ.)"));
			General.nMkStAuto++;
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 3:
			if (pView->m_mgrFeeding->GetMpeSignal(0, 1))	// ��ŷ�� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)-20141030
			{
				pDoc->LogAuto(_T("PLC: ��ŷ�� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)"));
				General.nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 4:
			if (!Thread.bTHREAD_SHIFT2MK)
			{
				pDoc->LogAuto(_T("PC: ��ŷ�� Feeding�Ϸ� OFF"));
				pView->MpeWrite(_T("MB440101"), 0);	// ��ŷ�� Feeding�Ϸ�
				m_bShift2Mk = TRUE;
				Shift2MkFromThread();

				pView->SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();

				General.nMkStAuto++;
			}
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

					General.nMkStAuto++;
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
				General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 7:
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 8:
			General.nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 9:
			General.bMkSt = FALSE;
			break;
		}
	}
}

void CManagerProcedure::Mk2PtLotDiff()
{
	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::LotDiff) :
			Stop();
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 1:
			if (IDYES == pView->MsgBox(_T("���� �ϸ��� Lot�� �ٸ��ϴ�.\r\n��� �۾��� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
			{
				General.bContDiffLot = TRUE;
			}
			else
			{
				General.bContDiffLot = FALSE;
				General.bLotEnd = TRUE;
				General.nLotEndAuto = LOT_END;
			}
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 2:
			if (IsRun())
			{
				if (General.bContDiffLot)
					General.nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn);
				else
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 3:
			General.bContDiffLot = FALSE;
			General.bLotEnd = TRUE;
			General.nLotEndAuto = LOT_END;
			break;
		}
	}
}

void CManagerProcedure::Mk2PtReject()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	int a, b;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case REJECT_ST:
			Buzzer(TRUE, 0);
			Stop();

			if (IDYES == pView->MsgBox(_T("��Ʈ üũ �ҷ��Դϴ�.\r\n���� ó���� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				General.nMkStAuto++;
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("���� ó���� ����Ͽ����ϴ�.\r\n�ҷ��� ��ŷ ó���� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					General.nMkStAuto++;
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
					General.nMkStAuto++;
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);	// Mk ��ŷ ����
				}
				else
					General.nMkStAuto = REJECT_ST;
			}
			break;
		case REJECT_ST + 2:
			SetReject();
			General.nMkStAuto++;
			break;
		case REJECT_ST + 3:
			General.nMkStAuto++;
			break;
		case REJECT_ST + 4:
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !General.nPrevMkStAuto)
				{
					General.nPrevMkStAuto = REJECT_ST + 4;
					General.nMkStAuto = REJECT_ST + 2;		// Mk ��ŷ ����
					General.bCam = TRUE;

					m_bDoneMk[0] = FALSE;
					m_bDoneMk[1] = FALSE;

					ResetMkStrip();
				}
				else
				{
					if (IsReview())
					{
						if (!General.bCam)
						{
							General.nPrevStepAuto = REJECT_ST + 4;
							General.nMkStAuto = REJECT_ST + 2;		// Mk ��ŷ ����
							General.bCam = TRUE;
							pView->MsgBox(_T("�� Jog ��ư�� �̿��Ͽ� ��ŷ��ġ�� Ȯ���Ͽ� �ּ���."));
						}
						else
						{
							General.bCam = FALSE;
							General.nMkStAuto++;	// Mk ��ŷ �Ϸ�

											//sMsg = _T("");
											//DispStsBar(sMsg, 0);
						}
					}
					else
					{
						General.nMkStAuto++;	// Mk ��ŷ �Ϸ�

										//sMsg = _T("");
										//DispStsBar(sMsg, 0);
					}
				}
			}
			break;
		case REJECT_ST + 5:
			General.nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk);				// Align���� �ʱ�ȭ
			break;
		}
	}
}

void CManagerProcedure::Mk2PtErrStop()
{
	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case ERROR_ST:
			Stop();

			if (IDYES == pView->MsgBox(_T("��Ʈ üũ Error�Դϴ�.\r\n�ٽ� ��Ʈ üũ�� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				General.nMkStAuto++;
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("��Ʈ üũ�� ����Ͽ����ϴ�.\r\n�ҷ��� ��ŷ ó���� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					General.nMkStAuto++;
				}
				else
					General.nMkStAuto++;
			}
			break;
		case ERROR_ST + 1:
			General.nMkStAuto++;
			break;
		case ERROR_ST + 2:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE;
					General.nMkStAuto = MK_ST + (Mk2PtIdx::ChkElec); // DoElecChk
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk2PtIdx::DoMk);	// Mk ��ŷ ����
				}
				else
					General.nMkStAuto++;
			}
			break;
		case ERROR_ST + 3:
			General.nMkStAuto = ERROR_ST;
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
	//Mk2PtElecChk();
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
	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST:	// PLC MK ��ȣ Ȯ��	
			if (IsRun())
			{
				SetListBuf();
				General.nMkStAuto++;
			}
			break;
		case MK_ST + 1:
			pDoc->LogAuto(_T("PC: ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)"));
			pView->MpeWrite(_T("MB440150"), 1);// ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Start) :	// 2
			if (bDualTest)
			{
				if (ListBuf[1].nTot > 0) // AOI-Dn
				{
					General.nMkStAuto++;

					General.nBufDnSerial[0] = ListBuf[1].Pop();
					General.nBufUpSerial[0] = General.nBufDnSerial[0];
					if (ListBuf[1].nTot > 0) // AOI-Dn
					{
						General.nBufDnSerial[1] = ListBuf[1].Pop();
						General.nBufUpSerial[1] = General.nBufDnSerial[1];
					}
					else
					{
						General.nBufDnSerial[1] = 0;
						General.nBufUpSerial[1] = 0;
					}
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						General.bLotEnd = TRUE;
						General.nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (General.nBufUpSerial[0] == 1)
					{
						SetLastProc(ID_AOIDN);
					}
				}
			}
			else
			{
				if (ListBuf[0].nTot > 0) // AOI-Up
				{
					General.nMkStAuto++;
					General.nBufUpSerial[0] = ListBuf[0].Pop();
					if (ListBuf[0].nTot > 0) // AOI-Up
						General.nBufUpSerial[1] = ListBuf[0].Pop();
					else
						General.nBufUpSerial[1] = 0;
				}
				else
				{
					if (!pDoc->GetAoiUpVsStatus())
					{
						General.bLotEnd = TRUE;
						General.nLotEndAuto = LOT_END;
					}
				}

				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (General.nBufUpSerial[0] == 1)
					{
						SetLastProc(ID_AOIDN);
					}
				}
			}
											break;
		case MK_ST + (Mk4PtIdx::Start) + 1:
			General.nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtChkSerial()
{
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	double dFdEnc;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::ChkSn) :
			if (General.nBufUpSerial[0] == General.nBufUpSerial[1])
			{
				Stop();
				pView->ClrDispMsg();
				AfxMessageBox(_T("��/�� ��ŷ �ø����� �����ϴ�."));
				SetListBuf();
				General.nMkStAuto = MK_ST + (Mk4PtIdx::Start);
				break;
			}

			if (!Thread.bTHREAD_DISP_DEF)
			{
				General.nMkStAuto++;
				Thread.nStepTHREAD_DISP_DEF = 0;
				Thread.bTHREAD_DISP_DEF = TRUE;		// DispDefImg() : CopyDefImg Start
				pView->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
			}
			break;

		case MK_ST + (Mk4PtIdx::ChkSn) + 1:
			General.nMkStAuto = MK_ST + (Mk4PtIdx::InitMk);			// InitMk()
			nSerial = General.nBufUpSerial[0];
			sNewLot = m_sNewLotUp;

			if (nSerial > 0)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[1] < General.nLotEndSerial)
					{
						nSerial = General.nBufUpSerial[0]; // Test
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[1] > General.nLotEndSerial)
					{
						nSerial = General.nBufUpSerial[0]; // Test
					}
				}

				bNewModel = GetAoiUpInfo(nSerial, &nNewLot, TRUE);
				if (bDualTest)
				{
					bNewModel = GetAoiDnInfo(nSerial, &nNewLot, TRUE);

					if (!IsSameUpDnLot() && !General.bContDiffLot)
					{
						General.nMkStAuto = MK_ST + (Mk4PtIdx::LotDiff);
						break;
					}
				}

				if (bNewModel)	// AOI ����(AoiCurrentInfoPath) -> AOI Feeding Offset
				{
					;
				}
				if (nNewLot)
				{
					ChgLot();
				}
			}
			else
			{
				Stop();
				pView->MsgBox(_T("����(��) Serial�� �����ʽ��ϴ�."));
			}
			break;
		}
	}
}

void CManagerProcedure::Mk4PtInit()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::InitMk) :
			if (InitMk())	// �ø����� ���� �Ǵ� ���� : TRUE �ƴϰ� ������ : FALSE
			{
				if (General.bSerialDecrese)
				{
					if ((General.nBufUpSerial[0] <= General.nLotEndSerial || General.nBufUpSerial[1] <= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off) - 20160718
						pDoc->LogAuto(_T("PC: ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)"));
					}
				}
				else
				{
					if ((General.nBufUpSerial[0] >= General.nLotEndSerial || General.nBufUpSerial[1] >= General.nLotEndSerial) && General.nLotEndSerial > 0)
					{
						pView->MpeWrite(_T("MB440171"), 1); // ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off) - 20160718
						pDoc->LogAuto(_T("PC: ��ŷ�� �۾��Ϸ�.(PC�� On, PLC�� Ȯ�� �� Off)"));
					}
				}
			}
			else
			{
				Stop();
				pView->MsgBox(_T("Serial ���� �����ʽ��ϴ�."));
			}
										General.nMkStAuto++;
										break;

		case MK_ST + (Mk4PtIdx::InitMk) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt0()
{
	if (!IsRun())
		return;

	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[1].MasterInfo;
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move0Cam1) :	// Move - Cam1 - Pt0
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0))
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0))
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))
								General.nMkStAuto++;
						}
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0))
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(0))
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))
								General.nMkStAuto++;
						}
					}
				}
			}
												break;
		case MK_ST + (Mk4PtIdx::Move0Cam1) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) :	// Move - Cam0 - Pt0
			if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
			{
				if (MoveAlign1(0))
					General.nMkStAuto++;
			}
			else
			{
				if (MoveAlign0(0))
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				General.nMkStAuto++;
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
											General.nMkStAuto++;
											break;
		case MK_ST + (Mk4PtIdx::Align0_0) :	// 4PtAlign - Cam0 - Pt0
			if (!m_bSkipAlign[0][0])
			{
				if (FourPointAlign0(0))
					m_bFailAlign[0][0] = FALSE;
				else
					m_bFailAlign[0][0] = TRUE;
			}
											General.nMkStAuto++;
											break;
		case MK_ST + (Mk4PtIdx::Align0_0) + 1:
			if (m_bFailAlign[0][0])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[0][0] = FALSE;
						m_bSkipAlign[0][0] = TRUE;
						m_bSkipAlign[0][1] = TRUE;
						m_bSkipAlign[0][2] = TRUE;
						m_bSkipAlign[0][3] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam0); // FourPointAlign0(0) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][0] = TRUE;
					m_bSkipAlign[0][0] = FALSE;
					m_bSkipAlign[0][1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam0); // FourPointAlign0(0) ���� ����. - ī�޶� ������
					Stop();
				}
			}
			if (m_bFailAlign[1][0])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[1][0] = FALSE;
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); // FourPointAlign1(0) ���� ����. - ī�޶� ������
						Stop();
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
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); // FourPointAlign1(0) ���� ����. - ī�޶� ������
					Stop();
				}
			}

			if (m_bFailAlign[0][0] || m_bFailAlign[1][0])
			{
				if (!m_bReAlign[0][0] && !m_bReAlign[1][0])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						General.nMkStAuto++; //General.nMkStAuto = MK_ST + 27; // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						General.nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // �ҷ��̹��� Display, Align���� �ʱ�ȭ (Skip 65 : Mk())
					}
				}
				else
				{
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); // TwoPointAlign1(0) ���� ����. - ī�޶� ������
				}
			}
			else
				General.nMkStAuto++;

			break;
		case MK_ST + (Mk4PtIdx::Align0_0) + 2:
			if (IsRun())
				General.nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt1()
{
	if (!IsRun())
		return;

	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[1].MasterInfo;
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move1Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								General.nMkStAuto++;
						}
					}
				}
			}
										   break;
		case MK_ST + (Mk4PtIdx::Move1Cam1) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) :
			if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
			{
				if (MoveAlign0(1))	// Move - Cam0 - Pt1
					General.nMkStAuto++;
			}
			else
			{
				if (MoveAlign0(1))	// Move - Cam0 - Pt1
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				General.nMkStAuto++;
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

			General.nMkStAuto++;
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

			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_1) + 1:
			if (m_bFailAlign[0][1])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[0][1] = FALSE;
						m_bSkipAlign[0][1] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam0); // FourPointAlign0(1) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][1] = TRUE;
					m_bSkipAlign[0][1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam0); // FourPointAlign1(1) ���� ����. - ī�޶� ������
					Stop();
				}
			}
			if (m_bFailAlign[1][1])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[1][1] = FALSE;
						m_bSkipAlign[1][1] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); // FourPointAlign1(1) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][1] = TRUE;
					m_bSkipAlign[1][1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); // FourPointAlign1(1) ���� ����. - ī�޶� ������
					Stop();
				}
			}

			if (m_bFailAlign[0][1] || m_bFailAlign[1][1])
			{
				if (!m_bReAlign[0][1] && !m_bReAlign[1][1])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						General.nMkStAuto++;//General.nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						General.nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // �ҷ��̹��� Display, Align���� �ʱ�ȭ (Skip 65 : Mk())
					}
				}
				else
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); // FourPointAlign1(1) ���� ����. - ī�޶� ������
																 //General.nMkStAuto = MK_ST + (Mk4PtIdx::Align1_1); // FourPointAlign1(1) ���� ����. - ī�޶� ������
			}
			else
				General.nMkStAuto++;

			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt2()
{
	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[1].MasterInfo;
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move2Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								General.nMkStAuto++;
						}
					}
				}
			}
										   break;
		case MK_ST + (Mk4PtIdx::Move2Cam1) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) :
			if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
			{
				if (MoveAlign1(2))	// Move - Cam0 - Pt2
					General.nMkStAuto++;
			}
			else
			{
				if (MoveAlign0(2))	// Move - Cam0 - Pt2
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				General.nMkStAuto++;
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

			General.nMkStAuto++;
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

			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_2) + 1:
			if (m_bFailAlign[0][2])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[0][2] = FALSE;
						m_bSkipAlign[0][2] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam0); // FourPointAlign0(2) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][2] = TRUE;
					m_bSkipAlign[0][2] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam0); // FourPointAlign0(2) ���� ����. - ī�޶� ������
					Stop();
				}
			}
			if (m_bFailAlign[1][2])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[1][2] = FALSE;
						m_bSkipAlign[1][2] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); // FourPointAlign1(2) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][2] = TRUE;
					m_bSkipAlign[1][2] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); // FourPointAlign1(2) ���� ����. - ī�޶� ������
					Stop();
				}
			}

			if (m_bFailAlign[0][2] || m_bFailAlign[1][2])
			{
				if (!m_bReAlign[0][2] && !m_bReAlign[1][2])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						General.nMkStAuto++;//General.nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						General.nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // �ҷ��̹��� Display, Align���� �ʱ�ȭ (Skip 65 : Mk())
					}
				}
				else
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); // FourPointAlign1(2) ���� ����. - ī�޶� ������
																 //General.nMkStAuto = MK_ST + (Mk4PtIdx::Align1_2); // FourPointAlign1(2) ���� ����. - ī�޶� ������
			}
			else
				General.nMkStAuto++;

			break;
		}
	}
}

void CManagerProcedure::Mk4PtAlignPt3()
{
	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[1].MasterInfo;
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move3Cam1) :
			if (bDualTest)
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufDnSerial[0] - 1 < General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufDnSerial[0] + 1 > General.nLotEndSerial)	// AOI�ϸ� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
					}
				}
			}
			else
			{
				if (General.bSerialDecrese)
				{
					if (General.bLastProc && General.nBufUpSerial[0] - 1 < General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[0] + 1 > General.nLotEndSerial)	// AOI��� Serial
					{
						m_bSkipAlign[1][0] = TRUE;
						m_bSkipAlign[1][1] = TRUE;
						m_bSkipAlign[1][2] = TRUE;
						m_bSkipAlign[1][3] = TRUE;
						m_bDoMk[1] = FALSE;
						m_bDoneMk[1] = TRUE;
						General.nMkStAuto++;
					}
					else
					{
						if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
						{
							if (MoveAlign0(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								General.nMkStAuto++;
						}
					}
				}
			}
										   break;
		case MK_ST + (Mk4PtIdx::Move3Cam1) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) :
			if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3) // 1 : �¿� �̷� , 3 : 180 ȸ��
			{
				if (MoveAlign1(3))	// Move - Cam0 - Pt3
					General.nMkStAuto++;
			}
			else
			{
				if (MoveAlign0(3))	// Move - Cam0 - Pt3
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) + 1:
			if (IsRun())
				General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				General.nMkStAuto++;
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

			General.nMkStAuto++;
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

			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_3) + 1:
			if (m_bFailAlign[0][3])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[0][3] = FALSE;
						m_bSkipAlign[0][3] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam0); // FourPointAlign0(3) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][3] = TRUE;
					m_bSkipAlign[0][3] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam0); // FourPointAlign0(3) ���� ����. - ī�޶� ������
					Stop();
				}
			}
			if (m_bFailAlign[1][3])
			{
				Buzzer(TRUE, 0);

				if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ٽ� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ� ������ ���� ���Ͻðڽ��ϱ�?"), 0, MB_YESNO))
					{
						m_bReAlign[1][3] = FALSE;
						m_bSkipAlign[1][3] = TRUE;
						if (IDNO == pView->MsgBox(_T("ī�޶�(��)�� �˻��ǳ��� �ҷ���ŷ �Ͻðڽ��ϱ�?"), 0, MB_YESNO))
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
						General.nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); // FourPointAlign1(3) ���� ����. - ī�޶� ������
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][3] = TRUE;
					m_bSkipAlign[1][3] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); // FourPointAlign1(3) ���� ����. - ī�޶� ������
					Stop();
				}
			}

			if (m_bFailAlign[0][3] || m_bFailAlign[1][3])
			{
				if (!m_bReAlign[0][3] && !m_bReAlign[1][3])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						General.nMkStAuto++;//General.nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						General.nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); // // �ҷ��̹��� Display, Align���� �ʱ�ȭ (Skip 65 : Mk())
					}
				}
				else
					General.nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); // FourPointAlign1(3) ���� ����. - ī�޶� ������
			}
			else
				General.nMkStAuto++;

			break;
		}
	}
}

void CManagerProcedure::Mk4PtMoveInitPos()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::MoveInitPt) :
			MoveInitPos0();
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::MoveInitPt) + 1:
			if (m_bDoMk[1])
				MoveInitPos1();
			else
				MoveMkEdPos1();
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::MoveInitPt) + 2:
			if (IsMoveDone())
				General.nMkStAuto++;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtDoMarking()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::DoMk) :				// Mk ��ŷ ����
			if (pDoc->GetTestMode() == MODE_OUTER)
				SetMkIts(TRUE);						// ITS ��ŷ ����
			else
				SetMk(TRUE);						// Mk ��ŷ ����

			General.nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::DoMk) + 1:
			Sleep(100);
			General.nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::Verify) :
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !General.nPrevMkStAuto)
				{
					General.nPrevMkStAuto = MK_ST + (Mk4PtIdx::Verify);
					General.nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);		// Mk ��ŷ ����
					General.bCam = TRUE;
				}
				else
				{
					if (IsReview())
					{
						if (!General.bCam)
						{
							General.nPrevStepAuto = MK_ST + (Mk4PtIdx::Verify);
							General.nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);		// Mk ��ŷ ����
							General.bCam = TRUE;
							pView->MsgBox(_T("�� Jog ��ư�� �̿��Ͽ� ��ŷ��ġ�� Ȯ���Ͽ� �ּ���."));
						}
						else
						{
							General.bCam = FALSE;
							General.nMkStAuto++;	// Mk ��ŷ �Ϸ�

											//sMsg = _T("");
											//DispStsBar(sMsg, 0);
						}
					}
					else
					{
						General.nMkStAuto++;	// Mk ��ŷ �Ϸ�

										//sMsg = _T("");
										//DispStsBar(sMsg, 0);
					}
				}
			}
			else if (IsReMk())
			{
				General.nPrevMkStAuto = MK_ST + (Mk4PtIdx::Verify);
				General.nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);		// Mk �����
			}
			else
			{
				sMsg = _T("");
				sMsg += pView->m_sDispSts[0];
				sMsg += _T(",");
				sMsg += pView->m_sDispSts[1];
				//DispStsBar(sMsg, 0);
			}
										break;

		case MK_ST + (Mk4PtIdx::DoneMk) :	 // Align���� �ʱ�ȭ
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
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 1:  // Mk���� �ʱ�ȭ
			m_bDoMk[0] = TRUE;
			m_bDoMk[1] = TRUE;
			m_bDoneMk[0] = FALSE;
			m_bDoneMk[1] = FALSE;
			m_bReMark[0] = FALSE;
			m_bReMark[1] = FALSE;
			General.bCam = FALSE;
			General.nPrevMkStAuto = 0;

			ResetMkStrip();
			General.nMkStAuto++;
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

			pView->MpeWrite(_T("MB440150"), 0);	// ��ŷ�� ��ŷ�� ON (PC�� ON, OFF)
			pView->MpeWrite(_T("MB440170"), 1);	// ��ŷ�Ϸ�(PLC�� Ȯ���ϰ� Reset��Ŵ.)-20141029
			pDoc->LogAuto(_T("PC: ��ŷ�Ϸ�(PLC�� Ȯ���ϰ� Reset��Ŵ.)"));
			if (IsNoMk() || IsShowLive())
				ShowLive(FALSE);

			General.nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 3:
			if (pView->m_mgrFeeding->GetMpeSignal(0, 1))	// ��ŷ�� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)-20141030
			{
				pDoc->LogAuto(_T("PLC: ��ŷ�� Feeding�Ϸ�(PLC�� On��Ű�� PC�� Ȯ���ϰ� Reset��Ŵ.)"));
				pView->MpeWrite(_T("MB440101"), 0);	// ��ŷ�� Feeding�Ϸ�

				Shift2Mk();			// PCR �̵�(Buffer->Marked) // ���(WorkingInfo.LastJob.sSerial)
				UpdateRst();
				pView->SetMkFdLen();

				SetCycTime();
				m_dwCycSt = GetTickCount();

				UpdateWorking();	// Update Working Info...
				ChkYield();
				General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 4:
			ChkLotCutPos();
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 5:
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 6:
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 7:
			General.nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 8:
			General.bMkSt = FALSE;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtLotDiff()
{
	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::LotDiff) :
			Stop();
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 1:
			if (IDYES == pView->MsgBox(_T("���� �ϸ��� Lot�� �ٸ��ϴ�.\r\n��� �۾��� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
			{
				General.bContDiffLot = TRUE;
			}
			else
			{
				General.bContDiffLot = FALSE;
				General.bLotEnd = TRUE;
				General.nLotEndAuto = LOT_END;
			}
			General.nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 2:
			if (IsRun())
			{
				if (General.bContDiffLot)
					General.nMkStAuto = MK_ST + (Mk4PtIdx::ChkSn);
				else
					General.nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 3:
			General.bContDiffLot = FALSE;
			General.bLotEnd = TRUE;
			General.nLotEndAuto = LOT_END;
			break;
		}
	}
}

void CManagerProcedure::Mk4PtReject()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	int a, b;

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case REJECT_ST:
			Buzzer(TRUE, 0);
			Stop();

			if (IDYES == pView->MsgBox(_T("��Ʈ üũ �ҷ��Դϴ�.\r\n���� ó���� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				General.nMkStAuto++;
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("���� ó���� ����Ͽ����ϴ�.\r\n�ҷ��� ��ŷ ó���� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					General.nMkStAuto++;
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
					General.nMkStAuto++;
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);	// Mk ��ŷ ����
				}
				else
					General.nMkStAuto = REJECT_ST;
			}
			break;
		case REJECT_ST + 2:
			SetReject();
			General.nMkStAuto++;
			break;
		case REJECT_ST + 3:
			General.nMkStAuto++;
			break;
		case REJECT_ST + 4:
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !General.nPrevMkStAuto)
				{
					General.nPrevMkStAuto = REJECT_ST + 4;
					General.nMkStAuto = REJECT_ST + 2;		// Mk ��ŷ ����
					General.bCam = TRUE;

					m_bDoneMk[0] = FALSE;
					m_bDoneMk[1] = FALSE;
					ResetMkStrip();
				}
				else
				{
					if (IsReview())
					{
						if (!General.bCam)
						{
							General.nPrevStepAuto = REJECT_ST + 4;
							General.nMkStAuto = REJECT_ST + 2;		// Mk ��ŷ ����
							General.bCam = TRUE;
							pView->MsgBox(_T("�� Jog ��ư�� �̿��Ͽ� ��ŷ��ġ�� Ȯ���Ͽ� �ּ���."));
						}
						else
						{
							General.bCam = FALSE;
							General.nMkStAuto++;	// Mk ��ŷ �Ϸ�

											//sMsg = _T("");
											//DispStsBar(sMsg, 0);
						}
					}
					else
					{
						General.nMkStAuto++;	// Mk ��ŷ �Ϸ�

										//sMsg = _T("");
										//DispStsBar(sMsg, 0);
					}
				}
			}
			break;
		case REJECT_ST + 5:
			General.nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk);				// Align���� �ʱ�ȭ
			break;
		}
	}
}

void CManagerProcedure::Mk4PtErrStop()
{
	stGeneral& General = (pView->m_mgrStatus->General);

	if (General.bMkSt && IsBuffer())
	{
		switch (General.nMkStAuto)
		{
		case ERROR_ST:
			Stop();

			if (IDYES == pView->MsgBox(_T("��Ʈ üũ Error�Դϴ�.\r\n�ٽ� ��Ʈ üũ�� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE;
				General.nMkStAuto++;
			}
			else
			{
				if (IDYES == pView->MsgBox(_T("��Ʈ üũ�� ����Ͽ����ϴ�.\r\n�ҷ��� ��ŷ ó���� �����Ͻðڽ��ϱ�?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE;
					General.nMkStAuto++;
				}
				else
					General.nMkStAuto++;
			}
			break;
		case ERROR_ST + 1:
			General.nMkStAuto++;
			break;
		case ERROR_ST + 2:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::ChkElec); // DoElecChk
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE;
					General.nMkStAuto = MK_ST + (Mk4PtIdx::DoMk);	// Mk ��ŷ ����
				}
				else
					General.nMkStAuto++;
			}
			break;
		case ERROR_ST + 3:
			General.nMkStAuto = ERROR_ST;
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
	//Mk4PtElecChk();
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
			pView->m_pDlgMenu01->General.bLastProc = TRUE;

		pView->MpeWrite(_T("MB440186"), 1);			// �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141112
		pView->MpeWrite(_T("MB440181"), 1);			// �ܷ�ó��(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)-20141031
		pDoc->LogAuto(_T("PC: �ܷ�ó�� AOI(��) ����(PC�� On��Ű��, PLC�� Ȯ���ϰ� Off��Ŵ)"));
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

BOOL CManagerProcedure::GetItsSerialInfo(int nItsSerial, BOOL &bDualTest, CString &sLot, CString &sLayerUp, CString &sLayerDn, int nOption)		// ���������� ITS �ø����� ����
{
	TCHAR szData[512];
	CString str, sName, sPath, Path[3];

	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//Path[1] = m_sEngModel;
	Path[2] = pDoc->m_sItsCode;

	sName.Format(_T("%s.txt"), pDoc->m_sItsCode);
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt

	if (sPath.IsEmpty())
		return FALSE;

	CString strTemp;
	CFileFind finder;
	if (finder.FindFile(sPath) == FALSE)
	{
		Path[1] = pDoc->m_sEngModel;
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

	// ITS_Code.txt ������ ����
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
		strFileData.Format(_T("PCR������ �������� �ʾҽ��ϴ�."));
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
		strFileData.Format(_T("PCR ������ �������� �ʽ��ϴ�.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
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

	if (pDoc->WorkingInfo.LastJob.sLotUp != PcrShare[0].sLot || pDoc->WorkingInfo.LastJob.sEngItsCode != PcrShare[0].sItsCode)
	{
		bUpdate = TRUE;
		pDoc->WorkingInfo.LastJob.sLotUp = PcrShare[0].sLot;
		pDoc->m_sItsCode = WorkingInfo.LastJob.sEngItsCode = PcrShare[0].sItsCode;
	}

	if (pDoc->WorkingInfo.LastJob.sModelUp != PcrShare[0].sModel || pDoc->WorkingInfo.LastJob.sLayerUp != PcrShare[0].sLayer || General.bInitAutoLoadMstInfo)
	{
		bUpdate = TRUE;
		pDoc->WorkingInfo.LastJob.sModelUp = PcrShare[0].sModel;
		pDoc->WorkingInfo.LastJob.sLayerUp = PcrShare[0].sLayer;

		if (General.bBufEmptyF[0])
		{
			if (!General.bBufEmpty[0])
				General.bBufEmptyF[0] = FALSE;

			pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo();
			if (pDoc->m_nAoiCamInfoStrPcs[0] > -1)
			{
				if ((pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != WorkingInfo.System.bStripPcsRgnBin)
				{
					return FALSE;
				}
			}

			General.bInitAutoLoadMstInfo = FALSE;
			return TRUE;
		}

		if (General.bInitAutoLoadMstInfo)
		{
			General.bInitAutoLoadMstInfo = FALSE;
			return TRUE;
		}
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();

		//if (m_pReelMapUp)
		//	m_pReelMapUp->ResetReelmapPath();

		//if (GetTestMode() == MODE_OUTER)
		//{
		//	BOOL bDualTestInner;
		//	CString sLot, sLayerUp, sLayerDn, str;
		//	if (!pDoc->GetItsSerialInfo(nSerial, bDualTestInner, sLot, sLayerUp, sLayerDn))
		//	{
		//		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		//		pView->MsgBox(str);
		//		return FALSE; // TRUE: CHANGED, FALSE: NO CHANGED 
		//	}

		//	if (m_pReelMapInnerUp)
		//		m_pReelMapInnerUp->ResetReelmapPath();

		//	if (bDualTestInner)
		//	{
		//		if (m_pReelMapInnerDn)
		//			m_pReelMapInnerDn->ResetReelmapPath();
		//		if (m_pReelMapInnerAllUp)
		//			m_pReelMapInnerAllUp->ResetReelmapPath();
		//		if (m_pReelMapInnerAllDn)
		//			m_pReelMapInnerAllDn->ResetReelmapPath();
		//	}
		//}
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
		strFileData.Format(_T("PCR������ �������� �ʾҽ��ϴ�."));
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
		strFileData.Format(_T("PCR ������ �������� �ʽ��ϴ�.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
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
		pDoc->m_sItsCode = WorkingInfo.LastJob.sEngItsCode = PcrShare[1].sItsCode;
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

			pDoc->m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo();
			if (pDoc->m_nAoiCamInfoStrPcs[1] > -1)
			{
				if ((pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE) != WorkingInfo.System.bStripPcsRgnBin)
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

int CManagerProcedure::GetBufUpSerial(int nCam)
{
	stGeneral& General = (pView->m_mgrStatus->General);
	return General.nBufUpSerial[nCam];
}

int CManagerProcedure::GetBufDnSerial(int nCam)
{
	stGeneral& General = (pView->m_mgrStatus->General);
	return General.nBufDnSerial[nCam];
}

void CManagerProcedure::DispDefImg()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nSerialL = General.nBufUpSerial[0]; // ���� Camera
	int nSerialR = General.nBufUpSerial[1]; // ���� Camera
	//CString sNewLot = m_sNewLotUp;;
	CString str;
	BOOL bNewModel = FALSE;
	int nNewLot = 0;
	int nBreak = 0;
	int nLastShot = 0;

	switch (Thread.nStepTHREAD_DISP_DEF)
	{
		// CopyDefImg Start ============================================
	case 0:
		Thread.nStepTHREAD_DISP_DEF++;

		if (nSerialL == General.nLotEndSerial)
			nBreak = 1;

		if (nSerialL > 0)
		{
			if (!CopyDefImg(nSerialL, sNewLot)) // ���� Camera
			{
				sNewLot.Empty();
				m_bDispMsgDoAuto[7] = TRUE;
				m_nStepDispMsg[7] = FROM_DISPDEFIMG + 7;
				break;
			}

			if (pView->m_bSerialDecrese)
			{
				if (General.bLastProc && nSerialR < General.nLotEndSerial)
				{
					nLastShot = nSerialL;
				}
				else
				{
					if (nSerialR > 0)
					{
						if (!CopyDefImg(nSerialR, sNewLot)) // ���� Camera
						{
							sNewLot.Empty();
							m_bDispMsgDoAuto[6] = TRUE;
							m_nStepDispMsg[6] = FROM_DISPDEFIMG + 6;
							break;
						}
					}
				}
			}
			else
			{
				if (General.bLastProc && nSerialR > General.nLotEndSerial)
				{
					nLastShot = nSerialL;
				}
				else
				{
					if (nSerialR > 0)
					{
						if (!CopyDefImg(nSerialR, sNewLot)) // ���� Camera
						{
							sNewLot.Empty();
							m_bDispMsgDoAuto[6] = TRUE;
							m_nStepDispMsg[6] = FROM_DISPDEFIMG + 6;
							break;
						}
					}
				}
			}

		}
		else
		{
			if (!General.bLastProc)
			{
				m_bDispMsgDoAuto[5] = TRUE;
				m_nStepDispMsg[5] = FROM_DISPDEFIMG + 5;
			}
		}
		sNewLot.Empty();
		break;
	case 1:
		//Sleep(300);
		Thread.nStepTHREAD_DISP_DEF++;
		if (m_pReelMap)
		{
			str = m_pReelMap->GetIpPath();
			pView->SetMkMenu01(_T("DispDefImg"), _T("ReelmapPath"), str);
		}
		str.Format(_T("%d"), nSerialL);
		pView->SetMkMenu01(_T("DispDefImg"), _T("SerialL"), str);
		str.Format(_T("%d"), nSerialR);
		pView->SetMkMenu01(_T("DispDefImg"), _T("SerialR"), str);
		break;
	case 2:
		if (IsDoneDispMkInfo())	 // Check �ҷ��̹��� Display End
		{
			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerialL < General.nLotEndSerial))
				{
					nLastShot = nSerialL;
					SetSerialReelmap(nSerialL, TRUE);	// Reelmap(��) Display Start
					SetSerialMkInfo(nSerialL, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else if (ChkLastProc() && nSerialL < 1)
				{
					SetSerialReelmap(General.nLotEndSerial + 1, TRUE);		// Reelmap(��) Display Start
					SetSerialMkInfo(General.nLotEndSerial + 1, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else
				{
					SetFixPcs(nSerialL);
					SetSerialReelmap(nSerialL);	// Reelmap(��) Display Start
					SetSerialMkInfo(nSerialL);	// �ҷ��̹���(��) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerialL > General.nLotEndSerial))
				{
					nLastShot = nSerialL;
					SetSerialReelmap(nSerialL, TRUE);	// Reelmap(��) Display Start
					SetSerialMkInfo(nSerialL, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else if (ChkLastProc() && nSerialL < 1)
				{
					SetSerialReelmap(General.nLotEndSerial + 1, TRUE);		// Reelmap(��) Display Start
					SetSerialMkInfo(General.nLotEndSerial + 1, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else
				{
					SetFixPcs(nSerialL);
					SetSerialReelmap(nSerialL);	// Reelmap(��) Display Start
					SetSerialMkInfo(nSerialL);	// �ҷ��̹���(��) Display Start
				}
			}


			if (IsFixPcsUp(nSerialL))
			{
				m_bDispMsgDoAuto[2] = TRUE;
				m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			}
			if (IsFixPcsDn(nSerialL))
			{
				m_bDispMsgDoAuto[3] = TRUE;
				m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
			}

			Thread.nStepTHREAD_DISP_DEF++;
		}

		break;

	case 3:
		if (IsDoneDispMkInfo())	 // Check �ҷ��̹���(��) Display End
		{
			if (nSerialR > 0) // ���� Camera
			{
				if (pView->m_bSerialDecrese)
				{
					if (ChkLastProc() && (nSerialR < General.nLotEndSerial))
						SetSerialMkInfo(nSerialR, TRUE);	// �ҷ��̹���(��) Display Start
					else
						SetSerialMkInfo(nSerialR);		// �ҷ��̹���(��) Display Start
				}
				else
				{
					if (ChkLastProc() && (nSerialR > General.nLotEndSerial))
						SetSerialMkInfo(nSerialR, TRUE);	// �ҷ��̹���(��) Display Start
					else
						SetSerialMkInfo(nSerialR);		// �ҷ��̹���(��) Display Start
				}
			}
			else
			{
				if (ChkLastProc())
				{
					if (pView->m_bSerialDecrese)
						SetSerialMkInfo(General.nLotEndSerial - 1, TRUE);	// �ҷ��̹���(��) Display Start
					else
						SetSerialMkInfo(General.nLotEndSerial + 1, TRUE);	// �ҷ��̹���(��) Display Start
				}
			}
			Thread.nStepTHREAD_DISP_DEF++;
		}
		break;
	case 4:
		if (nSerialR > 0) // ���� Camera
		{
			Thread.nStepTHREAD_DISP_DEF++;

			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerialR < General.nLotEndSerial))
				{
					SetSerialReelmap(nSerialR, TRUE);	// Reelmap(��) Display Start
				}
				else
				{
					SetFixPcs(nSerialR);
					SetSerialReelmap(nSerialR);			// Reelmap(��) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerialR > General.nLotEndSerial))
				{
					SetSerialReelmap(nSerialR, TRUE);	// Reelmap(��) Display Start
				}
				else
				{
					SetFixPcs(nSerialR);
					SetSerialReelmap(nSerialR);			// Reelmap(��) Display Start
				}
			}

			if (bDualTest)
			{
				if (IsFixPcsUp(nSerialR))
				{
					m_bDispMsgDoAuto[2] = TRUE;
					m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
				}
				if (IsFixPcsDn(nSerialR))
				{
					m_bDispMsgDoAuto[3] = TRUE;
					m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
				}
			}
			else
			{
				if (IsFixPcsUp(nSerialR))
				{
					m_bDispMsgDoAuto[2] = TRUE;
					m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
				}
			}
		}
		else
		{
			if (ChkLastProc())
			{
				Thread.nStepTHREAD_DISP_DEF++;
				if (pView->m_bSerialDecrese)
					SetSerialReelmap(General.nLotEndSerial - 1, TRUE);	// �ҷ��̹���(��) Display Start
				else
					SetSerialReelmap(General.nLotEndSerial + 1, TRUE);	// �ҷ��̹���(��) Display Start
			}
			else
			{
				if (General.bLastProc && nSerialL == General.nLotEndSerial)
					Thread.nStepTHREAD_DISP_DEF++;
				else
				{
					Thread.nStepTHREAD_DISP_DEF++;
				}
			}
		}
		break;
	case 5:
		Thread.nStepTHREAD_DISP_DEF++;
		break;
	case 6:
		Thread.nStepTHREAD_DISP_DEF++;
		break;
	case 7:
		Thread.nStepTHREAD_DISP_DEF++;
		break;
	case 8:
		Thread.nStepTHREAD_DISP_DEF++;
		break;
	case 9:
		Thread.nStepTHREAD_DISP_DEF++;
		break;
	case 10:
		Thread.nStepTHREAD_DISP_DEF++;
		break;

	case 11:
		if (IsDoneDispMkInfo() && IsRun())	 // Check �ҷ��̹���(��) Display End
			Thread.nStepTHREAD_DISP_DEF++;
		break;
	case 12:
		Thread.bTHREAD_DISP_DEF = FALSE;
		pView->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("0"));
		if (m_pEngrave)
			m_pEngrave->SwMenu01DispDefImg(TRUE);

		break;
		// CopyDefImg End ============================================
	}

}

void CManagerProcedure::DispDefImgInner()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	int nBreak = 0;

	switch (Thread.nStepTHREAD_DISP_DEF_INNER)
	{
		// CopyDefImg Start ============================================
	case 0:
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 1:
		Sleep(300);
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 2:
		nSerial = General.nBufUpSerial[0]; // ���� Camera

		if (IsDoneDispMkInfoInner())	 // Check �ҷ��̹��� Display End
		{
			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerial < General.nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(��) Display Start
					SetSerialMkInfoInner(nSerial, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else if (ChkLastProc() && nSerial < 1)
				{
					SetSerialReelmapInner(General.nLotEndSerial - 1, TRUE);		// Reelmap(��) Display Start
					SetSerialMkInfoInner(General.nLotEndSerial - 1, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else
				{
					SetSerialReelmapInner(nSerial);	// Reelmap(��) Display Start
					SetSerialMkInfoInner(nSerial);	// �ҷ��̹���(��) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerial > General.nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(��) Display Start
					SetSerialMkInfoInner(nSerial, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else if (ChkLastProc() && nSerial < 1)
				{
					SetSerialReelmapInner(General.nLotEndSerial + 1, TRUE);		// Reelmap(��) Display Start
					SetSerialMkInfoInner(General.nLotEndSerial + 1, TRUE);		// �ҷ��̹���(��) Display Start
				}
				else
				{
					//SetFixPcs(nSerial);
					SetSerialReelmapInner(nSerial);	// Reelmap(��) Display Start
					SetSerialMkInfoInner(nSerial);	// �ҷ��̹���(��) Display Start
				}
			}

			Thread.nStepTHREAD_DISP_DEF_INNER++;
		}

		break;

	case 3:
		if (IsDoneDispMkInfoInner())	 // Check �ҷ��̹���(��) Display End
		{
			nSerial = General.nBufUpSerial[1]; // ���� Camera

			if (nSerial > 0)
			{
				if (pView->m_bSerialDecrese)
				{
					if (ChkLastProc() && (nSerial < General.nLotEndSerial))
						SetSerialMkInfoInner(nSerial, TRUE);	// �ҷ��̹���(��) Display Start
					else
						SetSerialMkInfoInner(nSerial);		// �ҷ��̹���(��) Display Start
				}
				else
				{
					if (ChkLastProc() && (nSerial > General.nLotEndSerial))
						SetSerialMkInfoInner(nSerial, TRUE);	// �ҷ��̹���(��) Display Start
					else
						SetSerialMkInfoInner(nSerial);		// �ҷ��̹���(��) Display Start
				}
			}
			else
			{
				if (pView->m_bSerialDecrese)
				{
					if (ChkLastProc())
						SetSerialMkInfoInner(General.nLotEndSerial - 1, TRUE);	// �ҷ��̹���(��) Display Start
				}
				else
				{
					if (ChkLastProc())
						SetSerialMkInfoInner(General.nLotEndSerial + 1, TRUE);	// �ҷ��̹���(��) Display Start
				}
			}
			Thread.nStepTHREAD_DISP_DEF_INNER++;
		}
		break;
	case 4:
		nSerial = General.nBufUpSerial[1]; // ���� Camera

		if (nSerial > 0)
		{
			Thread.nStepTHREAD_DISP_DEF_INNER++;

			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerial < General.nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(��) Display Start
				}
				else
				{
					//SetFixPcs(nSerial);
					SetSerialReelmapInner(nSerial);			// Reelmap(��) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerial > General.nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(��) Display Start
				}
				else
				{
					//SetFixPcs(nSerial);
					SetSerialReelmapInner(nSerial);			// Reelmap(��) Display Start
				}
			}
		}
		else
		{
			if (ChkLastProc())
			{
				Thread.nStepTHREAD_DISP_DEF_INNER++;
				if (pView->m_bSerialDecrese)
					SetSerialReelmapInner(General.nLotEndSerial - 1, TRUE);	// �ҷ��̹���(��) Display Start
				else
					SetSerialReelmapInner(General.nLotEndSerial + 1, TRUE);	// �ҷ��̹���(��) Display Start
			}
			else
			{
				if (bDualTest)
				{
					if (General.bLastProc && General.nBufDnSerial[0] == General.nLotEndSerial)
						Thread.nStepTHREAD_DISP_DEF_INNER++;
					else
					{
						Thread.nStepTHREAD_DISP_DEF_INNER++;
					}
				}
				else
				{
					if (General.bLastProc && General.nBufUpSerial[0] == General.nLotEndSerial)
						Thread.nStepTHREAD_DISP_DEF_INNER++;
					else
					{
						Thread.nStepTHREAD_DISP_DEF_INNER++;
					}
				}
			}
		}
		break;
	case 5:
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 6:
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 7:
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 8:
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 9:
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 10:
		Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;

	case 11:
		if (IsDoneDispMkInfoInner() && IsRun())	 // Check �ҷ��̹���(��) Display End
			Thread.nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 12:
		Thread.bTHREAD_DISP_DEF_INNER = FALSE;
		break;
		// Disp DefImg End ============================================
	}

}

void CManagerProcedure::Shift2Mk()
{
	stGeneral& General = (pView->m_mgrStatus->General);

	int nSerialL = General.nBufUpSerial[0];
	int nSerialR = General.nBufUpSerial[1];
	int nLastShot = 0;

	if (General.bLastProc && nSerialL == General.nLotEndSerial)
	{
		if (nSerialL > 0)
			pDoc->Shift2Mk(nSerialL);	// Cam0

		if (pView->m_pDlgFrameHigh)
			pView->m_pDlgFrameHigh->SetMkLastShot(nSerialL);
	}
	else
	{
		if (nSerialL > 0)
		{
			pDoc->Shift2Mk(nSerialL);	// Cam0
			nLastShot = nSerialL;

			if (nSerialR > 0)
			{
				pDoc->Shift2Mk(nSerialR);	// Cam1
				nLastShot = nSerialR;
			}

			if (pView->m_pDlgFrameHigh)
				pView->m_pDlgFrameHigh->SetMkLastShot(nLastShot);
		}
	}

	m_bShift2Mk = FALSE;
}

void CManagerProcedure::Shift2MkFromThread() // DoShift2Mk
{
	pView->m_mgrThread->Shift2MkFromThread();
}

void CManagerProcedure::StringToChar(CString str, char *szStr)
{
	int nLen = str.GetLength();
	strcpy(szStr, CT2A(str));
	szStr[nLen] = _T('\0');
}

void CManagerProcedure::ChgLot()
{
	stPcrShare* PcrShare = (pView->m_mgrStatus->PcrShare);

	pDoc->WorkingInfo.LastJob.sLotUp = PcrShare[0].sLot;
	SetModelInfoUp();

	pDoc->WorkingInfo.LastJob.sLotDn = PcrShare[1].sLot;
	SetModelInfoDn();

	SetPathAtBuf();
}

void CManagerProcedure::SetModelInfoUp()
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData = pDoc->WorkingInfo.LastJob.sModelUp;
	::WritePrivateProfileString(_T("Last Job"), _T("ModelUp Name"), sData, sPath);

	sData = pDoc->WorkingInfo.LastJob.sLayerUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerUp Name"), sData, sPath);

	sData = pDoc->WorkingInfo.LastJob.sLotUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LotUp No"), sData, sPath);
}

void CManagerProcedure::SetModelInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	CString sData, sPath = PATH_WORKING_INFO;
	sData = pDoc->WorkingInfo.LastJob.sModelDn;
	::WritePrivateProfileString(_T("Last Job"), _T("ModelDn Name"), sData, sPath);

	sData = pDoc->WorkingInfo.LastJob.sLayerDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerDn Name"), sData, sPath);

	sData = pDoc->WorkingInfo.LastJob.sLotDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LotDn No"), sData, sPath);
}

void CManagerProcedure::SetPathAtBuf()
{
	pView->m_mgrReelmap->SetPathAtBuf();
}

BOOL CManagerProcedure::InitMk()
{
	//m_nStepElecChk = 0;
	int nRSer = ChkSerial(); // (nSerial0 - nLastShot) -> 0: Same Serial, -: Decrese Serial, +: Increase Serial 
	int nSerial = GetBuffer0();

	if (nRSer)
	{
		m_bSerialDecrese = FALSE;

		if (pDoc->m_bUseRTRYShiftAdjust)
		{
			if (nSerial > 2)
				AdjPinPos();
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CManagerProcedure::MoveAlign0(int nPos)
{
	return pView->m_mgrPunch->MoveAlign0(nPos);
}

BOOL CManagerProcedure::MoveAlign1(int nPos)
{
	return pView->m_mgrPunch->MoveAlign1(nPos);
}

BOOL CManagerProcedure::IsMoveDone()
{
	return pView->m_mgrPunch->IsMoveDone();
}

BOOL CManagerProcedure::IsMoveDone0()
{
	return pView->m_mgrPunch->IsMoveDone0();
}

BOOL CManagerProcedure::IsMoveDone1()
{
	return pView->m_mgrPunch->IsMoveDone1();
}

BOOL CManagerProcedure::TwoPointAlign0(int nPos)
{
	return pView->m_mgrPunch->TwoPointAlign0(nPos);
}

BOOL CManagerProcedure::TwoPointAlign1(int nPos)
{
	return pView->m_mgrPunch->TwoPointAlign1(nPos);
}

void CManagerProcedure::Buzzer(BOOL bOn, int nCh)
{
	if (pView->m_mgrFeeding)
		pView->m_mgrFeeding->Buzzer(bOn, nCh);
}

BOOL CManagerProcedure::SetMkIts(BOOL bRun)	// Marking Start
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	CfPoint ptPnt;
	int nSerial, nTot, a, b;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	ResetMkStrip();

	if (bRun)
	{
		if (m_bDoMk[0])
		{
			if (!Thread.bTHREAD_MK[0])
			{
				m_nStepMk[0] = 0;
				m_nMkPcs[0] = 0;
				m_bDoneMk[0] = FALSE;
				Thread.bTHREAD_MK[0] = TRUE;

				nSerial = General.nBufUpSerial[0]; // Cam0

				m_nTotMk[0] = nTot = GetTotDefPcs0Its(nSerial);
				m_nCurMk[0] = 0;
				if (nTot > 0)
				{
					ptPnt = GetMkPnt0Its(nSerial, 0);
					m_dTarget[AXIS_X0] = ptPnt.x;
					m_dTarget[AXIS_Y0] = ptPnt.y;
					if (nTot > 1)
					{
						ptPnt = GetMkPnt0Its(nSerial, 1);
						m_dNextTarget[AXIS_X0] = ptPnt.x;
						m_dNextTarget[AXIS_Y0] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X0] = -1.0;
						m_dNextTarget[AXIS_Y0] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X0] = -1.0;
					m_dTarget[AXIS_Y0] = -1.0;
					m_dNextTarget[AXIS_X0] = -1.0;
					m_dNextTarget[AXIS_Y0] = -1.0;
				}
			}
		}

		if (m_bDoMk[1])
		{
			if (!Thread.bTHREAD_MK[1])
			{
				m_nStepMk[1] = 0;
				m_nMkPcs[1] = 0;
				m_bDoneMk[1] = FALSE;
				Thread.bTHREAD_MK[1] = TRUE;

				nSerial = General.nBufUpSerial[1]; // Cam1

				m_nTotMk[1] = nTot = GetTotDefPcs1Its(nSerial);
				m_nCurMk[1] = 0;
				if (nTot > 0)
				{
					ptPnt = GetMkPnt1Its(nSerial, 0);
					m_dTarget[AXIS_X1] = ptPnt.x;
					m_dTarget[AXIS_Y1] = ptPnt.y;
					if (nTot > 1)
					{
						ptPnt = GetMkPnt1Its(nSerial, 1);
						m_dNextTarget[AXIS_X1] = ptPnt.x;
						m_dNextTarget[AXIS_Y1] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X1] = -1.0;
						m_dNextTarget[AXIS_Y1] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X1] = -1.0;
					m_dTarget[AXIS_Y1] = -1.0;
					m_dNextTarget[AXIS_X1] = -1.0;
					m_dNextTarget[AXIS_Y1] = -1.0;
				}
			}
		}
	}
	else
	{
		Thread.bTHREAD_MK[0] = FALSE;
		Thread.bTHREAD_MK[1] = FALSE;
	}
	return TRUE;
}

BOOL CManagerProcedure::SetMk(BOOL bRun)	// Marking Start
{
	CfPoint ptPnt;
	int nSerial, nTot, a, b;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	ResetMkStrip();

	if (bRun)
	{
		if (m_bDoMk[0])
		{
			if (!Thread.bTHREAD_MK[0])
			{
				m_nStepMk[0] = 0;
				m_nMkPcs[0] = 0;
				m_bDoneMk[0] = FALSE;
				//m_bReMark[0] = FALSE;
				Thread.bTHREAD_MK[0] = TRUE;

				nSerial = General.nBufUpSerial[0]; // Cam0

				m_nTotMk[0] = nTot = GetTotDefPcs0(nSerial);
				m_nCurMk[0] = 0;
				if (nTot > 0)
				{
					ptPnt = GetMkPnt0(nSerial, 0);
					m_dTarget[AXIS_X0] = ptPnt.x;
					m_dTarget[AXIS_Y0] = ptPnt.y;
					if (nTot > 1)
					{
						ptPnt = GetMkPnt0(nSerial, 1);
						m_dNextTarget[AXIS_X0] = ptPnt.x;
						m_dNextTarget[AXIS_Y0] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X0] = -1.0;
						m_dNextTarget[AXIS_Y0] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X0] = -1.0;
					m_dTarget[AXIS_Y0] = -1.0;
					m_dNextTarget[AXIS_X0] = -1.0;
					m_dNextTarget[AXIS_Y0] = -1.0;
				}
			}
		}

		if (m_bDoMk[1])
		{
			if (!Thread.bTHREAD_MK[1])
			{
				m_nStepMk[1] = 0;
				m_nMkPcs[1] = 0;
				m_bDoneMk[1] = FALSE;
				//m_bReMark[1] = FALSE;
				Thread.bTHREAD_MK[1] = TRUE;

				nSerial = General.nBufUpSerial[1]; // Cam1

				m_nTotMk[1] = nTot = GetTotDefPcs1(nSerial);
				m_nCurMk[1] = 0;
				if (nTot > 0)
				{
					ptPnt = GetMkPnt1(nSerial, 0);
					m_dTarget[AXIS_X1] = ptPnt.x;
					m_dTarget[AXIS_Y1] = ptPnt.y;
					if (nTot > 1)
					{
						ptPnt = GetMkPnt1(nSerial, 1);
						m_dNextTarget[AXIS_X1] = ptPnt.x;
						m_dNextTarget[AXIS_Y1] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X1] = -1.0;
						m_dNextTarget[AXIS_Y1] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X1] = -1.0;
					m_dTarget[AXIS_Y1] = -1.0;
					m_dNextTarget[AXIS_X1] = -1.0;
					m_dNextTarget[AXIS_Y1] = -1.0;
				}
			}
		}
	}
	else
	{
		Thread.bTHREAD_MK[0] = FALSE;
		Thread.bTHREAD_MK[1] = FALSE;
	}
	return TRUE;
}

void CManagerProcedure::SetReMk(BOOL bMk0, BOOL bMk1)
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	CfPoint ptPnt;
	int nSerial, nTot;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bMk0)
	{
		if (!Thread.bTHREAD_MK[0])
		{
			m_nStepMk[0] = 0;
			m_nMkPcs[0] = 0;
			m_bDoneMk[0] = FALSE;
			Thread.bTHREAD_MK[0] = TRUE;

			nSerial = General.nBufUpSerial[0]; // Cam0

			nTot = GetTotDefPcs0(nSerial);
			if (nTot > 0)
			{
				ptPnt = GetMkPnt0(nSerial, 0);
				m_dTarget[AXIS_X0] = ptPnt.x;
				m_dTarget[AXIS_Y0] = ptPnt.y;
				if (nTot > 1)
				{
					ptPnt = GetMkPnt0(nSerial, 1);
					m_dNextTarget[AXIS_X0] = ptPnt.x;
					m_dNextTarget[AXIS_Y0] = ptPnt.y;
				}
				else
				{
					m_dNextTarget[AXIS_X0] = -1.0;
					m_dNextTarget[AXIS_Y0] = -1.0;
				}
			}
			else
			{
				m_dTarget[AXIS_X0] = -1.0;
				m_dTarget[AXIS_Y0] = -1.0;
				m_dNextTarget[AXIS_X0] = -1.0;
				m_dNextTarget[AXIS_Y0] = -1.0;
			}
		}
	}

	if (bMk1)
	{
		if (!Thread.bTHREAD_MK[1])
		{
			m_nStepMk[1] = 0;
			m_nMkPcs[1] = 0;
			m_bDoneMk[1] = FALSE;
			//m_bReMark[1] = FALSE;
			Thread.bTHREAD_MK[1] = TRUE;

			nSerial = General.nBufUpSerial[1]; // Cam1

			nTot = GetTotDefPcs1(nSerial);
			if (nTot > 0)
			{
				ptPnt = GetMkPnt1(nSerial, 0);
				m_dTarget[AXIS_X1] = ptPnt.x;
				m_dTarget[AXIS_Y1] = ptPnt.y;
				if (nTot > 1)
				{
					ptPnt = GetMkPnt1(nSerial, 1);
					m_dNextTarget[AXIS_X1] = ptPnt.x;
					m_dNextTarget[AXIS_Y1] = ptPnt.y;
				}
				else
				{
					m_dNextTarget[AXIS_X1] = -1.0;
					m_dNextTarget[AXIS_Y1] = -1.0;
				}
			}
			else
			{
				m_dTarget[AXIS_X1] = -1.0;
				m_dTarget[AXIS_Y1] = -1.0;
				m_dNextTarget[AXIS_X1] = -1.0;
				m_dNextTarget[AXIS_Y1] = -1.0;
			}
		}
	}
}

BOOL CManagerProcedure::CheckMkPnt()
{
	if (pView->m_mgrPunch)
		return pView->m_mgrPunch->CheckMkPnt();
	return FALSE;
}

void CManagerProcedure::UpdateYield() 
{
	if (pView->m_mgrThread)
		pView->m_mgrThread->UpdateYield();
}

void CManagerProcedure::UpdateWorking()
{
	pView->UpdateWorking();
	if (m_pEngrave)
		m_pEngrave->SwMenu01UpdateWorking(TRUE);
}

BOOL CManagerProcedure::IsMkDone()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (m_bDoneMk[0] && m_bDoneMk[1] && !Thread.bTHREAD_DISP_DEF)
		return TRUE;
	return FALSE;
}

BOOL CManagerProcedure::IsVerify()
{
	if (!pView->m_mgrFeeding || pView->m_mgrStatus) return FALSE;
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bVerify = FALSE;
	int nSerial0 = General.nBufUpSerial[0]; // Cam0
	int nSerial1 = General.nBufUpSerial[1]; // Cam1
	int nPeriod = pDoc->WorkingInfo.LastJob.nVerifyPeriod;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->WorkingInfo.LastJob.bVerify)
	{
		double dFdLen = pView->m_mgrFeeding->GetMkFdLen(); 
		double dVerifyLen = _tstof(pDoc->WorkingInfo.LastJob.sVerifyLen)*1000.0;

		if (dFdLen < dVerifyLen)
		{
			if (nSerial0 == 1 || nPeriod == 0 || nPeriod == 1 || nPeriod == 2 || General.bStopF_Verify)
			{
				General.bStopF_Verify = FALSE;
				bVerify = TRUE;
			}
			else
			{
				if (!(nSerial0 % nPeriod) || !(nSerial1 % nPeriod))
					bVerify = TRUE;
			}
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bVerify = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();
		}
	}

	return bVerify;
}

BOOL CManagerProcedure::IsReview()
{
	return (pDoc->WorkingInfo.LastJob.bReview);
}

BOOL CManagerProcedure::IsReMk()
{
	if (IsMoveDone())
		return FALSE;

	if (m_bReMark[0] && m_bDoneMk[1])
	{
		m_bReMark[0] = FALSE;
		SetReMk(TRUE, FALSE);
		return TRUE;
	}
	else if (m_bDoneMk[0] && m_bReMark[1])
	{
		m_bReMark[1] = FALSE;
		SetReMk(FALSE, TRUE);
		return TRUE;
	}
	else if (m_bReMark[0] && m_bReMark[1])
	{
		m_bReMark[0] = FALSE;
		m_bReMark[1] = FALSE;
		SetReMk(TRUE, TRUE);
		return TRUE;
	}

	return FALSE;
}

BOOL CManagerProcedure::CopyDefImg(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bUp, bDn;
	if (bDualTest)
	{
		bUp = CopyDefImgUp(nSerial, sNewLot);
		bDn = CopyDefImgDn(nSerial, sNewLot);

		if (!bUp || !bDn)
			return FALSE;
	}
	else
	{
		bUp = CopyDefImgUp(nSerial, sNewLot);

		if (!bUp)
			return FALSE;
	}

	return TRUE;
}

BOOL CManagerProcedure::CopyDefImgUp(int nSerial, CString sNewLot)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.18"));
		return 0;
	}

	CString strDefImgPathS, strDefImgPathD;// , strMakeFolderPath;
	int i;
	CFileFind finder;
	CString strTemp;
	CString sLot;

	CString strAOIImgDataPath;
	strAOIImgDataPath.Format(_T("%s\\VRSImage"), pDoc->WorkingInfo.System.sPathAoiUpDefImg);

	if (sNewLot.IsEmpty())
		sLot = pDoc->WorkingInfo.LastJob.sLotUp;
	else
		sLot = sNewLot;

	pDoc->MakeImageDirUp(nSerial);

	int nIdx = GetIdxPcrBufUp(nSerial);
	if (nIdx < 0)
		return FALSE;

	int nTotDef = 0;
	if (m_pPcr[0])
	{
		if (m_pPcr[0][nIdx])
			nTotDef = m_pPcr[0][nIdx]->m_nTotDef;
	}
	if (nTotDef <= 0)
		return TRUE;

	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		if (pDoc->m_pPcr[0][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			int nDefImg = pDoc->m_pPcr[0][nIdx]->m_pImg[i];
			if (strAOIImgDataPath.Right(1) != "\\")
				strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLayerUp,
					sLot,
					nSerial,
					nDefImg);
			else
				strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLayerUp,
					sLot,
					nSerial,
					nDefImg);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg);
			else
				strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg);

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}


			int nStrip = -1, nCol = -1, nRow = -1;
			int nPcrIdx = pDoc->GetPcrIdx0(nSerial);
			int nPcsIdx = pDoc->m_pPcr[0][nPcrIdx]->m_pDefPcs[i];
			int nDefCode = pDoc->m_pPcr[0][nPcrIdx]->m_pDefType[i];
			if (pDoc->m_Master[0].m_pPcsRgn)
				pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg, m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}
			else
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg, m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image Position File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image Position File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}
		}
	}

	return TRUE;
}

BOOL CManagerProcedure::CopyDefImgDn(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.19"));
		return 0;
	}

	CString strDefImgPathS, strDefImgPathD;// , strMakeFolderPath;
	int i;
	CFileFind finder;
	CString strTemp;
	CString sLot;

	if (sNewLot.IsEmpty())
		sLot = WorkingInfo.LastJob.sLotDn;
	else
		sLot = sNewLot;

	CString strAOIImgDataPath;
	strAOIImgDataPath.Format(_T("%s\\VRSImage"), WorkingInfo.System.sPathAoiDnDefImg);

	MakeImageDirDn(nSerial);

	int nIdx = GetIdxPcrBufDn(nSerial);
	if (nIdx < 0)
		return FALSE;

	int nTotDef = 0;
	if (m_pPcr[1])
	{
		if (m_pPcr[1][nIdx])
			nTotDef = m_pPcr[1][nIdx]->m_nTotDef;
	}
	if (nTotDef <= 0)
		return TRUE;

	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		if (pDoc->m_pPcr[1][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			int nDefImg = pDoc->m_pPcr[1][nIdx]->m_pImg[i];

			if (strAOIImgDataPath.Right(1) != "\\")
				strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelUp,
					//WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLayerDn,
					sLot,
					nSerial,
					nDefImg);
			else
				strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModelUp,
					//WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLayerDn,
					sLot,
					nSerial,
					nDefImg);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					//WorkingInfo.LastJob.sModelDn,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg);
			else
				strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					//WorkingInfo.LastJob.sModelDn,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg);

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}


			int nStrip = -1, nCol = -1, nRow = -1;
			int nPcrIdx = pDoc->GetPcrIdx1(nSerial);
			int nPcsIdx = pDoc->m_pPcr[1][nPcrIdx]->m_pDefPcs[i];
			int nDefCode = pDoc->m_pPcr[1][nPcrIdx]->m_pDefType[i];
			if (pDoc->m_Master[0].m_pPcsRgn)
				pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg, m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}
			else
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg, m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image Position File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image Position File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}
		}
	}

	return TRUE;
}

int CManagerProcedure::GetIdxPcrBufUp(int nSerial)
{
	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapUp) return 0;
	CDataMarking* pPcr = NULL;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.21"));
		return 0;
	}

	int i, nIdx = -1;

	for (i = 0; i < MAX_PCR_PNL; i++)
	{
		pPcr = pView->m_mgrReelmap->m_pReelMapUp->GetPcr(i);
		if (pPcr->m_nSerial == nSerial)
		{
			nIdx = i;
			break;
		}
	}

	return nIdx;
}

int CManagerProcedure::GetIdxPcrBufDn(int nSerial)
{
	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapDn) return 0;
	CDataMarking* pPcr = NULL;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.22"));
		return 0;
	}

	int i, nIdx = -1;

	for (i = 0; i < MAX_PCR_PNL; i++)
	{
		pPcr = pView->m_mgrReelmap->m_pReelMapDn->GetPcr(i);
		if (pPcr->m_nSerial == nSerial)
		{
			nIdx = i;
			break;
		}
	}

	return nIdx;
}

void CManagerProcedure::CompletedMk(int nCam) // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
{
	if (!pView->m_mgrStatus) return;
	stGeneral& General = (pView->m_mgrStatus->General);

	int nSerial = -1;

	switch (nCam)
	{
	case 0:
		nSerial = General.nBufUpSerial[0];
		break;
	case 1:
		nSerial = General.nBufUpSerial[1];
		break;
	case 2:
		nSerial = General.nBufUpSerial[1];
		break;
	case 3:
		return;
	default:
		return;
	}

	pView->SetCompletedSerial(nSerial);
}

int CManagerProcedure::GetMkStrip(int nCameraIdx, int nStripIdx) // [nCam][nStrip] - [��/��][] : ��Ʈ���� ��Ī�� �ǽ� �� count
{
	return pView->m_mgrPunch->GetMkStrip(nCameraIdx, nStripIdx);
}

void CManagerProcedure::ResetMkStrip()
{
	pView->m_mgrPunch->ResetMkStrip();
}

void CManagerProcedure::UpdateYieldOnRmap()
{
	if (!pView->m_mgrStatus)	return;
	stThread& Thread = (pView->m_mgrStatus->Thread);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	Thread.bTHREAD_REELMAP_YIELD_UP = TRUE;		// UpdateReelmapYieldUp(); // Yield Reelmap
	if (bDualTest)
		Thread.bTHREAD_REELMAP_YIELD_DN = TRUE;	// UpdateReelmapYieldDn(); // Yield Reelmap
}

BOOL CManagerProcedure::IsNoMk()
{
	if (!pView->m_mgrStatus)	return FALSE;
	stGeneral& General = (pView->m_mgrStatus->General);

	BOOL bNoMk = (pDoc->WorkingInfo.System.bNoMk | General.bCam);
	return bNoMk;
}

void CManagerProcedure::ResetReelmapPath()
{
	if (pView->m_mgrReelmap)
		pView->m_mgrReelmap->ResetReelmapPath();
}

BOOL CManagerProcedure::ChkYield()
{
	if (!pView->m_mgrReelmap) return FALSE;
	return pView->m_mgrReelmap->ChkYield();
}
