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
	m_pRect = NULL;
	Create();
}

CDlgMenu03::~CDlgMenu03()
{
	DelImg();

	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

void CDlgMenu03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu03, CDialog)
	ON_WM_SHOWWINDOW()
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
	InitStatic();
	InitBtn();
	InitGroup();
	InitLabel();

	LoadImg();
}

void CDlgMenu03::InitGroup()
{
	myGrp[0].SubclassDlgItem(IDC_GRP_00, this);
	myGrp[0].SetHwnd(this->GetSafeHwnd(), IDC_GRP_00);	// Main

	myGrp[1].SubclassDlgItem(IDC_GRP_1, this);
	myGrp[1].SetHwnd(this->GetSafeHwnd(), IDC_GRP_1);	// Recoiler

	myGrp[2].SubclassDlgItem(IDC_GRP_02, this);
	myGrp[2].SetHwnd(this->GetSafeHwnd(), IDC_GRP_02);	// Punching

	myGrp[3].SubclassDlgItem(IDC_GRP_4, this);
	myGrp[3].SetHwnd(this->GetSafeHwnd(), IDC_GRP_4);	// AOI하

	myGrp[4].SubclassDlgItem(IDC_GRP_03, this);
	myGrp[4].SetHwnd(this->GetSafeHwnd(), IDC_GRP_03);	// AOI상

	myGrp[5].SubclassDlgItem(IDC_GRP_04, this);
	myGrp[5].SetHwnd(this->GetSafeHwnd(), IDC_GRP_04);	// Uncoiler

	myGrp[6].SubclassDlgItem(IDC_GRP_05, this);
	myGrp[6].SetHwnd(this->GetSafeHwnd(), IDC_GRP_05);	// TorqueMotor

	myGrp[7].SubclassDlgItem(IDC_GRP_16, this);
	myGrp[7].SetHwnd(this->GetSafeHwnd(), IDC_GRP_16);	// InductionMotor

	myGrp[8].SubclassDlgItem(IDC_GRP_17, this);
	myGrp[8].SetHwnd(this->GetSafeHwnd(), IDC_GRP_17);	// Core150mm

	myGrp[9].SubclassDlgItem(IDC_GRP_3, this);
	myGrp[9].SetHwnd(this->GetSafeHwnd(), IDC_GRP_3);	// Engraving

	for (int i = 0; i < MAX_MENU03_GRP; i++)
	{
		myGrp[i].SetFontName(_T("Arial"));
		myGrp[i].SetFontSize(14);
		myGrp[i].SetFontBold(TRUE);
		myGrp[i].SetTextColor(RGB_NAVY);
		myGrp[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::InitBtn()
{
	// Main
	myBtn[0].SubclassDlgItem(IDC_CHK_0, this);			// Run
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_0);
	myBtn[0].SetBoarder(FALSE);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[34].SubclassDlgItem(IDC_CHK_34, this);		// 운전준비
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_CHK_34);
	myBtn[34].SetBoarder(FALSE);
	myBtn[34].SetBtnType(BTN_TYPE_CHECK);

	myBtn[33].SubclassDlgItem(IDC_CHK_33, this);		// Stop
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_CHK_33);
	myBtn[33].SetBoarder(FALSE);
	myBtn[33].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_1, this);			// Reset
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1);
	myBtn[1].SetBoarder(FALSE);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	// Torque Motor
	myBtn[2].SubclassDlgItem(IDC_CHK_2, this);			// 펀칭부
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2);
	myBtn[2].SetBoarder(FALSE);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_3, this);			// 검사부
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_3);
	myBtn[3].SetBoarder(FALSE);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[83].SubclassDlgItem(IDC_CHK_84, this);		// 각인부
	myBtn[83].SetHwnd(this->GetSafeHwnd(), IDC_CHK_84);
	myBtn[83].SetBoarder(FALSE);
	myBtn[83].SetBtnType(BTN_TYPE_CHECK);

	// 초음파 세정기 속도
	myBtn[85].SubclassDlgItem(IDC_CHK_87, this);		// 초음파 세정기 속도 - 각인부
	myBtn[85].SetHwnd(this->GetSafeHwnd(), IDC_CHK_87);
	myBtn[85].SetBoarder(FALSE);
	myBtn[85].SetBtnType(BTN_TYPE_CHECK);

	myBtn[86].SubclassDlgItem(IDC_CHK_88, this);		// 초음파 세정기 속도 - AOI 하면
	myBtn[86].SetHwnd(this->GetSafeHwnd(), IDC_CHK_88);
	myBtn[86].SetBoarder(FALSE);
	myBtn[86].SetBtnType(BTN_TYPE_CHECK);

	// Induction Motor
	myBtn[68].SubclassDlgItem(IDC_CHK_68, this);		// Induction Motor - Recoiler\r역방향 ON
	myBtn[68].SetHwnd(this->GetSafeHwnd(), IDC_CHK_68);
	myBtn[68].SetBoarder(FALSE);
	myBtn[68].SetBtnType(BTN_TYPE_CHECK);

	myBtn[69].SubclassDlgItem(IDC_CHK_69, this);		// Induction Motor - Uncoiler\r역방향 ON
	myBtn[69].SetHwnd(this->GetSafeHwnd(), IDC_CHK_69);
	myBtn[69].SetBoarder(FALSE);
	myBtn[69].SetBtnType(BTN_TYPE_CHECK);

	// Core 150mm
	myBtn[70].SubclassDlgItem(IDC_CHK_70, this);		// Core 150mm - Recoiler
	myBtn[70].SetHwnd(this->GetSafeHwnd(), IDC_CHK_70);
	myBtn[70].SetBoarder(FALSE);
	myBtn[70].SetBtnType(BTN_TYPE_CHECK);

	myBtn[71].SubclassDlgItem(IDC_CHK_71, this);		// Core 150mm - Uncoiler
	myBtn[71].SetHwnd(this->GetSafeHwnd(), IDC_CHK_71);
	myBtn[71].SetBoarder(FALSE);
	myBtn[71].SetBtnType(BTN_TYPE_CHECK);

	// Recoiler
	myBtn[4].SubclassDlgItem(IDC_CHK_4, this);			// 연동선택
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4);
	myBtn[4].SetBoarder(FALSE);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_5, this);			// ◀정회전
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_5);
	myBtn[5].SetBoarder(FALSE);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_6, this);			// 역회전▶
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_6);
	myBtn[6].SetBoarder(FALSE);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[41].SubclassDlgItem(IDC_CHK_41, this);		// 제품척\r클램프
	myBtn[41].SetHwnd(this->GetSafeHwnd(), IDC_CHK_41);
	myBtn[41].SetBoarder(FALSE);
	myBtn[41].SetBtnType(BTN_TYPE_CHECK);

	myBtn[42].SubclassDlgItem(IDC_CHK_42, this);		// 제품댄서롤\r상승/하강
	myBtn[42].SetHwnd(this->GetSafeHwnd(), IDC_CHK_42);
	myBtn[42].SetBoarder(FALSE);
	myBtn[42].SetBtnType(BTN_TYPE_CHECK);

	myBtn[43].SubclassDlgItem(IDC_CHK_43, this);		// 제품이음매(좌)\r상승/하강
	myBtn[43].SetHwnd(this->GetSafeHwnd(), IDC_CHK_43);
	myBtn[43].SetBoarder(FALSE);
	myBtn[43].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_7, this);			// 제품이음매(우)\r상승/하강
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_7);
	myBtn[7].SetBoarder(FALSE);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_8, this);			// 제품이음매\r진공
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_8);
	myBtn[8].SetBoarder(FALSE);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[44].SubclassDlgItem(IDC_CHK_44, this);		// 간지 척\r클램프
	myBtn[44].SetHwnd(this->GetSafeHwnd(), IDC_CHK_44);
	myBtn[44].SetBoarder(FALSE);
	myBtn[44].SetBtnType(BTN_TYPE_CHECK);

	myBtn[45].SubclassDlgItem(IDC_CHK_45, this);		// 간지 휠\r◀정회전
	myBtn[45].SetHwnd(this->GetSafeHwnd(), IDC_CHK_45);
	myBtn[45].SetBoarder(FALSE);
	myBtn[45].SetBtnType(BTN_TYPE_CHECK);

	myBtn[46].SubclassDlgItem(IDC_CHK_46, this);		// 간지 휠\r역회전▶
	myBtn[46].SetHwnd(this->GetSafeHwnd(), IDC_CHK_46);
	myBtn[46].SetBoarder(FALSE);
	myBtn[46].SetBtnType(BTN_TYPE_DEFAULT);

	myBtn[47].SubclassDlgItem(IDC_CHK_47, this);		// Recoiler-정지
	myBtn[47].SetHwnd(this->GetSafeHwnd(), IDC_CHK_47);
	myBtn[47].SetBoarder(FALSE);
	myBtn[47].SetBtnType(BTN_TYPE_CHECK);

	myBtn[66].SubclassDlgItem(IDC_CHK_66, this);		// Rewinder\r동작
	myBtn[66].SetHwnd(this->GetSafeHwnd(), IDC_CHK_66);
	myBtn[66].SetBoarder(FALSE);
	myBtn[66].SetBtnType(BTN_TYPE_CHECK);

	myBtn[67].SubclassDlgItem(IDC_CHK_67, this);		// Rewinder\r제품&&간지
	myBtn[67].SetHwnd(this->GetSafeHwnd(), IDC_CHK_67);
	myBtn[67].SetBoarder(FALSE);
	myBtn[67].SetBtnType(BTN_TYPE_CHECK);

	// Punching
	myBtn[9].SubclassDlgItem(IDC_CHK_9, this);			// 연동선택
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_9);
	myBtn[9].SetBoarder(FALSE);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	myBtn[10].SubclassDlgItem(IDC_CHK_10, this);		// ◀정회전
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_10);
	myBtn[10].SetBoarder(FALSE);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_11, this);		// 역회전▶
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_11);
	myBtn[11].SetBoarder(FALSE);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_12, this);		// 피딩진공
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_12);
	myBtn[12].SetBoarder(FALSE);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_13, this);		// 제품푸쉬
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_13);
	myBtn[13].SetBoarder(FALSE);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_14, this);		// 테이블\r블로워
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_14);
	myBtn[14].SetBoarder(FALSE);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_CHK_15, this);		// 테이블\r진공
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_15);
	myBtn[15].SetBoarder(FALSE);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);


	myBtn[51].SubclassDlgItem(IDC_CHK_51, this);		// 피딩\r클램프
	myBtn[51].SetHwnd(this->GetSafeHwnd(), IDC_CHK_51);
	myBtn[51].SetBoarder(FALSE);
	myBtn[51].SetBtnType(BTN_TYPE_CHECK);

	myBtn[52].SubclassDlgItem(IDC_CHK_52, this);		// 텐션\r클램프
	myBtn[52].SetHwnd(this->GetSafeHwnd(), IDC_CHK_52);
	myBtn[52].SetBoarder(FALSE);
	myBtn[52].SetBtnType(BTN_TYPE_CHECK);

	myBtn[16].SubclassDlgItem(IDC_CHK_16, this);		// 한판넬\r이송
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_16);
	myBtn[16].SetBoarder(FALSE);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[49].SubclassDlgItem(IDC_CHK_49, this);		// 레이져\r마크
	myBtn[49].SetHwnd(this->GetSafeHwnd(), IDC_CHK_49);
	myBtn[49].SetBoarder(FALSE);
	myBtn[49].SetBtnType(BTN_TYPE_CHECK);

	myBtn[48].SubclassDlgItem(IDC_CHK_48, this);		// 댄서롤\r상승 / 하강
	myBtn[48].SetHwnd(this->GetSafeHwnd(), IDC_CHK_48);
	myBtn[48].SetBoarder(FALSE);
	myBtn[48].SetBtnType(BTN_TYPE_CHECK);


	// AOI 상면.	
	myBtn[17].SubclassDlgItem(IDC_CHK_17, this);		// 연동선택
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_17);
	myBtn[17].SetBoarder(FALSE);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_CHK_18, this);		// ◀정회전
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_18);
	myBtn[18].SetBoarder(FALSE);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);

	myBtn[19].SubclassDlgItem(IDC_CHK_19, this);		// 역회전▶
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_19);
	myBtn[19].SetBoarder(FALSE);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);

	myBtn[20].SubclassDlgItem(IDC_CHK_20, this);		// 피딩진공
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_20);
	myBtn[20].SetBoarder(FALSE);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_21, this);		// 제품푸쉬
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_21);
	myBtn[21].SetBoarder(FALSE);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_CHK_22, this);		// 테이블\r블로워
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_22);
	myBtn[22].SetBoarder(FALSE);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);

	myBtn[23].SubclassDlgItem(IDC_CHK_23, this);		// 테이블\r진공
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_23);
	myBtn[23].SetBoarder(FALSE);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);

	myBtn[53].SubclassDlgItem(IDC_CHK_53, this);		// 피딩\r클램프
	myBtn[53].SetHwnd(this->GetSafeHwnd(), IDC_CHK_53);
	myBtn[53].SetBoarder(FALSE);
	myBtn[53].SetBtnType(BTN_TYPE_CHECK);

	myBtn[54].SubclassDlgItem(IDC_CHK_54, this);		// 텐션\r클램프
	myBtn[54].SetHwnd(this->GetSafeHwnd(), IDC_CHK_54);
	myBtn[54].SetBoarder(FALSE);
	myBtn[54].SetBtnType(BTN_TYPE_CHECK);

	myBtn[24].SubclassDlgItem(IDC_CHK_24, this);		// 한판넬\r이송
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_24);
	myBtn[24].SetBoarder(FALSE);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);

	myBtn[50].SubclassDlgItem(IDC_CHK_50, this);		// 레이져\r마크
	myBtn[50].SetHwnd(this->GetSafeHwnd(), IDC_CHK_50);
	myBtn[50].SetBoarder(FALSE);
	myBtn[50].SetBtnType(BTN_TYPE_CHECK);


	// Uncoiler
	myBtn[25].SubclassDlgItem(IDC_CHK_25, this);		// 연동선택
	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_25);
	myBtn[25].SetBoarder(FALSE);
	myBtn[25].SetBtnType(BTN_TYPE_CHECK);

	myBtn[26].SubclassDlgItem(IDC_CHK_26, this);		// ◀정회전
	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_26);
	myBtn[26].SetBoarder(FALSE);
	myBtn[26].SetBtnType(BTN_TYPE_CHECK);

	myBtn[27].SubclassDlgItem(IDC_CHK_27, this);		// 역회전▶
	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_27);
	myBtn[27].SetBoarder(FALSE);
	myBtn[27].SetBtnType(BTN_TYPE_CHECK);

	myBtn[28].SubclassDlgItem(IDC_CHK_28, this);		// 제품댄서롤\r상승/하강
	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_CHK_28);
	myBtn[28].SetBoarder(FALSE);
	myBtn[28].SetBtnType(BTN_TYPE_CHECK);

	myBtn[29].SubclassDlgItem(IDC_CHK_29, this);		// 클린롤러\r상승/하강
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_29);
	myBtn[29].SetBoarder(FALSE);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);

	myBtn[30].SubclassDlgItem(IDC_CHK_30, this);		// 제품이음매(좌)\r상승/하강
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_30);
	myBtn[30].SetBoarder(FALSE);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);

	myBtn[31].SubclassDlgItem(IDC_CHK_31, this);		// 간지 척\r클램프
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_31);
	myBtn[31].SetBoarder(FALSE);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);

	myBtn[32].SubclassDlgItem(IDC_CHK_32, this);		// 간지 휠\r◀정회전
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_32);
	myBtn[32].SetBoarder(FALSE);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);

	myBtn[35].SubclassDlgItem(IDC_CHK_35, this);		// 제품척\r클램프
	myBtn[35].SetHwnd(this->GetSafeHwnd(), IDC_CHK_35);
	myBtn[35].SetBoarder(FALSE);
	myBtn[35].SetBtnType(BTN_TYPE_CHECK);

	myBtn[36].SubclassDlgItem(IDC_CHK_36, this);		// 클린롤러누름\r상승/하강
	myBtn[36].SetHwnd(this->GetSafeHwnd(), IDC_CHK_36);
	myBtn[36].SetBoarder(FALSE);
	myBtn[36].SetBtnType(BTN_TYPE_CHECK);

	myBtn[37].SubclassDlgItem(IDC_CHK_37, this);		// 제품이음매(우)\r상승/하강
	myBtn[37].SetHwnd(this->GetSafeHwnd(), IDC_CHK_37);
	myBtn[37].SetBoarder(FALSE);
	myBtn[37].SetBtnType(BTN_TYPE_CHECK);

	myBtn[38].SubclassDlgItem(IDC_CHK_38, this);		// 제품이음매\r진공
	myBtn[38].SetHwnd(this->GetSafeHwnd(), IDC_CHK_38);
	myBtn[38].SetBoarder(FALSE);
	myBtn[38].SetBtnType(BTN_TYPE_CHECK);

	myBtn[39].SubclassDlgItem(IDC_CHK_39, this);		// 간지 휠\r역회전▶
	myBtn[39].SetHwnd(this->GetSafeHwnd(), IDC_CHK_39);
	myBtn[39].SetBoarder(FALSE);
	myBtn[39].SetBtnType(BTN_TYPE_CHECK);

	myBtn[40].SubclassDlgItem(IDC_CHK_40, this);		// Uncoiler-정지
	myBtn[40].SetHwnd(this->GetSafeHwnd(), IDC_CHK_40);
	myBtn[40].SetBoarder(FALSE);
	myBtn[40].SetBtnType(BTN_TYPE_CHECK);


	// AOI 하면.
	myBtn[55].SubclassDlgItem(IDC_CHK_55, this);		// 연동선택
	myBtn[55].SetHwnd(this->GetSafeHwnd(), IDC_CHK_55);
	myBtn[55].SetBoarder(FALSE);
	myBtn[55].SetBtnType(BTN_TYPE_CHECK);

	myBtn[56].SubclassDlgItem(IDC_CHK_56, this);		// ◀정회전
	myBtn[56].SetHwnd(this->GetSafeHwnd(), IDC_CHK_56);
	myBtn[56].SetBoarder(FALSE);
	myBtn[56].SetBtnType(BTN_TYPE_CHECK);

	myBtn[57].SubclassDlgItem(IDC_CHK_57, this);		// 역회전▶
	myBtn[57].SetHwnd(this->GetSafeHwnd(), IDC_CHK_57);
	myBtn[57].SetBoarder(FALSE);
	myBtn[57].SetBtnType(BTN_TYPE_CHECK);

	myBtn[58].SubclassDlgItem(IDC_CHK_58, this);		// 피딩진공
	myBtn[58].SetHwnd(this->GetSafeHwnd(), IDC_CHK_58);
	myBtn[58].SetBoarder(FALSE);
	myBtn[58].SetBtnType(BTN_TYPE_CHECK);

	myBtn[59].SubclassDlgItem(IDC_CHK_59, this);		// 제품푸쉬
	myBtn[59].SetHwnd(this->GetSafeHwnd(), IDC_CHK_59);
	myBtn[59].SetBoarder(FALSE);
	myBtn[59].SetBtnType(BTN_TYPE_CHECK);

	myBtn[60].SubclassDlgItem(IDC_CHK_60, this);		// 테이블\r블로워
	myBtn[60].SetHwnd(this->GetSafeHwnd(), IDC_CHK_60);
	myBtn[60].SetBoarder(FALSE);
	myBtn[60].SetBtnType(BTN_TYPE_CHECK);

	myBtn[61].SubclassDlgItem(IDC_CHK_61, this);		// 테이블\r진공
	myBtn[61].SetHwnd(this->GetSafeHwnd(), IDC_CHK_61);
	myBtn[61].SetBoarder(FALSE);
	myBtn[61].SetBtnType(BTN_TYPE_CHECK);

	myBtn[62].SubclassDlgItem(IDC_CHK_64, this);		// 피딩\r클램프
	myBtn[62].SetHwnd(this->GetSafeHwnd(), IDC_CHK_64);
	myBtn[62].SetBoarder(FALSE);
	myBtn[62].SetBtnType(BTN_TYPE_CHECK);

	myBtn[63].SubclassDlgItem(IDC_CHK_65, this);		// 텐션\r클램프
	myBtn[63].SetHwnd(this->GetSafeHwnd(), IDC_CHK_65);
	myBtn[63].SetBoarder(FALSE);
	myBtn[63].SetBtnType(BTN_TYPE_CHECK);

	myBtn[64].SubclassDlgItem(IDC_CHK_62, this);		// 한판넬\r이송
	myBtn[64].SetHwnd(this->GetSafeHwnd(), IDC_CHK_62);
	myBtn[64].SetBoarder(FALSE);
	myBtn[64].SetBtnType(BTN_TYPE_CHECK);

	myBtn[65].SubclassDlgItem(IDC_CHK_63, this);		// 레이져\r마크
	myBtn[65].SetHwnd(this->GetSafeHwnd(), IDC_CHK_63);
	myBtn[65].SetBoarder(FALSE);
	myBtn[65].SetBtnType(BTN_TYPE_CHECK);


	// Engraving
	myBtn[72].SubclassDlgItem(IDC_CHK_72, this);		// 연동선택
	myBtn[72].SetHwnd(this->GetSafeHwnd(), IDC_CHK_72);
	myBtn[72].SetBoarder(FALSE);
	myBtn[72].SetBtnType(BTN_TYPE_CHECK);

	myBtn[73].SubclassDlgItem(IDC_CHK_73, this);		// ◀정회전
	myBtn[73].SetHwnd(this->GetSafeHwnd(), IDC_CHK_73);
	myBtn[73].SetBoarder(FALSE);
	myBtn[73].SetBtnType(BTN_TYPE_CHECK);

	myBtn[74].SubclassDlgItem(IDC_CHK_74, this);		// 역회전▶
	myBtn[74].SetHwnd(this->GetSafeHwnd(), IDC_CHK_74);
	myBtn[74].SetBoarder(FALSE);
	myBtn[74].SetBtnType(BTN_TYPE_CHECK);

	myBtn[75].SubclassDlgItem(IDC_CHK_75, this);		// 피딩진공
	myBtn[75].SetHwnd(this->GetSafeHwnd(), IDC_CHK_75);
	myBtn[75].SetBoarder(FALSE);
	myBtn[75].SetBtnType(BTN_TYPE_CHECK);

	myBtn[76].SubclassDlgItem(IDC_CHK_76, this);		// 제품푸쉬
	myBtn[76].SetHwnd(this->GetSafeHwnd(), IDC_CHK_76);
	myBtn[76].SetBoarder(FALSE);
	myBtn[76].SetBtnType(BTN_TYPE_CHECK);

	myBtn[77].SubclassDlgItem(IDC_CHK_77, this);		// 테이블\r블로워
	myBtn[77].SetHwnd(this->GetSafeHwnd(), IDC_CHK_77);
	myBtn[77].SetBoarder(FALSE);
	myBtn[77].SetBtnType(BTN_TYPE_CHECK);

	myBtn[78].SubclassDlgItem(IDC_CHK_78, this);		// 테이블\r진공
	myBtn[78].SetHwnd(this->GetSafeHwnd(), IDC_CHK_78);
	myBtn[78].SetBoarder(FALSE);
	myBtn[78].SetBtnType(BTN_TYPE_CHECK);

	myBtn[79].SubclassDlgItem(IDC_CHK_82, this);		// 피딩\r클램프
	myBtn[79].SetHwnd(this->GetSafeHwnd(), IDC_CHK_82);
	myBtn[79].SetBoarder(FALSE);
	myBtn[79].SetBtnType(BTN_TYPE_CHECK);

	myBtn[80].SubclassDlgItem(IDC_CHK_83, this);		// 텐션\r클램프
	myBtn[80].SetHwnd(this->GetSafeHwnd(), IDC_CHK_83);
	myBtn[80].SetBoarder(FALSE);
	myBtn[80].SetBtnType(BTN_TYPE_CHECK);

	myBtn[81].SubclassDlgItem(IDC_CHK_79, this);		// 한판넬\r이송
	myBtn[81].SetHwnd(this->GetSafeHwnd(), IDC_CHK_79);
	myBtn[81].SetBoarder(FALSE);
	myBtn[81].SetBtnType(BTN_TYPE_CHECK);

	myBtn[82].SubclassDlgItem(IDC_CHK_81, this);		// 레이져\r마크
	myBtn[82].SetHwnd(this->GetSafeHwnd(), IDC_CHK_81);
	myBtn[82].SetBoarder(FALSE);
	myBtn[82].SetBtnType(BTN_TYPE_CHECK);

	myBtn[84].SubclassDlgItem(IDC_CHK_80, this);		// 댄서롤\r상승/하강
	myBtn[84].SetHwnd(this->GetSafeHwnd(), IDC_CHK_80);
	myBtn[84].SetBoarder(FALSE);
	myBtn[84].SetBtnType(BTN_TYPE_CHECK);

	int i;
	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		myBtn[i].SetFont(_T("굴림체"), 12, TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
	}
}

