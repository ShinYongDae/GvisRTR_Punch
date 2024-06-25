#pragma once
#include "afxwin.h"

#include "Device/MpDevice.h"

#define TIM_INIT_FEEDING			0
#define TIM_SCAN_MPE				10

struct stStatusFeeding
{
	BOOL bAuto, bManual, bOneCycle;								// Mode ����ġ
	BOOL bSwJogLeft, bSwJogFast, bSwJogStep;					// Jog �ǳ� ���� ����ġ
	BOOL bDoorMk[4], bDoorAoi[12];								// ���� ����
	BOOL bDoorMkF[4], bDoorAoiF[12];							// ���� ����
	BOOL bEmgMk[2], bEmgAoi[4];									// ������� ����ġ
	BOOL bEmgUc, bEmgRc;										// ������� ����ġ
	BOOL bEmgEngv[2], bEmgEngvF[2];								// ������� ����ġ
	BOOL bEmgMkF[2], bEmgAoiF[4];								// ������� ����ġ
	BOOL bEmgUcF, bEmgRcF;										// ������� ����ġ
	BOOL bMainAirMk, bMainAirAoi;								// ���� ����
	BOOL bSensTblVacMk, bSensTblVacAoi;							// ���̺� ���� ����
	BOOL bSensTblPrsMk, bSensTblPrsAoi;							// ���̺� �з� ����
	BOOL bSensSaftyMk, bSensSaftyAoi;							// ���� ����
	BOOL bSensSaftyMkF, bSensSaftyAoiF;							// ���� ����
	BOOL bSensLmtBufMk[2], bSensLmtBufRc[2], bSensLmtBufUc[2];	// ���� �ѷ� ����Ʈ ����
	BOOL bSigTestDoneAoi, bSigTblAirAoi;						// �˻�� ��ȣ
	BOOL bDoorUc[4], bDoorRe[5];								// ���� ����
	BOOL bDoorUcF[4], bDoorReF[5];								// ���� ����
	BOOL bDoorEngv[4], bDoorEngvF[4];							// ���� ����

	stStatusFeeding()
	{
		bAuto = FALSE; bManual = FALSE; bOneCycle = FALSE; bSwJogLeft = FALSE; bSwJogFast = FALSE; bSwJogStep = FALSE;
		bDoorEngv[0] = FALSE; bDoorEngv[1] = FALSE; bDoorEngv[2] = FALSE; bDoorEngv[3] = FALSE;
		bDoorEngvF[0] = FALSE; bDoorEngvF[1] = FALSE; bDoorEngvF[2] = FALSE; bDoorEngvF[3] = FALSE;
		bDoorMk[0] = FALSE; bDoorMk[1] = FALSE; bDoorMk[2] = FALSE; bDoorMk[3] = FALSE;
		bDoorMkF[0] = FALSE; bDoorMkF[1] = FALSE; bDoorMkF[2] = FALSE; bDoorMkF[3] = FALSE;
		bDoorAoi[0] = FALSE; bDoorAoi[1] = FALSE; bDoorAoi[2] = FALSE; bDoorAoi[3] = FALSE; bDoorAoi[4] = FALSE; bDoorAoi[5] = FALSE;
		bDoorAoiF[0] = FALSE; bDoorAoiF[1] = FALSE; bDoorAoiF[2] = FALSE; bDoorAoiF[3] = FALSE; bDoorAoiF[4] = FALSE; bDoorAoiF[5] = FALSE;
		bEmgMk[0] = FALSE; bEmgMk[1] = FALSE;
		bEmgAoi[0] = FALSE; bEmgAoi[1] = FALSE;
		bEmgUc = FALSE; bEmgRc = FALSE;
		bEmgEngv[0] = FALSE; bEmgEngvF[0] = FALSE;
		bEmgEngv[1] = FALSE; bEmgEngvF[1] = FALSE;
		bEmgMkF[0] = FALSE; bEmgMkF[1] = FALSE;
		bEmgAoiF[0] = FALSE; bEmgAoiF[1] = FALSE;
		bEmgUcF = FALSE; bEmgRcF = FALSE;
		bMainAirMk = FALSE; bMainAirAoi = FALSE;
		bSensTblVacMk = FALSE; bSensTblVacAoi = FALSE;
		bSensTblPrsMk = FALSE; bSensTblPrsAoi = FALSE;
		bSensSaftyMk = FALSE; bSensSaftyAoi = FALSE;
		bSensSaftyMkF = FALSE; bSensSaftyAoiF = FALSE;
		bSensLmtBufMk[0] = FALSE; bSensLmtBufMk[1] = FALSE;
		bSensLmtBufUc[0] = FALSE; bSensLmtBufUc[1] = FALSE;
		bSensLmtBufRc[0] = FALSE; bSensLmtBufRc[1] = FALSE;
		bSigTestDoneAoi = FALSE; bSigTblAirAoi = FALSE;
		bDoorUc[0] = FALSE; bDoorUc[1] = FALSE; bDoorUc[2] = FALSE; bDoorUc[3] = FALSE;
		bDoorUcF[0] = FALSE; bDoorUcF[1] = FALSE; bDoorUcF[2] = FALSE; bDoorUcF[3] = FALSE;
		bDoorRe[0] = FALSE; bDoorRe[1] = FALSE; bDoorRe[2] = FALSE; bDoorRe[3] = FALSE; bDoorRe[4] = FALSE;
		bDoorReF[0] = FALSE; bDoorReF[1] = FALSE; bDoorReF[2] = FALSE; bDoorReF[3] = FALSE; bDoorReF[4] = FALSE;
	}
};

