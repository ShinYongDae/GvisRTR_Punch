// DlgMenu04.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu04.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu04 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu04, CDialog)

CDlgMenu04::CDlgMenu04(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_04, pParent)
{
	Create();
}

CDlgMenu04::~CDlgMenu04()
{
	DelImg();
}

void CDlgMenu04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu04, CDialog)
END_MESSAGE_MAP()


// CDlgMenu04 �޽��� ó�����Դϴ�.

BOOL CDlgMenu04::Create()
{
	return CDialog::Create(CDlgMenu04::IDD);
}

void CDlgMenu04::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU04_BTN; i++)
	{
		switch (i)
		{
		case 0:
		case 1:
		case 2:
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu04, BTN_IMG_DN);
			break;
		case 5:
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu04, BTN_IMG_DN);
			break;
		case 6:
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu04, BTN_IMG_DN);
			break;
		default:
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu04, BTN_IMG_DN);
			break;
		}
	}
}

void CDlgMenu04::DelImg()
{
	int i;
	for (i = 0; i < MAX_MENU04_BTN; i++)
		myBtn[i].DelImgList();
}

void CDlgMenu04::InitGui()
{
	LoadImg();
}

BOOL CDlgMenu04::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
