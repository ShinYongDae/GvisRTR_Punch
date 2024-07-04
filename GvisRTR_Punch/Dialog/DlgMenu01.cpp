// DlgMenu01.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu01.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu01 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMenu01, CDialog)

CDlgMenu01::CDlgMenu01(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_01, pParent)
{
	m_pRect = NULL;
	m_bLastProc = FALSE;
	Create();
}

CDlgMenu01::~CDlgMenu01()
{
	DelImg();

	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

void CDlgMenu01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu01, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHK_USER_INFO, &CDlgMenu01::OnBnClickedChkUserInfo)
END_MESSAGE_MAP()


// CDlgMenu01 메시지 처리기입니다.

BOOL CDlgMenu01::Create()
{
	return CDialog::Create(CDlgMenu01::IDD);
}

void CDlgMenu01::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU01_BTN; i++)
	{
		if (i == 4 || i == 15 || i == 16 || i == 17 || i == 18 || i == 19 || i == 20)
			continue;

		if (i == 7 || i == 8 || i == 9 || i == 10 || i == 11)
		{
			myBtn[i].LoadBkImage(NI_BTN_UP_DlgMenu01, BTN_IMG_UP);
			myBtn[i].LoadBkImage(NI_BTN_DN_DlgMenu01, BTN_IMG_DN);
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
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu01, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu01, BTN_IMG_DN);
		}
	}
}

void CDlgMenu01::DelImg()
{
	int i;
	for (i = 0; i < MAX_MENU01_BTN; i++)
	{
		if (i == 4 || i == 15 || i == 16 || i == 17 || i == 18 || i == 19 || i == 20)
			continue;

		myBtn[i].DelImgList();
	}
}

void CDlgMenu01::Reset()
{
	m_bLastProc = FALSE;

	if (MODE_INNER != pDoc->GetTestMode())
		m_bLastProcFromUp = TRUE;
	else
		m_bLastProcFromEng = TRUE;

	ResetSerial();
	ResetLastProc();
}

void CDlgMenu01::InitGui()
{
	InitStatic();
	InitBtn();

	LoadImg();
}

