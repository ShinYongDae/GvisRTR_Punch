// Smac.cpp : implementation file
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "Smac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc *pDoc;
extern CGvisRTR_PunchView *pView;

/////////////////////////////////////////////////////////////////////////////
// CSmac

CSmac::CSmac(CWnd* pParent /*=NULL*/)
{
	m_pParent = pParent;
	m_bRs232Run = FALSE;
	CWnd::Create(NULL,NULL, WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE, CRect(0,0,0,0), pParent, 0);

	//m_bTimerStop = FALSE;
	m_bReturnCmdEnd = FALSE;
	m_bRunTimerCheckEnd = FALSE;

	m_nCh = -1;
}

CSmac::~CSmac()
{
	Close();
}


BEGIN_MESSAGE_MAP(CSmac, CWnd)
	//{{AFX_MSG_MAP(CSmac)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECEIVE_RS232, OnReceiveRs232)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSmac message handlers

void CSmac::Init(int nCh)
{
	m_nCh = nCh;
	m_Rs232.SetHwnd(this->GetSafeHwnd());
	m_Rs232.m_nPort = (BYTE)_tstoi(pDoc->WorkingInfo.VoiceCoil[m_nCh].sPort);
	m_Rs232.m_dwBaudRate = (DWORD)_tstoi(pDoc->WorkingInfo.VoiceCoil[m_nCh].sBaudRate);
	m_Rs232.m_bCR = pDoc->WorkingInfo.VoiceCoil[m_nCh].bCr;
	m_Rs232.m_bLF = pDoc->WorkingInfo.VoiceCoil[m_nCh].bLf;
	m_bRs232Run = m_Rs232.OpenComPort();

#ifdef USE_SMAC
	if(!m_bRs232Run)
	{
		CString sMsg;
		sMsg.Format(_T("보이스코일이 초기화되지 않았습니다.(COM%d)"), m_Rs232.m_nPort);
//		pView->MsgBox(_T("보이스코일이 초기화되지 않았습니다.");
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
	}
#endif
	m_sRcvRs232 = _T("");
}

void CSmac::Close()
{
	if(m_bRs232Run)
	{
		m_Rs232.DestroyRs232();
		m_bRs232Run = FALSE;
	}
}

BOOL CSmac::Send(CString str)
{
	int nLen = str.GetLength();
	
	char* pRtn = NULL;
	char* cSend = new char[nLen+1];
	strcpy(cSend, pRtn=StringToChar(str));
	BOOL bRtn = m_Rs232.WriteRs232Block(cSend, nLen);
	delete cSend;

	if(pRtn)
		delete pRtn;

	return TRUE;
}

void CSmac::SearchHomeSmac()
{
	int nCamNum = VOICE_COIL_FIRST_CAM;

#ifdef USE_SMAC
	CString strSend, sMsg;
	strSend.Format(_T("MS220")); // Homming
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	Wait(30);
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail Homming Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
	}
	//SetWaitSmacCmdEnd(_T("OK"));
#endif
}


double CSmac::GetSmacPosition()
{
	double dGetSmacPos=0.0;
	int nPos1, nPos2, nGetSmacPos;
	CString strSend, strVal, sMsg;
	strSend.Format(_T("MS200")); // Get Position.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if(!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail GetPosition Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return 0.0;
	}

	strVal = m_strReceiveVoiceCoil;

	nPos1 = strVal.Find(_T("OK"));
	nPos2 = strVal.GetLength();
	strVal = strVal.Left(nPos1-3);

	nPos1 = strVal.ReverseFind(':');
	nPos2 = strVal.GetLength();
	strVal.Delete(0, nPos1+3);

	nGetSmacPos = _tstoi(strVal);
	dGetSmacPos = double(nGetSmacPos * 0.005 ); // 엔코더 해상도 : [5um / 1pluse]
	strVal.Format(_T("%.3f"), dGetSmacPos);
	m_strReceiveVoiceCoil = _T("");

	return dGetSmacPos;
}
	
void CSmac::MoveSmacShiftPos()
{
	CString strSend, sMsg;
	strSend.Format(_T("MS40")); // Move Shift Position.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail MoveSmacShiftPos Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
	}
	//SetWaitSmacCmdEnd(_T("OK"));
}

