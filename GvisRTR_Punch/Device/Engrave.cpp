#include "../stdafx.h"
#include "Engrave.h"

#include "EngraveDef.h"
#include "../Global/GlobalDefine.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

BEGIN_MESSAGE_MAP(CEngrave, CWnd)
	//{{AFX_MSG_MAP(CEngrave)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_UPDATE_CONNECTION, OnUpdateConnection)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


CEngrave::CEngrave(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent /*=NULL*/)
{
	if(pParent)
		m_hParentWnd = pParent->GetSafeHwnd();

	m_pClient = NULL;
	m_nServerID = ID_ENGRAVE;
	m_bWaitForResponse = FALSE;
	m_strResponse = _T("");
	m_nCmd = _None_;
	m_bCmdF = FALSE;
	m_bAutoConnect = TRUE;

	RECT rt = { 0,0,0,0 };
	Create(NULL, _T("None"), WS_CHILD, rt, FromHandle(m_hParentWnd), (UINT)this);

	StartClient(sAddrCli, sAddrSvr, sPortSvr);
	Sleep(10);

	m_pThread = NULL;
	m_bTIM_CHECK_CONNECT = FALSE;
	m_bTIM_MPE_OFFSET_INITPOS_MOVE = FALSE;

	m_bGetOpInfo = FALSE; m_bGetInfo = FALSE; m_bGetEngInfo = FALSE;
	m_bGetSignalMain = FALSE; m_bGetSignalTorqueMotor = FALSE; m_bGetSignalInductionMotor = FALSE; m_bGetSignalCore150mm = FALSE; m_bGetSignalEtc = FALSE;
	m_bGetSignalRecoiler = FALSE; m_bGetSignalPunch = FALSE; m_bGetSignalAOIDn = FALSE; m_bGetSignalAOIUp = FALSE; m_bGetSignalEngrave = FALSE; m_bGetSignalUncoiler = FALSE;
	m_bGetSignalEngraveAutoSequence = FALSE;
	m_bGetTotRatio = FALSE; m_bGetStTime = FALSE; m_bGetRunTime = FALSE; m_bGetEdTime = FALSE; m_bGetStripRatio = FALSE; m_bGetDef = FALSE;
	m_bGet2DReader = FALSE; m_bGetEngInfo = FALSE; m_bGetFdInfo = FALSE; m_bGetAoiInfo = FALSE; m_bGetMkInfo = FALSE; m_bGetMkInfoLf = FALSE; m_bGetMkInfoRt = FALSE;

}


CEngrave::~CEngrave()
{
	m_bTIM_CHECK_CONNECT = FALSE;
	m_bTIM_MPE_OFFSET_INITPOS_MOVE = FALSE;

	if (m_pClient)
	{
		m_bAutoConnect = FALSE;
		StopClient();
	}
}


///////////////////////////////////////////////////////////////////////////////
// OnUpdateConnection
// This message is sent by server manager to indicate connection status
LRESULT CEngrave::OnUpdateConnection(WPARAM wParam, LPARAM lParam)
{
	UINT uEvent = (UINT)wParam;
	CEngrave* pClient = reinterpret_cast<CEngrave*>(lParam);

	if (pClient != NULL)
	{
		// Server socket is now connected, we need to pick a new one
		if (uEvent == EVT_CONSUCCESS)
		{
			m_bWaitForResponse = FALSE;
		}
		else if (uEvent == EVT_CONFAILURE || uEvent == EVT_CONDROP)
		{
			// Disconnect socket
			StopClient();
			// 자동접속시도
			if (uEvent == EVT_CONDROP && m_bAutoConnect)
				SetTimer(TIM_CONNECT, 9000, NULL);
		}
	}

	return 1L;
}

void CEngrave::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CEngrave::StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
{
	if (!m_pClient)
	{
		m_strAddrCli = sAddrCli;
		m_strAddrSvr = sAddrSvr;
		m_strPortSvr = sPortSvr;

		m_pClient = new CTcpIpClient(this);
		m_pClient->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
		m_pClient->SetServer(m_nServerID);
		m_pClient->Start();
	}
}

void CEngrave::StopClient()
{
	if (m_pClient)
	{
		m_pClient->Stop();
		Sleep(10);
		m_pClient->StopThread();
		Sleep(10);
		delete m_pClient;
		m_pClient = NULL;
		Sleep(10);
	}
}

LRESULT CEngrave::wmClientClosed(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	switch (nServerID)
	{
	case ID_ENGRAVE:
		if (m_pClient)
			m_pClient = NULL;
		break;
	}

	return (LRESULT)1;
}

SOCKET_DATA CEngrave::GetSocketData()
{
	return m_SocketData;
}

LRESULT CEngrave::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	if (nServerID == ID_ENGRAVE)
	{
		m_SocketData = m_pClient->GetSocketData();
	}


	return (LRESULT)OnClientReceived(wParam, lParam);
}

int CEngrave::OnClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;
	if (m_hParentWnd)
		::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)ID_ENGRAVE, (LPARAM)&m_SocketData); // "OrderNum-ShotNum" (9bytes'-'3bytes)
	m_bWaitForResponse = FALSE;

	return 1;
}

BOOL CEngrave::CheckResponse(int nCmd, CString sResponse)
{
	int nPos;
	CString sParsing;

	switch (nCmd)
	{
		;
	}

	return FALSE;
}

BOOL CEngrave::IsRunning()
{
	return (m_bWaitForResponse);
}

BOOL CEngrave::ReadComm(CString &sData)
{
	if (!m_bWaitForResponse)
	{
		sData = m_strResponse;
		return TRUE;
	}

	return FALSE;
}

int CEngrave::WriteComm(CString sMsg, DWORD dwTimeout)
{
	return m_pClient->WriteComm(sMsg);
}

// Thread
void CEngrave::StartThread() // Worker Thread 구동관련 Step8
{
	if (m_pThread == NULL)
	{
		m_bModify = TRUE;
		m_evtThread.ResetEvent();
		m_pThread = AfxBeginThread(RunThread, this);
		if (m_pThread)
			m_hThread = m_pThread->m_hThread;
	}
}

void CEngrave::StopThread() // Worker Thread 구동관련 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	m_pThread = NULL;
	m_bModify = FALSE;
}

