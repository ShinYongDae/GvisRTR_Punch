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
	Create();
}

CDlgMenu01::~CDlgMenu01()
{
	DelImg();
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

void CDlgMenu01::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU01_BTN; i++)
	{
		if (i == 7 || i == 8 || i == 9 || i == 10 || i == 11)
		{
			myBtn[i].LoadBkImage(NI_BTN_UP_DlgMenu01, BTN_IMG_UP);
			myBtn[i].LoadBkImage(NI_BTN_DN_DlgMenu01, BTN_IMG_DN);
		}
		else if (i == 15 || i == 20)
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_PURPLE_S, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_PURPLE_S, BTN_IMG_DN);
		}
		else if (i == 16 || i == 17 || i == 18 || i == 19)
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_GRAY_S, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_BLUE_S, BTN_IMG_DN);
		}
		else
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu01, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu01, BTN_IMG_DN);
		}
	}
}

void CDlgMenu01::DelImg()
{
	int i;
	for (i = 0; i < MAX_MENU01_BTN; i++)
		myBtn[i].DelImgList();
}

void CDlgMenu01::InitGui()
{
	LoadImg();
}

BOOL CDlgMenu01::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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

