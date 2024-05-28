// DlgMenu06.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu06.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu06 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMenu06, CDialog)

CDlgMenu06::CDlgMenu06(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_06, pParent)
{
	m_pRect = NULL;
	Create();
}

CDlgMenu06::~CDlgMenu06()
{
	DelImg();

	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

void CDlgMenu06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu06, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgMenu06 메시지 처리기입니다.

BOOL CDlgMenu06::Create()
{
	return CDialog::Create(CDlgMenu06::IDD);
}

void CDlgMenu06::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU06_BTN; i++)
	{
		if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8
			|| i == 9 || i == 10 || i == 11 || i == 15 || i == 21 || i == 22)
			continue;

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

void CDlgMenu06::DelImg()
{
	int i;
	for (i = 0; i < MAX_MENU06_BTN; i++)
	{
		if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8
			|| i == 9 || i == 10 || i == 11 || i == 15 || i == 21 || i == 22)
			continue;


		myBtn[i].DelImgList();
	}
}

void CDlgMenu06::InitGui()
{
	InitStatic();
	InitBtn();

	LoadImg();
}

void CDlgMenu06::InitBtn()
{
	//myBtn[0].SubclassDlgItem(IDC_CHK_TP_STOP, this);
	//myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_TP_STOP);
	//myBtn[0].SetBtnType(BTN_TYPE_CHECK);
	//myBtn[0].SetText(_T("On"), BTN_DN);
	//myBtn[0].SetText(_T("Off"), BTN_UP);

	//myBtn[1].SubclassDlgItem(IDC_CHK_USER_INFO, this);
	//myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USER_INFO);
	//myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[2].SubclassDlgItem(IDC_CHK_REVIEW, this);
	//myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REVIEW);
	//myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[3].SubclassDlgItem(IDC_CHK_EJECT_BUFFER, this);
	//myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_EJECT_BUFFER);
	//myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[4].SubclassDlgItem(IDC_CHK_REMARKING, this);
	//myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REMARKING);
	//myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[5].SubclassDlgItem(IDC_CHK_REVIEW_MOVE, this);
	//myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REVIEW_MOVE);
	//myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[6].SubclassDlgItem(IDC_CHK_LOT_END, this);
	//myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LOT_END);
	//myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[7].SubclassDlgItem(IDC_CHK_MK_1, this);
	//myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_1);
	//myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[8].SubclassDlgItem(IDC_CHK_MK_2, this);
	//myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_2);
	//myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[9].SubclassDlgItem(IDC_CHK_MK_3, this);
	//myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_3);
	//myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[10].SubclassDlgItem(IDC_CHK_MK_4, this);
	//myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_4);
	//myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[11].SubclassDlgItem(IDC_BTN_MK_ALL, this);
	//myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_ALL);

	myBtn[12].SubclassDlgItem(IDC_CHK_DEF_UP, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_DEF_UP);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_DEF_DN, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_DEF_DN);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_DEF_ALL, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_DEF_ALL);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[15].SubclassDlgItem(IDC_BTN_ST, this);
	//myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ST);

	myBtn[16].SubclassDlgItem(IDC_BTN_GD_RA_1, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_1);

	myBtn[17].SubclassDlgItem(IDC_BTN_GD_RA_2, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_2);

	myBtn[18].SubclassDlgItem(IDC_BTN_GD_RA_3, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_3);

	myBtn[19].SubclassDlgItem(IDC_BTN_GD_RA_4, this);
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_4);

	myBtn[20].SubclassDlgItem(IDC_BTN_GD_RA_ALL, this);
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_ALL);

	//myBtn[21].SubclassDlgItem(IDC_CHK_ELEC, this);
	//myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ELEC);
	//myBtn[21].SetBtnType(BTN_TYPE_CHECK);
	//myBtn[21].ShowWindow(SW_HIDE);

	//myBtn[22].SubclassDlgItem(IDC_CHK_ELEC_STOP, this);
	//myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ELEC_STOP);
	//myBtn[22].SetBtnType(BTN_TYPE_CHECK);
	//myBtn[22].ShowWindow(SW_HIDE);

	int i;
	for (i = 0; i < MAX_MENU06_BTN; i++)
	{
		if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 7 || i == 8
			|| i == 9 || i == 10 || i == 11 || i == 15 || i == 21 || i == 22)
			continue;

		if (i == 1 || i == 2)
			myBtn[i].SetFont(_T("굴림체"), 12, TRUE);
		else if (i == 7 || i == 8 || i == 9 || i == 10 || i == 11)
			myBtn[i].SetFont(_T("굴림체"), 11, TRUE);
		else if (i == 12 || i == 13 || i == 14)
			myBtn[i].SetFont(_T("굴림체"), 12, TRUE);
		else if (i == 15 || i == 16 || i == 17 || i == 18 || i == 19 || i == 20 || i == 21)
		{
			myBtn[i].SetFont(_T("굴림체"), 11, TRUE);
		}
		else
			myBtn[i].SetFont(_T("굴림체"), 12, TRUE);

		if (i == 15 || i == 20)
			myBtn[i].SetTextColor(RGB_WHITE);
		else
			myBtn[i].SetTextColor(RGB_BLACK);

		myBtn[i].SetBtnType(BTN_TYPE_CHECK);
	}
}

