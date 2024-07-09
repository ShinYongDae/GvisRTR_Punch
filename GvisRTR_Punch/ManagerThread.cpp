#include "stdafx.h"
#include "ManagerThread.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

CManagerThread::CManagerThread(CWnd* pParent)
{
	m_pParent = pParent;

	Reset();
	Init();
}

CManagerThread::~CManagerThread()
{
	ThreadKill();
}

BEGIN_MESSAGE_MAP(CManagerThread, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerThread::Create()
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

void CManagerThread::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnTimer(nIDEvent);
}

void CManagerThread::Init()
{
	m_bThread[0] = FALSE;
	m_dwThreadTick[0] = 0;
	m_bThread[1] = FALSE;
	m_dwThreadTick[1] = 0;
	m_bThread[2] = FALSE;
	m_dwThreadTick[2] = 0;
	m_bThread[3] = FALSE;
	m_dwThreadTick[3] = 0;
	m_bThread[4] = FALSE;
	m_dwThreadTick[4] = 0;
	m_bThread[5] = FALSE;
	m_dwThreadTick[5] = 0;
	m_bThread[6] = FALSE;
	m_dwThreadTick[6] = 0;
	m_bThread[7] = FALSE;
	m_dwThreadTick[7] = 0;
	m_bThread[8] = FALSE;
	m_dwThreadTick[8] = 0;
	m_bThread[9] = FALSE;
	m_dwThreadTick[9] = 0;
	m_bThread[10] = FALSE;
	m_dwThreadTick[10] = 0;
	m_bThread[11] = FALSE;
	m_dwThreadTick[11] = 0;
	m_bThread[12] = FALSE;
	m_dwThreadTick[12] = 0;
	m_bThread[13] = FALSE;
	m_dwThreadTick[13] = 0;
	m_bThread[14] = FALSE;
	m_dwThreadTick[14] = 0;
	m_bThread[15] = FALSE;
	m_dwThreadTick[15] = 0;
	m_bThread[16] = FALSE;
	m_dwThreadTick[16] = 0;
	m_bThread[17] = FALSE;
	m_dwThreadTick[17] = 0;
	m_bThread[18] = FALSE;
	m_dwThreadTick[18] = 0;
	m_bThread[19] = FALSE;
	m_dwThreadTick[19] = 0;
	m_bThread[20] = FALSE;
	m_dwThreadTick[20] = 0;
	m_bThread[21] = FALSE;
	m_dwThreadTick[21] = 0;
	m_bThread[22] = FALSE;
	m_dwThreadTick[22] = 0;
	m_bThread[23] = FALSE;
	m_dwThreadTick[23] = 0;
	m_bThread[24] = FALSE;
	m_dwThreadTick[24] = 0;
	m_bThread[25] = FALSE;
	m_dwThreadTick[25] = 0;
	m_bThread[26] = FALSE;
	m_dwThreadTick[26] = 0;
	m_bThread[27] = FALSE;
	m_dwThreadTick[27] = 0;
	m_bThread[28] = FALSE;
	m_dwThreadTick[28] = 0;
	m_bThread[29] = FALSE;
	m_dwThreadTick[29] = 0;
	m_bThread[30] = FALSE;
	m_dwThreadTick[30] = 0;
	m_bThread[31] = FALSE;
	m_dwThreadTick[31] = 0;
	m_bThread[32] = FALSE;
	m_dwThreadTick[32] = 0;
	m_bThread[33] = FALSE;
	m_dwThreadTick[33] = 0;
	m_bThread[34] = FALSE;
	m_dwThreadTick[34] = 0;
	m_bThread[35] = FALSE;
	m_dwThreadTick[35] = 0;
	m_bThread[36] = FALSE;
	m_dwThreadTick[36] = 0;
	m_bThread[37] = FALSE;
	m_dwThreadTick[37] = 0;
	m_bThread[38] = FALSE;
	m_dwThreadTick[38] = 0;
}