struct stBtnPush
{
	BOOL bSwRun, bSwRunF;
	BOOL bSwStop, bSwStopF;
	BOOL bSwReset, bSwResetF;
	BOOL bSwReady, bSwReadyF;

	stBtnPush()
	{
		bSwRun = FALSE; bSwRunF = FALSE;
		bSwStop = FALSE; bSwStopF = FALSE;
		bSwReset = FALSE; bSwResetF = FALSE;
		bSwReady = FALSE; bSwReadyF = FALSE;
	}
};


class CManagerFeeding : public CWnd
{
	CWnd* m_pParent;
	CMpDevice* m_pMpe;

	BOOL m_bTIM_INIT_FEEDING;
	BOOL m_bTIM_SCAN_MPE;
	int m_nStepInitFeeding;
	BOOL m_bCycleStopF;
	CString m_sAoiUpAlarmReStartMsg, m_sAoiDnAlarmReStartMsg;
	CString m_sAoiUpAlarmReTestMsg, m_sAoiDnAlarmReTestMsg;
	int m_nMonAlmF, m_nClrAlmF;
	BOOL m_bEngStSw, m_bEng2dStSw;

	BOOL Init();
	BOOL Create();
	void Free();
	void CntMk();
	void GetMpeIO();
	void GetMpeSignal();
	void ChkEmg();
	BOOL ChkSaftySen();
	unsigned long ChkDoor();
	void ChkRcvSig();
	void ChkReadyDone();

	void MonDispMain();

	void DoIO();
	void DoEmgSens();		// SliceIo[4]
	void DoSaftySens();
	void DoDoorSens();
	void DoModeSel();		// SliceIo[0]
	void DoMainSw();		// SliceIo[0]
	void DoEngraveSens();	// MpeIo[28]
	void MonPlcAlm();
	void MonPlcSignal();
	void PlcAlm(BOOL bMon, BOOL bClr);
	void FindAlarm();
	void ResetMonAlm();
	void ClrAlarm();
	void ChkReTestAlarmOnAoiUp();
	void ChkReTestAlarmOnAoiDn();

	void DoAutoEng();
	void DoAtuoGetEngStSignal();
	void DoAtuoGet2dReadStSignal();


public:
	CManagerFeeding(CWnd* pParent = NULL);
	~CManagerFeeding();

	// Ư���Դϴ�.
public:
	stBtnPush Btn;
	stStatusFeeding Status;


	// �۾��Դϴ�.
public:
	void Reset();
	BOOL InitAct();
	BOOL IsAuto();
	long GetMpeData(int nSection, int nName);
	BOOL GetMpeSignal(int nSection, int nName);
	BOOL MpeWrite(CString strRegAddr, long lData, BOOL bCheck = FALSE);
	void Buzzer(BOOL bOn, int nCh = 0);
	void SetLed(int nIdx, BOOL bOn = TRUE);

	void SetBufInitPos(double dPos);
	void SetBufHomeParam(double dVel, double dAcc);

	BOOL IsRun();
	BOOL IsStop();
	BOOL IsReady();
	BOOL IsInitRun();
	void ResetReady();
	void Stop();
	void CycleStop();

	BOOL IsLoaderOnAoiUp();
	BOOL IsLoaderOnAoiDn();
	BOOL IsTestingAoiUp();
	BOOL IsTestingAoiDn();
	BOOL IsDoneWriteFdOffsetAoiUp();
	BOOL IsDoneWriteFdOffsetAoiDn();
	BOOL IsDoneWriteFdOffsetEng();
	CString GetAoiAlarmReStartMsg(int nId);	// Up, Dn
	CString GetAoiAlarmReTestMsg(int nId);	// Up, Dn

	// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

