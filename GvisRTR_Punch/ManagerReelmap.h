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