void CManagerThread::Reset()
{
	m_bTHREAD_DISP_DEF = FALSE;
	m_nStepTHREAD_DISP_DEF = 0;
	m_bTHREAD_DISP_DEF_INNER = FALSE;
	m_nStepTHREAD_DISP_DEF_INNER = 0;

	m_bTHREAD_UPDATAE_YIELD[0] = FALSE;
	m_bTHREAD_UPDATAE_YIELD[1] = FALSE;
	m_nSerialTHREAD_UPDATAE_YIELD[0] = 0;
	m_nSerialTHREAD_UPDATAE_YIELD[1] = 0;

	m_bTHREAD_SHIFT2MK = FALSE;
	m_bTHREAD_UPDATE_REELMAP_UP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLUP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_DN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLDN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ITS = FALSE;
	m_bTHREAD_MAKE_ITS_FILE_UP = FALSE;
	m_bTHREAD_MAKE_ITS_FILE_DN = FALSE;

	m_bTHREAD_UPDATE_REELMAP_INNER_UP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_INNER_DN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE;

	m_nSnTHREAD_UPDATAE_YIELD = 0;

	m_bTHREAD_REELMAP_YIELD_UP = FALSE;
	m_bTHREAD_REELMAP_YIELD_ALLUP = FALSE;
	m_bTHREAD_REELMAP_YIELD_DN = FALSE;
	m_bTHREAD_REELMAP_YIELD_ALLDN = FALSE;
	m_bTHREAD_REELMAP_YIELD_ITS = FALSE;

	m_bTHREAD_UPDATE_YIELD_UP = FALSE;
	m_bTHREAD_UPDATE_YIELD_ALLUP = FALSE;
	m_bTHREAD_UPDATE_YIELD_DN = FALSE;
	m_bTHREAD_UPDATE_YIELD_ALLDN = FALSE;
	m_bTHREAD_UPDATE_YIELD_ITS = FALSE;

	m_bTHREAD_UPDATE_YIELD_INNER_UP = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_ALLUP = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_DN = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_ALLDN = FALSE;

	m_bTHREAD_RELOAD_RST_UP = FALSE;
	m_bTHREAD_RELOAD_RST_ALLUP = FALSE;
	m_bTHREAD_RELOAD_RST_DN = FALSE;
	m_bTHREAD_RELOAD_RST_ALLDN = FALSE;
	m_bTHREAD_RELOAD_RST_ITS = FALSE;
	m_bTHREAD_RELOAD_RST_UP_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_ALLUP_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_DN_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_ALLDN_INNER = FALSE;

	m_bTHREAD_MK[0] = FALSE;
	m_bTHREAD_MK[1] = FALSE;
	m_bTHREAD_MK[2] = FALSE;
	m_bTHREAD_MK[3] = FALSE;
}

BOOL CManagerThread::InitAct()
{
	// ChkCollision
	if (!m_bThread[1])
		m_Thread[1].Start(GetSafeHwnd(), this, ThreadProc1);

	// DispDefImg
	if (!m_bThread[2])
		m_Thread[2].Start(GetSafeHwnd(), this, ThreadProc2);

	// UpdateYield
	if (!m_bThread[3])
		m_Thread[3].Start(GetSafeHwnd(), this, ThreadProc3);

	// DoShift2Mk
	if (!m_bThread[4])
		m_Thread[4].Start(GetSafeHwnd(), this, ThreadProc4);

	// Engrave Auto Sequence - Response Check
	if (!m_bThread[5])
		m_Thread[5].Start(GetSafeHwnd(), this, ThreadProc5);

	// UpdataeReelmapUp
	if (!m_bThread[6])
		m_Thread[6].Start(GetSafeHwnd(), this, ThreadProc6);

	// UpdataeReelmapAllUp
	if (!m_bThread[7])
		m_Thread[7].Start(GetSafeHwnd(), this, ThreadProc7);

	// UpdataeReelmapDn
	if (!m_bThread[8])
		m_Thread[8].Start(GetSafeHwnd(), this, ThreadProc8);

	// UpdataeReelmapAllDn
	if (!m_bThread[9])
		m_Thread[9].Start(GetSafeHwnd(), this, ThreadProc9);

	// UpdataeRstUp
	if (!m_bThread[10])
		m_Thread[10].Start(GetSafeHwnd(), this, ThreadProc10);

	// UpdataeRstDn
	if (!m_bThread[11])
		m_Thread[11].Start(GetSafeHwnd(), this, ThreadProc11);

	// UpdataeRstAllUp
	if (!m_bThread[12])
		m_Thread[12].Start(GetSafeHwnd(), this, ThreadProc12);

	// UpdataeRstAllDn
	if (!m_bThread[13])
		m_Thread[13].Start(GetSafeHwnd(), this, ThreadProc13);

	// UpdataeRstUp
	if (!m_bThread[14])
		m_Thread[14].Start(GetSafeHwnd(), this, ThreadProc14);

	// UpdataeRstDn
	if (!m_bThread[15])
		m_Thread[15].Start(GetSafeHwnd(), this, ThreadProc15);

	// UpdataeRstAllUp
	if (!m_bThread[16])
		m_Thread[16].Start(GetSafeHwnd(), this, ThreadProc16);

	// UpdataeRstAllDn
	if (!m_bThread[17])
		m_Thread[17].Start(GetSafeHwnd(), this, ThreadProc17);

	// UpdataeReelmapITS
	if (!m_bThread[18])
		m_Thread[18].Start(GetSafeHwnd(), this, ThreadProc18);

	// DispDefImgInner
	if (!m_bThread[19])
		m_Thread[19].Start(GetSafeHwnd(), this, ThreadProc19);

	// UpdateRstIts
	if (!m_bThread[20])
		m_Thread[20].Start(GetSafeHwnd(), this, ThreadProc20);

	// ReloadReelmapUpInner
	if (!m_bThread[21])
		m_Thread[21].Start(GetSafeHwnd(), this, ThreadProc21);

	// ReloadReelmapDnInner
	if (!m_bThread[22])
		m_Thread[22].Start(GetSafeHwnd(), this, ThreadProc22);

	// ReloadReelmapAllUpInner
	if (!m_bThread[23])
		m_Thread[23].Start(GetSafeHwnd(), this, ThreadProc23);

	// ReloadReelmapAllUpInner
	if (!m_bThread[24])
		m_Thread[24].Start(GetSafeHwnd(), this, ThreadProc24);

	// ReloadReelmapIts
	if (!m_bThread[25])
		m_Thread[25].Start(GetSafeHwnd(), this, ThreadProc25);

	// UpdataeReelmapUp
	if (!m_bThread[26])
		m_Thread[26].Start(GetSafeHwnd(), this, ThreadProc26);

	// UpdataeReelmapAllUp
	if (!m_bThread[27])
		m_Thread[27].Start(GetSafeHwnd(), this, ThreadProc27);

	// UpdataeReelmapDn
	if (!m_bThread[28])
		m_Thread[28].Start(GetSafeHwnd(), this, ThreadProc28);

	// UpdataeReelmapAllDn
	if (!m_bThread[29])
		m_Thread[29].Start(GetSafeHwnd(), this, ThreadProc29);

	// UpdataeYieldUp
	if (!m_bThread[30])
		m_Thread[30].Start(GetSafeHwnd(), this, ThreadProc30);

	// UpdataeYieldDn
	if (!m_bThread[31])
		m_Thread[31].Start(GetSafeHwnd(), this, ThreadProc31);

	// UpdataeYieldAllUp
	if (!m_bThread[32])
		m_Thread[32].Start(GetSafeHwnd(), this, ThreadProc32);

	// UpdataeYieldAllDn
	if (!m_bThread[33])
		m_Thread[33].Start(GetSafeHwnd(), this, ThreadProc33);

	// UpdataeYieldInnerUp
	if (!m_bThread[34])
		m_Thread[34].Start(GetSafeHwnd(), this, ThreadProc34);

	// UpdataeYieldInnerDn
	if (!m_bThread[35])
		m_Thread[35].Start(GetSafeHwnd(), this, ThreadProc35);

	// UpdataeYieldInnerAllUp
	if (!m_bThread[36])
		m_Thread[36].Start(GetSafeHwnd(), this, ThreadProc36);

	// UpdataeYieldInnerAllDn
	if (!m_bThread[37])
		m_Thread[37].Start(GetSafeHwnd(), this, ThreadProc37);

	// UpdataeYieldIts
	if (!m_bThread[38])
		m_Thread[38].Start(GetSafeHwnd(), this, ThreadProc38);

	return TRUE;
}

