// DlgMenu04.cpp : 구현 파일입니다.
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

// CDlgMenu04 대화 상자입니다.

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


// CDlgMenu04 메시지 처리기입니다.

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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