void CDlgMenu01::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_TP_STOP, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_TP_STOP);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);
	myBtn[0].SetText(_T("On"), BTN_DN);
	myBtn[0].SetText(_T("Off"), BTN_UP);

	myBtn[1].SubclassDlgItem(IDC_CHK_USER_INFO, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USER_INFO);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	myBtn[2].SubclassDlgItem(IDC_CHK_REVIEW, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REVIEW);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_EJECT_BUFFER, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_EJECT_BUFFER);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[4].SubclassDlgItem(IDC_CHK_REMARKING, this);
	//myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REMARKING);
	//myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_REVIEW_MOVE, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REVIEW_MOVE);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_LOT_END, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LOT_END);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_MK_1, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_1);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_MK_2, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_2);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[9].SubclassDlgItem(IDC_CHK_MK_3, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_3);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	myBtn[10].SubclassDlgItem(IDC_CHK_MK_4, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_4);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_BTN_MK_ALL, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_ALL);

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

	//myBtn[16].SubclassDlgItem(IDC_BTN_GD_RA_1, this);
	//myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_1);

	//myBtn[17].SubclassDlgItem(IDC_BTN_GD_RA_2, this);
	//myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_2);

	//myBtn[18].SubclassDlgItem(IDC_BTN_GD_RA_3, this);
	//myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_3);

	//myBtn[19].SubclassDlgItem(IDC_BTN_GD_RA_4, this);
	//myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_4);

	//myBtn[20].SubclassDlgItem(IDC_BTN_GD_RA_ALL, this);
	//myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_ALL);

	int i;
	for (i = 0; i < MAX_MENU01_BTN; i++)
	{
		if (i == 4 || i == 15 || i == 16 || i == 17 || i == 18 || i == 19 || i == 20)
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

void CDlgMenu01::InitStatic()
{
	myStcReelmap.SubclassDlgItem(IDC_STC_REELMAP_IMG, this);
	myStcReelmap.SetHwnd(this->GetSafeHwnd(), IDC_STC_REELMAP_IMG);

	InitStcDefInf();
	InitStcTitle();
	InitStcData();
}

void CDlgMenu01::InitStcDefInf()
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

	for (int i = 0; i < MAX_MENU01_STC_DEFINFO; i++)
	{
		myStcDefInfo[i].SetFontName(_T("Arial"));
		myStcDefInfo[i].SetFontSize(12);
		myStcDefInfo[i].SetFontBold(FALSE);
		myStcDefInfo[i].SetTextColor(RGB_BLACK);
		myStcDefInfo[i].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu01::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_OPERATOR, this);
	myStcData[1].SubclassDlgItem(IDC_STC_MODEL, this);
	myStcData[2].SubclassDlgItem(IDC_STC_LAYER, this);
	myStcData[3].SubclassDlgItem(IDC_STC_LOT, this);
	myStcData[4].SubclassDlgItem(IDC_STC_TOTAL_REEL, this);
	myStcData[5].SubclassDlgItem(IDC_STC_WORK_RATIO, this);
	myStcData[6].SubclassDlgItem(IDC_STC_LOT_RATIO, this);
	myStcData[7].SubclassDlgItem(IDC_STC_TOTAL_SPEED, this);
	myStcData[8].SubclassDlgItem(IDC_STC_CYCLE_SPEED, this);
	myStcData[9].SubclassDlgItem(IDC_STC_TEMP_STOP, this);
	myStcData[10].SubclassDlgItem(IDC_STC_LOT_SEP, this);
	myStcData[11].SubclassDlgItem(IDC_STC_LOT_LEN, this);
	myStcData[12].SubclassDlgItem(IDC_STC_MK, this);
	myStcData[13].SubclassDlgItem(IDC_STC_AOI_DN, this);
	myStcData[14].SubclassDlgItem(IDC_STC_LOT_SRL, this);
	myStcData[15].SubclassDlgItem(IDC_STC_DEFECT_NUM_UP, this);
	myStcData[16].SubclassDlgItem(IDC_STC_DEFECT_RATIO_UP, this);
	myStcData[17].SubclassDlgItem(IDC_STC_GOOD_NUM_UP, this);
	myStcData[18].SubclassDlgItem(IDC_STC_GOOD_RATIO_UP, this);
	myStcData[19].SubclassDlgItem(IDC_STC_TOTAL_NUM_UP, this);
	myStcData[20].SubclassDlgItem(IDC_STC_REVIEW_LEN, this);
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

	myStcData[74].SubclassDlgItem(IDC_STC_AOI_UP, this);
	myStcData[75].SubclassDlgItem(IDC_STC_LAYER2, this);

	myStcData[76].SubclassDlgItem(IDC_STC_DEF_SHORT_TOT, this);
	myStcData[77].SubclassDlgItem(IDC_STC_DEF_SPACE_TOT, this);
	myStcData[78].SubclassDlgItem(IDC_STC_DEF_P_HOLE_TOT, this);
	myStcData[79].SubclassDlgItem(IDC_STC_DEF_H_MISS_TOT, this);
	myStcData[80].SubclassDlgItem(IDC_STC_DEF_VH_MISS_TOT, this);

	myStcData[81].SubclassDlgItem(IDC_STC_MK_CNT_L, this);
	myStcData[82].SubclassDlgItem(IDC_STC_MK_CNT_R, this);

	myStcData[83].SubclassDlgItem(IDC_STC_ENG, this);

	myStcData[84].SubclassDlgItem(IDC_STC_LAST_SHOT, this);

	myStcData[85].SubclassDlgItem(IDC_STC_TQ_VAL_L, this);
	myStcData[86].SubclassDlgItem(IDC_STC_TQ_VAL_R, this);

	myStcData[87].SubclassDlgItem(IDC_STC_TQ_DISP1_VAL_L, this);
	myStcData[88].SubclassDlgItem(IDC_STC_TQ_DISP1_VAL_R, this);
	myStcData[89].SubclassDlgItem(IDC_STC_TQ_DISP2_VAL_L, this);
	myStcData[90].SubclassDlgItem(IDC_STC_TQ_DISP2_VAL_R, this);

	for (int i = 0; i < MAX_MENU01_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		if (i == 84)
		{
			myStcData[i].SetFontSize(20);
			myStcData[i].SetFontBold(TRUE);
			myStcData[i].SetTextColor(RGB_RED);
			myStcData[i].SetBkColor(RGB_BLACK);
		}
		else
		{
			myStcData[i].SetFontSize(12);
			myStcData[i].SetFontBold(FALSE);
			myStcData[i].SetTextColor(RGB_BLACK);
			myStcData[i].SetBkColor(RGB_WHITE);
		}
	}
}

