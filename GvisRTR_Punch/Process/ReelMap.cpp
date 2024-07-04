// ReelMap.cpp : implementation file
//

#include "../stdafx.h"
#include "../Global/GlobalFunc.h"
//#include "../Dialog/DlgProgress.h"
#include "ReelMap.h"
#include "DataFile.h"
#include "IniFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MyFile.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CReelMap

CReelMap::CReelMap(int nLayer, int nPnl, int nPcs, CWnd* pParent /*=NULL*/) : CReelYield(nLayer, nPnl, nPcs, pParent)
{
	//Yield = new CReelYield(nLayer, nPnl, nPcs, nDir, this);

	int nC, nR;

	m_pParent = pParent;

	m_dwLotSt = 0;
	m_dwLotEd = 0;

	//m_nLayer = -1;
	//nTotPnl = nPnl;
	//nTotPcs = nPcs;
	//nDir = ROT_NONE;
	//m_nBeforeSerial = 0;

	m_dTotLen = 0.0;
	m_bUseLotSep = FALSE;
	m_bContFixDef = FALSE;
	m_dAdjRatio = 1.0;
	m_dTotLen = 0.0;
	m_dPnlLen = 0.0;
	m_dLotLen = 0.0;
	m_dTempPauseLen = 0.0;
	m_dLotCutPosLen = 0.0;
	m_nSerial = 0;
	m_nLastShot = 0;
	m_nCompletedShot = 0;

	//m_nCntFixPcs = 0;
	m_nPrevSerial[0] = 0;	// --
	m_nPrevSerial[1] = 0;	// ++


	m_pPnlNum = NULL;
	m_pPnlDefNum = NULL;
	pPcsDef = NULL;
	pFrmRgn = NULL;
	pPcsRgn = NULL;
// 	pMkInfo = NULL;

	m_pPnlBuf = NULL;
	m_nPnlBuf = 0;

	if(nPnl>0 && nPcs>0)
	{
		pFrmRgn = new CRect[nPnl];
		m_pPnlNum = new int[nPnl];
		m_pPnlDefNum = new int[nPnl];
	}

	m_bThreadAliveRemakeReelmap = FALSE;
	m_sPathOnThread = _T("");
	
	m_bThreadAliveReloadReelmap = FALSE;
	m_nLastOnThread = 0;
	m_nTotalProgressReloadReelmap = 0;
	m_nProgressReloadReelmap = 0;
	m_bDoneReloadReelmap = FALSE;

	m_nSelMarkingPnl = 2;

	m_nLayer = nLayer;
	m_sPathBuf = GetRmapPath(m_nLayer);
	//m_sPathYield = GetYieldPath(m_nLayer);


	LoadConfig();
 	InitRst();
	InitPcs();
	ClrPnlNum();	
	ClrPnlDefNum();
	ClrFixPcs();

	m_bThreadAliveFinalCopyItsFiles = FALSE;

	for (nC = 0; nC < FIX_PCS_COL_MAX; nC++)
		for (nR = 0; nR < FIX_PCS_ROW_MAX; nR++)
			m_FixPcsRpt[nC][nR] = 0;
}

CReelMap::~CReelMap()
{
	int k;

	m_bThreadAliveReloadReelmap = FALSE;
	m_bThreadAliveRemakeReelmap = FALSE;
	m_bThreadAliveFinalCopyItsFiles = FALSE;

	CloseRst();

	if(m_pPnlNum)
	{
		delete[] m_pPnlNum;
		m_pPnlNum = NULL;
	}

	if(m_pPnlDefNum)
	{
		delete[] m_pPnlDefNum;
		m_pPnlDefNum = NULL;
	}

	if(pFrmRgn)
	{
		delete[] pFrmRgn;
		pFrmRgn = NULL;
	}

	if(pPcsRgn)
	{
		for(k=0; k<nTotPnl; k++)
		{
			delete[] pPcsRgn[k];
			pPcsRgn[k] = NULL;
		}
		delete[] pPcsRgn;
		pPcsRgn = NULL;
	}

	if(pPcsDef)
	{
		for(k=0; k<nTotPnl; k++)
		{
			delete[] pPcsDef[k];
			pPcsDef[k] = NULL;
		}

		delete[] pPcsDef;
		pPcsDef = NULL;
	}

	//if (Yield)
	//{
	//	delete Yield;
	//	Yield = NULL;
	//}
}


BEGIN_MESSAGE_MAP(CReelMap, CWnd)
	//{{AFX_MSG_MAP(CReelMap)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CReelMap message handlers
void CReelMap::LoadConfig()
{
}

//BOOL CReelMap::LoadDefectTableDB()
//{
//	if (pView && pView->m_pDts)
//	{
//		COLORREF rgbDef[MAX_PATH];
//		int nDefCode[MAX_PATH], nMaxR, nMaxC;
//		CString sEngN[MAX_PATH], sKorN[MAX_PATH];
//
//		return pView->m_pDts->LoadDefectTable(nDefCode, rgbDef, sKorN, sEngN, &nMaxR, &nMaxC);
//	}
//
//	return FALSE;
//}


int CReelMap::LoadPCR(int nSerial)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i, nC, nR;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	CPcsRgn* pPcsRgn = ((CManagerReelmap*)m_pParent)->m_Master[0].m_pPcsRgn;

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		//AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr)
	{
		strFileData.Format(_T("PCR[0]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		//AfxMessageBox(strFileData);
		return(2);
	}

	int nIdx = GetPcrIdx(nSerial);
	//if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
	//	nIdx = GetPcrIdx0(nSerial, TRUE);
	//else
	//	nIdx = GetPcrIdx0(nSerial);

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	//if (bFromShare)
	//	sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsShareUp, nSerial);
	//else
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
#endif

	//strcpy(FileD, sPath);
	//_tcscpy(FileD, sPath);
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
		strFileData.Format(_T("PCR[Up] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		//AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr)
		return(2);

	BOOL bResetMkInfo = FALSE;

	m_pPcr[nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[nIdx]->m_sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[nIdx]->m_sItsCode = sItsCode;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcr[nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				m_pPcr[nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			}
			else
				m_pPcr[nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);

			m_pPcr[nIdx]->m_pLayer[i] = 0; // Up

											  // BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_pDefType[i] = _tstoi(strBadName);

			pPcsRgn->GetMkMatrix(m_pPcr[nIdx]->m_pDefPcs[i], nC, nR);
			m_pPcr[nIdx]->m_arDefType[nR][nC] = m_pPcr[nIdx]->m_pDefType[i];
			m_pPcr[nIdx]->m_arPcrLineNum[nR][nC] = i;

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
}

void CReelMap::SetPnlNum(int *pPnlNum)
{
	if(!m_pPnlNum)
		return;

	int k;
	if(pPnlNum)
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlNum[k]=pPnlNum[k];
	}
	else
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlNum[k]=-1;
	}
}

void CReelMap::SetPnlDefNum(int *pPnlDefNum)
{
	if(!m_pPnlDefNum)
		return;

	int k;
	if(pPnlDefNum)
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlDefNum[k] = pPnlDefNum[k];
	}
	else
	{
		for(k=0; k<nTotPnl; k++)
			m_pPnlDefNum[k] = -1;
	}
}

void CReelMap::ClrPnlNum()
{
	for(int k=0; k<nTotPnl; k++)
		m_pPnlNum[k]=-1;
}

void CReelMap::ClrPnlDefNum()
{
	for(int k=0; k<nTotPnl; k++)
		m_pPnlDefNum[k] = -1;
}

// Panel Index k increases from 0 to (nTotPnl-1)...
void CReelMap::IncPnlNum()
{
	if(!m_pPnlNum)
		return;

	int k;
	for(k=nTotPnl-1; k>0; k--)
		m_pPnlNum[k] = m_pPnlNum[k-1];
	m_pPnlNum[0]++;
}

void CReelMap::AddPnlDefNum(int nDef)
{
	if(!m_pPnlDefNum)
		return;

	int k;
	for(k=nTotPnl-1; k>0; k--)
		m_pPnlDefNum[k] = m_pPnlDefNum[k-1];
	m_pPnlDefNum[0] = nDef;
}

void CReelMap::AddPnlNum(int nNum)
{
	if(!m_pPnlNum)
		return;

	int k;
	for(k=nTotPnl-1; k>0; k--)
		m_pPnlNum[k] = m_pPnlNum[k-1];
	m_pPnlNum[0] = nNum;
}

void CReelMap::SelMarkingPnl(int nNum)
{
	m_nSelMarkingPnl = nNum;
}

void CReelMap::SetAdjRatio(double dRatio)
{
	if (dRatio < 1.0)
		dRatio = 1.0;

	m_dAdjRatio = dRatio;
}

double CReelMap::GetAdjRatio()
{
	return m_dAdjRatio;
}

BOOL CReelMap::Open()
{
	//if (pDoc->GetTestMode() == MODE_OUTER)
	//	MakeItsReelmapHeader();

	return MakeHeader(GetRmapPath(m_nLayer));
}

BOOL CReelMap::Open(CString sPath)
{
	//if (pDoc->GetTestMode() == MODE_OUTER)
	//	MakeItsReelmapHeader();

	return MakeHeader(sPath);
}

BOOL CReelMap::Write(int nSerial)
{
	int nLayer = RMAP_NONE;

	if(nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.62"));
		return 0;
	}

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//	MakeItsReelmapHeader();

	CString sPath = GetRmapPath(m_nLayer);
	MakeHeader(sPath);

	//if (pDoc->GetTestMode() == MODE_INNER)
	//	pDoc->SetItsSerialInfo(nSerial);

	int nIdx = GetPcrIdx(nSerial);
	int nNodeX = m_pPcsRgn->nCol;
	int nNodeY = m_pPcsRgn->nRow;
	int nStripY = m_pPcsRgn->nRow / MAX_STRIP;
	int nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;

	//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	//	nLayer = m_nLayer - RMAP_UP;
	//else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	//	nLayer = m_nLayer - RMAP_INNER_UP;
	//
	//if (m_nLayer == RMAP_ITS)
	//{
	//	;
	//}
	//
	//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	//{
	//	nLayer = m_nLayer - RMAP_UP;
	//
	//	if (m_pPcr[nIdx])
	//		nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
	//}
	//else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	//{
	//	nLayer = m_nLayer - RMAP_INNER_UP;
	//
	//	if (m_pPcrInner[nLayer])
	//	{
	//		if (m_pPcrInner[nLayer][nIdx])
	//			nTotDefPcs = m_pPcrInner[nLayer][nIdx]->m_nTotDef;
	//	}
	//}
	//else if (m_nLayer == RMAP_ITS)
	//{
	//	if (m_pPcrIts)
	//	{
	//		if (m_pPcrIts[nIdx])
	//			nTotDefPcs = m_pPcrIts[nIdx]->m_nTotDef;
	//	}
	//}
	//else
	//	return 0;


	short **pPnlBuf;
	int i, nC, nR, nPcsId, nDefCode;//, nTot, nDef, nGood;
	pPnlBuf = new short*[nNodeY];
	for (i = 0; i < nNodeY; i++)
	{
		pPnlBuf[i] = new short[nNodeX];
		memset(pPnlBuf[i], 0, sizeof(short)*nNodeX);
		memset(m_pPnlBuf[nSerial - 1][i], 0, sizeof(short)*nNodeX);
	}

	CString strData, strTemp;
	int nTotVerifyed = 0;

	for (i = 0; i < nTotDefPcs; i++)
	{
		if (m_pPcr[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				switch (MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
				{
				case 0:
					nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];
					break;
				case 1:
					nPcsId = MirrorLR(m_pPcr[nIdx]->m_pDefPcs[i]);
					break;
				case 2:
					nPcsId = MirrorUD(m_pPcr[nIdx]->m_pDefPcs[i]);
					break;
				case 3:
					nPcsId = Rotate180(m_pPcr[nIdx]->m_pDefPcs[i]);
					break;
				default:
					nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];
					break;
				}
			}
			else
				nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];

			nDefCode = m_pPcr[nIdx]->m_pDefType[i];

			nC = int(nPcsId / nNodeY);
			if (nC % 2)	// 홀수.
				nR = nNodeY * (nC + 1) - nPcsId - 1;
			else		// 짝수.
				nR = nPcsId - nNodeY * nC;
			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
			if (m_pPnlBuf)
				m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
		}
		else
		{
			nTotVerifyed++;
		}
	}

	//for (i = 0; i < nTotDefPcs; i++)
	//{
	//	if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	//	{
	//		nLayer = m_nLayer - RMAP_UP;
	//
	//		if (m_pPcr[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
	//		{
	//			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
	//			{
	//				switch (MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
	//				{
	//				case 0:
	//					nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];
	//					break;
	//				case 1:
	//					nPcsId = MirrorLR(m_pPcr[nIdx]->m_pDefPcs[i]);
	//					break;
	//				case 2:
	//					nPcsId = MirrorUD(m_pPcr[nIdx]->m_pDefPcs[i]);
	//					break;
	//				case 3:
	//					nPcsId = Rotate180(m_pPcr[nIdx]->m_pDefPcs[i]);
	//					break;
	//				default:
	//					nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];
	//					break;
	//				}
	//			}
	//			else
	//				nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];
	//
	//			nDefCode = m_pPcr[nIdx]->m_pDefType[i];
	//
	//			nC = int(nPcsId / nNodeY);
	//			if (nC % 2)	// 홀수.
	//				nR = nNodeY * (nC + 1) - nPcsId - 1;
	//			else		// 짝수.
	//				nR = nPcsId - nNodeY * nC;
	//			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
	//			if (m_pPnlBuf)
	//				m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
	//		}
	//		else
	//		{
	//			nTotVerifyed++;
	//		}
	//	}
	//	else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	//	{
	//		nLayer = m_nLayer - RMAP_INNER_UP;
	//
	//		if (m_pPcrInner[nLayer][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
	//		{
	//			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
	//			{
	//				switch (pDoc->m_MasterInner[0].MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
	//				{
	//				case 0:
	//					nPcsId = m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];
	//					break;
	//				case 1:
	//					nPcsId = MirrorLR(m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
	//					break;
	//				case 2:
	//					nPcsId = MirrorUD(m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
	//					break;
	//				case 3:
	//					nPcsId = Rotate180(m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i]);
	//					break;
	//				default:
	//					nPcsId = m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];
	//					break;
	//				}
	//			}
	//			else
	//				nPcsId = m_pPcrInner[nLayer][nIdx]->m_pDefPcs[i];
	//
	//			nDefCode = m_pPcrInner[nLayer][nIdx]->m_pDefType[i];
	//
	//			nC = int(nPcsId / nNodeY);
	//			if (nC % 2)	// 홀수.
	//				nR = nNodeY * (nC + 1) - nPcsId - 1;
	//			else		// 짝수.
	//				nR = nPcsId - nNodeY * nC;
	//			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
	//			if (m_pPnlBuf)
	//				m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
	//		}
	//		else
	//		{
	//			nTotVerifyed++;
	//		}
	//	}
	//	else if (m_nLayer == RMAP_ITS)
	//	{
	//		if (m_pPcrIts[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
	//		{
	//			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
	//			{
	//				switch (MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
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
	//				m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.
	//		}
	//		else
	//		{
	//			nTotVerifyed++;
	//		}
	//	}
	//	else
	//		return 0;
	//}

	CString sPnl, sRow;
	i = 0; strData = _T("");
	::WritePrivateProfileString(sPnl, _T("Marked Date"), strData, sPath);
	sPnl.Format(_T("%d"), nSerial);
	strData.Format(_T("%d"), nTotDefPcs - nTotVerifyed);
	::WritePrivateProfileString(sPnl, _T("Total Defects"), strData, sPath);

	for (int nRow = 0; nRow < nNodeX; nRow++)			// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 첫번째 Col부터 시작해서 밑으로 내려감.
	{
		sRow.Format(_T("%02d"), nRow);
		strData.Format(_T(""));
		strTemp.Format(_T(""));

		for (int nCol = 0; nCol < nNodeY; nCol++)		// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Col : 4열 3열 2열 1열 스트립으로 표시됨.
		{
			nR = (nNodeY - 1) - nCol;					// 릴맵상의 Row
			nC = nRow;								// 릴맵상의 Col

			if (m_pPcr[nIdx]->m_nErrPnl == -1 || m_pPcr[nIdx]->m_nErrPnl == -2)
			{
				nDefCode = DEF_LIGHT;
				m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
			}
			else
				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];

			strTemp.Format(_T("%2d,"), nDefCode);	// 불량코드를 2칸으로 설정

			if (!nCol)								// strData에 처음으로 데이터를 추가
				strData.Insert(0, strTemp);
			else
			{
				int nLen = strData.GetLength();
				if (!(nCol%nStripY))				// Separate Strip (스트립 마다)
				{
					strData.Insert(nLen, _T("  "));
					nLen = strData.GetLength();
				}
				strData.Insert(nLen, strTemp);
			}
		}

		int nPos = strData.ReverseFind(',');		// 릴맵 Text 맨 우측의 ','를 삭제
		strData.Delete(nPos, 1);
		::WritePrivateProfileString(sPnl, sRow, strData, sPath); // 한 라인씩 릴맵 Text를 기록.
	}	// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 마지막 Col까지 기록하고 끝남.

	//for(int nRow=0; nRow<nNodeX; nRow++)			// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 첫번째 Col부터 시작해서 밑으로 내려감.
	//{
	//	sRow.Format(_T("%02d"), nRow);
	//	strData.Format(_T(""));
	//	strTemp.Format(_T(""));
	//
	//	for(int nCol=0; nCol<nNodeY; nCol++)		// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Col : 4열 3열 2열 1열 스트립으로 표시됨.
	//	{
	//		nR = (nNodeY-1)-nCol;					// 릴맵상의 Row
	//		nC = nRow;								// 릴맵상의 Col
	//
	//		if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	//		{
	//			nLayer = m_nLayer - RMAP_UP;
	//
	//			if (m_pPcr[nIdx]->m_nErrPnl == -1 || m_pPcr[nIdx]->m_nErrPnl == -2)
	//			{
	//				nDefCode = DEF_LIGHT;
	//				m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
	//			}
	//			else
	//				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
	//		}
	//		else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	//		{
	//			nLayer = m_nLayer - RMAP_INNER_UP;
	//
	//			if (m_pPcrInner[nLayer][nIdx]->m_nErrPnl == -1 || m_pPcrInner[nLayer][nIdx]->m_nErrPnl == -2)
	//			{
	//				nDefCode = DEF_LIGHT;
	//				m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
	//			}
	//			else
	//				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
	//		}
	//		else if (m_nLayer == RMAP_ITS)
	//		{
	//			if (m_pPcrIts[nIdx]->m_nErrPnl == -1 || m_pPcrIts[nIdx]->m_nErrPnl == -2)
	//			{
	//				nDefCode = DEF_LIGHT;
	//				m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
	//			}
	//			else
	//				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];
	//		}
	//		else
	//			return 0;
	//
	//		strTemp.Format(_T("%2d,"), nDefCode);	// 불량코드를 2칸으로 설정
	//		
	//		if(!nCol)								// strData에 처음으로 데이터를 추가
	//			strData.Insert(0, strTemp);
	//		else
	//		{
	//			int nLen = strData.GetLength();
	//			if( !(nCol%nStripY) )				// Separate Strip (스트립 마다)
	//			{
	//				strData.Insert(nLen, _T("  "));
	//				nLen = strData.GetLength();
	//			}
	//			strData.Insert(nLen, strTemp);
	//		}
	//	}
	//
	//	int nPos = strData.ReverseFind(',');		// 릴맵 Text 맨 우측의 ','를 삭제
	//	strData.Delete(nPos, 1);
	//	::WritePrivateProfileString(sPnl, sRow, strData, sPath); // 한 라인씩 릴맵 Text를 기록.
	//}	// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 마지막 Col까지 기록하고 끝남.

	for(i=0; i<nNodeY; i++)
		delete[]  pPnlBuf[i];
	delete[] pPnlBuf;

	return TRUE;
}

