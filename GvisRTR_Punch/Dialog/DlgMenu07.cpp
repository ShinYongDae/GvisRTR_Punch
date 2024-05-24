// DlgMenu07.cpp : 구현 파일입니다.
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

// CDlgMenu07 대화 상자입니다.

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


// CDlgMenu07 메시지 처리기입니다.

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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
