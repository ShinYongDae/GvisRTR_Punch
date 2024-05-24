// DlgInfo.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgInfo.h"
#include "afxdialogex.h"


// CDlgInfo ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgInfo, CDialog)

CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_INFO, pParent)
{
	Create();
}

CDlgInfo::~CDlgInfo()
{
	DelImg();
}

BOOL CDlgInfo::Create()
{
	return CDialog::Create(CDlgInfo::IDD);
}

void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
END_MESSAGE_MAP()


// CDlgInfo �޽��� ó�����Դϴ�.


void CDlgInfo::LoadImg()
{
	int i;
	for (i = 0; i < MAX_INFO_BTN; i++)
	{
		myBtn[i].LoadBkImage(IMG_BTN_UP_DlgInfo, BTN_IMG_UP);
		myBtn[i].LoadBkImage(IMG_BTN_DN_DlgInfo, BTN_IMG_DN);
	}

	myBtnExit.LoadBkImage(MSG_BTN_BK_UP, BTN_IMG_UP);
	myBtnExit.LoadBkImage(MSG_BTN_BK_DN, BTN_IMG_DN);
	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_UP, CSize(40, 40), BTN_POS_LEFT);
	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_DN, CSize(40, 40), BTN_POS_LEFT);
}

void CDlgInfo::DelImg()
{
	int i;
	for (i = 0; i < MAX_INFO_BTN; i++)
		myBtn[i].DelImgList();

	myBtnExit.DelImgList();
}

BOOL CDlgInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	LoadImg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