void CDlgMenu01::InitStcTitle()
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

	myStcTitle[4].SubclassDlgItem(IDC_STC_TOT_REEL, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_WK_RA, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_LT_RA, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_TOT_SPD, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_CY_SPD, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_TP_STOP, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_LT_SEP, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_LT_LEN, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_MKAOI, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_LT_SRL, this);
	for (i = 4; i <= 13; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
		myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	}

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

	myStcTitle[64].SubclassDlgItem(IDC_STC_MK_CNT, this);
	myStcTitle[64].SetFontName(_T("Arial"));
	myStcTitle[64].SetFontSize(12);
	myStcTitle[64].SetFontBold(FALSE);
	myStcTitle[64].SetTextColor(RGB_WHITE);
	myStcTitle[64].SetBkColor(RGB_DARKBLUE);

	myStcTitle[65].SubclassDlgItem(IDC_STC_MK_L, this);
	myStcTitle[66].SubclassDlgItem(IDC_STC_MK_R, this);

	myStcTitle[67].SubclassDlgItem(IDC_STC_TQ_L, this);
	myStcTitle[68].SubclassDlgItem(IDC_STC_TQ_R, this);

	myStcTitle[69].SubclassDlgItem(IDC_STC_380mm, this);

	myStcTitle[70].SubclassDlgItem(IDC_STC_TQ_DSIP, this);
	myStcTitle[71].SubclassDlgItem(IDC_STC_TQ_DSIP_L, this);
	myStcTitle[72].SubclassDlgItem(IDC_STC_TQ_DSIP_R, this);
	myStcTitle[73].SubclassDlgItem(IDC_STC_TQ_DSIP1, this);
	myStcTitle[74].SubclassDlgItem(IDC_STC_TQ_DSIP2, this);

	for (i = 65; i < MAX_MENU01_STC_TITLE; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		if (i == 69)
		{
			myStcTitle[i].SetFontBold(TRUE);
			myStcTitle[i].SetTextColor(RGB_YELLOW);
			myStcTitle[i].SetBkColor(RGB_RED);
		}
		else if (i == 71 || i == 72)
		{
			myStcTitle[i].SetTextColor(RGB_WHITE);
			myStcTitle[i].SetBkColor(RGB_DARKGREEN);
			myStcTitle[i].SetFontBold(TRUE);
		}
		else if (i == 73 || i == 74)
		{
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTGRAY);
			myStcTitle[i].SetFontBold(FALSE);
		}
		else
		{
			myStcTitle[i].SetFontBold(FALSE);
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
		}
	}
}

BOOL CDlgMenu01::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMenu01::OnShowWindow(BOOL bShow, UINT nStatus)
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
		//this->MoveWindow(m_pRect, TRUE);
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu01::AtDlgShow()
{
	//UpdateData();

	//if (pDoc->WorkingInfo.LastJob.nMergingLayer == 0) 	// [0]:AOI-Up , [1]:AOI-Dn
	//{
	//	myStcTitle[61].SetTextColor(RGB_WHITE);		// IDC_STC_UP
	//	myStcTitle[61].SetBkColor(RGB_RED);			// IDC_STC_UP
	//	myStcTitle[62].SetTextColor(RGB_WHITE);		// IDC_STC_DN
	//	myStcTitle[62].SetBkColor(RGB_DARKBLUE);	// IDC_STC_DN
	//}
	//else if (pDoc->WorkingInfo.LastJob.nMergingLayer == 1) 	// [0]:AOI-Up , [1]:AOI-Dn
	//{
	//	myStcTitle[61].SetTextColor(RGB_WHITE);		// IDC_STC_UP
	//	myStcTitle[61].SetBkColor(RGB_DARKBLUE);	// IDC_STC_UP
	//	myStcTitle[62].SetTextColor(RGB_WHITE);		// IDC_STC_DN
	//	myStcTitle[62].SetBkColor(RGB_RED);			// IDC_STC_DN
	//}

	//myBtn[21].SetCheck(pDoc->WorkingInfo.Probing[0].bUse);
	//myBtn[22].SetCheck(pDoc->WorkingInfo.Probing[0].bStopOnOpen);

}