void CReelMap::InitPcs()
{
	int k, i;
	if(nTotPnl>0 && nTotPcs>0)
	{
		if(!pPcsRgn)
		{
			pPcsRgn = new CRect*[nTotPnl];
			for(k=0; k<nTotPnl; k++)
			{
				pPcsRgn[k] = new CRect[nTotPcs];
			}
		}

		if(!pPcsDef)
		{
			pPcsDef = new int*[nTotPnl];
			for(k=0; k<nTotPnl; k++)
			{
				if (nTotPcs > MAX_PCS)
				{
					pView->ClrDispMsg();
					AfxMessageBox(_T("MAX_PCS 초과 Error."));
				}
				pPcsDef[k] = new int[MAX_PCS];
				for(i=0; i<MAX_PCS; i++)
					pPcsDef[k][i] = DEF_NONE;
			}
		}

	}
	else
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-InitPcs."));
	}
}

void CReelMap::ClrPcs()
{
	for(int k=0; k<nTotPnl; k++)
	{
		for(int i=0; i<nTotPcs; i++)
		{
			pPcsDef[k][i] = DEF_NONE;
		}
	}
}

BOOL CReelMap::Disp(int nMkPnl, BOOL bDumy)
{
	stGeneral& General = (pView->m_mgrStatus->General);

	if(!bDumy)
	{
		if(!pPcsDef || !m_pPnlNum || !m_pPnlDefNum)
		{
			pView->MsgBox(_T("Reelmap Memory Error."));
			return FALSE;
		}
	}

	if(!m_pPcsRgn)
	{
		pView->MsgBox(_T("PCS RGN Error."));
		return FALSE;
	}

	m_sPathBuf = GetRmapPath(m_nLayer);

	CDataFile *pDataFile = new CDataFile;
	CString sMsg;
	int nRepeat = 0;

	while(!pDataFile->Open(m_sPathBuf))
	{
		Sleep(500);
		if (!pDataFile->Open(m_sPathBuf))
		{
			Sleep(500);
			if (!pDataFile->Open(m_sPathBuf))
			{
				Sleep(500);
				if (!pDataFile->Open(m_sPathBuf))
				{
					if (nRepeat > 20)
					{
						sMsg.Format(_T("릴맵파일을 읽지 못했습니다.\r\n%s\r\n릴맵파일을 다시 읽으시겠습니까?"), m_sPathBuf);
						if (IDNO == pView->MsgBox(sMsg, 0, MB_YESNO))
						{
							;
						}
					}
					else
					{
						nRepeat++;
						Sleep(500);
					}
				}
				else
					break;
			}
			else
				break;
		}
		else
			break;
	}
	
	m_nSerial = nMkPnl; // 8

	int nMkedPnl = nTotPnl-m_nSelMarkingPnl-1; // 8-2-1 = 5
	int nPrevPnl = m_nSelMarkingPnl; // 2

	int nFromPnl = nMkPnl-nMkedPnl-1; // 8-5-1 = 2

	CString sPnl, sRow, sVal;
	TCHAR sep[] = { _T(",/;\r\n\t") };
	TCHAR szData[MAX_PATH];
	int k, i, nR, nC, nP, nInc=0;
	int nLoadPnl, nDefCode;
	int nNodeX = m_pPcsRgn->nCol; // 10
	int nNodeY = m_pPcsRgn->nRow; // 5
	int nActionCode;

	for(k=nTotPnl-1; k>=0; k--) // k = 7 ~ 0
	{
		nInc++;
		nLoadPnl = nInc+nFromPnl; // 1 + 2 = 3 ~ 8 + 2 = 10

		if(pDoc->WorkingInfo.LastJob.bLotSep)
		{
			if(nLoadPnl > General.nLotEndSerial)
			{
				for(int aa=k; aa>=0; aa--)
				{
					m_pPnlNum[aa] = nLoadPnl = -1;
					m_pPnlDefNum[aa] = -1;
				}
				break;
			}
		}

		if (General.bSerialDecrese)
		{
			if (nLoadPnl < (pView->m_mgrStatus->General.nLotEndSerial) && pView->m_mgrStatus->General.nLotEndSerial > 0)
			{
				for (int bb = k; bb >= 0; bb--)
				{
					m_pPnlNum[bb] = nLoadPnl = -1;
					m_pPnlDefNum[bb] = -1;
				}
				break;
			}
			else if (pView->m_mgrStatus->General.bLastProc && (nLoadPnl < pView->m_mgrStatus->General.nLotEndSerial && pView->m_mgrStatus->General.nLotEndSerial > 0))
			{
				for (int bb = k; bb >= 0; bb--)
				{
					m_pPnlNum[bb] = nLoadPnl = -1;
					m_pPnlDefNum[bb] = -1;
				}
				break;
			}
			else if (nLoadPnl < pView->m_mgrStatus->General.nLotEndSerial && pView->m_mgrStatus->General.nLotEndSerial > 0)
			{
				for (int cc = k; cc >= 0; cc--)
				{
					m_pPnlDefNum[cc] = -1;
				}
			}
		}
		else
		{
			if (nLoadPnl > pView->m_mgrStatus->General.nLotEndSerial && pView->m_mgrStatus->General.nLotEndSerial > 0)
			{
				for (int bb = k; bb >= 0; bb--)
				{
					m_pPnlNum[bb] = nLoadPnl = -1;
					m_pPnlDefNum[bb] = -1;
				}
				break;
			}
			else if (pView->m_mgrStatus->General.bLastProc && (nLoadPnl > pView->m_mgrStatus->General.nLotEndSerial && pView->m_mgrStatus->General.nLotEndSerial > 0))
			{
				for (int bb = k; bb >= 0; bb--)
				{
					m_pPnlNum[bb] = nLoadPnl = -1;
					m_pPnlDefNum[bb] = -1;
				}
				break;
			}
			else if (nLoadPnl > pView->m_mgrStatus->General.nLotEndSerial && pView->m_mgrStatus->General.nLotEndSerial > 0)
			{
				for (int cc = k; cc >= 0; cc--)
				{
					m_pPnlDefNum[cc] = -1;
				}
			}
		}


		if(nLoadPnl <= 0)
		{
			m_pPnlNum[k] = -1;
			m_pPnlDefNum[k] = -1;
			for(i=0; i<nTotPcs; i++)
				pPcsDef[k][i] = DEF_NONE;
		}
		else
		{
			if (pView->m_mgrStatus->General.bSerialDecrese)
			{
				if (pDoc->WorkingInfo.LastJob.bLotSep && nLoadPnl < pView->m_mgrStatus->General.nLotEndSerial)
				{
					m_pPnlNum[k] = 0;
					m_pPnlDefNum[k] = -1;
					break;
				}
				else if (nLoadPnl < pView->m_mgrStatus->ListBuf[0].GetLast()) 
				{
					m_pPnlNum[k] = 0;
					m_pPnlDefNum[k] = -1;
					break;
				}

				m_pPnlNum[k] = nLoadPnl; // 3 ~ 10
				if (nLoadPnl >= pView->m_mgrStatus->General.nLotEndSerial || pView->m_mgrStatus->General.nLotEndSerial == 0)
				{
					m_pPnlDefNum[k] = m_pPcr[GetPcrIdx(nLoadPnl)]->m_nTotDef;
					//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
					//{
					//	m_pPnlDefNum[k] = m_pPcr[GetPcrIdx(nLoadPnl)]->m_nTotDef;
					//}
				}
			}
			else
			{
				if (pDoc->WorkingInfo.LastJob.bLotSep && nLoadPnl > pView->m_mgrStatus->General.nLotEndSerial)
				{
					m_pPnlNum[k] = 0;
					m_pPnlDefNum[k] = -1;
					break;
				}
				else if (nLoadPnl > pView->m_mgrStatus->ListBuf[0].GetLast())
				{
					m_pPnlNum[k] = 0;
					m_pPnlDefNum[k] = -1;
					break;
				}

				m_pPnlNum[k] = nLoadPnl; // 3 ~ 10
				if (nLoadPnl <= pView->m_mgrStatus->General.nLotEndSerial || pView->m_mgrStatus->General.nLotEndSerial == 0)
				{
					m_pPnlDefNum[k] = m_pPcr[GetPcrIdx(nLoadPnl)]->m_nTotDef;
					//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
					//{
					//	m_pPnlDefNum[k] = m_pPcr[GetPcrIdx(nLoadPnl)]->m_nTotDef;
					//}
				}
			}

			for(nR=0; nR<nNodeX; nR++)  // nR = 0 ~ 5
			{
				sPnl.Format(_T("%d"), nLoadPnl);
				sRow.Format(_T("%02d"), nR);
				
				if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), m_sPathBuf)) // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일
				{
					for(nC=0; nC<nNodeY; nC++)
					{
						if(nC==0)
							sVal = _tcstok(szData,sep);
						else
							sVal = _tcstok(NULL,sep);
						
						if(nR%2)	// 홀수.
							nP = nC + nNodeY * nR;
						else		// 짝수.				
							nP = nNodeY * (nR+1) - (nC+1);

						if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
						{
							nActionCode = MasterInfo.nActionCode;
							//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
							//{
							//	nActionCode = MasterInfo.nActionCode;
							//}

							switch (nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
							{
							case 0:
								break;
							case 1:
								nP = MirrorLR(nP);
								break;
							case 2:
								nP = MirrorUD(nP);
								break;
							case 3:
								nP = Rotate180(nP);
								break;
							default:
								break;
							}
						}

						nDefCode = _tstoi(sVal);
						pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가

					}
				}
				else
				{
					Sleep(300);

					if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), m_sPathBuf))
					{
						for (nC = 0; nC < nNodeY; nC++)
						{
							if (nC == 0)
								sVal = _tcstok(szData, sep);
							else
								sVal = _tcstok(NULL, sep);

							if (nR % 2)	// 홀수.
								nP = nC + nNodeY * nR;
							else		// 짝수.				
								nP = nNodeY * (nR + 1) - (nC + 1);

							if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
							{
								nActionCode = MasterInfo.nActionCode;
								//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
								//{
								//	nActionCode = MasterInfo.nActionCode;
								//}

								switch (nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
								{
								case 0:
									break;
								case 1:
									nP = MirrorLR(nP);
									break;
								case 2:
									nP = MirrorUD(nP);
									break;
								case 3:
									nP = Rotate180(nP);
									break;
								default:
									break;
								}
							}

							nDefCode = _tstoi(sVal);
							pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가
						}
					}
					else
					{
						if (!bDumy)
						{
							if (!pView->m_mgrStatus->General.bLastProc && !pDoc->WorkingInfo.LastJob.bSampleTest)
							{
								{
									m_pPnlNum[k] = -1;
									m_pPnlDefNum[k] = -1;
									for (i = 0; i < nTotPcs; i++)
										pPcsDef[k][i] = DEF_NONE;
								}
							}
							else
							{
								m_pPnlNum[k] = 0;
								m_pPnlDefNum[k] = -1;

								for (nC = 0; nC < nNodeY; nC++)
								{
									if (nR % 2)	// 홀수.
										nP = nC + nNodeY * nR;
									else		// 짝수.				
										nP = nNodeY * (nR + 1) - (nC + 1);

									if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
									{
										nActionCode = MasterInfo.nActionCode;
										//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
										//{
										//	nActionCode = MasterInfo.nActionCode;
										//}

										switch (nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
										{
										case 0:
											break;
										case 1:
											nP = MirrorLR(nP);
											break;
										case 2:
											nP = MirrorUD(nP);
											break;
										case 3:
											nP = Rotate180(nP);
											break;
										default:
											break;
										}
									}

									nDefCode = DEF_NONE;
									pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가
								}
							}
						}
						else
						{
							for (nC = 0; nC < nNodeY; nC++)
							{
								if (nC == 0)
									sVal = _tcstok(szData, sep);
								else
									sVal = _tcstok(NULL, sep);

								if (nR % 2)	// 홀수.
									nP = nC + nNodeY * nR;
								else		// 짝수.
									nP = nNodeY * (nR + 1) - (nC + 1);

								if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
								{
									nActionCode = MasterInfo.nActionCode;
									//if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
									//{
									//	nActionCode = MasterInfo.nActionCode;
									//}

									switch (nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
									{
									case 0:
										break;
									case 1:
										nP = MirrorLR(nP);
										break;
									case 2:
										nP = MirrorUD(nP);
										break;
									case 3:
										nP = Rotate180(nP);
										break;
									default:
										break;
									}
								}

								nDefCode = 0;
								pPcsDef[k][nP] = nDefCode; // k=7, nP = PcsIdx : 좌상단에서 nP 0이 시작하여 ZigZeg로 우하단으로 증가
							}
						}
					}
				}
			}
		}
	}

	if(pDataFile)
	{
		delete pDataFile;
		pDataFile = NULL;
	}

	return TRUE;
}

