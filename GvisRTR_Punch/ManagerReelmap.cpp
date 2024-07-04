#include "stdafx.h"
#include "ManagerReelmap.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerReelmap::CManagerReelmap(CWnd* pParent)
{
	m_pParent = pParent;

	Reset();
	Init();

	if (!Create())
	{
		pView->ClrDispMsg();
	}
}


CManagerReelmap::~CManagerReelmap()
{
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

	m_rgbDef[DEF_NONE] = (RGB_WHITE); // (RGB_GREEN)
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

BOOL CManagerReelmap::ReloadReelmap(int nSerial)
{
	m_nReloadReelmapSerial = nSerial;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_bTHREAD_RELOAD_RST_UP = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_RELOAD_RST_DN = TRUE;
		m_bTHREAD_RELOAD_RST_ALLUP = TRUE;
		m_bTHREAD_RELOAD_RST_ALLDN = TRUE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		m_bTHREAD_RELOAD_RST_UP_INNER = TRUE;
		m_bTHREAD_RELOAD_RST_ITS = TRUE;
		if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		{
			m_bTHREAD_RELOAD_RST_DN_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLUP_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLDN_INNER = TRUE;
		}
	}

	Sleep(100);

	return TRUE;
}

void CManagerReelmap::InitReelmap()
{
	pDoc->InitReelmap();
	pDoc->SetReelmap(ROT_NONE);
	pDoc->UpdateData();
}

void CManagerReelmap::InitReelmapUp()
{
	pDoc->InitReelmapUp();
	pDoc->SetReelmap(ROT_NONE);
	pDoc->UpdateData();
}

void CManagerReelmap::InitReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->InitReelmapDn();
		pDoc->SetReelmap(ROT_NONE);
		pDoc->UpdateData();
	}
}

void CManagerReelmap::InitReelmapInner()
{
	pDoc->InitReelmapInner();
	pDoc->SetReelmapInner(ROT_NONE);
}

void CManagerReelmap::InitReelmapInnerUp()
{
	pDoc->InitReelmapInnerUp();
	pDoc->SetReelmapInner(ROT_NONE);
}

void CManagerReelmap::InitReelmapInnerDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if (bDualTest)
	{
		pDoc->InitReelmapInnerDn();
		pDoc->SetReelmapInner(ROT_NONE);
	}
}

BOOL CManagerReelmap::OpenReelmapFromBuf(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	stModelInfo stInfoUp, stInfoDn;
	CString sSrc;
	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!pDoc->GetPcrInfo(sSrc, stInfoUp))
	{
		pView->DispStsBar(_T("E(6)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(6)"));
		return FALSE;
	}
	if (bDualTest)
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);
		if (!pDoc->GetPcrInfo(sSrc, stInfoDn))
		{
			pView->DispStsBar(_T("E(7)"), 5);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error-GetPcrInfo(7)"));
			return FALSE;
		}
	}

	if (m_pReelMapUp)
		m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->Open();
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->Open();
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->Open();
	}

	return TRUE;
}

void CManagerReelmap::OpenReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapDn)
			m_pReelMapDn->Open();
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->Open();
		if (m_pReelMapAllDn)
			m_pReelMapAllDn->Open();
	}
}

void CManagerReelmap::OpenReelmapUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
		m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapAllUp)
			m_pReelMapAllUp->Open();
	}

	//if (m_pReelMap)
	//{
	//	if (m_pReelMap->m_nLayer < 0)
	//		m_pReelMap->m_nLayer = pView->m_nSelRmap;

	//	if (bDualTest)
	//	{
	//		if (m_pReelMap->m_nLayer == RMAP_UP || m_pReelMap->m_nLayer == RMAP_ALLUP)
	//			m_pReelMap->Open();
	//	}
	//	else
	//	{
	//		if (m_pReelMap->m_nLayer == RMAP_UP)
	//			m_pReelMap->Open();
	//	}
	//}
}

void CManagerReelmap::OpenReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (m_pReelMapDn)
		m_pReelMapDn->Open();
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->Open();

	//if (m_pReelMap)
	//{
	//	if (m_pReelMap->m_nLayer < 0)
	//		m_pReelMap->m_nLayer = pView->m_nSelRmap;

	//	if (m_pReelMap->m_nLayer == RMAP_DN || m_pReelMap->m_nLayer == RMAP_ALLDN)
	//		m_pReelMap->Open();
	//}
}