void CDlgMenu01::AtDlgHide()
{
}

//void CDlgMenu01::SetRgbStcDef()
//{
//	if (!pDoc->m_pReelMap)
//		return;
//
//	COLORREF rgbDef;
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_OPEN];
//	myStcTitle[23].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_SHORT];
//	myStcTitle[24].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_USHORT];
//	myStcTitle[25].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_SPACE];
//	myStcTitle[26].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_EXTRA];
//	myStcTitle[27].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_PROTRUSION];
//	myStcTitle[28].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_PINHOLE];
//	myStcTitle[29].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_PAD];
//	myStcTitle[30].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_HOLE_OPEN];
//	myStcTitle[31].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_HOLE_MISS];
//	myStcTitle[32].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_HOLE_POSITION];
//	myStcTitle[33].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_HOLE_DEFECT];
//	myStcTitle[34].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_NICK];
//	myStcTitle[35].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_POI];
//	myStcTitle[36].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_VH_OPEN];
//	myStcTitle[37].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_VH_MISS];
//	myStcTitle[38].SetBkColor(RGB_YELLOW);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_VH_POSITION];
//	myStcTitle[39].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_VH_DEF];
//	myStcTitle[40].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_LIGHT];
//	myStcTitle[41].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_EDGE_NICK];
//	myStcTitle[42].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_EDGE_PROT];
//	myStcTitle[43].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_EDGE_SPACE];
//	myStcTitle[44].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_USER_DEFINE_1];
//	myStcTitle[45].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_NARROW];
//	myStcTitle[46].SetBkColor(rgbDef);
//
//	rgbDef = pDoc->m_pReelMap->m_rgbDef[DEF_WIDE];
//	myStcTitle[47].SetBkColor(rgbDef);
//}
//
//void CDlgMenu01::SetTitleStcDef()
//{
//	if (!pDoc->m_pReelMap)
//		return;
//
//	myStcTitle[23].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_OPEN]);
//	myStcTitle[24].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_SHORT]);
//	myStcTitle[25].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_USHORT]);
//	myStcTitle[26].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_SPACE]);
//	myStcTitle[27].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_EXTRA]);
//	myStcTitle[28].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_PROTRUSION]);
//	myStcTitle[29].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_PINHOLE]);
//	myStcTitle[30].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_PAD]);
//	myStcTitle[31].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_HOLE_OPEN]);
//	myStcTitle[32].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_HOLE_MISS]);
//	myStcTitle[33].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_HOLE_POSITION]);
//	myStcTitle[34].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_HOLE_DEFECT]);
//	myStcTitle[35].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_NICK]);
//	myStcTitle[36].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_POI]);
//	myStcTitle[37].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_VH_OPEN]);
//	myStcTitle[38].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_VH_MISS]);
//	myStcTitle[39].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_VH_POSITION]);
//	myStcTitle[40].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_VH_DEF]);
//	myStcTitle[41].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_LIGHT]);
//
//	myStcTitle[42].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_EDGE_NICK]);
//	myStcTitle[43].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_EDGE_PROT]);
//	myStcTitle[44].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_EDGE_SPACE]);
//	myStcTitle[45].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_USER_DEFINE_1]);
//	myStcTitle[46].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_NARROW]);
//	myStcTitle[47].SetText(pDoc->m_pReelMap->m_sKorDef[DEF_WIDE]);
//}

void CDlgMenu01::DispMain(CString sMsg, COLORREF rgb)
{
	if (myStcTitle[14].GetText() != sMsg)
	{
		myStcTitle[14].SetText(sMsg);
		myStcTitle[14].SetTextColor(rgb);
	}
}

void CDlgMenu01::DispLotTime()
{
	DispStTime();
	DispRunTime();
	DispEdTime();
}

