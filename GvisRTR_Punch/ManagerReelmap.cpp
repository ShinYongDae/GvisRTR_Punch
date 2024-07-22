#include "stdafx.h"
#include "ManagerReelmap.h"

#include "Dialog/DlgProgress.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerReelmap::CManagerReelmap(CWnd* pParent)
{
	m_pParent = pParent;
	m_bThreadAliveFinalCopyItsFiles = FALSE;

	Reset();
	Init();

	if (!Create())
	{
		pView->ClrDispMsg();
	}
}


CManagerReelmap::~CManagerReelmap()
{
	m_bThreadAliveFinalCopyItsFiles = FALSE;
}

BEGIN_MESSAGE_MAP(CManagerReelmap, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CManagerReelmap::Create()
{
	LPCTSTR lpszClassName = NULL;
	LPCTSTR lpszWindowName = _T("None");
	DWORD dwStyle = WS_CHILD;
	const RECT& rect = CRect(0, 0, 0, 0);
	CWnd* pParentWnd = m_pParent;
	UINT nID = (UINT)this;
	CCreateContext* pContext = NULL;

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CManagerReelmap::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnTimer(nIDEvent);
}

void CManagerReelmap::Init()
{
	m_cBigDefCode[0] = '*';		//	None
	m_cBigDefCode[1] = 'N';		//	NICK
	m_cBigDefCode[2] = 'D';		//	PROTRUSION
	m_cBigDefCode[3] = 'A';		//	SPACE
	m_cBigDefCode[4] = 'O';		//	OPEN
	m_cBigDefCode[5] = 'S';		//	SHORT
	m_cBigDefCode[6] = 'U';		//	USHORT
	m_cBigDefCode[7] = 'I';		//	PINHOLE
	m_cBigDefCode[8] = 'H';		//	HOLE_MISS
	m_cBigDefCode[9] = 'E';		//	EXTRA
	m_cBigDefCode[10] = 'P';	//	PAD
	m_cBigDefCode[11] = 'L';	//	HOLE_POSITION
	m_cBigDefCode[12] = 'X';	//	POI
	m_cBigDefCode[13] = 'T';	//	VH_POSITION
	m_cBigDefCode[14] = 'M';	//	VH_MISS
	m_cBigDefCode[15] = 'F';	//	HOLE_DEFECT
	m_cBigDefCode[16] = 'C';	//	HOLE_OPEN
	m_cBigDefCode[17] = 'G';	//	VH_OPEN
	m_cBigDefCode[18] = 'V';	//	VH_DEF
	m_cBigDefCode[19] = 'K';	//	E.Nick
	m_cBigDefCode[20] = 'B';	//	E.Prot
	m_cBigDefCode[21] = 'J';	//	E.Space
	m_cBigDefCode[22] = 'Q';	//	UDD1
	m_cBigDefCode[23] = 'R';	//	Narrow
	m_cBigDefCode[24] = 'W';	//	Wide
	m_cBigDefCode[25] = '?';	//	Light

	m_cSmallDefCode[0] = '*';	//	None
	m_cSmallDefCode[1] = 'n';	//	NICK
	m_cSmallDefCode[2] = 'd';	//	PROTRUSION
	m_cSmallDefCode[3] = 'a';	//	SPACE
	m_cSmallDefCode[4] = 'o';
	m_cSmallDefCode[5] = 's';
	m_cSmallDefCode[6] = 'u';
	m_cSmallDefCode[7] = 'i';
	m_cSmallDefCode[8] = 'h';
	m_cSmallDefCode[9] = 'e';
	m_cSmallDefCode[10] = 'p';
	m_cSmallDefCode[11] = 'l';
	m_cSmallDefCode[12] = 'x';
	m_cSmallDefCode[13] = 't';
	m_cSmallDefCode[14] = 'm';
	m_cSmallDefCode[15] = 'f';
	m_cSmallDefCode[16] = 'c';
	m_cSmallDefCode[17] = 'g';
	m_cSmallDefCode[18] = 'v';
	m_cSmallDefCode[19] = 'k';	//	E.Nick
	m_cSmallDefCode[20] = 'b';	//	E.Prot
	m_cSmallDefCode[21] = 'j';	//	E.Space
	m_cSmallDefCode[22] = 'q';	//	UDD1
	m_cSmallDefCode[23] = 'r';	//	Narrow
	m_cSmallDefCode[24] = 'w';	//	Wide
	m_cSmallDefCode[25] = '?';	//	Light

	m_cBigDef[0] = '*';		//	NONE
	m_cBigDef[1] = 'N';		//	NICK
	m_cBigDef[2] = 'D';		//	PROTRUSION
	m_cBigDef[3] = 'A';		//	SPACE
	m_cBigDef[4] = 'O';		//	OPEN
	m_cBigDef[5] = 'S';		//	SHORT
	m_cBigDef[6] = 'U';		//	USHORT
	m_cBigDef[7] = 'I';		//	PINHOLE
	m_cBigDef[8] = 'H';		//	HOLE_MISS
	m_cBigDef[9] = 'E';		//	EXTRA
	m_cBigDef[10] = 'P';	//	PAD
	m_cBigDef[11] = 'L';	//	HOLE_POSITION
	m_cBigDef[12] = 'X';	//	POI
	m_cBigDef[13] = 'T';	//	VH_POSITION
	m_cBigDef[14] = 'M';	//	VH_MISS
	m_cBigDef[15] = 'F';	//	HOLE_DEFECT
	m_cBigDef[16] = 'C';	//	HOLE_OPEN
	m_cBigDef[17] = 'G';	//	VH_OPEN
	m_cBigDef[18] = 'V';	//	VH_DEF

	m_cSmallDef[0] = '*';
	m_cSmallDef[1] = 'n';
	m_cSmallDef[2] = 'd';
	m_cSmallDef[3] = 'a';
	m_cSmallDef[4] = 'o';
	m_cSmallDef[5] = 's';
	m_cSmallDef[6] = 'u';
	m_cSmallDef[7] = 'i';
	m_cSmallDef[8] = 'h';
	m_cSmallDef[9] = 'e';
	m_cSmallDef[10] = 'p';
	m_cSmallDef[11] = 'l';
	m_cSmallDef[12] = 'x';
	m_cSmallDef[13] = 't';
	m_cSmallDef[14] = 'm';
	m_cSmallDef[15] = 'f';
	m_cSmallDef[16] = 'c';
	m_cSmallDef[17] = 'g';
	m_cSmallDef[18] = 'v';

	m_rgbDef[DEF_NONE] = (RGB_WHITE);
	m_rgbDef[DEF_NICK] = (RGB_MAGENTA);
	m_rgbDef[DEF_PROTRUSION] = (RGB_SKYBLUE);
	m_rgbDef[DEF_SPACE] = (RGB_LTGREEN);
	m_rgbDef[DEF_OPEN] = (RGB_LTRED);
	m_rgbDef[DEF_SHORT] = (RGB_RED);
	m_rgbDef[DEF_USHORT] = (RGB_LTCYAN);
	m_rgbDef[DEF_PINHOLE] = (RGB_LLTGREEN);
	m_rgbDef[DEF_HOLE_MISS] = (RGB_LTBLUE);
	m_rgbDef[DEF_EXTRA] = (RGB_CLOUDBLUE);
	m_rgbDef[DEF_PAD] = (RGB_LTPURPLE);
	m_rgbDef[DEF_HOLE_POSITION] = (RGB_PINK);
	m_rgbDef[DEF_POI] = (RGB_LTMAGENTA);
	m_rgbDef[DEF_VH_POSITION] = (RGB_LTYELLOW);
	m_rgbDef[DEF_VH_MISS] = (RGB_BOON);
	m_rgbDef[DEF_HOLE_DEFECT] = (RGB_LTPINK);
	m_rgbDef[DEF_HOLE_OPEN] = (RGB_YELLOWGREEN);
	m_rgbDef[DEF_VH_OPEN] = (RGB_RED);
	m_rgbDef[DEF_VH_DEF] = (RGB_BROWN);
	m_rgbDef[DEF_LIGHT] = (RGB_YELLOW);

	LoadConfig();
}

void CManagerReelmap::Reset()
{
}

BOOL CManagerReelmap::InitAct()
{

	return TRUE;
}

void CManagerReelmap::LoadConfig()
{
	TCHAR szData[MAX_PATH];
	TCHAR sep[] = { _T(",;\r\n\t") };
	CString sVal;

	if (0 < ::GetPrivateProfileString(_T("REELMAP"), _T("BackGround"), NULL, szData, sizeof(szData), PATH_CONFIG))
	{
		sVal = _tcstok(szData, sep);
		m_nBkColor[0] = _tstoi(sVal);
		sVal = _tcstok(NULL, sep);
		m_nBkColor[1] = _tstoi(sVal);
		sVal = _tcstok(NULL, sep);
		m_nBkColor[2] = _tstoi(sVal);

		SetRgbBackGround(m_nBkColor[0], m_nBkColor[1], m_nBkColor[2]);
	}

	LoadDefectTableIni();
}

BOOL CManagerReelmap::LoadDefectTableIni()
{
	TCHAR szData[200];
	TCHAR sep[] = { _T(",;\r\n\t") };
	CString sIdx, sVal;
	int k;

	for (k = 1; k < MAX_DEF; k++)
	{
		sIdx.Format(_T("%d"), k);
		if (0 < ::GetPrivateProfileString(_T("DEFECT"), sIdx, NULL, szData, sizeof(szData), PATH_CONFIG))
		{
			sVal = _tcstok(szData, sep);
			m_sEngDef[k].Format(_T("%s"), sVal);
			sVal = _tcstok(NULL, sep);
			m_sKorDef[k].Format(_T("%s"), sVal);
			sVal = _tcstok(NULL, sep);
			m_cBigDef[k] = sVal.GetAt(0);
			sVal = _tcstok(NULL, sep);
			m_cSmallDef[k] = sVal.GetAt(0);
			sVal = _tcstok(NULL, sep);
			m_rgbDef[k] = (COLORREF)_tstoi(sVal);
			sVal = _tcstok(NULL, sep);
			m_nOdr[k] = _tstoi(sVal);

			SetRgbDef(k, m_rgbDef[k]);
		}
		else
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - LoadDefectTableIni()"));
			return FALSE;
		}
	}

	return TRUE;
}

void CManagerReelmap::SetRgbDef(int nDef, COLORREF rgbVal)
{
	m_rgbDef[nDef] = rgbVal;

	if (m_pReelMapUp)
		m_pReelMapUp->m_rgbDef[nDef] = rgbVal;
	if (m_pReelMapDn)
		m_pReelMapDn->m_rgbDef[nDef] = rgbVal;
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->m_rgbDef[nDef] = rgbVal;
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->m_rgbDef[nDef] = rgbVal;
}

void CManagerReelmap::SetRgbBackGround(int nRed, int nGreen, int nBlue)
{
	m_nBkColor[0] = nRed;
	m_nBkColor[1] = nGreen;
	m_nBkColor[2] = nBlue;

	if (m_pReelMapUp)
	{
		m_pReelMapUp->m_nBkColor[0] = nRed;
		m_pReelMapUp->m_nBkColor[1] = nGreen;
		m_pReelMapUp->m_nBkColor[2] = nBlue;
	}
	if (m_pReelMapDn)
	{
		m_pReelMapDn->m_nBkColor[0] = nRed;
		m_pReelMapDn->m_nBkColor[1] = nGreen;
		m_pReelMapDn->m_nBkColor[2] = nBlue;
	}
	if (m_pReelMapAllUp)
	{
		m_pReelMapAllUp->m_nBkColor[0] = nRed;
		m_pReelMapAllUp->m_nBkColor[1] = nGreen;
		m_pReelMapAllUp->m_nBkColor[2] = nBlue;
	}
	if (m_pReelMapAllDn)
	{
		m_pReelMapAllDn->m_nBkColor[0] = nRed;
		m_pReelMapAllDn->m_nBkColor[1] = nGreen;
		m_pReelMapAllDn->m_nBkColor[2] = nBlue;
	}
}

