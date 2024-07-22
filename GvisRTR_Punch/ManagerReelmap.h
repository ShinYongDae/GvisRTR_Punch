#pragma once

#include "afxwin.h"

#include "Global/GlobalDefine.h"
#include "Process/CamMaster.h"
#include "Process/ReelMap.h"
#include "Process/DataMarking.h"

#define TIM_INIT_REELMAP			0

class CManagerReelmap : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_REELMAP;
	int m_nStepInitReelmap;

	void Init();
	BOOL Create();
	void LoadConfig();
	BOOL LoadDefectTableIni();

	void GetResult();
	void MakeResult();
	void MakeResultIts();
	void MakeSapp3();
	BOOL RemakeReelmap();
	BOOL RemakeReelmapInner();
	void DuplicateRmap(int nRmap);
	CString CharToString(const char *szStr);
	char* TCHARToChar(TCHAR *tszStr);
	char* StringToChar(CString str);
	char* StrToChar(CString str);
	void StrToChar(CString str, char* pCh); // char* returned must be deleted... 
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 
	BOOL DirectoryExists(LPCTSTR szPath);
	CString GetItsPath(int nSerial, int nLayer);	// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN

public:
	CManagerReelmap(CWnd* pParent = NULL);
	~CManagerReelmap();

	// 특성입니다.
public:
	char m_cBigDefCode[MAX_DEF];
	char m_cSmallDefCode[MAX_DEF];
	CString m_sKorDef[MAX_DEF], m_sEngDef[MAX_DEF];
	char m_cBigDef[MAX_DEF], m_cSmallDef[MAX_DEF];
	int m_nBkColor[3]; //RGB
	COLORREF m_rgbDef[MAX_DEF];
	int m_nOdr[MAX_DEF];


	// 현재 작업인 데이터구조 ===================================================================
	BOOL m_bCamChged;
	CCamMaster m_Master[2];

	CReelMap* m_pReelMapDisp;
	CReelMap* m_pReelMap;
	CReelMap *m_pReelMapUp, *m_pReelMapDn, *m_pReelMapAllUp, *m_pReelMapAllDn;
	//CDataMarking* m_pPcr[MAX_PCR][MAX_PCR_PNL];	//릴맵화면표시를 위한 데이터	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

	// 내층 작업한 데이터구조  ====================================================================
	CCamMaster m_MasterInner[2];

	CReelMap* m_pReelMapInner;
	CReelMap *m_pReelMapInnerUp, *m_pReelMapInnerDn, *m_pReelMapInnerAllUp, *m_pReelMapInnerAllDn;
	//CDataMarking* m_pPcrInner[MAX_PCR][MAX_PCR_PNL];	//릴맵화면표시를 위한 데이터	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

	CReelMap* m_pReelMapIts;
	//CDataMarking* m_pPcrIts[MAX_PCR_PNL];				//릴맵화면표시를 위한 데이터	// 내외층 merging

	//=============================================================================================


	// 작업입니다.
