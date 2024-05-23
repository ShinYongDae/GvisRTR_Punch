#pragma once
#include "afxwin.h"

#define TIM_INIT_THREAD			0

class CManagerThread : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_THREAD;
	int m_nStepInitThread;

	void Init();
	BOOL Create();

public:
	CManagerThread(CWnd* pParent = NULL);
	~CManagerThread();

	// 특성입니다.
public:


	// 작업입니다.
public:
	BOOL InitAct();

	// 생성된 메시지 맵 함수
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

