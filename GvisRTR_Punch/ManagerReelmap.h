#pragma once
#include "afxwin.h"

#define TIM_INIT_REELMAP			0

class CManagerReelmap : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_REELMAP;
	int m_nStepInitReelmap;

	void Init();
	BOOL Create();
	void LoadConfig();

public:
	CManagerReelmap(CWnd* pParent = NULL);
	~CManagerReelmap();

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