void CEngrave::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
{
	TRACE("WaitUntilThreadEnd(0x%08x:RunThread) Entering\n", hThread);
	MSG message;
	const DWORD dwTimeOut = 500000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode;
	while (GetExitCodeThread(hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE && m_bAlive) {
		// Time Out Check
		if (GetTickCount() >= (dwStartTick + dwTimeOut))
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!", NULL, MB_OK | MB_ICONSTOP));
			return;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	TRACE("WaitUntilThreadEnd(0x%08x:DispMsgThread) Exit\n", hThread);
}

UINT CEngrave::RunThread(LPVOID pParam)
{
	CEngrave* pMk = (CEngrave*)pParam;
	DWORD dwMilliseconds = 10;
	pMk->m_bAlive = TRUE;

	MSG message;
	CString sMsg;

	while (WAIT_OBJECT_0 != ::WaitForSingleObject(pMk->m_evtThread, dwMilliseconds))
	{
		Sleep(100);
	}

	pMk->m_bAlive = FALSE;
	return 0;
}

void CEngrave::Close()
{
	StopThread();
	Sleep(10);
}

// General Function
BOOL CEngrave::SendCommand(SOCKET_DATA SocketData, BOOL bWait)
{
	if (!m_pClient)
		return FALSE;

	BOOL bRtn = TRUE;
	SocketData.nTxPC = _Punch;		// Client
	SocketData.nRxPC = _Engrave;	// Server
	m_pClient->WriteCommData(SocketData, INFINITE);

	if (bWait)
	{
		m_bWaitForResponse = TRUE;
		bRtn = WaitResponse();
	}

	return bRtn;
}

BOOL CEngrave::WaitResponse()
{
	MSG message;
	DWORD dwStartTick = GetTickCount();

	while (IsRunning())
	{
		if (GetTickCount() - dwStartTick > DELAY_RESPONSE)
		{
			// Disconnect socket
			m_pClient->SetConnectedStatus(FALSE);
			return FALSE;
		}

		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(30);
	}

	return TRUE;
}

BOOL CEngrave::IsConnected()
{
	if (!m_pClient)
		return FALSE;

	return m_pClient->IsConnected();
}

void CEngrave::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SOCKET_DATA SocketData;

	switch (nIDEvent)
	{
	case TIM_CONNECT:
		KillTimer(TIM_CONNECT);
		SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	case TIM_CONNECT + 1:
		KillTimer(TIM_CONNECT + 1);
#ifndef TEST_MODE
		StartClient(m_strAddrCli, m_strAddrSvr, m_strPortSvr);
#endif
		SetTimer(TIM_CONNECT + 2, 1000, NULL);
		break;
	case TIM_CONNECT + 2:
		KillTimer(TIM_CONNECT + 2);
		if(!IsConnected())
			SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	case TIM_CHECK_CONNECT:
		KillTimer(TIM_CHECK_CONNECT);
		if (m_pClient && m_bTIM_CHECK_CONNECT)
		{
			if(!IsRunning())
			{
				SocketData.nCmdCode = _GetSig;
				SocketData.nMsgID = _SigInx::_Connect;
				SendCommand(SocketData);
			}
			if (m_bTIM_CHECK_CONNECT)
				SetTimer(TIM_CHECK_CONNECT, DELAY_CHECK_CONNECT, NULL);
		}
		break;
	case TIM_MPE_OFFSET_INITPOS_MOVE:
		KillTimer(TIM_MPE_OFFSET_INITPOS_MOVE);
		if (m_bTIM_MPE_OFFSET_INITPOS_MOVE)
		{
#ifdef USE_MPE
			if (pView->m_mgrFeeding->GetMpeSignal(3, 15))
			{
				SetTimer(TIM_MPE_OFFSET_INITPOS_MOVE, 100, NULL);
			}
			else
			{
#ifdef USE_ENGRAVE
				Set2DOffsetInitPosMove(FALSE);	//_stSigInx::_2DOffsetInitPosMove
#endif
				m_bTIM_MPE_OFFSET_INITPOS_MOVE = FALSE;
			}
#endif
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

// Communcation

void CEngrave::GetSysSignal(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetEngInfo(SockData);

	GetSignalDisp(SockData);
	GetSignalMain(SockData);
	GetSignalTorqueMotor(SockData);
	GetSignalInductionMotor(SockData);
	GetSignalCore150mm(SockData);
	GetSignalEtc(SockData);
	GetSignalRecoiler(SockData);
	GetSignalPunch(SockData);
	GetSignalAOIDn(SockData);
	GetSignalAOIUp(SockData);
	GetSignalEngrave(SockData);
	GetSignalUncoiler(SockData);

	GetSignalEngraveAutoSequence(SockData);
	GetSignalMyMsg(SockData);

	GetSignal2dEng(SockData);
}

void CEngrave::GetSignalDisp(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_DispReady:
			BtnStatus.Disp.Init();
			BtnStatus.Disp.Ready = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispRun:
			BtnStatus.Disp.Init();
			BtnStatus.Disp.Run = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispStop:
			BtnStatus.Disp.Init();
			BtnStatus.Disp.Stop = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispDualSample:
			BtnStatus.Disp.Init();
			BtnStatus.Disp.DualSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispSingleSample:
			BtnStatus.Disp.Init();
			BtnStatus.Disp.SingleSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispDualTest:
			BtnStatus.Disp.Init();
			BtnStatus.Disp.DualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DispSingleTest:
			BtnStatus.Disp.Init();
			BtnStatus.Disp.SingleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		// Is
		case _SigInx::_IsDispReady:
			BtnStatus.Disp.IsReady = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispRun:
			BtnStatus.Disp.IsRun = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispStop:
			BtnStatus.Disp.IsStop = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispDualSample:
			BtnStatus.Disp.IsDualSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispSingleSample:
			BtnStatus.Disp.IsSingleSample = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispDualTest:
			BtnStatus.Disp.IsDualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDispSingleTest:
			BtnStatus.Disp.IsSingleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}


void CEngrave::GetSignalMain(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Ready:
			BtnStatus.Main.Ready = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_Ready] = TRUE;
			break;
		case _SigInx::_Run:
			BtnStatus.Main.Run = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.Main.Stop = (SockData.nData1 > 0) ? FALSE : BtnStatus.Main.Stop;
			m_bRcvSig[_SigInx::_Run] = TRUE;
			break;
		case _SigInx::_Reset:
			BtnStatus.Main.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_Reset] = TRUE;
			break;
		case _SigInx::_Stop:
			BtnStatus.Main.Stop = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.Main.Run = (SockData.nData1 > 0) ? FALSE : BtnStatus.Main.Run;
			m_bRcvSig[_SigInx::_Stop] = TRUE;
			break;
		case _SigInx::_Auto:
			BtnStatus.Main.Auto = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.Main.Manual = (SockData.nData1 > 0) ? FALSE : BtnStatus.Main.Manual;
			break;
		case _SigInx::_Manual:
			BtnStatus.Main.Manual = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.Main.Auto = (SockData.nData1 > 0) ? FALSE : BtnStatus.Main.Auto;
			break;
			// Is
		case _SigInx::_IsReady:
			BtnStatus.Main.IsReady = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsRun:
			BtnStatus.Main.IsRun = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsReset:
			BtnStatus.Main.IsReset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsStop:
			BtnStatus.Main.IsStop = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsAuto:			
			pView->m_mgrFeeding->Status.bAuto = BtnStatus.Main.IsAuto = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsManual:
			pView->m_mgrFeeding->Status.bManual = BtnStatus.Main.IsManual = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalTorqueMotor(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_MkTq:
			BtnStatus.Tq.Mk = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_Stop] = TRUE;
			break;
		case _SigInx::_AoiTq:
			BtnStatus.Tq.Aoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_AoiTq] = TRUE;
			break;
		case _SigInx::_EngTq:
			BtnStatus.Tq.Eng = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_EngTq] = TRUE;
			break;
			// Is
		case _SigInx::_IsMkTq:
			BtnStatus.Tq.IsMk = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsAoiTq:
			BtnStatus.Tq.IsAoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngTq:
			BtnStatus.Tq.IsEng = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalInductionMotor(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_CcwModRe:
			BtnStatus.Induct.Rc = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_CcwModRe] = TRUE;
			break;
		case _SigInx::_CcwModUn:
			BtnStatus.Induct.Uc = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_CcwModUn] = TRUE;
			break;
			// Is
		case _SigInx::_IsCcwModRe:
			BtnStatus.Induct.IsRc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsCcwModUn:
			BtnStatus.Induct.IsUc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalCore150mm(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Core150Re:
			BtnStatus.Core150.Rc = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_Core150Re] = TRUE;
			break;
		case _SigInx::_Core150Un:
			BtnStatus.Core150.Uc = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_Core150Un] = TRUE;
			break;
			// Is
		case _SigInx::_IsCore150Re:
			BtnStatus.Core150.IsRc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsCore150Un:
			BtnStatus.Core150.IsUc = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalEtc(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EmgAoi:
			BtnStatus.Etc.EmgAoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
			// Is
		case _SigInx::_IsEmgAoi:
			BtnStatus.Etc.IsEmgAoi = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalRecoiler(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwRe:
			BtnStatus.Rc.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_MvCwRe] = TRUE;
			break;
		case _SigInx::_MvCcwRe:
			BtnStatus.Rc.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_MvCcwRe] = TRUE;
			break;
		case _SigInx::_PrdChuckRe:
			BtnStatus.Rc.ReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PrdChuckRe] = TRUE;
			break;
		case _SigInx::_DancerUpRe:
			BtnStatus.Rc.DcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_DancerUpRe] = TRUE;
			break;
		case _SigInx::_PasteUpLfRe:
			BtnStatus.Rc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PasteUpLfRe] = TRUE;
			break;
		case _SigInx::_PasteUpRtRe:
			BtnStatus.Rc.ReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PasteUpRtRe] = TRUE;
			break;
		case _SigInx::_PasteVacRe:
			BtnStatus.Rc.ReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PasteVacRe] = TRUE;
			break;
		case _SigInx::_PprChuckRe:
			BtnStatus.Rc.PprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PprChuckRe] = TRUE;
			break;
		case _SigInx::_PprCwRe:
			BtnStatus.Rc.PprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PprCwRe] = TRUE;
			break;
		case _SigInx::_PprCcwRe:
			BtnStatus.Rc.PprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PprCcwRe] = TRUE;
			break;
		case _SigInx::_DoRe:
			BtnStatus.Rc.Rewine = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_DoRe] = TRUE;
			break;
		case _SigInx::_PrdPprRe:
			BtnStatus.Rc.RewineReelPpr = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PrdPprRe] = TRUE;
			break;
			// Is
		case _SigInx::_IsRelation:
			BtnStatus.Rc.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwRe:
			BtnStatus.Rc.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwRe:
			BtnStatus.Rc.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPrdChuckRe:
			BtnStatus.Rc.IsReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpRe:
			BtnStatus.Rc.IsDcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpLfRe:
			BtnStatus.Rc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpRtRe:
			BtnStatus.Rc.IsReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteVacRe:
			BtnStatus.Rc.IsReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprChuckRe:
			BtnStatus.Rc.IsPprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCwRe:
			BtnStatus.Rc.IsPprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCcwRe:
			BtnStatus.Rc.IsPprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDoRe:
			BtnStatus.Rc.IsRewine = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPrdPprRe:
			BtnStatus.Rc.IsRewineReelPpr = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalPunch(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:	// 마킹부 연동 온/오프 스위치
			BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_Relation] = TRUE;
			break;
		case _SigInx::_MvCwMk:	// 마킹부 피딩 정회전 스위치
			BtnStatus.Mk.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_MvCwMk] = TRUE;
			break;
		case _SigInx::_MvCcwMk:	// 마킹부 피딩 역회전 스위치
			BtnStatus.Mk.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_MvCcwMk] = TRUE;
			break;
		case _SigInx::_FdVacMk:	// 마킹부 피딩 진공 스위치
			BtnStatus.Mk.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_FdVacMk] = TRUE;
			break;
		case _SigInx::_PushUpMk:	// 마킹부 제품푸쉬 스위치 // (토크 진공 스위치) - X
			BtnStatus.Mk.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PushUpMk] = TRUE;
			break;
		case _SigInx::_TblBlwMk:
			BtnStatus.Mk.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_TblBlwMk] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_14, 1);	// 마킹부 테이블 브로워 스위치
			break;
		case _SigInx::_TblVacMk:
			BtnStatus.Mk.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_TblVacMk] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_15, 1);	// 마킹부 테이블 진공 스위치
			break;
		case _SigInx::_FdClampMk:
			BtnStatus.Mk.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_FdClampMk] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_51, 1);	// 마킹부 피딩 클램프 스위치
			break;
		case _SigInx::_TensClampMk:
			BtnStatus.Mk.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_TensClampMk] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_52, 1);	// 마킹부 텐션 클램프 스위치
			break;
		case _SigInx::_OnePnlMk:
			BtnStatus.Mk.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_OnePnlMk] = TRUE;
			//pView->MpeWrite(_T("MB440151"), (long)SockData.nData1);
			break;
		case _SigInx::_DancerUpMk:
			BtnStatus.Mk.DcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_DancerUpMk] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_48, 1);	// 마킹부 댄서롤 상승/하강 스위치
			break;
			// Is
		case _SigInx::_IsRelation:
			BtnStatus.Mk.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwMk:
			BtnStatus.Mk.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwMk:
			BtnStatus.Mk.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacMk:
			BtnStatus.Mk.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpMk:
			BtnStatus.Mk.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblBlwMk:
			BtnStatus.Mk.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacMk:
			BtnStatus.Mk.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampMk:
			BtnStatus.Mk.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampMk:
			BtnStatus.Mk.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlMk:
			BtnStatus.Mk.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpMk:
			BtnStatus.Mk.IsDcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalAOIDn(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwAoiDn:
			BtnStatus.AoiDn.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_MvCwAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_56, 1);	// 검사부 하 피딩 정회전 스위치
			break;
		case _SigInx::_MvCcwAoiDn:
			BtnStatus.AoiDn.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_MvCcwAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_57, 1);	// 검사부 하 피딩 역회전 스위치
			break;
		case _SigInx::_FdVacAoiDn:
			BtnStatus.AoiDn.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_FdVacAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_58, 1);	// 검사부 하 피딩 진공 스위치
			break;
		case _SigInx::_PushUpAoiDn:
			BtnStatus.AoiDn.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_PushUpAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_59, 1);	// 검사부 하 제품푸쉬 스위치 // (토크 진공 스위치) - X
			break;
		case _SigInx::_TblBlwAoiDn:
			BtnStatus.AoiDn.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_TblBlwAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_60, 1);	// 검사부 하 테이블 브로워 스위치
			break;
		case _SigInx::_TblVacAoiDn:
			BtnStatus.AoiDn.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_TblVacAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_61, 1);	// 검사부 하 테이블 진공 스위치
			break;
		case _SigInx::_FdClampAoiDn:
			BtnStatus.AoiDn.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_FdClampAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_64, 1);	// 검사부 하 피딩 클램프 스위치
			break;
		case _SigInx::_TensClampAoiDn:
			BtnStatus.AoiDn.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_TensClampAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_65, 1);	// 검사부 하 텐션 클램프 스위치
			break;
		case _SigInx::_OnePnlAoiDn:
			BtnStatus.AoiDn.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_OnePnlAoiDn] = TRUE;
			//pView->MpeWrite(_T("MB440151"), (long)SockData.nData1);
			break;
		case _SigInx::_ClrRollAoiDn:
			BtnStatus.AoiDn.ClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_VelClrSonicAoiDn:
			BtnStatus.AoiDn.VelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_VelClrSonicAoiDn] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_88, 1);	// AOI(하) 초음파 세정기 속도 ON (PC가 ON/OFF시킴)
			break;
		case _SigInx::_TestAoiDn:
			BtnStatus.AoiDn.Test = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ResetAoiDn:
			BtnStatus.AoiDn.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_LotEndAoiDn:
			BtnStatus.AoiDn.LotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
			// Is
		case _SigInx::_IsRelation:
			BtnStatus.AoiDn.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwAoiDn:
			BtnStatus.AoiDn.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwAoiDn:
			BtnStatus.AoiDn.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacAoiDn:
			BtnStatus.AoiDn.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpAoiDn:
			BtnStatus.AoiDn.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblBlwAoiDn:
			BtnStatus.AoiDn.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacAoiDn:
			BtnStatus.AoiDn.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampAoiDn:
			BtnStatus.AoiDn.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampAoiDn:
			BtnStatus.AoiDn.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlAoiDn:
			BtnStatus.AoiDn.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollAoiDn:
			BtnStatus.AoiDn.IsClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsVelClrSonicAoiDn:
			BtnStatus.AoiDn.IsVelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTestAoiDn:
			BtnStatus.AoiDn.IsTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsResetAoiDn:
			BtnStatus.AoiDn.IsReset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsLotEndAoiDn:
			BtnStatus.AoiDn.IsLotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalAOIUp(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwAoiUp:
			BtnStatus.AoiUp.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwAoiUp:
			BtnStatus.AoiUp.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdVacAoiUp:
			BtnStatus.AoiUp.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PushUpAoiUp:
			BtnStatus.AoiUp.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblBlwAoiUp:
			BtnStatus.AoiUp.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblVacAoiUp:
			BtnStatus.AoiUp.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdClampAoiUp:
			BtnStatus.AoiUp.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TensClampAoiUp:
			BtnStatus.AoiUp.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_OnePnlAoiUp:
			BtnStatus.AoiUp.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_OnePnlAoiUp] = TRUE;
			break;
		case _SigInx::_ClrRollAoiUp:
			BtnStatus.AoiUp.ClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TestAoiUp:
			BtnStatus.AoiUp.Test = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ResetAoiUp:
			BtnStatus.AoiUp.Reset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_LotEndAoiUp:
			BtnStatus.AoiUp.LotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
			// Is
		case _SigInx::_IsRelation:
			BtnStatus.AoiUp.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwAoiUp:
			BtnStatus.AoiUp.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwAoiUp:
			BtnStatus.AoiUp.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacAoiUp:
			BtnStatus.AoiUp.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpAoiUp:
			BtnStatus.AoiUp.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblBlwAoiUp:
			BtnStatus.AoiUp.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacAoiUp:
			BtnStatus.AoiUp.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampAoiUp:
			BtnStatus.AoiUp.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampAoiUp:
			BtnStatus.AoiUp.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlAoiUp:
			BtnStatus.AoiUp.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollAoiUp:
			BtnStatus.AoiUp.IsClrRoll = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTestAoiUp:
			BtnStatus.AoiUp.IsTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsResetAoiUp:
			BtnStatus.AoiUp.IsReset = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsLotEndAoiUp:
			BtnStatus.AoiUp.IsLotEnd = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalEngrave(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwEng:
			BtnStatus.Eng.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwEng:
			BtnStatus.Eng.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdVacEng:
			BtnStatus.Eng.FdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PushUpEng:
			BtnStatus.Eng.PushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _TblBlwEng:
			BtnStatus.Eng.TblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TblVacEng:
			BtnStatus.Eng.TblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_FdClampEng:
			BtnStatus.Eng.FdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_TensClampEng:
			BtnStatus.Eng.TqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_OnePnlEng:
			BtnStatus.Eng.MvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_OnePnlEng] = TRUE;
			//pView->MpeWrite(_T("MB440151"), (long)SockData.nData1);
			break;
		case _SigInx::_DancerUpEng:
			BtnStatus.Eng.DcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_VelClrSonicEng:
			BtnStatus.Eng.VelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_VelClrSonicEng] = TRUE;
			//if (pView && pView->m_pDlgMenu03)
			//	pView->m_pDlgMenu03->SwMpeBtn(IDC_CHK_87, 1);	// 각인부 초음파 세정기 속도 ON (PC가 ON/OFF시킴)
			break;
			// Is
		case _SigInx::_IsRelation:
			BtnStatus.Eng.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwEng:
			BtnStatus.Eng.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwEng:
			BtnStatus.Eng.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdVacEng:
			BtnStatus.Eng.IsFdVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPushUpEng:
			BtnStatus.Eng.IsPushUp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _IsTblBlwEng:
			BtnStatus.Eng.IsTblBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTblVacEng:
			BtnStatus.Eng.IsTblVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsFdClampEng:
			BtnStatus.Eng.IsFdClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsTensClampEng:
			BtnStatus.Eng.IsTqClp = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsOnePnlEng:
			BtnStatus.Eng.IsMvOne = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpEng:
			BtnStatus.Eng.IsDcRSol = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsVelClrSonicEng:
			BtnStatus.Eng.IsVelSonicBlw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalUncoiler(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_Relation:
			//BtnStatus.Rc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Mk.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiDn.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.AoiUp.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Eng.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			//BtnStatus.Uc.Relation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCwUn:
			BtnStatus.Uc.FdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_MvCcwUn:
			BtnStatus.Uc.FdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PrdChuckUn:
			BtnStatus.Uc.ReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_DancerUpUn:
			BtnStatus.Uc.DcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteUpLfUn:
			BtnStatus.Uc.ReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteUpRtUn:
			BtnStatus.Uc.ReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PasteVacUn:
			BtnStatus.Uc.ReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprChuckUn:
			BtnStatus.Uc.PprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprCwUn:
			BtnStatus.Uc.PprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_PprCcwUn:
			BtnStatus.Uc.PprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ClrRollUpUn:
			BtnStatus.Uc.ClRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_ClrRollPushUn:
			BtnStatus.Uc.ClRlPshUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
			// Is
		case _SigInx::_IsRelation:
			BtnStatus.Uc.IsRelation = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCwUn:
			BtnStatus.Uc.IsFdCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMvCcwUn:
			BtnStatus.Uc.IsFdCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPrdChuckUn:
			BtnStatus.Uc.IsReelChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsDancerUpUn:
			BtnStatus.Uc.IsDcRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpLfUn:
			BtnStatus.Uc.IsReelJoinL = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteUpRtUn:
			BtnStatus.Uc.IsReelJoinR = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPasteVacUn:
			BtnStatus.Uc.IsReelJoinVac = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprChuckUn:
			BtnStatus.Uc.IsPprChuck = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCwUn:
			BtnStatus.Uc.IsPprCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsPprCcwUn:
			BtnStatus.Uc.IsPprCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollUpUn:
			BtnStatus.Uc.IsClRlUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsClrRollPushUn:
			BtnStatus.Uc.IsClRlPshUpDn = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

void CEngrave::GetSignalEngraveAutoSequence(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EngAutoInit:
			BtnStatus.EngAuto.Init = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[_SigInx::_EngAutoInit] = TRUE;
			break;
		case _SigInx::_EngAutoSeqMkSt:
			BtnStatus.EngAuto.MkSt = (SockData.nData1 > 0) ? TRUE : FALSE;
//#ifdef USE_MPE	
//			if (pView)
//				pView->MpeWrite(_T("MB440103"), SockData.nData1);// 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
//#endif
			break;
		case _SigInx::_EngAutoSeqOnMkIng:
			BtnStatus.EngAuto.OnMking = (SockData.nData1 > 0) ? TRUE : FALSE;
//#ifdef USE_MPE	
//			if (pView)
//				pView->MpeWrite(_T("MB440173"), SockData.nData1);// 2D(GUI) 각인 동작Running신호(PC On->PC Off)
//#endif
			break;
		case _SigInx::_EngAutoSeqMkDone:
			BtnStatus.EngAuto.MkDone = (SockData.nData1 > 0) ? TRUE : FALSE;
//#ifdef USE_MPE	
//			if (pView)
//				pView->MpeWrite(_T("MB440174"), 1);// 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
//				//pView->MpeWrite(_T("MB440174"), SockData.nData1);// 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
//#endif
			break;
		case _SigInx::_EngAutoSeq2dReadSt:
			BtnStatus.EngAuto.Read2dSt = (SockData.nData1 > 0) ? TRUE : FALSE;
//#ifdef USE_MPE	
//			if (pView)
//				pView->MpeWrite(_T("MB440105"), SockData.nData1);// 각인부 2D 리더 시작신호(PLC On->PC Off)
//#endif
			break;
		case _SigInx::_EngAutoSeqOnReading2d:
			BtnStatus.EngAuto.OnRead2d = (SockData.nData1 > 0) ? TRUE : FALSE;
//#ifdef USE_MPE	
//			if (pView)
//				pView->MpeWrite(_T("MB440178"), SockData.nData1);// 각인부 2D 리더 작업중 신호(PC On->PC Off)
//#endif
			break;
		case _SigInx::_EngAutoSeq2dReadDone:
			BtnStatus.EngAuto.Read2dDone = (SockData.nData1 > 0) ? TRUE : FALSE;
//#ifdef USE_MPE	
//			if (pView)
//				pView->MpeWrite(_T("MB440179"), 1);// 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
//				//pView->MpeWrite(_T("MB440179"), SockData.nData1);// 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
//#endif
			break;
			// Is
		case _SigInx::_IsEngAutoInit:
			BtnStatus.EngAuto.IsInit = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqMkSt:
			BtnStatus.EngAuto.IsMkSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqOnMkIng:
			BtnStatus.EngAuto.IsOnMking = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqMkDone:
			BtnStatus.EngAuto.IsMkDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeq2dReadSt:
			BtnStatus.EngAuto.IsRead2dSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeqOnReading2d:
			BtnStatus.EngAuto.IsOnRead2d = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsEngAutoSeq2dReadDone:
			BtnStatus.EngAuto.IsRead2dDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}


void CEngrave::GetSignalMyMsg(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_MyMsgYes:
			m_bRcvSig[_SigInx::_MyMsgYes] = TRUE;
			//pView->SetMyMsgYes();
			break;
		case _SigInx::_MyMsgNo:
			m_bRcvSig[_SigInx::_MyMsgNo] = TRUE;
			//pView->SetMyMsgNo();
			break;
		case _SigInx::_MyMsgOk:
			m_bRcvSig[_SigInx::_MyMsgOk] = TRUE;
			//pView->SetMyMsgOk();
			break;
			// Is
		case _SigInx::_IsMyMsgYes:
			BtnStatus.Msg.IsYes = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMyMsgNo:
			BtnStatus.Msg.IsNo = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_IsMyMsgOk:
			BtnStatus.Msg.IsOk = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
}

// Start for GetSysData()

void CEngrave::GetSysData(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetTotRatio(SockData);
	GetStTime(SockData);
	GetRunTime(SockData);
	GetEdTime(SockData);
	GetStripRatio(SockData);
	GetDef(SockData);
	Get2DReader(SockData);
	GetEngInfo(SockData);
	GetFdInfo(SockData);
	GetAoiInfo(SockData);
	GetMkInfo(SockData);
	GetMkInfoLf(SockData);
	GetMkInfoRt(SockData);
	GetAlarmMsg(SockData);
}

void CEngrave::GetOpInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode; // _SetSig or _SetData
	int nMsgId = SockData.nMsgID;

	CString sVal;
	//m_bGetOpInfo = FALSE;
	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_DualTest:
			if (pDoc->WorkingInfo.LastJob.bDualTest != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bDualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_DualTest] = TRUE;
				//pView->SetDualTest(pDoc->WorkingInfo.LastJob.bDualTest);
			}
			break;
		case _SigInx::_SampleTest:
			if (pDoc->WorkingInfo.LastJob.bSampleTest != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bSampleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_SampleTest] = TRUE;
