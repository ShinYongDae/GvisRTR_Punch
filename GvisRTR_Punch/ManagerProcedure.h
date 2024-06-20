#pragma once
#include "afxwin.h"

#include "Device/Engrave.h"

#define TIM_INIT_PROCEDURE			0

#define AT_LP					10
#define MK_DONE_CHECK			30	// m_nStepMk
#define MK_LIGHT_ERR			50	// m_nStepMk
#define MK_END					100	// m_nStepMk

#define MK_ST					100	// m_nMkStAuto
#define ERR_PROC				120	// m_nMkStAuto
#define REJECT_ST				200	// m_nMkStAuto
#define ERROR_ST				250	// m_nMkStAuto

#define MK_VERIFY				200	// m_nSetpAuto
#define MK_REVIEW				220	// m_nSetpAuto
#define LAST_PROC				250	// m_nSetpAuto
#define LAST_PROC_VS_ALL		300	// m_nSetpAuto
#define LAST_PROC_VS_UP			350	// m_nSetpAuto
#define LAST_PROC_VS_DN			400	// m_nSetpAuto
#define LOT_END					500	// m_nSetpAuto

#define ENG_ST					100	// m_nEngStAuto
#define ENG_2D_ST				150	// m_nEngStAuto

class CManagerProcedure : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_PROCEDURE;
	int m_nStepInitProcedure;
	BOOL m_bMkSt, m_bMkStSw;
	int m_nMkStAuto;
	BOOL m_bLotEnd;
	int m_nLotEndAuto;


	void Init();
	BOOL Create();

	void DoAuto();
	BOOL DoAutoGetLotEndSignal();
	BOOL DoAtuoGetMkStSignal();

public:
	CManagerProcedure(CWnd* pParent = NULL);
	~CManagerProcedure();

	// 특성입니다.
public:
	CEngrave* m_pEngrave;


	// 작업입니다.
public:
	BOOL InitAct();
	void InitAuto(BOOL bInit = TRUE); // When doReset...
	void Auto();
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