void CReelMap::SetLotSt()
{
	CString sData;
	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.StTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.StTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.StTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.StTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.StTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.StTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(_T("Info"), _T("Lot Start"), sData, m_sPathBuf);
}

CString CReelMap::GetLotSt()
{
	CString sLot = _T("");
	TCHAR szData[MAX_PATH];
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Start"), NULL, szData, sizeof(szData), m_sPathBuf))
		sLot = CString(szData);
	return sLot;
}

void CReelMap::SetLotEd()
{
	CString sData;
	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.EdTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.EdTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.EdTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.EdTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.EdTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.EdTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(_T("Info"), _T("Lot End"), sData, m_sPathBuf);

	if((m_dwLotEd - m_dwLotSt) > 0)
	{
		int nDiff = (m_dwLotEd - m_dwLotSt)/1000;
		nHour = int(nDiff/3600);
		nMin = int((nDiff-3600*nHour)/60);
		nSec = nDiff % 60;
		sData.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
		::WritePrivateProfileString(_T("Info"), _T("Lot Run"), sData, m_sPathBuf);
	}
}

CString CReelMap::GetLotEd()
{
	CString sLot = _T("");
	TCHAR szData[MAX_PATH];
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot End"), NULL, szData, sizeof(szData), m_sPathBuf))
		sLot = CString(szData);
	return sLot;
}


int CReelMap::GetLastSerial() 	// 마킹이 끝난 후 피딩한 시리얼.
{
	int nSerial;
	TCHAR szData[200];

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), GetRmapPath(m_nLayer)))
		nSerial = _ttoi(szData);
	else
	{
		nSerial = 0;
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error : GetLastSerial()"));
	}

	return nSerial;
}

void CReelMap::SetLastSerial(int nSerial) 	// 마킹이 끝난 후 피딩한 시리얼.
{
	if(nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.64"));
		return;
	}

	m_nLastShot = nSerial;
	m_sPathBuf = GetRmapPath(m_nLayer);

	CString sSerial, sData;
	sSerial.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Info"), _T("Marked Shot"), sSerial, m_sPathBuf);

	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.CurTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.CurTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.CurTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.CurTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.CurTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.CurTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(sSerial, _T("Marked Date"), sData, m_sPathBuf);
}

BOOL CReelMap::InitRst()
{
	int nPnl, nRow;
	int TotPnl = PNL_TOT;
	int nDumyPnl = 20;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	int k, i;

 	CloseRst();

	m_nPnlBuf = TotPnl+nDumyPnl;
	if(!m_pPnlBuf)
	{
		m_pPnlBuf = new short**[m_nPnlBuf];
		for(nPnl=0; nPnl<m_nPnlBuf; nPnl++)
		{
			m_pPnlBuf[nPnl] = new short*[nNodeY];
			for(nRow=0; nRow<nNodeY; nRow++)
				m_pPnlBuf[nPnl][nRow] = new short[nNodeX];
		}
	}

	m_nTotPcs = 0;
	m_nGoodPcs = 0;
	m_nBadPcs = 0;
	for(i=0; i<MAX_DEF; i++)
		m_nDef[i] = 0;

	m_nTotStOut = 0;
	for(k=0; k<MAX_STRIP; k++)
	{
		m_nStripOut[k] = 0;
		m_nDefStrip[k] = 0;
		for(i=0; i<MAX_DEF; i++)
			m_nDefPerStrip[k][i] = 0;
	}

	return TRUE;
}

void CReelMap::CloseRst()
{
	int nPnl, nRow;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	if(m_pPnlBuf)
	{
		for(nPnl=0; nPnl<m_nPnlBuf; nPnl++)
		{
			for(nRow=0; nRow<nNodeY; nRow++)
				delete[] m_pPnlBuf[nPnl][nRow];
			delete[] m_pPnlBuf[nPnl];
		}

		delete[] m_pPnlBuf;
		m_pPnlBuf = NULL;
		m_nPnlBuf = 0;
	}
}

void CReelMap::ClrRst()
{
	m_nTotPcs = 0;
	m_nGoodPcs = 0;
	m_nBadPcs = 0;

	int k, i;
	for(i=1; i<MAX_DEF; i++)
		m_nDef[i] = 0;

	m_nTotStOut = 0;
	for(k=0; k<MAX_STRIP; k++)
	{
		m_nStripOut[k] = 0;
		m_nDefStrip[k] = 0;
		for(i=1; i<MAX_DEF; i++)
			m_nDefPerStrip[k][i] = 0;
	}

	int nPnl, nRow, nCol;
	int TotPnl = PNL_TOT;
	int nNodeX = PNLBUF_X;
	int nNodeY = PNLBUF_Y;

	if(m_pPnlBuf)
	{
		for(nPnl=0; nPnl<TotPnl; nPnl++)
		{
			for(nRow=0; nRow<nNodeY; nRow++)
			{
				for(nCol=0; nCol<nNodeX; nCol++)
					m_pPnlBuf[nPnl][nRow][nCol] = 0;
			}
		}
	}
}

//int CReelMap::GetDefNum(int nDefCode)
//{
//	return m_stYield.nDefA[nDefCode];
//}
//
//int CReelMap::GetDefStrip(int nStrip)
//{
//	return m_stYield.nDefStrip[nStrip];
//}
//
//int CReelMap::GetDefStrip(int nStrip, int nDefCode)
//{
//	return m_stYield.nDefPerStrip[nStrip][nDefCode];
//}
//
//int CReelMap::GetStripOut(int nStrip)
//{
//	return m_stYield.nStripOut[nStrip];
//}
//
//void CReelMap::GetPcsNum(int &nGood, int &nBad)
//{
//	nGood = m_stYield.nGood;
//	nBad = m_stYield.nDef;
//}

void CReelMap::Clear()
{
	m_nSerial = 0;
	m_nLastShot = 0;
	ClrFixPcs();
	ClrRst();
	ClrPcs();
	ClrPnlNum();
	ClrPnlDefNum();
}

void CReelMap::SetFixPcs(int nSerial)
{
	int nNodeX = m_pPcsRgn->nCol;
	int nNodeY = m_pPcsRgn->nRow;
	int nR, nC, nDefCode;

	for(nR=0; nR<nNodeY; nR++)
	{
		for(nC=0; nC<nNodeX; nC++)
		{
			nDefCode = m_pPnlBuf[nSerial-1][nR][nC]; // DefCode
			if(nDefCode > 0)
				SetFixPcs(nSerial, nC, nR);
		}
	}
}

void CReelMap::SetFixPcs(int nSerial, int nCol, int nRow) // nShot : 0 ~ 
{
	if(nSerial < 1)
		return;

	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nS = (nSerial-1) % nRange;
	m_FixPcs[nS][nCol][nRow] = TRUE;
}

void CReelMap::ClrFixPcs()
{
	m_nPrevSerial[0] = 0;	// --
	m_nPrevSerial[1] = 0;	// ++

	for(int nS=0; nS<FIX_PCS_SHOT_MAX; nS++)
	{
		if(!nS)
		{
			m_nPrevSerial[0] = 0; // [0] : --
			m_nPrevSerial[1] = 0; // [1] : ++
		}

		for(int nC=0; nC<FIX_PCS_COL_MAX; nC++)
		{
			for(int nR=0; nR<FIX_PCS_ROW_MAX; nR++)
			{
				m_FixPcs[nS][nC][nR] = FALSE;
				if(!nS)
				{
					m_FixPcsPrev[nC][nR] = 0;
					m_FixPcsPrevStSerial[nC][nR] = 0;
				}		
			}
		}
	}
}

void CReelMap::ClrFixPcs(int nCol, int nRow)
{
	for(int nS=0; nS<FIX_PCS_SHOT_MAX; nS++)
	{
		m_FixPcs[nS][nCol][nRow] = FALSE;
	}
	m_FixPcsPrev[nCol][nRow] = 0;
	m_FixPcsPrevStSerial[nCol][nRow] = 0;
}

BOOL CReelMap::IsFixPcs(int nSerial, int &Col, int &Row)
{
	BOOL bRtn = FALSE;
	int nNodeX = m_pPcsRgn->nCol;
	int nNodeY = m_pPcsRgn->nRow;

	BOOL bOn = pDoc->WorkingInfo.LastJob.bContFixDef;
	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nJudge = _tstoi(pDoc->WorkingInfo.LastJob.sNumContFixDef);

	if(m_nPrevSerial[0] == nSerial)
		return bRtn;
	m_nPrevSerial[0] = nSerial;

	int nS;

	for(int nC=0; nC<nNodeX; nC++)
	{
		for(int nR=0; nR<nNodeY; nR++)
		{
			if(nSerial >= nRange)
			{
				nS = (nSerial-1) % nRange;
				if(nS == nRange-1)
					nS = 0;
				else
					nS++;

				if(m_FixPcs[nS][nC][nR]) // Defect PCS
				{
					if(m_FixPcsPrev[nC][nR] > 0)
						m_FixPcsPrev[nC][nR]--;
				}
				m_FixPcs[nS][nC][nR] = FALSE;
			}

			nS = (nSerial-1) % nRange;
			if(m_FixPcs[nS][nC][nR]) // Defect PCS
			{
				m_FixPcsPrev[nC][nR]++;
			}

			if(bOn)
			{
				if(m_FixPcsPrev[nC][nR] >= nJudge)
				{
					m_FixPcsRpt[nC][nR]++;
					Col = nC;
					Row = nR;
					ClrFixPcs(nC, nR);
					bRtn = TRUE;
				}				
			}
		}
	}

	return bRtn;
}

BOOL CReelMap::IsFixPcs(int nSerial, int* pCol, int* pRow, int &nTot, BOOL &bCont) // nTot : total of PCS Over nJudge
{
	BOOL bRtn = FALSE;
	int nNodeX = m_pPcsRgn->nCol;
	int nNodeY = m_pPcsRgn->nRow;

	BOOL bOn = pDoc->WorkingInfo.LastJob.bContFixDef;
	int nRange = _tstoi(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	int nJudge = _tstoi(pDoc->WorkingInfo.LastJob.sNumContFixDef);

	if(m_nPrevSerial[0] == nSerial)
		return bRtn;
	m_nPrevSerial[0] = nSerial;

	int nS = (nSerial-1) % nRange;
	nTot = 0;

	for(int nC=0; nC<nNodeX; nC++)
	{
		for(int nR=0; nR<nNodeY; nR++)
		{
			if(nSerial >= nRange)
			{
				nS = (nSerial-1) % nRange;
				if(nS == nRange-1)
					nS = 0;
				else
					nS++;

				if(m_FixPcs[nS][nC][nR]) // Defect PCS
				{
					if(m_FixPcsPrev[nC][nR] > 0)
						m_FixPcsPrev[nC][nR]--;
				}
				m_FixPcs[nS][nC][nR] = FALSE;
			}

			nS = (nSerial-1) % nRange;
			if(m_FixPcs[nS][nC][nR]) // Defect PCS
			{
				if (!m_FixPcsPrev[nC][nR])
					m_FixPcsPrevStSerial[nC][nR] = nSerial;
				m_FixPcsPrev[nC][nR]++;
			}

			if(bOn)
			{
				if(m_FixPcsPrev[nC][nR] >= nJudge)
				{
					if (nSerial - m_FixPcsPrevStSerial[nC][nR] == nJudge - 1)
						bCont = TRUE;
					m_FixPcsRpt[nC][nR]++;
					pCol[nTot] = nC;
					pRow[nTot] = nR;
					nTot++;
					ClrFixPcs(nC, nR);
					bRtn = TRUE;
				}				
			}
		}
	}

	return bRtn;
}

BOOL CReelMap::MakeDirRmap()
{
	CString str, sPath, Path[5];

	switch (m_nLayer)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	//case RMAP_INNER_UP:
	//	str = _T("YieldUp.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
	//	break;
	//case RMAP_INNER_DN:
	//	str = _T("YieldDn.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
	//	break;
	//case RMAP_INNER_ALLUP:
	//	str = _T("YieldAll.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
	//	break;
	//case RMAP_INNER_ALLDN:
	//	str = _T("YieldAll.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
	//	Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
	//	break;
	//case RMAP_INOUTER_UP:
	//	break;
	//case RMAP_INOUTER_DN:
	//	break;
	//case RMAP_ITS:
	//	str = _T("YieldIts.txt");
	//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	//	Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
	//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
	//	Path[3] = _T("");
	//	Path[4] = _T("");
	//	break;
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

CString CReelMap::MakeDirRmapRestore()
{
	CString str, sPath, Path[5];

	switch (m_nLayer)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
		Path[4] = _T("");
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
		Path[4] = _T("");
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
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
		str = _T("ReelMapDataIts.txt");
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

					sPath.Format(_T("%s%s\\%s\\%s\\%s\\Restore"), Path[0], Path[1], Path[2], Path[3], Path[4]);
					if (!DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
				else
				{
					sPath.Format(_T("%s%s\\%s\\%s\\Restore"), Path[0], Path[1], Path[2], Path[3]);
					if (!DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
			}
			else
			{
				sPath.Format(_T("%s%s\\%s\\Restore"), Path[0], Path[1], Path[2]);
				if (!DirectoryExists(sPath))
					CreateDirectory(sPath, NULL);
			}
		}
		else
		{
			sPath.Format(_T("%s%s\\Restore"), Path[0], Path[1]);
			if (!DirectoryExists(sPath))
				CreateDirectory(sPath, NULL);
		}
	}
	else
	{
		pView->MsgBox(_T("Error- Path[0].IsEmpty() || Path[1].IsEmpty()"));
		sPath = _T("");
	}

	return sPath;
}

void CReelMap::SetPathAtBuf()
{
	m_sPathBuf = GetRmapPath(m_nLayer);
}

CString CReelMap::GetRmapPath(int nRmap)
{
	CString sPath = _T("");
	CString Path[5], str;

	switch (nRmap)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INOUTER_UP:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_INOUTER_DN:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ITS:
		//pDoc->GetCurrentInfoEng();
		str = _T("ReelMapDataIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sEngItsCode,	//pDoc->m_sItsCode,
			str);
		break;
	}

	return sPath;
}

void CReelMap::UpdateProcessNum(CString sProcessNum, int nLayer)
{
	::WritePrivateProfileString(_T("Info"), _T("Process Code"), sProcessNum, GetRmapPath(nLayer));
}


void CReelMap::StartThreadRemakeReelmap() 
{
	m_bRtnThreadRemakeReelmap = FALSE;
	m_bThreadAliveRemakeReelmap = TRUE;	
	m_ThreadTaskRemakeReelmap.Start( GetSafeHwnd(), this, ThreadProcRemakeReelmap);// Start the thread
}

void CReelMap::StopThreadRemakeReelmap() 
{
	m_ThreadTaskRemakeReelmap.Stop();// Stop the thread
}

// Home thread body
BOOL CReelMap::ThreadProcRemakeReelmap( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CReelMap* pThread = reinterpret_cast< CReelMap* >( lpContext );
//	DWORD dwTimePeriod = 10; // 10ms sec sleep

	pThread->m_bRtnThreadRemakeReelmap = TRUE;
	pThread->m_bThreadAliveRemakeReelmap = TRUE;	
	
	//pThread->m_cs.Lock();
	pThread->m_bRtnThreadRemakeReelmap = pThread->RemakeReelmap();
	//pThread->m_cs.Unlock();

	pThread->m_bThreadAliveRemakeReelmap = FALSE;

	return (pThread->m_bRtnThreadRemakeReelmap);
}


BOOL CReelMap::GetNodeXYonRmap(int &nNodeX, int &nNodeY, CString sPath)
{
	TCHAR szData[MAX_PATH];
	CString sPnl, sRow, sLine;
	int nX, nY, nSt = 0;

	nNodeX = 0;
	nNodeY = 0;

	for (nY = 0; nY<10000; nY++)
	{
		sPnl = _T("1");
		sRow.Format(_T("%02d"), nY);
		if (0 >= ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), sPath))
		{
			nNodeX = nY;
			break;
		}
		else
		{
			sLine = CString(szData);
			for (nX = 0; nX<100000; nX++)
			{
				nSt = sLine.Find(',', nSt + 1);
				if (nSt < 0)
					break;
				else
					nNodeY++;
			}
			nNodeY++;
		}
	}

	if (!nNodeX || !nNodeY)
	{
		nNodeX = m_pPcsRgn->nCol; // on Cam
		nNodeY = m_pPcsRgn->nRow; // on Cam
	}

	return TRUE;
}