//#ifdef USE_MPE
//				pView->MpeWrite(_T("MB44017B"), (pDoc->WorkingInfo.LastJob.bSampleTest) ? 1 : 0);		// Sample 검사 On
//#endif
			}
			break;
		case _SigInx::_TestMode:
			if(pDoc->GetTestMode() != (int)SockData.nData1) // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.nTestMode = (int)SockData.nData1; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
				m_bRcvSig[_SigInx::_TestMode] = TRUE;
				//pDoc->SetTestMode((int)SockData.nData1); // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
			}
			break;
		case _SigInx::_RecoilerCcw:
			if(pDoc->WorkingInfo.LastJob.bOneMetal != (SockData.nData1 > 0) ? TRUE : FALSE)	// OneMetal : TRUE -> SetTwoMetal(FALSE);
			{
				m_bGetOpInfo = TRUE;
				BtnStatus.Induct.Rc = pDoc->WorkingInfo.LastJob.bOneMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// OneMetal : TRUE -> SetTwoMetal(FALSE);
				m_bRcvSig[_SigInx::_RecoilerCcw] = TRUE;
			}
			break;
		case _SigInx::_UncoilerCcw:
			if(pDoc->WorkingInfo.LastJob.bTwoMetal != (SockData.nData1 > 0) ? TRUE : FALSE)	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
			{
				m_bGetOpInfo = TRUE;
				BtnStatus.Induct.Uc = pDoc->WorkingInfo.LastJob.bTwoMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
				m_bRcvSig[_SigInx::_UncoilerCcw] = TRUE;
			}
			break;
		case _SigInx::_AlignMethode:
			if(pDoc->WorkingInfo.LastJob.nAlignMethode != (int)SockData.nData1) // TWO_POINT, FOUR_POINT
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.nAlignMethode = (int)SockData.nData1; // TWO_POINT, FOUR_POINT
			}
			break;
		case _SigInx::_DoorRecoiler:
			if (pDoc->WorkingInfo.LastJob.bRclDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bRclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_DoorRecoiler] = TRUE;
			}
			break;
		case _SigInx::_DoorAoiUp:
			if (pDoc->WorkingInfo.LastJob.bAoiUpDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bAoiUpDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_DoorAoiUp] = TRUE;
			}
			break;
		case _SigInx::_DoorAoiDn:
			if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bAoiDnDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_DoorAoiDn] = TRUE;
			}
			break;
		case _SigInx::_DoorMk:
			if (pDoc->WorkingInfo.LastJob.bMkDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bMkDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_DoorMk] = TRUE;
			}
			break;
		case _SigInx::_DoorEngrave:
			if (pDoc->WorkingInfo.LastJob.bEngvDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bEngvDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_DoorEngrave] = TRUE;
			}
			break;

		case _SigInx::_DoorUncoiler:
			if (pDoc->WorkingInfo.LastJob.bUclDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_DoorUncoiler] = TRUE;
			}
			break;
		case _SigInx::_SaftyMk:
			if (pDoc->WorkingInfo.LastJob.bMkSftySen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bMkSftySen = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		case _SigInx::_CleannerAoiUp:
			if (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_CleannerAoiUp] = TRUE;
			}
			break;
		case _SigInx::_CleannerAoiDn:
			if (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_CleannerAoiDn] = TRUE;
			}
			break;
		case _SigInx::_UltraSonicAoiDn:
			if(pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_UltraSonicAoiDn] = TRUE;
			}
			break;
		case _SigInx::_UltraSonicEngrave:
			if (pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_UltraSonicEngrave] = TRUE;
			}
			break;
		case _SigInx::_Use380mm:
			if (pDoc->WorkingInfo.LastJob.bUse380mm != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUse380mm = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_Use380mm] = TRUE;
			}
			break;
		case _SigInx::_TempPause:
			if (pDoc->WorkingInfo.LastJob.bTempPause != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_TempPause] = TRUE;
				//pView->MpeWrite(_T("MB440183"), pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)
			}
			break;
		case _SigInx::_LotCut:
			if (pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE;
			}
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_OpName:
			if (pDoc->WorkingInfo.LastJob.sSelUserName != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);

				if (pDoc)
					::WritePrivateProfileString(_T("Last Job"), _T("Operator Name"), pDoc->WorkingInfo.LastJob.sSelUserName, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_SampleShotNum:
			sVal.Format(_T("%d"), (int)SockData.nData1);
			if (pDoc->WorkingInfo.LastJob.sSampleTestShotNum != sVal)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSampleTestShotNum = sVal;

				::WritePrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), sVal, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_TotReelLen:
			if (pDoc->WorkingInfo.LastJob.sReelTotLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sTotalReelDist = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), pDoc->WorkingInfo.LastJob.sReelTotLen, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), pDoc->WorkingInfo.Lot.sTotalReelDist, PATH_WORKING_INFO);

#ifdef USE_MPE
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 1000.0);
				if (pView)
					pView->MpeWrite(_T("ML45000"), lData);	// 전체 Reel 길이 (단위 M * 1000)
#endif
			}
			break;
		case _ItemInx::_OnePnlLen:
			if (pDoc->WorkingInfo.Motion.sMkFdDist != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);

				//if (pDoc->m_pReelMap)
				//	pDoc->m_pReelMap->m_dPnlLen = _tstof(pDoc->WorkingInfo.Motion.sMkFdDist);
				::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
#ifdef USE_MPE
				lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdDist) * 1000.0);
				if (pView)
					pView->MpeWrite(_T("ML45032"), lData);	// 한 판넬 길이 (단위 mm * 1000)
#endif
			}
			break;
		case _ItemInx::_TempStopLen:
			if (pDoc->WorkingInfo.LastJob.sTempPauseLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sStopDist = CharToString(SockData.strData);

				//if (pDoc->m_pReelMap)
				//	pDoc->m_pReelMap->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
				::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), pDoc->WorkingInfo.LastJob.sTempPauseLen, PATH_WORKING_INFO);

#ifdef USE_MPE
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen) * 1000.0);
				if (pView)
					pView->MpeWrite(_T("ML45006"), lData);	// 일시정지 길이 (단위 M * 1000)
#endif
			}
			break;
		case _ItemInx::_LotCutLen:
			if (pDoc->WorkingInfo.LastJob.sLotSepLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sSeparateDist = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotCutPosLen:
			if (pDoc->WorkingInfo.LastJob.sLotCutPosLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sCuttingDist = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), pDoc->WorkingInfo.LastJob.sLotCutPosLen, PATH_WORKING_INFO);
				//if (pDoc->m_pReelMap)
				//	pDoc->m_pReelMap->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
				::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), pDoc->WorkingInfo.LastJob.sLotCutPosLen, PATH_WORKING_INFO);

#ifdef USE_MPE
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen) * 1000.0);
				if(pView)
					pView->MpeWrite(_T("ML45004"), lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
#endif
			}
			break;
		case _ItemInx::_LmtTotYld:
			if(pDoc->WorkingInfo.LastJob.sLmtTotYld != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLmtTotYld = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), pDoc->WorkingInfo.LastJob.sLmtTotYld, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_LmtPatlYld:
			if(pDoc->WorkingInfo.LastJob.sLmtPatlYld != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLmtPatlYld = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), pDoc->WorkingInfo.LastJob.sLmtPatlYld, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_StripOutRatio:
			if(pDoc->WorkingInfo.LastJob.sStripOutRatio != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sStripOutRatio = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), pDoc->WorkingInfo.LastJob.sStripOutRatio, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_CustomNeedRatio:
			if (pDoc->WorkingInfo.LastJob.sCustomNeedRatio != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sCustomNeedRatio = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), pDoc->WorkingInfo.LastJob.sCustomNeedRatio, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_NumRangeFixDef:
			if (pDoc->WorkingInfo.LastJob.sNumRangeFixDef != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sNumRangeFixDef = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), pDoc->WorkingInfo.LastJob.sNumRangeFixDef, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_NumContFixDef:
			if (pDoc->WorkingInfo.LastJob.sNumContFixDef != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sNumContFixDef = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), pDoc->WorkingInfo.LastJob.sNumContFixDef, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_UltraSonicStTim:
			if(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, PATH_WORKING_INFO);
				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim) * 100.0);
#ifdef USE_MPE
				if (pView)
				{
					pView->MpeWrite(_T("MW05940"), (long)lData);	// AOI_Dn (단위 [초] * 100) : 1 is 10 mSec.
					pView->MpeWrite(_T("MW05942"), (long)lData);	// AOI_Up (단위 [초] * 100) : 1 is 10 mSec.
				}
#endif
			}
			break;
		case _ItemInx::_EngItsCode:
			if (pDoc->WorkingInfo.LastJob.sEngItsCode != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->SetEngItsCode(CharToString(SockData.strData));
			}
			break;
		}
	}
}

