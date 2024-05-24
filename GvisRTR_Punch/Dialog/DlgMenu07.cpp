// DlgMenu07.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu07.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu07 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu07, CDialog)

CDlgMenu07::CDlgMenu07(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_07, pParent)
{
	Create();
}

CDlgMenu07::~CDlgMenu07()
{
	DelImg();
}

void CDlgMenu07::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu07, CDialog)
END_MESSAGE_MAP()


// CDlgMenu07 �޽��� ó�����Դϴ�.

BOOL CDlgMenu07::Create()
{
	return CDialog::Create(CDlgMenu07::IDD);
}

void CDlgMenu07::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU07_BTN; i++)
	{
		if (i == 7 || i == 8 || i == 9 || i == 10 || i == 11)
		{
			myBtn[i].LoadBkImage(NI_BTN_UP_DlgMenu06, BTN_IMG_UP);
			myBtn[i].LoadBkImage(NI_BTN_DN_DlgMenu06, BTN_IMG_DN);
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
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu06, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu06, BTN_IMG_DN);
		}
	}
}

void CDlgMenu07::DelImg()
{
	int i;
	for (i = 0; i < MAX_MENU07_BTN; i++)
		myBtn[i].DelImgList();
}

void CDlgMenu07::InitGui()
{
	LoadImg();
}

BOOL CDlgMenu07::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
