#pragma once
#include "afxwin.h"
#include "Global/GlobalDefine.h"
#include "Process/CamMaster.h"
#include "Process/ReelMap.h"
#include "Process/DataMarking.h"

#define TIM_INIT_REELMAP			0

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

class CManagerReelmap : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_REELMAP;
	int m_nStepInitReelmap;

	void Init();
	BOOL Create();
	void LoadConfig();
	BOOL LoadDefectTableIni();

public:
	CManagerReelmap(CWnd* pParent = NULL);
	~CManagerReelmap();

	// Ư���Դϴ�.
public:
	stPcrShare PcrShare[2];
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


	// ������ �޽��� �� �Լ�
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