void CEngrave::GetInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	//m_bGetInfo = FALSE;
	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_TempPause:
			if(pDoc->WorkingInfo.LastJob.bTempPause != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
				m_bRcvSig[_SigInx::_TempPause] = TRUE;
			}
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_OpName:
			if (pDoc->WorkingInfo.LastJob.sSelUserName != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_ModelUpName:
			if (pDoc->WorkingInfo.EngInfo.sModelUp != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sModelUp = CharToString(SockData.strData);
			}
			break;
		//case _ItemInx::_ModelDnName:
		//	if (pDoc->WorkingInfo.LastJob.sModelDn != CharToString(SockData.strData))
		//	{
		//		m_bGetInfo = TRUE;
		//		pDoc->WorkingInfo.LastJob.sModelDn = CharToString(SockData.strData);
		//	}
		//	break;
		case _ItemInx::_LotUpName:
			if (pDoc->WorkingInfo.EngInfo.sLotUp != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sLotUp = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotDnName:
			if (pDoc->WorkingInfo.EngInfo.sLotDn != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sLotDn = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LayerUpName:
			if (pDoc->WorkingInfo.EngInfo.sLayerUp != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sLayerUp = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LayerDnName:
			if (pDoc->WorkingInfo.EngInfo.sLayerDn = CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sLayerDn = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LoadMstInfo:
			pView->m_bLoadMstInfo = TRUE;
			break;
		case _ItemInx::_TotReelLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sTotalReelDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_PartVel:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sPartialSpd = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Last Job"), _T("Partial Speed"), pDoc->WorkingInfo.LastJob.sPartialSpd, PATH_WORKING_INFO);
			break;
		case _ItemInx::_TempStopLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sStopDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_LotCutLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sSeparateDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_LotCutPosLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sCuttingDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_LotSerial:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sLotSerial = CharToString(SockData.strData);
			break;
		case _ItemInx::_MkVerfyLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sVerifyLen = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetTotRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
			;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_DefNumUp:
			pDoc->m_nBad[0] = SockData.nData1;
			break;
		case _ItemInx::_DefRtoUp:
			pDoc->m_dBadRatio[0] = (double)SockData.fData1;
			break;
		case _ItemInx::_GoodNumUp:
			pDoc->m_nGood[0] = SockData.nData1;
			break;
		case _ItemInx::_GoodRtoUp:
			pDoc->m_dGoodRatio[0] = (double)SockData.fData1;
			break;
		case _ItemInx::_TestNumUp:
			pDoc->m_nTestNum[0] = SockData.nData1;
			break;
		case _ItemInx::_DefNumDn:
			pDoc->m_nBad[1] = SockData.nData1;
			break;
		case _ItemInx::_DefRtoDn:
			pDoc->m_dBadRatio[1] = (double)SockData.fData1;
			break;
		case _ItemInx::_GoodNumDn:
			pDoc->m_nGood[1] = SockData.nData1;
			break;
		case _ItemInx::_GoodRtoDn:
			pDoc->m_dGoodRatio[1] = (double)SockData.fData1;
			break;
		case _ItemInx::_TestNumDn:
			pDoc->m_nTestNum[1] = SockData.nData1;
			break;
		case _ItemInx::_DefNumTot:
			pDoc->m_nBad[2] = SockData.nData1;
			break;
		case _ItemInx::_DefRtoTot:
			pDoc->m_dBadRatio[2] = (double)SockData.fData1;
			break;
		case _ItemInx::_GoodNumTot:
			pDoc->m_nGood[2] = SockData.nData1;
			break;
		case _ItemInx::_GoodRtoTot:
			pDoc->m_dGoodRatio[2] = (double)SockData.fData1;
			break;
		case _ItemInx::_TestNumTot:
			pDoc->m_nTestNum[2] = SockData.nData1;
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetStTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_LotStTime:
			pDoc->m_sLotStTime = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetRunTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case  _ItemInx::_LotRunTime:
			pDoc->m_sLotRunTime = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetEdTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_LotEdTime:
			pDoc->m_sLotEdTime = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetStripRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_1LnGoodRtoUp:
			pDoc->m_dStripRatio[0][0] = (double)SockData.fData1;
			break;
		case _ItemInx::_2LnGoodRtoUp:
			pDoc->m_dStripRatio[0][1] = (double)SockData.fData1;
			break;
		case _ItemInx::_3LnGoodRtoUp:
			pDoc->m_dStripRatio[0][2] = (double)SockData.fData1;
			break;
		case _ItemInx::_4LnGoodRtoUp:
			pDoc->m_dStripRatio[0][3] = (double)SockData.fData1;
			break;
		case _ItemInx::_AllLnGoodRtoUp:
			pDoc->m_dStripRatio[0][4] = (double)SockData.fData1;
			break;
		case _ItemInx::_1LnGoodRtoDn:
			pDoc->m_dStripRatio[1][0] = (double)SockData.fData1;
			break;
		case _ItemInx::_2LnGoodRtoDn:
			pDoc->m_dStripRatio[1][1] = (double)SockData.fData1;
			break;
		case _ItemInx::_3LnGoodRtoDn:
			pDoc->m_dStripRatio[1][2] = (double)SockData.fData1;
			break;
		case _ItemInx::_4LnGoodRtoDn:
			pDoc->m_dStripRatio[1][3] = (double)SockData.fData1;
			break;
		case _ItemInx::_AllLnGoodRtoDn:
			pDoc->m_dStripRatio[1][4] = (double)SockData.fData1;
			break;
		case _ItemInx::_1LnGoodRtoTot:
			pDoc->m_dStripRatio[2][0] = (double)SockData.fData1;
			break;
		case _ItemInx::_2LnGoodRtoTot:
			pDoc->m_dStripRatio[2][1] = (double)SockData.fData1;
			break;
		case _ItemInx::_3LnGoodRtoTot:
			pDoc->m_dStripRatio[2][2] = (double)SockData.fData1;
			break;
		case _ItemInx::_4LnGoodRtoTot:
			pDoc->m_dStripRatio[2][3] = (double)SockData.fData1;
			break;
		case _ItemInx::_AllLnGoodRtoTot:
			pDoc->m_dStripRatio[2][4] = (double)SockData.fData1;
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetDef(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_DefNumOpen:
			pDoc->m_nDef[DEF_OPEN] = SockData.nData1; // IDC_STC_DEF_OPEN
			break;
		case _ItemInx::_DefNumShort:
			pDoc->m_nDef[DEF_SHORT] = SockData.nData1; // IDC_STC_DEF_SHORT
			break;
		case _ItemInx::_DefNumUshort:
			pDoc->m_nDef[DEF_USHORT] = SockData.nData1; // IDC_STC_DEF_U_SHORT
			break;
		case _ItemInx::_DefNumLnW:
			pDoc->m_nDef[DEF_SPACE] = SockData.nData1; // IDC_STC_DEF_SPACE
			break;
		case _ItemInx::_DefExtr:
			pDoc->m_nDef[DEF_EXTRA] = SockData.nData1; // IDC_STC_DEF_EXTRA
			break;
		case _ItemInx::_DefNumProt:
			pDoc->m_nDef[DEF_PROTRUSION] = SockData.nData1; // IDC_STC_DEF_PROT
			break;
		case _ItemInx::_DefNumPhole:
			pDoc->m_nDef[DEF_PINHOLE] = SockData.nData1; // IDC_STC_DEF_P_HOLE
			break;
		case _ItemInx::_DefNumPad:
			pDoc->m_nDef[DEF_PAD] = SockData.nData1; // IDC_STC_DEF_PAD
			break;
		case _ItemInx::_DefNumHopen:
			pDoc->m_nDef[DEF_HOLE_OPEN] = SockData.nData1; // IDC_STC_DEF_H_OPEN
			break;
		case _ItemInx::_DefNumHmiss:
			pDoc->m_nDef[DEF_HOLE_MISS] = SockData.nData1; // IDC_STC_DEF_H_MISS
			break;
		case _ItemInx::_DefNumHpos:
			pDoc->m_nDef[DEF_HOLE_POSITION] = SockData.nData1; // IDC_STC_DEF_H_POS
			break;
		case _ItemInx::_DefNumHdef:
			pDoc->m_nDef[DEF_HOLE_DEFECT] = SockData.nData1; // IDC_STC_DEF_H_DEF
			break;
		case _ItemInx::_DefNumNick:
			pDoc->m_nDef[DEF_NICK] = SockData.nData1; // IDC_STC_DEF_NICK
			break;
		case _ItemInx::_DefNumPoi:
			pDoc->m_nDef[DEF_POI] = SockData.nData1; // IDC_STC_DEF_POI
			break;
		case _ItemInx::_DefNumVhOpen:
			pDoc->m_nDef[DEF_VH_OPEN] = SockData.nData1; // IDC_STC_DEF_VH_OPEN
			break;
		case _ItemInx::_DefNumVhMiss:
			pDoc->m_nDef[DEF_VH_MISS] = SockData.nData1; // IDC_STC_DEF_VH_MISS
			break;
		case _ItemInx::_DefNumVhPos:
			pDoc->m_nDef[DEF_VH_POSITION] = SockData.nData1; // IDC_STC_DEF_VH_POS
			break;
		case _ItemInx::_DefNumVhd:
			pDoc->m_nDef[DEF_VH_DEF] = SockData.nData1; // IDC_STC_DEF_VH_DEF
			break;
		case _ItemInx::_DefNumLight:
			pDoc->m_nDef[DEF_LIGHT] = SockData.nData1; // IDC_STC_DEF_LIGHT
			break;
		case _ItemInx::_DefNumEnick:
			pDoc->m_nDef[DEF_EDGE_NICK] = SockData.nData1;
			break;
		case _ItemInx::_DefNumEprot:
			pDoc->m_nDef[DEF_EDGE_PROT] = SockData.nData1;
			break;
		case _ItemInx::_DefNumEspace:
			pDoc->m_nDef[DEF_EDGE_SPACE] = SockData.nData1;
			break;
		case _ItemInx::_DefNumUdd1:
			pDoc->m_nDef[DEF_USER_DEFINE_1] = SockData.nData1;
			break;
		case _ItemInx::_DefNumNrw:
			pDoc->m_nDef[DEF_NARROW] = SockData.nData1;
			break;
		case _ItemInx::_DefNumWide:
			pDoc->m_nDef[DEF_WIDE] = SockData.nData1;
			break;
		default:
			break;
		}
	}
}

void CEngrave::Get2DReader(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_2DEngLen:
			pDoc->WorkingInfo.Motion.s2DEngLen = CharToString(SockData.strData);
			pDoc->SetEngraveReaderDist(_tstof(pDoc->WorkingInfo.Motion.s2DEngLen));
			break;
		case _ItemInx::_2DAoiLen:
			pDoc->WorkingInfo.Motion.s2DAoiLen = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMkLen:
			pDoc->WorkingInfo.Motion.s2DMkLen = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMoveVel:
			pDoc->WorkingInfo.Motion.s2DMoveVel = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMoveAcc:
			pDoc->WorkingInfo.Motion.s2DMoveAcc = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DOneShotLen:
			pDoc->WorkingInfo.Motion.s2DOneShotRemainLen = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetEngInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EngBuffJogCw:
			pDoc->WorkingInfo.Motion.bEngBuffJogCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffJogCcw:
			pDoc->WorkingInfo.Motion.bEngBuffJogCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffOrgMv:
			pDoc->WorkingInfo.Motion.bEngBuffHomming = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffOrgMvDone:
			pDoc->WorkingInfo.Motion.bEngBuffHommingDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffInitPosMv:
			pDoc->WorkingInfo.Motion.bEngBuffInitMv = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffInitPosMvDone:
			pDoc->WorkingInfo.Motion.bEngBuffInitMvDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		case _SigInx::_EngBuffInitPosSave:
			pDoc->WorkingInfo.Motion.bEngBuffInitPosSave = (SockData.nData1 > 0) ? TRUE : FALSE;
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_EngLeadPitch:
			pDoc->SetEngraveFdPitch(_tstof(CharToString(SockData.strData)));
			break;
		case _ItemInx::_EngPushOffLen:
			pDoc->WorkingInfo.Motion.sEngraveFdVacOff = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngTqVal:
			pDoc->WorkingInfo.Motion.sEngraveTq = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngAoiLen:
			pDoc->SetEngraveAoiDist(_tstoi(CharToString(SockData.strData)));
			break;
		case _ItemInx::_EngFdDiffMax:
			pDoc->WorkingInfo.Motion.sEngFdDiffMax = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngFdDiffRng:
			pDoc->WorkingInfo.Motion.sEngFdDiffRng = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngFdDiffNum:
			pDoc->WorkingInfo.Motion.sEngFdDiffNum = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngBuffInitPos:
			pDoc->WorkingInfo.Motion.sEngBuffInitPos = CharToString(SockData.strData);
			pDoc->SetEngBufInitPos(_tstof(pDoc->WorkingInfo.Motion.sEngBuffInitPos));
			break;
		case _ItemInx::_EngBuffCurrPos:
			pDoc->WorkingInfo.Motion.sEngBuffCurrPos = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetFdInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	
	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_FdVel:
			pDoc->WorkingInfo.Motion.sMkJogVel = pDoc->WorkingInfo.Motion.sAoiJogVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), pDoc->WorkingInfo.Motion.sMkJogVel, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), pDoc->WorkingInfo.Motion.sAoiJogVel, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogVel) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45038"), lData);	// 연속공급 속도 (단위 mm/sec * 1000)
#endif
			break;
		case _ItemInx::_FdAcc:
			pDoc->WorkingInfo.Motion.sMkJogAcc = pDoc->WorkingInfo.Motion.sAoiJogAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), pDoc->WorkingInfo.Motion.sMkJogAcc, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), pDoc->WorkingInfo.Motion.sAoiJogAcc, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogAcc) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45040"), lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)
#endif
			break;
		case _ItemInx::_OnePnlLen:
			pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_OnePnlVel:
			pDoc->WorkingInfo.Motion.sMkFdVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), pDoc->WorkingInfo.Motion.sMkFdVel, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), pDoc->WorkingInfo.Motion.sMkFdVel, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdVel) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45034"), lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
#endif
			break;
		case _ItemInx::_OnePnlAcc:
			pDoc->WorkingInfo.Motion.sMkFdAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), pDoc->WorkingInfo.Motion.sMkFdAcc, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), pDoc->WorkingInfo.Motion.sMkFdAcc, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdAcc) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45036"), lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)
#endif
			break;
		case _ItemInx::_FdDiffMax:
			pDoc->WorkingInfo.Motion.sLmtFdErr = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), pDoc->WorkingInfo.Motion.sLmtFdErr, PATH_WORKING_INFO);
			break;
		case _ItemInx::_FdDiffRng:
			pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet, PATH_WORKING_INFO);
			break;
		case _ItemInx::_FdDiffNum:
			pDoc->WorkingInfo.Motion.sLmtFdOvrNum = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), pDoc->WorkingInfo.Motion.sLmtFdOvrNum, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetAoiInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_AoiLeadPitch:
			pDoc->WorkingInfo.Motion.sAoiFdLead = CharToString(SockData.strData);

			pDoc->SetAoiFdPitch(_tstof(pDoc->WorkingInfo.Motion.sAoiFdLead));
			break;
		case _ItemInx::_AoiPushOffLen:
			pDoc->WorkingInfo.Motion.sAoiFdVacOff = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), pDoc->WorkingInfo.Motion.sAoiFdVacOff, PATH_WORKING_INFO);
			break;
		case _ItemInx::_AoiTqVal:
			pDoc->WorkingInfo.Motion.sAoiTq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), pDoc->WorkingInfo.Motion.sAoiTq, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiTq) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45042"), lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif
			break;
		case _ItemInx::_AoiBuffShotNum:
			pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * 1000);
			if (pView)
				pView->MpeWrite(_T("ML45010"), lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
#endif
			break;
		case _ItemInx::_AoiMkLen:
			pDoc->WorkingInfo.Motion.sFdMkAoiInitDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45008"), lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)
#endif
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfo(SOCKET_DATA SockData)
{
	long lData;
	CString sData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkLeadPitch:
			pDoc->WorkingInfo.Motion.sMkFdLead = CharToString(SockData.strData);

			pDoc->SetMkFdPitch(_tstof(pDoc->WorkingInfo.Motion.sMkFdLead));
			break;
		case _ItemInx::_MkPushOffLen:
			pDoc->WorkingInfo.Motion.sMkFdVacOff = CharToString(SockData.strData);
	
			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), pDoc->WorkingInfo.Motion.sMkFdVacOff, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkTqVal:
			pDoc->WorkingInfo.Motion.sMkTq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), pDoc->WorkingInfo.Motion.sMkTq, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkTq) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45044"), lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif
			break;
		case _ItemInx::_MkBuffInitPos:
			pDoc->WorkingInfo.Motion.sStBufPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), pDoc->WorkingInfo.Motion.sStBufPos, PATH_WORKING_INFO);
#ifdef USE_MPE
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sStBufPos) * 1000.0);
			if (pView)
				pView->MpeWrite(_T("ML45016"), lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
#endif
			break;
		case _ItemInx::_MkBuffCurrPos:
			pDoc->m_dMkBuffCurrPos = (double)SockData.fData1;
			break;
		case _ItemInx::_MkNumLf:
			pDoc->WorkingInfo.Marking[0].nMkCnt = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[0].nMkCnt);
			::WritePrivateProfileString(_T("Marking0"), _T("Marking Count"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkNumRt:
			pDoc->WorkingInfo.Marking[1].nMkCnt = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[1].nMkCnt);
			::WritePrivateProfileString(_T("Marking1"), _T("Marking Count"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkMaxNumLf:
			pDoc->WorkingInfo.Marking[0].nMkLimit = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[0].nMkLimit);
			::WritePrivateProfileString(_T("Marking0"), _T("Marking Limit"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkMaxNumRt:
			pDoc->WorkingInfo.Marking[1].nMkLimit = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[1].nMkLimit);
			::WritePrivateProfileString(_T("Marking1"), _T("Marking Limit"), sData, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfoLf(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkInitPosLf:
			pDoc->WorkingInfo.Marking[0].sWaitPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), pDoc->WorkingInfo.Marking[0].sWaitPos, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkInitVelLf:
			pDoc->WorkingInfo.Marking[0].sWaitVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), pDoc->WorkingInfo.Marking[0].sWaitVel, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkInitAccLf:
			pDoc->WorkingInfo.Marking[0].sWaitAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), pDoc->WorkingInfo.Marking[0].sWaitAcc, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkFnlPosLf:
			pDoc->WorkingInfo.Marking[0].sMarkingPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), pDoc->WorkingInfo.Marking[0].sMarkingPos, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlVelLf:
			pDoc->WorkingInfo.Marking[0].sMarkingVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), pDoc->WorkingInfo.Marking[0].sMarkingVel, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlAccLf:
			pDoc->WorkingInfo.Marking[0].sMarkingAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), pDoc->WorkingInfo.Marking[0].sMarkingAcc, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlTqLf:
			pDoc->WorkingInfo.Marking[0].sMarkingToq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), pDoc->WorkingInfo.Marking[0].sMarkingToq, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkHgtPosX1Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY1Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX2Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY2Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX3Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY3Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX4Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY4Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtAvgPosLf:
			pDoc->WorkingInfo.Marking[0].sAverDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), pDoc->WorkingInfo.Marking[0].sAverDist, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfoRt(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkInitPosRt:
			pDoc->WorkingInfo.Marking[1].sWaitPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), pDoc->WorkingInfo.Marking[1].sWaitPos, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkInitVelRt:
			pDoc->WorkingInfo.Marking[1].sWaitVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), pDoc->WorkingInfo.Marking[1].sWaitVel, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkInitAccRt:
			pDoc->WorkingInfo.Marking[1].sWaitAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), pDoc->WorkingInfo.Marking[1].sWaitAcc, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkFnlPosRt:
			pDoc->WorkingInfo.Marking[1].sMarkingPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), pDoc->WorkingInfo.Marking[1].sMarkingPos, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlVelRt:
			pDoc->WorkingInfo.Marking[1].sMarkingVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), pDoc->WorkingInfo.Marking[1].sMarkingVel, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlAccRt:
			pDoc->WorkingInfo.Marking[1].sMarkingAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), pDoc->WorkingInfo.Marking[1].sMarkingAcc, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlTqRt:
			pDoc->WorkingInfo.Marking[1].sMarkingToq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), pDoc->WorkingInfo.Marking[1].sMarkingToq, PATH_WORKING_INFO);
			pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkHgtPosX1Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY1Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX2Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY2Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX3Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY3Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX4Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY4Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtAvgPosRt:
			pDoc->WorkingInfo.Marking[1].sAverDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), pDoc->WorkingInfo.Marking[1].sAverDist, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetAlarmMsg(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _stAlarmInx::_Alarm:
			//pDoc->m_sAlmMsg = CharToString(SockData.strData);
			break;
		case _stAlarmInx::_IsAlarm:
			pDoc->m_sIsAlmMsg = CharToString(SockData.strData);
			break;
		}
	}
}

void CEngrave::GetMsgBox(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _stMsgBoxInx::_MsgBox:
			pDoc->m_sMsgBox = CharToString(SockData.strData);
			break;
		case _stMsgBoxInx::_IsMsgBox:
			pDoc->m_sIsMsgBox = CharToString(SockData.strData);
			break;
		}
	}
}

// End for GetSysData()

// Start for SetSysSignal()

