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

public:
	CManagerReelmap(CWnd* pParent = NULL);
	~CManagerReelmap();

	// Ư���Դϴ�.
public:
	char m_cBigDefCode[MAX_DEF];
	char m_cSmallDefCode[MAX_DEF];
	CString m_sKorDef[MAX_DEF], m_sEngDef[MAX_DEF];
	char m_cBigDef[MAX_DEF], m_cSmallDef[MAX_DEF];
	COLORREF m_rgbDef[MAX_DEF];
	int m_nOdr[MAX_DEF];
	int m_nBkColor[3]; //RGB


	// ���� �۾��� �����ͱ��� ===================================================================
	BOOL m_bCamChged;
	CCamMaster m_Master[2];

	CReelMap* m_pReelMapDisp;
	CReelMap* m_pReelMap;
	CReelMap *m_pReelMapUp, *m_pReelMapDn, *m_pReelMapAllUp, *m_pReelMapAllDn;
	//CDataMarking* m_pPcr[MAX_PCR][MAX_PCR_PNL];	//����ȭ��ǥ�ø� ���� ������	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

	// ���� �۾��� �����ͱ���  ====================================================================
	CCamMaster m_MasterInner[2];

	CReelMap* m_pReelMapInner;
	CReelMap *m_pReelMapInnerUp, *m_pReelMapInnerDn, *m_pReelMapInnerAllUp, *m_pReelMapInnerAllDn;
	//CDataMarking* m_pPcrInner[MAX_PCR][MAX_PCR_PNL];	//����ȭ��ǥ�ø� ���� ������	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

	CReelMap* m_pReelMapIts;
	//CDataMarking* m_pPcrIts[MAX_PCR_PNL];				//����ȭ��ǥ�ø� ���� ������	// ������ merging

	//=============================================================================================


	// �۾��Դϴ�.
public:
	void Reset();
	BOOL InitAct();
	void SetRgbDef(int nDef, COLORREF rgbVal);
	void ClrFixPcs();
	void RestoreReelmap();

	int LoadPcrUp(int nSerial);			// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)
	int LoadPcrDn(int nSerial);			// return : 2(Failed), 1(����), -1(Align Error, �뱤�ҷ�), -2(Lot End)

	int GetVsBufLastSerial();
	int GetVsUpBufLastSerial();
	int GetVsDnBufLastSerial();

	void MakeResultMDS();

	BOOL InitReelmap();
	BOOL InitReelmapUp();
	BOOL InitReelmapDn();
	BOOL InitReelmapInner();
	BOOL InitReelmapInnerUp();
	BOOL InitReelmapInnerDn();
	void SetReelmapInner(int nDir = ROT_NONE);

	BOOL OpenReelmapFromBuf(int nSerial);
	void OpenReelmap();
	void OpenReelmapUp();
	void OpenReelmapDn();
	void OpenReelmapInner();
	void OpenReelmapInnerUp();
	void OpenReelmapInnerDn();

	BOOL ReloadReelmap();
	BOOL ReloadReelmap(int nSerial);
	BOOL IsDoneReloadReelmap(int& nProc);

	BOOL ReloadReelmapInner();
	BOOL ReloadReelmapInner(int nSerial);
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

	BOOL UpdateReelmap(int nSerial);	// �ø��������� ������ ������ ���� 
	void UpdateRMapInnerUp();
	void UpdateRMapInnerDn();
	void UpdateRMapInnerAllUp();
	void UpdateRMapInnerAllDn();

	// ITS
	CString GetPathReelmapIts();
	BOOL MakeItsReelmapHeader();	// ������ ��¡�� ���� ���
	BOOL WriteIts(int nItsSerial);
	BOOL MakeItsFile(int nSerial, int nLayer);			// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
	CString GetItsFileData(int nSerial, int nLayer);	// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
	BOOL MakeDirIts();
	BOOL MakeItsFile(int nSerial);

	BOOL m_bThreadAliveFinalCopyItsFiles, m_bRtnThreadFinalCopyItsFiles;
	CThreadTask m_ThreadTaskFinalCopyItsFiles; // CThreadTask class, handles the threading code
	void StartThreadFinalCopyItsFiles();
	void StopThreadFinalCopyItsFiles();
	BOOL FinalCopyItsFiles();
	BOOL CopyItsFile(CString sPathSrc, CString sPathDest);
	static BOOL ThreadProcFinalCopyItsFiles(LPVOID lpContext);

	// ������ �޽��� �� �Լ�
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

