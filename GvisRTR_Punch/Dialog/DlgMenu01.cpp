// DlgMenu01.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu01.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu01 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu01, CDialog)

CDlgMenu01::CDlgMenu01(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_01, pParent)
{

}

CDlgMenu01::~CDlgMenu01()
{
}

void CDlgMenu01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu01, CDialog)
END_MESSAGE_MAP()


// CDlgMenu01 �޽��� ó�����Դϴ�.

BOOL CDlgMenu01::Create()
{
	return CDialog::Create(CDlgMenu01::IDD);
}


void CDlgMenu01::ChkUserInfo(BOOL bOn)
{
	if (bOn)
	{
		pView->PostMessage(WM_DLG_INFO, 0, 0);
	}
	else
	{
		UpdateData();
	}

	//myBtn[1].SetCheck(bOn);
}
