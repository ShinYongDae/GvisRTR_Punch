#pragma once
#include "afxwin.h"

#include "Global/GlobalDefine.h"
#include "Process/ThreadTask.h"
#include "Device/Motion.h"
#include "Device/Smac.h"
#include "Device/Vision.h"
#include "Device/Light.h"
#include "Device/SR1000W.h"

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
	BOOL m_bDoneMk[2];			// [nCam] : TRUE(Punching �Ϸ�), FALSE(Punching �̿Ϸ�)
	BOOL m_bCollision[2], m_bPriority[4];

	BOOL Init();
	BOOL Create();
	BOOL CreateDevices();
	void DoInterlock();

public:
	CManagerPunch(CWnd* pParent = NULL);
	~CManagerPunch();

	// Ư���Դϴ�.
public:
	BOOL m_bAuto, m_bManual, m_bOneCycle;
	stStatusPunch Status;
	CSr1000w* m_pSr1000w;


	// �۾��Դϴ�.
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
	void MoveInitPos0(BOOL bWait = TRUE);
	void MoveInitPos1(BOOL bWait = TRUE);
	void GetEnc();
	BOOL IsRunAxisX();
	void EStop();

	BOOL ChkCollision();
	BOOL GetCollision(int nCam);
	void ResetPriority();
	void SetPriority();

	void DoMark0();
	void DoMark1();
	void DoMark0Its();
	void DoMark1Its();
	void DoMark0All();
	void DoMark1All();
	void DoReject0();
	void DoReject1();

	BOOL SaveMk0Img(int nMkPcsIdx);
	BOOL SaveMk1Img(int nMkPcsIdx);

	BOOL GetDoneMk(int nCam);
	void SetDoneMk(int nCam);

	// �����۾��Դϴ�.
public:
	int MsgBox(CString sMsg, int nThreadIdx = 0, int nType = MB_OK, int nTimOut = DEFAULT_TIME_OUT, BOOL bEngave = TRUE);		// SyncMsgBox

	// ������ �޽��� �� �Լ�
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

