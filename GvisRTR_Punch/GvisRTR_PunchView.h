
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
	void SetListBuf();
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

	// from Engrave
	BOOL m_bRcvSig[_SigInx::_EndIdx];

// 작업입니다.
public:
	void DestroyView();

	BOOL m_bDispMsg, m_bWaitClrDispMsg;
	BOOL m_bDispMsgDoAuto[10];
	int m_nStepDispMsg[10];
	void ClrDispMsg();
	LONG OnQuitDispMsg(UINT wParam, LONG lParam);
	void DoDispMsg(CString strMsg, CString strTitle = _T(""), COLORREF color = RGB(255, 0, 0), DWORD dwDispTime = 0, BOOL bOverWrite = TRUE);
	void GetDispMsg(CString &strMsg, CString &strTitle);
	void DispMsg(CString strMsg, CString strTitle = _T(""), COLORREF color = RGB(255, 0, 0), DWORD dwDispTime = 0, BOOL bOverWrite = TRUE);
	void SetMyMsgYes();
	void SetMyMsgNo();
	void SetMyMsgOk();

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

	void DispMain(CString sMsg, COLORREF rgb = RGB(0, 255, 0));
	void DispStsBar(CString sMsg, int nIdx = 0);

	void SetDualTest(BOOL bOn);
	void SetTestMode(int nMode);

	// ManagerFeeding
	long GetMpeData(int nSection, int nName);
	BOOL MpeWrite(CString strRegAddr, long lData, BOOL bCheck = FALSE);
	BOOL IsAuto();

	// ManagerProcedure

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