void CReelMap::RestoreReelmap()
{
	CString sPath = MakeDirRmapRestore();
	int nLastDir = GetLastRmapRestoreDir(sPath);

	CString sUserRestoreDir;
	sUserRestoreDir.Format(_T("%s\\%d"), sPath, nLastDir+1);
	
	CFileFind finder;
	if (!DirectoryExists(sUserRestoreDir))
		CreateDirectory(sUserRestoreDir, NULL);


	CString Path[5], str, sSrc, sDest;

	switch (m_nLayer)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INOUTER_UP:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_INOUTER_DN:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ITS:
		//pDoc->GetCurrentInfoEng();
		str = _T("ReelMapDataIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,	//pDoc->m_sItsCode,
			str);
		break;
	}

	sSrc = sPath;
	sDest.Format(_T("%s\\%s"), sUserRestoreDir, str);

	CMyFile *pFile;
	pFile = new CMyFile();

	BOOL bRst = pFile->Copy(sSrc, sDest);			// 파일을 복사한다.

	delete pFile;
}

int CReelMap::GetLastRmapRestoreDir(CString strPath)
{
	CString strFileName;

	CFileFind cFile;
	BOOL bExist = cFile.FindFile(strPath);
	if(!bExist)
	{
		return 0; // 존재하지 않음.
	}

	int nFirstFolder = 1000000;
	int nLastFolder = 0;

	strPath += _T("\\*.*");
	bExist = cFile.FindFile(strPath);

	while(bExist)
	{
		bExist = cFile.FindNextFile();
		if(cFile.IsDots()) continue;
		if(cFile.IsDirectory())
		{
			// 폴더명을 얻음.
			strFileName = cFile.GetFileName();

			int nFileNum = _tstoi(strFileName);
			if(nFileNum < nFirstFolder)
				nFirstFolder = nFileNum;
			if(nFileNum > nLastFolder)
				nLastFolder = nFileNum;
		}
	}

	return nLastFolder;
}




void CReelMap::StartThreadReloadReelmap()
{
	m_bRtnThreadReloadReelmap = FALSE;
	m_bThreadAliveReloadReelmap = TRUE;
	m_ThreadTaskReloadReelmap.Start( GetSafeHwnd(),this,ThreadProcReloadReelmap);// Start the thread
}

void CReelMap::StopThreadReloadReelmap()
{
	m_ThreadTaskReloadReelmap.Stop();// Stop the thread
}

// Home thread body
BOOL CReelMap::ThreadProcReloadReelmap( LPVOID lpContext )
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CReelMap* pThread = reinterpret_cast< CReelMap* >( lpContext );
//	DWORD dwTimePeriod = 10; // 10ms sec sleep

	pThread->m_bRtnThreadReloadReelmap = FALSE;
	pThread->m_bThreadAliveReloadReelmap = TRUE;	
	
	//pThread->m_cs.Lock();
	//int nSerial = pThread->GetLastSerial();	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.
	pThread->m_bRtnThreadReloadReelmap = pThread->ReloadReelmap(pThread->m_nLastOnThread);
	//pThread->m_cs.Unlock();

	pThread->m_bThreadAliveReloadReelmap = FALSE;

	return (pThread->m_bRtnThreadReloadReelmap);
}


BOOL CReelMap::IsDoneReloadReelmap()
{
	return m_bDoneReloadReelmap;
}

int CReelMap::GetProgressReloadReelmap()
{
	if(m_nTotalProgressReloadReelmap <= 0)
		return 0;

	double dA = (double)m_nProgressReloadReelmap;
	double dB = (double)m_nTotalProgressReloadReelmap;
	double dC = 100.0 * dA / dB;
	int nC = int(dC);

	return nC;
}

BOOL CReelMap::ReloadReelmap()
{
	BOOL bRtn;
	
	int nLastOnThread = GetLastSerial();	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.
	m_nLastOnThread = nLastOnThread;	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.

	//if(nSerial > 0)
	if(m_nLastOnThread > 0)
	{
		//bRtn = ReloadReelmap(nSerial);
		m_bDoneReloadReelmap = FALSE;
		m_nProgressReloadReelmap = 0;
		m_nTotalProgressReloadReelmap = 0;
		StartThreadReloadReelmap();
		bRtn = TRUE;
	}
	else 
		bRtn = FALSE;

	return bRtn;
}

BOOL CReelMap::ReloadReelmap(int nTo)
{
	if(!m_pPnlBuf)
	{
		pView->ClrDispMsg(); 
		AfxMessageBox(_T("Memory is not allocated.- PnlBuf"));
		m_bDoneReloadReelmap = TRUE;
		return FALSE;
	}

	int nPnl, nRow, nCol, nDefCode, nStrip, nC, nR;
	CString sPnl, sRow, sVal;
	TCHAR sep[] = { _T(",/;\r\n\t") };
	TCHAR szData[MAX_PATH];

	int nNodeX = m_pPcsRgn->nCol;
	int nNodeY = m_pPcsRgn->nRow;
	int nTotPcs = nNodeX * nNodeY;
	int nStripPcs = nTotPcs / 4;

	int nDefStrip[4];
	

//  InitRst();
	ClrRst();
	m_nTotPcs = 0;
	m_nGoodPcs = 0;
	m_nBadPcs = 0;
	
	m_nTotalProgressReloadReelmap = nTo*(nNodeX*nNodeY+4)+4*MAX_DEF+MAX_DEF;
	m_nProgressReloadReelmap=0;

	for(nPnl=0; nPnl<nTo; nPnl++)
	{
		if (!m_bThreadAliveReloadReelmap)
		{
			pView->ClrDispMsg(); 
			AfxMessageBox(_T("Thread is stopped on reloading reelmap.- PnlBuf"));
			m_bDoneReloadReelmap = TRUE;
			return FALSE;
		}

		nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;

		for(nRow=0; nRow<nNodeX; nRow++)
		{
			sPnl.Format(_T("%d"), nPnl+1);
			sRow.Format(_T("%02d"), nRow);
			if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), m_sPathBuf))
			{
				for(nCol=0; nCol<nNodeY; nCol++)
				{
					m_nProgressReloadReelmap++;

					if(nCol==0)
						sVal = _tcstok(szData,sep);
					else
						sVal = _tcstok(NULL,sep);//strtok

					nDefCode = _tstoi(sVal);

					nR = (nNodeY-1)-nCol;
					nC = nRow;

					m_pPnlBuf[nPnl][nR][nC] = (short)nDefCode;
					m_nDef[nDefCode]++;

					nStrip = int(nR / (nNodeY/4));
					if(nStrip > -1 && nStrip < 4)
					{
						if(nDefCode > 0)
						{
							nDefStrip[nStrip]++;
							m_nDefStrip[nStrip]++;
							m_nDefPerStrip[nStrip][nDefCode]++;
						}
					}

					m_nTotPcs++;
					if(nDefCode > 0)
						m_nBadPcs++;
					else
						m_nGoodPcs++;
				}
			}
		}

		double dStOutRto = _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0; //atof
		for(nStrip=0; nStrip<4; nStrip++)
		{
			m_nProgressReloadReelmap++;

			if(nDefStrip[nStrip] >= nStripPcs * dStOutRto)
				m_nStripOut[nStrip]++;
		}
	}


	// 수율 데이타를 갱신함.

	// Piece infomation..............
	CString strData, strMenu, strItem;
	int k, i;

	strData.Format(_T("%d"), m_nTotPcs);
	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, m_sPathBuf);

	strData.Format(_T("%d"), m_nGoodPcs);
	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, m_sPathBuf);

	strData.Format(_T("%d"), m_nBadPcs);
	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, m_sPathBuf);

	int nTotStOut = 0;
	for(k=0; k<4; k++)
	{
		strMenu.Format(_T("Strip%d"), k);
		strData.Format(_T("%d"), m_nDefStrip[k]);
		::WritePrivateProfileString(_T("Info"), strMenu, strData, m_sPathBuf);
		
		strMenu.Format(_T("%d"), k);
		strData.Format(_T("%d"), m_nStripOut[k]);
		::WritePrivateProfileString(_T("StripOut"), strMenu, strData, m_sPathBuf);
		nTotStOut += m_nStripOut[k];

		for(i=1; i<MAX_DEF; i++)
		{
			m_nProgressReloadReelmap++;

			strItem.Format(_T("Strip%d"), k);
			strMenu.Format(_T("%d"), i);
			strData.Format(_T("%d"), m_nDefPerStrip[k][i]);
			::WritePrivateProfileString(strItem, strMenu, strData, m_sPathBuf);
		}			
	}
	strData.Format(_T("%d"), m_nTotStOut=nTotStOut);
	::WritePrivateProfileString(_T("StripOut"), _T("Total"), strData, m_sPathBuf);

	for(i=1; i<MAX_DEF; i++)
	{
		m_nProgressReloadReelmap++;

		strMenu.Format(_T("%d"), i);
		strData.Format(_T("%d"), m_nDef[i]); // 불량이름별 불량수
		::WritePrivateProfileString(_T("Info"), strMenu, strData, m_sPathBuf);
	}

	m_bDoneReloadReelmap = TRUE;
	return TRUE;
}


char* CReelMap::StrToChar(CString str) // char* returned must be deleted... 
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

void CReelMap::StrToChar(CString str, char* pCh) // char* returned must be deleted... 
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

