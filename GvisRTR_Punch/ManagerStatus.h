#pragma once

struct stGeneral
{
	BOOL bLotEnd, bLastProc, bMkSt, bCam, bLastProcFromUp;
	BOOL bLastProcFromEng, bCycleStop, bSerialDecrese;
	BOOL bContDiffLot, bDoneChgLot, bUseRTRYShiftAdjust;
	BOOL bUpdateYield, bUpdateYieldOnRmap;
	BOOL bSkipAlign, bReAlign;
	BOOL bFailAlign, bDoMk, bDoneMk, bReMark, bRejectDone, bAnswer;
	BOOL bInitAuto;

	int nLotEndAuto, nMkStAuto, nPrevMkStAuto, nLastProcAuto, nTestMode, nLotEndSerial;
	int nPrevStepAuto, nMkStrip, nSaveMk0Img, nSaveMk1Img;

	CString sNewLotUp;

	stGeneral()
	{
		bLotEnd = FALSE; bLastProc = FALSE; bMkSt = FALSE; bCam = FALSE; bLastProcFromUp = FALSE;
		bLastProcFromEng = FALSE; bCycleStop = FALSE; bSerialDecrese = FALSE;
		bContDiffLot = FALSE; bDoneChgLot = FALSE; bUseRTRYShiftAdjust = FALSE;
		bUpdateYield = FALSE; bUpdateYieldOnRmap = FALSE;
		bSkipAlign = FALSE; bReAlign = FALSE;
		bFailAlign = FALSE; bDoMk = FALSE; bDoneMk = FALSE;
		bReMark = FALSE; bRejectDone = FALSE; bAnswer = FALSE;
		bInitAuto = FALSE;

		nLotEndAuto = 0; nMkStAuto = 0; nPrevMkStAuto = 0;
		nLastProcAuto = 0; nTestMode = 0; nLotEndSerial = 0;
		nPrevStepAuto = 0; nMkStrip = 0;
		nSaveMk0Img = 0; nSaveMk1Img = 0;

		sNewLotUp = _T("");
	}
};

struct stThread
{
	BOOL bTHREAD_DISP_DEF, bTHREAD_SHIFT2MK;
	BOOL bTHREAD_UPDATE_REELMAP_UP, bTHREAD_UPDATE_REELMAP_DN, bTHREAD_UPDATE_REELMAP_ALLUP;
	BOOL bTHREAD_UPDATE_REELMAP_ALLDN, bTHREAD_UPDATE_REELMAP_INNER_UP;
	BOOL bTHREAD_UPDATE_REELMAP_INNER_DN, bTHREAD_UPDATE_REELMAP_INNER_ALLUP, bTHREAD_UPDATE_REELMAP_INNER_ALLDN;
	BOOL bTHREAD_DISP_DEF_INNER;
	BOOL bTHREAD_UPDATAE_YIELD[2], bTHREAD_UPDATE_YIELD_UP, bTHREAD_UPDATE_YIELD_DN;
	BOOL bTHREAD_UPDATE_YIELD_ALLUP, bTHREAD_UPDATE_YIELD_ALLDN;
	BOOL bTHREAD_REELMAP_YIELD_UP, bTHREAD_REELMAP_YIELD_DN, bTHREAD_REELMAP_YIELD_ALLUP, bTHREAD_REELMAP_YIELD_ALLDN;
	BOOL bTHREAD_REELMAP_YIELD_ITS;

	int nStepTHREAD_DISP_DEF, nStepTHREAD_DISP_DEF_INNER;

	stThread()
	{
		bTHREAD_DISP_DEF = FALSE; bTHREAD_SHIFT2MK = FALSE;
		bTHREAD_UPDATE_REELMAP_UP = FALSE; bTHREAD_UPDATE_REELMAP_DN = FALSE;
		bTHREAD_UPDATE_REELMAP_ALLUP = FALSE; bTHREAD_UPDATE_REELMAP_ALLDN = FALSE;
		bTHREAD_UPDATE_REELMAP_INNER_UP = FALSE; bTHREAD_UPDATE_REELMAP_INNER_DN = FALSE;
		bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE; bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE;
		bTHREAD_DISP_DEF_INNER = FALSE;
		bTHREAD_UPDATAE_YIELD[0] = FALSE; bTHREAD_UPDATAE_YIELD[1] = FALSE;
		bTHREAD_UPDATE_YIELD_UP = FALSE; bTHREAD_UPDATE_YIELD_DN = FALSE;
		bTHREAD_UPDATE_YIELD_ALLUP = FALSE; bTHREAD_UPDATE_YIELD_ALLDN = FALSE;
		bTHREAD_REELMAP_YIELD_UP = FALSE; bTHREAD_REELMAP_YIELD_DN = FALSE;
		bTHREAD_REELMAP_YIELD_ALLUP = FALSE; bTHREAD_REELMAP_YIELD_ALLDN = FALSE;
		bTHREAD_REELMAP_YIELD_ITS = FALSE;

		nStepTHREAD_DISP_DEF = 0; nStepTHREAD_DISP_DEF_INNER = 0;
	}
};

struct stEngrave
{
	BOOL IsOnMking, IsMkDone, IsOnRead2d, IsRead2dDone;

	stEngrave()
	{
		IsOnMking = FALSE; IsMkDone = FALSE; IsOnRead2d = FALSE; IsRead2dDone = FALSE;
	}
};


class CManagerStatus
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_STATUS;
	int m_nStepInitStatus;


public:
	CManagerStatus();
	~CManagerStatus();

	// 특성입니다.
public:
	stGeneral General;
	stThread Thread;
	stEngrave Engrave;

	// 작업입니다.
public:

};

