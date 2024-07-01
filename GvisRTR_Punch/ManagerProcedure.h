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

#define FROM_DOMARK0			10
#define FROM_DOMARK1			50
#define FROM_DISPDEFIMG			100

namespace Read2dIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitRead = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, MoveInitPt = 29, ChkElec = 32, DoRead = 35,
		Verify = 37, DoneRead = 38, LotDiff = 50
	};
}

namespace Mk1PtIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitMk = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, MoveInitPt = 29, ChkElec = 32, DoMk = 35,
		Verify = 37, DoneMk = 38, LotDiff = 50
	};
}

namespace Mk2PtIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitMk = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, MoveInitPt = 29, ChkElec = 32, DoMk = 35,
		Verify = 38, DoneMk = 39, LotDiff = 50, Shift2Mk = 60
	};
}

namespace Mk4PtIdx
{
	typedef enum Index {
		Start = 2, ChkSn = 4, InitMk = 10, Move0Cam1 = 12, Move0Cam0 = 14, Align1_0 = 17, Align0_0 = 18,
		Move1Cam1 = 21, Move1Cam0 = 23, Align1_1 = 26, Align0_1 = 27, Move2Cam1 = 29, Move2Cam0 = 31,
		Align1_2 = 34, Align0_2 = 35, Move3Cam1 = 37, Move3Cam0 = 39, Align1_3 = 42, Align0_3 = 43,
		MoveInitPt = 45, ChkElec = 48, DoMk = 51,
		Verify = 53, DoneMk = 54, LotDiff = 60, Shift2Mk = 70
	};
}

namespace AtLpVsIdx
{
	typedef enum Index {
		ChkShare = 0, ChkShareVs = 4, UpdateReelmap = 8, MakeItsFile = 12, ResetShare = 14
	};
}

class CManagerProcedure : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_PROCEDURE;
	int m_nStepInitProcedure;
	BOOL m_bMkSt, m_bMkStSw;
	int m_nMkStAuto;
	BOOL m_bLotEnd, m_bLastProc;
	int m_nLotEndAuto, m_nLastProcAuto;
	unsigned long m_Flag;
	BOOL m_bAoiTestF[2], m_bAoiFdWriteF[2], bAoiFdWriteF[2];
	BOOL m_bEngTestF, m_bEngFdWriteF, m_bCycleStopF;
	BOOL m_bDispMsgDoAuto[10];
	int m_nStepDispMsg[10];
	CString m_sFixMsg[2]; //[0]:up , [1]:dn

	BOOL Init();
	BOOL Create();

	void DoAuto();
	BOOL DoAutoGetLotEndSignal();
	BOOL DoAtuoGetMkStSignal();
	void DoAutoSetLastProcAtPlc();
	void DoAutoSetFdOffsetLastProc();
	void DoAutoSetFdOffset();
	void DoAutoSetFdOffsetEngrave();
	void DoAutoChkCycleStop();
	void DoAutoDispMsg();
	void DoAutoChkShareVsFolder();
	void DoAutoChkShareFolder();
	void DoAutoMarking();
	void DoAutoMarkingEngrave();

	BOOL ReloadReelmap(int nSerial);
	void UpdateRst();
	void LotEnd();
	BOOL ChkLastProcFromUp();
	BOOL ChkLastProcFromEng();

public:
	CManagerProcedure(CWnd* pParent = NULL);
	~CManagerProcedure();

	// 특성입니다.
public:
	CEngrave* m_pEngrave;


	// 작업입니다.
public:
	void Reset();
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
	BOOL IsRun();
	BOOL IsLotEnd();
	void SetAoiUpAlarmRestartMsg(CString sAoiUpAlarmReStartMsg);		// m_sAoiUpAlarmReStartMsg
	void SetAoiDnAlarmRestartMsg(CString sAoiDnAlarmReStartMsg);		// m_sAoiDnAlarmReStartMsg
	void SetAoiUpAlarmReTestMsg(CString sAoiUpAlarmReTestMsg);			// m_sAoiUpAlarmReTestMsg
	void SetAoiDnAlarmReTestMsg(CString sAoiDnAlarmReTestMsg);			// m_sAoiDnAlarmReTestMsg

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