//BOOL CReelMap::MakeDirYield(CString sPath)
//{
//	CString str, Path[5];
//
//	switch (m_nLayer)
//	{
//	case RMAP_UP:
//		str = _T("YieldUp.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//		Path[4] = _T("");
//		break;
//	case RMAP_ALLUP:
//		str = _T("YieldAll.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
//		Path[4] = _T("");
//		break;
//	case RMAP_DN:
//		str = _T("YieldDn.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//		Path[4] = _T("");
//		break;
//	case RMAP_ALLDN:
//		str = _T("YieldAll.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sLotUp;
//		Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
//		Path[4] = _T("");
//		break;
//	case RMAP_INNER_UP:
//		str = _T("YieldUp.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
//		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
//		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
//		break;
//	case RMAP_INNER_DN:
//		str = _T("YieldDn.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
//		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
//		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
//		break;
//	case RMAP_INNER_ALLUP:
//		str = _T("YieldAll.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
//		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
//		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
//		break;
//	case RMAP_INNER_ALLDN:
//		str = _T("YieldAll.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
//		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
//		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
//		break;
//	case RMAP_INOUTER_UP:
//		break;
//	case RMAP_INOUTER_DN:
//		break;
//	case RMAP_ITS:
//		str = _T("YieldIts.txt");
//		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
//		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
//		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
//		Path[3] = _T("");
//		Path[4] = _T("");
//		break;
//	}
//
//	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
//	{
//		sPath.Format(_T("%s%s"), Path[0], Path[1]);
//		if (!DirectoryExists(sPath))
//			CreateDirectory(sPath, NULL);
//
//		if (!Path[2].IsEmpty())
//		{
//			sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
//			if (!DirectoryExists(sPath))
//				CreateDirectory(sPath, NULL);
//
//			if (!Path[3].IsEmpty())
//			{
//				sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
//				if (!DirectoryExists(sPath))
//					CreateDirectory(sPath, NULL);
//
//				if (!Path[4].IsEmpty())
//				{
//					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
//					if (!DirectoryExists(sPath))
//						CreateDirectory(sPath, NULL);
//				}
//			}
//		}
//	}
//
//	return TRUE;
//}
//
//BOOL CReelMap::ReadYield(int nSerial, CString sPath)
//{
//	int dwStart = GetTickCount();
//
//	int i, k;
//	TCHAR szData[200];
//
//	CString sCode, strMenu, strItem, sMsg;
//
//	strMenu.Format(_T("%d"), nSerial);
//	for (i = 1; i < MAX_DEF; i++)
//	{
//		sCode.Format(_T("%d"), i);
//		if (0 < ::GetPrivateProfileString(strMenu, sCode, NULL, szData, sizeof(szData), sPath))
//		{
//			m_stYield.nDefA[i] = _ttoi(szData);
//		}
//		else
//		{
//			if (nSerial == 1)
//			{
//				m_stYield.nDefA[i] = 0;
//			}
//			else
//			{
//				//sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-불량(%d)\r\n%s"), nSerial, i, sPath);
//				//AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//				return FALSE;
//			}
//		}
//	}
//
//	if (0 < ::GetPrivateProfileString(strMenu, _T("Total Pcs"), NULL, szData, sizeof(szData), sPath))
//	{
//		m_stYield.nTot = _ttoi(szData);
//	}
//	else
//	{
//		if (nSerial == 1)
//		{
//			m_stYield.nTot = 0;
//		}
//		else
//		{
//			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Total Pcs\r\n%s"), nSerial, sPath);
//			pView->ClrDispMsg(); 
//			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//			return FALSE;
//		}
//	}
//
//	if (0 < ::GetPrivateProfileString(strMenu, _T("Good Pcs"), NULL, szData, sizeof(szData), sPath))
//	{
//		m_stYield.nGood = _ttoi(szData);
//	}
//	else
//	{
//		if (nSerial == 1)
//		{
//			m_stYield.nGood = 0;
//		}
//		else
//		{
//			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Good Pcs\r\n%s"), nSerial, sPath);
//			pView->ClrDispMsg(); 
//			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//			return FALSE;
//		}
//	}
//
//	if (0 < ::GetPrivateProfileString(strMenu, _T("Bad Pcs"), NULL, szData, sizeof(szData), sPath))
//	{
//		m_stYield.nDef = _ttoi(szData);
//	}
//	else
//	{
//		if (nSerial == 1)
//		{
//			m_stYield.nDef = 0;
//		}
//		else
//		{
//			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Bad Pcs\r\n%s"), nSerial, sPath);
//			pView->ClrDispMsg(); 
//			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//			return FALSE;
//		}
//	}
//
//	for (k = 0; k < MAX_STRIP; k++)
//	{
//		strItem.Format(_T("Strip%d"), k);
//		if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
//		{
//			m_stYield.nDefStrip[k] = _ttoi(szData);
//		}
//		else
//		{
//			if (nSerial == 1)
//			{
//				m_stYield.nDefStrip[k] = 0;
//			}
//			else
//			{
//				sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Strip%d\r\n%s"), nSerial, k, sPath);
//				pView->ClrDispMsg(); 
//				AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//				return FALSE;
//			}
//		}
//
//		strItem.Format(_T("StripOut_%d"), k);
//		if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
//		{
//			m_stYield.nStripOut[k] = _ttoi(szData);
//		}
//		else
//		{
//			if (nSerial == 1)
//			{
//				m_stYield.nStripOut[k] = 0;
//			}
//			else
//			{
//				sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-StripOut_%d\r\n%s"), nSerial, k, sPath);
//				pView->ClrDispMsg(); 
//				AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//				return FALSE;
//			}
//		}
//
//		for (i = 1; i < MAX_DEF; i++)
//		{
//			strItem.Format(_T("Strip%d_%d"), k, i);
//			if (0 < ::GetPrivateProfileString(strMenu, strItem, NULL, szData, sizeof(szData), sPath))
//			{
//				m_stYield.nDefPerStrip[k][i] = _ttoi(szData);
//			}
//			else
//			{
//				if (nSerial == 1)
//				{
//					m_stYield.nDefPerStrip[k][i] = 0;
//				}
//				else
//				{
//					sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-Strip%d_%d\r\n%s"), nSerial, k, i, sPath);
//					pView->ClrDispMsg(); 
//					AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//					return FALSE;
//				}
//			}
//		}
//	}
//
//	if (0 < ::GetPrivateProfileString(strMenu, _T("StripOut_Total"), NULL, szData, sizeof(szData), sPath))
//	{
//		m_stYield.nTotSriptOut = _ttoi(szData);
//	}
//	else
//	{
//		if (nSerial == 1)
//		{
//			m_stYield.nTotSriptOut = 0;
//		}
//		else
//		{
//			sMsg.Format(_T("이전 수율 읽기 오류 : Shot(%d)-StripOut_Total\r\n%s"), nSerial, sPath);
//			pView->ClrDispMsg(); 
//			AfxMessageBox(sMsg, MB_ICONWARNING | MB_OK);
//			return FALSE;
//		}
//	}
//	
//	int dwEnd = GetTickCount();
//	int dwElapsed = dwEnd - dwStart;
//
//	return TRUE;
//}
//
//BOOL CReelMap::WriteYield(int nSerial, CString sPath)
//{
//	m_nBeforeSerial = nSerial;
//
//	int dwStart = GetTickCount();
//	int nNodeX = 0, nNodeY = 0;
//#ifdef USE_CAM_MASTER
//	nNodeX = m_pPcsRgn->nCol;
//	nNodeY = m_pPcsRgn->nRow;
//#endif
//	CString sDefNum, strData;
//	int nPnl, nRow, nCol, nDefCode, nStrip;
//	int nTotPcs = nNodeX * nNodeY;
//	int nStripPcs = nTotPcs / 4;
//	double dStOutRto = _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
//	nPnl = nSerial - 1;
//
//	int i, k;
//	TCHAR szData[200];
//
//	CString sCode, strMenu, strItem, sMsg;
//	int nTot = 0, nGood = 0, nDef = 0;
//	int nTotSriptOut = 0;
//
//	int nDefStrip[MAX_STRIP], nDefA[MAX_DEF], nDefPerStrip[MAX_STRIP][MAX_DEF], nStripOut[MAX_STRIP];
//	nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;
//	nStripOut[0] = 0; nStripOut[1] = 0; nStripOut[2] = 0; nStripOut[3] = 0;
//
//	nTot = nNodeX * nNodeY;
//
//	for (k = 0; k < MAX_DEF; k++)
//	{
//		nDefA[k] = 0;
//
//		for (i = 0; i < MAX_STRIP; i++)
//		{
//			nDefPerStrip[i][k] = 0;
//		}
//	}
//
//	for (nRow = 0; nRow < nNodeY; nRow++)
//	{
//		for (nCol = 0; nCol < nNodeX; nCol++)
//		{
//			if (m_pPnlBuf)
//			{
//				nDefCode = (int)m_pPnlBuf[nPnl][nRow][nCol] < 0 ? 0 : (int)m_pPnlBuf[nPnl][nRow][nCol];
//				nDefA[nDefCode]++;
//
//				nStrip = int(nRow / (nNodeY / MAX_STRIP));
//				if (nStrip > -1 && nStrip < MAX_STRIP)
//				{
//					if (nDefCode > 0)
//					{
//						nDefStrip[nStrip]++;
//						nDefPerStrip[nStrip][nDefCode]++;
//					}
//				}
//			}
//		}
//	}
//
//	for (nStrip = 0; nStrip < MAX_STRIP; nStrip++)
//	{
//		if (nDefStrip[nStrip] >= nStripPcs * dStOutRto)
//			nStripOut[nStrip]++;
//	}
//
//	for (i = 1; i < MAX_DEF; i++)
//	{
//		m_stYield.nDefA[i] = m_stYield.nDefA[i] + nDefA[i];
//		nDef += nDefA[i];
//	}
//	nGood = nTot - nDef;
//
//	m_stYield.nTot = m_stYield.nTot + nTot;
//	m_stYield.nGood = m_stYield.nGood + nGood;
//	m_stYield.nDef = m_stYield.nDef + nDef;
//
//	for (k = 0; k < MAX_STRIP; k++)
//	{
//		m_stYield.nDefStrip[k] = m_stYield.nDefStrip[k] + nDefStrip[k];
//		m_stYield.nStripOut[k] = m_stYield.nStripOut[k] + nStripOut[k];
//		nTotSriptOut += nStripOut[k];
//		for (i = 1; i < MAX_DEF; i++)
//			m_stYield.nDefPerStrip[k][i] = m_stYield.nDefPerStrip[k][i] + nDefPerStrip[k][i];
//	}
//	m_stYield.nTotSriptOut = m_stYield.nTotSriptOut + nTotSriptOut;
//
//
//	FILE *fp = NULL;
//	char FileName[MAX_PATH];
//
//	BOOL bExist = FALSE;
//	CFileFind findfile;
//	if (findfile.FindFile(sPath))
//		bExist = TRUE;
//	else
//		MakeDirYield(sPath);
//
//	StrToChar(sPath, FileName);
//
//	fp = fopen(FileName, "a+");
//	if (fp == NULL)
//	{
//		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
//		//AfxMessageBox(_T("It is trouble to open ReelMap.txt"),MB_ICONWARNING|MB_OK);
//		return FALSE;
//	}
//
//	if (!bExist)
//	{
//		m_nStartSerial = nSerial;
//
//		fprintf(fp, "[Info]\n");
//		fprintf(fp, "설  비  명 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "운  용  자 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "모      델 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sModelUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
//		fprintf(fp, "\n");
//
//		fprintf(fp, "Process Code = \n");
//		fprintf(fp, "CamPcsX = %d\n", nNodeX);
//		fprintf(fp, "CamPcsY = %d\n", nNodeY);
//		fprintf(fp, "\n");
//
//		fprintf(fp, "Start Shot=%d\n", m_nStartSerial);
//		fprintf(fp, "End Shot = \n\n");
//		fprintf(fp, "\n");
//
//		fprintf(fp, "Lot Start=\n");
//		fprintf(fp, "Lot End=\n");
//		fprintf(fp, "Entire Speed=\n");
//		fprintf(fp, "\n");
//
//		fprintf(fp, "Marked Shot=\n");
//		fprintf(fp, "\n");
//
//		fprintf(fp, "Total Shot=\n");
//		fprintf(fp, "Total Pcs=\n");
//		fprintf(fp, "Good Pcs=\n");
//		fprintf(fp, "Bad Pcs=\n");
//		fprintf(fp, "\n");
//
//		for (i = 1; i <= MAX_DEF; i++)
//			fprintf(fp, "%d=\n", i);
//		fprintf(fp, "\n");
//
//		fprintf(fp, "TotalStripOut=\n");
//		fprintf(fp, "\n");
//
//		for (k = 0; k < MAX_STRIP; k++)
//		{
//			fprintf(fp, "[%d Strip]\n", k);
//			fprintf(fp, "StripOut=\n");
//			fprintf(fp, "TotalPcs=\n");
//			for (i = 1; i <= MAX_DEF; i++)
//				fprintf(fp, "%d=\n", i);
//			fprintf(fp, "\n");
//		}
//
//		fprintf(fp, "\n");
//	}
//
//	fclose(fp);
//
//	strMenu.Format(_T("%d"), nSerial);
//
//	for (i = 1; i < MAX_DEF; i++)
//	{
//		sCode.Format(_T("%d"), i);
//		sDefNum.Format(_T("%d"), m_stYield.nDefA[i]);
//
//		// [Info]
//		::WritePrivateProfileString(_T("Info"), sCode, sDefNum, sPath);
//
//		// [Serial]
//		::WritePrivateProfileString(strMenu, sCode, sDefNum, sPath);
//	}
//
//	// [Info]
//	strData.Format(_T("%d"), nSerial);
//	::WritePrivateProfileString(_T("Info"), _T("End Shot"), strData, sPath);
//
//	strData.Format(_T("%d"), nSerial - m_nStartSerial + 1);
//	::WritePrivateProfileString(_T("Info"), _T("Total Shot"), strData, sPath);
//
//	strData.Format(_T("%d"), m_stYield.nTot);
//	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, sPath);
//
//	strData.Format(_T("%d"), m_stYield.nGood);
//	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, sPath);
//
//	strData.Format(_T("%d"), m_stYield.nDef);
//	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, sPath);
//
//	// [Serial]
//	strData.Format(_T("%d"), m_stYield.nTot);
//	::WritePrivateProfileString(strMenu, _T("Total Pcs"), strData, sPath);
//
//	strData.Format(_T("%d"), m_stYield.nGood);
//	::WritePrivateProfileString(strMenu, _T("Good Pcs"), strData, sPath);
//
//	strData.Format(_T("%d"), m_stYield.nDef);
//	::WritePrivateProfileString(strMenu, _T("Bad Pcs"), strData, sPath);
//
//	for (k = 0; k < MAX_STRIP; k++)
//	{
//		strItem.Format(_T("Strip%d"), k);
//		strData.Format(_T("%d"), m_stYield.nDefStrip[k]);
//		// [Info]
//		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
//		// [Serial]
//		::WritePrivateProfileString(strMenu, strItem, strData, sPath);
//
//		strItem.Format(_T("StripOut_%d"), k);
//		strData.Format(_T("%d"), m_stYield.nStripOut[k]);
//		// [Info]
//		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
//		// [Serial]
//		::WritePrivateProfileString(strMenu, strItem, strData, sPath);
//
//		for (i = 1; i < MAX_DEF; i++)
//		{
//			strItem.Format(_T("Strip%d_%d"), k, i);
//			strData.Format(_T("%d"), m_stYield.nDefPerStrip[k][i]);
//			// [Info]
//			::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);
//			// [Serial]
//			::WritePrivateProfileString(strMenu, strItem, strData, sPath);
//		}
//	}
//
//	strData.Format(_T("%d"), m_stYield.nTotSriptOut);
//	// [Info]
//	::WritePrivateProfileString(_T("Info"), _T("StripOut_Total"), strData, sPath);
//	// [Serial]
//	::WritePrivateProfileString(strMenu, _T("StripOut_Total"), strData, sPath);
//
//	int dwEnd = GetTickCount();
//	int dwElapsed = dwEnd - dwStart;
//
//	return TRUE;
//}

BOOL CReelMap::MakeHeader(CString sPath)
{
	FILE *fp = NULL;
	char FileName[MAX_PATH];
	BOOL bExist = FALSE;

	CFileFind findfile;
	if (findfile.FindFile(sPath))
	{
		bExist = TRUE;
		return TRUE;
	}

	MakeDirRmap();
	StrToChar(sPath, FileName);

	fp = fopen(FileName, "w+");
	if (fp == NULL)
	{
		pView->MsgBox(_T("It is trouble to open ReelMap.txt"));
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
	fprintf(fp, "로      트 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLotUp)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "상면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerUp)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "하면레이어 = %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sLayerDn)); if (pRtn) delete pRtn; pRtn = NULL;
	fprintf(fp, "\n");

	fprintf(fp, "Process Code = \n");
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

void CReelMap::ResetReelmapPath()
{
	m_sPathBuf = GetRmapPath(m_nLayer);
}


BOOL CReelMap::RemakeReelmap()
{
	char* cBigDefCode = ((CManagerReelmap*)m_pParent)->m_cBigDefCode;
	CString* sKorDef = ((CManagerReelmap*)m_pParent)->m_sKorDef;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sPath = GetRmapPath(m_nLayer);

	FILE *fp = NULL;
	char FileName[MAX_PATH];
	CString strFileName, strPathName;
	CFileFind findfile;
	int nStripNumY, nPieceNumPerStrip;

	CString sModel, sLot, sLayer[2];
	int i, nLastShot, nPnl, nRow, nCol, nDefCode;
	CString sPnl, sRow, sVal;
	TCHAR sep[] = { _T(",/;\r\n\t") };
	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sPath))
		nLastShot = _tstoi(szData);
	else
	{
		nLastShot = 0; // Failed.
		pView->MsgBox(_T("릴맵에 Marked Shot 정보가 없습니다."));
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("모      델"), NULL, szData, sizeof(szData), sPath))
		sModel = CString(szData);
	else
	{
		pView->MsgBox(_T("Model 정보가 없습니다."));
		return FALSE;
	}

	MakeDirRmap();

	CString sFile = _T(""), sRmapPath = sPath;

	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sFile = sRmapPath.Right(sRmapPath.GetLength() - nPos - 1);
		sRmapPath.Delete(nPos, sPath.GetLength() - nPos);
	}

	CString sPathRmapF = sRmapPath + _T("\\Reelmap");

	if (!DirectoryExists(sPathRmapF))
		CreateDirectory(sPathRmapF, NULL);

	sFile.MakeUpper();

	if (sFile == _T("REELMAPDATAALL.TXT"))
		strPathName = sPathRmapF + _T("\\ReelMap-TOTAL_SIDE.txt");
	else if (sFile == _T("REELMAPDATAUP.TXT"))
		strPathName = sPathRmapF + _T("\\ReelMap-TOP_SIDE.txt");
	else if (sFile == _T("REELMAPDATADN.TXT"))
		strPathName = sPathRmapF + _T("\\ReelMap-BOTTOM_SIDE.txt");
	else if (sFile == _T("REELMAPDATAITS.TXT"))
		strPathName = sPathRmapF + _T("\\ReelMap-ITS.txt");
	else
		strPathName = sPathRmapF + _T("\\ReelMap.txt");

	if (findfile.FindFile(strPathName))
		DeleteFile(strPathName);
	else
	{
		if (!DirectoryExists(sPathRmapF))
			CreateDirectory(sPath, NULL);
	}

	int nNodeX = 0;// = pDoc->m_pPcsRgn->nCol; // on Cam
	int nNodeY = 0;// = pDoc->m_pPcsRgn->nRow; // on Cam

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("CamPcsX"), NULL, szData, sizeof(szData), sPath))
		nNodeX = _tstoi(szData);
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("CamPcsY"), NULL, szData, sizeof(szData), sPath))
		nNodeY = _tstoi(szData);

	if (!nNodeX || !nNodeY)
		GetNodeXYonRmap(nNodeX, nNodeY, sPath);

	nStripNumY = MAX_STRIP;
	nPieceNumPerStrip = nNodeY / nStripNumY;

	m_dProgressRatio = 0.0;

	StringToChar(strPathName, FileName);

	fp = fopen(FileName, "a+");
	char* pRtn = NULL;
	if (fp != NULL)
	{
		fprintf(fp, "장비호기명 : %s\n", pRtn = StrToChar(pDoc->WorkingInfo.System.sMcName)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "운  용  자 : %s\n", pRtn = StrToChar(pDoc->WorkingInfo.LastJob.sSelUserName)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "모      델 : %s\n", pRtn = StrToChar(sModel)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "로      트 : %s\n", pRtn = StrToChar(sLot)); if (pRtn) delete pRtn; pRtn = NULL;
		fprintf(fp, "상면레이어 : %s\n", pRtn = StrToChar(sLayer[0])); if (pRtn) delete pRtn; pRtn = NULL;
		//if (bDualTest)
		if(!sLayer[1].IsEmpty())
		{
			fprintf(fp, "하면레이어 : %s\n", pRtn = StrToChar(sLayer[1])); if (pRtn) delete pRtn; pRtn = NULL;
		}
		fprintf(fp, "양폐 스트립 수율[%%] : %.1f\n", _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio)); if (pRtn) delete pRtn; pRtn = NULL;//atof

		for (i = 1; i<DEF_UPPER; i++)
		{
			fprintf(fp, "%c - %s\n", cBigDefCode[i], pRtn = StrToChar(sKorDef[i])); if (pRtn) delete pRtn; pRtn = NULL;
		}
		fprintf(fp, "? - Missed Align Panel(i.e 노광불량)\r\n\n");

		// Reload Reelmap.....
		for (nPnl = 0; nPnl<nLastShot; nPnl++)
		{
			if (!m_bThreadAliveRemakeReelmap)
				break;

			m_dProgressRatio = double(nPnl + 1) / double(nLastShot) * 100.0;

			fprintf(fp, "%d\n", nPnl + 1);
			for (nRow = 0; nRow<nNodeX; nRow++)
			{
				sPnl.Format(_T("%d"), nPnl + 1);
				sRow.Format(_T("%02d"), nRow);
				if (0 < ::GetPrivateProfileString(sPnl, sRow, NULL, szData, sizeof(szData), sPath))
				{
					for (nCol = 0; nCol<nNodeY; nCol++)
					{
						if (nCol == 0)
							sVal = _tcstok(szData, sep);
						else
							sVal = _tcstok(NULL, sep);

						nDefCode = _tstoi(sVal);

						if (nCol % nPieceNumPerStrip == 0)
							fprintf(fp, " ");

						fprintf(fp, "%c", cBigDefCode[nDefCode]);
					}
					fprintf(fp, "\n");
				}
			}
			fprintf(fp, "\n");
		}
	}
	else
	{
		pView->MsgBox(_T("It is trouble to remake ReelMap."));
		return FALSE;
	}

	fclose(fp);

	return TRUE;
}