void CManagerThread::ThreadKill()
{
	if (m_bThread[0])
	{
		m_Thread[0].Stop();
		Sleep(20);
		while (m_bThread[0])
		{
			Sleep(20);
		}
	}
	if (m_bThread[1])
	{
		m_Thread[1].Stop();
		Sleep(20);
		while (m_bThread[1])
		{
			Sleep(20);
		}
	}
	if (m_bThread[2])
	{
		m_Thread[2].Stop();
		Sleep(20);
		while (m_bThread[2])
		{
			Sleep(20);
		}
	}
	if (m_bThread[3])
	{
		m_Thread[3].Stop();
		Sleep(20);
		while (m_bThread[3])
		{
			Sleep(20);
		}
	}
	if (m_bThread[4])
	{
		m_Thread[4].Stop();
		Sleep(20);
		while (m_bThread[4])
		{
			Sleep(20);
		}
	}
	if (m_bThread[5])
	{
		CWinThread* pPointer = m_Thread[5].GetPointer();
		m_Thread[5].Stop();
		Sleep(20);
		//while (m_bThread[5])
		//{
		//	Sleep(20);
		//}
	}
	if (m_bThread[6])
	{
		m_Thread[6].Stop();
		Sleep(20);
		while (m_bThread[6])
		{
			Sleep(20);
		}
	}
	if (m_bThread[7])
	{
		m_Thread[7].Stop();
		Sleep(20);
		while (m_bThread[7])
		{
			Sleep(20);
		}
	}
	if (m_bThread[8])
	{
		m_Thread[8].Stop();
		Sleep(20);
		while (m_bThread[8])
		{
			Sleep(20);
		}
	}
	if (m_bThread[9])
	{
		m_Thread[9].Stop();
		Sleep(20);
		while (m_bThread[9])
		{
			Sleep(20);
		}
	}
	if (m_bThread[10])
	{
		m_Thread[10].Stop();
		Sleep(20);
		while (m_bThread[10])
		{
			Sleep(20);
		}
	}
	if (m_bThread[11])
	{
		m_Thread[11].Stop();
		Sleep(20);
		while (m_bThread[11])
		{
			Sleep(20);
		}
	}
	if (m_bThread[12])
	{
		m_Thread[12].Stop();
		Sleep(20);
		while (m_bThread[12])
		{
			Sleep(20);
		}
	}
	if (m_bThread[13])
	{
		m_Thread[13].Stop();
		Sleep(20);
		while (m_bThread[13])
		{
			Sleep(20);
		}
	}
	if (m_bThread[14])
	{
		m_Thread[14].Stop();
		Sleep(20);
		while (m_bThread[14])
		{
			Sleep(20);
		}
	}
	if (m_bThread[15])
	{
		m_Thread[15].Stop();
		Sleep(20);
		while (m_bThread[15])
		{
			Sleep(20);
		}
	}
	if (m_bThread[16])
	{
		m_Thread[16].Stop();
		Sleep(20);
		while (m_bThread[16])
		{
			Sleep(20);
		}
	}
	if (m_bThread[17])
	{
		m_Thread[17].Stop();
		Sleep(20);
		while (m_bThread[17])
		{
			Sleep(20);
		}
	}
	if (m_bThread[18])
	{
		m_Thread[18].Stop();
		Sleep(20);
		while (m_bThread[18])
		{
			Sleep(20);
		}
	}
	if (m_bThread[19])
	{
		m_Thread[19].Stop();
		Sleep(20);
		while (m_bThread[19])
		{
			Sleep(20);
		}
	}
	if (m_bThread[20])
	{
		m_Thread[20].Stop();
		Sleep(20);
		while (m_bThread[20])
		{
			Sleep(20);
		}
	}
	if (m_bThread[21])
	{
		m_Thread[21].Stop();
		Sleep(20);
		while (m_bThread[21])
		{
			Sleep(20);
		}
	}
	if (m_bThread[22])
	{
		m_Thread[22].Stop();
		Sleep(20);
		while (m_bThread[22])
		{
			Sleep(20);
		}
	}
	if (m_bThread[23])
	{
		m_Thread[23].Stop();
		Sleep(20);
		while (m_bThread[23])
		{
			Sleep(20);
		}
	}
	if (m_bThread[24])
	{
		m_Thread[24].Stop();
		Sleep(20);
		while (m_bThread[24])
		{
			Sleep(20);
		}
	}
	if (m_bThread[25])
	{
		m_Thread[25].Stop();
		Sleep(20);
		while (m_bThread[25])
		{
			Sleep(20);
		}
	}
	if (m_bThread[26])
	{
		m_Thread[26].Stop();
		Sleep(20);
		while (m_bThread[26])
		{
			Sleep(20);
		}
	}
	if (m_bThread[27])
	{
		m_Thread[27].Stop();
		Sleep(20);
		while (m_bThread[27])
		{
			Sleep(20);
		}
	}
	if (m_bThread[28])
	{
		m_Thread[28].Stop();
		Sleep(20);
		while (m_bThread[28])
		{
			Sleep(20);
		}
	}
	if (m_bThread[29])
	{
		m_Thread[29].Stop();
		Sleep(20);
		while (m_bThread[29])
		{
			Sleep(20);
		}
	}
	if (m_bThread[30])
	{
		m_Thread[30].Stop();
		Sleep(20);
		while (m_bThread[30])
		{
			Sleep(20);
		}
	}
	if (m_bThread[31])
	{
		m_Thread[31].Stop();
		Sleep(20);
		while (m_bThread[31])
		{
			Sleep(20);
		}
	}
	if (m_bThread[32])
	{
		m_Thread[32].Stop();
		Sleep(20);
		while (m_bThread[32])
		{
			Sleep(20);
		}
	}
	if (m_bThread[33])
	{
		m_Thread[33].Stop();
		Sleep(20);
		while (m_bThread[33])
		{
			Sleep(20);
		}
	}
	if (m_bThread[34])
	{
		m_Thread[34].Stop();
		Sleep(20);
		while (m_bThread[34])
		{
			Sleep(20);
		}
	}
	if (m_bThread[35])
	{
		m_Thread[35].Stop();
		Sleep(20);
		while (m_bThread[35])
		{
			Sleep(20);
		}
	}
	if (m_bThread[36])
	{
		m_Thread[36].Stop();
		Sleep(20);
		while (m_bThread[36])
		{
			Sleep(20);
		}
	}
	if (m_bThread[37])
	{
		m_Thread[37].Stop();
		Sleep(20);
		while (m_bThread[37])
		{
			Sleep(20);
		}
	}
	if (m_bThread[38])
	{
		m_Thread[38].Stop();
		Sleep(20);
		while (m_bThread[38])
		{
			Sleep(20);
		}
	}
}

