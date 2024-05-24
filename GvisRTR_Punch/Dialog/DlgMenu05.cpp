// DlgMenu05.cpp : 구현 파일입니다.
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

// CDlgMenu05 대화 상자입니다.

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


// CDlgMenu05 메시지 처리기입니다.

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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
