#pragma once
#include "afxwin.h"

#include "Global/GlobalDefine.h"
#include "Process/ThreadTask.h"
#include "Device/Motion.h"
#include "Device/Smac.h"
#include "Device/Vision.h"
#include "Device/Light.h"

#define TIM_INIT_PUNCH			0
#define TIM_SCAN_STATUS			10

struct stStatusPunch
{
	BOOL bStopFeeding;

	stStatusPunch()
	{
		bStopFeeding = FALSE;
	}
};

class CManagerPunch : public CWnd
{
	CWnd* m_pParent;
	BOOL m_bCreated;

	BOOL m_bTIM_INIT_PUNCH, m_bTIM_SCAN_STATUS;
	int m_nStepInitPunch;

	CMotion* m_pMotion;
	CSmac* m_pVoiceCoil[2];	// [0] : LeftCam , [1] : RightCam
	CVision* m_pVision[2];	// [0] : LeftCam , [1] : RightCam
	CVision* m_pVisionInner[2];	// [0] : LeftCam , [1] : RightCam
	CLight* m_pLight;

	double m_dEnc[MAX_AXIS];

	BOOL Init();
	BOOL Create();
	BOOL CreateDevices();
	void DoInterlock();

public:
	CManagerPunch(CWnd* pParent = NULL);
	~CManagerPunch();

	// 특성입니다.
public:
	BOOL m_bAuto, m_bManual, m_bOneCycle;
	stStatusPunch Status;


	// 작업입니다.
public:
	void Reset();
	BOOL InitAct();

	void SetLight(int nVal = -1);
	void SetLight2(int nVal = -1);
	void ResetLight();
	void ResetLight2();
	int GetLight();
	int GetLight2();

	void ResetMotion();
	void ResetMotion(int nMsId);
	void StopFeeding(BOOL bStop = TRUE);

	// 보조작업입니다.
public:
	int MsgBox(CString sMsg, int nThreadIdx = 0, int nType = MB_OK, int nTimOut = DEFAULT_TIME_OUT, BOOL bEngave = TRUE);		// SyncMsgBox

	// 생성된 메시지 맵 함수
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