void CManagerReelmap::ClrFixPcs()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMap)
		m_pReelMap->ClrFixPcs();
	if (m_pReelMapUp)
		m_pReelMapUp->ClrFixPcs();
	
	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->ClrFixPcs();
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->ClrFixPcs();
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->ClrFixPcs();
	}
	
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapInnerUp)
			m_pReelMapInnerUp->ClrFixPcs();
	
		if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		{
			if (m_pReelMapInnerDn)
				m_pReelMapInnerDn->ClrFixPcs();
			if (m_pReelMapInnerAllUp)
				m_pReelMapInnerAllUp->ClrFixPcs();
			if (m_pReelMapInnerAllDn)
				m_pReelMapInnerAllDn->ClrFixPcs();
		}
	}
}

void CManagerReelmap::RestoreReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		m_pReelMapUp->RestoreReelmap();

	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->RestoreReelmap();
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->RestoreReelmap();
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->RestoreReelmap();
	}
}

BOOL CManagerReelmap::ReloadReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bDualTestInner = pDoc->WorkingInfo.LastJob.bDualTestInner;


	BOOL bRtn[5] = { 1 };
	int nSt = 0;

	if (m_pReelMapUp)
		bRtn[1] = m_pReelMapUp->ReloadReelmap();

	if (bDualTest)
	{
		if (m_pReelMapDn)
			bRtn[2] = m_pReelMapDn->ReloadReelmap();

		if (m_pReelMapAllUp)
			bRtn[3] = m_pReelMapAllUp->ReloadReelmap();

		if (m_pReelMapAllDn)
			bRtn[4] = m_pReelMapAllDn->ReloadReelmap();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		bRtn[0] = ReloadReelmapInner();
	}
	else
		nSt = 1;

	for (int i = nSt; i < 5; i++)
	{
		if (!bRtn[i])
		{
			AfxMessageBox(_T("ReloadReelmap() is Failed."));
			return FALSE;
		}
	}

	double dRatio = 0.0;
	CString sVal = _T("");
	CDlgProgress dlg;
	sVal.Format(_T("On Reloading Reelmap."));
	dlg.Create(sVal);

	int nRatio[5] = { 0 };
	BOOL bDone[5] = { 0 };
	int nTo = 0;
	if (bDualTest)
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (bDualTestInner)
				nTo = 400 + 500; //[%]
			else
				nTo = 400 + 200; //[%]
		}
		else
		{
			nTo = 400; //[%]
		}
	}
	else
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (bDualTestInner)
				nTo = 100 + 500; //[%]
			else
				nTo = 100 + 200; //[%]
		}
		else
		{
			nTo = 100; //[%]
		}
	}


	dlg.SetRange(0, nTo);
	dlg.SetPos(1);

	int nProc = 0, nProcOutter = 0, nProcInner = 0;
	for (nProc = 0; nProc < nTo;)
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if ((bDone[0] = IsDoneReloadReelmap(nProcOutter)) && (bDone[1] = IsDoneReloadReelmapInner(nProcInner)))
				break;
			nProc = nProcOutter + nProcInner;
		}
		else
		{
			if (bDone[0] = IsDoneReloadReelmap(nProc))
				break;
		}
		dlg.SetPos(nProc);
		Sleep(100);
	}

	dlg.DestroyWindow();


	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (!bDone[0] || !bDone[1])
		{
			AfxMessageBox(_T("IsDoneReloadReelmap or IsDoneReloadReelmapInner is FALSE."));
			return FALSE;
		}
	}
	else
	{
		if (!bDone[0])
		{
			AfxMessageBox(_T("IsDoneReloadReelmap is FALSE."));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CManagerReelmap::IsDoneReloadReelmap(int& nProc)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nRatio[5] = { 0 };
	BOOL bDone[5] = { 0 };

	if (m_pReelMapUp)
	{
		nRatio[1] = m_pReelMapUp->GetProgressReloadReelmap();
		bDone[1] = m_pReelMapUp->IsDoneReloadReelmap();
	}
	else
		bDone[1] = TRUE;

	if (bDualTest)
	{
		if (m_pReelMapDn)
		{
			nRatio[2] = m_pReelMapDn->GetProgressReloadReelmap();
			bDone[2] = m_pReelMapDn->IsDoneReloadReelmap();
		}
		else
			bDone[2] = TRUE;
		if (m_pReelMapAllUp)
		{
			nRatio[3] = m_pReelMapAllUp->GetProgressReloadReelmap();
			bDone[3] = m_pReelMapAllUp->IsDoneReloadReelmap();
		}
		else
			bDone[3] = TRUE;

		if (m_pReelMapAllDn)
		{
			nRatio[4] = m_pReelMapAllDn->GetProgressReloadReelmap();
			bDone[4] = m_pReelMapAllDn->IsDoneReloadReelmap();
		}
		else
			bDone[4] = TRUE;
	}

	nProc = nRatio[1] + nRatio[2] + nRatio[3] + nRatio[4];

	if (bDone[1] && bDone[2] && bDone[3] && bDone[4])
		return TRUE;

	return FALSE;
}

BOOL CManagerReelmap::ReloadReelmapFromThread()
{
	return pView->m_mgrThread->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapUp()
{
	if (m_pReelMapUp)
		m_pReelMapUp->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapAllUp()
{
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapDn()
{
	if (m_pReelMapDn)
		m_pReelMapDn->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapAllDn()
{
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapUpInner()
{
	if (m_pReelMapInnerUp)
		m_pReelMapInnerUp->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapAllUpInner()
{
	if (m_pReelMapInnerAllUp)
		m_pReelMapInnerAllUp->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapDnInner()
{
	if (m_pReelMapInnerDn)
		m_pReelMapInnerDn->ReloadReelmap();
}

void CManagerReelmap::ReloadReelmapAllDnInner()
{
	if (m_pReelMapInnerAllDn)
		m_pReelMapInnerAllDn->ReloadReelmap();
}

BOOL CManagerReelmap::ReloadReelmapInner()
{
	double dRatio = 0.0;
	CString sVal = _T("");

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	BOOL bRtn[7] = { 1 };

	if (m_pReelMapInnerUp)
		bRtn[1] = m_pReelMapInnerUp->ReloadReelmap();

	if (m_pReelMapIts)
		bRtn[2] = m_pReelMapIts->ReloadReelmap();

	bRtn[3] = TRUE;

	if (bDualTest)
	{
		if (m_pReelMapInnerDn)
			bRtn[4] = m_pReelMapInnerDn->ReloadReelmap();
		if (m_pReelMapInnerAllUp)
			bRtn[5] = m_pReelMapInnerAllUp->ReloadReelmap();
		if (m_pReelMapInnerAllDn)
			bRtn[6] = m_pReelMapInnerAllDn->ReloadReelmap();
	}

	for (int i = 1; i < 7; i++)
	{
		if (!bRtn[i])
			return FALSE;
	}

	return TRUE;
}

BOOL CManagerReelmap::IsDoneReloadReelmapInner(int& nProc)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	int nRatio[7] = { 0 };
	BOOL bDone[7] = { 0 };

	bDone[0] = TRUE;

	if (m_pReelMapInnerUp)
	{
		nRatio[1] = m_pReelMapInnerUp->GetProgressReloadReelmap();
		bDone[1] = m_pReelMapInnerUp->IsDoneReloadReelmap();
	}
	else
		bDone[1] = TRUE;

	if (m_pReelMapIts)
	{
		nRatio[2] = m_pReelMapIts->GetProgressReloadReelmap();
		bDone[2] = m_pReelMapIts->IsDoneReloadReelmap();
	}
	else
		bDone[2] = TRUE;

	bDone[3] = TRUE;

	if (bDualTest)
	{
		if (m_pReelMapInnerDn)
		{
			nRatio[4] = m_pReelMapInnerDn->GetProgressReloadReelmap();
			bDone[4] = m_pReelMapInnerDn->IsDoneReloadReelmap();
		}
		else
			bDone[4] = TRUE;

		if (m_pReelMapInnerAllUp)
		{
			nRatio[5] = m_pReelMapInnerAllUp->GetProgressReloadReelmap();
			bDone[5] = m_pReelMapInnerAllUp->IsDoneReloadReelmap();
		}
		else
			bDone[5] = TRUE;

		if (m_pReelMapAllDn)
		{
			nRatio[6] = m_pReelMapInnerAllDn->GetProgressReloadReelmap();
			bDone[6] = m_pReelMapInnerAllDn->IsDoneReloadReelmap();
		}
		else
			bDone[6] = TRUE;
	}
	else
	{
		bDone[4] = TRUE;
		bDone[5] = TRUE;
		bDone[6] = TRUE;
	}

	nProc = nRatio[0] + nRatio[1] + nRatio[2] + nRatio[3] + nRatio[4] + nRatio[5] + nRatio[6];

	if (bDone[0] && bDone[1] && bDone[2] && bDone[3] && bDone[4] && bDone[5] && bDone[6])
		return TRUE;

	return FALSE;
}

void CManagerReelmap::UpdateData()
{
	if (m_pReelMap)
	{
		m_pReelMap->m_sMc = pDoc->WorkingInfo.System.sMcName;
		m_pReelMap->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;

		m_pReelMap->m_bUseLotSep = pDoc->WorkingInfo.LastJob.bLotSep;
		m_pReelMap->m_bUseTempPause = pDoc->WorkingInfo.LastJob.bTempPause;

		m_pReelMap->m_dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen)*1000.0;
		m_pReelMap->m_dPnlLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
		m_pReelMap->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
		if (m_pReelMap->m_bUseLotSep)
			m_pReelMap->m_dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen);
		else
			m_pReelMap->m_dLotLen = m_pReelMap->m_dTotLen;
		m_pReelMap->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	}

	if (m_pReelMapUp)
	{
		m_pReelMapUp->m_sMc = pDoc->WorkingInfo.System.sMcName;
		m_pReelMapUp->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;

		m_pReelMapUp->m_bUseLotSep = pDoc->WorkingInfo.LastJob.bLotSep;
		m_pReelMapUp->m_bUseTempPause = pDoc->WorkingInfo.LastJob.bTempPause;

		m_pReelMapUp->m_dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen)*1000.0;
		m_pReelMapUp->m_dPnlLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
		m_pReelMapUp->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
		if (m_pReelMapUp->m_bUseLotSep)
			m_pReelMapUp->m_dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen);
		else
			m_pReelMapUp->m_dLotLen = m_pReelMap->m_dTotLen;
		m_pReelMapUp->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	}

	if (m_pReelMapDn)
	{
		m_pReelMapDn->m_sMc = pDoc->WorkingInfo.System.sMcName;
		m_pReelMapDn->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;

		m_pReelMapDn->m_bUseLotSep = pDoc->WorkingInfo.LastJob.bLotSep;
		m_pReelMapDn->m_bUseTempPause = pDoc->WorkingInfo.LastJob.bTempPause;

		m_pReelMapDn->m_dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen)*1000.0;
		m_pReelMapDn->m_dPnlLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
		m_pReelMapDn->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
		if (m_pReelMapDn->m_bUseLotSep)
			m_pReelMapDn->m_dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen);
		else
			m_pReelMapDn->m_dLotLen = m_pReelMap->m_dTotLen;
		m_pReelMapDn->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	}

	if (m_pReelMapAllUp)
	{
		m_pReelMapAllUp->m_sMc = pDoc->WorkingInfo.System.sMcName;
		m_pReelMapAllUp->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;

		m_pReelMapAllUp->m_bUseLotSep = pDoc->WorkingInfo.LastJob.bLotSep;
		m_pReelMapAllUp->m_bUseTempPause = pDoc->WorkingInfo.LastJob.bTempPause;

		m_pReelMapAllUp->m_dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen)*1000.0;
		m_pReelMapAllUp->m_dPnlLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
		m_pReelMapAllUp->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
		if (m_pReelMapAllUp->m_bUseLotSep)
			m_pReelMapAllUp->m_dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen);
		else
			m_pReelMapAllUp->m_dLotLen = m_pReelMap->m_dTotLen;
		m_pReelMapAllUp->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	}

	if (m_pReelMapAllDn)
	{
		m_pReelMapAllDn->m_sMc = pDoc->WorkingInfo.System.sMcName;
		m_pReelMapAllDn->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;

		m_pReelMapAllDn->m_bUseLotSep = pDoc->WorkingInfo.LastJob.bLotSep;
		m_pReelMapAllDn->m_bUseTempPause = pDoc->WorkingInfo.LastJob.bTempPause;

		m_pReelMapAllDn->m_dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen)*1000.0;
		m_pReelMapAllDn->m_dPnlLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
		m_pReelMapAllDn->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
		if (m_pReelMapAllDn->m_bUseLotSep)
			m_pReelMapAllDn->m_dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen);
		else
			m_pReelMapAllDn->m_dLotLen = m_pReelMap->m_dTotLen;
		m_pReelMapAllDn->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	}

}

