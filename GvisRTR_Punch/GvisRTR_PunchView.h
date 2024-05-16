
// GvisRTR_PunchView.h : CGvisRTR_PunchView 클래스의 인터페이스
//

#pragma once

#include "ManagerReelmap.h"
#include "ManagerProcedure.h"
#include "ManagerPunch.h"
#include "ManagerThread.h"

#include "GvisRTR_PunchDoc.h"

#include "Dialog/DlgMyMsg.h"
#include "Dialog/DlgMsgBox.h"

#define TIM_INIT_VIEW			0

class CGvisRTR_PunchView : public CFormView
{
	BOOL m_bTIM_INIT_VIEW;
	int m_nStepInitView;

	void InitMgr();
	void InitMgrProcedure();
	void InitMgrReelmap();
	void InitMgrPunch();
	void InitMgrThread();

	CDlgMyMsg* m_pDlgMyMsg;
	CDlgMsgBox* m_pDlgMsgBox;
	void InitDispMsg();

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

// 작업입니다.
public:
	void ClrDispMsg();
	LONG OnQuitDispMsg(UINT wParam, LONG lParam);
	void DoDispMsg(CString strMsg, CString strTitle = _T(""), COLORREF color = RGB(255, 0, 0), DWORD dwDispTime = 0, BOOL bOverWrite = TRUE);

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
};

#ifndef _DEBUG  // GvisRTR_PunchView.cpp의 디버그 버전
inline CGvisRTR_PunchDoc* CGvisRTR_PunchView::GetDocument() const
   { return reinterpret_cast<CGvisRTR_PunchDoc*>(m_pDocument); }
#endif

