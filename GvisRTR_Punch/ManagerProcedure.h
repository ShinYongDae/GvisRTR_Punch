#pragma once
#include "afxwin.h"

#define TIM_INIT_PROCEDURE			0


class CManagerProcedure : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_PROCEDURE;
	int m_nStepInitProcedure;

	BOOL Create();

public:
	CManagerProcedure(CWnd* pParent = NULL);
	~CManagerProcedure();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:
	BOOL InitAct();


	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