BOOL CEngrave::SetSysSignal()
{
	if (!IsRunning())
	{
		SetSignalMain();
		SetSignalTorqueMotor();
		SetSignalInductionMotor();
		SetSignalCore150mm();
		//SetSignalEtc();
		SetSignalRecoiler();
		SetSignalPunch();
		SetSignalAOIDn();
		SetSignalAOIUp();
		SetSignalEngrave();
		SetSignalUncoiler();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetSignalMain()
{
	//if (BtnStatus.Main.PrevReady != BtnStatus.Main.Ready)
	{
		BtnStatus.Main.PrevReady = BtnStatus.Main.Ready;
		SwReady(BtnStatus.Main.Ready);		// 마킹부 운전준비 스위치 "MB005503" IDC_CHK_34
	}
	//if (BtnStatus.Main.PrevRun != BtnStatus.Main.Run)
	{
		BtnStatus.Main.PrevRun = BtnStatus.Main.Run;
		SwRun(BtnStatus.Main.Run);			// 마킹부 운전 스위치 "MB005501" IDC_CHK_0
	}
	//if (BtnStatus.Main.PrevReset != BtnStatus.Main.Reset)
	{
		BtnStatus.Main.PrevReset = BtnStatus.Main.Reset;
		SwReset(BtnStatus.Main.Reset);		// 마킹부 리셋 스위치 "MB005504" IDC_CHK_1
	}
	//if (BtnStatus.Main.PrevStop != BtnStatus.Main.Stop)
	{
		BtnStatus.Main.PrevStop = BtnStatus.Main.Stop;
		SwStop(BtnStatus.Main.Stop);			// 마킹부 정지 스위치 "MB005502" IDC_CHK_33
	}
	//if (BtnStatus.Main.PrevAuto != BtnStatus.Main.Auto)
	{
		BtnStatus.Main.PrevAuto = BtnStatus.Main.Auto;
		SwAuto(BtnStatus.Main.Auto);			// 마킹부 자동 스위치 MB005505,	X4205	,	마킹부 자동/수동 (ON)
	}
	//if (BtnStatus.Main.PrevManual != BtnStatus.Main.Manual)
	{
		BtnStatus.Main.PrevManual = BtnStatus.Main.Manual;
		SwManual(BtnStatus.Main.Manual);		// 마킹부 수동 스위치 MB005505,	X4205	,	마킹부 자동/수동 (OFF)
	}
}

void CEngrave::SetSignalTorqueMotor()
{
	//if (BtnStatus.Tq.PrevMk != BtnStatus.Tq.Mk)
	{
		BtnStatus.Tq.PrevMk = BtnStatus.Tq.Mk;
		SwMkTq(BtnStatus.Tq.Mk); 				// 마킹부 텐션 ON (PC가 ON/OFF시킴) "MB440155" IDC_CHK_2
		// pDoc->WorkingInfo.Motion.bMkTq
	}
	//if (BtnStatus.Tq.PrevAoi != BtnStatus.Tq.Aoi)
	{
		BtnStatus.Tq.PrevAoi = BtnStatus.Tq.Aoi;
		SwAoiTq(BtnStatus.Tq.Aoi); 			// 검사부 텐션 ON (PC가 ON/OFF시킴) "MB440156" IDC_CHK_3
		// pDoc->WorkingInfo.Motion.bAoiTq
	}
	//if (BtnStatus.Tq.PrevEng != BtnStatus.Tq.Eng)
	{
		BtnStatus.Tq.PrevEng = BtnStatus.Tq.Eng;
		SwEngTq(BtnStatus.Tq.Eng); 			// 각인부 텐션 ON (PC가 ON/OFF시킴) "MB440154" IDC_CHK_84
		// pDoc->WorkingInfo.Motion.bEngraveTq
	}
}

void CEngrave::SetSignalInductionMotor()
{
	//if (BtnStatus.Induct.PrevRc != BtnStatus.Induct.Rc)
	{
		BtnStatus.Induct.PrevRc = BtnStatus.Induct.Rc;
		SwRcInductionCcw(BtnStatus.Induct.Rc);	// One Metal IDC_CHK_68		
		//pView->SetTwoMetal(FALSE, TRUE);
	}
	//if (BtnStatus.Induct.PrevUc != BtnStatus.Induct.Uc)
	{
		BtnStatus.Induct.PrevUc = BtnStatus.Induct.Uc;
		SwUcInductionCcw(BtnStatus.Induct.Uc);	// Two Metal IDC_CHK_69
		//pView->SetTwoMetal(TRUE, TRUE);
	}
}

void CEngrave::SetSignalCore150mm()
{
	//if (BtnStatus.Core150.PrevRc != BtnStatus.Core150.Rc)
	{
		BtnStatus.Core150.PrevRc = BtnStatus.Core150.Rc;
		SwRcCore150mm(BtnStatus.Core150.Rc);	// Recoiler IDC_CHK_70		
		//SetCore150mmRecoiler(TRUE);
	}
	//if (BtnStatus.Core150.PrevUc != BtnStatus.Core150.Uc)
	{
		BtnStatus.Core150.PrevUc = BtnStatus.Core150.Uc;
		SwUcCore150mm(BtnStatus.Core150.Uc);	// Uncoiler IDC_CHK_71		
		//SetCore150mmUncoiler(TRUE);
	}
}

void CEngrave::SetSignalEtc()
{
	//SwAoiEmg(BtnStatus.Etc.EmgAoi);
}

void CEngrave::SetSignalRecoiler()
{
	//if (BtnStatus.Rc.PrevRelation != BtnStatus.Rc.Relation)
	{
		BtnStatus.Rc.PrevRelation = BtnStatus.Rc.Relation;
		SwRcRelation(BtnStatus.Rc.Relation);						// 리코일러 연동 온/오프 스위치 "MB005801" IDC_CHK_4
	}
	//if (BtnStatus.Rc.PrevFdCw != BtnStatus.Rc.FdCw)
	{
		BtnStatus.Rc.PrevFdCw = BtnStatus.Rc.FdCw;
		SwRcFdCw(BtnStatus.Rc.FdCw);								// 리코일러 제품휠 정회전 스위치 "MB00580C" IDC_CHK_5
	}
	//if (BtnStatus.Rc.PrevFdCcw != BtnStatus.Rc.FdCcw)
	{
		BtnStatus.Rc.PrevFdCcw = BtnStatus.Rc.FdCcw;
		SwRcFdCcw(BtnStatus.Rc.FdCcw);							// 리코일러 제품휠 역회전 스위치 "MB00580D" IDC_CHK_6
	}
	//if (BtnStatus.Rc.PrevReelChuck != BtnStatus.Rc.ReelChuck)
	{
		BtnStatus.Rc.PrevReelChuck = BtnStatus.Rc.ReelChuck;
		SwRcReelChuck(BtnStatus.Rc.ReelChuck);					// 리코일러 제품척 클램프 스위치 "MB00580B" IDC_CHK_41
	}
	//if (BtnStatus.Rc.PrevDcRlUpDn != BtnStatus.Rc.DcRlUpDn)
	{
		BtnStatus.Rc.PrevDcRlUpDn = BtnStatus.Rc.DcRlUpDn;
		SwRcDcRlUpDn(BtnStatus.Rc.DcRlUpDn);						// 리코일러 댄서롤 상승/하강 스위치 "MB005802" IDC_CHK_42
	}
	//if (BtnStatus.Rc.PrevReelJoinL != BtnStatus.Rc.ReelJoinL)
	{
		BtnStatus.Rc.PrevReelJoinL = BtnStatus.Rc.ReelJoinL;
		SwRcReelJoinL(BtnStatus.Rc.ReelJoinL);					// 리코일러 제품 이음매(상/좌) 스위치 "MB005805" IDC_CHK_43
	}
	//if (BtnStatus.Rc.PrevReelJoinR != BtnStatus.Rc.ReelJoinR)
	{
		BtnStatus.Rc.PrevReelJoinR = BtnStatus.Rc.ReelJoinR;
		SwRcReelJoinR(BtnStatus.Rc.ReelJoinR);					// 리코일러 제품 이음매(하/우) 스위치 "MB005806" IDC_CHK_7
	}
	//if (BtnStatus.Rc.PrevReelJoinVac != BtnStatus.Rc.ReelJoinVac)
	{
		BtnStatus.Rc.PrevReelJoinVac = BtnStatus.Rc.ReelJoinVac;
		SwRcReelJoinVac(BtnStatus.Rc.ReelJoinVac);				// 리코일러 제품 이음매 진공 스위치 "MB00580F" IDC_CHK_8
	}
	//if (BtnStatus.Rc.PrevPprChuck != BtnStatus.Rc.PprChuck)
	{
		BtnStatus.Rc.PrevPprChuck = BtnStatus.Rc.PprChuck;
		SwRcPaperChuck(BtnStatus.Rc.PprChuck);					// 리코일러 간지척 클램프 스위치 "MB005808" IDC_CHK_44
	}
	//if (BtnStatus.Rc.PrevPprCw != BtnStatus.Rc.PprCw)
	{
		BtnStatus.Rc.PrevPprCw = BtnStatus.Rc.PprCw;
		SwRcPaperCw(BtnStatus.Rc.PprCw);							// 리코일러 간지휠 정회전 스위치 "MB005809" IDC_CHK_45
	}
	//if (BtnStatus.Rc.PrevPprCcw != BtnStatus.Rc.PprCcw)
	{
		BtnStatus.Rc.PrevPprCcw = BtnStatus.Rc.PprCcw;
		SwRcPaperCcw(BtnStatus.Rc.PprCcw);						// 리코일러 간지휠 역회전 스위치 "MB00580A" IDC_CHK_46
	}
	//if (BtnStatus.Rc.PrevRewine != BtnStatus.Rc.Rewine)
	{
		BtnStatus.Rc.PrevRewine = BtnStatus.Rc.Rewine;
		SwRcRewinder(BtnStatus.Rc.Rewine);						// 리코일러 Rewinder 동작 스위치 "MB005803" IDC_CHK_66
	}
	//if (BtnStatus.Rc.PrevRewineReelPpr != BtnStatus.Rc.RewineReelPpr)
	{
		BtnStatus.Rc.PrevRewineReelPpr = BtnStatus.Rc.RewineReelPpr;
		SwRcRewinderReelPaper(BtnStatus.Rc.RewineReelPpr);		// 리코일러 Rewinder 제품 & 간지 스위치 "MB005804" IDC_CHK_67
	}
}

void CEngrave::SetSignalPunch()
{
	//if (BtnStatus.Mk.PrevRelation != BtnStatus.Mk.Relation)
	{
		BtnStatus.Mk.PrevRelation = BtnStatus.Mk.Relation;
		SwMkRelation(BtnStatus.Mk.Relation);			// 마킹부 연동 온/오프 스위치 "MB005511" IDC_CHK_9
	}
	//if (BtnStatus.Mk.PrevFdCw != BtnStatus.Mk.FdCw)
	{
		BtnStatus.Mk.PrevFdCw = BtnStatus.Mk.FdCw;
		SwMkFdCw(BtnStatus.Mk.FdCw);					// 마킹부 피딩 정회전 스위치 "MB005513" IDC_CHK_10
	}
	//if (BtnStatus.Mk.PrevFdCcw != BtnStatus.Mk.FdCcw)
	{
		BtnStatus.Mk.PrevFdCcw = BtnStatus.Mk.FdCcw;
		SwMkFdCcw(BtnStatus.Mk.FdCcw);				// 마킹부 피딩 역회전 스위치 "MB005514" IDC_CHK_11
	}
	//if (BtnStatus.Mk.PrevFdVac != BtnStatus.Mk.FdVac)
	{
		BtnStatus.Mk.PrevFdVac = BtnStatus.Mk.FdVac;
		SwMkFdVac(BtnStatus.Mk.FdVac);				// 마킹부 피딩 진공 스위치 "MB005515" IDC_CHK_12
	}
	//if (BtnStatus.Mk.PrevPushUp != BtnStatus.Mk.PushUp)
	{
		BtnStatus.Mk.PrevPushUp = BtnStatus.Mk.PushUp;
		SwMkPushUp(BtnStatus.Mk.PushUp);				// 마킹부 제품푸쉬 스위치 "MB005516" // (토크 진공 스위치) - X IDC_CHK_13
	}
	//if (BtnStatus.Mk.PrevTblBlw != BtnStatus.Mk.TblBlw)
	{
		BtnStatus.Mk.PrevTblBlw = BtnStatus.Mk.TblBlw;
		SwMkTblBlw(BtnStatus.Mk.TblBlw);				// 마킹부 테이블 브로워 스위치 "MB005512" IDC_CHK_14
	}
	//if (BtnStatus.Mk.PrevTblVac != BtnStatus.Mk.TblVac)
	{
		BtnStatus.Mk.PrevTblVac = BtnStatus.Mk.TblVac;
		SwMkTblVac(BtnStatus.Mk.TblVac);				// 마킹부 테이블 진공 스위치 "MB005517" IDC_CHK_15
	}
	//if (BtnStatus.Mk.PrevFdClp != BtnStatus.Mk.FdClp)
	{
		BtnStatus.Mk.PrevFdClp = BtnStatus.Mk.FdClp;
		SwMkFdClp(BtnStatus.Mk.FdClp);				// 마킹부 피딩 클램프 스위치 "MB005519" IDC_CHK_51
	}
	//if (BtnStatus.Mk.PrevTqClp != BtnStatus.Mk.TqClp)
	{
		BtnStatus.Mk.PrevTqClp = BtnStatus.Mk.TqClp;
		SwMkTqClp(BtnStatus.Mk.TqClp);				// 마킹부 텐션 클램프 스위치 "MB00551A" IDC_CHK_52
	}
	//if (BtnStatus.Mk.PrevMvOne != BtnStatus.Mk.MvOne)
	{
		BtnStatus.Mk.PrevMvOne = BtnStatus.Mk.MvOne;
		SwMkMvOne(BtnStatus.Mk.MvOne);				// 마킹부 한판넬 이송 스위치 "MB440151" IDC_CHK_16
	}
	//if (BtnStatus.Mk.PrevLsrPt != BtnStatus.Mk.LsrPt)
	{
		BtnStatus.Mk.PrevLsrPt = BtnStatus.Mk.LsrPt;
		SwMkLsrPt(BtnStatus.Mk.LsrPt);				// 마킹부 레이져마크 스위치 "MB005518" IDC_CHK_49
	}
	//if (BtnStatus.Mk.PrevDcRSol != BtnStatus.Mk.DcRSol)
	{
		BtnStatus.Mk.PrevDcRSol = BtnStatus.Mk.DcRSol;
		SwMkDcRSol(BtnStatus.Mk.DcRSol);				// 마킹부 댄서롤 상승/하강 스위치 "MB00551B", "X421B" IDC_CHK_48
	}
}

void CEngrave::SetSignalAOIDn()
{
	//if (BtnStatus.AoiDn.PrevRelation != BtnStatus.AoiDn.Relation)
	{
		BtnStatus.AoiDn.PrevRelation = BtnStatus.AoiDn.Relation;
		SwAoiDnRelation(BtnStatus.AoiDn.Relation);			// 검사부 하 연동 온/오프 스위치 "MB005701" IDC_CHK_55
	}
	//if (BtnStatus.AoiDn.PrevFdCw != BtnStatus.AoiDn.FdCw)
	{
		BtnStatus.AoiDn.PrevFdCw = BtnStatus.AoiDn.FdCw;
		SwAoiDnFdCw(BtnStatus.AoiDn.FdCw);					// 검사부 하 피딩 정회전 스위치 "MB005703" IDC_CHK_56
	}
	//if (BtnStatus.AoiDn.PrevFdCcw != BtnStatus.AoiDn.FdCcw)
	{
		BtnStatus.AoiDn.PrevFdCcw = BtnStatus.AoiDn.FdCcw;
		SwAoiDnFdCcw(BtnStatus.AoiDn.FdCcw);					// 검사부 하 피딩 역회전 스위치 "MB005704" IDC_CHK_57
	}
	//if (BtnStatus.AoiDn.PrevFdVac != BtnStatus.AoiDn.FdVac)
	{
		BtnStatus.AoiDn.PrevFdVac = BtnStatus.AoiDn.FdVac;
		SwAoiDnFdVac(BtnStatus.AoiDn.FdVac);					// 검사부 하 피딩 진공 스위치 "MB005705" IDC_CHK_58
	}
	//if (BtnStatus.AoiDn.PrevPushUp != BtnStatus.AoiDn.PushUp)
	{
		BtnStatus.AoiDn.PrevPushUp = BtnStatus.AoiDn.PushUp;
		SwAoiDnPushUp(BtnStatus.AoiDn.PushUp);				// 검사부 하 제품푸쉬 스위치 "MB005706" IDC_CHK_59 // (토크 진공 스위치) - X
	}
	//if (BtnStatus.AoiDn.PrevTblBlw != BtnStatus.AoiDn.TblBlw)
	{
		BtnStatus.AoiDn.PrevTblBlw = BtnStatus.AoiDn.TblBlw;
		SwAoiDnTblBlw(BtnStatus.AoiDn.TblBlw);				// 검사부 하 테이블 브로워 스위치 "MB005702" IDC_CHK_60
	}
	//if (BtnStatus.AoiDn.PrevTblVac != BtnStatus.AoiDn.TblVac)
	{
		BtnStatus.AoiDn.PrevTblVac = BtnStatus.AoiDn.TblVac;
		SwAoiDnTblVac(BtnStatus.AoiDn.TblVac);				// 검사부 하 테이블 진공 스위치 "MB005707" IDC_CHK_61
	}
	//if (BtnStatus.AoiDn.PrevFdClp != BtnStatus.AoiDn.FdClp)
	{
		BtnStatus.AoiDn.PrevFdClp = BtnStatus.AoiDn.FdClp;
		SwAoiDnFdClp(BtnStatus.AoiDn.FdClp);					// 검사부 하 피딩 클램프 스위치 "MB005709" IDC_CHK_64
	}
	//if (BtnStatus.AoiDn.PrevTqClp != BtnStatus.AoiDn.TqClp)
	{
		BtnStatus.AoiDn.PrevTqClp = BtnStatus.AoiDn.TqClp;
		SwAoiDnTqClp(BtnStatus.AoiDn.TqClp);					// 검사부 하 텐션 클램프 스위치 "MB00570A" IDC_CHK_65
	}
	//if (BtnStatus.AoiDn.PrevMvOne != BtnStatus.AoiDn.MvOne)
	{
		BtnStatus.AoiDn.PrevMvOne = BtnStatus.AoiDn.MvOne;
		SwAoiDnMvOne(BtnStatus.AoiDn.MvOne);					// 검사부 하 한판넬 이송 스위치 "MB440151" IDC_CHK_62
	}
	//if (BtnStatus.AoiDn.PrevLsrPt != BtnStatus.AoiDn.LsrPt)
	{
		BtnStatus.AoiDn.PrevLsrPt = BtnStatus.AoiDn.LsrPt;
		SwAoiDnLsrPt(BtnStatus.AoiDn.LsrPt);					// 검사부 하 레이져마크 스위치 "MB005708" IDC_CHK_63
	}
	//if (BtnStatus.AoiDn.PrevVelSonicBlw != BtnStatus.AoiDn.VelSonicBlw)
	{
		BtnStatus.AoiDn.PrevVelSonicBlw = BtnStatus.AoiDn.VelSonicBlw;
		SwAoiDnVelSonicBlw(BtnStatus.AoiDn.VelSonicBlw);		// 검사부 하 초음파 세정기 속도 스위치 "MB44014F"  IDC_CHK_88 // pDoc->WorkingInfo.LastJob.bAoiDnCleanner
	}
}

void CEngrave::SetSignalAOIUp()
{
	//if (BtnStatus.AoiUp.PrevRelation != BtnStatus.AoiUp.Relation)
	{
		BtnStatus.AoiUp.PrevRelation = BtnStatus.AoiUp.Relation;
		SwAoiUpRelation(BtnStatus.AoiUp.Relation);	// 검사부 상 연동 온/오프 스위치 "MB005601" IDC_CHK_17
	}
	//if (BtnStatus.AoiUp.PrevFdCw != BtnStatus.AoiUp.FdCw)
	{
		BtnStatus.AoiUp.PrevFdCw = BtnStatus.AoiUp.FdCw;
		SwAoiUpFdCw(BtnStatus.AoiUp.FdCw);			// 검사부 상 피딩 정회전 스위치 "MB005603" IDC_CHK_18
	}
	//if (BtnStatus.AoiUp.PrevFdCcw != BtnStatus.AoiUp.FdCcw)
	{
		BtnStatus.AoiUp.PrevFdCcw = BtnStatus.AoiUp.FdCcw;
		SwAoiUpFdCcw(BtnStatus.AoiUp.FdCcw);			// 검사부 상 피딩 역회전 스위치 "MB005604" IDC_CHK_19
	}
	//if (BtnStatus.AoiUp.PrevFdVac != BtnStatus.AoiUp.FdVac)
	{
		BtnStatus.AoiUp.PrevFdVac = BtnStatus.AoiUp.FdVac;
		SwAoiUpFdVac(BtnStatus.AoiUp.FdVac);			// 검사부 상 피딩 진공 스위치 "MB005605" IDC_CHK_20
	}
	//if (BtnStatus.AoiUp.PrevPushUp != BtnStatus.AoiUp.PushUp)
	{
		BtnStatus.AoiUp.PrevPushUp = BtnStatus.AoiUp.PushUp;
		SwAoiUpPushUp(BtnStatus.AoiUp.PushUp);		// 검사부 상 제품푸쉬 스위치 "MB005606" IDC_CHK_21 // (토크 진공 스위치) - X
	}
	//if (BtnStatus.AoiUp.PrevTblBlw != BtnStatus.AoiUp.TblBlw)
	{
		BtnStatus.AoiUp.PrevTblBlw = BtnStatus.AoiUp.TblBlw;
		SwAoiUpTblBlw(BtnStatus.AoiUp.TblBlw);		// 검사부 상 테이블 브로워 스위치 "MB005602" IDC_CHK_22
	}
	//if (BtnStatus.AoiUp.PrevTblVac != BtnStatus.AoiUp.TblVac)
	{
		BtnStatus.AoiUp.PrevTblVac = BtnStatus.AoiUp.TblVac;
		SwAoiUpTblVac(BtnStatus.AoiUp.TblVac);		// 검사부 상 테이블 진공 스위치 "MB005607" IDC_CHK_23
	}
	//if (BtnStatus.AoiUp.PrevFdClp != BtnStatus.AoiUp.FdClp)
	{
		BtnStatus.AoiUp.PrevFdClp = BtnStatus.AoiUp.FdClp;
		SwAoiUpFdClp(BtnStatus.AoiUp.FdClp);			// 검사부 상 피딩 클램프 스위치 "MB005609" IDC_CHK_53
	}
	//if (BtnStatus.AoiUp.PrevTqClp != BtnStatus.AoiUp.TqClp)
	{
		BtnStatus.AoiUp.PrevTqClp = BtnStatus.AoiUp.TqClp;
		SwAoiUpTqClp(BtnStatus.AoiUp.TqClp);			// 검사부 상 텐션 클램프 스위치 "MB00560A" IDC_CHK_54
	}
	//if (BtnStatus.AoiUp.PrevMvOne != BtnStatus.AoiUp.MvOne)
	{
		BtnStatus.AoiUp.PrevMvOne = BtnStatus.AoiUp.MvOne;
		SwAoiUpMvOne(BtnStatus.AoiUp.MvOne);			// 검사부 상 한판넬 이송 스위치  "MB440151" IDC_CHK_24
	}
	//if (BtnStatus.AoiUp.PrevLsrPt != BtnStatus.AoiUp.LsrPt)
	{
		BtnStatus.AoiUp.PrevLsrPt = BtnStatus.AoiUp.LsrPt;
		SwAoiUpLsrPt(BtnStatus.AoiUp.LsrPt);			// 검사부 상 레이져마크 스위치 "MB005608" IDC_CHK_50
	}
}

void CEngrave::SetSignalEngrave()
{
	//if (BtnStatus.Eng.PrevRelation != BtnStatus.Eng.Relation)
	{
		BtnStatus.Eng.PrevRelation = BtnStatus.Eng.Relation;
		SwEngRelation(BtnStatus.Eng.Relation);		// 각인부 연동 온/오프 스위치 IDC_CHK_72
	}
	//if (BtnStatus.Eng.PrevFdCw != BtnStatus.Eng.FdCw)
	{
		BtnStatus.Eng.PrevFdCw = BtnStatus.Eng.FdCw;
		SwEngFdCw(BtnStatus.Eng.FdCw);				// 각인부 피딩 정회전 스위치 IDC_CHK_73
	}
	//if (BtnStatus.Eng.PrevFdCcw != BtnStatus.Eng.FdCcw)
	{
		BtnStatus.Eng.PrevFdCcw = BtnStatus.Eng.FdCcw;
		SwEngFdCcw(BtnStatus.Eng.FdCcw);				// 각인부 피딩 역회전 스위치 IDC_CHK_74
	}
	//if (BtnStatus.Eng.PrevFdVac != BtnStatus.Eng.FdVac)
	{
		BtnStatus.Eng.PrevFdVac = BtnStatus.Eng.FdVac;
		SwEngFdVac(BtnStatus.Eng.FdVac);				// 각인부 피딩 진공 스위치 IDC_CHK_75
	}
	//if (BtnStatus.Eng.PrevPushUp != BtnStatus.Eng.PushUp)
	{
		BtnStatus.Eng.PrevPushUp = BtnStatus.Eng.PushUp;
		SwEngPushUp(BtnStatus.Eng.PushUp);			// 각인부 제품푸쉬 스위치 IDC_CHK_76 // (토크 진공 스위치) - X
	}
	//if (BtnStatus.Eng.PrevTblBlw != BtnStatus.Eng.TblBlw)
	{
		BtnStatus.Eng.PrevTblBlw = BtnStatus.Eng.TblBlw;
		SwEngTblBlw(BtnStatus.Eng.TblBlw);			// 각인부 테이블 브로워 스위치 IDC_CHK_77
	}
	//if (BtnStatus.Eng.PrevTblVac != BtnStatus.Eng.TblVac)
	{
		BtnStatus.Eng.PrevTblVac = BtnStatus.Eng.TblVac;
		SwEngTblVac(BtnStatus.Eng.TblVac);			// 각인부 테이블 진공 스위치 IDC_CHK_78
	}
	//if (BtnStatus.Eng.PrevFdClp != BtnStatus.Eng.FdClp)
	{
		BtnStatus.Eng.PrevFdClp = BtnStatus.Eng.FdClp;
		SwEngFdClp(BtnStatus.Eng.FdClp);				// 각인부 피딩 클램프 스위치 IDC_CHK_82
	}
	//if (BtnStatus.Eng.PrevTqClp != BtnStatus.Eng.TqClp)
	{
		BtnStatus.Eng.PrevTqClp = BtnStatus.Eng.TqClp;
		SwEngTqClp(BtnStatus.Eng.TqClp);				// 각인부 텐션 클램프 스위치 IDC_CHK_83
	}
	//if (BtnStatus.Eng.PrevMvOne != BtnStatus.Eng.MvOne)
	{
		BtnStatus.Eng.PrevMvOne = BtnStatus.Eng.MvOne;
		SwEngMvOne(BtnStatus.Eng.MvOne);				// 각인부 한판넬 이송 스위치  "MB440151" IDC_CHK_79
	}
	//if (BtnStatus.Eng.PrevLsrPt != BtnStatus.Eng.LsrPt)
	{
		BtnStatus.Eng.PrevLsrPt = BtnStatus.Eng.LsrPt;
		SwEngLsrPt(BtnStatus.Eng.LsrPt);				// 각인부 레이져마크 스위치 "" IDC_CHK_81
	}
	//if (BtnStatus.Eng.PrevVelSonicBlw != BtnStatus.Eng.VelSonicBlw)
	{
		BtnStatus.Eng.PrevVelSonicBlw = BtnStatus.Eng.VelSonicBlw;
		SwEngVelSonicBlw(BtnStatus.Eng.VelSonicBlw);		// 각인부 초음파 세정기 스위치 "MB44014E" IDC_CHK_87 pDoc->WorkingInfo.LastJob.bEngraveCleanner
	}
	//if (BtnStatus.Eng.PrevDcRSol != BtnStatus.Eng.DcRSol)
	{
		BtnStatus.Eng.PrevDcRSol = BtnStatus.Eng.DcRSol;
		SwEngDcRSol(BtnStatus.Eng.DcRSol);			// 각인부 댄서롤 상승/하강 스위치 IDC_CHK_80
	}
}

void CEngrave::SetSignalUncoiler()
{
	//if (BtnStatus.Uc.PrevRelation != BtnStatus.Uc.Relation)
	{
		BtnStatus.Uc.PrevRelation = BtnStatus.Uc.Relation;
		SwUcRelation(BtnStatus.Uc.Relation);			// 언코일러 연동 온/오프 스위치 "MB005401" IDC_CHK_25
	}
	//if (BtnStatus.Uc.PrevFdCw != BtnStatus.Uc.FdCw)
	{
		BtnStatus.Uc.PrevFdCw = BtnStatus.Uc.FdCw;
		SwUcFdCw(BtnStatus.Uc.FdCw);					// 언코일러 제품휠 정회전 스위치 "MB00540C" IDC_CHK_26
	}
	//if (BtnStatus.Uc.PrevFdCcw != BtnStatus.Uc.FdCcw)
	{
		BtnStatus.Uc.PrevFdCcw = BtnStatus.Uc.FdCcw;
		SwUcFdCcw(BtnStatus.Uc.FdCcw);				// 언코일러 제품휠 역회전 스위치 "MB00540D" IDC_CHK_27
	}
	//if (BtnStatus.Uc.PrevReelChuck != BtnStatus.Uc.ReelChuck)
	{
		BtnStatus.Uc.PrevReelChuck = BtnStatus.Uc.ReelChuck;
		SwUcReelChuck(BtnStatus.Uc.ReelChuck);		// 언코일러 제품척 클램프 스위치 "MB00540B" IDC_CHK_35
	}
	//if (BtnStatus.Uc.PrevDcRlUpDn != BtnStatus.Uc.DcRlUpDn)
	{
		BtnStatus.Uc.PrevDcRlUpDn = BtnStatus.Uc.DcRlUpDn;
		SwUcDcRlUpDn(BtnStatus.Uc.DcRlUpDn);			// 언코일러 댄서롤 상승/하강 스위치 "MB005402" IDC_CHK_28
	}
	//if (BtnStatus.Uc.PrevReelJoinL != BtnStatus.Uc.ReelJoinL)
	{
		BtnStatus.Uc.PrevReelJoinL = BtnStatus.Uc.ReelJoinL;
		SwUcReelJoinL(BtnStatus.Uc.ReelJoinL);		// 언코일러 제품 이음매(상/좌) 스위치 "MB005405" IDC_CHK_30
	}
	//if (BtnStatus.Uc.PrevReelJoinR != BtnStatus.Uc.ReelJoinR)
	{
		BtnStatus.Uc.PrevReelJoinR = BtnStatus.Uc.ReelJoinR;
		SwUcReelJoinR(BtnStatus.Uc.ReelJoinR);		// 언코일러 제품 이음매(하/우) 스위치 "MB005406" IDC_CHK_37
	}
	//if (BtnStatus.Uc.PrevReelJoinVac != BtnStatus.Uc.ReelJoinVac)
	{
		BtnStatus.Uc.PrevReelJoinVac = BtnStatus.Uc.ReelJoinVac;
		SwUcReelJoinVac(BtnStatus.Uc.ReelJoinVac);	// 언코일러 제품 이음매 진공 스위치 "MB00540F" IDC_CHK_38
	}
	//if (BtnStatus.Uc.PrevPprChuck != BtnStatus.Uc.PprChuck)
	{
		BtnStatus.Uc.PrevPprChuck = BtnStatus.Uc.PprChuck;
		SwUcPaperChuck(BtnStatus.Uc.PprChuck);		// 언코일러 간지척 클램프 스위치 "MB005408" IDC_CHK_31
	}
	//if (BtnStatus.Uc.PrevPprCw != BtnStatus.Uc.PprCw)
	{
		BtnStatus.Uc.PrevPprCw = BtnStatus.Uc.PprCw;
		SwUcPaperCw(BtnStatus.Uc.PprCw);				// 언코일러 간지휠 정회전 스위치 "MB005409" IDC_CHK_32
	}
	//if (BtnStatus.Uc.PrevPprCcw != BtnStatus.Uc.PprCcw)
	{
		BtnStatus.Uc.PrevPprCcw = BtnStatus.Uc.PprCcw;
		SwUcPaperCcw(BtnStatus.Uc.PprCcw);			// 언코일러 간지휠 역회전 스위치 "MB00540A" IDC_CHK_39
	}
	//if (BtnStatus.Uc.PrevClRlUpDn != BtnStatus.Uc.ClRlUpDn)
	{
		BtnStatus.Uc.PrevClRlUpDn = BtnStatus.Uc.ClRlUpDn;
		SwUcClRlUpDn(BtnStatus.Uc.ClRlUpDn);			// 언코일러 클린롤러 상승/하강 스위치 "MB005403" IDC_CHK_29
	}
	//if (BtnStatus.Uc.PrevClRlPshUpDn != BtnStatus.Uc.ClRlPshUpDn)
	{
		BtnStatus.Uc.PrevClRlPshUpDn = BtnStatus.Uc.ClRlPshUpDn;
		SwUcClRlPshUpDn(BtnStatus.Uc.ClRlPshUpDn);	// 언코일러 클린롤러누름 상승/하강 스위치 "MB005404" IDC_CHK_36
	}
}

// End SetSysSignal()

// Start for SetSysData()

BOOL CEngrave::SetSysData()
{
	if (!IsRunning())
	{
		SetOpInfo();
		SetInfo();
		SetTotRatio();
		SetStTime();
		SetRunTime();
		SetEdTime();
		SetStripRatio();
		SetDef();
		Set2DReader();
		SetEngInfo();
		SetFdInfo();
		SetAoiInfo();
		SetMkInfo();
		SetMkInfoLf();
		SetMkInfoRt();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetOpInfo()
{
	SetOpName();
	SetDualTest();
	SetSampleTest();
	SetSampleShotNum();
	SetTestMode();
	SetRecoilerCcw();
	SetUncoilerCcw();
	SetAlignMethode();
	SetDoorRecoiler();
	SetDoorAoiUp();
	SetDoorAoiDn();
	SetDoorMk();
	SetDoorEngrave();
	SetDoorUncoiler();
	SetSaftyMk();
	SetCleannerAoiUp();
	SetCleannerAoiDn();
	SetUltraSonicAoiDn();
	SetUltraSonicEngrave();
	SetTotReelLen();
	SetOnePnlLen();
	SetTempPause();
	SetTempStopLen();
	SetLotCut();
	SetLotCutLen();
	SetLotCutPosLen();
	SetLmtTotYld();
	SetLmtPatlYld();
	SetStripOutRatio();
	SetCustomNeedRatio();
	SetNumRangeFixDef();
	SetFixDef();
	SetNumContFixDef();
	SetUltraSonicStTim();
	SetEngItsCode();
}

void CEngrave::SetInfo()
{
	SetModelUpName();
	//SetModelDnName();
	SetLotUpName();
	SetLotDnName();
	SetLayerUpName();
	SetLayerDnName();
	//SetOrderNum();
	SetShotNum();
	SetTotOpRto();
	SetTotVel();
	SetPartVel();
	SetMkDoneLen();
	SetAoiDnDoneLen();
	SetAoiUpDoneLen();
	SetLotSerial();
	SetMkVerfyLen();
}

void CEngrave::SetTotRatio()
{
	SetDefNumUp();
	SetDefRtoUp();
	SetGoodNumUp();
	SetGoodRtoUp();
	SetTestNumUp();
	SetDefNumDn();
	SetDefRtoDn();
	SetGoodNumDn();
	SetGoodRtoDn();
	SetTestNumDn();
	SetDefNumTot();
	SetDefRtoTot();
	SetGoodNumTot();
	SetGoodRtoTot();
	SetTestNumTot();
}

void CEngrave::SetStTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if(pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetStTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotStTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetRunTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetRunTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotRunTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetEdTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetEdTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotEdTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetStripRatio()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nGood = 0, nBad = 0, nTot = 0, nStTot = 0, nSum = 0, nVal[2][4];
	int nMer[MAX_STRIP];
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!pDoc->m_pReelMapUp)
		return;

	if (bDualTest)
	{
		if (!pDoc->m_pReelMapDn || !pDoc->m_pReelMapAllUp)
			return;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 4; k++)
			nVal[i][k] = 0;
	}

	// < 스트립 별 수율 >
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;
	nStTot = nTot / 4;

	// 상면
	nSum = 0;
	nVal[0][0] = pDoc->m_pReelMapUp->GetDefStrip(0);
	nSum += nVal[0][0];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][0]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_1LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][1] = pDoc->m_pReelMapUp->GetDefStrip(1);
	nSum += nVal[0][1];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][1]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_2LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][2] = pDoc->m_pReelMapUp->GetDefStrip(2);
	nSum += nVal[0][2];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][2]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_3LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][3] = pDoc->m_pReelMapUp->GetDefStrip(3);
	nSum += nVal[0][3];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][3]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_4LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	if (nTot > 0)
		dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_AllLnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);


	if (bDualTest)
	{
		// 하면
		nSum = 0;
		nVal[1][0] = pDoc->m_pReelMapDn->GetDefStrip(0);
		nSum += nVal[1][0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_1LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][1] = pDoc->m_pReelMapDn->GetDefStrip(1);
		nSum += nVal[1][1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_2LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][2] = pDoc->m_pReelMapDn->GetDefStrip(2);
		nSum += nVal[1][2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_3LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][3] = pDoc->m_pReelMapDn->GetDefStrip(3);
		nSum += nVal[1][3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_4LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_AllLnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		// 상면 + 하면
		nSum = 0;
		nMer[0] = pDoc->m_pReelMapAllUp->GetDefStrip(0);
		nSum += nMer[0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_1LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[1] = pDoc->m_pReelMapAllUp->GetDefStrip(1);
		nSum += nMer[1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_2LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[2] = pDoc->m_pReelMapAllUp->GetDefStrip(2);
		nSum += nMer[2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_3LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[3] = pDoc->m_pReelMapAllUp->GetDefStrip(3);
		nSum += nMer[3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_4LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_AllLnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDef()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nNum = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CReelMap* pReelMap=NULL;

	//if (bDualTest)
	//{
	//	switch (pView->m_nSelRmap)
	//	{
	//	case RMAP_UP:
	//		pReelMap = pDoc->m_pReelMapUp;
	//		break;
	//	case RMAP_DN:
	//		pReelMap = pDoc->m_pReelMapDn;
	//		break;
	//	case RMAP_ALLUP:
	//		pReelMap = pDoc->m_pReelMapAllUp;
	//		break;
	//	case RMAP_ALLDN:
	//		pReelMap = pDoc->m_pReelMapAllDn;
	//		break;
	//	}
	//}
	//else
	//	pReelMap = pDoc->m_pReelMapUp;


	switch (pView->m_nSelRmap)
	{
	case RMAP_UP:
		if (pDoc->m_pReelMapUp)
			pReelMap = pDoc->m_pReelMapUp;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapUp is NULL."));
		}
		break;
	case RMAP_DN:
		if (pDoc->m_pReelMapDn)
			pReelMap = pDoc->m_pReelMapDn;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapDn is NULL."));
		}
		break;
	case RMAP_ALLUP:
		if (pDoc->m_pReelMapAllUp)
			pReelMap = pDoc->m_pReelMapAllUp;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapAllUp is NULL."));
		}
		break;
	case RMAP_ALLDN:
		if (pDoc->m_pReelMapAllDn)
			pReelMap = pDoc->m_pReelMapAllDn;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapAllDn is NULL."));
		}
		break;
	case RMAP_INNER_UP:
		if (pDoc->m_pReelMapInnerUp)
			pReelMap = pDoc->m_pReelMapInnerUp;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerUp is NULL."));
		}
		break;
	case RMAP_INNER_DN:
		if (pDoc->m_pReelMapInnerDn)
			pReelMap = pDoc->m_pReelMapInnerDn;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerDn is NULL."));
		}
		break;
	case RMAP_INNER_ALLUP:
		if (pDoc->m_pReelMapInnerAllUp)
			pReelMap = pDoc->m_pReelMapInnerAllUp;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerAllUp is NULL."));
		}
		break;
	case RMAP_INNER_ALLDN:
		if (pDoc->m_pReelMapInnerAllDn)
			pReelMap = pDoc->m_pReelMapInnerAllDn;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerAllDn is NULL."));
		}
		break;
	case RMAP_ITS:
		if (pDoc->m_pReelMapIts)
			pReelMap = pDoc->m_pReelMapIts;
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapIts is NULL."));
		}
		break;
	}

	if(pReelMap)
	{
		nNum = pReelMap->GetDefNum(DEF_OPEN); // IDC_STC_DEF_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumOpen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_SHORT); // IDC_STC_DEF_SHORT
		SocketData.nMsgID = _stItemInx::_DefNumShort;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_USHORT); // IDC_STC_DEF_U_SHORT
		SocketData.nMsgID = _stItemInx::_DefNumUshort;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_SPACE); // IDC_STC_DEF_SPACE
		SocketData.nMsgID = _stItemInx::_DefNumLnW;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EXTRA); // IDC_STC_DEF_EXTRA
		SocketData.nMsgID = _stItemInx::_DefExtr;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PROTRUSION); // IDC_STC_DEF_PROT
		SocketData.nMsgID = _stItemInx::_DefNumProt;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PINHOLE); // IDC_STC_DEF_P_HOLE
		SocketData.nMsgID = _stItemInx::_DefNumPhole;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PAD); // IDC_STC_DEF_PAD
		SocketData.nMsgID = _stItemInx::_DefNumPad;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_OPEN); // IDC_STC_DEF_H_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumHopen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_MISS); // IDC_STC_DEF_H_MISS
		SocketData.nMsgID = _stItemInx::_DefNumHmiss;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_POSITION); // IDC_STC_DEF_H_POS
		SocketData.nMsgID = _stItemInx::_DefNumHpos;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_DEFECT); // IDC_STC_DEF_H_DEF
		SocketData.nMsgID = _stItemInx::_DefNumHdef;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_NICK); // IDC_STC_DEF_NICK
		SocketData.nMsgID = _stItemInx::_DefNumNick;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_POI); // IDC_STC_DEF_POI
		SocketData.nMsgID = _stItemInx::_DefNumPoi;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_OPEN); // IDC_STC_DEF_VH_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumVhOpen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_MISS); // IDC_STC_DEF_VH_MISS
		SocketData.nMsgID = _stItemInx::_DefNumVhMiss;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_POSITION); // IDC_STC_DEF_VH_POS
		SocketData.nMsgID = _stItemInx::_DefNumVhPos;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_DEF); // IDC_STC_DEF_VH_DEF
		SocketData.nMsgID = _stItemInx::_DefNumVhd;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_LIGHT); // IDC_STC_DEF_LIGHT
		SocketData.nMsgID = _stItemInx::_DefNumLight;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_NICK);
		SocketData.nMsgID = _stItemInx::_DefNumEnick;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_PROT);
		SocketData.nMsgID = _stItemInx::_DefNumEprot;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_SPACE);
		SocketData.nMsgID = _stItemInx::_DefNumEspace;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_USER_DEFINE_1);
		SocketData.nMsgID = _stItemInx::_DefNumUdd1;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_NARROW);
		SocketData.nMsgID = _stItemInx::_DefNumNrw;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_WIDE);
		SocketData.nMsgID = _stItemInx::_DefNumWide;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);
	}
}