void CDlgMenu01::DispStTime()
{
	CString str, sPrev;
	int nYear, nMonth, nDay, nHour, nMin, nSec;

	nYear = pDoc->WorkingInfo.Lot.StTime.nYear;
	nMonth = pDoc->WorkingInfo.Lot.StTime.nMonth;
	nDay = pDoc->WorkingInfo.Lot.StTime.nDay;
	nHour = pDoc->WorkingInfo.Lot.StTime.nHour;
	nMin = pDoc->WorkingInfo.Lot.StTime.nMin;
	nSec = pDoc->WorkingInfo.Lot.StTime.nSec;

	if (!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		str = _T("");
	else
		str.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);

	// 	sPrev = myStcData[21].GetText();
	GetDlgItem(IDC_STC_LOT_START)->GetWindowText(sPrev);
	if (sPrev != str)
	{
		myStcData[21].SetText(str);
		pView->SetMkMenu01(_T("LotTime"), _T("Start"), str);
	}
}

void CDlgMenu01::DispRunTime()
{
	CString str, sPrev;
	int nDiff;
	int nHour, nMin, nSec;
	int nStYear, nStMonth, nStDay, nStHour, nStMin, nStSec;
	int nCurYear, nCurMonth, nCurDay, nCurHour, nCurMin, nCurSec;
	int nEdYear, nEdMonth, nEdDay, nEdHour, nEdMin, nEdSec;

	nStYear = pDoc->WorkingInfo.Lot.StTime.nYear;
	nStMonth = pDoc->WorkingInfo.Lot.StTime.nMonth;
	nStDay = pDoc->WorkingInfo.Lot.StTime.nDay;
	nStHour = pDoc->WorkingInfo.Lot.StTime.nHour;
	nStMin = pDoc->WorkingInfo.Lot.StTime.nMin;
	nStSec = pDoc->WorkingInfo.Lot.StTime.nSec;

	nCurYear = pDoc->WorkingInfo.Lot.CurTime.nYear;
	nCurMonth = pDoc->WorkingInfo.Lot.CurTime.nMonth;
	nCurDay = pDoc->WorkingInfo.Lot.CurTime.nDay;
	nCurHour = pDoc->WorkingInfo.Lot.CurTime.nHour;
	nCurMin = pDoc->WorkingInfo.Lot.CurTime.nMin;
	nCurSec = pDoc->WorkingInfo.Lot.CurTime.nSec;

	nEdYear = pDoc->WorkingInfo.Lot.EdTime.nYear;
	nEdMonth = pDoc->WorkingInfo.Lot.EdTime.nMonth;
	nEdDay = pDoc->WorkingInfo.Lot.EdTime.nDay;
	nEdHour = pDoc->WorkingInfo.Lot.EdTime.nHour;
	nEdMin = pDoc->WorkingInfo.Lot.EdTime.nMin;
	nEdSec = pDoc->WorkingInfo.Lot.EdTime.nSec;

	if (!nStYear && !nStMonth && !nStDay && !nStHour && !nStMin && !nStSec)
	{
		str = _T("");
	}
	else if (!nEdYear && !nEdMonth && !nEdDay && !nEdHour && !nEdMin && !nEdSec)
	{
		nDiff = (GetTickCount() - pView->GetLotSt()) / 1000;
		nHour = int(nDiff / 3600);
		nMin = int((nDiff - 3600 * nHour) / 60);
		nSec = nDiff % 60;
		str.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
	}
	else
	{
		if (pView->GetLotEd() > 0)
		{
			nDiff = (pView->GetLotEd() - pView->GetLotSt()) / 1000;
			nHour = int(nDiff / 3600);
			nMin = int((nDiff - 3600 * nHour) / 60);
			nSec = nDiff % 60;
			str.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
		}
		else
		{
			nHour = nEdHour - nStHour;
			if (nHour < 0)
				nHour += 24;

			nMin = nEdMin - nStMin;
			if (nMin < 0)
				nMin += 60;

			nSec = nEdSec - nStSec;
			if (nSec < 0)
				nSec += 60;

			str.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
		}
	}

	GetDlgItem(IDC_STC_LOT_START)->GetWindowText(sPrev);

	if (!sPrev.IsEmpty())
	{
		GetDlgItem(IDC_STC_LOT_RUN)->GetWindowText(sPrev);
		if (sPrev != str)
		{
			myStcData[22].SetText(str);
			pView->SetMkMenu01(_T("LotTime"), _T("Run"), str);
		}
	}
}

