// DlgMenu03.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu03.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu03 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMenu03, CDialog)

CDlgMenu03::CDlgMenu03(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_03, pParent)
{
	Create();
}

CDlgMenu03::~CDlgMenu03()
{
	DelImg();
}

void CDlgMenu03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu03, CDialog)
END_MESSAGE_MAP()


// CDlgMenu03 메시지 처리기입니다.

BOOL CDlgMenu03::Create()
{
	return CDialog::Create(CDlgMenu03::IDD);
}

void CDlgMenu03::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
		myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
	}

	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		switch (i)
		{
		case 0:		// Run-Main
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 33:	// Stop-Main
		case 49:	// LaserPoint-Punching
		case 50:	// LaserPoint-AOI상
		case 65:	// LaserPoint-AOI하
		case 82:	// LaserPoint-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_RED_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 34:	// 운전준비-Main
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 1:		// Reset-Main
		case 40:	// 정지-Uncoiler
		case 47:	// 정지-Recoiler
			myBtn[i].LoadBkImage(IMG_CIR_YLO_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_YLO_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 4:		// 연동선택-Recoiler
		case 9:		// 연동선택-Punching
		case 17:	// 연동선택-AOI상
		case 25:	// 연동선택-Uncoiler
		case 55:	// 연동선택-AOI하
		case 72:	// 연동선택-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRY_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 2:		// TorqueMotor-Punching
		case 3:		// TorqueMotor-AOI
		case 5:		// 정회전-Recoiler
		case 6:		// 역회전-Recoiler
		case 10:	// 정회전-Punching
		case 11:	// 역회전-Punching
		case 18:	// 정회전-AOI상
		case 19:	// 역회전-AOI상
		case 26:	// 정회전-Uncoiler
		case 27:	// 역회전-Uncoiler
		case 32:	// 간지 정회전-Uncoiler
		case 39:	// 간지 역회전-Uncoiler
		case 45:	// 간지 정회전-Uncoiler
		case 46:	// 간지 역회전-Uncoiler
		case 56:	// 정회전-AOI하
		case 57:	// 역회전-AOI하
		case 70:	// Core150mm-Recoiler
		case 71:	// Core150mm-Uncoiler
		case 73:	// 정회전-Engraving
		case 74:	// 역회전-Engraving
		case 83:	// TorqueMotor-Engraving
		case 85:	// 초음파세정기-Engrave
		case 86:	// 초음파세정기-AOI_DN
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 12:	// 피딩진공-Punching
		case 13:	// 제품푸쉬-Punching
		case 20:	// 피딩진공-AOI상
		case 21:	// 제품푸쉬-AOI상
		case 28:	// 제품댄서롤상승-Uncoiler
		case 29:	// 클린롤러상승-Uncoiler
		case 31:	// 간지척-Uncoiler
		case 35:	// 제품척-Uncoiler
		case 36:	// 클린롤러누름상승-Uncoiler
		case 41:	// 제품척-Recoiler
		case 42:	// 제품댄서롤상승-Recoiler
		case 44:	// 간지척-Recoiler
		case 48:	// 댄서롤상승-Punching
		case 51:	// 피딩클램프-Punching
		case 52:	// 텐션클램프-Punching
		case 53:	// 피딩클램프-AOI상
		case 54:	// 텐션클램프-AOI상
		case 58:	// 피딩진공-AOI하
		case 59:	// 제품푸쉬-AOI하
		case 62:	// 피딩클램프-AOI하
		case 63:	// 텐션클램프-AOI하
		case 66:	// Rewinder동작-Recoiler
		case 67:	// Rewinder제품간지-Recoiler
		case 75:	// 피딩진공-Engraving
		case 76:	// 제품푸쉬-Engraving
		case 79:	// 피딩클램프-Engraving
		case 80:	// 텐션클램프-Engraving
		case 84:	// 댄서롤상승-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_DGN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_DGN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 7:		// 제품이음매상승우-Recoiler
		case 8:		// 제품이음매진공-Recoiler
		case 14:	// 테이블블로워-Punching
		case 15:	// 테이블진공-Punching
		case 22:	// 테이블블로워-AOI상
		case 23:	// 테이블진공-AOI상
		case 30:	// 제품이음매상승좌-Uncoiler
		case 37:	// 제품이음매상승우-Uncoiler
		case 38:	// 제품이음매진공-Uncoiler
		case 43:	// 제품이음매상승좌-Recoiler
		case 60:	// 테이블블로워-AOI하
		case 61:	// 테이블진공-AOI하
		case 77:	// 테이블블로워-Engraving
		case 78:	// 테이블진공-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_PNK_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_PNK_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 16:	// 한판넬이송-Punching
		case 24:	// 한판넬이송-AOI상	
		case 64:	// 한판넬이송-AOI하	
		case 81:	// 한판넬이송-Engraving	
			myBtn[i].LoadBkImage(IMG_CIR_ORG_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 68:	// InductionMotor-Recoiler
		case 69:	// InductionMotor-Uncoiler	
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		}
	}

	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
}

void CDlgMenu03::DelImg()
{
	int i;

	for (i = 0; i < MAX_MENU03_LABEL; i++)
		myLabel[i].DelImgList();

	for (i = 0; i < MAX_MENU03_BTN; i++)
		myBtn[i].DelImgList();

	myStcTitle[33].DelImgList();
	myStcTitle[34].DelImgList();
	myStcTitle[35].DelImgList();
}

void CDlgMenu03::InitGui()
{
	LoadImg();
}

BOOL CDlgMenu03::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
