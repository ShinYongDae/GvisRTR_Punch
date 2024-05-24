// DlgFrameHigh.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgFrameHigh.h"
#include "afxdialogex.h"


#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgFrameHigh 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgFrameHigh, CDialog)

CDlgFrameHigh::CDlgFrameHigh(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_FRAME_HIGH, pParent)
{

	Create();
}

CDlgFrameHigh::~CDlgFrameHigh()
{
	DelImg();
}

BOOL CDlgFrameHigh::Create()
{
	return CDialog::Create(CDlgFrameHigh::IDD);
}

void CDlgFrameHigh::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlgFrameHigh, CDialog)
	ON_BN_CLICKED(IDC_CHK_MENU_01, &CDlgFrameHigh::OnBnClickedChkMenu01)
	ON_BN_CLICKED(IDC_CHK_MENU_02, &CDlgFrameHigh::OnBnClickedChkMenu02)
	ON_BN_CLICKED(IDC_CHK_MENU_03, &CDlgFrameHigh::OnBnClickedChkMenu03)
	ON_BN_CLICKED(IDC_CHK_MENU_04, &CDlgFrameHigh::OnBnClickedChkMenu04)
	ON_BN_CLICKED(IDC_CHK_MENU_05, &CDlgFrameHigh::OnBnClickedChkMenu05)
	ON_BN_CLICKED(IDC_CHK_MENU_06, &CDlgFrameHigh::OnBnClickedChkMenu06)
END_MESSAGE_MAP()


// CDlgFrameHigh 메시지 처리기입니다.

void CDlgFrameHigh::LoadImg()
{
	myChkMenu01.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu01.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu01.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	myChkMenu02.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu02.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu02.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	myChkMenu03.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu03.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu03.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	myChkMenu04.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu04.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu04.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	myChkMenu05.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu05.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu05.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	myChkMenu06.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu06.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu06.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	int i;
	for (i = 0; i < MAX_FRMHIGH_LABEL; i++)
	{
		switch (i)
		{
		case 0:
		case 1:
			myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
			myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
			break;
		case 2:
			myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
			myLabel[i].LoadImage(ICO_LED_RED_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
			break;
		case 3:
		case 4:
			myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
			myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
			break;
		}
	}
}

void CDlgFrameHigh::DelImg()
{
	myChkMenu01.DelImgList();
	myChkMenu02.DelImgList();
	myChkMenu03.DelImgList();
	myChkMenu04.DelImgList();
	myChkMenu05.DelImgList();
	myChkMenu06.DelImgList();

	int i;
	for (i = 0; i < MAX_FRMHIGH_LABEL; i++)
		myLabel[i].DelImgList();
}

BOOL CDlgFrameHigh::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	LoadImg();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgFrameHigh::ChkMenu01()
{
	// TODO: Add your control notification handler code here
	//SetChk(IDC_CHK_MENU_01);
	pView->ShowDlg(IDD_DLG_MENU_01);
}

void CDlgFrameHigh::OnBnClickedChkMenu01()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//if (myChkMenu01.GetCheck())
	//{
	//	SetChk(IDC_CHK_MENU_01);
	//	return;
	//}
	ChkMenu01();
}


void CDlgFrameHigh::OnBnClickedChkMenu02()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgFrameHigh::OnBnClickedChkMenu03()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgFrameHigh::OnBnClickedChkMenu04()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgFrameHigh::OnBnClickedChkMenu05()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgFrameHigh::OnBnClickedChkMenu06()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