BOOL CManagerReelmap::InitReelmap()
{
	if (!m_Master[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		//pView->MsgBox(strMsg);
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	int nTotPcs = m_Master[0].m_pPcsRgn->nTotPcs;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
	{
		delete m_pReelMapUp;
		m_pReelMapUp = NULL;
	}
	m_pReelMapUp = new CReelMap(RMAP_UP, MAX_DISP_PNL, nTotPcs);

	if (bDualTest)
	{
		if (m_pReelMapDn)
		{
			delete m_pReelMapDn;
			m_pReelMapDn = NULL;
		}
		m_pReelMapDn = new CReelMap(RMAP_DN, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapAllUp)
		{
			delete m_pReelMapAllUp;
			m_pReelMapAllUp = NULL;
		}
		m_pReelMapAllUp = new CReelMap(RMAP_ALLUP, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapAllDn)
		{
			delete m_pReelMapAllDn;
			m_pReelMapAllDn = NULL;
		}
		m_pReelMapAllDn = new CReelMap(RMAP_ALLDN, MAX_DISP_PNL, nTotPcs);

		if (pDoc->GetTestMode() != MODE_OUTER)
			m_pReelMap = m_pReelMapAllUp;
		else
		{
			if (m_pReelMapIts)
			{
				delete m_pReelMapIts;
				m_pReelMapIts = NULL;
			}
			m_pReelMapIts = new CReelMap(RMAP_ITS, MAX_DISP_PNL, nTotPcs); // Default: RMAP_NONE (RMAP_INNER -> RMAP_INNER_UP)
			m_pReelMap = m_pReelMapIts;
		}
	}
	else
	{
		if (pDoc->GetTestMode() != MODE_OUTER)
			m_pReelMap = m_pReelMapUp;
		else
		{
			if (m_pReelMapIts)
				m_pReelMap = m_pReelMapIts;
		}
	}

	if (m_pReelMap)
	{
		m_pReelMapDisp = m_pReelMap;

		if (bDualTest)
			pView->m_pDlgMenu01->SelMap(ALL);
		else
			pView->m_pDlgMenu01->SelMap(UP);
	}

	SetReelmap(ROT_NONE);
	UpdateData();
}

void CManagerReelmap::InitReelmapUp()
{
	InitReelmapUp();
	SetReelmap(ROT_NONE);
	UpdateData();
}

void CManagerReelmap::InitReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		InitReelmapDn();
		SetReelmap(ROT_NONE);
		UpdateData();
	}
}

void CManagerReelmap::SetReelmap(int nDir)
{
	if (!m_pReelMap || !m_Master[0].m_pPcsRgn)
		return;

	if (!m_pReelMap->pFrmRgn || !m_pReelMap->pPcsRgn)
		return;

	int i, k;
	double fData1, fData2, fData3, fData4, fDistX, fDistY;
	double fWidth, fHeight, fRight, fBottom;

	m_pReelMap->nDir = nDir;

	int nTotPnl = m_pReelMap->nTotPnl;
	int nTotPcs = m_pReelMap->nTotPcs;

	// 	double dScale = (MasterInfo.dPixelSize/10.0);
	double dScale = (m_Master[0].MasterInfo.dPixelSize / 10.0);
	m_pReelMap->SetAdjRatio(dScale);
	dScale = m_pReelMap->GetAdjRatio();

	for (k = 0; k < nTotPnl; k++)
	{
		switch (nDir)
		{
		case ROT_NONE:
			fWidth = (m_Master[0].m_pPcsRgn->pPcs[0].right - m_Master[0].m_pPcsRgn->pPcs[0].left);
			fHeight = (m_Master[0].m_pPcsRgn->pPcs[0].bottom - m_Master[0].m_pPcsRgn->pPcs[0].top);
			fRight = m_Master[0].m_pPcsRgn->rtFrm.right - fWidth * (1.0 - RMAP_PCS_SCALE);
			fBottom = m_Master[0].m_pPcsRgn->rtFrm.bottom - fHeight * (1.0 - RMAP_PCS_SCALE);;
			m_pReelMap->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.left;
			m_pReelMap->pFrmRgn[k].top = m_Master[0].m_pPcsRgn->rtFrm.top;
			m_pReelMap->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fRight;
			m_pReelMap->pFrmRgn[k].bottom = fBottom;

			if (m_pReelMapDisp)
			{
				m_pReelMapDisp->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.left;
				m_pReelMapDisp->pFrmRgn[k].top = m_Master[0].m_pPcsRgn->rtFrm.top;
				m_pReelMapDisp->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fRight;
				m_pReelMapDisp->pFrmRgn[k].bottom = fBottom;
			}

			for (i = 0; i < nTotPcs; i++)
			{
				fWidth = (m_Master[0].m_pPcsRgn->pPcs[i].right - m_Master[0].m_pPcsRgn->pPcs[i].left);
				fHeight = (m_Master[0].m_pPcsRgn->pPcs[i].bottom - m_Master[0].m_pPcsRgn->pPcs[i].top);
				fData1 = m_Master[0].m_pPcsRgn->pPcs[i].left;	// left
				fData2 = m_Master[0].m_pPcsRgn->pPcs[i].top;	// top
				fData3 = fData1 + fWidth * RMAP_PCS_SCALE; // right
				fData4 = fData2 + fHeight * RMAP_PCS_SCALE; // bottom

				m_pReelMap->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMap->pPcsRgn[k][i].top = fData2;
				m_pReelMap->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMap->pPcsRgn[k][i].bottom = fData4;

				if (m_pReelMapDisp)
				{
					m_pReelMapDisp->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
					m_pReelMapDisp->pPcsRgn[k][i].top = fData2;
					m_pReelMapDisp->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
					m_pReelMapDisp->pPcsRgn[k][i].bottom = fData4;
				}
			}
			break;
		case ROT_CCW_90: // right->bottom, top->left, bottom->right, left->top ; Dir (x *= 1, y *= -1) 
			fDistX = 0;
			fDistY = m_Master[0].m_pPcsRgn->rtFrm.left + m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMap->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.top;
			m_pReelMap->pFrmRgn[k].top = fDistY - m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMap->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.bottom;
			m_pReelMap->pFrmRgn[k].bottom = fDistY - m_Master[0].m_pPcsRgn->rtFrm.left;

			if (m_pReelMapDisp)
			{
				m_pReelMapDisp->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.top;
				m_pReelMapDisp->pFrmRgn[k].top = fDistY - m_Master[0].m_pPcsRgn->rtFrm.right;
				m_pReelMapDisp->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.bottom;
				m_pReelMapDisp->pFrmRgn[k].bottom = fDistY - m_Master[0].m_pPcsRgn->rtFrm.left;
			}

			for (i = 0; i < nTotPcs; i++)
			{
				fData1 = m_Master[0].m_pPcsRgn->pPcs[i].top;	// left
				fData2 = fDistY - m_Master[0].m_pPcsRgn->pPcs[i].right;	// top
				fData3 = m_Master[0].m_pPcsRgn->pPcs[i].bottom;	// right
				fData4 = fDistY - m_Master[0].m_pPcsRgn->pPcs[i].left; // bottom

				m_pReelMap->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMap->pPcsRgn[k][i].top = fData2;
				m_pReelMap->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMap->pPcsRgn[k][i].bottom = fData4;

				if (m_pReelMapDisp)
				{
					m_pReelMapDisp->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
					m_pReelMapDisp->pPcsRgn[k][i].top = fData2;
					m_pReelMapDisp->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
					m_pReelMapDisp->pPcsRgn[k][i].bottom = fData4;
				}
			}
			break;
		}
	}
}

void CManagerReelmap::SwitchReelmapDisp(int nSelRmap)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		switch (nSelRmap)
		{
		case RMAP_UP:
			if (m_pReelMapUp)
				m_pReelMapDisp = m_pReelMapUp;
			break;
		case RMAP_DN:
			if (m_pReelMapDn)
				m_pReelMapDisp = m_pReelMapDn;
			break;
		case RMAP_ALLUP:
		case RMAP_ALLDN:
			if (m_pReelMapAllUp)
				m_pReelMapDisp = m_pReelMapAllUp;
			break;
		case RMAP_ITS:
			if (m_pReelMapIts)
				m_pReelMapDisp = m_pReelMapIts;
			break;
		}
	}
	else
	{
		if (m_pReelMapUp)
			m_pReelMapDisp = m_pReelMapUp;
	}

	SetReelmap(ROT_NONE);
}