void CDlgMenu01::DispEdTime()
{
	CString str, sPrev;
	int nYear, nMonth, nDay, nHour, nMin, nSec;

	nYear = pDoc->WorkingInfo.Lot.EdTime.nYear;
	nMonth = pDoc->WorkingInfo.Lot.EdTime.nMonth;
	nDay = pDoc->WorkingInfo.Lot.EdTime.nDay;
	nHour = pDoc->WorkingInfo.Lot.EdTime.nHour;
	nMin = pDoc->WorkingInfo.Lot.EdTime.nMin;
	nSec = pDoc->WorkingInfo.Lot.EdTime.nSec;

	if (!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		str = _T("");
	else
		str.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);

	GetDlgItem(IDC_STC_LOT_END)->GetWindowText(sPrev);
	if (sPrev != str)
	{
		myStcData[23].SetText(str);
		pView->SetMkMenu01(_T("LotTime"), _T("End"), str);
	}
}

CString CDlgMenu01::GetRunTime()
{
	CString str;
	GetDlgItem(IDC_STC_LOT_RUN)->GetWindowText(str);
	return str;
}

void CDlgMenu01::ChkUserInfo(BOOL bOn)
{
	if (bOn)
	{
		pView->PostMessage(WM_DLG_INFO, 0, 0);
	}
	else
	{
		UpdateData();
	}

	myBtn[1].SetCheck(bOn);
}

void CDlgMenu01::OnBnClickedChkUserInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[1].GetCheck();
	ChkUserInfo(bOn);
	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu01::EnableItsMode(BOOL bEnable)
{
	if (bEnable)
	{
		myBtn[12].ShowWindow(SW_HIDE);	// IDC_CHK_DEF_UP
		myBtn[13].ShowWindow(SW_HIDE);	// IDC_CHK_DEF_DN
		myBtn[14].ShowWindow(SW_HIDE);	// IDC_CHK_DEF_ALL

		myStcTitle[49].SetText(_T("외층")); // IDC_STC_WK_UP
		myStcTitle[50].SetText(_T("내층")); // IDC_STC_WK_DN
		myStcTitle[53].SetText(_T("외층")); // IDC_STC_ST_UP
		myStcTitle[54].SetText(_T("내층")); // IDC_STC_ST_DN
	}
	else
	{
		myBtn[12].ShowWindow(SW_SHOW);	// IDC_CHK_DEF_UP
		myBtn[13].ShowWindow(SW_SHOW);	// IDC_CHK_DEF_DN
		myBtn[14].ShowWindow(SW_SHOW);	// IDC_CHK_DEF_ALL

		myStcTitle[49].SetText(_T("상면")); // IDC_STC_WK_UP
		myStcTitle[50].SetText(_T("하면")); // IDC_STC_WK_DN
		myStcTitle[53].SetText(_T("상면")); // IDC_STC_ST_UP
		myStcTitle[54].SetText(_T("하면")); // IDC_STC_ST_DN
	}
}

void CDlgMenu01::DispLotEndSerial(int nSerial)
{
	CString str = _T("");

	if (nSerial > 0)
		str.Format(_T("%d"), nSerial);

	myStcData[84].SetWindowText(str);
}

void CDlgMenu01::ResetLotTime()
{
	pDoc->WorkingInfo.Lot.StTime.nYear = 0;
	pDoc->WorkingInfo.Lot.StTime.nMonth = 0;
	pDoc->WorkingInfo.Lot.StTime.nDay = 0;
	pDoc->WorkingInfo.Lot.StTime.nHour = 0;
	pDoc->WorkingInfo.Lot.StTime.nMin = 0;
	pDoc->WorkingInfo.Lot.StTime.nSec = 0;

	pDoc->WorkingInfo.Lot.CurTime.nYear = 0;
	pDoc->WorkingInfo.Lot.CurTime.nMonth = 0;
	pDoc->WorkingInfo.Lot.CurTime.nDay = 0;
	pDoc->WorkingInfo.Lot.CurTime.nHour = 0;
	pDoc->WorkingInfo.Lot.CurTime.nMin = 0;
	pDoc->WorkingInfo.Lot.CurTime.nSec = 0;

	pDoc->WorkingInfo.Lot.EdTime.nYear = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMonth = 0;
	pDoc->WorkingInfo.Lot.EdTime.nDay = 0;
	pDoc->WorkingInfo.Lot.EdTime.nHour = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMin = 0;
	pDoc->WorkingInfo.Lot.EdTime.nSec = 0;

	myStcData[21].SetText(_T(""));
	myStcData[22].SetText(_T(""));
	myStcData[23].SetText(_T(""));
}