void CEngrave::Set2DReader()
{
	Set2DEngLen();
	Set2DAoiLen();
	Set2DMkLen();
	Set2DMoveVel();
	Set2DMoveAcc();
	Set2DOneShotLen();
}

void CEngrave::SetEngInfo()
{
	SetEngLeadPitch();
	SetEngPushOffLen();
	SetEngTqVal();
	SetEngAoiLen();
	SetEngFdDiffMax();
	SetEngFdDiffRng();
	SetEngFdDiffNum();
	SetEngBuffInitPos();
	SetEngBuffCurrPos();
}

void CEngrave::SetFdInfo()
{
	SetFdVel();
	SetFdAcc();
	SetOnePnlVel();
	SetOnePnlAcc();
	SetFdDiffMax();
	SetFdDiffRng();
	SetFdDiffNum();
}

void CEngrave::SetAoiInfo()
{
	SetAoiLeadPitch();
	SetAoiPushOffLen();
	SetAoiTqVal();
	SetAoiBuffShotNum();
	SetAoiMkLen();
}

void CEngrave::SetMkInfo()
{
	SetMkLeadPitch();
	SetMkPushOffLen();
	SetMkTqVal();
	SetMkBuffInitPos();
	SetMkBuffCurrPos();
	SetMkNumLf();
	SetMkNumRt();
	SetMkMaxNumLf();
	SetMkMaxNumRt();
}