UINT CManagerThread::ThreadProc0(LPVOID lpContext)	// DoMark0(), DoMark1()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);
	stMasterInfo& MasterInfo = pView->m_mgrReelmap->m_Master[0].MasterInfo;

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[0] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[0].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[0] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;
#ifndef TEST_MODE
			if (MasterInfo.nActionCode == 1 || MasterInfo.nActionCode == 3)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
			{
				if (pThread->m_bTHREAD_MK[0])
				{
					if (pThread->GetBufUpSerial(0) > 0)
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
							pThread->DoMark0Its();
						else
							pThread->DoMark0();
					}
					else
					{
						pThread->SetDoneMk(0);
						pThread->m_bTHREAD_MK[0] = FALSE;
					}
				}
				if (pThread->m_bTHREAD_MK[1])
				{
					if (pThread->GetBufUpSerial(1) > 0)
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
							pThread->DoMark1Its();
						else
							pThread->DoMark1();
					}
					else
					{
						pThread->SetDoneMk(1);
						pThread->m_bTHREAD_MK[1] = FALSE;
					}
				}
			}
			else
			{
				if (pThread->m_bTHREAD_MK[1])
				{
					if (pThread->GetBufUpSerial(1) > 0)
						pThread->DoMark1();
					else
					{
						pThread->SetDoneMk(1);
						pThread->m_bTHREAD_MK[1] = FALSE;
					}
				}
				if (pThread->m_bTHREAD_MK[0])
				{
					if (pThread->GetBufUpSerial(0) > 0)
						pThread->DoMark0();
					else
					{
						pThread->SetDoneMk(0);
						pThread->m_bTHREAD_MK[0] = FALSE;
					}
				}
			}
