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

	// 특성입니다.
public:


	// 작업입니다.
public:
	BOOL InitAct();
	void SetBufInitPos(double dPos);
	void SetBufHomeParam(double dVel, double dAcc);

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