//BOOL CReelMap::GetResult() // TRUE: Make Result, FALSE: Load Result or Failed.
//{
//	//파일을 읽어옴. ======================================================================
//
//	// 불량관련 데이타를 읽어옴.
//	int i, k;
//	CString sPathRmap, sPathYield;
//	CString strMsg, strMenu, strItem;
//	TCHAR szData[MAX_PATH];
//	CFileFind findfile;
//
//	m_stResult.nEntireStripNum = 0;
//
//	sPathRmap = GetRmapPath(m_nLayer);
//	sPathYield = GetYieldPath(m_nLayer);
//
//	if (!findfile.FindFile(sPathRmap)) // Can not find file.
//	{
//		strMsg.Format(_T("%s file open is failed"), sPathRmap);
//		pView->ClrDispMsg();
//		AfxMessageBox(strMsg);
//	}
//	else
//	{
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("설  비  명"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.sMachin = CString(szData);
//		else
//			m_stResult.sMachin = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("운  용  자"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.sOpName = CString(szData);
//		else
//			m_stResult.sOpName = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("모      델"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.sModel = CString(szData);
//		else
//			m_stResult.sModel = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("로      트"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.sLot = CString(szData);
//		else
//			m_stResult.sLot = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("상면레이어"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.sLayerUp = CString(szData);
//		else
//			m_stResult.sLayerUp = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("하면레이어"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.sLayerDn = CString(szData);
//		else
//			m_stResult.sLayerDn = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Start"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.strLotStartTime = CString(szData);
//		else
//			m_stResult.strLotStartTime = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Run"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.strLotWorkingTime = CString(szData);
//		else
//			m_stResult.strLotWorkingTime = _T("");
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot End"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.strLotEndTime = CString(szData);
//		else
//			m_stResult.strLotEndTime = _T("");
//
//		// 공종코드
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Process Code"), NULL, szData, sizeof(szData), sPathRmap))
//			m_stResult.sProcessNum = CString(szData);
//		else
//			m_stResult.sProcessNum = _T("");
//
//		// 속도
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Entire Speed"), NULL, szData, sizeof(szData), sPathYield))
//			m_stResult.dEntireSpeed = _tstof(szData);
//		else
//			m_stResult.dEntireSpeed = 0.0;
//
//		// 수율 데이타를 읽어옴.
//
//		// Read Piece infomation..............
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total Pcs"), NULL, szData, sizeof(szData), sPathYield))
//			m_stResult.nEntirePieceNum = _tstoi(szData);
//		else
//			m_stResult.nEntirePieceNum = 0;
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Good Pcs"), NULL, szData, sizeof(szData), sPathYield))
//			m_stResult.nGoodPieceNum = _tstoi(szData);
//		else
//			m_stResult.nGoodPieceNum = 0;
//
//		if (0 < ::GetPrivateProfileString(_T("Info"), _T("Bad Pcs"), NULL, szData, sizeof(szData), sPathYield))
//			m_stResult.nDefectPieceNum = _tstoi(szData);
//		else
//			m_stResult.nDefectPieceNum = 0;
//
//		m_stResult.nTotStOut = 0;
//		for (k = 0; k<MAX_STRIP; k++)
//		{
//			strMenu.Format(_T("Strip%d"), k);
//			if (0 < ::GetPrivateProfileString(_T("Info"), strMenu, NULL, szData, sizeof(szData), sPathYield))
//				m_stResult.nDefStrip[k] = _tstoi(szData);
//			else
//				m_stResult.nDefStrip[k] = 0;
//
//			strMenu.Format(_T("%d"), k);
//			if (0 < ::GetPrivateProfileString(_T("StripOut"), strMenu, NULL, szData, sizeof(szData), sPathYield))
//				m_stResult.nStripOut[k] = _tstoi(szData);
//			else
//				m_stResult.nStripOut[k] = 0;
//
//			m_stResult.nTotStOut += m_stResult.nStripOut[k];
//
//			for (i = 1; i<MAX_DEF; i++)
//			{
//				strItem.Format(_T("Strip%d"), k);
//				strMenu.Format(_T("%d"), i);
//				if (0 < ::GetPrivateProfileString(strItem, strMenu, NULL, szData, sizeof(szData), sPathYield))
//					m_stResult.nDefPerStrip[k][i] = _tstoi(szData);
//				else
//					m_stResult.nDefPerStrip[k][i] = 0;
//			}
//		}
//
//		for (i = 1; i<MAX_DEF; i++)
//		{
//			strMenu.Format(_T("%d"), i);
//			if (0 < ::GetPrivateProfileString(_T("Info"), strMenu, NULL, szData, sizeof(szData), sPathYield))
//				m_stResult.nEntireAddedDefect[i] = _tstoi(szData);  // 불량이름별 불량수를 파일에서 읽어옴.
//			else
//			{
//				m_stResult.nEntireAddedDefect[i] = 0;
//			}
//		}
//
//		m_stResult.nEntireStripNum = (m_stResult.nSerialEd - m_stResult.nSerialSt + 1) * MAX_STRIP;
//
//	}
//
//	return TRUE;
//}
//
//CString CReelMap::GetResultTxt()
//{
//	CString strFileData, strData;
//	int nTot;
//	nTot = m_stResult.nDefPerStrip[0][DEF_LIGHT] + m_stResult.nDefPerStrip[1][DEF_LIGHT] + m_stResult.nDefPerStrip[2][DEF_LIGHT] + m_stResult.nDefPerStrip[3][DEF_LIGHT];
//
//	//리포트 작성. =====================================================================
//
//	strFileData = _T("");
//	strFileData += _T("1. 일반 정보\r\n");
//	strData.Format(_T("    모 델 명 :  %s\r\n"), m_stResult.sModel);
//	strFileData += strData;
//	strData.Format(_T("    로 트 명 :  %s\r\n"), m_stResult.sLot);
//	strFileData += strData;
//	strData.Format(_T("    시작시간 :  %s\r\n"), m_stResult.strLotStartTime);
//	strFileData += strData;
//	strData.Format(_T("    진행시간 :  %s\r\n"), m_stResult.strLotWorkingTime);
//	strFileData += strData;
//	strData.Format(_T("    종료시간 :  %s\r\n"), m_stResult.strLotEndTime);
//	strFileData += strData;
//	strFileData += _T("\r\n");
//
//	strFileData += _T("2. 제품 검사 결과\r\n");
//	strData.Format(_T("    총 검 사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum);
//	strFileData += strData;
//	strData.Format(_T("    양    품 Unit 수 : %9d        양  품  율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum - m_stResult.nDefectPieceNum, 100.0*(double)(m_stResult.nEntirePieceNum - m_stResult.nDefectPieceNum) / (double)m_stResult.nEntirePieceNum);
//	strFileData += strData;
//	strData.Format(_T("    불    량 Unit 수 : %9d        불  량  율(%%) : %9.2f\r\n"), m_stResult.nDefectPieceNum, 100.0*(double)m_stResult.nDefectPieceNum / (double)m_stResult.nEntirePieceNum);
//	strFileData += strData;
//	strFileData += _T("\r\n");
//
//	strFileData += _T("3. 열별 검사 수율\r\n");
//	strData.Format(_T("    1열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
//	strFileData += strData;
//	strData.Format(_T("    1열 양품 Unit 수 : %9d        1열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[0], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[0]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strData.Format(_T("    1열 불량 Unit 수 : %9d        1열 불량율(%%) : %9.2f\r\n"), m_stResult.nDefStrip[0], 100.0*(double)m_stResult.nDefStrip[0] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strFileData += _T("\r\n");
//	strData.Format(_T("    2열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
//	strFileData += strData;
//	strData.Format(_T("    2열 양품 Unit 수 : %9d        2열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[1], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[1]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strData.Format(_T("    2열 불량 Unit 수 : %9d        2열 불량율(%%) : %9.2f\r\n"), m_stResult.nDefStrip[1], 100.0*(double)m_stResult.nDefStrip[1] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strFileData += _T("\r\n");
//	strData.Format(_T("    3열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
//	strFileData += strData;
//	strData.Format(_T("    3열 양품 Unit 수 : %9d        3열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_nDefStrip[2], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[2]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strData.Format(_T("    3열 불량 Unit 수 : %9d        3열 불량율(%%) : %9.2f\r\n"), m_nDefStrip[2], 100.0*(double)m_stResult.nDefStrip[2] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strFileData += _T("\r\n");
//	strData.Format(_T("    4열 검사 Unit 수 : %9d\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP);
//	strFileData += strData;
//	strData.Format(_T("    4열 양품 Unit 수 : %9d        4열 양품율(%%) : %9.2f\r\n"), m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[3], 100.0*(double)(m_stResult.nEntirePieceNum / MAX_STRIP - m_stResult.nDefStrip[3]) / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strData.Format(_T("    4열 불량 Unit 수 : %9d        4열 불량율(%%) : %9.2f\r\n"), m_stResult.nDefStrip[3], 100.0*(double)m_stResult.nDefStrip[3] / (double)(m_stResult.nEntirePieceNum / MAX_STRIP));
//	strFileData += strData;
//	strFileData += _T("\r\n");
//
//	strFileData += _T("4. 불량 내역\r\n");
//	strFileData += _T("    -----------------------------------------------------------------------------\r\n");
//	strFileData += _T("    번호     불량유형       1열       2열       3열       4열       유형별 불량수\r\n");
//	strFileData += _T("    -----------------------------------------------------------------------------\r\n");
//	strData.Format(_T("     1         오픈  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_OPEN], m_stResult.nDefPerStrip[1][DEF_OPEN], m_stResult.nDefPerStrip[2][DEF_OPEN], m_stResult.nDefPerStrip[3][DEF_OPEN], m_stResult.nEntireAddedDefect[DEF_OPEN]);
//	strFileData += strData;
//	strData.Format(_T("     2         쇼트  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_SHORT], m_stResult.nDefPerStrip[1][DEF_SHORT], m_stResult.nDefPerStrip[2][DEF_SHORT], m_stResult.nDefPerStrip[3][DEF_SHORT], m_stResult.nEntireAddedDefect[DEF_SHORT]);
//	strFileData += strData;
//	strData.Format(_T("     3        U-쇼트 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_USHORT], m_stResult.nDefPerStrip[1][DEF_USHORT], m_stResult.nDefPerStrip[2][DEF_USHORT], m_stResult.nDefPerStrip[3][DEF_USHORT], m_stResult.nEntireAddedDefect[DEF_USHORT]);
//	strFileData += strData;
//	strData.Format(_T("     4         결손  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_NICK], m_stResult.nDefPerStrip[1][DEF_NICK], m_stResult.nDefPerStrip[2][DEF_NICK], m_stResult.nDefPerStrip[3][DEF_NICK], m_stResult.nEntireAddedDefect[DEF_NICK]);
//	strFileData += strData;
//	strData.Format(_T("     5        선간폭 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_SPACE], m_stResult.nDefPerStrip[1][DEF_SPACE], m_stResult.nDefPerStrip[2][DEF_SPACE], m_stResult.nDefPerStrip[3][DEF_SPACE], m_stResult.nEntireAddedDefect[DEF_SPACE]);
//	strFileData += strData;
//	strData.Format(_T("     6         잔동  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_EXTRA], m_stResult.nDefPerStrip[1][DEF_EXTRA], m_stResult.nDefPerStrip[2][DEF_EXTRA], m_stResult.nDefPerStrip[3][DEF_EXTRA], m_stResult.nEntireAddedDefect[DEF_EXTRA]);
//	strFileData += strData;
//	strData.Format(_T("     7         돌기  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_PROTRUSION], m_stResult.nDefPerStrip[1][DEF_PROTRUSION], m_stResult.nDefPerStrip[2][DEF_PROTRUSION], m_stResult.nDefPerStrip[3][DEF_PROTRUSION], m_stResult.nEntireAddedDefect[DEF_PROTRUSION]);
//	strFileData += strData;
//	strData.Format(_T("     8         핀홀  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_PINHOLE], m_stResult.nDefPerStrip[1][DEF_PINHOLE], m_stResult.nDefPerStrip[2][DEF_PINHOLE], m_stResult.nDefPerStrip[3][DEF_PINHOLE], m_stResult.nEntireAddedDefect[DEF_PINHOLE]);
//	strFileData += strData;
//	strData.Format(_T("     9       패드결함%10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_PAD], m_stResult.nDefPerStrip[1][DEF_PAD], m_stResult.nDefPerStrip[2][DEF_PAD], m_stResult.nDefPerStrip[3][DEF_PAD], m_stResult.nEntireAddedDefect[DEF_PAD]);
//	strFileData += strData;
//	strData.Format(_T("    10        홀오픈 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_OPEN], m_stResult.nDefPerStrip[1][DEF_HOLE_OPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN], m_stResult.nDefPerStrip[3][DEF_HOLE_OPEN], m_stResult.nEntireAddedDefect[DEF_HOLE_OPEN]);
//	strFileData += strData;
//	strData.Format(_T("    11        홀없음 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_MISS], m_stResult.nDefPerStrip[1][DEF_HOLE_MISS], m_stResult.nDefPerStrip[2][DEF_HOLE_MISS], m_stResult.nDefPerStrip[3][DEF_HOLE_MISS], m_stResult.nEntireAddedDefect[DEF_HOLE_MISS]);
//	strFileData += strData;
//	strData.Format(_T("    12        홀편심 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_POSITION], m_stResult.nDefPerStrip[1][DEF_HOLE_POSITION], m_stResult.nDefPerStrip[2][DEF_HOLE_POSITION], m_stResult.nDefPerStrip[3][DEF_HOLE_POSITION], m_stResult.nEntireAddedDefect[DEF_HOLE_POSITION]);
//	strFileData += strData;
//	strData.Format(_T("    13       홀내불량%10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_HOLE_DEFECT], m_stResult.nDefPerStrip[1][DEF_HOLE_DEFECT], m_stResult.nDefPerStrip[2][DEF_HOLE_DEFECT], m_stResult.nDefPerStrip[3][DEF_HOLE_DEFECT], m_stResult.nEntireAddedDefect[DEF_HOLE_DEFECT]);
//	strFileData += strData;
//	strData.Format(_T("    14          POI  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_POI], m_stResult.nDefPerStrip[1][DEF_POI], m_stResult.nDefPerStrip[2][DEF_POI], m_stResult.nDefPerStrip[3][DEF_POI], m_stResult.nEntireAddedDefect[DEF_POI]);
//	strFileData += strData;
//	strData.Format(_T("    15        VH오픈 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_OPEN], m_stResult.nDefPerStrip[1][DEF_VH_OPEN], m_stResult.nDefPerStrip[2][DEF_VH_OPEN], m_stResult.nDefPerStrip[3][DEF_VH_OPEN], m_stResult.nEntireAddedDefect[DEF_VH_OPEN]);
//	strFileData += strData;
//	strData.Format(_T("    16        VH없음 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_MISS], m_stResult.nDefPerStrip[1][DEF_VH_MISS], m_stResult.nDefPerStrip[2][DEF_VH_MISS], m_stResult.nDefPerStrip[3][DEF_VH_MISS], m_stResult.nEntireAddedDefect[DEF_VH_MISS]);
//	strFileData += strData;
//	strData.Format(_T("    17        VH편심 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_POSITION], m_stResult.nDefPerStrip[1][DEF_VH_POSITION], m_stResult.nDefPerStrip[2][DEF_VH_POSITION], m_stResult.nDefPerStrip[3][DEF_VH_POSITION], m_stResult.nEntireAddedDefect[DEF_VH_POSITION]);
//	strFileData += strData;
//	strData.Format(_T("    18        VH결함 %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_VH_DEF], m_stResult.nDefPerStrip[1][DEF_VH_DEF], m_stResult.nDefPerStrip[2][DEF_VH_DEF], m_stResult.nDefPerStrip[3][DEF_VH_DEF], m_stResult.nEntireAddedDefect[DEF_VH_DEF]);
//	strFileData += strData;
//	strData.Format(_T("    19         노광  %10d%10d%10d%10d%20d\r\n"), m_stResult.nDefPerStrip[0][DEF_LIGHT], m_stResult.nDefPerStrip[1][DEF_LIGHT], m_stResult.nDefPerStrip[2][DEF_LIGHT], m_stResult.nDefPerStrip[3][DEF_LIGHT], nTot);
//	strFileData += strData;
//	strFileData += _T("    -----------------------------------------------------------------------------\r\n");
//	strFileData += _T("                                                                                 \r\n");
//	strFileData += _T("                                                                                 \r\n");
//	strFileData += _T("                                                                                 \r\n");
//	strFileData += _T("\r\n");
//
//	return strFileData;
//}
//
//CString CReelMap::GetSapp3Txt()
//{
//	CString strFileData = _T("");
//	CString strData;
//	int nSum, nStripPcs;
//	double dRateBeforeVerify, dRateAfterVerify;
//	nStripPcs = m_stResult.nEntirePieceNum / MAX_STRIP;
//
//	// 파일 이름.
//	strFileData.Format(_T("FileName : %9s_%4s_%5s.txt\r\n\r\n"), m_stResult.sLot, m_stResult.sProcessNum, m_stResult.sMachin);
//
//	// 열별 투입/완성/수율 Data.
//	strFileData += _T("1Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[0], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//	strFileData += _T("2Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[1], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//	strFileData += _T("3Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[2], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//	strFileData += _T("4Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[3], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//
//	strFileData += _T("\r\n");
//
//
//	// 열별 불량 Data.
//	strFileData += _T("1X\r\n");
//
//	if (m_stResult.nDefPerStrip[0][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[0][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_SHORT];// + m_stResult.nDefPerStrip[0][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[0][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[0][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_SPACE] + m_stResult.nDefPerStrip[0][DEF_EXTRA] + m_stResult.nDefPerStrip[0][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[0][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//	nSum = m_stResult.nDefPerStrip[0][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[0][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[0][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_VH_OPEN] + m_stResult.nDefPerStrip[0][DEF_VH_MISS] + m_stResult.nDefPerStrip[0][DEF_VH_POSITION] + m_stResult.nDefPerStrip[0][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//	strFileData += _T("2X\r\n");
//
//	if (m_stResult.nDefPerStrip[1][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[1][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_SHORT];// + m_stResult.nDefPerStrip[2][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[1][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[1][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_SPACE] + m_stResult.nDefPerStrip[1][DEF_EXTRA] + m_stResult.nDefPerStrip[1][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[1][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[1][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_VH_OPEN] + m_stResult.nDefPerStrip[1][DEF_VH_MISS] + m_stResult.nDefPerStrip[1][DEF_VH_POSITION] + m_stResult.nDefPerStrip[1][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//
//	strFileData += _T("3X\r\n");
//
//	if (m_stResult.nDefPerStrip[2][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[2][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_SHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[2][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[2][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_SPACE] + m_stResult.nDefPerStrip[2][DEF_EXTRA] + m_stResult.nDefPerStrip[2][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[2][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[2][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_VH_OPEN] + m_stResult.nDefPerStrip[2][DEF_VH_MISS] + m_stResult.nDefPerStrip[2][DEF_VH_POSITION] + m_stResult.nDefPerStrip[2][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//
//	strFileData += _T("4X\r\n");
//
//	if (m_stResult.nDefPerStrip[3][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[3][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_SHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[3][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[3][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_SPACE] + m_stResult.nDefPerStrip[3][DEF_EXTRA] + m_stResult.nDefPerStrip[3][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[3][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[3][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[3][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_VH_OPEN] + m_stResult.nDefPerStrip[3][DEF_VH_MISS] + m_stResult.nDefPerStrip[3][DEF_VH_POSITION] + m_stResult.nDefPerStrip[3][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//	// 속도.
//	strFileData += _T("\r\nS\r\n");
//	strData.Format(_T("%.2f"), m_stResult.dEntireSpeed);
//	strFileData += strData;
//	strFileData += _T("\r\n");
//
//	return strFileData;
//}
//
//CString CReelMap::GetSapp3TxtReverse()
//{
//	CString strFileData = _T("");
//	CString strData;
//	int nSum, nStripPcs;
//	double dRateBeforeVerify, dRateAfterVerify;
//	nStripPcs = m_stResult.nEntirePieceNum / MAX_STRIP;
//
//	// 파일 이름.
//	strFileData.Format(_T("FileName : %9s_%4s_%5s.txt\r\n\r\n"), m_stResult.sLot, m_stResult.sProcessNum, m_stResult.sMachin);
//
//	// 열별 투입/완성/수율 Data.
//	strFileData += _T("1Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[3]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[3], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//	strFileData += _T("2Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[2]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[2], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//	strFileData += _T("3Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[1]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[1], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//	strFileData += _T("4Q\r\n");
//	dRateBeforeVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
//	dRateAfterVerify = 100.0 * (nStripPcs - m_stResult.nDefStrip[0]) / nStripPcs;
//	strData.Format(_T("%d,%d,%.1f,%.1f\r\n"), nStripPcs, nStripPcs - m_stResult.nDefStrip[0], dRateBeforeVerify, dRateAfterVerify); // 투입수량, 완성수량, Verify전 수량, Verify후 수량
//	strFileData += strData;
//
//
//	strFileData += _T("\r\n");
//
//
//	// 열별 불량 Data.
//	strFileData += _T("1X\r\n");
//
//	if (m_stResult.nDefPerStrip[3][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[3][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_SHORT];// + m_stResult.nDefPerStrip[0][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[3][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[3][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_SPACE] + m_stResult.nDefPerStrip[3][DEF_EXTRA] + m_stResult.nDefPerStrip[3][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[3][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//	nSum = m_stResult.nDefPerStrip[3][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[3][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[3][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[3][DEF_VH_OPEN] + m_stResult.nDefPerStrip[3][DEF_VH_MISS] + m_stResult.nDefPerStrip[3][DEF_VH_POSITION] + m_stResult.nDefPerStrip[3][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//
//	strFileData += _T("2X\r\n");
//
//	if (m_stResult.nDefPerStrip[2][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[2][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_SHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[2][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[2][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_SPACE] + m_stResult.nDefPerStrip[2][DEF_EXTRA] + m_stResult.nDefPerStrip[2][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[2][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[2][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[2][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[2][DEF_VH_OPEN] + m_stResult.nDefPerStrip[2][DEF_VH_MISS] + m_stResult.nDefPerStrip[2][DEF_VH_POSITION] + m_stResult.nDefPerStrip[2][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//
//	strFileData += _T("3X\r\n");
//
//	if (m_stResult.nDefPerStrip[1][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[1][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_SHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[1][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[1][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_SPACE] + m_stResult.nDefPerStrip[1][DEF_EXTRA] + m_stResult.nDefPerStrip[1][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[1][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[1][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[1][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[1][DEF_VH_OPEN] + m_stResult.nDefPerStrip[1][DEF_VH_MISS] + m_stResult.nDefPerStrip[1][DEF_VH_POSITION] + m_stResult.nDefPerStrip[1][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//
//	strFileData += _T("4X\r\n");
//
//	if (m_stResult.nDefPerStrip[0][DEF_OPEN] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN], m_stResult.nDefPerStrip[0][DEF_OPEN]); // 오픈(B102)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_SHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT], nSum); // 쇼트(B129) // +u쇼트
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_USHORT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT], nSum); // u쇼트(B314)
//		strFileData += strData;
//	}
//
//	if (m_stResult.nDefPerStrip[0][DEF_NICK] > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_NICK], m_stResult.nDefPerStrip[0][DEF_NICK]); // 결손(B137)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_SPACE] + m_stResult.nDefPerStrip[0][DEF_EXTRA] + m_stResult.nDefPerStrip[0][DEF_PROTRUSION];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION], nSum); // 선간폭+잔동+돌기(B160)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_PINHOLE];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE], nSum); // 핀홀(B134)
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_PAD];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_PAD], nSum); // 패드(B316)
//		strFileData += strData;
//	}
//
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN], m_stResult.nDefPerStrip[0][DEF_HOLE_OPEN]);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_HOLE_MISS] + m_stResult.nDefPerStrip[0][DEF_HOLE_POSITION] + m_stResult.nDefPerStrip[0][DEF_HOLE_DEFECT];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD], nSum);
//		strFileData += strData;
//	}
//
//	nSum = m_stResult.nDefPerStrip[0][DEF_VH_OPEN] + m_stResult.nDefPerStrip[0][DEF_VH_MISS] + m_stResult.nDefPerStrip[0][DEF_VH_POSITION] + m_stResult.nDefPerStrip[0][DEF_VH_DEF];
//	if (nSum > 0 && pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] > 0)
//	{
//		strData.Format(_T("B%d,%d\r\n"), pDoc->WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF], nSum);
//		strFileData += strData;
//	}
//
//	// 속도.
//	strFileData += _T("\r\nS\r\n");
//	strData.Format(_T("%.2f"), m_stResult.dEntireSpeed);
//	strFileData += strData;
//	strFileData += _T("\r\n");
//
//	return strFileData;
//}