void CManagerReelmap::OpenReelmapInner()
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (m_pReelMapInnerUp)
		m_pReelMapInnerUp->Open();

	if (m_pReelMapIts)
		m_pReelMapIts->Open();

	if (bDualTest)
	{
		if (m_pReelMapInnerDn)
			m_pReelMapInnerDn->Open();
		if (m_pReelMapInnerAllUp)
			m_pReelMapInnerAllUp->Open();
		if (m_pReelMapInnerAllDn)
			m_pReelMapInnerAllDn->Open();
	}

	//if (m_pReelMapInner)
	//{
	//	if (m_pReelMapInner->m_nLayer < 0)
	//		m_pReelMapInner->m_nLayer = pView->m_nSelRmapInner;
	//	m_pReelMapInner->Open();
	//}

	//if (m_pReelMapInOuterUp)
	//	m_pReelMapInOuterUp->Open(GetRmapPath(RMAP_INOUTER_UP));

	//bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	//if (bDualTest)
	//{
	//	if (m_pReelMapInOuterDn)
	//		m_pReelMapInOuterDn->Open(GetRmapPath(RMAP_INOUTER_UP));
	//}
}

void CManagerReelmap::OpenReelmapInnerUp()
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (m_pReelMapInnerUp)
		m_pReelMapInnerUp->Open();

	if (bDualTest)
	{
		if (m_pReelMapInnerAllUp)
			m_pReelMapInnerAllUp->Open();
	}

	//if (m_pReelMapInner)
	//{
	//	if (m_pReelMapInner->m_nLayer < 0)
	//		m_pReelMapInner->m_nLayer = pView->m_nSelRmapInner;

	//	if (bDualTest)
	//	{
	//		if (m_pReelMapInner->m_nLayer == RMAP_UP || m_pReelMapInner->m_nLayer == RMAP_ALLUP)
	//			m_pReelMapInner->Open();
	//	}
	//	else
	//	{
	//		if (m_pReelMapInner->m_nLayer == RMAP_UP)
	//			m_pReelMapInner->Open();
	//	}
	//}
}

void CManagerReelmap::OpenReelmapInnerDn()
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (!bDualTest)
		return;

	if (m_pReelMapInnerDn)
		m_pReelMapInnerDn->Open();
	if (m_pReelMapInnerAllDn)
		m_pReelMapInnerAllDn->Open();

	//if (m_pReelMapInner)
	//{
	//	if (m_pReelMapInner->m_nLayer < 0)
	//		m_pReelMapInner->m_nLayer = pView->m_nSelRmapInner;

	//	if (m_pReelMapInner->m_nLayer == RMAP_DN || m_pReelMapInner->m_nLayer == RMAP_ALLDN)
	//		m_pReelMapInner->Open();
	//}
}

int CManagerReelmap::LoadPcrUp(int nSerial)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (m_pReelMapUp)
		m_pReelMapUp->LoadPCR(nSerial);
}

int CManagerReelmap::LoadPcrDn(int nSerial)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (m_pReelMapDn)
		m_pReelMapDn->LoadPCR(nSerial);
}

BOOL CManagerReelmap::UpdateReelmap(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.54"));
		return 0;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_nSerialRmapUpdate = nSerial;

	m_bTHREAD_UPDATE_REELMAP_UP = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_UPDATE_REELMAP_DN = TRUE;
		m_bTHREAD_UPDATE_REELMAP_ALLUP = TRUE;
		m_bTHREAD_UPDATE_REELMAP_ALLDN = TRUE;
	}

	//if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	//	UpdateReelmapInner(nSerial);

	Sleep(100);
	return TRUE;
}

BOOL CManagerReelmap::MakeItsFile(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.54"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	m_nSerialMakeItsFile = nSerial;
	m_bTHREAD_MAKE_ITS_FILE_UP = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_MAKE_ITS_FILE_DN = TRUE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
		m_bTHREAD_UPDATE_REELMAP_ITS = TRUE;

	Sleep(100);
	return TRUE;
}

int CManagerReelmap::GetVsBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotMk();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

int CManagerReelmap::GetVsUpBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotUp();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

