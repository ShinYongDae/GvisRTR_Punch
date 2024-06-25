#include "../stdafx.h"
#include "ReelYield.h"

#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

CReelYield::CReelYield(int nLayer, int nPnl, int nPcs, CWnd* pParent)
{
	m_pParent = pParent;
	m_nLayer = -1;
	nTotPnl = nPnl;
	nTotPcs = nPcs;
	nDir = ROT_NONE;

	Init();
}


CReelYield::~CReelYield()
{
}


void CReelYield::Init()
{
	m_nBeforeSerial = 0;

	TCHAR szData[MAX_PATH];
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Start Shot"), NULL, szData, sizeof(szData), m_sPathYield))
		m_nStartSerial = _tstoi(szData);
	else
		m_nStartSerial = 0;
}

BOOL CReelYield::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

CString CReelYield::GetPath(int nRmap)
{
	CString sPath;
	CString str;

	switch (nRmap)
	{
	case RMAP_UP:
		str = _T("YieldUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_ALLUP:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_DN:
		str = _T("YieldDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ALLDN:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp,
			str);
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerDn,
			str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerUp,
			str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLotUp, pDoc->WorkingInfo.LastJob.sInnerLayerDn,
			str);
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode, //pDoc->m_sItsCode,
			str);
		break;
	}

	return sPath;
}

void CReelYield::Reset()
{
	m_nBeforeSerial = 0;

	m_stYield.nTot = 0; m_stYield.nGood = 0; m_stYield.nDef = 0;
	m_stYield.nTotSriptOut = 0;
	m_stYield.nDefStrip[0] = 0; m_stYield.nDefStrip[1] = 0; m_stYield.nDefStrip[2] = 0; m_stYield.nDefStrip[3] = 0;
	m_stYield.nStripOut[0] = 0; m_stYield.nStripOut[1] = 0; m_stYield.nStripOut[2] = 0; m_stYield.nStripOut[3] = 0;

	for (int k = 0; k < MAX_DEF; k++)
	{
		m_stYield.nDefA[k] = 0;

		for (int i = 0; i < MAX_STRIP; i++)
		{
			m_stYield.nDefPerStrip[i][k] = 0;
		}
	}
}

BOOL CReelYield::Update(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.68"));
		return 0;
	}

	m_sPathYield = GetPath(m_nLayer);
	CString sPath = m_sPathYield;

	BOOL bExist = FALSE;
	CFileFind findfile;
	if (findfile.FindFile(sPath))
		bExist = TRUE;

	if (bExist)
	{
		TCHAR szData[MAX_PATH];
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Start Shot"), NULL, szData, sizeof(szData), sPath))
			m_nBeforeSerial = _tstoi(szData);
	}

	int nPnl = m_nBeforeSerial;//nSerial - 1;

	if (bExist && nPnl > 0)
		Read(nPnl, sPath);

	Write(nSerial, sPath);

	Sleep(10);

	return TRUE;
}

void CReelYield::UpdateTotalSpeed(CString sSpeed, int nLayer)
{
	::WritePrivateProfileString(_T("Info"), _T("Entire Speed"), sSpeed, GetPath(nLayer));
}

int CReelYield::GetDefNum(int nDefCode)
{
	return m_stYield.nDefA[nDefCode];
}

int CReelYield::GetDefStrip(int nStrip)
{
	return m_stYield.nDefStrip[nStrip];
}

int CReelYield::GetDefStrip(int nStrip, int nDefCode)
{
	return m_stYield.nDefPerStrip[nStrip][nDefCode];
}

int CReelYield::GetStripOut(int nStrip)
{
	return m_stYield.nStripOut[nStrip];
}

void CReelYield::GetPcsNum(int &nGood, int &nBad)
{
	nGood = m_stYield.nGood;
	nBad = m_stYield.nDef;
}

BOOL CReelYield::MakeDir()
{
	CString str, sPath, Path[5];

	switch (m_nLayer)
	{
	case RMAP_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
		sPath.Format(_T("%s%s"), Path[0], Path[1]);
		if (!DirectoryExists(sPath))
			CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
			sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
			if (!DirectoryExists(sPath))
				CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
				sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
				if (!DirectoryExists(sPath))
					CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
					if (!DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
			}
		}
	}

	return TRUE;

}

BOOL CReelYield::MakeDir(CString sPath)
{
	CString str, Path[5];

	switch (m_nLayer)
	{
	case RMAP_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
		sPath.Format(_T("%s%s"), Path[0], Path[1]);
		if (!DirectoryExists(sPath))
			CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
			sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
			if (!DirectoryExists(sPath))
				CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
				sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
				if (!DirectoryExists(sPath))
					CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
					if (!DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
			}
		}
	}

	return TRUE;
}

