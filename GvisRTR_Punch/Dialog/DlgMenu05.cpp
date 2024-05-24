// DlgMenu05.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu05.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu05 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu05, CDialog)

CDlgMenu05::CDlgMenu05(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_05, pParent)
{
	Create();
}

CDlgMenu05::~CDlgMenu05()
{
	DelImg();
}

void CDlgMenu05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu05, CDialog)
END_MESSAGE_MAP()


// CDlgMenu05 �޽��� ó�����Դϴ�.

BOOL CDlgMenu05::Create()
{
	return CDialog::Create(CDlgMenu05::IDD);
}

void CDlgMenu05::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU05_LABEL; i++)
	{
		myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
		myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
	}
}

void CDlgMenu05::DelImg()
{
	int i;

	for (i = 0; i < MAX_MENU05_LABEL; i++)
		myLabel[i].DelImgList();
}

void CDlgMenu05::InitGui()
{
	LoadImg();
}

BOOL CDlgMenu05::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
