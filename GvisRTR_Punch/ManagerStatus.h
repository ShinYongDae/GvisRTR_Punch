#pragma once
#include "afxwin.h"

#define BUF_SZ						50

struct stGeneral
{
	BOOL bLotEnd, bLotEndF, bLastProc, bMkSt, bCam, bLastProcFromUp;
	BOOL bLastProcFromEng, bCycleStop, bSerialDecrese;
	BOOL bContDiffLot, bDoneChgLot, bUseRTRYShiftAdjust;
	BOOL bUpdateYield, bUpdateYieldOnRmap;
	BOOL bSkipAlign, bReAlign;
	BOOL bFailAlign, bDoMk, bDoneMk, bReMark, bRejectDone, bAnswer;
	BOOL bInitAuto, bLoadMstInfo;
	BOOL bBufEmpty[2], bBufEmptyF[2];							// [0] : Up, [1] : Dn
	BOOL bCont, bAoiTest[2], bWaitPcr[2], bAoiFdWrite[2], bEngTest, bEngFdWrite;
	BOOL bReadyDone;

	int nLotEndAuto, nMkStAuto, nPrevMkStAuto, nLastProcAuto, nTestMode, nLotEndSerial;
	int nStepAuto, nPrevStepAuto, nMkStrip, nSaveMk0Img, nSaveMk1Img;
	int nTotMk[2], nPrevTotMk[2], nMkPcs[2], nPrevMkPcs[2];		// [0] : Up, [1] : Dn
	int nAoiCamInfoStrPcs[2];									// [0] : Up, [1] : Dn

	CString sNewLotUp;

	stGeneral()
	{
		bLotEnd = FALSE; bLotEndF = FALSE; bLastProc = FALSE; bMkSt = FALSE; bCam = FALSE; bLastProcFromUp = FALSE;
		bLastProcFromEng = FALSE; bCycleStop = FALSE; bSerialDecrese = FALSE;
		bContDiffLot = FALSE; bDoneChgLot = FALSE; bUseRTRYShiftAdjust = FALSE;
		bUpdateYield = FALSE; bUpdateYieldOnRmap = FALSE;
		bSkipAlign = FALSE; bReAlign = FALSE;
		bFailAlign = FALSE; bDoMk = FALSE; bDoneMk = FALSE;
		bReMark = FALSE; bRejectDone = FALSE; bAnswer = FALSE;
		bInitAuto = FALSE; bLoadMstInfo = FALSE;
		bBufEmpty[0] = FALSE; bBufEmptyF[0] = FALSE;
		bBufEmpty[1] = FALSE; bBufEmptyF[1] = FALSE;
		bCont = FALSE; bAoiTest[0] = FALSE; bAoiTest[1] = FALSE; bWaitPcr[0] = FALSE; bWaitPcr[1] = FALSE;
		bAoiFdWrite[0] = FALSE; bAoiFdWrite[1] = FALSE; bEngTest = FALSE; bEngFdWrite = FALSE;
		bReadyDone = FALSE;

		nLotEndAuto = 0; nMkStAuto = 0; nPrevMkStAuto = 0;
		nLastProcAuto = 0; nTestMode = 0; nLotEndSerial = 0;
		nStepAuto = 0; nPrevStepAuto = 0; nMkStrip = 0;
		nSaveMk0Img = 0; nSaveMk1Img = 0;
		nTotMk[0] = 0; nTotMk[1] = 0;
		nPrevTotMk[0] = 0; nPrevTotMk[1] = 0;
		nMkPcs[0] = 0; nMkPcs[1] = 0;
		nPrevMkPcs[0] = 0; nPrevMkPcs[1] = 0;
		nAoiCamInfoStrPcs[0] = -1; nAoiCamInfoStrPcs[1] = -1;

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

struct stListBuf
{
	int nTot;
	int* nSerial;

	stListBuf()
	{
		nTot = 0;
		nSerial = new int[BUF_SZ];
		for (int i = 0; i < BUF_SZ; i++)
			nSerial[i] = 0;
	}
	~stListBuf()
	{
		delete[] nSerial;
	}

	BOOL stListBuf::Push(int nS)
	{
		if (nS < 1 || (nTot + 1) > BUF_SZ)
			return FALSE;
		nSerial[nTot] = nS;
		nTot++;
		return TRUE;
	}

	int stListBuf::Pop()
	{
		if (nSerial[0] < 1 || (nTot - 1) < 0)
			return 0;

		int nS = nSerial[0];
		for (int i = 0; i < (nTot - 1); i++)
			nSerial[i] = nSerial[i + 1];
		nTot--;
		return nS;
	}

	int stListBuf::GetLast()
	{
		if (nSerial[0] < 1 || (nTot - 1) < 0)
			return 0;
		return nSerial[nTot - 1];
	}

	void stListBuf::Clear()
	{
		nTot = 0;
		for (int i = 0; i < BUF_SZ; i++)
			nSerial[i] = 0;
	}

};


class CManagerStatus : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_STATUS;
	int m_nStepInitStatus;

	void Init();
	BOOL Create();

public:
	CManagerStatus(CWnd* pParent = NULL);
	~CManagerStatus();

	// 특성입니다.
public:
	stGeneral General;
	stThread Thread;
	stEngrave Engrave;
	stListBuf ListBuf[2]; // [0]:AOI-Up , [1]:AOI-Dn

	// 작업입니다.
public:
	void Reset();

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

};