void CDlgMenu03::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_SIG1, this);
	myLabel[1].SubclassDlgItem(IDC_STC_SIG2, this);
	myLabel[2].SubclassDlgItem(IDC_STC_SIG3, this);
	myLabel[3].SubclassDlgItem(IDC_STC_SIG4, this);
	myLabel[4].SubclassDlgItem(IDC_STC_SIG5, this);

	for (int i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(18);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_DARKRED);
		myLabel[i].SetImageBk(LBL_IMG_UP);
	}
}

void CDlgMenu03::InitStatic()
{
	// Main
	myStcTitle[0].SubclassDlgItem(IDC_STC_000_, this);		// Run
	myStcTitle[1].SubclassDlgItem(IDC_STC_01_, this);		// Reset
	myStcTitle[39].SubclassDlgItem(IDC_STC_000_3, this);	// Stop
	myStcTitle[40].SubclassDlgItem(IDC_STC_000_4, this);	// 운전준비

	myStcTitle[33].SubclassDlgItem(IDC_STC_02, this);		// LED-자동 
	myStcTitle[34].SubclassDlgItem(IDC_STC_03, this);		// LED-1회운전
	myStcTitle[35].SubclassDlgItem(IDC_STC_04, this);		// LED-수동 

	myStcTitle[36].SubclassDlgItem(IDC_STC_02_, this);		// 자동
	myStcTitle[37].SubclassDlgItem(IDC_STC_03_, this);		// 1회운전
	myStcTitle[38].SubclassDlgItem(IDC_STC_04_, this);		// 수동

															// TorqueMotor
	myStcTitle[2].SubclassDlgItem(IDC_STC_000_2, this);		// 펀칭부
	myStcTitle[3].SubclassDlgItem(IDC_STC_01_2, this);		// 검사부
	myStcTitle[78].SubclassDlgItem(IDC_STC_000_9, this);	// 각인부

															// InductionMotor
	myStcTitle[74].SubclassDlgItem(IDC_STC_000_6, this);	// Recoiler역방향
	myStcTitle[75].SubclassDlgItem(IDC_STC_01_3, this);		// Uncoiler역방향

															// Core150mm
	myStcTitle[76].SubclassDlgItem(IDC_STC_000_7, this);	// Recoiler
	myStcTitle[77].SubclassDlgItem(IDC_STC_01_4, this);		// Uncoiler

															// Recoiler
	myStcTitle[4].SubclassDlgItem(IDC_STC_10_, this);		// 연동선택
	myStcTitle[5].SubclassDlgItem(IDC_STC_12_2, this);		// 정회전
	myStcTitle[6].SubclassDlgItem(IDC_STC_12_, this);		// 역회전
	myStcTitle[7].SubclassDlgItem(IDC_STC_12_4, this);		// 제품이음매(우)상승
	myStcTitle[8].SubclassDlgItem(IDC_STC_12_3, this);		// 제품이음매진공
	myStcTitle[47].SubclassDlgItem(IDC_STC_12_32, this);	// 제품척클램프
	myStcTitle[48].SubclassDlgItem(IDC_STC_12_33, this);	// 제품댄서롤상승
	myStcTitle[49].SubclassDlgItem(IDC_STC_12_34, this);	// 제품이음매(좌)상승
	myStcTitle[50].SubclassDlgItem(IDC_STC_12_36, this);	// 간지척클램프
	myStcTitle[51].SubclassDlgItem(IDC_STC_12_35, this);	// 간지휠정회전
	myStcTitle[52].SubclassDlgItem(IDC_STC_12_37, this);	// 간지휠역회전
	myStcTitle[53].SubclassDlgItem(IDC_STC_12_38, this);	// 정지
	myStcTitle[72].SubclassDlgItem(IDC_STC_12_55, this);	// Rewinder동작
	myStcTitle[73].SubclassDlgItem(IDC_STC_12_56, this);	// Rewinder제품간지

															// Punching
	myStcTitle[9].SubclassDlgItem(IDC_STC_20_, this);		// 연동선택
	myStcTitle[10].SubclassDlgItem(IDC_STC_12_6, this);		// 정회전
	myStcTitle[11].SubclassDlgItem(IDC_STC_12_5, this);		// 역회전
	myStcTitle[12].SubclassDlgItem(IDC_STC_12_8, this);		// 피딩진공
	myStcTitle[13].SubclassDlgItem(IDC_STC_12_7, this);		// 제품푸쉬
	myStcTitle[14].SubclassDlgItem(IDC_STC_12_10, this);	// 테이블블로워
	myStcTitle[15].SubclassDlgItem(IDC_STC_12_9, this);		// 테이블진공
	myStcTitle[16].SubclassDlgItem(IDC_STC_12_12, this);	// 한판넬이송
	myStcTitle[54].SubclassDlgItem(IDC_STC_000_5, this);	// 댄서롤상승
	myStcTitle[55].SubclassDlgItem(IDC_STC_12_39, this);	// 레이져마크
	myStcTitle[57].SubclassDlgItem(IDC_STC_12_42, this);	// 피딩클램프
	myStcTitle[58].SubclassDlgItem(IDC_STC_12_41, this);	// 텐션클램프

															// AOI하
	myStcTitle[61].SubclassDlgItem(IDC_STC_30_2, this);		// 연동선택
	myStcTitle[62].SubclassDlgItem(IDC_STC_12_46, this);	// 정회전
	myStcTitle[63].SubclassDlgItem(IDC_STC_12_45, this);	// 역회전
	myStcTitle[64].SubclassDlgItem(IDC_STC_12_48, this);	// 피딩진공
	myStcTitle[65].SubclassDlgItem(IDC_STC_12_47, this);	// 제품푸쉬
	myStcTitle[66].SubclassDlgItem(IDC_STC_12_50, this);	// 테이블블로워
	myStcTitle[67].SubclassDlgItem(IDC_STC_12_49, this);	// 테이블진공
	myStcTitle[68].SubclassDlgItem(IDC_STC_12_54, this);	// 피딩클램프
	myStcTitle[69].SubclassDlgItem(IDC_STC_12_53, this);	// 텐션클램프
	myStcTitle[70].SubclassDlgItem(IDC_STC_12_51, this);	// 한판넬이송
	myStcTitle[71].SubclassDlgItem(IDC_STC_12_52, this);	// 레이져마크

															// AOI상
	myStcTitle[17].SubclassDlgItem(IDC_STC_30_, this);		// 연동선택
	myStcTitle[18].SubclassDlgItem(IDC_STC_12_13, this);	// 정회전
	myStcTitle[19].SubclassDlgItem(IDC_STC_12_11, this);	// 역회전
	myStcTitle[20].SubclassDlgItem(IDC_STC_12_15, this);	// 피딩진공
	myStcTitle[21].SubclassDlgItem(IDC_STC_12_14, this);	// 제품푸쉬
	myStcTitle[22].SubclassDlgItem(IDC_STC_12_17, this);	// 테이블블로워
	myStcTitle[23].SubclassDlgItem(IDC_STC_12_16, this);	// 테이블진공
	myStcTitle[24].SubclassDlgItem(IDC_STC_12_19, this);	// 한판넬이송
	myStcTitle[56].SubclassDlgItem(IDC_STC_12_40, this);	// 레이져마크
	myStcTitle[59].SubclassDlgItem(IDC_STC_12_44, this);	// 피딩클램프
	myStcTitle[60].SubclassDlgItem(IDC_STC_12_43, this);	// 텐션클램프

															// Uncoiler
	myStcTitle[25].SubclassDlgItem(IDC_STC_40_, this);		// 연동선택
	myStcTitle[26].SubclassDlgItem(IDC_STC_12_20, this);	// 정회전
	myStcTitle[27].SubclassDlgItem(IDC_STC_12_18, this);	// 역회전
	myStcTitle[28].SubclassDlgItem(IDC_STC_12_22, this);	// 제품댄서롤상승
	myStcTitle[29].SubclassDlgItem(IDC_STC_12_21, this);	// 클린롤러상승
	myStcTitle[30].SubclassDlgItem(IDC_STC_12_24, this);	// 제품이음매(좌)상승
	myStcTitle[31].SubclassDlgItem(IDC_STC_12_25, this);	// 간지척클램프
	myStcTitle[32].SubclassDlgItem(IDC_STC_12_23, this);	// 간지휠정회전
	myStcTitle[41].SubclassDlgItem(IDC_STC_12_26, this);	// 제품척클램프
	myStcTitle[42].SubclassDlgItem(IDC_STC_12_27, this);	// 클린롤러누름상승
	myStcTitle[43].SubclassDlgItem(IDC_STC_12_28, this);	// 제품이음매(우)상승
	myStcTitle[44].SubclassDlgItem(IDC_STC_12_29, this);	// 제품이음매진공
	myStcTitle[45].SubclassDlgItem(IDC_STC_12_30, this);	// 간지휠역회전
	myStcTitle[46].SubclassDlgItem(IDC_STC_12_31, this);	// 정지

															// Engraving
	myStcTitle[79].SubclassDlgItem(IDC_STC_20_2, this);		// 연동선택
	myStcTitle[80].SubclassDlgItem(IDC_STC_12_58, this);	// 정회전
	myStcTitle[81].SubclassDlgItem(IDC_STC_12_57, this);	// 역회전
	myStcTitle[82].SubclassDlgItem(IDC_STC_12_60, this);	// 피딩진공
	myStcTitle[83].SubclassDlgItem(IDC_STC_12_59, this);	// 제품푸쉬
	myStcTitle[84].SubclassDlgItem(IDC_STC_12_62, this);	// 테이블블로워
	myStcTitle[85].SubclassDlgItem(IDC_STC_12_61, this);	// 테이블진공
	myStcTitle[86].SubclassDlgItem(IDC_STC_12_63, this);	// 한판넬이송
	myStcTitle[87].SubclassDlgItem(IDC_STC_000_8, this);	// 댄서롤상승
	myStcTitle[88].SubclassDlgItem(IDC_STC_12_64, this);	// 레이져마크
	myStcTitle[89].SubclassDlgItem(IDC_STC_12_66, this);	// 피딩클램프
	myStcTitle[90].SubclassDlgItem(IDC_STC_12_65, this);	// 텐션클램프

															// 세정기
	myStcTitle[91].SubclassDlgItem(IDC_STC_000_10, this);	// 각인부 세정기
	myStcTitle[92].SubclassDlgItem(IDC_STC_000_11, this);	// AOI(하) 세정기

	for (int i = 0; i < MAX_MENU03_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_DLG_FRM);
	}
}

BOOL CDlgMenu03::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMenu03::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_pRect)
	{
		m_pRect = new CRect;

		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if (bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu03::AtDlgShow()
{
	//SetTimer(TIM_CHK_MREG, 300, NULL);
	//UpdateSignal();
}

void CDlgMenu03::AtDlgHide()
{
}

void CDlgMenu03::SetLed(int nIdx, BOOL bOn)
{
	if (bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_DN)
		myLabel[nIdx].SetImageBk(LBL_IMG_DN);
	else if (!bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_UP)
		myLabel[nIdx].SetImageBk(LBL_IMG_UP);
}