#else
			pThread->DoMark1();
			pThread->DoMark0();
			Sleep(100);
#endif
			if (pThread->m_bTHREAD_MK[3])
				pThread->DoReject1();
			if (pThread->m_bTHREAD_MK[2])
				pThread->DoReject0();

			if (!pThread->m_bTHREAD_MK[0] && !pThread->m_bTHREAD_MK[1] &&
				!pThread->m_bTHREAD_MK[2] && !pThread->m_bTHREAD_MK[3])
				Sleep(50);
			bLock = FALSE;
		}
		Sleep(10);
	}
	pThread->m_bThread[0] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc1(LPVOID lpContext)	// ChkCollision()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE, bEStop = FALSE, bCollision = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[1] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[1].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[1] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;

			pThread->GetEnc();

			if (!pThread->m_bTHREAD_MK[0] && !pThread->m_bTHREAD_MK[1] &&
				!pThread->m_bTHREAD_MK[2] && !pThread->m_bTHREAD_MK[3])
			{
				if (pThread->ChkCollision() && !bEStop)
				{
					if (pThread->IsRunAxisX())
					{
						bEStop = TRUE;
						pThread->EStop();
					}
				}
				else if (!pThread->ChkCollision() && bEStop)
				{
					bEStop = FALSE;
				}
			}
			else
			{
				if (pThread->GetCollision(0) && pThread->GetCollision(1))
				{
					if (!bCollision)
					{
						bCollision = TRUE;
						pThread->SetPriority();
					}
				}
				else
					bCollision = FALSE;
			}

			bLock = FALSE;
		}
		Sleep(10);
	}
	pThread->m_bThread[1] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc2(LPVOID lpContext)	// DispDefImg()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[2] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[2].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[2] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;

			if (pThread->m_bTHREAD_DISP_DEF)
			{
				pThread->DispDefImg();
				Sleep(0);
			}
			else
				Sleep(30);


			bLock = FALSE;
		}
	}
	pThread->m_bThread[2] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc3(LPVOID lpContext)	// UpdateYield()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[3] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[3].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[3] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATAE_YIELD[0]) // Left Punching Yield
		{
			pThread->UpdateYield(pThread->m_nSerialTHREAD_UPDATAE_YIELD[0]);
			pThread->m_bTHREAD_UPDATAE_YIELD[0] = FALSE;
			Sleep(0);
		}
		else if (pThread->m_bTHREAD_UPDATAE_YIELD[1] && !pThread->m_bTHREAD_UPDATE_YIELD_UP) // Right Punching Yield
		{
			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (!pThread->m_bTHREAD_UPDATE_YIELD_DN && !pThread->m_bTHREAD_UPDATE_YIELD_ALLUP && !pThread->m_bTHREAD_UPDATE_YIELD_ALLDN)
				{
					pThread->UpdateYield(pThread->m_nSerialTHREAD_UPDATAE_YIELD[1]);
					pThread->m_bTHREAD_UPDATAE_YIELD[1] = FALSE;
					Sleep(0);
				}
			}
			else
			{
				pThread->m_bTHREAD_UPDATAE_YIELD[1] = FALSE;
				pThread->UpdateYield(pThread->m_nSerialTHREAD_UPDATAE_YIELD[1]);
				Sleep(0);
			}
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[3] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc4(LPVOID lpContext)	// RunShift2Mk()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[4] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[4].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[4] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_SHIFT2MK)
		{
			if (!pThread->m_bTHREAD_UPDATAE_YIELD[0] && !pThread->m_bTHREAD_UPDATAE_YIELD[1])
			{
				pThread->RunShift2Mk();
				pThread->m_bTHREAD_SHIFT2MK = FALSE;
				Sleep(0);
			}
			else
				Sleep(30);

		}
		else
			Sleep(30);
	}

	pThread->m_bThread[4] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc5(LPVOID lpContext)	// GetCurrentInfoSignal()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[5] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[5].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[5] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;
			pThread->GetCurrentInfoSignal();
			bLock = FALSE;
		}

		Sleep(100);
	}
	pThread->m_bThread[5] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc6(LPVOID lpContext)	// UpdateRMapUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[6] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[6].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[6] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
		{
			if (!pThread->m_bTHREAD_REELMAP_YIELD_UP) // Yield Reelmap
			{
				pThread->UpdateRMapUp(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_UP = FALSE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[6] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc7(LPVOID lpContext)	// UpdateRMapDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[7] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[7].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[7] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_DN) // Write Reelmap
		{
			//pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP = TRUE;
			//pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN = TRUE;
			if (!pThread->m_bTHREAD_REELMAP_YIELD_DN) // Yield Reelmap
			{
				pThread->UpdateRMapDn(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_DN = FALSE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[7] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc8(LPVOID lpContext)	// UpdateRMapAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[8] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[8].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[8] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP) // Write Reelmap
		{
			if (!pThread->m_bTHREAD_REELMAP_YIELD_ALLUP) // Yield Reelmap
			{
				pThread->UpdateRMapAllUp(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP = FALSE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[8] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc9(LPVOID lpContext)	// UpdateRMapAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[9] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[9].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[9] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			if (!pThread->m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				pThread->UpdateRMapAllDn(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN = FALSE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[9] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc10(LPVOID lpContext)	// UpdateReelmapYieldUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[10] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[10].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[10] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_UP) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
			{
				pThread->UpdateReelmapYieldUp(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_UP = FALSE;
				if (!pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (pDoc->GetTestMode() == MODE_OUTER)
						pThread->m_bTHREAD_REELMAP_YIELD_ITS = TRUE;
				}

				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[10] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc11(LPVOID lpContext)	// UpdateReelmapYieldDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[11] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[11].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[11] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_DN) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_DN && !pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP && !pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
			{
				pThread->UpdateReelmapYieldDn(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_DN = FALSE;
				pThread->m_bTHREAD_REELMAP_YIELD_ALLUP = TRUE;
				pThread->m_bTHREAD_REELMAP_YIELD_ALLDN = TRUE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[11] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc12(LPVOID lpContext)	// UpdateReelmapYieldAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[12] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[12].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[12] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_ALLUP) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP) // Write Reelmap
			{
				pThread->UpdateReelmapYieldAllUp(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_ALLUP = FALSE;
				if (pDoc->GetTestMode() == MODE_OUTER)
					pThread->m_bTHREAD_REELMAP_YIELD_ITS = TRUE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[12] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc13(LPVOID lpContext)	// UpdateReelmapYieldAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[13] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[13].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[13] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
			{
				pThread->UpdateReelmapYieldAllDn(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_ALLDN = FALSE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[13] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc14(LPVOID lpContext)	// ReloadReelmapUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[14] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[14].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[14] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_UP)
		{
			pThread->ReloadReelmapUp();
			pThread->m_bTHREAD_RELOAD_RST_UP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[14] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc15(LPVOID lpContext)	// ReloadReelmapDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[15] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[15].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[15] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_DN)
		{
			pThread->ReloadReelmapDn();
			pThread->m_bTHREAD_RELOAD_RST_DN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[15] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc16(LPVOID lpContext)	// ReloadReelmapAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[16] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[16].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[16] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLUP)
		{
			pThread->ReloadReelmapAllUp();
			pThread->m_bTHREAD_RELOAD_RST_ALLUP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[16] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc17(LPVOID lpContext)	// ReloadReelmapAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[17] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[17].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[17] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLDN)
		{
			pThread->ReloadReelmapAllDn();
			pThread->m_bTHREAD_RELOAD_RST_ALLDN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[17] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc18(LPVOID lpContext) // WriteReelmapIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[18] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[18].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[18] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ITS) // Write Reelmap
		{
			pThread->WriteReelmapIts();
			pThread->m_bTHREAD_UPDATE_REELMAP_ITS = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[18] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc19(LPVOID lpContext) // DispDefImgInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[19] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[19].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[19] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;

			if (pThread->m_bTHREAD_DISP_DEF_INNER)
			{
				pThread->DispDefImgInner();
				Sleep(0);
			}
			else
				Sleep(30);


			bLock = FALSE;
		}
	}
	pThread->m_bThread[19] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc20(LPVOID lpContext)	// UpdateReelmapYieldIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[20] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[20].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[20] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
		{
			pThread->UpdateReelmapYieldIts(); // Yield Reelmap
			pThread->m_bTHREAD_REELMAP_YIELD_ITS = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[20] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc21(LPVOID lpContext)	// ReloadReelmapUpInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[21] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[21].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[21] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_UP_INNER)
		{
			pThread->ReloadReelmapUpInner();
			pThread->m_bTHREAD_RELOAD_RST_UP_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[21] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc22(LPVOID lpContext)	// ReloadReelmapDnInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[22] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[22].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[22] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_DN_INNER)
		{
			pThread->ReloadReelmapDnInner();
			pThread->m_bTHREAD_RELOAD_RST_DN_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[22] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc23(LPVOID lpContext)	// ReloadReelmapAllUpInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[23] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[23].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[23] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLUP_INNER)
		{
			pThread->ReloadReelmapAllUpInner();
			pThread->m_bTHREAD_RELOAD_RST_ALLUP_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[23] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc24(LPVOID lpContext)	// ReloadReelmapAllDnInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[24] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[24].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[24] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLDN_INNER)
		{
			pThread->ReloadReelmapAllDnInner();
			pThread->m_bTHREAD_RELOAD_RST_ALLDN_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[24] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc25(LPVOID lpContext)	// ReloadReelmapIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[25] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[25].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[25] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ITS)
		{
			pThread->ReloadReelmapIts();
			pThread->m_bTHREAD_RELOAD_RST_ITS = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[25] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc26(LPVOID lpContext)	// MakeItsFileUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[26] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[26].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[26] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_UP) // MakeReelmapInnerFile
		{
			pThread->UpdateRMapInnerUp();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_UP = FALSE;
			Sleep(0);
		}
		if (pThread->m_bTHREAD_MAKE_ITS_FILE_UP) // MakeItsFile
		{
			pThread->MakeItsFileUp(pThread->m_nSerialMakeItsFile);
			pThread->m_bTHREAD_MAKE_ITS_FILE_UP = FALSE;
			Sleep(0);
		}

		Sleep(30);
	}

	pThread->m_bThread[26] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc27(LPVOID lpContext)	// MakeItsFileDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[27] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[27].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[27] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_DN) // MakeReelmapInnerFile
		{
			pThread->UpdateRMapInnerDn();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_DN = FALSE;
			Sleep(0);
		}
		if (pThread->m_bTHREAD_MAKE_ITS_FILE_DN) // MakeItsFile
		{
			pThread->MakeItsFileDn(pThread->m_nSerialMakeItsFile);
			pThread->m_bTHREAD_MAKE_ITS_FILE_DN = FALSE;
			Sleep(0);
		}

		Sleep(30);
	}

	pThread->m_bThread[27] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc28(LPVOID lpContext)	// UpdateRMapAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[28] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[28].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[28] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP) // Write Reelmap
		{
			pThread->UpdateRMapInnerAllUp();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[28] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc29(LPVOID lpContext)	// UpdateRMapInnerAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[29] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[29].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[29] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
		{
			pThread->UpdateRMapInnerAllDn();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[29] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc30(LPVOID lpContext)	// UpdateYieldUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[30] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[30].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[30] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_UP)
		{
			pThread->UpdateYieldUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_UP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[30] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc31(LPVOID lpContext)	// UpdateYieldDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[31] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[31].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[31] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_DN)
		{
			pThread->UpdateYieldDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_DN = FALSE;
			pThread->m_bTHREAD_UPDATE_YIELD_ALLUP = TRUE;
			pThread->m_bTHREAD_UPDATE_YIELD_ALLDN = TRUE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[31] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc32(LPVOID lpContext)	// UpdateYieldAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[32] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[32].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[32] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_ALLUP && !pThread->m_bTHREAD_UPDATE_YIELD_UP)
		{
			pThread->UpdateYieldAllUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_ALLUP = FALSE;

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				pThread->m_bTHREAD_UPDATE_YIELD_ITS = TRUE;
				//pThread->m_bTHREAD_UPDATE_YIELD_INNER_UP = TRUE;
				//if (pDoc->WorkingInfo.LastJob.bDualTestInner)
				//{
				//	pThread->m_bTHREAD_UPDATE_YIELD_INNER_DN = TRUE;
				//}
			}

			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[32] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc33(LPVOID lpContext)	// UpdateYieldAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[33] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[33].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[33] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_ALLDN && !pThread->m_bTHREAD_UPDATE_YIELD_UP)
		{
			pThread->UpdateYieldAllDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_ALLDN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[33] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc34(LPVOID lpContext)	// UpdateYieldInnerUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[34] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[34].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[34] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_UP)
		{
			pThread->UpdateYieldInnerUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_UP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[34] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc35(LPVOID lpContext)	// UpdateYieldInnerDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[35] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[35].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[35] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_DN)
		{
			pThread->UpdateYieldInnerDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_DN = FALSE;
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLUP = TRUE;
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLDN = TRUE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[35] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc36(LPVOID lpContext)	// UpdateYieldInnerAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[36] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[36].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[36] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLUP)
		{
			pThread->UpdateYieldInnerAllUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLUP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[36] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc37(LPVOID lpContext)	// UpdateYieldInnerAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[37] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[37].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[37] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)
		{
			pThread->UpdateYieldInnerAllDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLDN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[37] = FALSE;

	return 0;
}

UINT CManagerThread::ThreadProc38(LPVOID lpContext)	// UpdateYieldIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerThread* pThread = reinterpret_cast<CManagerThread*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[38] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[38].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[38] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_ITS)
		{
			pThread->UpdateYieldIts(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_ITS = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[38] = FALSE;

	return 0;
}

void CManagerThread::GetEnc()
{
	pView->m_mgrPunch->GetEnc();
}

BOOL CManagerThread::ChkCollision()
{
	return pView->m_mgrPunch->ChkCollision();
}

BOOL CManagerThread::GetCollision(int nCam)
{
	return pView->m_mgrPunch->GetCollision(nCam);
}

void CManagerThread::SetPriority()
{
	pView->m_mgrPunch->SetPriority();
}

BOOL CManagerThread::IsRunAxisX()
{
	return pView->m_mgrPunch->IsRunAxisX();
}

void CManagerThread::EStop()
{
	pView->m_mgrPunch->EStop();
}

BOOL CManagerThread::GetDoneMk(int nCam)
{
	return pView->m_mgrPunch->GetDoneMk(nCam);
}

void CManagerThread::SetDoneMk(int nCam)
{
	return pView->m_mgrPunch->SetDoneMk(nCam);
}

int CManagerThread::GetBufUpSerial(int nCam)
{
	return pView->m_mgrProcedure->GetBufUpSerial(nCam);
}

int CManagerThread::GetBufDnSerial(int nCam)
{
	return pView->m_mgrProcedure->GetBufDnSerial(nCam);
}

void CManagerThread::DoMark0()
{
	return pView->m_mgrPunch->DoMark0();
}

void CManagerThread::DoMark1()
{
	return pView->m_mgrPunch->DoMark1();
}

void CManagerThread::DoMark0Its()
{
	return pView->m_mgrPunch->DoMark0Its();
}

void CManagerThread::DoMark1Its()
{
	return pView->m_mgrPunch->DoMark1Its();
}

void CManagerThread::DoReject0()
{
	return pView->m_mgrPunch->DoReject0();
}

void CManagerThread::DoReject1()
{
	return pView->m_mgrPunch->DoReject1();
}

void CManagerThread::UpdateYield(int nSerial)
{
	pView->m_mgrReelmap->UpdateYield(nSerial);
}

void CManagerThread::UpdateYieldUp(int nSerial)
{
	pView->m_mgrReelmap->UpdateYieldUp();
}

void CManagerThread::UpdateYieldDn(int nSerial)
{
	pView->m_mgrReelmap->UpdateYieldDn();
}

void CManagerThread::UpdateYieldAllUp(int nSerial)
{
	pView->m_mgrReelmap->UpdateYieldAllUp();
}

void CManagerThread::UpdateYieldAllDn(int nSerial)
{
	pView->m_mgrReelmap->UpdateYieldAllDn();
}

void CManagerThread::UpdateRMapInnerUp()
{
	pView->m_mgrReelmap->UpdateRMapInnerUp();
}

void CManagerThread::UpdateRMapInnerDn()
{
	pView->m_mgrReelmap->UpdateRMapInnerDn();
}

void CManagerThread::UpdateRMapInnerAllUp()
{
	pView->m_mgrReelmap->UpdateRMapInnerAllUp();
}

void CManagerThread::UpdateRMapInnerAllDn()
{
	pView->m_mgrReelmap->UpdateRMapInnerAllDn();
}

BOOL CManagerThread::GetCurrentInfoSignal()
{
	stBtnStatus& BtnStatus = (pView->m_mgrProcedure->m_pEngrave->BtnStatus);

	BtnStatus.EngAuto.IsInit = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoInit);
	BtnStatus.EngAuto.IsMkSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt);
	BtnStatus.EngAuto.IsOnMking = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng);
	BtnStatus.EngAuto.IsMkDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone);
	BtnStatus.EngAuto.IsRead2dSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt);
	BtnStatus.EngAuto.IsOnRead2d = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d);
	BtnStatus.EngAuto.IsRead2dDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone);
	BtnStatus.EngAuto.IsFdDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone);

	return TRUE;
}

void CManagerThread::DispDefImg()
{
	pView->m_mgrProcedure->DispDefImg();
}
