// PcsRgn.cpp : implementation file
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "CamMaster.h"
#include "PcsRgn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CPcsRgn

CPcsRgn::CPcsRgn(int nPcs, CWnd* pParent /*=NULL*/)
{
	m_pParent = pParent;
	pPcs = NULL;
	pCenter = NULL;
	pMkPnt[0] = NULL;
	pMkPnt[1] = NULL;
	nTotPcs = nPcs;
	if(nPcs>0)
	{
		pPcs = new CRect[nPcs];
		pCenter = new CfPoint[nPcs];
		pMkPnt[0] = new CfPoint[nPcs];
		pMkPnt[1] = new CfPoint[nPcs];
	}

	nCol = 0;
	nRow = 0;

	m_ptPinPos[0].x = 0.0;
	m_ptPinPos[0].y = 0.0;
	m_ptPinPos[1].x = 0.0;
	m_ptPinPos[1].y = 0.0;
	
	m_ptOffset.x = 0.0;
	m_ptOffset.y = 0.0;
}

CPcsRgn::~CPcsRgn()
{
	if(pPcs)
	{
		delete[] pPcs;
		pPcs = NULL;
	}
	if(pCenter)
	{
		delete[] pCenter;
		pCenter = NULL;
	}
	if(pMkPnt[0])
	{
		delete[] pMkPnt[0];
		pMkPnt[0] = NULL;
	}
	if(pMkPnt[1])
	{
		delete[] pMkPnt[1];
		pMkPnt[1] = NULL;
	}
}


BEGIN_MESSAGE_MAP(CPcsRgn, CWnd)
	//{{AFX_MSG_MAP(CPcsRgn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPcsRgn message handlers

void CPcsRgn::SetMkPnt(int nCam)
{
	CfPoint ptRef, ptTgt,  ptRefPin[2], ptAdjCM[2];
	int nC, nR, idx=0;

	CCamMaster* pMst = (CCamMaster*)m_pParent;

	if(nCam == CAM_BOTH)
	{
		for(nC=0; nC<nCol; nC++)
		{
			for(nR=0; nR<nRow; nR++)
			{
				ptRef.x = pMst->m_stPcsMk[idx].X;
				ptRef.y = pMst->m_stPcsMk[idx].Y;

				pMkPnt[0][idx].x = ptRef.x + m_ptPinPos[0].x;
				pMkPnt[0][idx].y = ptRef.y + m_ptPinPos[0].y;
				pMkPnt[1][idx].x = ptRef.x + m_ptPinPos[1].x;
				pMkPnt[1][idx].y = ptRef.y + m_ptPinPos[1].y;

				idx++;
			}
		}
	}
	else
 	{
		for(nC=0; nC<nCol; nC++)
		{
			for(nR=0; nR<nRow; nR++)
			{
				ptRef.x = pMst->m_stPcsMk[idx].X;
				ptRef.y = pMst->m_stPcsMk[idx].Y;

				pMkPnt[nCam][idx].x = ptRef.x + m_ptPinPos[nCam].x;
				pMkPnt[nCam][idx].y = ptRef.y + m_ptPinPos[nCam].y;

				idx++;
			}
		}
	}
}

CfPoint CPcsRgn::GetMkPnt(int nPcsId)
{
	CfPoint ptPnt;
	ptPnt.x = pMkPnt[0][nPcsId].x;
	ptPnt.y = pMkPnt[0][nPcsId].y;
	return ptPnt;
}

CfPoint CPcsRgn::GetMkPnt0(int nPcsId)
{
	CfPoint ptPnt;
	ptPnt.x = pMkPnt[0][nPcsId].x;
	ptPnt.y = pMkPnt[0][nPcsId].y;

	return ptPnt;
}

CfPoint CPcsRgn::GetMkPnt1(int nPcsId)
{
	CfPoint ptPnt;
	ptPnt.x = pMkPnt[1][nPcsId].x;
	ptPnt.y = pMkPnt[1][nPcsId].y;
	return ptPnt;
}

void CPcsRgn::GetMkPnt(int nC, int nR, int &nPcsId, CfPoint &ptPnt)
{
	int nNodeX = nCol;
	int nNodeY = nRow;

	if(nC<nNodeX && nR<nNodeY)
	{
		if(nC%2)	// 홀수.
			nPcsId = nNodeY * (nC+1) - 1 - nR;
		else		// 짝수.
			nPcsId = nNodeY * nC + nR;

		ptPnt.x = pMkPnt[0][nPcsId].x;
		ptPnt.y = pMkPnt[0][nPcsId].y;
	}
	else
		nPcsId = -1;
}

void CPcsRgn::GetPcsRgn(int nC, int nR, int &nPcsId, CRect &ptRect)
{
	int nNodeX = nCol;
	int nNodeY = nRow;

	if (nC < nNodeX && nR < nNodeY)
	{
		if (nC % 2)	// 홀수.
			nPcsId = nNodeY * (nC + 1) - 1 - nR;
		else		// 짝수.
			nPcsId = nNodeY * nC + nR;

		ptRect.left = pPcs[nPcsId].left;
		ptRect.top = pPcs[nPcsId].top;
		ptRect.right = pPcs[nPcsId].right;
		ptRect.bottom = pPcs[nPcsId].bottom;
	}
	else
		nPcsId = -1;
}