int CManagerReelmap::GetVsDnBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotDn();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
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
	nNodeX = m_pPcsRgn->nCol;
	nNodeY = m_pPcsRgn->nRow;
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
	if (nItsSerial <= 0)
	{
		pView->ClrDispMsg();
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.163"));
		return FALSE;
	}

	if (pDoc->GetTestMode() != MODE_OUTER)
		return FALSE;

	CString str, sPath;
	sPath = GetPathReelmapIts(); // pDoc->GetItsReelmapPath();
	if (sPath.IsEmpty())
	{
		str.Format(_T("ReelmapITS Path is emptied."));
		pView->MsgBox(str);
		return FALSE;
	}

	if (!MakeItsReelmapHeader())	// 내외층 머징된 릴맵 헤드
	{
		str.Format(_T("It is trouble to open ReelmapIts.txt\r\n%s"), sPath);
		pView->MsgBox(str);
		return FALSE;
	}

	int nIdx = pDoc->GetPcrIdx0(nItsSerial);
	int nNodeX = pDoc->m_MasterInner[0].m_pPcsRgn->nCol;
	int nNodeY = pDoc->m_MasterInner[0].m_pPcsRgn->nRow;
	int nStripY = pDoc->m_MasterInner[0].m_pPcsRgn->nRow / MAX_STRIP; // Strip(1~4);
	int nTotDefPcs = 0;
	if (m_pPcrIts)
	{
		if (m_pPcrIts[nIdx])
			nTotDefPcs = m_pPcrIts[nIdx]->m_nTotDef;
	}

	short **pPnlBuf;
	int i, nC, nR, nPcsId, nDefCode;//, nTot, nDef, nGood;
	pPnlBuf = new short*[nNodeY];
	for (i = 0; i < nNodeY; i++)
	{
		pPnlBuf[i] = new short[nNodeX];
		memset(pPnlBuf[i], 0, sizeof(short)*nNodeX);
		memset(m_pPnlBuf[nItsSerial - 1][i], 0, sizeof(short)*nNodeX);
	}

	CString strData, strTemp;
	int nTotVerifyed = 0;

	for (i = 0; i < nTotDefPcs; i++)
	{
		if (m_pPcrIts[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				switch (pDoc->m_MasterInner[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
				{
				case 0:
					nPcsId = m_pPcrIts[nIdx]->m_pDefPcs[i];
					break;
				case 1:
					nPcsId = MirrorLR(m_pPcrIts[nIdx]->m_pDefPcs[i]);
					break;
				case 2:
					nPcsId = MirrorUD(m_pPcrIts[nIdx]->m_pDefPcs[i]);
					break;
				case 3:
					nPcsId = Rotate180(m_pPcrIts[nIdx]->m_pDefPcs[i]);
					break;
				default:
					nPcsId = m_pPcrIts[nIdx]->m_pDefPcs[i];
					break;
				}
			}
			else
				nPcsId = m_pPcrIts[nIdx]->m_pDefPcs[i];

			nDefCode = m_pPcrIts[nIdx]->m_pDefType[i];

			nC = int(nPcsId / nNodeY);
			if (nC % 2)	// 홀수.
				nR = nNodeY * (nC + 1) - nPcsId - 1;
			else		// 짝수.
				nR = nPcsId - nNodeY * nC;
			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
			if (m_pPnlBuf)
				m_pPnlBuf[nItsSerial - 1][nR][nC] = pPnlBuf[nR][nC];   // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용. #define PNL_TOT
		}
		else
		{
			nTotVerifyed++;
		}
	}

	CString sPnl, sRow;
	i = 0; strData = _T("");
	::WritePrivateProfileString(sPnl, _T("Marked Date"), strData, sPath);
	sPnl.Format(_T("%d"), nItsSerial);
	strData.Format(_T("%d"), nTotDefPcs - nTotVerifyed);
	::WritePrivateProfileString(sPnl, _T("Total Defects"), strData, sPath);

	for (int nRow = 0; nRow < nNodeX; nRow++)
	{
		sRow.Format(_T("%02d"), nRow);
		strData.Format(_T(""));
		strTemp.Format(_T(""));

		for (int nCol = 0; nCol < nNodeY; nCol++)
		{
			nR = (nNodeY - 1) - nCol;
			nC = nRow;

			if (m_pPcrIts[nIdx]->m_nErrPnl == -1 || m_pPcrIts[nIdx]->m_nErrPnl == -2)
			{
				nDefCode = DEF_LIGHT;
				m_pPnlBuf[nItsSerial - 1][nR][nC] = (short)nDefCode;
			}
			else
				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];

			strTemp.Format(_T("%2d,"), nDefCode);

			if (!nCol)
				strData.Insert(0, strTemp);
			else
			{
				int nLen = strData.GetLength();
				if (!(nCol%nStripY)) // Separate Strip
				{
					strData.Insert(nLen, _T("  "));
					nLen = strData.GetLength();
				}
				strData.Insert(nLen, strTemp);
			}
		}

		int nPos = strData.ReverseFind(',');
		strData.Delete(nPos, 1);
		::WritePrivateProfileString(sPnl, sRow, strData, sPath);
	}

	for (i = 0; i < nNodeY; i++)
		delete[]  pPnlBuf[i];
	delete[] pPnlBuf;

	pDoc->m_nWritedItsSerial = nItsSerial;
	return TRUE;
}