void CEngrave::SetMkInfoLf()
{
	SetMkInitPosLf();
	SetMkInitVelLf();
	SetMkInitAccLf();
	SetMkFnlPosLf();
	SetMkFnlVelLf();
	SetMkFnlAccLf();
	SetMkFnlTqLf();
	SetMkHgtPosX1Lf();
	SetMkHgtPosY1Lf();
	SetMkHgtPosX2Lf();
	SetMkHgtPosY2Lf();
	SetMkHgtPosX3Lf();
	SetMkHgtPosY3Lf();
	SetMkHgtPosX4Lf();
	SetMkHgtPosY4Lf();
	SetMkHgtAvgPosLf();
}

void CEngrave::SetMkInfoRt()
{
	SetMkInitPosRt();
	SetMkInitVelRt();
	SetMkInitAccRt();
	SetMkFnlPosRt();
	SetMkFnlVelRt();
	SetMkFnlAccRt();
	SetMkFnlTqRt();
	SetMkHgtPosX1Rt();
	SetMkHgtPosY1Rt();
	SetMkHgtPosX2Rt();
	SetMkHgtPosY2Rt();
	SetMkHgtPosX3Rt();
	SetMkHgtPosY3Rt();
	SetMkHgtPosX4Rt();
	SetMkHgtPosY4Rt();
	SetMkHgtAvgPosRt();
}

// SetOpInfo()
void CEngrave::SetOpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetDualTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DualTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispContRun()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DispContRun;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDispContRun ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispLotEnd()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DispLotEnd;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDispLotEnd ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_SampleTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bSampleTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_SampleShotNum;
	SocketData.nData1 = _ttoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	SendCommand(SocketData);
}

void CEngrave::SetTestMode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _stSigInx::_TestMode;
	SocketData.nData1 = pDoc->GetTestMode(); // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	//SocketData.nData1 = pDoc->WorkingInfo.LastJob.nTestMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	SendCommand(SocketData);
}

void CEngrave::SetRecoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_RecoilerCcw;						// OneMetal : TRUE -> SetTwoMetal(FALSE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bOneMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUncoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UncoilerCcw;						// TwoMetal : TRUE -> SetTwoMetal(TRUE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTwoMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetAlignMethode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _stSigInx::_AlignMethode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nAlignMethode; // TWO_POINT, FOUR_POINT
	SendCommand(SocketData);
}

void CEngrave::SetDoorRecoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorRecoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorUncoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorUncoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSaftyMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_SaftyMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkSftySen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CleannerAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CleannerAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UltraSonicAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UltraSonicEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTotReelLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);
}

void CEngrave::SetTempPause()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUse380mm()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Use380mm;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUse380mm ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTempStopLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCut()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotCut;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bLotSep ? 1 : 0;	// pDoc->m_pReelMap->m_bUseLotSep
	SendCommand(SocketData);
}

void CEngrave::SetLotCutLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCutPosLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotCutPosLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotCutPosLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtTotYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LmtTotYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtTotYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtPatlYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LmtPatlYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtPatlYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetStripOutRatio()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_StripOutRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sStripOutRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetCustomNeedRatio()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_CustomNeedRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sCustomNeedRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetNumRangeFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_NumRangeFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumRangeFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FixDef;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bContFixDef ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetNumContFixDef()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_NumContFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumContFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicStTim()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_UltraSonicStTim;
	StringToChar(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngItsCode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngItsCode;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngItsCode, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetInfo()
void CEngrave::SetModelUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_ModelUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModelUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

//void CEngrave::SetModelDnName()
//{
//	if (!pDoc)
//		return;
//
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetData;
//	char cData[BUFFER_DATA_SIZE];
//
//	SocketData.nMsgID = _stItemInx::_ModelDnName;
//	StringToChar(pDoc->WorkingInfo.LastJob.sModelDn, cData);
//	sprintf(SocketData.strData, "%s", cData);
//	SendCommand(SocketData);
//}

void CEngrave::SetLotUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLayerUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LayerUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLayerDnName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LayerDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLoadMstInfo()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_LoadMstInfo;
	SendCommand(SocketData);
}