BOOL CPcsRgn::GetMkMatrix(int nPcsId, int &nC, int &nR)
{
	int nNodeX = nCol;
	int nNodeY = nRow;

	CCamMaster* pMst = (CCamMaster*)m_pParent;

	if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
	{
		switch (pMst->MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
		{
		case 0:
			break;
		case 1:
			nPcsId = MirrorLR(nPcsId);
			break;
		case 2:
			nPcsId = MirrorUD(nPcsId);
			break;
		case 3:
			nPcsId = Rotate180(nPcsId);
			break;
		default:
			break;
		}
	}

	if(-1 < nPcsId && nPcsId < (nNodeX*nNodeY))
	{
		nC = int(nPcsId/nNodeY);
		if(nC%2)	// 홀수.
			nR = nNodeY*(nC+1)-1-nPcsId;
		else		// 짝수.
			nR = nPcsId-nC*nNodeY;
	}
	else
	{
		nC = -1;
		nR = -1;
		return FALSE;
	}

	return TRUE;
}

BOOL CPcsRgn::GetMkMatrix(int nPcsId, int &nStrip, int &nC, int &nR) // nStrip:0~3 , nC:0~ , nR:0~
{
	int nNodeX = nCol;
	int nNodeY = nRow;
	int nStPcsY = nNodeY / MAX_STRIP;
	int nRow;// , nCol;

	CCamMaster* pMst = (CCamMaster*)m_pParent;

	if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
	{
		switch (pMst->MasterInfo.nActionCode)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
		{
		case 0:
			break;
		case 1:
			nPcsId = MirrorLR(nPcsId);
			break;
		case 2:
			nPcsId = MirrorUD(nPcsId);
			break;
		case 3:
			nPcsId = Rotate180(nPcsId);
			break;
		default:
			break;
		}
	}

	if(-1 < nPcsId && nPcsId < (nNodeX*nNodeY))
	{
		nC = int(nPcsId/nNodeY);
		if(nC%2)	// 홀수.
			nRow = nNodeY*(nC+1)-1-nPcsId;
		else		// 짝수.
			nRow = nPcsId-nC*nNodeY;
	}
	else
	{
		nC = -1;
		nR = -1;
		return FALSE;
	}

	nStrip = int(nRow / nStPcsY);
	nR = nRow % nStPcsY;

	return TRUE;
}

void CPcsRgn::SetPinPos(int nCam, CfPoint ptPnt)
{
	m_ptPinPos[nCam].x = ptPnt.x;
	m_ptPinPos[nCam].y = ptPnt.y;
}


void CPcsRgn::GetShotRowCol(int& nR, int& nC)
{
	nR = nRow;
	nC = nCol;
}

void CPcsRgn::SetShotRowCol(int nR, int nC)
{
	nRow = nR;
	nCol = nC;
}

void CPcsRgn::SetShotRgn(CRect rect)
{
	rtFrm.left = rect.left;
	rtFrm.top = rect.top;
	rtFrm.right = rect.right;
	rtFrm.bottom = rect.bottom;
}

CRect CPcsRgn::GetShotRgn()
{
	return rtFrm;
}

int CPcsRgn::GetTotPcs()
{
	return nTotPcs;
}

int CPcsRgn::MirrorLR(int nPcsId) // 좌우 미러링
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	CCamMaster* pMst = (CCamMaster*)m_pParent;

	int nId, nCol, nRow, nC, nR;
	int nNodeX = pMst->m_pPcsRgn->nCol; // 1 ~
	int nNodeY = pMst->m_pPcsRgn->nRow; // 1 ~

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

int CPcsRgn::MirrorUD(int nPcsId) // 상하 미러링
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	CCamMaster* pMst = (CCamMaster*)m_pParent;

	int nId, nCol, nRow, nC, nR;
	int nNodeX = pMst->m_pPcsRgn->nCol; // 1 ~
	int nNodeY = pMst->m_pPcsRgn->nRow; // 1 ~

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

int CPcsRgn::Rotate180(int nPcsId) // 180도 회전
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	CCamMaster* pMst = (CCamMaster*)m_pParent;

	int nId, nCol, nRow, nC, nR;
	int nNodeX = pMst->m_pPcsRgn->nCol; // 1 ~
	int nNodeY = pMst->m_pPcsRgn->nRow; // 1 ~

	if (nNodeX % 2)		// 홀수 : 시작 감소
	{
		nCol = (nNodeX - 1) - int(nPcsId / nNodeY); // 0 ~
		if (nCol % 2)
			nRow = nPcsId - nNodeY * (nNodeX - nCol - 1); // 0 ~
		else
			nRow = nNodeY * (nNodeX - nCol) - nPcsId - 1; // 0 ~

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

		nR = nRow;
		nC = nCol;

		if (nC % 2)
			nId = nNodeY*(nC + 1) - nR - 1; // 0 ~
		else
			nId = nR + nNodeY*nC;
	}
	return nId;
}
