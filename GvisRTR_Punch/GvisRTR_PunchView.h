
// GvisRTR_PunchView.h : CGvisRTR_PunchView 클래스의 인터페이스
//

#pragma once

#include "ManagerReelmap.h"
#include "ManagerProcedure.h"
#include "ManagerPunch.h"
#include "ManagerThread.h"
#include "ManagerFeeding.h"
#include "ManagerStatus.h"

#include "GvisRTR_PunchDoc.h"

#include "Dialog/DlgMyMsg.h"
#include "Dialog/DlgMsgBox.h"

#include "Dialog/DlgFrameHigh.h"
#include "Dialog/DlgInfo.h"
#include "Dialog/DlgMenu01.h"
#include "Dialog/DlgMenu02.h"
#include "Dialog/DlgMenu03.h"
#include "Dialog/DlgMenu04.h"
#include "Dialog/DlgMenu05.h"
#include "Dialog/DlgMenu06.h"
#include "Dialog/DlgMenu07.h"
#include "Dialog/DlgOption01.h"

#define TIM_INIT_VIEW			0
#define TIM_DISP_STATUS			14

typedef struct _DispMain
{
	CString sMsg;
	COLORREF rgb;

	_DispMain()
	{
		Init();
	}
	_DispMain(CString Msg, COLORREF Rgb)
	{
		sMsg = Msg;
		rgb = Rgb;
	}

	void Init()
	{
		sMsg = _T("");
		rgb = RGB_WHITE;
	}

}stDispMain;


class CGvisRTR_PunchView : public CFormView
{
	BOOL m_bTIM_INIT_VIEW, m_bTIM_DISP_STATUS;
	int m_nStepInitView;

	stDispMain m_stDispMain;
	CString m_sDispMain, m_sDispTime;
	CString m_sDispStatusBar[10];
	CString m_sShare[2], m_sBuf[2];			// [0]: AOI-Up , [1]: AOI-Dn
	int	m_pBufSerial[2][100], m_nBufTot[2]; // [0]: AOI-Up , [1]: AOI-Dn
	__int64 m_nBufSerialSorting[2][100];	// [0]: AOI-Up , [1]: AOI-Dn

	DWORD m_dwLotSt, m_dwLotEd;

	void InitMgr();
	void CreateMgr();
	void CreateMgrProcedure();
	void CreateMgrReelmap();
	void CreateMgrPunch();
	void CreateMgrThread();
	void CreateMgrFeeding();
	void CreateMgrStatus();
	void CloseMgr();
	void CloseMgrProcedure();
	void CloseMgrReelmap();
	void CloseMgrPunch();
	void CloseMgrThread();
	void CloseMgrFeeding();
	void CloseMgrStatus();

	void InitDlg();

	CDlgMyMsg* m_pDlgMyMsg;
	CDlgMsgBox* m_pDlgMsgBox;
	void InitDispMsg();
	void CloseDispMsg();
	void CloseMyMsg();
	void CloseMsgBox();

	int DoDispMain();

	void DispStsBar();
	void DispStsMainMsg(int nIdx = 0);	// 0
	void DispTime();					// 7
	void ChkShare();					// 2, 4
	void ChkBuf();						// 1, 3
	CString GetTime(stLotTime &LotTime);
	void ChkShareUp();
	void ChkShareDn();
	BOOL ChkShare(int &nSerial);
	BOOL ChkShareUp(int &nSerial);
	BOOL ChkShareDn(int &nSerial);
	void ChkBufUp();
	void ChkBufDn();
	BOOL ChkBufUp(int* pSerial, int &nTot);
	BOOL ChkBufDn(int* pSerial, int &nTot);
	BOOL SortingInUp(CString sPath, int nIndex);
	BOOL SortingOutUp(int* pSerial, int nTot);
	void SwapUp(__int64 *num1, __int64 *num2);
	BOOL SortingInDn(CString sPath, int nIndex);
	BOOL SortingOutDn(int* pSerial, int nTot);
	void SwapDn(__int64 *num1, __int64 *num2);

	void DelOverLotEndSerialUp(int nSerial);
	void DelOverLotEndSerialDn(int nSerial);
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 

	void DispLotTime();

protected: // serialization에서만 만들어집니다.
	CGvisRTR_PunchView();
	DECLARE_DYNCREATE(CGvisRTR_PunchView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_GVISRTR_PUNCH_FORM };
#endif

// 특성입니다.
public:
	CGvisRTR_PunchDoc* GetDocument() const;

	CManagerReelmap *m_mgrReelmap;
	CManagerProcedure *m_mgrProcedure;
	CManagerPunch *m_mgrPunch;
	CManagerThread *m_mgrThread;
	CManagerFeeding *m_mgrFeeding;
	CManagerStatus* m_mgrStatus;

	CString m_sAlmMsg, m_sPrevAlmMsg, m_sIsAlmMsg;

	// from Engrave
	BOOL m_bRcvSig[_SigInx::_EndIdx];

// 작업입니다.
public:
	void DestroyView();