BOOL CReelYield::Read(int nSerial, CString sPath)
{
	int dwStart = GetTickCount();

	int i, k;
	TCHAR szData[200];

	CString sCode, strMenu, strItem, sMsg;

	strMenu.Format(_T("%d"), nSerial);
	for (i = 1; i < MAX_DEF; i++)
	{
		sCode.Format(_T("%d"), i);
		if (0 < ::GetPrivateProfileString(strMenu, sCode, NULL, szData, sizeof(szData), sPath))
		{
			m_stYield.nDefA[i] = _ttoi(szData);
		}
		else
		{
			if (nSerial == 1)
			{
				m_stYield.nDefA[i] = 0;
			}
			else
			{
				//sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-불량(%d)\r\n%s"), nSerial, i, sPath);
				//AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
				return FALSE;
			}
		}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("Total Pcs"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nTot = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nTot = 0;
		}
		else
		{
			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Total Pcs\r\n%s"), nSerial, sPath);
			pView->ClrDispMsg();
			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("Good Pcs"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nGood = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nGood = 0;
		}
		else
		{
			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Good Pcs\r\n%s"), nSerial, sPath);
			pView->ClrDispMsg();
			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("Bad Pcs"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nDef = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nDef = 0;
		}
		else
		{
			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Bad Pcs\r\n%s"), nSerial, sPath);
			pView->ClrDispMsg();
			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
	}

	for (k = 0; k < MAX_STRIP; k++)
	{
		strItem.Format(_T("Strip%d"), k);
		if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
		{
			m_stYield.nDefStrip[k] = _ttoi(szData);
		}
		else
		{
			if (nSerial == 1)
			{
				m_stYield.nDefStrip[k] = 0;
			}
			else
			{
				sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Strip%d\r\n%s"), nSerial, k, sPath);
				pView->ClrDispMsg();
				AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
				return FALSE;
			}
		}

		strItem.Format(_T("StripOut_%d"), k);
		if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
		{
			m_stYield.nStripOut[k] = _ttoi(szData);
		}
		else
		{
			if (nSerial == 1)
			{
				m_stYield.nStripOut[k] = 0;
			}
			else
			{
				sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-StripOut_%d\r\n%s"), nSerial, k, sPath);
				pView->ClrDispMsg();
				AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
				return FALSE;
			}
		}

		for (i = 1; i < MAX_DEF; i++)
		{
			strItem.Format(_T("Strip%d_%d"), k, i);
			if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
			{
				m_stYield.nDefPerStrip[k][i] = _ttoi(szData);
			}
			else
			{
				if (nSerial == 1)
				{
					m_stYield.nDefPerStrip[k][i] = 0;
				}
				else
				{
					sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Strip%d_%d\r\n%s"), nSerial, k, i, sPath);
					pView->ClrDispMsg();
					AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
					return FALSE;
				}
			}
		}
	}

	if (0 < ::GetPrivateProfileString(strMenu, _T("StripOut_Total"), NULL, szData, sizeof(szData), sPath))
	{
		m_stYield.nTotSriptOut = _ttoi(szData);
	}
	else
	{
		if (nSerial == 1)
		{
			m_stYield.nTotSriptOut = 0;
		}
		else
		{
			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-StripOut_Total\r\n%s"), nSerial, sPath);
			pView->ClrDispMsg();
			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
			return FALSE;
		}
	}

	int dwEnd = GetTickCount();
	int dwElapsed = dwEnd - dwStart;

	return TRUE;
}

BOOL CReelYield::Write(int nSerial, CString sPath)
{
	CReelMap* pReelmap = (CReelMap*)m_pParent;

	m_nBeforeSerial = nSerial;

	int dwStart = GetTickCount();
	int nNodeX = 0, nNodeY = 0;
#ifdef USE_CAM_MASTER
	nNodeX = m_pPcsRgn->nCol;
	nNodeY = m_pPcsRgn->nRow;
#endif
	CString sDefNum, strData;
	int nPnl, nRow, nCol, nDefCode, nStrip;
	int nTotPcs = nNodeX * nNodeY;
	int nStripPcs = nTotPcs / 4;
	double dStOutRto = _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	nPnl = nSerial - 1;

	int i, k;
	TCHAR szData[200];

	CString sCode, strMenu, strItem, sMsg;
	int nTot = 0, nGood = 0, nDef = 0;
	int nTotSriptOut = 0;

	int nDefStrip[MAX_STRIP], nDefA[MAX_DEF], nDefPerStrip[MAX_STRIP][MAX_DEF], nStripOut[MAX_STRIP];
	nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;
	nStripOut[0] = 0; nStripOut[1] = 0; nStripOut[2] = 0; nStripOut[3] = 0;

	nTot = nNodeX * nNodeY;

	for (k = 0; k < MAX_DEF; k++)
	{
		nDefA[k] = 0;

		for (i = 0; i < MAX_STRIP; i++)
		{
			nDefPerStrip[i][k] = 0;
		}
	}

	for (nRow = 0; nRow < nNodeY; nRow++)
	{
		for (nCol = 0; nCol < nNodeX; nCol++)
		{
			if (pReelmap->m_pPnlBuf)
			{
				nDefCode = (int)pReelmap->m_pPnlBuf[nPnl][nRow][nCol] < 0 ? 0 : (int)pReelmap->m_pPnlBuf[nPnl][nRow][nCol];
				nDefA[nDefCode]++;

				nStrip = int(nRow / (nNodeY / MAX_STRIP));
				if (nStrip > -1 && nStrip < MAX_STRIP)
				{
					if (nDefCode > 0)
					{
						nDefStrip[nStrip]++;
						nDefPerStrip[nStrip][nDefCode]++;
					}
				}
			}
		}
	}

	for (nStrip = 0; nStrip < MAX_STRIP; nStrip++)
	{
		if (nDefStrip[nStrip] >= nStripPcs * dStOutRto)
			nStripOut[nStrip]++;
	}

	for (i = 1; i < MAX_DEF; i++)
	{
		m_stYield.nDefA[i] = m_stYield.nDefA[i] + nDefA[i];
		nDef += nDefA[i];
	}
	nGood = nTot - nDef;

	m_stYield.nTot = m_stYield.nTot + nTot;
	m_stYield.nGood = m_stYield.nGood + nGood;
	m_stYield.nDef = m_stYield.nDef + nDef;

	for (k = 0; k < MAX_STRIP; k++)
	{
		m_stYield.nDefStrip[k] = m_stYield.nDefStrip[k] + nDefStrip[k];
		m_stYield.nStripOut[k] = m_stYield.nStripOut[k] + nStripOut[k];
		nTotSriptOut += nStripOut[k];
		for (i = 1; i < MAX_DEF; i++)
			m_stYield.nDefPerStrip[k][i] = m_stYield.nDefPerStrip[k][i] + nDefPerStrip[k][i];
	}
	m_stYield.nTotSriptOut = m_stYield.nTotSriptOut + nTotSriptOut;


	FILE *fp = NULL;
	char FileName[MAX_PATH];

	BOOL bExist = FALSE;
	CFileFind findfile;
	if (findfile.FindFile(sPath))
		bExist = TRUE;
	else
		MakeDir(sPath);

	StrToChar(sPath, FileName);

	fp = fopen(FileName, "a+");
	if (fp == NULL)
	{
		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
		//AfxMessageBox(_T("It is trouble to open ReelMap.txt"),MB_ICONWARNING|MB_OK);
		return FALSE;
	}

	char* pRtn = NULL;
	if (!bExist)
	{
		m_nStartSerial = nSerial;

		fprintf(fp, "[Info]\n");
		fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "\n");

		fprintf(fp, "Process Code = \n");
		fprintf(fp, "CamPcsX = %d\n", nNodeX);
		fprintf(fp, "CamPcsY = %d\n", nNodeY);
		fprintf(fp, "\n");

		fprintf(fp, "Start Shot=%d\n", m_nStartSerial);
		fprintf(fp, "End Shot = \n\n");
		fprintf(fp, "\n");

		fprintf(fp, "Lot Start=\n");
		fprintf(fp, "Lot End=\n");
		fprintf(fp, "Entire Speed=\n");
		fprintf(fp, "\n");

		fprintf(fp, "Marked Shot=\n");
		fprintf(fp, "\n");

		fprintf(fp, "Total Shot=\n");
		fprintf(fp, "Total Pcs=\n");
		fprintf(fp, "Good Pcs=\n");
		fprintf(fp, "Bad Pcs=\n");
		fprintf(fp, "\n");

		for (i = 1; i <= MAX_DEF; i++)
			fprintf(fp, "%d=\n", i);
		fprintf(fp, "\n");

		fprintf(fp, "TotalStripOut=\n");
		fprintf(fp, "\n");

		for (k = 0; k < MAX_STRIP; k++)
		{
			fprintf(fp, "[%d Strip]\n", k);
			fprintf(fp, "StripOut=\n");
			fprintf(fp, "TotalPcs=\n");
			for (i = 1; i <= MAX_DEF; i++)
				fprintf(fp, "%d=\n", i);
			fprintf(fp, "\n");
		}

		fprintf(fp, "\n");
	}

	fclose(fp);

	strMenu.Format(_T("%d"), nSerial);

	for (i = 1; i < MAX_DEF; i++)
	{
		sCode.Format(_T("%d"), i);
		sDefNum.Format(_T("%d"), m_stYield.nDefA[i]);

		// [Info]
		::WritePrivateProfileString(_T("Info"), sCode, sDefNum, sPath);

		// [Serial]
		::WritePrivateProfileString(strMenu, sCode, sDefNum, sPath);
	}

	// [Info]
	strData.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Info"), _T("End Shot"), strData, sPath);

	strData.Format(_T("%d"), nSerial - m_nStartSerial + 1);
	::WritePrivateProfileString(_T("Info"), _T("Total Shot"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nTot);
	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nGood);
	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nDef);
	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, sPath);

	// [Serial]
	strData.Format(_T("%d"), m_stYield.nTot);
	::WritePrivateProfileString(strMenu, _T("Total Pcs"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nGood);
	::WritePrivateProfileString(strMenu, _T("Good Pcs"), strData, sPath);

	strData.Format(_T("%d"), m_stYield.nDef);
	::WritePrivateProfileString(strMenu, _T("Bad Pcs"), strData, sPath);

	for (k = 0; k < MAX_STRIP; k++)
	{
		strItem.Format(_T("Strip%d"), k);
		strData.Format(_T("%d"), m_stYield.nDefStrip[k]);
		// [Info]
		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
		// [Serial]
		::WritePrivateProfileString(strMenu, strItem, strData, sPath);

		strItem.Format(_T("StripOut_%d"), k);
		strData.Format(_T("%d"), m_stYield.nStripOut[k]);
		// [Info]
		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
		// [Serial]
		::WritePrivateProfileString(strMenu, strItem, strData, sPath);

		for (i = 1; i < MAX_DEF; i++)
		{
			strItem.Format(_T("Strip%d_%d"), k, i);
			strData.Format(_T("%d"), m_stYield.nDefPerStrip[k][i]);
			// [Info]
			::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
			// [Serial]
			::WritePrivateProfileString(strMenu, strItem, strData, sPath);
		}
	}

	strData.Format(_T("%d"), m_stYield.nTotSriptOut);
	// [Info]
	::WritePrivateProfileString(_T("Info"), _T("StripOut_Total"), strData, sPath);
	// [Serial]
	::WritePrivateProfileString(strMenu, _T("StripOut_Total"), strData, sPath);

	int dwEnd = GetTickCount();
	int dwElapsed = dwEnd - dwStart;

	return TRUE;
}

BOOL CReelYield::GetResult() // TRUE: Make Result, FALSE: Load Result or Failed.
{
	//파일을 읽어옴. ======================================================================

	// 불량관련 데이타를 읽어옴.
	int i, k;
	CString sPath;
	CString strMsg, strMenu, strItem;
	TCHAR szData[MAX_PATH];
	CFileFind findfile;

	m_stResult.nEntireStripNum = 0;

	//sPathRmap = GetRmapPath(m_nLayer);
	sPath = GetPath(m_nLayer);

	if (!findfile.FindFile(sPath)) // Can not find file.
	{
		strMsg.Format(_T("%s file open is failed"), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(strMsg);
	}
	else
	{
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("설  비  명"), NULL, szData, sizeof(szData), sPath))
			m_stResult.sMachin = CString(szData);
		else
			m_stResult.sMachin = _T("");

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("운  용  자"), NULL, szData, sizeof(szData), sPath))
			m_stResult.sOpName = CString(szData);
		else
			m_stResult.sOpName = _T("");

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("모      델"), NULL, szData, sizeof(szData), sPath))
			m_stResult.sModel = CString(szData);
		else
			m_stResult.sModel = _T("");

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("로      트"), NULL, szData, sizeof(szData), sPath))
			m_stResult.sLot = CString(szData);
		else
			m_stResult.sLot = _T("");

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("상면레이어"), NULL, szData, sizeof(szData), sPath))
			m_stResult.sLayerUp = CString(szData);
		else
			m_stResult.sLayerUp = _T("");

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("하면레이어"), NULL, szData, sizeof(szData), sPath))
			m_stResult.sLayerDn = CString(szData);
		else
			m_stResult.sLayerDn = _T("");


		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Process Code"), NULL, szData, sizeof(szData), sPath))
			m_stResult.sProcessNum = CString(szData);
		else
			m_stResult.sProcessNum = _T("");


		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Start Shot"), NULL, szData, sizeof(szData), sPath))
			m_stResult.nSerialSt = _tstoi(szData);
		else
			m_stResult.nSerialSt = 0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("End Shot"), NULL, szData, sizeof(szData), sPath))
			m_stResult.nSerialEd = _tstoi(szData);
		else
			m_stResult.nSerialEd = 0;

		//m_stResult.nSerialEd - m_stResult.nSerialSt

		m_stResult.nEntireStripNum = m_stResult.nEntireShotNum * MAX_STRIP;


		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Start"), NULL, szData, sizeof(szData), sPath))
			m_stResult.strLotStartTime = CString(szData);
		else
			m_stResult.strLotStartTime = _T("");

		//if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Run"), NULL, szData, sizeof(szData), sPathRmap))
		//	m_stResult.strLotWorkingTime = CString(szData);
		//else
		//	m_stResult.strLotWorkingTime = _T("");

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot End"), NULL, szData, sizeof(szData), sPath))
			m_stResult.strLotEndTime = CString(szData);
		else
			m_stResult.strLotEndTime = _T("");

		// 속도
		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Entire Speed"), NULL, szData, sizeof(szData), sPath))
			m_stResult.dEntireSpeed = _tstof(szData);
		else
			m_stResult.dEntireSpeed = 0.0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total Pcs"), NULL, szData, sizeof(szData), sPath))
			m_stResult.nEntirePieceNum = _tstoi(szData);
		else
			m_stResult.nEntirePieceNum = 0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Good Pcs"), NULL, szData, sizeof(szData), sPath))
			m_stResult.nGoodPieceNum = _tstoi(szData);
		else
			m_stResult.nGoodPieceNum = 0;

		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Bad Pcs"), NULL, szData, sizeof(szData), sPath))
			m_stResult.nDefectPieceNum = _tstoi(szData);
		else
			m_stResult.nDefectPieceNum = 0;


		for (i = 1; i < MAX_DEF; i++)
		{
			strItem.Format(_T("%d"), i);
			if (0 < ::GetPrivateProfileString(_T("Info"), strItem, NULL, szData, sizeof(szData), sPath))
				m_stResult.nEntireAddedDefect[i] = _tstoi(szData);  // 불량이름별 불량수를 파일에서 읽어옴.
			else
			{
				m_stResult.nEntireAddedDefect[i] = 0;
			}
		}


		m_stResult.nTotStOut = 0;
		for (k = 0; k < MAX_STRIP; k++)
		{
			strMenu.Format(_T("%d Strip"), k);
			if (0 < ::GetPrivateProfileString(strMenu, _T("TotalPcs"), NULL, szData, sizeof(szData), sPath))
				m_stResult.nDefStrip[k] = _tstoi(szData);
			else
				m_stResult.nDefStrip[k] = 0;

			if (0 < ::GetPrivateProfileString(strMenu, _T("StripOut"), NULL, szData, sizeof(szData), sPath))
				m_stResult.nStripOut[k] = _tstoi(szData);
			else
				m_stResult.nStripOut[k] = 0;

			m_stResult.nTotStOut += m_stResult.nStripOut[k];

			for (i = 1; i < MAX_DEF; i++)
			{
				strItem.Format(_T("%d"), i);
				if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
					m_stResult.nDefPerStrip[k][i] = _tstoi(szData);
				else
					m_stResult.nDefPerStrip[k][i] = 0;
			}
		}
	}

	return TRUE;
}