void CSmac::MoveSmacFinalPos()
{
	double dPos;
	CString strSend, sMsg;
	strSend.Format(_T("MS50")); // Move Final Position.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);
	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail Moving Final Position Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
	}
}

void CSmac::ResetSmac() 
{
	CString strSend, sMsg;
	strSend.Format(_T("MS110")); // Reset.
	ClearReceive();

	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail Reset Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
	}
}

CString CSmac::GetSmacStatus()
{
	double dGetSmacPos=0.0;
	CString strSend, strVal, sMsg;
	strSend.Format(_T("MS20")); // Get Status.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");

	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail GetStatus Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return _T("NG");
	}

	if(m_strReceiveVoiceCoil.Find(_T(":STATUS_OK;")) != -1)
		strVal = _T("Good");		

	if(m_strReceiveVoiceCoil.Find(_T(":SERVO_OFF_OK;")) != -1)
		strVal = _T("Motor Off");

	if(m_strReceiveVoiceCoil.Find(_T(":SERVO_ERROR_OK;")) != -1)
		strVal = _T("Error");

	m_strReceiveVoiceCoil = _T("");

	return strVal;
}




double CSmac::GetSmacMeasureOfSurface()
{
	CString strVal, sMsg;
	double dPos, dFinalPos;
	CString strSend;
	int nPos1, nPos2;

	strSend.Format(_T("MS70")); // Measure Surface.
	ClearReceive();

	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail Reset Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return 0.0;
	}

	strVal = m_strReceiveVoiceCoil;

	nPos1 = strVal.Find(_T("OK"));
	nPos2 = strVal.GetLength();
	strVal = strVal.Left(nPos1-3);

	nPos1 = strVal.ReverseFind(':');
	nPos2 = strVal.GetLength();
	strVal.Delete(0, nPos1+3);

	dFinalPos = _tstoi(strVal);

	m_strReceiveVoiceCoil = _T("");
	Sleep(500);
	SearchHomeSmac();	
		
	return (double(dFinalPos * 0.005));	
}

void CSmac::SendStringToVoiceCoil(CString strSend)
{
	CString strTemp = strSend +_T("\n");
	TRACE(strTemp);
	int strSendLength;
	
	strSendLength=strSend.GetLength();

	LPWSTR pchData = new WCHAR[strSendLength];
	VARIANT vrOutp;
	char* pRtn = NULL;
	MultiByteToWideChar(CP_ACP, 0, pRtn=StringToChar(strSend), strSendLength, pchData, strSendLength);
	vrOutp.vt = VT_BSTR;
	vrOutp.bstrVal = ::SysAllocStringLen(pchData, strSendLength);

	delete pchData;

	if(pRtn)
		delete pRtn;
	
	Send(strSend);
	::SysFreeString(vrOutp.bstrVal);
}
	