void CEngrave::SetOrderNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_OrderNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveOrderNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_ShotNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveLastShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotOpRto()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_TotOpRto;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%d"), (int)(dFdTotLen / _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 100.0));
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotVel()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_TotVel;
	sVal.Format(_T("%.1f"), pView->GetTotVel());
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetPartVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_PartVel;
	StringToChar(pDoc->WorkingInfo.LastJob.sPartialSpd, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkDoneLen()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_MkDoneLen;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%.2f"), dFdTotLen / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiDnDoneLen()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_AoiDnDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiDnFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiUpDoneLen()
{
	if (!pView)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_AoiUpDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiUpFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotSerial()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotSerial;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSerial, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkVerfyLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkVerfyLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sVerifyLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetTotRatio()
void CEngrave::SetDefNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;

	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _stItemInx::_DefNumUp;
	SocketData.nData1 = nBad;
	SendCommand(SocketData);
}

void CEngrave::SetDefRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_DefRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetGoodNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	SocketData.nMsgID = _stItemInx::_GoodNumUp;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);
}

void CEngrave::SetGoodRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_GoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetTestNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _stItemInx::_TestNumUp;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);
}

void CEngrave::SetDefNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{		
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_DefNumDn;
		SocketData.nData1 = nBad;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_DefRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면

		SocketData.nMsgID = _stItemInx::_GoodNumDn;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_GoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_TestNumDn;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{		
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_DefNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_DefRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체

		SocketData.nMsgID = _stItemInx::_GoodNumTot;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_GoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_TestNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

// Set2DReader()
void CEngrave::Set2DEngLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DEngLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DEngLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMkLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMoveVel;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMoveAcc;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DOneShotLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DOneShotLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DOneShotRemainLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetEngInfo()
void CEngrave::SetEngLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngAoiLen;
	pDoc->WorkingInfo.Motion.sEngAoiLen = pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist;
	StringToChar(pDoc->WorkingInfo.Motion.sEngAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffMax;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffMax, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffRng;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffRng, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffNum;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffInitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffCurrPos()
{
	if (!pDoc || !pDoc->m_pMpeData)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	CString str;
	double dBufEnc = (double)pDoc->m_pMpeData[1][1] / 1000.0;	// 각인부 버퍼 엔코더 값(단위 mm * 1000)
	str.Format(_T("%.1f"), dBufEnc);
	pDoc->WorkingInfo.Motion.sEngBuffCurrPos = str;

	SocketData.nMsgID = _stItemInx::_EngBuffCurrPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffCurrPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffJogCw;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffJogCw ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffJogCcw;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffJogCcw ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffHome()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffOrgMv;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffHomming ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffHomeDone()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffOrgMvDone;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffHommingDone ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitMove()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosMv;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitMv ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitMoveDone()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosMvDone;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitMvDone ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitPosSave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosSave;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitPosSave ? 1 : 0;
	SendCommand(SocketData);
}


// SetFdInfo()
void CEngrave::SetFdVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_FdVel;
	//StringToChar(pDoc->WorkingInfo.Motion.sMkFdVel, cData);
	StringToChar(pDoc->WorkingInfo.Motion.sMkJogVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetFdAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_FdAcc;
	//StringToChar(pDoc->WorkingInfo.Motion.sMkFdAcc, cData);
	StringToChar(pDoc->WorkingInfo.Motion.sMkJogAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlVel;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlVel());
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlAcc;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlAcc());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffMax;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrLmt());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffRng;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrRng());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffNum;
	sprintf(SocketData.strData, "%d", pDoc->GetFdErrNum());
	SendCommand(SocketData);
}

// SetAoiInfo()
void CEngrave::SetAoiLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiBuffShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiBuffShotNum;
	StringToChar(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetMkInfo()
void CEngrave::SetMkLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sMkTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sStBufPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffCurrPos() // (Engrave)
{
	if (!pDoc || !pDoc->m_pMpeData)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkBuffCurrPos;
	double dBufEnc = (double)pDoc->m_pMpeData[1][1] / 1000.0;	// 각인부 버퍼 엔코더 값(단위 mm * 1000)
																//sprintf(SocketData.strData, "%.1f", dBufEnc);
	SocketData.fData1 = (float)dBufEnc;
	SendCommand(SocketData);
}

void CEngrave::SetMkNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkNumLf;
	SocketData.nData1 = pDoc->GetMkCntL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntL());
	SendCommand(SocketData);
}

void CEngrave::SetMkNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkNumRt;
	SocketData.nData1 = pDoc->GetMkCntR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntR());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkMaxNumLf;
	SocketData.nData1 = pDoc->GetMkLimitL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitL());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkMaxNumRt;
	SocketData.nData1 = pDoc->GetMkLimitR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitR());
	SendCommand(SocketData);
}

// SetMkInfoLf()
void CEngrave::SetMkInitPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlTqLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtAvgPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtAvgPosLf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetAverDist1());
	SendCommand(SocketData);
}

// SetMkInfoRt()
void CEngrave::SetMkInitPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _ItemInx::_MkInitVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlTqRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtAvgPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtAvgPosRt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetAverDist2());
	SendCommand(SocketData);
}

// End for SetSysData()


// Start Switch

// Main
void CEngrave::SwReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Main.Ready = bOn;
	SocketData.nMsgID = _SigInx::_Ready;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsReady;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Main.Run = bOn;
	SocketData.nMsgID = _SigInx::_Run;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRun;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Reset;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsReset;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Stop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsStop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAuto(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Auto;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAuto(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsAuto;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwManual(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Manual;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwManual(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsManual;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Torque Motor
void CEngrave::SwMkTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MkTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMkTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_AoiTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsAoiTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsEngTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Induction Motor
void CEngrave::SwRcInductionCcw(BOOL bOn)	// SetOneMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CcwModRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcInductionCcw(BOOL bOn)	// SetOneMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCcwModRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcInductionCcw(BOOL bOn)	// SetTwoMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CcwModUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcInductionCcw(BOOL bOn)	// SetTwoMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCcwModUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Core 150mm
void CEngrave::SwRcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Core150Re;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCore150Re;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Core150Un;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCore150Un;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Recoiler
void CEngrave::SwRcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpLfRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpLfRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpRtRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpRtRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteVacRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteVacRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinder(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRewinder(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDoRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinderReelPaper(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdPprRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRewinderReelPaper(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdPprRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Punching
void CEngrave::SwMkRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblBlw(BOOL bOn)	
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// AOIDn
void CEngrave::SwAoiDnRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_VelClrSonicAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsVelClrSonicAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TestAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTestAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ResetAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsResetAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotEndAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLotEndAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}



// AOIUp
void CEngrave::SwAoiUpRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiUpTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TestAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTestAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ResetAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsResetAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotEndAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLotEndAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Engrave
void CEngrave::SwEngRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_VelClrSonicEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsVelClrSonicEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Uncoiler
void CEngrave::SwUcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpLfUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpLfUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpRtUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpRtUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteVacUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteVacUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcClRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlPshUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollPushUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcClRlPshUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollPushUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Etc
void CEngrave::SwAoiEmg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EmgAoi;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiEmg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsEmgAoi;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// EngraveAutoSequence

void CEngrave::SwEngAutoInit(BOOL bOn) // 각인부 초기화(Reset)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoInit;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoInit(BOOL bOn) // 각인부 초기화(Reset)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoInit;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoMkSt(BOOL bOn) // 각인부 마킹시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.MkSt = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqMkSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoMkSt(BOOL bOn) // 각인부 마킹시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.MkSt = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqMkSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoOnMking(BOOL bOn) // 각인부 마킹중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.OnMking = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqOnMkIng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoOnMking(BOOL bOn) // 각인부 마킹중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.OnMking = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqOnMkIng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoMkDone(BOOL bOn) // 각인부 마킹완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.MkDone = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqMkDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoMkDone(BOOL bOn) // 각인부 마킹완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.MkDone = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqMkDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAuto2dReadSt(BOOL bOn) // 각인부 2D Read 시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.Read2dSt = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeq2dReadSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAuto2dReadSt(BOOL bOn) // 각인부 2D Read 시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.Read2dSt = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeq2dReadSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoOnReading2d(BOOL bOn) // 각인부 Read중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.OnRead2d = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqOnReading2d;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoOnReading2d(BOOL bOn) // 각인부 Read중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.OnRead2d = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqOnReading2d;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAuto2dReadDone(BOOL bOn) // 각인부 2D Read 완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.Read2dDone = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeq2dReadDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAuto2dReadDone(BOOL bOn) // 각인부 2D Read 완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.EngAuto.Read2dDone = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeq2dReadDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Set Engrave Data
void CEngrave::SetEngraveAoiDist()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngraveFdPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// DlgMyMsg
void CEngrave::SetMyMsgYes()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Msg.Yes = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgYes;
	SocketData.nData1 =1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgYes()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.Msg.IsYes = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgYes;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::SetMyMsgNo()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Msg.No = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgNo;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgNo()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.Msg.IsNo = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgNo;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::SetMyMsgOk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Msg.No = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgOk;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgOk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//BtnStatus.Msg.IsNo = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgOk;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

// Disp
void CEngrave::SetDispReady(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Disp.Init();
	BtnStatus.Disp.Ready = TRUE;
	SocketData.nMsgID = _SigInx::_DispReady;
	SocketData.nData1 = BtnStatus.Disp.Ready > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispReady;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispRun(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Disp.Init();
	BtnStatus.Disp.Run = TRUE;
	SocketData.nMsgID = _SigInx::_DispRun;
	SocketData.nData1 = BtnStatus.Disp.Run > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispRun;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispStop(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Disp.Init();
	BtnStatus.Disp.Stop = TRUE;
	SocketData.nMsgID = _SigInx::_DispStop;
	SocketData.nData1 = BtnStatus.Disp.Stop > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispStop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispDualSample(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Disp.Init();
	BtnStatus.Disp.DualSample = TRUE;
	SocketData.nMsgID = _SigInx::_DispDualSample;
	SocketData.nData1 = BtnStatus.Disp.DualSample > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispDualSample(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispDualSample;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispSingleSample(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Disp.Init();
	BtnStatus.Disp.SingleSample = TRUE;
	SocketData.nMsgID = _SigInx::_DispSingleSample;
	SocketData.nData1 = BtnStatus.Disp.SingleSample > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispSingleSample(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispSingleSample;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispDualTest(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Disp.Init();
	BtnStatus.Disp.DualTest = TRUE;
	SocketData.nMsgID = _SigInx::_DispDualTest;
	SocketData.nData1 = BtnStatus.Disp.DualTest > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispDualTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispDualTest;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispSingleTest(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	BtnStatus.Disp.Init();
	BtnStatus.Disp.SingleTest = TRUE;
	SocketData.nMsgID = _SigInx::_DispSingleTest;
	SocketData.nData1 = BtnStatus.Disp.SingleTest > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispSingleTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispSingleTest;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


//// Disp
//void CEngrave::SetDispReady(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	BtnStatus.Disp.Ready = bOn;
//	SocketData.nMsgID = _SigInx::_DispReady;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::IsSetDispReady(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	SocketData.nMsgID = _SigInx::_IsDispReady;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::SetDispRun(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	BtnStatus.Disp.Run = bOn;
//	SocketData.nMsgID = _SigInx::_DispRun;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::IsSetDispRun(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	SocketData.nMsgID = _SigInx::_IsDispRun;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::SetDispStop(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	BtnStatus.Disp.Stop = bOn;
//	SocketData.nMsgID = _SigInx::_DispStop;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::IsSetDispStop(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	SocketData.nMsgID = _SigInx::_IsDispStop;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}

// End Switch

// Alarm

void CEngrave::SetAlarm(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stAlarmInx::_Alarm;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::IsSetAlarm(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stAlarmInx::_IsAlarm;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMsgBox(CString sMsg, int nType)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stMsgBoxInx::_MsgBox;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SocketData.nData1 = nType;
	SendCommand(SocketData);
}

void CEngrave::IsSetMsgBox(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stMsgBoxInx::_IsMsgBox;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// On Running Auto

BOOL CEngrave::UpdateWorking()
{
	SetTotOpRto();		// 전체진행율
						// 로트진행율
	SetTotVel();		// 전체속도
	SetPartVel();		// 구간속도
	SetMkDoneLen();		// 마킹부 : Distance (FdDone) [M]
	SetAoiDnDoneLen();	// 검사부(하) : Distance (FdDone) [M]
	SetAoiUpDoneLen();	// 검사부(상) : Distance (FdDone) [M]
						// 각인부 : Distance (FdDone) [M]

	return TRUE;
}

BOOL CEngrave::UpdateRst()
{
	UpdateTotRatio();
	UpdateStripRatio();

	return TRUE;
}

BOOL CEngrave::UpdateTotRatio()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	// 상면
	SetDefNumUp();			// IDC_STC_DEFECT_NUM
	SetDefRtoUp();			// IDC_STC_DEFECT_RATIO
	SetGoodNumUp();			// IDC_STC_GOOD_NUM
	SetGoodRtoUp();			// IDC_STC_GOOD_RATIO
	SetTestNumUp();			// IDC_STC_TOTAL_NUM

	if (bDualTest)
	{
		// 하면
		SetDefNumDn();		// IDC_STC_DEFECT_NUM_DN
		SetDefRtoDn();		// IDC_STC_DEFECT_RATIO_DN
		SetGoodNumDn();		// IDC_STC_GOOD_NUM_DN
		SetGoodRtoDn();		// IDC_STC_GOOD_RATIO_DN
		SetTestNumDn();		// IDC_STC_TOTAL_NUM_DN

							// 전체
		SetDefNumTot();		// IDC_STC_DEFECT_NUM_ALL
		SetDefRtoTot();		// IDC_STC_DEFECT_RATIO_ALL
		SetGoodNumTot();	// IDC_STC_GOOD_NUM_ALL
		SetGoodRtoTot();	// IDC_STC_GOOD_RATIO_ALL
		SetTestNumTot();	// IDC_STC_TOTAL_NUM_ALL
	}

	return TRUE;
}

BOOL CEngrave::UpdateStripRatio()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	return TRUE;
}

BOOL CEngrave::UpdateDef()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	return TRUE;
}


void CEngrave::SwMenu01UpdateWorking(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UpdateWork;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::IsSwMenu01UpdateWorking(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsUpdateWork;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMenu01DispDefImg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DispDefImg;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::Set2DOffsetInitPosMove(BOOL bOn)
{
	if (!pDoc)
		return;

	BtnStatus.SettingEng.OffsetInitPosMove = bOn;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _stSigInx::_2DOffsetInitPosMove;
	SocketData.nData1 = BtnStatus.SettingEng.OffsetInitPosMove ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::GetSignal2dEng(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;
	long lData;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_2DOffsetInitPos:
			m_bRcvSig[_SigInx::_2DOffsetInitPos] = TRUE;
//#ifdef USE_MPE
//			lData = (long)(pDoc->GetOffsetInitPos() * 1000.0); // WorkingInfo.Motion.sOffsetInitPos
//			pView->MpeWrite(_T("ML44046"), lData);	// 각인부, 검사부, 마킹부 offset 이송 값 (단위 mm * 1000)
//#endif
			break;

		case _SigInx::_2DOffsetInitPosMove:
			BtnStatus.SettingEng.OffsetInitPosMove = (SockData.nData1 > 0) ? TRUE : FALSE;
			if (BtnStatus.SettingEng.OffsetInitPosMove)
			{
				m_bTIM_MPE_OFFSET_INITPOS_MOVE = TRUE;
				SetTimer(TIM_MPE_OFFSET_INITPOS_MOVE, 100, NULL);
				m_bRcvSig[_SigInx::_2DOffsetInitPosMove] = TRUE;
//#ifdef USE_MPE
//				if (pView->m_pMpe)
//					pView->MpeWrite(_T("MB44013F"), 1); // 각인부, 검사부, 마킹부 offset 이송 ON(PC가 On시키고, PLC가 확인하고 Off시킴)
//#endif
			}
			break;
		}
	}
}

CString CEngrave::CharToString(char *szStr)
{
	CString strRet;

	int nLen = strlen(szStr) + sizeof(char);
	wchar_t *tszTemp = NULL;
	tszTemp = new WCHAR[nLen];

	MultiByteToWideChar(CP_ACP, 0, szStr, -1, tszTemp, nLen * sizeof(WCHAR));

	strRet.Format(_T("%s"), (CString)tszTemp);
	if (tszTemp)
	{
		delete[] tszTemp;
		tszTemp = NULL;
	}
	return strRet;
}
