#if !defined(AFX_REELMAP_H__F5AB69D1_0DF4_4D15_985A_1421D64577D1__INCLUDED_)
#define AFX_REELMAP_H__F5AB69D1_0DF4_4D15_985A_1421D64577D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReelMap.h : header file
//

#define MAX_DEFINFO		12

#ifndef MAX_PCS
#define MAX_PCS			10000
#endif

#include "../Global/GlobalDefine.h"
#include "CamMasterDefine.h"
#include "PcsRgn.h"
#include "DataMarking.h"
#include "ThreadTask.h"
#include "ReelYield.h"

#define FIX_PCS_SHOT_MAX	500
#define FIX_PCS_COL_MAX		100
#define FIX_PCS_ROW_MAX		100

#pragma warning(disable: 4244)
#pragma warning(disable: 4996)
#pragma warning(disable: 4477)



//struct stResult
//{
//	CString sMachin, sOpName, sProcessNum;
//	CString sModel, sLot, sLayerUp, sLayerDn;
//	CString strLotStartTime, strLotEndTime, strLotWorkingTime;
//	int nEntirePieceNum, nEntireStripNum;
//	int nEntireStripDef[MAX_STRIP];
//	int nGoodPieceNum, nDefectPieceNum;
//	int nDefStrip[MAX_STRIP], nDefPerStrip[MAX_STRIP][MAX_DEF];
//	int nEntireAddedDefect[MAX_DEF];
//	int nSerialSt, nSerialEd;
//	int nStripOut[MAX_STRIP], nTotStOut;
//	double dEntireSpeed;
//
//	stResult()
//	{
//		sMachin = _T(""); sOpName = _T(""); sProcessNum = _T("");
//		sModel = _T(""); sLot = _T(""); sLayerUp = _T(""); sLayerDn = _T("");
//		strLotStartTime = _T(""); strLotEndTime = _T(""); strLotWorkingTime = _T("");
//		nEntirePieceNum = 0; nEntireStripNum = 0;
//		nGoodPieceNum = 0; nDefectPieceNum = 0;
//		nSerialSt = 0; nSerialEd = 0;
//		nTotStOut = 0;
//		dEntireSpeed = 0.0;
//
//		for (int nStrip = 0; nStrip < MAX_STRIP; nStrip++)
//		{
//			nEntireStripDef[nStrip] = 0;
//			nDefStrip[nStrip] = 0;
//			nStripOut[nStrip] = 0;
//
//			for (int nDef = 0; nDef < MAX_DEF; nDef++)
//			{
//				nDefPerStrip[nStrip][nDef] = 0;
//				if(!nStrip)
//					nEntireAddedDefect[nDef] = 0;
//			}
//		}
//	}
//};


/////////////////////////////////////////////////////////////////////////////
// CReelMap window

class CReelMap : public CReelYield //public CWnd
{
	BOOL m_FixPcs[FIX_PCS_SHOT_MAX][FIX_PCS_COL_MAX][FIX_PCS_ROW_MAX]; // [Col][Row]
	int m_FixPcsPrev[FIX_PCS_COL_MAX][FIX_PCS_ROW_MAX]; // [Col][Row]
	int m_FixPcsPrevStSerial[FIX_PCS_COL_MAX][FIX_PCS_ROW_MAX]; // [Col][Row]
	int m_FixPcsRpt[FIX_PCS_COL_MAX][FIX_PCS_ROW_MAX]; // [Col][Row]
	int m_nPrevSerial[2]; // [0] : -- , [1] : ++
	//int m_nPnlBuf;															// 메모리에 할당된 총 Shot수
	int m_nTotPcs, m_nGoodPcs, m_nBadPcs, m_nDef[MAX_DEF];	// [DefCode] : Total Num.
	int m_nDefStrip[MAX_STRIP], m_nDefPerStrip[MAX_STRIP][MAX_DEF];
	int m_nStripOut[MAX_STRIP], m_nTotStOut;
	CString m_sPathShare, m_sPathBuf, m_sIpPath;
	//CString m_sPathYield;
	//double m_dAdjRatio; // Master Image의 Pixel 해상도에 따른 Reelmap에서의 식별용 간격 비율.
	int m_nIdxDefInfo;	// MAX_DEFINFO에 들어가는 정보의 Index.
	BOOL m_bContFixDef;
	//int m_nBeforeSerial;