BOOL CSmac::WaitSmacCmdEnd(CString strEndCmd)
{
	CString sMsg;
	DWORD CurTimer, StartTimer;
	MSG message;

	StartTimer=GetTickCount();
	CurTimer=GetTickCount();

	if (m_bRunTimerCheckEnd)
	{
		sMsg.Format(_T("Error, On waiting Smac_%d Command End Signal."), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return FALSE;
	}

	m_strSmacEndCmd = strEndCmd;
	m_bRunTimerCheckEnd = TRUE;	
	SetTimer(TIMER_CHECK_END_CMD, 100, NULL);

	while (DELAY_TIME_ONE_MINUTE > int(CurTimer - StartTimer) && !m_bReturnCmdEnd)
	{
		CurTimer = GetTickCount();
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (!IsDialogMessage(&message))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
	}

	m_strSmacEndCmd = _T("");

	if (!m_bReturnCmdEnd)
	{
		sMsg.Format(_T("Error, Overed time of waiting Smac_%d Command End Signal."), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		m_bRunTimerCheckEnd = FALSE;
		return FALSE;
	}

	return TRUE;
}


void CSmac::Wait(int imSecond)
{
	DWORD CurTimer, StartTimer;
	MSG message;
	StartTimer=GetTickCount();
	CurTimer=GetTickCount();
	while(imSecond > int(CurTimer-StartTimer))
	{
		CurTimer=GetTickCount();
		if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
}


int CSmac::CheckSmacErrorCode()
{
	int nLength, nPos, nErrCode=0;
	CString strVal, strTemp; 

	strVal = m_strReceiveVoiceCoil;

	if (strVal.Find(_T("?")) != -1)
	{
		nPos = strVal.Find(_T("?"));
		nLength = strVal.Delete(0, nPos + 1);
		nErrCode = _tstoi(strVal);
		strTemp.Format(_T("Error-%d"), nErrCode);
	}

	return nErrCode; // 0 : Not exist error code...
}

CString CSmac::DisplaySmacErrorList(int nErrCode)
{
	CString strError, strVal;

	switch(nErrCode)
	{
	case 0:
		strError = _T("NONE");
		return strError;
	case 1:
		strError = _T("ARGUMENT ERROR");
		break;
	case 2:
		strError = _T("INVALID COMMAND");
		break;
	case 3:
		strError = _T("INVALID MACRO COMMAND");
		break;
	case 4:
		strError = _T("MACRO ARGUMENT ERROR");
		break;
	case 5:
		strError = _T("MACRO NOT DEFINED");
		break;
	case 6:
		strError = _T("MACRO OUT OF RANGE");
		break;
	case 7:
		strError = _T("OUT OF MACRO SPACE");
		break;
	case 8:
		strError = _T("CAN'T DEFINE MACRO IN A MACRO");
		break;
	case 9:
		strError = _T("CAN'T DEFINE MACRO WHILE SERVO ENABLED");
		break;
	case 10:
		strError = _T("MACRO JUMP ERROR");
		break;
	case 11:
		strError = _T("OUT OF MACRO STACK SPACE");
		break;
	case 12:
		strError = _T("MACRO MUST BE FIRST COMMAND");
		break;
	case 13:
		strError = _T("STRING ERROR");
		break;
	case 14:
		strError = _T("MACRO STRING ERROR");
		break;
	case 15:
		strError = _T("SYNTAX ERROR");
		break;
	case 16:
		strError = _T("MACRO SYNTAX ERROR");
		break;
	case 17:
		strError = _T("AXIS RANGE ERROR");
		break;
	case 18:
		strError = _T("INTERRUPT MACRO NOT DEFINED");
		break;
	case 19:
		strError = _T("INTERRUPT MACRO STACK ERROR");
		break;
	case 20:
		strError = _T("MACRO STACK OVERFLOW");
		break;
	case 21:
		strError = _T("MACRO STACK UNDERFLOW");
		break;
	default:
		strError=_T("");
		return strError;
	}

	strVal.Format(_T("SMAC Error code is %s ."), strError);
	//AfxMessageBox(strVal);

	return strError;
}

void CSmac::CalcAveSmacHeight()
{
	CString strVal;
	int i;
	double dAverDist=0.0;

	for (i = 0; i < 4; i++)
		dAverDist += _tstof(pDoc->WorkingInfo.VoiceCoil[VOICE_COIL_FIRST_CAM].sSmacHeightAtPos[i]);

	strVal.Format(_T("%.3f"), dAverDist / 4.0);
	pDoc->WorkingInfo.Marking[VOICE_COIL_FIRST_CAM].sAverDist = strVal;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), strVal, PATH_WORKING_INFO);
}

LRESULT CSmac::OnReceiveRs232(WPARAM wP, LPARAM lP)
{
	CString str;
	if(m_sRcvRs232.IsEmpty())
		m_sRcvRs232.Format(_T("%s"),m_Rs232.m_cRxBuffer);
	else
	{
		str.Format(_T("%s"),m_Rs232.m_cRxBuffer);
		m_sRcvRs232 += str;
		m_strReceiveVoiceCoil = m_sRcvRs232;
	}

	m_Rs232.ClearReceive();

	return 0L;
}

void CSmac::ClearReceive()
{
	m_sRcvRs232 = _T("");
	m_bReturnCmdEnd = FALSE;
}


void CSmac::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	int nErrCode;

	switch (nIDEvent)
	{
		case TIMER_CHECK_END_CMD:
			KillTimer(TIMER_CHECK_END_CMD);
			if (m_strReceiveVoiceCoil.Find(m_strSmacEndCmd) != -1)
			{
				m_bReturnCmdEnd = TRUE;
				m_bRunTimerCheckEnd = FALSE;
			}
			if (m_bRunTimerCheckEnd)
				SetTimer(TIMER_CHECK_END_CMD, 100, NULL);
			break;

		case TIMER_CHECK_ERROR_CODE:    
			KillTimer(TIMER_CHECK_ERROR_CODE);
			nErrCode = CheckSmacErrorCode();
			if(nErrCode) // 0: No Error...
				DisplaySmacErrorList(nErrCode);
			break;

		default:    //MPIMessageFATAL_ERROR;
			break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CSmac::SetMarkShiftData()
{
	double dShiftPos, dShiftVel, dShiftAcc;
	CString strVal, sMsg;
	
	dShiftPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitPos) * 200;
	dShiftVel = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitVel) * 2621;
	dShiftAcc = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sWaitAcc) * 0.5;
	
	strVal.Format(_T("AL%d,AR14"), int(dShiftPos));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Shift Position Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	strVal.Format(_T("AL%d,AR30"), int(dShiftVel));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Shift Velocity Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	strVal.Format(_T("AL%d,AR32"), int(dShiftAcc));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Shift Acceleration Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	//SetTimer(TIMER_CHECK_ERROR_CODE, 100, NULL);
}

