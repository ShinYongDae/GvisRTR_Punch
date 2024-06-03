#pragma once
#include "afxwin.h"

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

public:
	CManagerReelmap(CWnd* pParent = NULL);
	~CManagerReelmap();

	// Ư���Դϴ�.
public:
	stPcrShare PcrShare[2];


	// �۾��Դϴ�.
public:
	BOOL InitAct();


	// ������ �޽��� �� �Լ�
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