CString CReelYield::GetResultTxt()
{
	CString strFileData, strData;
	int nTot;
	nTot = m_stResult.nDefPerStrip[0][DEF_LIGHT] + m_stResult.nDefPerStrip[1][DEF_LIGHT] + m_stResult.nDefPerStrip[2][DEF_LIGHT] + m_stResult.nDefPerStrip[3][DEF_LIGHT];

	//리포트 작성. =====================================================================

	strFileData = _T("");
	strFileData += _T("1. 일반 정보\r\n");
	strData.Format(_T("    모 델 명 :  %s\r\n"), m_stResult.sModel);
	strFileData += strData;
	strData.Format(_T("    로 트 명 :  %s\r\n"), m_stResult.sLot);
	strFileData += strData;
	strData.Format(_T("    시작시간 :  %s\r\n"), m_stResult.strLotStartTime);
	strFileData += strData;
	strData.Format(_T("    진행시간 :  %s\r\n"), m_stResult.strLotWorkingTime);
	strFileData += strData;
	strData.Format(_T("    종료시간 :  %s\r\n"), m_stResult.strLotEndTime);
	strFileData += strData;
	strFileData += _T("\r\n");

	strFileData += _T("2. 제품 검사 결과\r\n");
	strData.Format(_T("    총 검 사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum);
	strFileData += strData;
	strData.Format(_T("    양    품 Unit 수 : %9d        양  품  율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum - m_stResult.nDefectPieceNum, 100.0*(double)(m_stResult.nEntirePieceNum - m_stResult.nDefectPieceNum) / (double)m_stResult.nEntirePieceNum);
	strFileData += strData;
	strData.Format(_T("    불    량 Unit 수 : %9d        불  량  율(%%) : %9.2f\r\n"), m_stResult.nDefectPieceNum, 100.0*(double)m_stResult.nDefectPieceNum / (double)m_stResult.nEntirePieceNum);
	strFileData += strData;
	strFileData += _T("\r\n");

	strFileData += _T("3. 열별 검사 수율\r\n");
	strData.Format(_T("    1열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
	strFileData += strData;
	strData.Format(_T("    1열 양품 Unit 수 : %9d        1열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[0], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[0]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strData.Format(_T("    1열 불량 Unit 수 : %9d        1열 불량율(%%) : %9.2f\r\n"), m_stResult.nDefStrip[0], 100.0*(double)m_stResult.nDefStrip[0] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strFileData += _T("\r\n");
	strData.Format(_T("    2열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
	strFileData += strData;
	strData.Format(_T("    2열 양품 Unit 수 : %9d        2열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[1], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[1]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strData.Format(_T("    2열 불량 Unit 수 : %9d        2열 불량율(%%) : %9.2f\r\n"), m_stResult.nDefStrip[1], 100.0*(double)m_stResult.nDefStrip[1] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strFileData += _T("\r\n");
	strData.Format(_T("    3열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
	strFileData += strData;
	strData.Format(_T("    3열 양품 Unit 수 : %9d        3열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[2], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[2]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strData.Format(_T("    3열 불량 Unit 수 : %9d        3열 불량율(%%) : %9.2f\r\n"), m_stResult.nDefStrip[2], 100.0*(double)m_stResult.nDefStrip[2] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strFileData += _T("\r\n");
	strData.Format(_T("    4열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
	strFileData += strData;
	strData.Format(_T("    4열 양품 Unit 수 : %9d        4열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[3], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[3]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strData.Format(_T("    4열 불량 Unit 수 : %9d        4열 불량율(%%) : %9.2f\r\n"), m_stResult.nDefStrip[3], 100.0*(double)m_stResult.nDefStrip[3] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
	strFileData += strData;
	strFileData += _T("\r\n");

	strFileData += _T("4. 불량 내역\r\n");
	strFileData += _T("    -----------------------------------------------------------------------------\r\n");
	strFileData += _T("    번호     불량유형       1열       2열       3열       4열       유형별 불량수\r\n");
	strFileData += _T("    -----------------------------------------------------------------------------\r\n");
	strData.Format(_T("     1         오픈  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_OPEN], m_stResult.nDefPerStrip[1][DEF_OPEN], m_stResult.nDefPerStrip[2][DEF_OPEN], m_stResult.nDefPerStrip[3][DEF_OPEN], m_stResult.nEntireAddedDefect[DEF_OPEN]);
	strFileData += strData;
	strData.Format(_T("     2         쇼트  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_SHORT], m_stResult.nDefPerStrip[1][DEF_SHORT], m_stResult.nDefPerStrip[2][DEF_SHORT], m_stResult.nDefPerStrip[3][DEF_SHORT], m_stResult.nEntireAddedDefect[DEF_SHORT]);
	strFileData += strData;
	strData.Format(_T("     3        U-쇼트 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_USHORT], m_stResult.nDefPerStrip[1][DEF_USHORT], m_stResult.nDefPerStrip[2][DEF_USHORT], m_stResult.nDefPerStrip[3][DEF_USHORT], m_stResult.nEntireAddedDefect[DEF_USHORT]);
	strFileData += strData;
	strData.Format(_T("     4         결손  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_NICK], m_stResult.nDefPerStrip[1][DEF_NICK], m_stResult.nDefPerStrip[2][DEF_NICK], m_stResult.nDefPerStrip[3][DEF_NICK], m_stResult.nEntireAddedDefect[DEF_NICK]);
	strFileData += strData;
	strData.Format(_T("     5        선간폭 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_SPACE], m_stResult.nDefPerStrip[1][DEF_SPACE], m_stResult.nDefPerStrip[2][DEF_SPACE], m_stResult.nDefPerStrip[3][DEF_SPACE], m_stResult.nEntireAddedDefect[DEF_SPACE]);
	strFileData += strData;
	strData.Format(_T("     6         잔동  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_EXTRA], m_stResult.nDefPerStrip[1][DEF_EXTRA], m_stResult.nDefPerStrip[2][DEF_EXTRA], m_stResult.nDefPerStrip[3][DEF_EXTRA], m_stResult.nEntireAddedDefect[DEF_EXTRA]);
	strFileData += strData;
	strData.Format(_T("     7         돌기  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_PROTRUSION], m_stResult.nDefPerStrip[1][DEF_PROTRUSION], m_stResult.nDefPerStrip[2][DEF_PROTRUSION], m_stResult.nDefPerStrip[3][DEF_PROTRUSION], m_stResult.nEntireAddedDefect[DEF_PROTRUSION]);
	strFileData += strData;
	strData.Format(_T("     8         핀홀  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_PINHOLE], m_stResult.nDefPerStrip[1][DEF_PINHOLE], m_stResult.nDefPerStrip[2][DEF_PINHOLE], m_stResult.nDefPerStrip[3][DEF_PINHOLE], m_stResult.nEntireAddedDefect[DEF_PINHOLE]);
	strFileData += strData;
	strData.Format(_T("     9       패드결함%10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_PAD], m_stResult.nDefPerStrip[1][DEF_PAD], m_stResult.nDefPerStrip[2][DEF_PAD], m_stResult.nDefPerStrip[3][DEF_PAD], m_stResult.nEntireAddedDefect[DEF_PAD]);
	strFileData += strData;
	strData.Format(_T("    10        홀오픈 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_OPEN], m_stResult.nDefPerStrip[1][DEF_HOLE_OPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN], m_stResult.nDefPerStrip[3][DEF_HOLE_OPEN], m_stResult.nEntireAddedDefect[DEF_HOLE_OPEN]);
	strFileData += strData;
	strData.Format(_T("    11        홀없음 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_MISS], m_stResult.nDefPerStrip[1][DEF_HOLE_MISS], m_stResult.nDefPerStrip[2][DEF_HOLE_MISS], m_stResult.nDefPerStrip[3][DEF_HOLE_MISS], m_stResult.nEntireAddedDefect[DEF_HOLE_MISS]);
	strFileData += strData;
	strData.Format(_T("    12        홀편심 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_POSITION], m_stResult.nDefPerStrip[1][DEF_HOLE_POSITION], m_stResult.nDefPerStrip[2][DEF_HOLE_POSITION], m_stResult.nDefPerStrip[3][DEF_HOLE_POSITION], m_stResult.nEntireAddedDefect[DEF_HOLE_POSITION]);
	strFileData += strData;
	strData.Format(_T("    13       홀내불량%10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_DEFECT], m_stResult.nDefPerStrip[1][DEF_HOLE_DEFECT], m_stResult.nDefPerStrip[2][DEF_HOLE_DEFECT], m_stResult.nDefPerStrip[3][DEF_HOLE_DEFECT], m_stResult.nEntireAddedDefect[DEF_HOLE_DEFECT]);
	strFileData += strData;
	strData.Format(_T("    14          POI  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_POI], m_stResult.nDefPerStrip[1][DEF_POI], m_stResult.nDefPerStrip[2][DEF_POI], m_stResult.nDefPerStrip[3][DEF_POI], m_stResult.nEntireAddedDefect[DEF_POI]);
	strFileData += strData;
	strData.Format(_T("    15        VH오픈 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_OPEN], m_stResult.nDefPerStrip[1][DEF_VH_OPEN], m_stResult.nDefPerStrip[2][DEF_VH_OPEN], m_stResult.nDefPerStrip[3][DEF_VH_OPEN], m_stResult.nEntireAddedDefect[DEF_VH_OPEN]);
	strFileData += strData;
	strData.Format(_T("    16        VH없음 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_MISS], m_stResult.nDefPerStrip[1][DEF_VH_MISS], m_stResult.nDefPerStrip[2][DEF_VH_MISS], m_stResult.nDefPerStrip[3][DEF_VH_MISS], m_stResult.nEntireAddedDefect[DEF_VH_MISS]);
	strFileData += strData;
	strData.Format(_T("    17        VH편심 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_POSITION], m_stResult.nDefPerStrip[1][DEF_VH_POSITION], m_stResult.nDefPerStrip[2][DEF_VH_POSITION], m_stResult.nDefPerStrip[3][DEF_VH_POSITION], m_stResult.nEntireAddedDefect[DEF_VH_POSITION]);
	strFileData += strData;
	strData.Format(_T("    18        VH결함 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_DEF], m_stResult.nDefPerStrip[1][DEF_VH_DEF], m_stResult.nDefPerStrip[2][DEF_VH_DEF], m_stResult.nDefPerStrip[3][DEF_VH_DEF], m_stResult.nEntireAddedDefect[DEF_VH_DEF]);
	strFileData += strData;
	strData.Format(_T("    19         노광  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_LIGHT], m_stResult.nDefPerStrip[1][DEF_LIGHT], m_stResult.nDefPerStrip[2][DEF_LIGHT], m_stResult.nDefPerStrip[3][DEF_LIGHT], nTot);
	strFileData += strData;
	strFileData += _T("    -----------------------------------------------------------------------------\r\n");
	strFileData += _T("                                                                                 \r\n");
	strFileData += _T("                                                                                 \r\n");
	strFileData += _T("                                                                                 \r\n");
	strFileData += _T("\r\n");

	return strFileData;
}

CString CReelYield::GetSapp3Txt()
{
	CString strFileData = _T("");
	CString strData;
	int nSum, nStripPcs;
	double dRateBeforeVerify, dRateAfterVerify;
	nStripPcs = m_stResult.nEntirePieceNum / MAX_STRIP;

	// 파일 이름.
	strFileData.Format(_T("FileName : %9s_%4s_%5s.txt\r\n\r\n"), m_stResult.sLot, m_stResult.sProcessNum, m_stResult.sMachin);

	// 열별 투입/완성/수율 Data.
	strFileData += _T("1Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[0], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;

	strFileData += _T("2Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[1], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;

	strFileData += _T("3Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[2], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;

	strFileData += _T("4Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[3], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;


	strFileData += _T("\r\n");


	// 열별 불량 Data.
	strFileData += _T("1X\r\n");

	if (m_stResult.nDefPerStrip[0][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[0][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_SHORT];// + m_stResult.nDefPerStrip[0][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[0][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[0][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_SPACE] + m_stResult.nDefPerStrip[0][DEF_EXTRA] + m_stResult.nDefPerStrip[0][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[0][DEF_HOLE_OPEN]);
		strFileData += strData;
	}
	nSum = m_stResult.nDefPerStrip[0][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[0][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[0][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_VH_OPEN] + m_stResult.nDefPerStrip[0][DEF_VH_MISS] + m_stResult.nDefPerStrip[0][DEF_VH_POSITION] + m_stResult.nDefPerStrip[0][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}

	strFileData += _T("2X\r\n");

	if (m_stResult.nDefPerStrip[1][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[1][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_SHORT];// + m_stResult.nDefPerStrip[2][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[1][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[1][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_SPACE] + m_stResult.nDefPerStrip[1][DEF_EXTRA] + m_stResult.nDefPerStrip[1][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN]);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[1][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[1][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_VH_OPEN] + m_stResult.nDefPerStrip[1][DEF_VH_MISS] + m_stResult.nDefPerStrip[1][DEF_VH_POSITION] + m_stResult.nDefPerStrip[1][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}


	strFileData += _T("3X\r\n");

	if (m_stResult.nDefPerStrip[2][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[2][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_SHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[2][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[2][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_SPACE] + m_stResult.nDefPerStrip[2][DEF_EXTRA] + m_stResult.nDefPerStrip[2][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN]);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[2][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[2][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_VH_OPEN] + m_stResult.nDefPerStrip[2][DEF_VH_MISS] + m_stResult.nDefPerStrip[2][DEF_VH_POSITION] + m_stResult.nDefPerStrip[2][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}


	strFileData += _T("4X\r\n");

	if (m_stResult.nDefPerStrip[3][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[3][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_SHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[3][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[3][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_SPACE] + m_stResult.nDefPerStrip[3][DEF_EXTRA] + m_stResult.nDefPerStrip[3][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[3][DEF_HOLE_OPEN]);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[3][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[3][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_VH_OPEN] + m_stResult.nDefPerStrip[3][DEF_VH_MISS] + m_stResult.nDefPerStrip[3][DEF_VH_POSITION] + m_stResult.nDefPerStrip[3][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}

	// 속도.
	strFileData += _T("\r\nS\r\n");
	strData.Format(_T("%.2f"), m_stResult.dEntireSpeed);
	strFileData += strData;
	strFileData += _T("\r\n");

	return strFileData;
}

CString CReelYield::GetSapp3TxtReverse()
{
	CString strFileData = _T("");
	CString strData;
	int nSum, nStripPcs;
	double dRateBeforeVerify, dRateAfterVerify;
	nStripPcs = m_stResult.nEntirePieceNum / MAX_STRIP;

	// 파일 이름.
	strFileData.Format(_T("FileName : %9s_%4s_%5s.txt\r\n\r\n"), m_stResult.sLot, m_stResult.sProcessNum, m_stResult.sMachin);

	// 열별 투입/완성/수율 Data.
	strFileData += _T("1Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[3], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;

	strFileData += _T("2Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[2], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;

	strFileData += _T("3Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[1], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;

	strFileData += _T("4Q\r\n");
	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[0], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
	strFileData += strData;


	strFileData += _T("\r\n");


	// 열별 불량 Data.
	strFileData += _T("1X\r\n");

	if (m_stResult.nDefPerStrip[3][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[3][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_SHORT];// + m_stResult.nDefPerStrip[0][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[3][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[3][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_SPACE] + m_stResult.nDefPerStrip[3][DEF_EXTRA] + m_stResult.nDefPerStrip[3][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[3][DEF_HOLE_OPEN]);
		strFileData += strData;
	}
	nSum = m_stResult.nDefPerStrip[3][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[3][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[3][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[3][DEF_VH_OPEN] + m_stResult.nDefPerStrip[3][DEF_VH_MISS] + m_stResult.nDefPerStrip[3][DEF_VH_POSITION] + m_stResult.nDefPerStrip[3][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}


	strFileData += _T("2X\r\n");

	if (m_stResult.nDefPerStrip[2][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[2][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_SHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[2][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[2][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_SPACE] + m_stResult.nDefPerStrip[2][DEF_EXTRA] + m_stResult.nDefPerStrip[2][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN]);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[2][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[2][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[2][DEF_VH_OPEN] + m_stResult.nDefPerStrip[2][DEF_VH_MISS] + m_stResult.nDefPerStrip[2][DEF_VH_POSITION] + m_stResult.nDefPerStrip[2][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}


	strFileData += _T("3X\r\n");

	if (m_stResult.nDefPerStrip[1][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[1][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_SHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[1][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[1][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_SPACE] + m_stResult.nDefPerStrip[1][DEF_EXTRA] + m_stResult.nDefPerStrip[1][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[1][DEF_HOLE_OPEN]);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[1][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[1][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[1][DEF_VH_OPEN] + m_stResult.nDefPerStrip[1][DEF_VH_MISS] + m_stResult.nDefPerStrip[1][DEF_VH_POSITION] + m_stResult.nDefPerStrip[1][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}


	strFileData += _T("4X\r\n");

	if (m_stResult.nDefPerStrip[0][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[0][DEF_OPEN]); // 오픈(B102)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_SHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_USHORT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
		strFileData += strData;
	}

	if (m_stResult.nDefPerStrip[0][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[0][DEF_NICK]); // 결손(B137)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_SPACE] + m_stResult.nDefPerStrip[0][DEF_EXTRA] + m_stResult.nDefPerStrip[0][DEF_PROTRUSION];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_PINHOLE];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_PAD];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
		strFileData += strData;
	}

	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[0][DEF_HOLE_OPEN]);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[0][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[0][DEF_HOLE_DEFECT];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
		strFileData += strData;
	}

	nSum = m_stResult.nDefPerStrip[0][DEF_VH_OPEN] + m_stResult.nDefPerStrip[0][DEF_VH_MISS] + m_stResult.nDefPerStrip[0][DEF_VH_POSITION] + m_stResult.nDefPerStrip[0][DEF_VH_DEF];
	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
	{
		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
		strFileData += strData;
	}

	// 속도.
	strFileData += _T("\r\nS\r\n");
	strData.Format(_T("%.2f"), m_stResult.dEntireSpeed);
	strFileData += strData;
	strFileData += _T("\r\n");

	return strFileData;
}

char* CReelYield::StrToChar(CString str) // char* returned must be deleted... 
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

void CReelYield::StrToChar(CString str, char* pCh) // char* returned must be deleted... 
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