CString CReelMap::GetPath()
{
	return m_sPathBuf;
}

CString CReelMap::GetIpPath()
{
	CString sPath = _T("");
	CString Path[5], str;

	switch (m_nLayer)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sIpPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sIpPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sIpPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sIpPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sIpPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sIpPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sIpPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sIpPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModelUp;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLotUp;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INOUTER_UP:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sIpPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_INOUTER_DN:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sIpPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLotUp, pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ITS:
		//pDoc->GetCurrentInfoEng();
		str = _T("ReelMapDataIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sIpPathItsFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sEngItsCode,	//pDoc->m_sItsCode,
			str);
		break;
	}

	m_sIpPath = sPath;
	return sPath;
}


int CReelMap::GetRptFixPcs(int nCol, int nRow)
{
	return m_FixPcsRpt[nCol][nRow];;
}

void CReelMap::SetLastSerialOnOffline(int nSerial) 
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.64"));
		return;
	}

	m_nLastShot = nSerial;
	CString sPath = GetRmapPathOnOffline(m_nLayer);

	if (sPath.IsEmpty())
		return;

	CString sData;
	sData.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Info"), _T("Marked Shot"), sData, sPath);

	int nYear, nMonth, nDay, nHour, nMin, nSec;
	nYear = pDoc->WorkingInfo.Lot.CurTime.nYear;
	nMonth = pDoc->WorkingInfo.Lot.CurTime.nMonth;
	nDay = pDoc->WorkingInfo.Lot.CurTime.nDay;
	nHour = pDoc->WorkingInfo.Lot.CurTime.nHour;
	nMin = pDoc->WorkingInfo.Lot.CurTime.nMin;
	nSec = pDoc->WorkingInfo.Lot.CurTime.nSec;

	if (!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		sData = _T("");
	else
		sData.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);
	::WritePrivateProfileString(_T("Info"), _T("Marked Date"), sData, sPath);
}

CString CReelMap::GetRmapPathOnOffline(int nRmap)
{
	CString sPath = _T("");
	CString Path[5], str;

	switch (nRmap)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%s"), 
			pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%s"), 
			pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%s"),
			pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%s"), 
			pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModelUp,
			pDoc->WorkingInfo.LastJob.sLotUp,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	}

	return sPath;
}

