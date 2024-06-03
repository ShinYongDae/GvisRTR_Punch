#pragma once
#include "afxwin.h"

#include "Device/Engrave.h"

#define TIM_INIT_PROCEDURE			0


class CManagerProcedure : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_PROCEDURE;
	int m_nStepInitProcedure;

	void Init();
	BOOL Create();

public:
	CManagerProcedure(CWnd* pParent = NULL);
	~CManagerProcedure();

	// Ư���Դϴ�.
public:
	CEngrave* m_pEngrave;


	// �۾��Դϴ�.
public:
	BOOL InitAct();


	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

