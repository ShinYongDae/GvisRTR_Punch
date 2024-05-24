#include "stdafx.h"
#include "ManagerStatus.h"

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


CManagerStatus::CManagerStatus()
{
	Init();

	if (Create())
	{
	}
}


CManagerStatus::~CManagerStatus()
{
}

void CManagerStatus::Init()
{

}

BOOL CManagerStatus::Create()
{
	return TRUE;
}