void CDlgMenu06::InitStatic()
{
	myStcReelmap.SubclassDlgItem(IDC_STC_REELMAP_INNER, this);
	myStcReelmap.SetHwnd(this->GetSafeHwnd(), IDC_STC_REELMAP_INNER);

	InitStcDefInf();
	InitStcTitle();
	InitStcData();
}

void CDlgMenu06::InitStcDefInf()
{
	myStcDefInfo[0].SubclassDlgItem(IDC_EDIT_DEF_INFO_001, this);
	myStcDefInfo[1].SubclassDlgItem(IDC_EDIT_DEF_INFO_002, this);
	myStcDefInfo[2].SubclassDlgItem(IDC_EDIT_DEF_INFO_003, this);
	myStcDefInfo[3].SubclassDlgItem(IDC_EDIT_DEF_INFO_004, this);
	myStcDefInfo[4].SubclassDlgItem(IDC_EDIT_DEF_INFO_005, this);
	myStcDefInfo[5].SubclassDlgItem(IDC_EDIT_DEF_INFO_006, this);
	myStcDefInfo[6].SubclassDlgItem(IDC_EDIT_DEF_INFO_007, this);
	myStcDefInfo[7].SubclassDlgItem(IDC_EDIT_DEF_INFO_008, this);
	myStcDefInfo[8].SubclassDlgItem(IDC_EDIT_DEF_INFO_009, this);
	myStcDefInfo[9].SubclassDlgItem(IDC_EDIT_DEF_INFO_010, this);
	myStcDefInfo[10].SubclassDlgItem(IDC_EDIT_DEF_INFO_011, this);
	myStcDefInfo[11].SubclassDlgItem(IDC_EDIT_DEF_INFO_012, this);

	for (int i = 0; i < MAX_MENU06_STC_DEFINFO; i++)
	{
		myStcDefInfo[i].SetFontName(_T("Arial"));
		myStcDefInfo[i].SetFontSize(12);
		myStcDefInfo[i].SetFontBold(FALSE);
		myStcDefInfo[i].SetTextColor(RGB_BLACK);
		myStcDefInfo[i].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu06::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_OPERATOR, this);
	myStcData[1].SubclassDlgItem(IDC_STC_MODEL, this);
	myStcData[2].SubclassDlgItem(IDC_STC_LAYER, this);
	myStcData[3].SubclassDlgItem(IDC_STC_LOT, this);
	//myStcData[4].SubclassDlgItem(IDC_STC_TOTAL_REEL, this);
	//myStcData[5].SubclassDlgItem(IDC_STC_WORK_RATIO, this);
	//myStcData[6].SubclassDlgItem(IDC_STC_LOT_RATIO, this);
	//myStcData[7].SubclassDlgItem(IDC_STC_TOTAL_SPEED, this);
	//myStcData[8].SubclassDlgItem(IDC_STC_CYCLE_SPEED, this);
	//myStcData[9].SubclassDlgItem(IDC_STC_TEMP_STOP, this);
	//myStcData[10].SubclassDlgItem(IDC_STC_LOT_SEP, this);
	//myStcData[11].SubclassDlgItem(IDC_STC_LOT_LEN, this);
	//myStcData[12].SubclassDlgItem(IDC_STC_MK, this);
	//myStcData[13].SubclassDlgItem(IDC_STC_AOI_DN, this);
	//myStcData[14].SubclassDlgItem(IDC_STC_LOT_SRL, this);
	myStcData[15].SubclassDlgItem(IDC_STC_DEFECT_NUM_UP, this);
	myStcData[16].SubclassDlgItem(IDC_STC_DEFECT_RATIO_UP, this);
	myStcData[17].SubclassDlgItem(IDC_STC_GOOD_NUM_UP, this);
	myStcData[18].SubclassDlgItem(IDC_STC_GOOD_RATIO_UP, this);
	myStcData[19].SubclassDlgItem(IDC_STC_TOTAL_NUM_UP, this);
	//myStcData[20].SubclassDlgItem(IDC_STC_REVIEW_LEN, this);
	myStcData[21].SubclassDlgItem(IDC_STC_LOT_START, this);
	myStcData[22].SubclassDlgItem(IDC_STC_LOT_RUN, this);
	myStcData[23].SubclassDlgItem(IDC_STC_LOT_END, this);
	myStcData[24].SubclassDlgItem(IDC_STC_DEF_OPEN, this);
	myStcData[25].SubclassDlgItem(IDC_STC_DEF_SHORT, this);
	myStcData[26].SubclassDlgItem(IDC_STC_DEF_U_SHORT, this);
	myStcData[27].SubclassDlgItem(IDC_STC_DEF_SPACE, this);
	myStcData[28].SubclassDlgItem(IDC_STC_DEF_EXTRA, this);
	myStcData[29].SubclassDlgItem(IDC_STC_DEF_PROT, this);
	myStcData[30].SubclassDlgItem(IDC_STC_DEF_P_HOLE, this);
	myStcData[31].SubclassDlgItem(IDC_STC_DEF_PAD, this);
	myStcData[32].SubclassDlgItem(IDC_STC_DEF_H_OPEN, this);
	myStcData[33].SubclassDlgItem(IDC_STC_DEF_H_MISS, this);
	myStcData[34].SubclassDlgItem(IDC_STC_DEF_H_POS, this);
	myStcData[35].SubclassDlgItem(IDC_STC_DEF_H_DEF, this);
	myStcData[36].SubclassDlgItem(IDC_STC_DEF_NICK, this);
	myStcData[37].SubclassDlgItem(IDC_STC_DEF_POI, this);
	myStcData[38].SubclassDlgItem(IDC_STC_DEF_VH_OPEN, this);
	myStcData[39].SubclassDlgItem(IDC_STC_DEF_VH_MISS, this);
	myStcData[40].SubclassDlgItem(IDC_STC_DEF_VH_POS, this);
	myStcData[41].SubclassDlgItem(IDC_STC_DEF_VH_DEF, this);
	myStcData[42].SubclassDlgItem(IDC_STC_DEF_LIGHT, this);
	myStcData[43].SubclassDlgItem(IDC_STC_DEF_E_NICK, this);
	myStcData[44].SubclassDlgItem(IDC_STC_DEF_E_PROT, this);
	myStcData[45].SubclassDlgItem(IDC_STC_DEF_E_SPACE, this);
	myStcData[46].SubclassDlgItem(IDC_STC_DEF_UDEF1, this);
	myStcData[47].SubclassDlgItem(IDC_STC_DEF_NARROW, this);
	myStcData[48].SubclassDlgItem(IDC_STC_DEF_WIDE, this);

	myStcData[49].SubclassDlgItem(IDC_STC_DEFECT_NUM_DN, this);
	myStcData[50].SubclassDlgItem(IDC_STC_DEFECT_RATIO_DN, this);
	myStcData[51].SubclassDlgItem(IDC_STC_GOOD_NUM_DN, this);
	myStcData[52].SubclassDlgItem(IDC_STC_GOOD_RATIO_DN, this);
	myStcData[53].SubclassDlgItem(IDC_STC_TOTAL_NUM_DN, this);

	myStcData[54].SubclassDlgItem(IDC_STC_DEFECT_NUM_ALL, this);
	myStcData[55].SubclassDlgItem(IDC_STC_DEFECT_RATIO_ALL, this);
	myStcData[56].SubclassDlgItem(IDC_STC_GOOD_NUM_ALL, this);
	myStcData[57].SubclassDlgItem(IDC_STC_GOOD_RATIO_ALL, this);
	myStcData[58].SubclassDlgItem(IDC_STC_TOTAL_NUM_ALL, this);

	myStcData[59].SubclassDlgItem(IDC_STC_GD_RA_1_UP, this);
	myStcData[60].SubclassDlgItem(IDC_STC_GD_RA_2_UP, this);
	myStcData[61].SubclassDlgItem(IDC_STC_GD_RA_3_UP, this);
	myStcData[62].SubclassDlgItem(IDC_STC_GD_RA_4_UP, this);
	myStcData[63].SubclassDlgItem(IDC_STC_GD_RA_ALL_UP, this);

	myStcData[64].SubclassDlgItem(IDC_STC_GD_RA_1_DN, this);
	myStcData[65].SubclassDlgItem(IDC_STC_GD_RA_2_DN, this);
	myStcData[66].SubclassDlgItem(IDC_STC_GD_RA_3_DN, this);
	myStcData[67].SubclassDlgItem(IDC_STC_GD_RA_4_DN, this);
	myStcData[68].SubclassDlgItem(IDC_STC_GD_RA_ALL_DN, this);

	myStcData[69].SubclassDlgItem(IDC_STC_GD_RA_1_ALL, this);
	myStcData[70].SubclassDlgItem(IDC_STC_GD_RA_2_ALL, this);
	myStcData[71].SubclassDlgItem(IDC_STC_GD_RA_3_ALL, this);
	myStcData[72].SubclassDlgItem(IDC_STC_GD_RA_4_ALL, this);
	myStcData[73].SubclassDlgItem(IDC_STC_GD_RA_ALL_ALL, this);

	//myStcData[74].SubclassDlgItem(IDC_STC_AOI_UP, this);
	myStcData[75].SubclassDlgItem(IDC_STC_LAYER2, this);

	myStcData[76].SubclassDlgItem(IDC_STC_DEF_SHORT_TOT, this);
	myStcData[77].SubclassDlgItem(IDC_STC_DEF_SPACE_TOT, this);
	myStcData[78].SubclassDlgItem(IDC_STC_DEF_P_HOLE_TOT, this);
	myStcData[79].SubclassDlgItem(IDC_STC_DEF_H_MISS_TOT, this);
	myStcData[80].SubclassDlgItem(IDC_STC_DEF_VH_MISS_TOT, this);

	//myStcData[81].SubclassDlgItem(IDC_STC_MK_CNT_L, this);
	//myStcData[82].SubclassDlgItem(IDC_STC_MK_CNT_R, this);

	//myStcData[83].SubclassDlgItem(IDC_STC_ENG, this);

	for (int i = 0; i < MAX_MENU06_STC_DATA; i++)
	{
		if (i == 4 || i == 5 || i == 6 || i == 7 || i == 8 || i == 9 || i == 10 || i == 11 || i == 12
			|| i == 13 || i == 14 || i == 20 || i == 74 || i == 81 || i == 82 || i == 83)
			continue;

		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(12);
		myStcData[i].SetFontBold(FALSE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

void CDlgMenu06::InitStcTitle()
{
	int i;

	myStcTitle[0].SubclassDlgItem(IDC_STC_OP, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_MDL, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_LAY, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_LT, this);
	for (i = 0; i <= 3; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKBLUE);
	}

	//myStcTitle[4].SubclassDlgItem(IDC_STC_TOT_REEL, this);
	//myStcTitle[5].SubclassDlgItem(IDC_STC_WK_RA, this);
	//myStcTitle[6].SubclassDlgItem(IDC_STC_LT_RA, this);
	//myStcTitle[7].SubclassDlgItem(IDC_STC_TOT_SPD, this);
	//myStcTitle[8].SubclassDlgItem(IDC_STC_CY_SPD, this);
	//myStcTitle[9].SubclassDlgItem(IDC_STC_TP_STOP, this);
	//myStcTitle[10].SubclassDlgItem(IDC_STC_LT_SEP, this);
	//myStcTitle[11].SubclassDlgItem(IDC_STC_LT_LEN, this);
	//myStcTitle[12].SubclassDlgItem(IDC_STC_MKAOI, this);
	//myStcTitle[13].SubclassDlgItem(IDC_STC_LT_SRL, this);
	//for (i = 4; i <= 13; i++)
	//{
	//	myStcTitle[i].SetFontName(_T("Arial"));
	//	myStcTitle[i].SetFontSize(12);
	//	myStcTitle[i].SetFontBold(TRUE);
	//	myStcTitle[i].SetTextColor(RGB_BLACK);
	//	myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	//}

	myStcTitle[14].SubclassDlgItem(IDC_STC_WORK_INFO, this);
	myStcTitle[14].SetFontName(_T("Arial"));
	myStcTitle[14].SetFontSize(60);
	myStcTitle[14].SetFontBold(TRUE);
	myStcTitle[14].SetTextColor(RGB_RED);
	myStcTitle[14].SetBkColor(RGB_BLACK);

	myStcTitle[15].SubclassDlgItem(IDC_STC_DEF_NUM, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_DEF_RT, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_GD_NUM, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_GD_RT, this);
	for (i = 15; i <= 18; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
		myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	}

	myStcTitle[19].SubclassDlgItem(IDC_STC_TOT_NUM, this);
	myStcTitle[19].SetFontName(_T("Arial"));
	myStcTitle[19].SetFontSize(12);
	myStcTitle[19].SetFontBold(TRUE);
	myStcTitle[19].SetTextColor(RGB_WHITE);
	myStcTitle[19].SetBkColor(RGB_DARKBLUE);

	myStcTitle[20].SubclassDlgItem(IDC_STC_LOT_ST, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_LOT_RN, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_LOT_ED, this);
	for (i = 20; i <= 22; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKGREEN);
	}

	myStcTitle[23].SubclassDlgItem(IDC_STC_DEF_OP, this);
	myStcTitle[23].SetBkColor(RGB_LTDKORANGE);
	myStcTitle[24].SubclassDlgItem(IDC_STC_DEF_SH, this);
	myStcTitle[24].SetBkColor(RGB_RED);
	myStcTitle[25].SubclassDlgItem(IDC_STC_DEF_USH, this);
	myStcTitle[25].SetBkColor(RGB_SKYBLUE);
	myStcTitle[26].SubclassDlgItem(IDC_STC_DEF_SP, this);
	myStcTitle[26].SetBkColor(RGB_PURPLE);
	myStcTitle[27].SubclassDlgItem(IDC_STC_DEF_EX, this);
	myStcTitle[27].SetBkColor(RGB_DARKGREEN);
	myStcTitle[28].SubclassDlgItem(IDC_STC_DEF_PR, this);
	myStcTitle[28].SetBkColor(RGB_BLUE);
	myStcTitle[29].SubclassDlgItem(IDC_STC_DEF_PHO, this);
	myStcTitle[29].SetBkColor(RGB_LTBLUE);
	myStcTitle[30].SubclassDlgItem(IDC_STC_DEF_PD, this);
	myStcTitle[30].SetBkColor(RGB_LTGREEN);
	myStcTitle[31].SubclassDlgItem(IDC_STC_DEF_HOP, this);
	myStcTitle[31].SetBkColor(RGB_PINK);
	myStcTitle[32].SubclassDlgItem(IDC_STC_DEF_HMS, this);
	myStcTitle[32].SetBkColor(RGB_GREEN);
	myStcTitle[33].SubclassDlgItem(IDC_STC_DEF_HPS, this);
	myStcTitle[33].SetBkColor(RGB_DKSKYBLUE);
	myStcTitle[34].SubclassDlgItem(IDC_STC_DEF_HDF, this);
	myStcTitle[34].SetBkColor(RGB_LTPINK);
	myStcTitle[35].SubclassDlgItem(IDC_STC_DEF_NK, this);
	myStcTitle[35].SetBkColor(RGB_CYAN);
	myStcTitle[36].SubclassDlgItem(IDC_STC_DEF_PO, this);
	myStcTitle[36].SetBkColor(RGB_DARKRED);
	myStcTitle[37].SubclassDlgItem(IDC_STC_DEF_VHO, this);
	myStcTitle[37].SetBkColor(RGB_DARKCYAN);
	myStcTitle[38].SubclassDlgItem(IDC_STC_DEF_VHMS, this);
	myStcTitle[38].SetBkColor(RGB_YELLOW);
	myStcTitle[39].SubclassDlgItem(IDC_STC_DEF_VHP, this);
	myStcTitle[39].SetBkColor(RGB_ORANGE);
	myStcTitle[40].SubclassDlgItem(IDC_STC_DEF_VHD, this);
	myStcTitle[40].SetBkColor(RGB_YELLOWGREEN);
	myStcTitle[41].SubclassDlgItem(IDC_STC_DEF_LTD, this);
	myStcTitle[41].SetBkColor(RGB_LTYELLOW);

	myStcTitle[42].SubclassDlgItem(IDC_STC_DEF_ENK, this);
	myStcTitle[42].SetBkColor(RGB_LTYELLOW);
	myStcTitle[43].SubclassDlgItem(IDC_STC_DEF_EPR, this);
	myStcTitle[43].SetBkColor(RGB_LTYELLOW);
	myStcTitle[44].SubclassDlgItem(IDC_STC_DEF_ESP, this);
	myStcTitle[44].SetBkColor(RGB_LTYELLOW);
	myStcTitle[45].SubclassDlgItem(IDC_STC_DEF_UDD1, this);
	myStcTitle[45].SetBkColor(RGB_LTYELLOW);
	myStcTitle[46].SubclassDlgItem(IDC_STC_DEF_NRW, this);
	myStcTitle[46].SetBkColor(RGB_LTYELLOW);
	myStcTitle[47].SubclassDlgItem(IDC_STC_DEF_WD, this);
	myStcTitle[47].SetBkColor(RGB_LTYELLOW);

	for (i = 23; i < 48; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(FALSE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
	}

	myStcTitle[48].SubclassDlgItem(IDC_STC_WK, this);
	myStcTitle[49].SubclassDlgItem(IDC_STC_WK_UP, this);
	myStcTitle[50].SubclassDlgItem(IDC_STC_WK_DN, this);
	myStcTitle[51].SubclassDlgItem(IDC_STC_WK_ALL, this);
	myStcTitle[52].SubclassDlgItem(IDC_STC_ST, this);
	myStcTitle[53].SubclassDlgItem(IDC_STC_ST_UP, this);
	myStcTitle[54].SubclassDlgItem(IDC_STC_ST_DN, this);
	myStcTitle[55].SubclassDlgItem(IDC_STC_ST_ALL, this);
	for (i = 48; i < 56; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKBLUE);
	}

	myStcTitle[56].SubclassDlgItem(IDC_STC_GD_RA_1, this);
	myStcTitle[57].SubclassDlgItem(IDC_STC_GD_RA_2, this);
	myStcTitle[58].SubclassDlgItem(IDC_STC_GD_RA_3, this);
	myStcTitle[59].SubclassDlgItem(IDC_STC_GD_RA_4, this);
	myStcTitle[60].SubclassDlgItem(IDC_STC_GD_RA_ALL, this);
	for (i = 56; i < 61; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
		myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	}
	myStcTitle[61].SubclassDlgItem(IDC_STC_UP, this);
	myStcTitle[62].SubclassDlgItem(IDC_STC_DN, this);
	myStcTitle[63].SubclassDlgItem(IDC_STC_LAY2, this);
	for (i = 61; i < 64; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKBLUE);
	}

	//myStcTitle[64].SubclassDlgItem(IDC_STC_MK_CNT, this);
	//myStcTitle[64].SetFontName(_T("Arial"));
	//myStcTitle[64].SetFontSize(12);
	//myStcTitle[64].SetFontBold(FALSE);
	//myStcTitle[64].SetTextColor(RGB_WHITE);
	//myStcTitle[64].SetBkColor(RGB_DARKBLUE);

	//myStcTitle[65].SubclassDlgItem(IDC_STC_MK_L, this);
	//myStcTitle[66].SubclassDlgItem(IDC_STC_MK_R, this);
	//for (i = 65; i < MAX_MENU06_STC_TITLE; i++)
	//{
	//	myStcTitle[i].SetFontName(_T("Arial"));
	//	myStcTitle[i].SetFontSize(12);
	//	myStcTitle[i].SetFontBold(FALSE);
	//	myStcTitle[i].SetTextColor(RGB_BLACK);
	//	myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	//}
}