	CDataMarking* m_pPcr[MAX_PCR_PNL];	//릴맵화면표시를 위한 데이터	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	DWORD m_dwLotSt, m_dwLotEd;
	//int m_nStartSerial;

	int MirrorLR(int nPcsId); // 좌우 미러링
	int MirrorUD(int nPcsId); // 상하 미러링
	int Rotate180(int nPcsId); // 180도 회전
	BOOL DirectoryExists(LPCTSTR szPath);

	void LoadConfig();
	BOOL MakeDirRmap();
	CString MakeDirRmapRestore();
	int GetLastRmapRestoreDir(CString strPath);

	char* StrToChar(CString str);
	void StrToChar(CString str, char* pCh);
	
	//BOOL ReadYield(int nSerial, CString sPath);
	//BOOL WriteYield(int nSerial, CString sPath);
	
	//BOOL LoadDefectTableDB();

	//BOOL MakeDirYield(CString sPath);

	int GetPcrIdx(int nSerial);

// Construction
public:
	CReelMap(int nLayer, int nPnl=0, int nPcs=0, CWnd* pParent = NULL);

// Attributes
public:
	CCriticalSection m_cs;
	//CReelYield* Yield;

	double m_dAdjRatio; // Master Image의 Pixel 해상도에 따른 Reelmap에서의 식별용 간격 비율.
	CString m_sMc, m_sUser;
// 	CString m_sModel, m_sLayer, m_sLot;
	int m_nSerial; // On marking Serial.

	//int m_nLayer;
	//int nTotPnl;
	//int nTotPcs;
	//int nDir;

	int *m_pPnlNum, *m_pPnlDefNum;
	CRect *pFrmRgn;
	CRect **pPcsRgn;

	int **pPcsDef; // [DispPnlIdx][PcsID] : 불량코드.

// 	CString *pMkInfo;

	int m_nSelMarkingPnl;
	double m_dTotLen, m_dPnlLen, m_dLotLen, m_dTempPauseLen, m_dLotCutPosLen;
	BOOL m_bUseLotSep, m_bUseTempPause;
	int m_nLastShot, m_nCompletedShot;
	double m_dProgressRatio;

	int m_nBkColor[3]; //RGB
	COLORREF m_rgbDef[MAX_DEF];


// Operations
public:
	int LoadPCR(int nSerial);
	void SetLotStartTime(DWORD dwTime);
	void SetLotEndTime(DWORD dwTime);

	void SetPnlNum(int *pPnlNum=NULL);
	void IncPnlNum();
	void AddPnlNum(int nNum);
	void SelMarkingPnl(int nNum);
	void SetAdjRatio(double dRatio);
	double GetAdjRatio();

	CString GetLotSt();
	CString GetLotEd();
	CfPoint GetMkPnt(int nSerial, int nMkPcs);

	BOOL Open();
	BOOL Open(CString sPath);
	BOOL Write(int nSerial);
	BOOL Disp(int nMkPnl, BOOL bDumy=FALSE);
	void SetLastSerial(int nSerial);
	void SetLotSt();
	void SetLotEd();
	BOOL InitRst();
	void CloseRst();
	void ClrRst();
	//int GetDefNum(int nDefCode);
	//int GetDefStrip(int nStrip);
	//int GetDefStrip(int nStrip, int nDefCode);
	//void GetPcsNum(int &nGood, int &nBad);
	void ClrPnlNum();
	void Clear();
	int GetLastSerial();
	void InitPcs();
	void ClrPcs();
	void InitPcr();
	void ClrPcr();