public:
	void Reset();
	BOOL InitAct();
	void SetRgbDef(int nDef, COLORREF rgbVal);
	void SetRgbBackGround(int nRed, int nGreen, int nBlue);
	void ClrFixPcs();
	void RestoreReelmap();
	int GetPcrIdx0(int nSerial, BOOL bNewLot = FALSE);	// Up - 릴맵화면 표시를 위한 Display buffer의 Shot 인덱스
	int GetPcrIdx1(int nSerial, BOOL bNewLot = FALSE);	// Dn - 릴맵화면 표시를 위한 Display buffer의 Shot 인덱스
	int LoadPcrUp(int nSerial);							// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	int LoadPcrDn(int nSerial);							// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	CfPoint GetMkPnt(int nSerial, int nMkPcs);			// pcr 시리얼, pcr 불량 피스 읽은 순서 인덱스
	CfPoint GetMkPntIts(int nSerial, int nMkPcs);		// pcr 시리얼, pcr 불량 피스 읽은 순서 인덱스
	BOOL MakeMkDir();
	void SetCompletedSerial(int nSerial);
	void ResetReelmapPath();
	BOOL ChkYield(); // 수율 양호 : TRUE , 수율 불량 : FALSE

	BOOL GetPcrInfo(CString sPath, stModelInfo &stInfo);
	void MakeResultMDS();
	void UpdateData();
	void SetPathAtBuf();
	void CropCadImgUp(int nIdxMkInfo, int nSerial, int nIdxDef);
	void CropCadImgDn(int nIdxMkInfo, int nSerial, int nIdxDef);
	void ClrMkInfo();

	BOOL InitReelmap();
	void InitReelmapUp();
	void InitReelmapDn();
	void SetReelmap(int nDir = ROT_NONE);
	void SwitchReelmapDisp(int nSelRmap);
	BOOL InitReelmapInner();
	BOOL InitReelmapInnerUp();
	BOOL InitReelmapInnerDn();
	void SetReelmapInner(int nDir = ROT_NONE);

	BOOL OpenReelmapFromBuf(int nSerial);
	BOOL OpenReelmap();
	BOOL OpenReelmapUp();
	BOOL OpenReelmapDn();
	BOOL OpenReelmapInner();
	BOOL OpenReelmapInnerUp();
	BOOL OpenReelmapInnerDn();

	BOOL ReloadReelmap();
	BOOL ReloadReelmapFromThread();
	BOOL IsDoneReloadReelmap(int& nProc);

	void ReloadReelmapUp();
	void ReloadReelmapAllUp();
	void ReloadReelmapDn();
	void ReloadReelmapAllDn();

	void ReloadReelmapUpInner();
	void ReloadReelmapAllUpInner();
	void ReloadReelmapDnInner();
	void ReloadReelmapAllDnInner();

	BOOL ReloadReelmapInner();
	BOOL IsDoneReloadReelmapInner(int& nProc);

	void UpdateProcessNum(CString sProcessNum);

	void UpdateYield(int nSerial);
	void UpdateYieldUp(int nSerial);
	void UpdateYieldDn(int nSerial);
	void UpdateYieldAllUp(int nSerial);
	void UpdateYieldAllDn(int nSerial);
	void UpdateYieldInnerUp(int nSerial);
	void UpdateYieldInnerDn(int nSerial);
	void UpdateYieldInnerAllUp(int nSerial);
	void UpdateYieldInnerAllDn(int nSerial);
	void UpdateYieldIts(int nSerial);

	BOOL UpdateReelmap(int nSerial);	// 시리얼파일의 정보로 릴맵을 만듬 
	void UpdateRMapUp();
	void UpdateRMapAllUp();
	void UpdateRMapDn();
	void UpdateRMapAllDn();

	void UpdateRMapInnerUp();
	void UpdateRMapInnerDn();
	void UpdateRMapInnerAllUp();
	void UpdateRMapInnerAllDn();

	// ITS
	CString GetItsFolderPath();
	CString GetItsTargetFolderPath();
	CString GetPathReelmapIts();
	BOOL MakeItsReelmapHeader();	// 내외층 머징된 릴맵 헤드
	BOOL WriteIts(int nItsSerial);
	BOOL MakeItsFileUp(int nSerial);
	BOOL MakeItsFileDn(int nSerial);
	BOOL MakeItsFile(int nSerial, int nLayer);			// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
	CString GetItsFileData(int nSerial, int nLayer);	// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
	BOOL MakeDirIts();
	BOOL MakeItsFile(int nSerial);
	BOOL ReloadReelmapIts();
	BOOL WriteReelmapIts();
	void DelItsAll(CString strPath);

	BOOL m_bThreadAliveFinalCopyItsFiles, m_bRtnThreadFinalCopyItsFiles;
	CThreadTask m_ThreadTaskFinalCopyItsFiles; // CThreadTask class, handles the threading code
	void StartThreadFinalCopyItsFiles();
	void StopThreadFinalCopyItsFiles();
	BOOL FinalCopyItsFiles();
	BOOL CopyItsFile(CString sPathSrc, CString sPathDest);
	static BOOL ThreadProcFinalCopyItsFiles(LPVOID lpContext);

	// 보조작업입니다.
public:
	void SetUserName(CString sName);
	void SetOnePnlLen(double dPnlLen);
	void SetTempPauseLen(double dTempPauseLen);
	void SetLotCutPosLen(double dLotCutPosLen);
	void SetPathAtBufUp();
	void SetPathAtBufDn();
	COLORREF GetRgbStcDef(int nDefColor);
	CString GetKorDef(int nDefCode);

	// 생성된 메시지 맵 함수
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