BOOL CDlgMenu06::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgMenu06::OnShowWindow(BOOL bShow, UINT nStatus)
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

void CDlgMenu06::AtDlgShow()
{
	//UpdateData();

	//if (pDoc->WorkingInfo.LastJob.nMergingLayer == 0) 	// [0]:AOI-Up , [1]:AOI-Dn
	//{
	//	myStcTitle[61].SetTextColor(RGB_WHITE);
	//	myStcTitle[61].SetBkColor(RGB_RED);
	//	myStcTitle[62].SetTextColor(RGB_WHITE);
	//	myStcTitle[62].SetBkColor(RGB_DARKBLUE);
	//}
	//else if (pDoc->WorkingInfo.LastJob.nMergingLayer == 1) 	// [0]:AOI-Up , [1]:AOI-Dn
	//{
	//	myStcTitle[61].SetTextColor(RGB_WHITE);
	//	myStcTitle[61].SetBkColor(RGB_DARKBLUE);
	//	myStcTitle[62].SetTextColor(RGB_WHITE);
	//	myStcTitle[62].SetBkColor(RGB_RED);
	//}

	//myBtn[21].SetCheck(pDoc->WorkingInfo.Probing[0].bUse);
	//myBtn[22].SetCheck(pDoc->WorkingInfo.Probing[0].bStopOnOpen);
}

void CDlgMenu06::AtDlgHide()
{
}
