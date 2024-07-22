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
	CLibMil *m_pMil;

	double m_dEnc[MAX_AXIS];
	BOOL m_bDoneMk[2];			// [nCam] : TRUE(Punching �Ϸ�), FALSE(Punching �̿Ϸ�)
	BOOL m_bCollision[2], m_bPriority[4];
	int m_nMkStrip[2][MAX_STRIP]; // [nCam][nStrip] - [��/��][] : ��Ʈ���� ��Ī�� �ǽ� �� count
	BOOL m_bRejectDone[2][MAX_STRIP]; // Shot[2], Strip[4] - [��/��][] : ��Ʈ���� ��Ī�� �ǽ� �� count�� ��Ʈ�� ��� ������ �Ϸ� ���� 

	BOOL Init();
	BOOL Create();
	BOOL CreateDevices();
	void DoInterlock();
	void Buzzer(BOOL bOn, int nCh = 0);
	BOOL IsNoMk();

	CfPoint GetMkPnt(int nIdx);								// CamMaster�� �ǽ����� �ε���
	CfPoint GetMkPnt0(int nIdx);							// CamMaster�� �ǽ����� �ε���
	CfPoint GetMkPnt1(int nIdx);							// CamMaster�� �ǽ����� �ε���
	CfPoint GetMkPnt0(int nSerial, int nMkPcs);				// pcr �ø���, pcr �ҷ� �ǽ� ���� ���� �ε���
	CfPoint GetMkPnt1(int nSerial, int nMkPcs);				// pcr �ø���, pcr �ҷ� �ǽ� ���� ���� �ε���
	CfPoint GetMkPnt0Its(int nSerial, int nMkPcs);			// pcr �ø���, pcr �ҷ� �ǽ� ���� ���� �ε���
	CfPoint GetMkPnt1Its(int nSerial, int nMkPcs);			// pcr �ø���, pcr �ҷ� �ǽ� ���� ���� �ε���
	int GetPcrIdx0(int nSerial, BOOL bNewLot = FALSE);		// Up - ����ȭ�� ǥ�ø� ���� Display buffer�� Shot �ε���
	int GetPcrIdx1(int nSerial, BOOL bNewLot = FALSE);		// Dn - ����ȭ�� ǥ�ø� ���� Display buffer�� Shot �ε���

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

	void StopFeeding(BOOL bStop = TRUE);

	void DoMark0();
	void DoMark1();
	void DoMark0Its();
	void DoMark1Its();
	void DoMark0All();
	void DoMark1All();
	void DoReject0();
	void DoReject1();

	BOOL GetDoneMk(int nCam);
	void SetDoneMk(int nCam);
	double CalcCameraPixelSize();
	int GetMkStrip(int nCameraIdx, int nStripIdx); // [nCam][nStrip] - [��/��][] : ��Ʈ���� ��Ī�� �ǽ� �� count

	// �����۾��Դϴ�.
public:
	int MsgBox(CString sMsg, int nThreadIdx = 0, int nType = MB_OK, int nTimOut = DEFAULT_TIME_OUT, BOOL bEngave = TRUE);		// SyncMsgBox

	//===> Motion
	void ResetMotion();
	void ResetMotion(int nMsId);
	void GetEnc();
	double GetEnc(int nAxis);
	BOOL IsRunAxisX();
	void EStop();
	BOOL ChkCollision();
	BOOL GetCollision(int nCam);
	void ResetPriority();
	void SetPriority();
	BOOL MoveAlign0(int nPos);
	BOOL MoveAlign1(int nPos);
	BOOL IsMoveDone();
	BOOL IsMoveDone0();
	BOOL IsMoveDone1();
	BOOL TwoPointAlign0(int nPos);
	BOOL TwoPointAlign1(int nPos);
	void MoveInitPos0(BOOL bWait = TRUE);
	void MoveInitPos1(BOOL bWait = TRUE);
	void MoveMkEdPos1();
	BOOL IsInitPos0();
	BOOL IsInitPos1();
	BOOL IsMkEdPos1();
	BOOL CheckMkPnt();
	void SetAlignPos();
	void SetAlignPosUp();
	void SetAlignPosDn();
	void ResetMkStrip();

	//===> Vision
	BOOL Do2PtAlign0(int nPos, BOOL bDraw = FALSE);
	BOOL Do2PtAlign1(int nPos, BOOL bDraw = FALSE);
	BOOL SaveMk0Img(int nMkPcsIdx);
	BOOL SaveMk1Img(int nMkPcsIdx);
	void ShowDispCadUp(int nIdxMkInfo, int nSerial, int nIdxDef); // From 0 To 12...for Screen display.
	void ShowDispCadDn(int nIdxMkInfo, int nSerial, int nIdxDef); // From 0 To 12...for Screen display.
	void ShowOvrCadUp(int nIdxMkInfo, int nSerial);
	void ShowOvrCadDn(int nIdxMkInfo, int nSerial);
	void ShowDispDefUp(int nIdxMkInfo, int nSerial, int nDefPcs);
	void ShowDispDefDn(int nIdxMkInfo, int nSerial, int nDefPcs);
	void LoadCADBufUp(UCHAR *pCADCellImg, long OrgStX, long OrgStY, long DesStX, long DesStY, long SizeX, long SizeY);
	void LoadCADBufDn(UCHAR *pCADCellImg, long OrgStX, long OrgStY, long DesStX, long DesStY, long SizeX, long SizeY);

	//===> SMAC
	void SetMarkShiftData(int nCamNum);
	void SetMarkFinalData(int nCamNum);

	// ������ �޽��� �� �Լ�
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