	void ClrFixPcs();
	void ClrFixPcs(int nCol, int nRow);
	void SetFixPcs(int nSerial, int nCol, int nRow); // nShot : 0 ~ 
	BOOL IsFixPcs(int nSerial, int &Col, int &Row);
	BOOL IsFixPcs(int nSerial, int* pCol, int* pRow, int &nTot, BOOL &bCont);
	int GetRptFixPcs(int nCol, int nRow);

	//BOOL Write(int nSerial, int nLayer, CString sPath);
	void SetPathAtBuf();
	CString GetRmapPath(int nRmap);

	void SetPnlDefNum(int *pPnlDefNum);
	void ClrPnlDefNum();
	void AddPnlDefNum(int nDef);
	//void UpdateTotVel(CString sVel, int nLayer);
	void UpdateProcessNum(CString sProcessNum, int nLayer);
	//int GetStripOut(int nStrip);
	void SetFixPcs(int nSerial);

	BOOL m_bThreadAliveRemakeReelmap, m_bRtnThreadRemakeReelmap;
	CString m_sPathOnThread;
	CThreadTask m_ThreadTaskRemakeReelmap; // CThreadTask class, handles the threading code
	BOOL RemakeReelmap();
	void StartThreadRemakeReelmap();
	static BOOL ThreadProcRemakeReelmap(LPVOID lpContext);
	void StopThreadRemakeReelmap();

	void RestoreReelmap();

	BOOL m_bThreadAliveReloadReelmap, m_bRtnThreadReloadReelmap, m_bDoneReloadReelmap;
	int m_nLastOnThread, m_nProgressReloadReelmap, m_nTotalProgressReloadReelmap;
	CThreadTask m_ThreadTaskReloadReelmap; // CThreadTask class, handles the threading code
	BOOL ReloadReelmap();
	BOOL ReloadReelmap(int nTo);
	BOOL IsDoneReloadReelmap();
	int GetProgressReloadReelmap();
	void StartThreadReloadReelmap();
	static BOOL ThreadProcReloadReelmap(LPVOID lpContext);
	void StopThreadReloadReelmap();

	BOOL MakeHeader(CString sPath);
	BOOL GetNodeXYonRmap(int &nNodeX, int &nNodeY, CString sPath);

	//stResult m_stResult;
	void ResetReelmapPath();
	//BOOL GetResult(); // TRUE: Make Result, FALSE: Load Result or Failed.
	//CString GetResultTxt();
	//CString GetSapp3Txt();
	//CString GetSapp3TxtReverse();
	CString GetPath();
	CString GetIpPath();

	void SetLastSerialOnOffline(int nSerial);
	CString GetRmapPathOnOffline(int nRmap);
	BOOL WriteOnOffline(int nSerial);
	BOOL WriteLastRmapInfoOnOffline();
	int GetFirstShotFromPcr();
	int GetLastShotFromPcr();

	CString GetItsFileData(int nSerial); // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
	CString GetItsPath(int nSerial);
	CString GetTimeIts();
	CString GetItsFolderPath();

	void CropCadImg(int nIdxMkInfo, int nSerial, int nIdxDef);
	void CropCadImg(short cell, short cx, short cy, int BufID);
	BOOL SetCADCoord(int CellNum, int StX, int StY, int Coord);
	void LoadCADBuf(int CurrCell, long OrgStX, long OrgStY, long DesStX, long DesStY, long SizeX, long SizeY);
	//BOOL VicFileLoadFromMem(MIL_ID MilImage, UCHAR *pCADImg, TiffData &tdat);
	//BOOL VicGetImgInfo(UCHAR *pCADImg, TiffData &tdat);
	//void ClrOvrCad(int nIdx);

	CDataMarking* GetPcr(int nIdx);
	void SetCompletedSerial(int nSerial);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReelMap)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CReelMap();
	//short ***m_pPnlBuf;	// DefCode 3D Array : [nSerial-1][nRow][nCol] on File -> [nSerial-1][NodeX][NodeY] : Rotated Cw 90 

	// Generated message map functions
protected:
	//{{AFX_MSG(CReelMap)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REELMAP_H__F5AB69D1_0DF4_4D15_985A_1421D64577D1__INCLUDED_)
