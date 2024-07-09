#pragma once
#include "afxwin.h"

#ifndef MAX_STRIP
	#define MAX_STRIP				4
#endif

#define BUF_SZ						50

struct stGeneral
{
	BOOL bLotEnd, bLotEndF, bLastProc, bMkSt, bCam, bLastProcFromUp;
	BOOL bLastProcFromEng, bCycleStop, bSerialDecrese;
	BOOL bContDiffLot, bDoneChgLot, bUseRTRYShiftAdjust;
	BOOL bUpdateYield, bUpdateYieldOnRmap;
	BOOL bSkipAlign, bReAlign;
	BOOL bFailAlign, bDoMk, bDoneMk, bReMark, bAnswer;
	BOOL bInitAuto, bLoadMstInfo;
	BOOL bBufEmpty[2], bBufEmptyF[2];							// [0] : Up, [1] : Dn
	BOOL bCont, bAoiTest[2], bWaitPcr[2], bAoiFdWrite[2], bEngTest, bEngFdWrite;
	BOOL bReadyDone, bStopF_Verify, bInitAutoLoadMstInfo;

	int nLotEndAuto, nMkStAuto, nPrevMkStAuto, nLastProcAuto, nTestMode, nLotEndSerial;
	int nStepAuto, nPrevStepAuto, nSaveMk0Img, nSaveMk1Img;
	int nTotMk[2], nPrevTotMk[2], nPrevMkPcs[2];		// [0] : Up, [1] : Dn
	int nAoiCamInfoStrPcs[2];									// [0] : Up, [1] : Dn

	int nStepMk[4], nMkPcs[4]; 	// [0] Auto-Left, [1] Auto-Right, [2] Manual-Left, [3] Manual-Right  ; m_nStepMk(마킹Sequence), nMkOrderIdx(마킹한 count)
	int nMkStrip[2][MAX_STRIP]; // [nCam][nStrip] - [좌/우][] : 스트립에 펀칭한 피스 수 count
	BOOL bRejectDone[2][MAX_STRIP]; // Shot[2], Strip[4] - [좌/우][] : 스트립에 펀칭한 피스 수 count가 스트립 폐기 설정수 완료 여부 
	int nEjectBufferLastShot;

	CString sNewLotUp;

	stGeneral()
	{
		bLotEnd = FALSE; bLotEndF = FALSE; bLastProc = FALSE; bMkSt = FALSE; bCam = FALSE; bLastProcFromUp = FALSE;
		bLastProcFromEng = FALSE; bCycleStop = FALSE; bSerialDecrese = FALSE;
		bContDiffLot = FALSE; bDoneChgLot = FALSE; bUseRTRYShiftAdjust = FALSE;
		bUpdateYield = FALSE; bUpdateYieldOnRmap = FALSE;
		bSkipAlign = FALSE; bReAlign = FALSE;
		bFailAlign = FALSE; bDoMk = FALSE; bDoneMk = FALSE;
		bReMark = FALSE; bAnswer = FALSE;
		bInitAuto = FALSE; bLoadMstInfo = FALSE;
		bBufEmpty[0] = FALSE; bBufEmptyF[0] = FALSE;
		bBufEmpty[1] = FALSE; bBufEmptyF[1] = FALSE;
		bCont = FALSE; bAoiTest[0] = FALSE; bAoiTest[1] = FALSE; bWaitPcr[0] = FALSE; bWaitPcr[1] = FALSE;
		bAoiFdWrite[0] = FALSE; bAoiFdWrite[1] = FALSE; bEngTest = FALSE; bEngFdWrite = FALSE;
		bReadyDone = FALSE; bStopF_Verify = FALSE; bInitAutoLoadMstInfo = FALSE;

		nLotEndAuto = 0; nMkStAuto = 0; nPrevMkStAuto = 0;
		nLastProcAuto = 0; nTestMode = 0; nLotEndSerial = 0;
		nStepAuto = 0; nPrevStepAuto = 0; 
		nSaveMk0Img = 0; nSaveMk1Img = 0;
		nTotMk[0] = 0; nTotMk[1] = 0;
		nPrevTotMk[0] = 0; nPrevTotMk[1] = 0;
		nMkPcs[0] = 0; nMkPcs[1] = 0;
		nPrevMkPcs[0] = 0; nPrevMkPcs[1] = 0;
		nAoiCamInfoStrPcs[0] = -1; nAoiCamInfoStrPcs[1] = -1;

		nStepMk[0] = 0; nStepMk[1] = 0; nStepMk[2] = 0; nStepMk[3] = 0;
		nMkPcs[0] = 0; nMkPcs[1] = 0; nMkPcs[2] = 0; nMkPcs[3] = 0;
		nMkStrip[0][0] = 0; nMkStrip[0][1] = 0; nMkStrip[0][2] = 0; nMkStrip[0][3] = 0;
		bRejectDone[0][0] = FALSE; bRejectDone[0][1] = FALSE; bRejectDone[0][2] = FALSE; bRejectDone[0][3] = FALSE;
		nMkStrip[1][0] = 0; nMkStrip[1][1] = 0; nMkStrip[1][2] = 0; nMkStrip[1][3] = 0;
		bRejectDone[1][0] = FALSE; bRejectDone[1][1] = FALSE; bRejectDone[1][2] = FALSE; bRejectDone[1][3] = FALSE;
		nEjectBufferLastShot = -1;

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

struct stPcrShare
{
	BOOL bExist;
	int nSerial;
	CString sModel, sLayer, sLot, sItsCode, sPrcsCode;

	stPcrShare()
	{
		bExist = FALSE;
		nSerial = 0;
		sModel = _T(""); sLayer = _T(""); sLot = _T(""); sItsCode = _T(""); sPrcsCode = _T("");
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
	stPcrShare PcrShare[2], PcrShareVs[2];

	// 작업입니다.
public:
	void Reset();

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

};