void CSmac::SetMarkFinalData()
{
	double dFinalPos, dFinalVel, dFinalAcc, dFinalTorque, dFinalTorqueOffset;
	CString strVal, sMsg;
	
	dFinalPos = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingPos) * 200;
	dFinalVel = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingVel) * 2621;	// 100[mm/s] = SV26214
	dFinalAcc = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingAcc) * 0.5;	// 100[mm/s2] = SA52
	//dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingToq) * 16.9;
	dFinalTorque = _tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingToq)/3.19;	// [-32767~32767]
	dFinalTorqueOffset = 8000 + (_tstof(pDoc->WorkingInfo.Marking[m_nCh].sMarkingToqOffset))*18.0;
	
	if(dFinalTorque > 1024.0)
		dFinalTorque = 1024.0;
	if(dFinalTorqueOffset > 32767.0)
		dFinalTorqueOffset = 32767.0;
	
	strVal.Format(_T("AL%d,AR16"), int(dFinalPos));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Final Position Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	strVal.Format(_T("AL%d,AR20"), int(dFinalVel));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Final Velocity Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	strVal.Format(_T("AL%d,AR22"), int(dFinalAcc));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Final Acceleration Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	strVal.Format(_T("AL%d,AR40"), int(dFinalTorque));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Final Torque Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	strVal.Format(_T("AL%d,AR48"), int(dFinalTorqueOffset));
	SendStringToVoiceCoil(strVal);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to set Final Torque Offset Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}

	//if(!m_bTimerStop)
	//	SetTimer(TIMER_CHECK_ERROR_CODE, 100, NULL);		
}


void CSmac::SetMark()
{
	CString strSend, sMsg;
	strSend.Format(_T("MS60")); // Punching
	ClearReceive();
	
	m_strReceiveVoiceCoil = _T("");
	SendStringToVoiceCoil(strSend);
	Sleep(30);
	SendStringToVoiceCoil(_T("\r\n"));
	Sleep(100);

	if (!WaitSmacCmdEnd(_T("OK")))
	{
		sMsg.Format(_T("Fail to mark in Smac_%d"), m_nCh);
		pView->ClrDispMsg(); 
		AfxMessageBox(sMsg);
		return;
	}
}

BOOL CSmac::IsDoneMark()
{
	return m_bReturnCmdEnd;
}


char* CSmac::StringToChar(CString str) // char* returned must be deleted... 
{
	char*		szStr = NULL;
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 
	szStr = new char[nLenth];  //메모리 할당 

	//3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, szStr, nLenth, 0, 0);
	return szStr;
}