BOOL CManagerReelmap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	MakeDirIts();

	CString sMsg;
	CFileFind cFile;
	CString sPath = pDoc->GetItsPath(nSerial, nLayer);

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

CString CManagerReelmap::GetItsFileData(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	CString sPath = pDoc->GetItsPath(nSerial, nLayer);
	CString str = _T(""), sSide = _T(""), sTemp = _T(""), sItsData = _T("");
	CString sItsCode = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//CString sItsCode = pDoc->m_sItsCode;

	int nNodeX = m_pPcsRgn->nCol;
	int nNodeY = m_pPcsRgn->nRow;
	int nStripY = m_pPcsRgn->nRow / 4; // Strip(1~4);
	int nIdx = pDoc->GetPcrIdx0(nSerial);

	int nTotDefPcs = 0;

	switch (nLayer)
	{
	case RMAP_UP:
		sSide = _T("T");
		if (m_pPcr)
		{
			if (m_pPcr[nIdx])
				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
		}
		break;
	case RMAP_DN:
		sSide = _T("B");
		if (m_pPcr)
		{
			if (m_pPcr[nIdx])
				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
		}
		break;
	case RMAP_INNER_UP:
		nLayer = RMAP_UP;
		sSide = _T("T");
		if (m_pPcr)
		{
			if (m_pPcr[nIdx])
				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
		}
		break;
	case RMAP_INNER_DN:
		nLayer = RMAP_DN;
		sSide = _T("B");
		if (m_pPcr)
		{
			if (m_pPcr[nIdx])
				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
		}
		break;
	}

	str.Format(_T("%d,%s,%04d\n"), nTotDefPcs, sItsCode, nSerial);
	sItsData = str;

	int nR, nRow, nCol, nDefCode;
	int nTotStrip = 0;

	// Strip A
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode > 0)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,A,%d,%d,B%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,A\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,A,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	sTemp = _T("");
	nTotStrip = 0;

	// Strip B
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow + nStripY;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode > 0)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,B,%d,%d,B%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,B\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,B,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	sTemp = _T("");
	nTotStrip = 0;

	// Strip C
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow + nStripY * 2;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode > 0)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,B,%d,%d,C%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,C\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,C,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	sTemp = _T("");
	nTotStrip = 0;

	// Strip D
	for (nRow = 0; nRow < nStripY; nRow++)
	{
		nR = nRow + nStripY * 3;
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			nDefCode = m_pPnlBuf[nSerial - 1][nR][nCol]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
			if (nDefCode > 0)
			{
				nTotStrip++;
				str.Format(_T("%s,%04d,%s,B,%d,%d,D%d\n"), sItsCode, nSerial, sSide, nCol + 1, nRow + 1, pDoc->GetItsDefCode(nDefCode));
				sTemp += str;
			}
		}
	}
	str.Format(_T("%d,%s,%04d,%s,D\n"), nTotStrip, sItsCode, nSerial, sSide);
	sItsData += str;
	sItsData += sTemp;
	str.Format(_T("%s,%04d,%s,D,EOS\n"), sItsCode, nSerial, sSide);
	sItsData += str;
	str.Format(_T("%s,%04d,%s,EOP\n"), sItsCode, nSerial, sSide);
	sItsData += str;

	return sItsData;
	return _T("");
}

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
	CString sItsFolderSrcPath = pDoc->GetItsFolderPath();
	CString sItsFolderDestPath = pDoc->GetItsTargetFolderPath();

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

void CManagerReelmap::StartThreadFinalCopyItsFiles()
{
	m_bRtnThreadFinalCopyItsFiles = FALSE;
	m_bThreadAliveFinalCopyItsFiles = TRUE;
	m_ThreadTaskFinalCopyItsFiles.Start(GetSafeHwnd(), this, ThreadProcFinalCopyItsFiles);// Start the thread
}

void CManagerReelmap::StopThreadFinalCopyItsFiles()
{
	m_ThreadTaskFinalCopyItsFiles.Stop();// Stop the thread
}

// Thread body
BOOL CManagerReelmap::ThreadProcFinalCopyItsFiles(LPVOID lpContext)
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CReelMap* pThread = reinterpret_cast<CReelMap*>(lpContext);

	pThread->m_bRtnThreadFinalCopyItsFiles = TRUE;
	pThread->m_bThreadAliveFinalCopyItsFiles = TRUE;
	pThread->m_bRtnThreadFinalCopyItsFiles = pThread->FinalCopyItsFiles();
	pThread->m_bThreadAliveFinalCopyItsFiles = FALSE;

	return (pThread->m_bRtnThreadFinalCopyItsFiles);
}
