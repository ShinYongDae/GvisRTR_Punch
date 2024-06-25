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
}