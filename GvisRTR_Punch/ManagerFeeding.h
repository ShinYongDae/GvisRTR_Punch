#pragma once
#include "afxwin.h"

#define TIM_INIT_FEEDING			0

class CManagerFeeding : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_FEEDING;
	int m_nStepInitFeeding;

	BOOL Create();

public:
	CManagerFeeding(CWnd* pParent = NULL);
	~CManagerFeeding();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:


	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

