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

	// 특성입니다.
public:
	CEngrave* m_pEngrave;


	// 작업입니다.
public:
	BOOL InitAct();
	void SetCurrentInfoBufUpTot(int nTotal);
	void SetCurrentInfoBufUp(int nIdx, int nData);
	void SetCurrentInfoBufDnTot(int nTotal);
	void SetCurrentInfoBufDn(int nIdx, int nData);
	int GetAoiUpCamMstInfo(); // AOI상 strpcs.bin 연결
	int GetAoiDnCamMstInfo(); // AOI하 strpcs.bin 연결
	void SetMkMenu01(CString sMenu, CString sItem, CString sData);
	void SetMkMenu03(CString sMenu, CString sItem, BOOL bOn);

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

