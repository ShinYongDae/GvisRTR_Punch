
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

class CGvisRTR_PunchView : public CFormView
{
	BOOL m_bTIM_INIT_VIEW;
	int m_nStepInitView;

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
	
	CDlgInfo *m_pDlgInfo;
	CDlgFrameHigh *m_pDlgFrameHigh;
	CDlgMenu01 *m_pDlgMenu01;
	CDlgMenu02 *m_pDlgMenu02;
	CDlgMenu03 *m_pDlgMenu03;
	CDlgMenu04 *m_pDlgMenu04;
	CDlgMenu05 *m_pDlgMenu05;
	CDlgMenu06 *m_pDlgMenu06;
	void ShowDlg(int nID);
	void HideAllDlg();
	void DelAllDlg();

	// 보조작업입니다.
	int MsgBox(CString sMsg, int nThreadIdx = 0, int nType = MB_OK, int nTimOut = DEFAULT_TIME_OUT, BOOL bEngave = TRUE);		// SyncMsgBox

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

