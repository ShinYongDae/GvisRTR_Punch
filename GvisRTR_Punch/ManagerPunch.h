#pragma once
#include "afxwin.h"

#define TIM_INIT_PUNCH			0

class CManagerPunch : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_PUNCH;
	int m_nStepInitPunch;

	BOOL Create();

public:
	CManagerPunch(CWnd* pParent = NULL);
	~CManagerPunch();

	// Ư���Դϴ�.
public:
	BOOL m_bAuto, m_bManual, m_bOneCycle;

	// �۾��Դϴ�.
public:

	// ������ �޽��� �� �Լ�
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

