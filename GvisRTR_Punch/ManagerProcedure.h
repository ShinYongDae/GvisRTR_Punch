#pragma once
#include "afxwin.h"

#include "Device/Engrave.h"

#define MAX_DISP				10
#define TIM_INIT_PROCEDURE		0

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
	BOOL m_bDispMsgDoAuto[MAX_DISP];
	int m_nStepDispMsg[MAX_DISP];
	CString m_sFixMsg[2]; //[0]:up , [1]:dn
	DWORD m_dwCycSt, m_dwCycTim;
	double m_dTotVel, m_dPartVel;

	BOOL m_bPcrInShare[2], m_bPcrInShareVs[2];	// [Up/Dn]
	BOOL m_bFailAlign[2][4];					// [nCam][nPos] 
	BOOL m_bReAlign[2][4];						// [nCam][nPos] 
	BOOL m_bSkipAlign[2][4];					// [nCam][nPos] 

	BOOL m_bDoMk[2];			// [nCam] : TRUE(Punching), FALSE(Stop Punching)
	BOOL m_bDoneMk[2];			// [nCam] : TRUE(Punching 완료), FALSE(Punching 미완료)
	BOOL m_bReMark[2];			// [nCam] : TRUE(Punching 다시시작), FALSE(pass)

	int m_nTotMk[2], m_nCurMk[2]; // [0]: 좌 MK, [1]: 우 MK
	int m_nPrevTotMk[2], m_nPrevCurMk[2]; // [0]: 좌 MK, [1]: 우 MK

	BOOL m_bEngSt, m_bEngStSw;
	BOOL m_bEng2dSt, m_bEng2dStSw;
	int m_nEngStAuto, m_nEng2dStAuto;

	int m_nBufUpSerial[2]; // [nCam]
	int m_nBufDnSerial[2]; // [nCam]
	int m_nBufUpCnt;
	int m_nBufDnCnt;

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

	void MarkingWith2PointAlign();
	void Mk2PtReady();
	void Mk2PtChkSerial();
	void Mk2PtInit();
	void Mk2PtAlignPt0();
	void Mk2PtAlignPt1();
	void Mk2PtMoveInitPos();
	void Mk2PtElecChk();
	void Mk2PtDoMarking();
	void Mk2PtShift2Mk();
	void Mk2PtLotDiff();
	void Mk2PtReject();
	void Mk2PtErrStop();

	void MarkingWith4PointAlign();
	void Mk4PtReady();
	void Mk4PtChkSerial();
	void Mk4PtInit();
	void Mk4PtAlignPt0();
	void Mk4PtAlignPt1();
	void Mk4PtAlignPt2();
	void Mk4PtAlignPt3();
	void Mk4PtMoveInitPos();
	void Mk4PtElecChk();
	void Mk4PtDoMarking();
	void Mk4PtLotDiff();
	void Mk4PtReject();
	void Mk4PtErrStop();

	BOOL ReloadReelmap(int nSerial);
	void UpdateRst();
	void LotEnd();
	void MakeResultMDS();

	BOOL ChkLastProc();
	BOOL ChkLastProcFromUp();
	BOOL ChkLastProcFromEng();
	void SetCycTime();
	void InitAutoEngSignal();
	void Shift2DummyBuf();
	void Shift2Buf();
	BOOL IsSetLotEnd();
	BOOL ChkStShotNum();
	void ResetMkInfo(int nAoi = 0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	BOOL IsLastJob(int nAoi); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	void SetAlignPos();
	void InitReelmap();
	void InitReelmapInner();
	void ModelChange(int nAoi = 0); // 0 : AOI-Up , 1 : AOI-Dn
	void InitReelmapDisp();
	void InitCamImgDisp();

	BOOL IsRun();
	BOOL IsAuto();
	BOOL IsReady();
	BOOL IsAoiLdRun();
	BOOL IsLotEnd();
	void Stop();
	void ResetWinker();
	void Winker(int nId, int nDly = 20); // 0:Ready, 1:Reset, 2:Run, 3:Stop
	CString GetDispMain();
	void DispMain(CString sMsg, COLORREF rgb = RGB(0, 255, 0));
	void SetAoiUpAlarmRestartMsg(CString sAoiUpAlarmReStartMsg);		// m_sAoiUpAlarmReStartMsg
	void SetAoiDnAlarmRestartMsg(CString sAoiDnAlarmReStartMsg);		// m_sAoiDnAlarmReStartMsg
	void SetAoiUpAlarmReTestMsg(CString sAoiUpAlarmReTestMsg);			// m_sAoiUpAlarmReTestMsg
	void SetAoiDnAlarmReTestMsg(CString sAoiDnAlarmReTestMsg);			// m_sAoiDnAlarmReTestMsg
	CString GetAoiUpAlarmRestartMsg();									// m_sAoiUpAlarmReStartMsg
	CString GetAoiDnAlarmRestartMsg();									// m_sAoiDnAlarmReStartMsg
	CString GetAoiUpAlarmReTestMsg();									// m_sAoiUpAlarmReTestMsg
	CString GetAoiDnAlarmReTestMsg();									// m_sAoiDnAlarmReTestMsg
	CString ReadAoiUpAlarmRestartMsg();									// m_sAoiUpAlarmReStartMsg
	CString ReadAoiDnAlarmRestartMsg();									// m_sAoiDnAlarmReStartMsg
	CString ReadAoiUpAlarmReTestMsg();									// m_sAoiUpAlarmReTestMsg
	CString ReadAoiDnAlarmReTestMsg();									// m_sAoiDnAlarmReTestMsg

	int GetCycTime();
	void MoveInitPos0(BOOL bWait = TRUE);
	void MoveInitPos1(BOOL bWait = TRUE);
	void InitIoWrite();
	void SetTest(BOOL bOn);
	void ClrMkInfo();
	void SetLastProc(int nFromMachine);
	BOOL IsSameUpDnLot();

	BOOL IsShare();
	BOOL IsShareUp();
	BOOL IsShareDn();
	int GetShareUp();
	int GetShareDn();

	BOOL IsShareVs();
	BOOL IsShareVsUp();
	BOOL IsShareVsDn();
	int GetShareVsUp();
	int GetShareVsDn();

	void SetListBuf();
	BOOL IsBuffer(int nNum = 0);

	int GetLotEndSerial();
	void SetLotEnd(int nSerial);
	void DispInfo();

	int GetAoiUpCamMstInfo(); // AOI상 strpcs.bin 연결
	int GetAoiDnCamMstInfo(); // AOI하 strpcs.bin 연결
	BOOL GetAoiInfoUp(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	BOOL GetAoiInfoDn(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	void SetCurrentInfoBufUpTot(int nTotal);
	void SetCurrentInfoBufUp(int nIdx, int nData);
	void SetCurrentInfoBufDnTot(int nTotal);
	void SetCurrentInfoBufDn(int nIdx, int nData);
	void SetMkMenu01(CString sMenu, CString sItem, CString sData);
	void SetMkMenu03(CString sMenu, CString sItem, BOOL bOn);
	BOOL OpenReelmapFromBuf(int nSerial);
	int LoadPcrUp(int nSerial);	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	int LoadPcrDn(int nSerial);	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	BOOL UpdateReelmap(int nSerial);
	BOOL MakeItsFile(int nSerial);

public:
	CManagerProcedure(CWnd* pParent = NULL);
	~CManagerProcedure();

	// 특성입니다.
public:
	CEngrave* m_pEngrave;
	BOOL m_bDoneDispMkInfo[2][2]; // [nCam][Up/Dn]

	int m_nShareUpS, m_nShareUpSprev;
	int m_nShareUpSerial[2]; // [nCam]
	int m_nShareDnS, m_nShareDnSprev;
	int m_nShareDnSerial[2]; // [nCam]
	int m_nShareUpCnt;
	int m_nShareDnCnt;


	// 작업입니다.
public:
	void Reset();
	BOOL InitAct();
	void InitAuto(BOOL bInit = TRUE); // When doReset...
	void Auto();

	BOOL GetItsSerialInfo(int nItsSerial, BOOL &bDualTest, CString &sLot, CString &sLayerUp, CString &sLayerDn, int nOption = 0);		// 내층에서의 ITS 시리얼의 정보
	BOOL GetAoiDnInfo(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	BOOL GetAoiUpInfo(int nSerial, int *pNewLot = NULL, BOOL bFromBuf = FALSE); // TRUE: CHANGED, FALSE: NO CHANGED
	int GetBufUpSerial(int nCam);
	int GetBufDnSerial(int nCam);
	void DispDefImg();

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

