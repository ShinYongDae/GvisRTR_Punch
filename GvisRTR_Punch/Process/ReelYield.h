#pragma once
#include "afxwin.h"

#include "../Global/GlobalDefine.h"

struct stResult
{
	CString sMachin, sOpName, sProcessNum;
	CString sModel, sLot, sLayerUp, sLayerDn;
	CString strLotStartTime, strLotEndTime, strLotWorkingTime;
	int nEntirePieceNum, nEntireStripNum, nEntireShotNum;
	int nEntireStripDef[MAX_STRIP];
	int nGoodPieceNum, nDefectPieceNum;
	int nDefStrip[MAX_STRIP], nDefPerStrip[MAX_STRIP][MAX_DEF];
	int nEntireAddedDefect[MAX_DEF];
	int nSerialSt, nSerialEd;
	int nStripOut[MAX_STRIP], nTotStOut;
	double dEntireSpeed;

	stResult()
	{
		sMachin = _T(""); sOpName = _T(""); sProcessNum = _T("");
		sModel = _T(""); sLot = _T(""); sLayerUp = _T(""); sLayerDn = _T("");
		strLotStartTime = _T(""); strLotEndTime = _T(""); strLotWorkingTime = _T("");
		nEntirePieceNum = 0; nEntireStripNum = 0; nEntireShotNum = 0;
		nGoodPieceNum = 0; nDefectPieceNum = 0;
		nSerialSt = 0; nSerialEd = 0;
		nTotStOut = 0;
		dEntireSpeed = 0.0;

		for (int nStrip = 0; nStrip < MAX_STRIP; nStrip++)
		{
			nEntireStripDef[nStrip] = 0;
			nDefStrip[nStrip] = 0;
			nStripOut[nStrip] = 0;

			for (int nDef = 0; nDef < MAX_DEF; nDef++)
			{
				nDefPerStrip[nStrip][nDef] = 0;
				if (!nStrip)
					nEntireAddedDefect[nDef] = 0;
			}
		}
	}
};


class CReelYield : public CWnd
{
	CWnd* m_pParent;
	CString m_sPathYield;

	int m_nStartSerial;
	int m_nBeforeSerial;

	void Init();
	BOOL DirectoryExists(LPCTSTR szPath);
	char* StrToChar(CString str);
	void StrToChar(CString str, char* pCh);

public:
	CReelYield(int nLayer, int nPnl = 0, int nPcs = 0, int nDir = 0, CWnd* pParent = NULL);
	~CReelYield();

	// 특성입니다.
public:
	stResult m_stResult;
	stYield m_stYield;

	int m_nLayer;
	int nTotPnl;
	int nTotPcs;
	int nDir;

	// 작업입니다.
public:
	void Reset();
	BOOL Update(int nSerial);
	void UpdateTotalSpeed(CString sSpeed, int nLayer);

	CString GetPath(int nRmap);
	int GetDefNum(int nDefCode);
	int GetDefStrip(int nStrip);
	int GetDefStrip(int nStrip, int nDefCode);
	int GetStripOut(int nStrip);
	void GetPcsNum(int &nGood, int &nBad);

	BOOL MakeDir();
	BOOL MakeDir(CString sPath);

	BOOL Read(int nSerial, CString sPath);
	BOOL Write(int nSerial, CString sPath);

	BOOL GetResult(); // TRUE: Make Result, FALSE: Load Result or Failed.
	CString GetResultTxt();
	CString GetSapp3Txt();
	CString GetSapp3TxtReverse();
};