BOOL CManagerReelmap::InitReelmapInner()
{
	if (!m_MasterInner[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	int nTotPcs = m_MasterInner[0].m_pPcsRgn->GetTotPcs();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (m_pReelMapInnerUp)
	{
		delete m_pReelMapInnerUp;
		m_pReelMapInnerUp = NULL;
	}
	m_pReelMapInnerUp = new CReelMap(RMAP_INNER_UP, MAX_DISP_PNL, nTotPcs);

	if (bDualTest)
	{
		if (m_pReelMapInnerDn)
		{
			delete m_pReelMapInnerDn;
			m_pReelMapInnerDn = NULL;
		}
		m_pReelMapInnerDn = new CReelMap(RMAP_INNER_DN, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapInnerAllUp)
		{
			delete m_pReelMapInnerAllUp;
			m_pReelMapInnerAllUp = NULL;
		}
		m_pReelMapInnerAllUp = new CReelMap(RMAP_INNER_ALLUP, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapInnerAllDn)
		{
			delete m_pReelMapInnerAllDn;
			m_pReelMapInnerAllDn = NULL;
		}
		m_pReelMapInnerAllDn = new CReelMap(RMAP_INNER_ALLDN, MAX_DISP_PNL, nTotPcs);
		m_pReelMapInner = m_pReelMapInnerAllUp;
	}
	else
	{
		m_pReelMapInner = m_pReelMapInnerUp;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapIts)
			m_pReelMap = m_pReelMapIts;
	}

	SetReelmapInner(ROT_NONE);
	return TRUE;
}

BOOL CManagerReelmap::InitReelmapInnerUp()
{
	if (!m_MasterInner[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		return FALSE;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	int nTotPcs = m_MasterInner[0].m_pPcsRgn->GetTotPcs();

	if (m_pReelMapInnerUp)
	{
		delete m_pReelMapInnerUp;
		m_pReelMapInnerUp = NULL;
	}
	m_pReelMapInnerUp = new CReelMap(RMAP_INNER_UP, MAX_DISP_PNL, nTotPcs);

	if (bDualTest)
	{

		if (m_pReelMapInnerAllUp)
		{
			delete m_pReelMapInnerAllUp;
			m_pReelMapInnerAllUp = NULL;
		}
		m_pReelMapInnerAllUp = new CReelMap(RMAP_INNER_ALLUP, MAX_DISP_PNL, nTotPcs);
		m_pReelMapInner = m_pReelMapInnerAllUp;
	}
	else
	{
		m_pReelMapInner = m_pReelMapInnerUp;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		m_pReelMap = m_pReelMapIts;
	}

	SetReelmapInner(ROT_NONE);

	return TRUE;
}

BOOL CManagerReelmap::InitReelmapInnerDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (!bDualTest)
		return TRUE;

	if (!m_MasterInner[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		return FALSE;
	}

	int nTotPcs = m_MasterInner[0].m_pPcsRgn->GetTotPcs();

	if (m_pReelMapInnerDn)
	{
		delete m_pReelMapInnerDn;
		m_pReelMapInnerDn = NULL;
	}
	m_pReelMapInnerDn = new CReelMap(RMAP_INNER_DN, MAX_DISP_PNL, nTotPcs);

	if (m_pReelMapInnerAllDn)
	{
		delete m_pReelMapInnerAllDn;
		m_pReelMapInnerAllDn = NULL;
	}

	m_pReelMapInnerAllDn = new CReelMap(RMAP_INNER_ALLDN, MAX_DISP_PNL, nTotPcs);
	SetReelmapInner(ROT_NONE);

	return TRUE;
}

void CManagerReelmap::SetReelmapInner(int nDir)
{
	if (!m_pReelMapInner || !m_MasterInner[0].m_pPcsRgn)
		return;

	if (!m_pReelMapInner->pFrmRgn || !m_pReelMapInner->pPcsRgn)
		return;

	int i, k;
	double fData1, fData2, fData3, fData4, fDistX, fDistY;
	double fWidth, fHeight, fRight, fBottom;

	m_pReelMapInner->nDir = nDir;

	int nTotPnl = m_pReelMapInner->nTotPnl;
	int nTotPcs = m_pReelMapInner->nTotPcs;

	// 	double dScale = (MasterInfo.dPixelSize/10.0);
	double dScale = (m_MasterInner[0].MasterInfo.dPixelSize / 10.0);
	m_pReelMapInner->SetAdjRatio(dScale);
	dScale = m_pReelMapInner->GetAdjRatio();

	for (k = 0; k < nTotPnl; k++)
	{
		switch (nDir)
		{
		case ROT_NONE:
			fWidth = (m_MasterInner[0].m_pPcsRgn->pPcs[0].right - m_MasterInner[0].m_pPcsRgn->pPcs[0].left);
			fHeight = (m_MasterInner[0].m_pPcsRgn->pPcs[0].bottom - m_MasterInner[0].m_pPcsRgn->pPcs[0].top);
			fRight = m_MasterInner[0].m_pPcsRgn->rtFrm.right - fWidth * (1.0 - RMAP_PCS_SCALE);
			fBottom = m_MasterInner[0].m_pPcsRgn->rtFrm.bottom - fHeight * (1.0 - RMAP_PCS_SCALE);;
			m_pReelMapInner->pFrmRgn[k].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_MasterInner[0].m_pPcsRgn->rtFrm.left;
			m_pReelMapInner->pFrmRgn[k].top = m_MasterInner[0].m_pPcsRgn->rtFrm.top;
			m_pReelMapInner->pFrmRgn[k].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fRight;
			m_pReelMapInner->pFrmRgn[k].bottom = fBottom;
			//m_pReelMap->pFrmRgn[k].left = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.left;
			//m_pReelMap->pFrmRgn[k].top = m_pPcsRgn->rtFrm.top;
			//m_pReelMap->pFrmRgn[k].right = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.right;
			//m_pReelMap->pFrmRgn[k].bottom = m_pPcsRgn->rtFrm.bottom;

			for (i = 0; i < nTotPcs; i++)
			{
				fWidth = (m_MasterInner[0].m_pPcsRgn->pPcs[i].right - m_MasterInner[0].m_pPcsRgn->pPcs[i].left);
				fHeight = (m_MasterInner[0].m_pPcsRgn->pPcs[i].bottom - m_MasterInner[0].m_pPcsRgn->pPcs[i].top);
				fData1 = m_MasterInner[0].m_pPcsRgn->pPcs[i].left;	// left
				fData2 = m_MasterInner[0].m_pPcsRgn->pPcs[i].top;	// top
				fData3 = fData1 + fWidth * RMAP_PCS_SCALE; // right
				fData4 = fData2 + fHeight * RMAP_PCS_SCALE; // bottom

				m_pReelMapInner->pPcsRgn[k][i].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMapInner->pPcsRgn[k][i].top = fData2;
				m_pReelMapInner->pPcsRgn[k][i].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMapInner->pPcsRgn[k][i].bottom = fData4;
			}
			break;
			//case ROT_NONE:
			//	m_pReelMap->pFrmRgn[k].left = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.left;
			//	m_pReelMap->pFrmRgn[k].top = m_pPcsRgn->rtFrm.top;
			//	m_pReelMap->pFrmRgn[k].right = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+m_pPcsRgn->rtFrm.right;
			//	m_pReelMap->pFrmRgn[k].bottom = m_pPcsRgn->rtFrm.bottom;

			//	for(i=0; i<nTotPcs; i++)
			//	{
			//		fData1 = m_pPcsRgn->pPcs[i].left; // left
			//		fData2 = m_pPcsRgn->pPcs[i].top; // top
			//		fData3 = m_pPcsRgn->pPcs[i].right; // right
			//		fData4 = m_pPcsRgn->pPcs[i].bottom; // bottom

			//		m_pReelMap->pPcsRgn[k][i].left = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+fData1;
			//		m_pReelMap->pPcsRgn[k][i].top = fData2;
			//		m_pReelMap->pPcsRgn[k][i].right = (m_pPcsRgn->rtFrm.right+MYGL_GAP_PNL*dScale)*(nTotPnl-1-k)+fData3;
			//		m_pReelMap->pPcsRgn[k][i].bottom = fData4;
			//	}
			//	break;
		case ROT_CCW_90: // right->bottom, top->left, bottom->right, left->top ; Dir (x *= 1, y *= -1) 
			fDistX = 0;
			fDistY = m_MasterInner[0].m_pPcsRgn->rtFrm.left + m_MasterInner[0].m_pPcsRgn->rtFrm.right;
			m_pReelMapInner->pFrmRgn[k].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_MasterInner[0].m_pPcsRgn->rtFrm.top;
			m_pReelMapInner->pFrmRgn[k].top = fDistY - m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMapInner->pFrmRgn[k].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_MasterInner[0].m_pPcsRgn->rtFrm.bottom;
			m_pReelMapInner->pFrmRgn[k].bottom = fDistY - m_MasterInner[0].m_pPcsRgn->rtFrm.left;

			for (i = 0; i < nTotPcs; i++)
			{
				fData1 = m_MasterInner[0].m_pPcsRgn->pPcs[i].top;	// left
				fData2 = fDistY - m_MasterInner[0].m_pPcsRgn->pPcs[i].right;	// top
				fData3 = m_MasterInner[0].m_pPcsRgn->pPcs[i].bottom;	// right
				fData4 = fDistY - m_MasterInner[0].m_pPcsRgn->pPcs[i].left; // bottom

				m_pReelMapInner->pPcsRgn[k][i].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMapInner->pPcsRgn[k][i].top = fData2;
				m_pReelMapInner->pPcsRgn[k][i].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMapInner->pPcsRgn[k][i].bottom = fData4;
			}
			break;
		}
	}
}

BOOL CManagerReelmap::OpenReelmapFromBuf(int nSerial)
{
	BOOL bRtn[4] = { 1 };
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	stModelInfo stInfoUp, stInfoDn;
	CString sSrc;
	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!GetPcrInfo(sSrc, stInfoUp))
	{
		pView->DispStsBar(_T("E(6)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(6)"));
		return FALSE;
	}
	if (bDualTest)
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);
		if (!GetPcrInfo(sSrc, stInfoDn))
		{
			pView->DispStsBar(_T("E(7)"), 5);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error-GetPcrInfo(7)"));
			return FALSE;
		}
	}

	if (m_pReelMapUp)
		bRtn[0] = m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapDn)
			bRtn[1] = m_pReelMapDn->Open();
		if (m_pReelMapAllUp)
			bRtn[2] = m_pReelMapAllUp->Open();
		if (m_pReelMapAllDn)
			bRtn[3] = m_pReelMapAllDn->Open();
	}

	if (bRtn[0] && bRtn[1] && bRtn[2] && bRtn[3])
		return TRUE;

	return FALSE;
}

BOOL CManagerReelmap::OpenReelmap()
{
	BOOL bRtn[4] = { 1 };
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		bRtn[0] = m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapDn)
			bRtn[1] = m_pReelMapDn->Open();
		if (m_pReelMapAllUp)
			bRtn[2] = m_pReelMapAllUp->Open();
		if (m_pReelMapAllDn)
			bRtn[3] = m_pReelMapAllDn->Open();
	}

	if (bRtn[0] && bRtn[1] && bRtn[2] && bRtn[3])
		return TRUE;

	return FALSE;
}

BOOL CManagerReelmap::OpenReelmapUp()
{
	BOOL bRtn[2] = { 1 };
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		bRtn[0] = m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapAllUp)
			bRtn[1] = m_pReelMapAllUp->Open();
	}

	if (bRtn[0] && bRtn[1])
		return TRUE;

	return FALSE;
}

BOOL CManagerReelmap::OpenReelmapDn()
{
	BOOL bRtn[2] = { 1 };
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (m_pReelMapDn)
		bRtn[0] = m_pReelMapDn->Open();
	if (m_pReelMapAllDn)
		bRtn[1] = m_pReelMapAllDn->Open();

	if (bRtn[0] && bRtn[1])
		return TRUE;

	return FALSE;
}

BOOL CManagerReelmap::OpenReelmapInner()
{
	BOOL bRtn[5] = { 1 };
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (m_pReelMapInnerUp)
		bRtn[0] = m_pReelMapInnerUp->Open();

	if (m_pReelMapIts)
		bRtn[1] = m_pReelMapIts->Open();

	if (bDualTest)
	{
		if (m_pReelMapInnerDn)
			bRtn[2] = m_pReelMapInnerDn->Open();
		if (m_pReelMapInnerAllUp)
			bRtn[3] = m_pReelMapInnerAllUp->Open();
		if (m_pReelMapInnerAllDn)
			bRtn[4] = m_pReelMapInnerAllDn->Open();
	}

	if (bRtn[0] && bRtn[1] && bRtn[2] && bRtn[3] && bRtn[4])
		return TRUE;

	return FALSE;
}

BOOL CManagerReelmap::OpenReelmapInnerUp()
{
	BOOL bRtn[2] = { 1 };
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (m_pReelMapInnerUp)
		bRtn[0] = m_pReelMapInnerUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapInnerAllUp)
			bRtn[1] = m_pReelMapInnerAllUp->Open();
	}

	if (bRtn[0] && bRtn[1])
		return TRUE;

	return FALSE;
}

BOOL CManagerReelmap::OpenReelmapInnerDn()
{
	BOOL bRtn[2] = { 1 };
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (!bDualTest)
		return FALSE;

	if (m_pReelMapInnerDn)
		bRtn[0] = m_pReelMapInnerDn->Open();
	if (m_pReelMapInnerAllDn)
		bRtn[1] = m_pReelMapInnerAllDn->Open();

	if (bRtn[0] && bRtn[1])
		return TRUE;

	return FALSE;
}

int CManagerReelmap::LoadPcrUp(int nSerial)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (m_pReelMapUp)
		return m_pReelMapUp->LoadPCR(nSerial);
	return 2;
}

int CManagerReelmap::LoadPcrDn(int nSerial)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (m_pReelMapDn)
		return m_pReelMapDn->LoadPCR(nSerial);
	return 2;
}

BOOL CManagerReelmap::UpdateReelmap(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.54"));
		return 0;
	}

	if (!MakeMkDir())
		return FALSE;

	return pView->m_mgrThread->UpdateReelmap(nSerial);
}

