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

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:
	BOOL InitAct();

	// ������ �޽��� �� �Լ�
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