void CDlgMenu01::UpdateData()
{
	CString sVal;

	myStcData[0].SetText(pDoc->WorkingInfo.LastJob.sSelUserName);	// 운용자
	pDoc->SetMkMenu01(_T("Info"), _T("Operator"), pDoc->WorkingInfo.LastJob.sSelUserName);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		myStcData[75].SetText(pDoc->WorkingInfo.LastJob.sLayerDn);		// 하면레이어
		pDoc->SetMkMenu01(_T("Info"), _T("LayerDn"), pDoc->WorkingInfo.LastJob.sLayerDn);
	}

	myStcData[1].SetText(pDoc->WorkingInfo.LastJob.sModelUp);		// 모델
	pDoc->SetMkMenu01(_T("Info"), _T("Model"), pDoc->WorkingInfo.LastJob.sModelUp);

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		myStcData[91].SetText(pDoc->m_sItsCode);					// ITS CODE
	else
		myStcData[91].SetText(_T(""));

	myStcData[3].SetText(pDoc->WorkingInfo.LastJob.sLotUp);			// 로트
	pDoc->SetMkMenu01(_T("Info"), _T("Lot"), pDoc->WorkingInfo.LastJob.sLotUp);
	myStcData[2].SetText(pDoc->WorkingInfo.LastJob.sLayerUp);		// 상면레이어
	pDoc->SetMkMenu01(_T("Info"), _T("LayerUp"), pDoc->WorkingInfo.LastJob.sLayerUp);

	double dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 1000.0;
	double dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen) * 1000.0;

	sVal.Format(_T("%.1f"), dTotLen / 1000.0);
	myStcData[4].SetText(sVal);			// 전체릴길이

	double dFdTotLen = pView->GetMkFdLen();

	sVal.Format(_T("%d"), (int)(dFdTotLen / dTotLen * 100.0));
	myStcData[5].SetText(sVal);			// 전체진행율

	sVal.Format(_T("%d"), (int)(dFdTotLen / dTotLen * 100.0));
	//myStcData[6].SetText(sVal);			// 로트진행율
	myStcData[6].SetText(_T(""));		// 로트진행율

	myStcData[10].SetText(pDoc->WorkingInfo.LastJob.sStripOutRatio);	// 스트립 양폐율[%]
	myStcData[10].SetBkColor(RGB_WHITE);

	sVal.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nVerifyPeriod);
	myStcData[14].SetText(sVal);

	myStcData[11].SetText(pDoc->WorkingInfo.LastJob.sCustomNeedRatio);	// 고객출하수율
	myStcData[11].SetBkColor(RGB_WHITE);

	if (pDoc->WorkingInfo.LastJob.bTempPause)
	{
		myBtn[0].SetCheck(TRUE);
		myStcData[9].SetText(pDoc->WorkingInfo.LastJob.sTempPauseLen);	// 일시정지길이
	}
	else
	{
		myBtn[0].SetCheck(FALSE);
		myStcData[9].SetText(pDoc->WorkingInfo.LastJob.sTempPauseLen);	// 일시정지길이
	}

	myBtn[2].SetCheck(pDoc->WorkingInfo.LastJob.bVerify);
	GetDlgItem(IDC_STC_REVIEW_LEN)->SetWindowText(pDoc->WorkingInfo.LastJob.sVerifyLen);
	pDoc->SetMkMenu01(_T("Data"), _T("VerifyLen"), pDoc->WorkingInfo.LastJob.sVerifyLen);

	((CButton*)GetDlgItem(IDC_CHK_2LAYER))->SetCheck(pDoc->WorkingInfo.LastJob.bUse2Layer);

	if (pDoc->WorkingInfo.LastJob.bUse380mm)
		GetDlgItem(IDC_STC_380mm)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_STC_380mm)->ShowWindow(SW_HIDE);
}