void CManagerReelmap::UpdateRMapUp()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (m_pReelMapUp)
	{
		m_pReelMapUp->Write(Thread.nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

		if (m_pReelMap == m_pReelMapUp)
		{
			DuplicateRmap(RMAP_UP);
		}
	}
}

void CManagerReelmap::UpdateRMapDn()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (m_pReelMapDn)
	{
		m_pReelMapDn->Write(Thread.nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CManagerReelmap::UpdateRMapAllUp()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (m_pReelMapAllUp)
	{
		m_pReelMapAllUp->Write(Thread.nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

		if (m_pReelMap == m_pReelMapAllUp)
		{
			DuplicateRmap(RMAP_ALLUP);
		}
	}
}

void CManagerReelmap::UpdateRMapAllDn()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (m_pReelMapAllDn)
	{
		m_pReelMapAllDn->Write(Thread.nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

BOOL CManagerReelmap::MakeItsFile(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.54"));
		return FALSE;
	}

	if (!MakeMkDir())
		return FALSE;

	return pView->m_mgrThread->MakeItsFile(nSerial);
}

BOOL CManagerReelmap::MakeMkDir()
{
	BOOL bRtn[2] = { 1 };
	bRtn[0] = OpenReelmap();
	bRtn[1] = OpenReelmapInner();

	if (bRtn[0] && bRtn[1])
		return TRUE;

	return FALSE;
}

void CManagerReelmap::MakeResultMDS()
{
	pDoc->WorkingInfo.LastJob.sProcessNum = pDoc->GetProcessNum();
	UpdateProcessNum(pDoc->WorkingInfo.LastJob.sProcessNum);

	// for SAPP3
	GetResult();		// Load data From Reelmap.txt
	MakeResult();		// Result.txt
	MakeResultIts();	// Result.txt
	MakeSapp3();		// GetSapp3Txt()

	RemakeReelmap();	// MDS(해성DS) Style의 릴맵으로 재생성

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//	RemakeReelmapInner();
}

void CManagerReelmap::UpdateProcessNum(CString sProcessNum)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_pReelMapUp->UpdateProcessNum(sProcessNum, 0); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	if (bDualTest)
	{
		m_pReelMapDn->UpdateProcessNum(sProcessNum, 1); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		m_pReelMapAllUp->UpdateProcessNum(sProcessNum, 2); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		m_pReelMapAllDn->UpdateProcessNum(sProcessNum, 3); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CManagerReelmap::GetResult()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	//if (m_pReelMapUp)
	//	m_pReelMapUp->GetResult();
	if (bDualTest)
	{
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->GetResult();
		//if (m_pReelMapDn)
		//	m_pReelMapDn->GetResult();
		//if (m_pReelMapAllDn)
		//	m_pReelMapAllDn->GetResult();
	}
	else
	{
		if (m_pReelMapUp)
			m_pReelMapUp->GetResult();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		//if (m_pReelMapInnerUp)
		//	m_pReelMapInnerUp->GetResult();
		if (m_pReelMapIts)
			m_pReelMapIts->GetResult();

		//if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		//{
		//	if (m_pReelMapInnerDn)
		//		m_pReelMapInnerDn->GetResult();
		//	if (m_pReelMapInnerAllUp)
		//		m_pReelMapInnerAllUp->GetResult();
		//	if (m_pReelMapInnerAllDn)
		//		m_pReelMapInnerAllDn->GetResult();
		//}
	}
}

void CManagerReelmap::MakeResult()
{
	// TODO: Add your control notification handler code here
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	// File Save......

	CString strDestPath;
	strDestPath.Format(_T("%s%s\\%s\\%s.txt"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		_T("Result"));

	CFileFind cFile;
	if (cFile.FindFile(strDestPath))
		DeleteFile(strDestPath);

	//////////////////////////////////////////////////////////
	// Directory location of Work file
	CString strData;
	//strData = m_pDlgMenu05->TxtDataMDS();
	if (bDualTest)
	{
		if (m_pReelMapAllUp)
			strData = m_pReelMapAllUp->GetResultTxt();
	}
	else
	{
		if (m_pReelMapUp)
			strData = m_pReelMapUp->GetResultTxt();
	}


	TCHAR lpszCurDirPathFile[MAX_PATH];
	_stprintf(lpszCurDirPathFile, _T("%s"), strDestPath);

	CFile file;
	CFileException pError;
	if (!file.Open(lpszCurDirPathFile, CFile::modeWrite, &pError))
	{
		if (!file.Open(lpszCurDirPathFile, CFile::modeCreate | CFile::modeWrite, &pError))
		{
			// 파일 오픈에 실패시 
#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
#endif
			return;
		}
	}
	//버퍼의 내용을 file에 복사한다.
	char* pRtn = NULL;
	file.SeekToBegin();
	file.Write(pRtn = StringToChar(strData), strData.GetLength());
	file.Close();
	if (pRtn)
		delete pRtn;
}

void CManagerReelmap::MakeResultIts()
{
	// TODO: Add your control notification handler code here
	if (pDoc->GetTestMode() != MODE_OUTER)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	// File Save......

	CString strDestPath;
	strDestPath.Format(_T("%s%s\\%s\\%s.txt"), pDoc->WorkingInfo.System.sPathItsFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sEngItsCode,
		_T("Result"));

	CFileFind cFile;
	if (cFile.FindFile(strDestPath))
		DeleteFile(strDestPath);

	//////////////////////////////////////////////////////////
	// Directory location of Work file
	CString strData;
	if (m_pReelMapIts)
		strData = m_pReelMapIts->GetResultTxt();


	TCHAR lpszCurDirPathFile[MAX_PATH];
	_stprintf(lpszCurDirPathFile, _T("%s"), strDestPath);

	CFile file;
	CFileException pError;
	if (!file.Open(lpszCurDirPathFile, CFile::modeWrite, &pError))
	{
		if (!file.Open(lpszCurDirPathFile, CFile::modeCreate | CFile::modeWrite, &pError))
		{
			// 파일 오픈에 실패시 
#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
#endif
			return;
		}
	}
	//버퍼의 내용을 file에 복사한다.
	char* pRtn = NULL;
	file.SeekToBegin();
	file.Write(pRtn = StringToChar(strData), strData.GetLength());
	file.Close();
	if (pRtn)
		delete pRtn;
}

void CManagerReelmap::MakeSapp3()
{
	if (pDoc->WorkingInfo.LastJob.sProcessNum.IsEmpty() || pDoc->WorkingInfo.System.sPathSapp3.IsEmpty())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	FILE *fp = NULL;
	TCHAR FileName[MAX_PATH];
	CString sPath, sVal, strMsg, sSapp3Data;

	if (bDualTest)
	{
		if (m_pReelMapAllUp)
			sSapp3Data = m_pReelMapAllUp->GetSapp3Txt();
	}
	else
	{
		if (m_pReelMapUp)
			sSapp3Data = m_pReelMapUp->GetSapp3Txt();
	}

	sPath.Format(_T("%s%9s_%4s_%5s.txt"), pDoc->WorkingInfo.System.sPathSapp3,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sProcessNum,
		pDoc->WorkingInfo.System.sMcName);

	_stprintf(FileName, _T("%s"), sPath);
	char* pRtn = NULL;
	fp = fopen(pRtn = TCHARToChar(FileName), "w+");
	if (pRtn) delete pRtn;
	pRtn = NULL;

	if (fp != NULL)
	{
		fprintf(fp, "%s\n", pRtn = StringToChar(sSapp3Data));
	}
	else
	{
		strMsg.Format(_T("It is trouble to open file.\r\n%s"), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK);
	}

	if (pRtn)
		delete pRtn;
	fclose(fp);
}

BOOL CManagerReelmap::RemakeReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sReelmapSrc, str;

	if (m_pReelMapUp)
		m_pReelMapUp->StartThreadRemakeReelmap();

	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->StartThreadRemakeReelmap();

		if (m_pReelMapAllUp)
			m_pReelMapAllUp->StartThreadRemakeReelmap();
	}

	return TRUE;
}

BOOL CManagerReelmap::RemakeReelmapInner()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sReelmapSrc, str;

	if (m_pReelMapIts)
		m_pReelMapIts->StartThreadRemakeReelmap();

	return TRUE;
}

void CManagerReelmap::UpdateYield(int nSerial)
{
	pView->m_mgrThread->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldUp(int nSerial)
{
	if (m_pReelMapUp)
		m_pReelMapUp->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldDn(int nSerial)
{
	if (m_pReelMapDn)
		m_pReelMapDn->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldAllUp(int nSerial)
{
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldAllDn(int nSerial)
{
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldInnerUp(int nSerial)
{
	if (m_pReelMapInnerUp)
		m_pReelMapInnerUp->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldInnerDn(int nSerial)
{
	if (m_pReelMapInnerDn)
		m_pReelMapInnerDn->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldInnerAllUp(int nSerial)
{
	if (m_pReelMapInnerAllUp)
		m_pReelMapInnerAllUp->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldInnerAllDn(int nSerial)
{
	if (m_pReelMapInnerAllDn)
		m_pReelMapInnerAllDn->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateYieldIts(int nSerial)
{
	if (m_pReelMapIts)
		m_pReelMapIts->UpdateYield(nSerial);
}

void CManagerReelmap::UpdateRMapInnerUp()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (pDoc->GetTestMode() == MODE_INNER)
	{
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapInnerUp)
			m_pReelMapInnerUp->Write(Thread.nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CManagerReelmap::UpdateRMapInnerDn()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (pDoc->GetTestMode() == MODE_INNER)
	{
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapInnerDn)
			m_pReelMapInnerDn->Write(Thread.nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CManagerReelmap::UpdateRMapInnerAllUp()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapInnerAllUp)
			m_pReelMapInnerAllUp->Write(Thread.nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CManagerReelmap::UpdateRMapInnerAllDn()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapInnerAllDn)
			m_pReelMapInnerAllDn->Write(Thread.nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

int CManagerReelmap::GetPcrIdx0(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.9"));
		return 0;
	}

	int nIdx;
	nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//if (!bNewLot)
	//{
	//	if (pDoc->WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
	//	{
	//		if (!m_bDoneChgLot && nSerial <= m_nLotLastShot)
	//			nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//		else // New Lot...
	//			nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
	//	}
	//	else
	//	{
	//		nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//	}
	//}
	//else
	//{
	//	if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
	//		nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
	//	else
	//		nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//}
	return nIdx;
}

int CManagerReelmap::GetPcrIdx1(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.10"));
		return 0;
	}

	int nIdx;
	nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//if (!bNewLot)
	//{
	//	if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
	//	{
	//		if (!m_bDoneChgLot && nSerial <= m_nLotLastShot)
	//			nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//		else // New Lot...
	//			nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
	//	}
	//	else
	//	{
	//		nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//	}
	//}
	//else
	//{
	//	if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
	//		nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
	//	else
	//		nIdx = (nSerial - 1) % MAX_PCR_PNL;
	//}
	return nIdx;
}

// for ITS
CString CManagerReelmap::GetPathReelmapIts()
{
	CString sName = _T("ReelMapDataIts.txt");
	CString str, sPath, Path[3];

	sPath = _T("");
	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode; // pDoc->m_sItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
		return _T("");

	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ReelmapIts.txt
	return sPath;
}

BOOL CManagerReelmap::MakeItsReelmapHeader()
{
	FILE *fp = NULL;
	char FileName[MAX_PATH];
	BOOL bExist = FALSE;
	CString sName = _T("ReelMapDataIts.txt");
	CString str, sPath, Path[3];

	sPath = _T("");
	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode; // pDoc->m_sItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
		return FALSE;

	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ReelmapIts.txt

	CFileFind findfile;
	if (findfile.FindFile(sPath))
	{
		bExist = TRUE;
		return TRUE;
	}

	sPath.Format(_T("%s%s"), Path[0], Path[1]);					// 모델 폴더
	if (!DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);	// ITS 코드 폴더
	if (!DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ReelmapIts.txt

	StrToChar(sPath, FileName);

	fp = fopen(FileName, "a+");
	if (fp == NULL)
	{
		str.Format(_T("It is trouble to open ReelmapIts.txt\r\n%s"), sPath);
		pView->MsgBox(str);
		return FALSE;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	char* pRtn = NULL;
	int nNodeX = 0, nNodeY = 0, i = 0, k = 0;
#ifdef USE_CAM_MASTER
	nNodeX = m_Master[0].m_pPcsRgn->nCol;
	nNodeY = m_Master[0].m_pPcsRgn->nRow;
#endif

	fprintf(fp, "[Info]\n");
	fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, " ITS Code  = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sEngItsCode)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "\n");

	fprintf(fp, "Process Code = \n");
	fprintf(fp, "\n");

	fprintf(fp, "CamPcsX = %d\n", nNodeX);
	fprintf(fp, "CamPcsY = %d\n", nNodeY);
	fprintf(fp, "\n");

	fprintf(fp, "Lot Start=\n");
	fprintf(fp, "Lot Run=\n");
	fprintf(fp, "Lot End=\n");
	fprintf(fp, "\n");

	fprintf(fp, "Marked Shot=\n");
	fprintf(fp, "\n");

	fprintf(fp, "\n");
	fprintf(fp, "// < 수율 정보 > \n");
	fprintf(fp, "\n");
	fprintf(fp, "Start Serial=\n");
	fprintf(fp, "End Serial=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Total Pcs=\n");
	fprintf(fp, "Good Pcs=\n");
	fprintf(fp, "Bad Pcs=\n");
	fprintf(fp, "\n");
	fprintf(fp, "Strip0=\n");
	fprintf(fp, "Strip1=\n");
	fprintf(fp, "Strip2=\n");
	fprintf(fp, "Strip3=\n");
	for (i = 1; i < MAX_DEF; i++)
		fprintf(fp, "%d=\n", i); // m_cBigDef[i]
	fprintf(fp, "\n");

	for (k = 0; k < MAX_STRIP; k++)
	{
		fprintf(fp, "[Strip%d]\n", k);
		for (i = 1; i < MAX_DEF; i++)
			fprintf(fp, "%d=\n", i); // m_cBigDef[i]
		fprintf(fp, "\n");
	}

	fprintf(fp, "[StripOut]\n");
	fprintf(fp, "Total=\n");
	for (k = 0; k < MAX_STRIP; k++)
		fprintf(fp, "%d=\n", k);
	fprintf(fp, "\n");

	fclose(fp);
	return TRUE;
}

BOOL CManagerReelmap::WriteIts(int nItsSerial)
{
	if (m_pReelMapIts)
		return m_pReelMapIts->Write(nItsSerial);

	return FALSE;
}
//BOOL CManagerReelmap::WriteIts(int nItsSerial)
//{
//	if (nItsSerial <= 0)
//	{
//		pView->ClrDispMsg();
//		AfxMessageBox(_T("Serial Error.163"));
//		return FALSE;
//	}
//
//	if (pDoc->GetTestMode() != MODE_OUTER)
//		return FALSE;
//
//	CString str, sPath;
//	sPath = GetPathReelmapIts(); // pDoc->GetItsReelmapPath();
//	if (sPath.IsEmpty())
//	{
//		str.Format(_T("ReelmapITS Path is emptied."));
//		pView->MsgBox(str);
//		return FALSE;
//	}
//
//	if (!MakeItsReelmapHeader())	// 내외층 머징된 릴맵 헤드
//	{
//		str.Format(_T("It is trouble to open ReelmapIts.txt\r\n%s"), sPath);
//		pView->MsgBox(str);
//		return FALSE;
//	}
//
//	int nIdx = GetPcrIdx0(nItsSerial);
//	int nNodeX = m_Master[0].m_pPcsRgn->nCol;
//	int nNodeY = m_Master[0].m_pPcsRgn->nRow;
//	int nStripY = m_Master[0].m_pPcsRgn->nRow / MAX_STRIP; // Strip(1~4);
//	int nTotDefPcs = 0;
//	if (m_pPcrIts)
//	{
//		if (m_pPcrIts[nIdx])
//			nTotDefPcs = m_pPcrIts[nIdx]->m_nTotDef;
//	}
//
//	short **pPnlBuf;
//	int i, nC, nR, nPcsId, nDefCode;//, nTot, nDef, nGood;
//	pPnlBuf = new short*[nNodeY];
//	for (i = 0; i < nNodeY; i++)
//	{
//		pPnlBuf[i] = new short[nNodeX];
//		memset(pPnlBuf[i], 0, sizeof(short)*nNodeX);
//		memset(m_pPnlBuf[nItsSerial - 1][i], 0, sizeof(short)*nNodeX);
//	}
//
//	CString strData, strTemp;
//	int nTotVerifyed = 0;
//
//	for (i = 0; i < nTotDefPcs; i++)
//	{
//		if (m_pPcrIts[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
//		{
//			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
//			{
//				switch (m_MasterInner[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
//				{
//				case 0:
//					nPcsId = m_pPcrIts[nIdx]->m_pDefPcs[i];
//					break;
//				case 1:
//					nPcsId = MirrorLR(m_pPcrIts[nIdx]->m_pDefPcs[i]);
//					break;
//				case 2:
//					nPcsId = MirrorUD(m_pPcrIts[nIdx]->m_pDefPcs[i]);
//					break;
//				case 3:
//					nPcsId = Rotate180(m_pPcrIts[nIdx]->m_pDefPcs[i]);
//					break;
//				default:
//					nPcsId = m_pPcrIts[nIdx]->m_pDefPcs[i];
//					break;
//				}
//			}
//			else
//				nPcsId = m_pPcrIts[nIdx]->m_pDefPcs[i];
//
//			nDefCode = m_pPcrIts[nIdx]->m_pDefType[i];
//
//			nC = int(nPcsId / nNodeY);
//			if (nC % 2)	// 홀수.
//				nR = nNodeY * (nC + 1) - nPcsId - 1;
//			else		// 짝수.
//				nR = nPcsId - nNodeY * nC;
//			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
//			if (m_pPnlBuf)
//				m_pPnlBuf[nItsSerial - 1][nR][nC] = pPnlBuf[nR][nC];   // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용. #define PNL_TOT
//		}
//		else
//		{
//			nTotVerifyed++;
//		}
//	}
//
//	CString sPnl, sRow;
//	i = 0; strData = _T("");
//	::WritePrivateProfileString(sPnl, _T("Marked Date"), strData, sPath);
//	sPnl.Format(_T("%d"), nItsSerial);
//	strData.Format(_T("%d"), nTotDefPcs - nTotVerifyed);
//	::WritePrivateProfileString(sPnl, _T("Total Defects"), strData, sPath);
//
//	for (int nRow = 0; nRow < nNodeX; nRow++)
//	{
//		sRow.Format(_T("%02d"), nRow);
//		strData.Format(_T(""));
//		strTemp.Format(_T(""));
//
//		for (int nCol = 0; nCol < nNodeY; nCol++)
//		{
//			nR = (nNodeY - 1) - nCol;
//			nC = nRow;
//
//			if (m_pPcrIts[nIdx]->m_nErrPnl == -1 || m_pPcrIts[nIdx]->m_nErrPnl == -2)
//			{
//				nDefCode = DEF_LIGHT;
//				m_pPnlBuf[nItsSerial - 1][nR][nC] = (short)nDefCode;
//			}
//			else
//				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
//
//			strTemp.Format(_T("%2d,"), nDefCode);
//
//			if (!nCol)
//				strData.Insert(0, strTemp);
//			else
//			{
//				int nLen = strData.GetLength();
//				if (!(nCol%nStripY)) // Separate Strip
//				{
//					strData.Insert(nLen, _T("  "));
//					nLen = strData.GetLength();
//				}
//				strData.Insert(nLen, strTemp);
//			}
//		}
//
//		int nPos = strData.ReverseFind(',');
//		strData.Delete(nPos, 1);
//		::WritePrivateProfileString(sPnl, sRow, strData, sPath);
//	}
//
//	for (i = 0; i < nNodeY; i++)
//		delete[]  pPnlBuf[i];
//	delete[] pPnlBuf;
//
//	pDoc->m_nWritedItsSerial = nItsSerial;
//	return TRUE;
//}

BOOL CManagerReelmap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	MakeDirIts();

	CString sMsg;
	CFileFind cFile;
	CString sPath = GetItsPath(nSerial, nLayer);

	if (cFile.FindFile(sPath))
		DeleteFile(sPath);

	char FileName[MAX_PATH];
	StringToChar(sPath, FileName);

	char* pRtn = NULL;
	FILE *fp = NULL;
	fp = fopen(FileName, "w+");
	if (fp != NULL)
	{
		fprintf(fp, "%s", pRtn = StringToChar(GetItsFileData(nSerial, nLayer)));
		if (pRtn)
		{
			delete pRtn;
			pRtn = NULL;
		}
	}
	else
	{
		Sleep(300);

		fp = fopen(FileName, "w+");
		if (fp != NULL)
		{
			fprintf(fp, "%s", pRtn = StringToChar(GetItsFileData(nSerial, nLayer)));
			if (pRtn)
			{
				delete pRtn;
				pRtn = NULL;
			}
		}
		else
		{
			sMsg.Format(_T("It is trouble to MakeItsFile.\r\n%s"), FileName);
			pView->MsgBox(sMsg);
			return FALSE;
		}
	}

	fclose(fp);

	return TRUE;
}

BOOL CManagerReelmap::MakeItsFileUp(int nSerial)
{
	if (pDoc->GetTestMode() == MODE_INNER)
	{
		return MakeItsFile(nSerial, RMAP_INNER_UP);
	}
	else if (pDoc->GetTestMode() == MODE_OUTER)
	{
		return MakeItsFile(nSerial, RMAP_UP);
	}

	return FALSE;
}

BOOL CManagerReelmap::MakeItsFileDn(int nSerial)
{
	if (pDoc->GetTestMode() == MODE_INNER)
	{
		return MakeItsFile(nSerial, RMAP_INNER_DN);
	}
	else if (pDoc->GetTestMode() == MODE_OUTER)
	{
		return MakeItsFile(nSerial, RMAP_DN);
	}

	return FALSE;
}

CString CManagerReelmap::GetItsFileData(int nSerial, int nLayer)
{
	switch (nLayer)
	{
	case RMAP_UP:
		return m_pReelMapUp->GetItsFileData(nSerial);
	case RMAP_DN:
		return m_pReelMapDn->GetItsFileData(nSerial);
	case RMAP_INNER_UP:
		return m_pReelMapInnerUp->GetItsFileData(nSerial);
	case RMAP_INNER_DN:
		return m_pReelMapInnerDn->GetItsFileData(nSerial);
	}
	return _T("");
}
//CString CManagerReelmap::GetItsFileData(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
//{
//	CString sPath = GetItsPath(nSerial, nLayer);
//	CString str = _T(""), sSide = _T(""), sTemp = _T(""), sItsData = _T("");
//	CString sItsCode = pDoc->WorkingInfo.LastJob.sEngItsCode;
//	//CString sItsCode = pDoc->m_sItsCode;
//
//	int nNodeX = m_Master[0].m_pPcsRgn->nCol;
//	int nNodeY = m_Master[0].m_pPcsRgn->nRow;
//	int nStripY = m_Master[0].m_pPcsRgn->nRow / 4; // Strip(1~4);
//	int nIdx = GetPcrIdx0(nSerial);
//
//	int nTotDefPcs = 0;
//
//	switch (nLayer)
//	{
//	case RMAP_UP:
//		sSide = _T("T");
//		if (m_pPcr)
//		{
//			if (m_pPcr[nIdx])
//				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
//		}
//		break;
//	case RMAP_DN:
//		sSide = _T("B");
//		if (m_pPcr)
//		{
//			if (m_pPcr[nIdx])
//				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
//		}
//		break;
//	case RMAP_INNER_UP:
//		nLayer = RMAP_UP;
//		sSide = _T("T");
//		if (m_pPcr)
//		{
//			if (m_pPcr[nIdx])
//				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
//		}
//		break;
//	case RMAP_INNER_DN:
//		nLayer = RMAP_DN;
//		sSide = _T("B");
//		if (m_pPcr)
//		{
//			if (m_pPcr[nIdx])
//				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
//		}
//		break;
//	}
//
//	str.Format(_T("%d,%s,%04d\n"), nTotDefPcs, sItsCode, nSerial);
//	sItsData = str;
//
//	int nR, nRow, nCol, nDefCode;
//	int nTotStrip = 0;
//
//	// Strip A
//	for (nRow = 0; nRow < nStripY; nRow++)
//	{
//		nR = nRow;
//		for (nCol = 0; nCol < nNodeX; nCol++)
//		{
//			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
//			if (nDefCode > 0)
//			{
//				nTotStrip++;
//				str.Format(_T("%s,%04d,%s,A,%d,%d,B%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
//				sTemp += str;
//			}
//		}
//	}
//	str.Format(_T("%d,%s,%04d,%s,A\n"), nTotStrip, sItsCode, nSerial, sSide);
//	sItsData += str;
//	sItsData += sTemp;
//	str.Format(_T("%s,%04d,%s,A,EOS\n"), sItsCode, nSerial, sSide);
//	sItsData += str;
//	sTemp = _T("");
//	nTotStrip = 0;
//
//	// Strip B
//	for (nRow = 0; nRow < nStripY; nRow++)
//	{
//		nR = nRow + nStripY;
//		for (nCol = 0; nCol < nNodeX; nCol++)
//		{
//			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
//			if (nDefCode > 0)
//			{
//				nTotStrip++;
//				str.Format(_T("%s,%04d,%s,B,%d,%d,B%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
//				sTemp += str;
//			}
//		}
//	}
//	str.Format(_T("%d,%s,%04d,%s,B\n"), nTotStrip, sItsCode, nSerial, sSide);
//	sItsData += str;
//	sItsData += sTemp;
//	str.Format(_T("%s,%04d,%s,B,EOS\n"), sItsCode, nSerial, sSide);
//	sItsData += str;
//	sTemp = _T("");
//	nTotStrip = 0;
//
//	// Strip C
//	for (nRow = 0; nRow < nStripY; nRow++)
//	{
//		nR = nRow + nStripY * 2;
//		for (nCol = 0; nCol < nNodeX; nCol++)
//		{
//			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
//			if (nDefCode > 0)
//			{
//				nTotStrip++;
//				str.Format(_T("%s,%04d,%s,B,%d,%d,C%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
//				sTemp += str;
//			}
//		}
//	}
//	str.Format(_T("%d,%s,%04d,%s,C\n"), nTotStrip, sItsCode, nSerial, sSide);
//	sItsData += str;
//	sItsData += sTemp;
//	str.Format(_T("%s,%04d,%s,C,EOS\n"), sItsCode, nSerial, sSide);
//	sItsData += str;
//	sTemp = _T("");
//	nTotStrip = 0;
//
//	// Strip D
//	for (nRow = 0; nRow < nStripY; nRow++)
//	{
//		nR = nRow + nStripY * 3;
//		for (nCol = 0; nCol < nNodeX; nCol++)
//		{
//			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
//			if (nDefCode > 0)
//			{
//				nTotStrip++;
//				str.Format(_T("%s,%04d,%s,B,%d,%d,D%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
//				sTemp += str;
//			}
//		}
//	}
//	str.Format(_T("%d,%s,%04d,%s,D\n"), nTotStrip, sItsCode, nSerial, sSide);
//	sItsData += str;
//	sItsData += sTemp;
//	str.Format(_T("%s,%04d,%s,D,EOS\n"), sItsCode, nSerial, sSide);
//	sItsData += str;
//	str.Format(_T("%s,%04d,%s,EOP\n"), sItsCode, nSerial, sSide);
//	sItsData += str;
//
//	return sItsData;
//	return _T("");
//}

BOOL CManagerReelmap::MakeDirIts()
{
	CFileFind finder;
	CString  Path[3], sItsPath;
	CString sPath = _T("");

	sItsPath = pDoc->WorkingInfo.System.sPathIts;
	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//Path[2] = pDoc->m_sItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
		return FALSE;

	sPath.Format(_T("%s%s"), Path[0], Path[1]);
	if (!DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
	if (!DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\Inner"), Path[0], Path[1], Path[2]);
	if (!DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\Outer"), Path[0], Path[1], Path[2]);
	if (!DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);


	if (sItsPath.IsEmpty())
		return FALSE;

	int pos = sItsPath.ReverseFind('\\');
	if (pos != -1)
		sItsPath.Delete(pos, sItsPath.GetLength() - pos);

	if (!DirectoryExists(sItsPath))
		CreateDirectory(sItsPath, NULL);

	return TRUE;
}

BOOL CManagerReelmap::FinalCopyItsFiles()
{
	CString sPathSrc, sPathDest;
	CString sPathSrcDir, sPathDestDir;
	CString sItsFolderSrcPath = GetItsFolderPath();
	CString sItsFolderDestPath = GetItsTargetFolderPath();

	if (pDoc->GetTestMode() == MODE_INNER)
		sPathSrcDir.Format(_T("%s\\Inner"), sItsFolderSrcPath);
	else if (pDoc->GetTestMode() == MODE_OUTER)
		sPathSrcDir.Format(_T("%s\\Outer"), sItsFolderSrcPath);
	else
		return FALSE;

	CFileFind cFile;
	BOOL bExist = cFile.FindFile(sPathSrcDir + _T("\\*.dat"));
	if (!bExist)
	{
		return FALSE; // dat파일이 존재하지 않음.
	}

	CString sFileName;
	int nTot = 0;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			sFileName = cFile.GetFileName();
			sPathSrc.Format(_T("%s\\%s"), sPathSrcDir, sFileName);
			sPathDest.Format(_T("%s\\%s"), sItsFolderDestPath, sFileName);
			CopyItsFile(sPathSrc, sPathDest);
			nTot++;
		}
		if (!m_bThreadAliveFinalCopyItsFiles)
			break;
	}

	pDoc->DelItsAll(sPathSrcDir);
	return TRUE;
}

BOOL CManagerReelmap::CopyItsFile(CString sPathSrc, CString sPathDest)
{
	CFileFind cFile;

	if (!cFile.FindFile(sPathSrc))
	{
		return FALSE;
	}

	CMyFile myFile;
	BOOL bRtn = myFile.Copy(sPathSrc, sPathDest);	// ITS 파일을 복사한다.

	return bRtn;
}

CString CManagerReelmap::GetItsFolderPath()
{
	CString  Path[3];
	CString sPath = _T("");

	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
	{
		if (Path[2].IsEmpty())
		{
			if (pDoc->GetCurrentInfoEng())
			{
				pDoc->WorkingInfo.LastJob.sEngItsCode = pDoc->m_sItsCode;
				Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
			}
		}

		if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
			return sPath;
	}

	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]); // ITS Folder Path

	return sPath;
}

CString CManagerReelmap::GetItsTargetFolderPath()
{
	CString sItsPath = pDoc->WorkingInfo.System.sPathIts;

	if (sItsPath.IsEmpty())
		return _T("");

	int pos = sItsPath.ReverseFind('\\');
	if (pos != -1)
		sItsPath.Delete(pos, sItsPath.GetLength() - pos);

	return sItsPath;
}

void CManagerReelmap::DelItsAll(CString strPath)
{
	pDoc->DelItsAll(strPath);
}

BOOL CManagerReelmap::ReloadReelmapIts()
{
	if (m_pReelMapIts)
		return m_pReelMapIts->ReloadReelmap();
	return FALSE;
}

void CManagerReelmap::DuplicateRmap(int nRmap)
{
	CFileFind finder;
	CString strTemp, sSrcPath, sDstPath;
	sDstPath = _T("C:\\PunchWork\\Reelmap.txt");

	switch (nRmap)
	{
	case RMAP_UP:
		sSrcPath = m_pReelMapUp->GetRmapPath(RMAP_UP);
		break;
	case RMAP_ALLUP:
		sSrcPath = m_pReelMapAllUp->GetRmapPath(RMAP_ALLUP);
		break;
	case RMAP_ITS:
		sSrcPath = m_pReelMapUp->GetRmapPath(RMAP_ITS);
		break;
	}

	if (finder.FindFile(sSrcPath))
	{
		if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
		{
			Sleep(100);
			if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
			{
				Sleep(300);
				if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
				{
					Sleep(500);
					//strTemp.Format(_T("%s \r\n: Reelmap File Copy Fail"), sSrcPath);
					//pView->MsgBox(strTemp);
					if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
					{
						return;
					}
				}
			}
		}
	}
	else
	{
		Sleep(30);
		strTemp.Format(_T("%s \r\n: Reelmap File Not Exist"), sSrcPath);
		pView->MsgBox(strTemp);
		return;
	}

}

BOOL CManagerReelmap::GetPcrInfo(CString sPath, stModelInfo &stInfo)
{

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;// , i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;


	//strcpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		//strFileData.Format(_T("%s"), CharToString(FileData));
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(_T(','), 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//m_pPcr[nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(_T(','), 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//Status.PcrShare[1].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(_T(','), 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//Status.PcrShare[1].sLayer = strLayer;

	if (pDoc->WorkingInfo.System.bUseITS)
	{
		// Lot
		nTemp = strFileData.Find(_T(','), 0);
		strLot = strFileData.Left(nTemp);
		strFileData.Delete(0, nTemp + 1);
		nFileSize = nFileSize - nTemp - 1;
		//Status.PcrShare[1].sLot = strLot;

		// Its Code
		nTemp = strFileData.Find(_T('\n'), 0);
		sItsCode = strFileData.Left(nTemp);
		strFileData.Delete(0, nTemp + 1);
		nFileSize = nFileSize - nTemp - 1;
	}
	else
	{
		// Lot
		nTemp = strFileData.Find(_T(','), 0);
		strLot = strFileData.Left(nTemp);
		strFileData.Delete(0, nTemp + 1);
		nFileSize = nFileSize - nTemp - 1;
		//Status.PcrShare[1].sLot = strLot;

		// Lot
		nTemp = strFileData.Find(_T('\n'), 0);
		strLot = strFileData.Left(nTemp);
		strFileData.Delete(0, nTemp + 1);
		nFileSize = nFileSize - nTemp - 1;
		//Status.PcrShare[1].sLot = strLot;
	}

	nTemp = strFileData.Find(_T('\n'), 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	stInfo.sModel = strModel;
	stInfo.sLayer = strLayer;
	stInfo.sLot = strLot;
	stInfo.sItsCode = sItsCode;

	return TRUE;
}

BOOL CManagerReelmap::WriteReelmapIts()
{
	stThread& Thread = (pView->m_mgrStatus->Thread);

	BOOL bRtn = m_pReelMapIts->Write(Thread.nSerialRmapUpdate);

	if (m_pReelMap == m_pReelMapIts)
	{
		DuplicateRmap(RMAP_ITS);
	}

	return bRtn;
}

// Start the thread
void CManagerReelmap::StartThreadFinalCopyItsFiles()
{
	m_bRtnThreadFinalCopyItsFiles = FALSE;
	m_bThreadAliveFinalCopyItsFiles = TRUE;
	m_ThreadTaskFinalCopyItsFiles.Start(GetSafeHwnd(), this, ThreadProcFinalCopyItsFiles);// Start the thread
}

// Stop the thread
void CManagerReelmap::StopThreadFinalCopyItsFiles()
{
	m_ThreadTaskFinalCopyItsFiles.Stop();
}

// Thread body
BOOL CManagerReelmap::ThreadProcFinalCopyItsFiles(LPVOID lpContext)
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CManagerReelmap* pThread = reinterpret_cast<CManagerReelmap*>(lpContext);

	pThread->m_bRtnThreadFinalCopyItsFiles = TRUE;
	pThread->m_bThreadAliveFinalCopyItsFiles = TRUE;
	pThread->m_bRtnThreadFinalCopyItsFiles = pThread->FinalCopyItsFiles();
	pThread->m_bThreadAliveFinalCopyItsFiles = FALSE;

	return (pThread->m_bRtnThreadFinalCopyItsFiles);
}

char* CManagerReelmap::TCHARToChar(TCHAR *tszStr)
{
	char *szStr = NULL;
	int nLength;

	USES_CONVERSION;
	LPWSTR lpWideCharStr = T2W(tszStr);

	nLength = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL, NULL, NULL, NULL);
	szStr = new char[nLength + 1];
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, szStr, nLength, NULL, NULL);

	return szStr;
}

char* CManagerReelmap::StringToChar(CString str) // char* returned must be deleted... 
{
	char*		szStr = NULL;
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 
	szStr = new char[nLenth];  //메모리 할당 

							   //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, szStr, nLenth, 0, 0);
	return szStr;
}

BOOL CManagerReelmap::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

char* CManagerReelmap::StrToChar(CString str) // char* returned must be deleted... 
{
	char*		szStr = NULL;
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 
	szStr = new char[nLenth];  //메모리 할당 

	//3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, szStr, nLenth, 0, 0);
	return szStr;
}

void CManagerReelmap::StrToChar(CString str, char* pCh) // char* returned must be deleted... 
{	
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}

void CManagerReelmap::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}

CString CManagerReelmap::GetItsPath(int nSerial, int nLayer)	// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	CString sPath = _T("");

	switch (nLayer)
	{
	case RMAP_UP: // 외층 Top
		sPath = m_pReelMapUp->GetItsPath(nSerial);
		break;
	case RMAP_DN: // 외층 Bottom
		sPath = m_pReelMapDn->GetItsPath(nSerial);
		break;
	case RMAP_INNER_UP: // 내층 Top
		sPath = m_pReelMapInnerUp->GetItsPath(nSerial);
		break;
	case RMAP_INNER_DN: // 내층 Bottom
		sPath = m_pReelMapInnerDn->GetItsPath(nSerial);
		break;
	}

	return sPath;
	//CString sPath, str;
	//CString sItsFolderPath = GetItsFolderPath();
	//CString sTime = pView->GetTimeIts();

	//switch (nLayer)
	//{
	//case RMAP_UP: // 외층 Top
	//	str.Format(_T("%s_L2_%04d_T_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
	//	sPath.Format(_T("%s\\Outer\\%s"), sItsFolderPath, str);
	//	break;
	//case RMAP_DN: // 외층 Bottom
	//	str.Format(_T("%s_L2_%04d_B_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
	//	sPath.Format(_T("%s\\Outer\\%s"), sItsFolderPath, str);
	//	break;
	//case RMAP_INNER_UP: // 내층 Top
	//	str.Format(_T("%s_L1_%04d_T_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
	//	sPath.Format(_T("%s\\Inner\\%s"), sItsFolderPath, str);
	//	break;
	//case RMAP_INNER_DN: // 내층 Bottom
	//	str.Format(_T("%s_L1_%04d_B_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
	//	sPath.Format(_T("%s\\Inner\\%s"), sItsFolderPath, str);
	//	break;
	//}

	//return sPath;
}

CString CManagerReelmap::CharToString(const char *szStr)
{
	CString strRet;

	int nLen = strlen(szStr) + sizeof(char);
	wchar_t *tszTemp = NULL;
	tszTemp = new WCHAR[nLen];

	MultiByteToWideChar(CP_ACP, 0, szStr, -1, tszTemp, nLen * sizeof(WCHAR));

	strRet.Format(_T("%s"), (CString)tszTemp);
	if (tszTemp)
	{
		delete[] tszTemp;
		tszTemp = NULL;
	}
	return strRet;
}

void CManagerReelmap::SetUserName(CString sName)
{
	if (m_pReelMapUp)
		m_pReelMapUp->m_sUser = sName;
	if (m_pReelMapDn)
		m_pReelMapDn->m_sUser = sName;
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->m_sUser = sName;
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->m_sUser = sName;
}

void CManagerReelmap::SetOnePnlLen(double dPnlLen)
{
	if (m_pReelMapUp)
		m_pReelMapUp->m_dPnlLen = dPnlLen;
	if (m_pReelMapDn)
		m_pReelMapDn->m_dPnlLen = dPnlLen;
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->m_dPnlLen = dPnlLen;
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->m_dPnlLen = dPnlLen;
}

void CManagerReelmap::SetTempPauseLen(double dTempPauseLen)
{
	if (m_pReelMapUp)
		m_pReelMapUp->m_dTempPauseLen = dTempPauseLen;
	if (m_pReelMapDn)
		m_pReelMapDn->m_dTempPauseLen = dTempPauseLen;
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->m_dTempPauseLen = dTempPauseLen;
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->m_dTempPauseLen = dTempPauseLen;
}

void CManagerReelmap::SetLotCutPosLen(double dLotCutPosLen)
{
	if (m_pReelMapUp)
		m_pReelMapUp->m_dLotCutPosLen = dLotCutPosLen;
	if (m_pReelMapDn)
		m_pReelMapDn->m_dLotCutPosLen = dLotCutPosLen;
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->m_dLotCutPosLen = dLotCutPosLen;
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->m_dLotCutPosLen = dLotCutPosLen;
}

void CManagerReelmap::SetPathAtBuf()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		m_pReelMapUp->SetPathAtBuf();
	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->SetPathAtBuf();
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->SetPathAtBuf();
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->SetPathAtBuf();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapIts)
			m_pReelMapIts->SetPathAtBuf();
	}
}

void CManagerReelmap::SetCompletedSerial(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		m_pReelMapUp->SetCompletedSerial(nSerial);
	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->SetCompletedSerial(nSerial);
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->SetCompletedSerial(nSerial);
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->SetCompletedSerial(nSerial);
	}
}

CfPoint CManagerReelmap::GetMkPnt(int nSerial, int nMkPcs) // pcr 시리얼, pcr 불량 피스 읽은 순서 인덱스
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

	if (bDualTest)
	{
		if(m_pReelMapAllUp)
			ptPnt = m_pReelMapAllUp->GetMkPnt(nSerial, nMkPcs);
	}
	else
	{
		if (m_pReelMapUp)
			ptPnt = m_pReelMapUp->GetMkPnt(nSerial, nMkPcs);
	}

	return ptPnt;
}

CfPoint CManagerReelmap::GetMkPntIts(int nSerial, int nMkPcs) // pcr 시리얼, pcr 불량 피스 읽은 순서 인덱스
{
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

	if (m_pReelMapIts)
		ptPnt = m_pReelMapIts->GetMkPnt(nSerial, nMkPcs);

	return ptPnt;
}

void CManagerReelmap::CropCadImgUp(int nIdxMkInfo, int nSerial, int nIdxDef)
{
	if (m_pReelMapUp)
		m_pReelMapUp->CropCadImg(nIdxMkInfo, nSerial, nIdxDef);
}

void CManagerReelmap::CropCadImgDn(int nIdxMkInfo, int nSerial, int nIdxDef)
{
	if (m_pReelMapDn)
		m_pReelMapDn->CropCadImg(nIdxMkInfo, nSerial, nIdxDef);
}

void CManagerReelmap::ClrMkInfo()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (m_pReelMapUp)
		m_pReelMapUp->Clear();
	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->Clear();
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->Clear();
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->Clear();
	}

	if (m_pReelMap != m_pReelMapUp && m_pReelMap != m_pReelMapDn && m_pReelMap != m_pReelMapAllUp && m_pReelMap != m_pReelMapAllDn)
	{
		if (m_pReelMap)
			m_pReelMap->Clear();
	}
}