BOOL CReelMap::WriteOnOffline(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.162"));
		return FALSE;
	}

	int nLayer = RMAP_NONE;

	// 수율계산 ===================================================================================================
	int nStripIdx, nDefIdx, nDefA[MAX_DEF], nDefStrip[MAX_STRIP], nDefPerStrip[MAX_STRIP][MAX_DEF], nStripOut[MAX_STRIP];

	for (int nS = 0; nS < MAX_STRIP; nS++)
	{
		nDefStrip[nS] = 0;
		nStripOut[nS] = 0;
	}

	for (int nD = 0; nD < MAX_DEF; nD++)
	{
		nDefA[nD] = 0;

		for (int nS1 = 0; nS1 < MAX_STRIP; nS1++)
		{
			nDefPerStrip[nS1][nD] = 0;
		}
	}
	// ===========================================================================================================


	CString sPath = GetRmapPathOnOffline(m_nLayer);
	MakeHeader(sPath);

	int nIdx = GetPcrIdx(nSerial);
	int nNodeX = m_pPcsRgn->nCol;
	int nNodeY = m_pPcsRgn->nRow;
	int nStripY = m_pPcsRgn->nRow / MAX_STRIP; // Strip(1~4);
	int nTotDefPcs = 0;

	if (m_nLayer == RMAP_UP || m_nLayer == RMAP_DN || m_nLayer == RMAP_ALLUP || m_nLayer == RMAP_ALLDN)
	{
		nLayer = m_nLayer - RMAP_UP;

		if (m_pPcr)
		{
			if (m_pPcr[nIdx])
				nTotDefPcs = m_pPcr[nIdx]->m_nTotDef;
		}
	}
	else if (m_nLayer == RMAP_INNER_UP || m_nLayer == RMAP_INNER_DN || m_nLayer == RMAP_INNER_ALLUP || m_nLayer == RMAP_INNER_ALLDN)
	{
		return FALSE;
	}
	else if (m_nLayer == RMAP_ITS)
	{
		return FALSE;
	}
	else
		return FALSE;

	short **pPnlBuf;
	int i, nC, nR, nPcsId, nDefCode;//, nTot, nDef, nGood;
	pPnlBuf = new short*[nNodeY];
	for (i = 0; i < nNodeY; i++)
	{
		pPnlBuf[i] = new short[nNodeX];
		memset(pPnlBuf[i], 0, sizeof(short)*nNodeX);
		memset(m_pPnlBuf[nSerial - 1][i], 0, sizeof(short)*nNodeX);
	}

	CString strData, strTemp;
	int nTotVerifyed = 0;

	for (i = 0; i < nTotDefPcs; i++)
	{
		nLayer = m_nLayer - RMAP_UP;

		if (m_pPcr[nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				switch (MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
				{
				case 0:
					nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];
					break;
				case 1:
					nPcsId = MirrorLR(m_pPcr[nIdx]->m_pDefPcs[i]);
					break;
				case 2:
					nPcsId = MirrorUD(m_pPcr[nIdx]->m_pDefPcs[i]);
					break;
				case 3:
					nPcsId = Rotate180(m_pPcr[nIdx]->m_pDefPcs[i]);
					break;
				default:
					nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];
					break;
				}
			}
			else
				nPcsId = m_pPcr[nIdx]->m_pDefPcs[i];

			nDefCode = m_pPcr[nIdx]->m_pDefType[i];

			nC = int(nPcsId / nNodeY);
			if (nC % 2)	// 홀수.
				nR = nNodeY * (nC + 1) - nPcsId - 1;
			else		// 짝수.
				nR = nPcsId - nNodeY * nC;
			pPnlBuf[nR][nC] = (short)nDefCode;	// nPnl의 열 정보.
			if (m_pPnlBuf)
				m_pPnlBuf[nSerial - 1][nR][nC] = pPnlBuf[nR][nC]; // DefCode 3D Array : [nSerial][nRow][nCol] - 릴맵파일 정보용.

			// 수율계산 ======================================================
			if (nDefCode > 0)
			{
				nDefA[nDefCode]++;
				m_stYield.nDefA[nDefCode]++;

				nStripIdx = int(nR / (nNodeY / MAX_STRIP));
				if (nStripIdx > -1 && nStripIdx < MAX_STRIP)
				{
					nDefStrip[nStripIdx]++;
					nDefPerStrip[nStripIdx][nDefCode]++;
				}
			}
			// ===============================================================
		}
		else
		{
			nTotVerifyed++;
		}
	}

	CString sPnl, sRow;
	i = 0; strData = _T("");
	::WritePrivateProfileString(sPnl, _T("Marked Date"), strData, sPath);
	sPnl.Format(_T("%d"), nSerial);
	strData.Format(_T("%d"), nTotDefPcs - nTotVerifyed);
	::WritePrivateProfileString(sPnl, _T("Total Defects"), strData, sPath);

	for (int nRow = 0; nRow < nNodeX; nRow++)			// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 첫번째 Col부터 시작해서 밑으로 내려감.
	{
		sRow.Format(_T("%02d"), nRow);
		strData.Format(_T(""));
		strTemp.Format(_T(""));

		for (int nCol = 0; nCol < nNodeY; nCol++)		// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Col : 4열 3열 2열 1열 스트립으로 표시됨.
		{
			nR = (nNodeY - 1) - nCol;					// 릴맵상의 Row
			nC = nRow;									// 릴맵상의 Col

			nLayer = m_nLayer - RMAP_UP;
			if (m_pPcr[nIdx]->m_nErrPnl == -1 || m_pPcr[nIdx]->m_nErrPnl == -2)
			{
				nDefCode = DEF_LIGHT;
				m_pPnlBuf[nSerial - 1][nR][nC] = (short)nDefCode;
			}
			else
				nDefCode = (int)pPnlBuf[nR][nC] < 0 ? 0 : (int)pPnlBuf[nR][nC];


			strTemp.Format(_T("%2d,"), nDefCode);	// 불량코드를 2칸으로 설정

			if (!nCol)								// strData에 처음으로 데이터를 추가
				strData.Insert(0, strTemp);
			else
			{
				int nLen = strData.GetLength();
				if (!(nCol%nStripY))				// Separate Strip (스트립 마다)
				{
					strData.Insert(nLen, _T("  "));
					nLen = strData.GetLength();
				}
				strData.Insert(nLen, strTemp);
			}
		}

		int nPos = strData.ReverseFind(',');		// 릴맵 Text 맨 우측의 ','를 삭제
		strData.Delete(nPos, 1);
		::WritePrivateProfileString(sPnl, sRow, strData, sPath); // 한 라인씩 릴맵 Text를 기록.
																 //::WritePrivateProfileString(sPnl, sRow, strData, m_sPathShare); // 한 라인씩 릴맵 Text를 기록.
	}	// 릴맵 Text(90도 시계방향으로 회전한 모습) 상의 Row : Shot의 마지막 Col까지 기록하고 끝남.


	// 수율계산 ======================================================
	int nTot = nNodeX * nNodeY;
	int nDef = nTotDefPcs - nTotVerifyed;
	int nGood = nTot - nDef;

	m_stYield.nTot = m_stYield.nTot + nTot;
	m_stYield.nGood = m_stYield.nGood + nGood;
	m_stYield.nDef = m_stYield.nDef + nDef;

	double dStOutRto = _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;

	int nTotSriptOut = 0;
	for (nStripIdx = 0; nStripIdx < MAX_STRIP; nStripIdx++)
	{
		if (nDefStrip[nStripIdx] >= nStripY * dStOutRto)
		{
			nStripOut[nStripIdx]++;
			nTotSriptOut++;
		}
		m_stYield.nDefStrip[nStripIdx] += nDefStrip[nStripIdx];
		m_stYield.nStripOut[nStripIdx] += nStripOut[nStripIdx];
		for (nDefIdx = 1; nDefIdx < MAX_DEF; nDefIdx++)
			m_stYield.nDefPerStrip[nStripIdx][nDefIdx] += nDefPerStrip[nStripIdx][nDefIdx];
	}	
	m_stYield.nTotSriptOut += nTotSriptOut;
	// ===============================================================

	for (i = 0; i < nNodeY; i++)
		delete[]  pPnlBuf[i];
	delete[] pPnlBuf;

	return TRUE;
}

BOOL CReelMap::WriteLastRmapInfoOnOffline()
{
	CFileFind findfile;

	CString sVal, sMsg, sPath;

	sPath = GetRmapPath(m_nLayer);
	if (!findfile.FindFile(sPath))
	{
		sMsg.Format(_T("Reelmap이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(sMsg);
		return FALSE;
	}

	CString sProcessCode, sEntireSpeed, sLotStart, sLotRun, sLotEnd;
	int nLastShot;
	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sPath))
		nLastShot = _tstoi(szData);
	else
	{
		nLastShot = 0; // Failed.
		pView->MsgBox(_T("릴맵에 Marked Shot 정보가 없습니다."));
		return FALSE;
	}

	// 공종코드
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Process Code"), NULL, szData, sizeof(szData), sPath))
		sProcessCode = CString(szData);
	else
		sProcessCode = _T("");

	// 속도
	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Entire Speed"), NULL, szData, sizeof(szData), sPath))
		sEntireSpeed = CString(szData);
	else
		sEntireSpeed = _T("0.0");

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Start"), NULL, szData, sizeof(szData), sPath))
		sLotStart = CString(szData);
	else
		sLotStart = _T("");

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot Run"), NULL, szData, sizeof(szData), sPath))
		sLotRun = CString(szData);
	else
		sLotRun = _T("");

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Lot End"), NULL, szData, sizeof(szData), sPath))
		sLotEnd = CString(szData);
	else
		sLotEnd = _T("");


	int k, i;
	CString strMenu, strItem, sCode, sDefNum, strData;

	sPath = GetRmapPathOnOffline(m_nLayer);
	MakeHeader(sPath);

	// 수율 정보 ===================================================================
	strData.Format(_T("%d"), m_stYield.nTot);
	::WritePrivateProfileString(_T("Info"), _T("Total Pcs"), strData, sPath);
	strData.Format(_T("%d"), m_stYield.nGood);
	::WritePrivateProfileString(_T("Info"), _T("Good Pcs"), strData, sPath);
	strData.Format(_T("%d"), m_stYield.nDef);
	::WritePrivateProfileString(_T("Info"), _T("Bad Pcs"), strData, sPath);

	for (i = 1; i < MAX_DEF; i++)
	{
		m_nDef[i] = m_stYield.nDefA[i];
		sCode.Format(_T("%d"), i);
		sDefNum.Format(_T("%d"), m_nDef[i]);
		::WritePrivateProfileString(_T("Info"), sCode, sDefNum, sPath);
	}

	int nTotStOut = 0;
	for (k = 0; k < MAX_STRIP; k++)
	{
		strItem.Format(_T("Strip%d"), k);
		m_nDefStrip[k] = m_stYield.nDefStrip[k];
		strData.Format(_T("%d"), m_nDefStrip[k]);
		::WritePrivateProfileString(_T("Info"), strItem, strData, sPath);

		strItem.Format(_T("%d"), k);
		m_nStripOut[k] = m_stYield.nStripOut[k];
		strData.Format(_T("%d"), m_nStripOut[k]);
		::WritePrivateProfileString(_T("StripOut"), strItem, strData, sPath);

		for (i = 1; i < MAX_DEF; i++)
		{
			strMenu.Format(_T("Strip%d"), k);
			strItem.Format(_T("%d"), i);
			m_nDefPerStrip[k][i] = m_stYield.nDefPerStrip[k][i];
			strData.Format(_T("%d"), m_nDefPerStrip[k][i]);
			::WritePrivateProfileString(strMenu, strItem, strData, sPath);
		}
	}
	strData.Format(_T("%d"), m_stYield.nTotSriptOut);
	::WritePrivateProfileString(_T("StripOut"), _T("Total"), strData, sPath);

	//=====================================================================================

	::WritePrivateProfileString(_T("Info"), _T("Process Code"), sProcessCode, sPath);
	::WritePrivateProfileString(_T("Info"), _T("Entire Speed"), sEntireSpeed, sPath);
	::WritePrivateProfileString(_T("Info"), _T("Lot Start"), sLotStart, sPath);
	::WritePrivateProfileString(_T("Info"), _T("Lot Run"), sLotRun, sPath);
	::WritePrivateProfileString(_T("Info"), _T("Lot End"), sLotEnd, sPath);
	sVal.Format(_T("%d"), GetFirstShotFromPcr());
	::WritePrivateProfileString(_T("Info"), _T("Start Serial"), sVal, sPath);
	sVal.Format(_T("%d"), GetLastShotFromPcr());
	::WritePrivateProfileString(_T("Info"), _T("End Serial"), sVal, sPath);

	return TRUE;
}


int CReelMap::GetFirstShotFromPcr()
{
	CString sPath;
	CString sFileN, sRmapPath;

	sRmapPath = GetRmapPathOnOffline(m_nLayer);
	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sFileN = sRmapPath.Right(sRmapPath.GetLength() - nPos - 1);
		sRmapPath.Delete(nPos, sRmapPath.GetLength() - nPos);
	}

	sPath = sRmapPath + _T("\\*.pcr");

	// 검색할 경로 및 파일
	// 특정 확장자를 갖는 파일을 검색하고 싶으면 '경로/*.pcr' 등으로 사용

	// 검색 클래스
	CFileFind finder;

	// CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(sPath);

	CString fileName;
	CString DirName;
	int nSerial = 0, nFirstShot = 100000;

	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();

		// folder 일 경우는 continue
		if (finder.IsDirectory() || finder.IsDots())
			continue;

		// 파일 일때
		// 파일의 이름
		CString _fileName = finder.GetFileName();

		// 현재폴더 상위폴더 썸네일파일은 제외
		if (_fileName == _T("Thumbs.db")) continue;

		fileName = finder.GetFileTitle();

		//읽어온 파일 이름에서 LastShot 번호를 찾는다.
		nSerial = _tstoi(fileName);

		if (nSerial < nFirstShot)
			nFirstShot = nSerial;
	}

	return nFirstShot;
}

int CReelMap::GetLastShotFromPcr()
{
	CString sPath;
	CString sFileN, sRmapPath;

	sRmapPath = GetRmapPathOnOffline(m_nLayer);
	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sFileN = sRmapPath.Right(sRmapPath.GetLength() - nPos - 1);
		sRmapPath.Delete(nPos, sRmapPath.GetLength() - nPos);
	}

	sPath = sRmapPath + _T("\\*.pcr");

	// 검색할 경로 및 파일
	// 특정 확장자를 갖는 파일을 검색하고 싶으면 '경로/*.pcr' 등으로 사용

	// 검색 클래스
	CFileFind finder;

	// CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(sPath);

	CString fileName;
	CString DirName;
	int nSerial = 0, nLastShot = 0;

	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();

		// folder 일 경우는 continue
		if (finder.IsDirectory() || finder.IsDots())
			continue;

		// 파일 일때
		// 파일의 이름
		CString _fileName = finder.GetFileName();

		// 현재폴더 상위폴더 썸네일파일은 제외
		if (_fileName == _T("Thumbs.db")) continue;

		fileName = finder.GetFileTitle();

		//읽어온 파일 이름에서 LastShot 번호를 찾는다.
		nSerial = _tstoi(fileName);

		if (nSerial > nLastShot)
			nLastShot = nSerial;
	}

	return nLastShot;
}

int CReelMap::GetPcrIdx(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.10"));
		return 0;
	}

	int nIdx = (nSerial - 1) % MAX_PCR_PNL;

	return nIdx;
}

void CReelMap::SetLotStartTime(DWORD dwTime)
{
	m_dwLotSt = dwTime;
}

void CReelMap::SetLotEndTime(DWORD dwTime)
{
	m_dwLotEd = dwTime;
}

int CReelMap::MirrorLR(int nPcsId) // 좌우 미러링
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	int nId, nCol, nRow, nC, nR;
	int nNodeX = m_pPcsRgn->nCol; // 1 ~
	int nNodeY = m_pPcsRgn->nRow; // 1 ~

	nCol = int(nPcsId / nNodeY); // 0 ~
	if (nCol % 2)
		nRow = nNodeY*(nCol + 1) - nPcsId - 1;
	else
		nRow = nPcsId - nNodeY*nCol; // 0 ~

	nR = nRow; // 0 ~
	nC = (nNodeX - 1) - nCol; // 0 ~
	if (nC % 2) // 홀수 : 시작 감소
		nId = nNodeY*(nC + 1) - nR - 1;
	else		// 짝수 : 시작 증가
		nId = nR + nNodeY*nC;

	return nId;
}

int CReelMap::MirrorUD(int nPcsId) // 상하 미러링
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	int nId, nCol, nRow, nC, nR;
	int nNodeX = m_pPcsRgn->nCol; // 1 ~
	int nNodeY = m_pPcsRgn->nRow; // 1 ~

	nCol = int(nPcsId / nNodeY); // 0 ~
	if (nCol % 2)
		nRow = nPcsId - nNodeY*nCol; // 0 ~
	else
		nRow = nNodeY*(nCol + 1) - nPcsId - 1;

	nR = nRow; // 0 ~
	nC = nCol; // 0 ~
	if (nC % 2) // 홀수 : 시작 감소
		nId = nNodeY*(nC + 1) - nR - 1;
	else		// 짝수 : 시작 증가
		nId = nR + nNodeY*nC;

	return nId;
}

int CReelMap::Rotate180(int nPcsId) // 180도 회전
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	int nId, nCol, nRow, nC, nR;
	int nNodeX = m_pPcsRgn->nCol; // 1 ~
	int nNodeY = m_pPcsRgn->nRow; // 1 ~

	if (nNodeX % 2)		// 홀수 : 시작 감소
	{
		nCol = (nNodeX - 1) - int(nPcsId / nNodeY); // 0 ~
		if (nCol % 2)
			nRow = nPcsId - nNodeY * (nNodeX - nCol - 1); // 0 ~
		else
			nRow = nNodeY * (nNodeX - nCol) - nPcsId - 1; // 0 ~

														  //nR = (nNodeY - 1) - nRow; // 0 ~
														  //nC = (nNodeX - 1) - nCol; // 0 ~
		nR = nRow;
		nC = nCol;

		if (nC % 2)
			nId = nNodeY*(nC + 1) - nR - 1; // 0 ~
		else
			nId = nR + nNodeY*nC;
	}
	else				// 짝수 : 시작 증가
	{
		nCol = (nNodeX - 1) - int(nPcsId / nNodeY); // 0 ~
		if (nCol % 2)
			nRow = nNodeY * (nNodeX - nCol) - nPcsId - 1; // 0 ~
		else
			nRow = nPcsId - nNodeY * (nNodeX - nCol - 1); // 0 ~

														  //nR = (nNodeY - 1) - nRow; // 0 ~
														  //nC = (nNodeX - 1) - nCol; // 0 ~
		nR = nRow;
		nC = nCol;

		if (nC % 2)
			nId = nNodeY*(nC + 1) - nR - 1; // 0 ~
		else
			nId = nR + nNodeY*nC;
	}
	return nId;
}

BOOL CReelMap::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}