	BOOL m_bDispMsg, m_bWaitClrDispMsg;
	int m_nStepDispMsg[10];
	void ClrDispMsg();
	LONG OnQuitDispMsg(UINT wParam, LONG lParam);
	void DoDispMsg(CString strMsg, CString strTitle = _T(""), COLORREF color = RGB(255, 0, 0), DWORD dwDispTime = 0, BOOL bOverWrite = TRUE);
	void GetDispMsg(CString &strMsg, CString &strTitle);
	void DispMsg(CString strMsg, CString strTitle = _T(""), COLORREF color = RGB(255, 0, 0), DWORD dwDispTime = 0, BOOL bOverWrite = TRUE);
	void DispContRun(BOOL bOn);
	void SetMyMsgYes();
	void SetMyMsgNo();
	void SetMyMsgOk();
	void SetAlarm(CString sMsg);
	void ClrAlarm();
	void SetListBuf();

	CDlgInfo *m_pDlgInfo;
	CDlgFrameHigh *m_pDlgFrameHigh;
	CDlgMenu01 *m_pDlgMenu01;
	CDlgMenu02 *m_pDlgMenu02;
	CDlgMenu03 *m_pDlgMenu03;
	CDlgMenu04 *m_pDlgMenu04;
	CDlgMenu05 *m_pDlgMenu05;
	CDlgMenu06 *m_pDlgMenu06;
	CDlgMenu07 *m_pDlgMenu07;
	void ShowDlg(int nID);
	void HideAllDlg();
	void DelAllDlg();

	void EnableItsMode(BOOL bEnable = TRUE);

	// 보조작업입니다.
	int MsgBox(CString sMsg, int nThreadIdx = 0, int nType = MB_OK, int nTimOut = DEFAULT_TIME_OUT, BOOL bEngave = TRUE);		// SyncMsgBox
	int MyPassword(CString strMsg, int nCtrlId = 0);

	CString GetDispMain();
	void DispMain(CString sMsg, COLORREF rgb = RGB(0, 255, 0));
	void DispStsBar(CString sMsg, int nIdx = 0);
	BOOL IsRun();
	void InitReelmapDisp();
	void InitCamImgDisp();

	void DispInfo();
	void ClrMkInfo();

	void ResetMkInfo(int nAoi = 0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	void SetAlignPos();
	void SetAlignPosUp();
	void SetAlignPosDn();

	void SetDualTest(BOOL bOn);
	void SetTestMode(int nMode);
	int GetLotEndSerial();
	void SetLotEnd(int nSerial);

	void SetLotSt();
	void SetLotEd();
	DWORD GetLotSt();
	DWORD GetLotEd();

	void SetMkMenu01(CString sMenu, CString sItem, CString sData);
	void SetMkMenu03(CString sMenu, CString sItem, BOOL bOn);
	void ChkReTestAlarmOnAoiUp();
	void ChkReTestAlarmOnAoiDn();

	BOOL IsBuffer(int nNum = 0);
	BOOL IsBufferUp();
	BOOL IsBufferDn();
	int GetBuffer(int *pPrevSerial);
	int GetBufferUp(int *pPrevSerial);
	int GetBufferDn(int *pPrevSerial);

	BOOL IsBuffer0();
	BOOL IsBufferUp0();
	BOOL IsBufferDn0();
	int GetBuffer0(int *pPrevSerial);
	int GetBufferUp0(int *pPrevSerial);
	int GetBufferDn0(int *pPrevSerial);

	BOOL IsBuffer1();
	BOOL IsBufferUp1();
	BOOL IsBufferDn1();
	int GetBuffer1(int *pPrevSerial);
	int GetBufferUp1(int *pPrevSerial);
	int GetBufferDn1(int *pPrevSerial);

	BOOL IsShare();
	BOOL IsShareUp();
	BOOL IsShareDn();
	int GetShareUp();
	int GetShareDn();
	BOOL IsShareVs();
	BOOL IsShareVsUp();
	BOOL IsShareVsDn();
	int GetShareVsUp();
	int GetShareVsDn();

	void ModelChange(int nAoi = 0); // 0 : AOI-Up , 1 : AOI-Dn
	void UpdateProcessNum(CString sProcessNum);

	// ManagerFeeding
	BOOL GetMpeSignal(int nSection, int nName);
	long GetMpeData(int nSection, int nName);
	BOOL MpeWrite(CString strRegAddr, long lData, BOOL bCheck = FALSE);

	BOOL IsAuto();
	BOOL GetMkStSignal();
	void ResetMkStSignal();
	int GetLastShotMk();

	BOOL IsEnableBtn(int nId);
	void EnableBtn(int nId, BOOL bEnable);
	void SetLed(int nIdx, BOOL bOn);
	void UpdateRst();
	void Buzzer(BOOL bOn, int nCh = 0);
	void Stop();
	void SetAoiUpOffset(CfPoint &OfSt);
	void SetAoiDnOffset(CfPoint &OfSt);
	void SetEngOffset(CfPoint &OfSt);
	void DispLotEndSerial(int nSerial);
	void DispLotEnd(BOOL bOn);
	void DispLotStTime();
	BOOL IsReady();
	BOOL IsAoiLdRun();

	// ManagerProcedure
	void Auto();

	// ManagerPunch
	void ResetMotion();

	// ManagerReelmap

	// ManagerStatus

	// ManagerThread

	
// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CGvisRTR_PunchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnDlgInfo(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // GvisRTR_PunchView.cpp의 디버그 버전
inline CGvisRTR_PunchDoc* CGvisRTR_PunchView::GetDocument() const
   { return reinterpret_cast<CGvisRTR_PunchDoc*>(m_pDocument); }
#endif