void CManagerReelmap::SetPathAtBufUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		m_pReelMapUp->SetPathAtBuf();

	if (bDualTest)
	{
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->SetPathAtBuf();
	}
}

void CManagerReelmap::SetPathAtBufDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (m_pReelMapDn)
		m_pReelMapDn->SetPathAtBuf();
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->SetPathAtBuf();
}

COLORREF CManagerReelmap::GetRgbStcDef(int nDefColor)
{
	if (m_pReelMap)
		return m_pReelMap->m_rgbDef[nDefColor];
	else if(m_pReelMapUp)
		return m_pReelMapUp->m_rgbDef[nDefColor];

	return RGB_BLACK;
}

CString CManagerReelmap::GetKorDef(int nDefCode)
{
	return m_sKorDef[nDefCode];
}

void CManagerReelmap::ResetReelmapPath()
{
	if (m_pReelMapUp)
		m_pReelMapUp->ResetReelmapPath();

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pReelMapInnerUp)
			m_pReelMapInnerUp->ResetReelmapPath();

		BOOL bDualTestInner;
		CString sLot, sLayerUp, sLayerDn, str;
		if (!pView->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn))
		{
			str.Format(_T("It is trouble to read GetItsSerialInfo()."));
			pView->MsgBox(str);
			return;
		}

		if (bDualTestInner)
		{
			if (m_pReelMapInnerDn)
				m_pReelMapInnerDn->ResetReelmapPath();
			if (m_pReelMapInnerAllUp)
				m_pReelMapInnerAllUp->ResetReelmapPath();
			if (m_pReelMapInnerAllDn)
				m_pReelMapInnerAllDn->ResetReelmapPath();
		}
	}
}

