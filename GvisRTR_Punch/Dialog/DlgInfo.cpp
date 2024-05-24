// DlgInfo.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgInfo.h"
#include "afxdialogex.h"


// CDlgInfo 대화 상자입니다.

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


// CDlgInfo 메시지 처리기입니다.


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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadImg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