BOOL CManagerReelmap::ChkYield() // 수율 양호 : TRUE , 수율 불량 : FALSE
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nGood = 0, nBad = 0, nTot;
	double dRatio;
	CString sMsg;

	if (bDualTest)
	{
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->GetPcsNum(nGood, nBad);
		else
		{
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			sMsg.Format(_T("일시정지 - Failed ChkYield()."));
			pView->MsgBox(sMsg);
			return FALSE;
		}
	}
	else
	{
		if (m_pReelMapUp)
			m_pReelMapUp->GetPcsNum(nGood, nBad);
		else
		{
			Buzzer(TRUE, 0);
			Stop();
			sMsg.Format(_T("일시정지 - Failed ChkYield()."));
			pView->MsgBox(sMsg);
			return FALSE;
		}
	}

	nTot = (nGood + nBad);
	double dTotLmt = _tstof(pDoc->WorkingInfo.LastJob.sLmtTotYld);
	if (dTotLmt > 0.0)
	{
		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		if (dRatio < dTotLmt)
		{
			Buzzer(TRUE, 0);
			Stop();
			sMsg.Format(_T("일시정지 - 전체 수율 제한범위 : %.1f 미달 ( %.1f )"), dTotLmt, dRatio);
			pView->MsgBox(sMsg);
			return FALSE;
		}
	}

	double dPrtLmt = _tstof(pDoc->WorkingInfo.LastJob.sLmtPatlYld);
	if (dPrtLmt > 0.0)
	{
		if (bDualTest)
		{
			if (pDoc->m_Yield[2].IsValid())
			{
				dRatio = pDoc->m_Yield[2].GetYield();

				if (dRatio < dPrtLmt)
				{
					Buzzer(TRUE, 0);
					Stop();
					sMsg.Format(_T("일시정지 - 구간 수율 제한범위 : %.1f 미달 ( %.1f )"), dPrtLmt, dRatio);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
		}
		else
		{
			if (pDoc->m_Yield[0].IsValid())
			{
				dRatio = pDoc->m_Yield[0].GetYield();

				if (dRatio < dPrtLmt)
				{
					Buzzer(TRUE, 0);
					Stop();
					sMsg.Format(_T("일시정지 - 구간 수율 제한범위 : %.1f 미달 ( %.1f )"), dPrtLmt, dRatio);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}
