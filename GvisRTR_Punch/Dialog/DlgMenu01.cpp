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
	m_pReelMapDisp = NULL;
	m_bLastProc = FALSE;
	m_nSelRmap = RMAP_ALLUP;
	m_nSelRmapPrev = RMAP_ALLUP;
	m_bEnableMkStrip = FALSE;
	m_bTIM_DISP_MK_CNT = FALSE;
	m_bTIM_DISP_DEF_IMG = FALSE;

	m_nIdxMkInfo[0] = 0;
	m_nIdxMkInfo[1] = 0;
	m_nDef[0] = 0;
	m_nDef[1] = 0;
	m_nSerialDispMkInfo = 0;
	m_nSerial = 0;

	Create();
	Reset();
}

CDlgMenu01::~CDlgMenu01()
{
	m_bTIM_DISP_MK_CNT = FALSE;
	m_bTIM_DISP_DEF_IMG = FALSE;

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
	ON_WM_TIMER()
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
	ResetSerial();
	ResetLastProc();
}

void CDlgMenu01::ResetSerial()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	m_nSerial = 0;

	if (myBtn[3].GetCheck())
	{
		myBtn[3].SetCheck(FALSE);
		if (MODE_INNER != pDoc->GetTestMode())
			General.bLastProcFromUp = TRUE;
		else
			General.bLastProcFromEng = TRUE;
		m_bLastProc = FALSE;
	}

	if (myBtn[6].GetCheck())
	{
		myBtn[6].SetCheck(FALSE);
		General.bLotEnd = FALSE;
		pView->MpeWrite(_T("MB440180"), 0);			// 작업종료(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
	}

	if (myBtn[4].GetCheck())
	{
		myBtn[4].SetCheck(FALSE);
		pView->MpeWrite(_T("MB440182"), 0);			// 재마킹(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
	}
}

void CDlgMenu01::ResetLastProc()
{
	stGeneral& General = (pView->m_mgrStatus->General);
	m_bLastProc = FALSE;
	General.bLastProcFromUp = FALSE;
	General.bLastProcFromEng = FALSE;

	pView->MpeWrite(_T("MB44019D"), 0);			// 잔량처리 각인부 부터(PC가 On시키고, PLC가 확인하고 Off시킴)
	pView->MpeWrite(_T("MB440185"), 0);			// 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
	pView->MpeWrite(_T("MB440186"), 0);			// 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
	pView->MpeWrite(_T("MB440181"), 0);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
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

	myStcData[91].SubclassDlgItem(IDC_STC_ITS_CODE, this);
	myStcData[92].SubclassDlgItem(IDC_STC_LOT_OFFSET, this);

	for (int i = 0; i < MAX_MENU01_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		if (i == 84 || i == 92)
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

	myStcTitle[75].SubclassDlgItem(IDC_STC_VS, this);

	myStcTitle[76].SubclassDlgItem(IDC_STC_ITS_, this);
	myStcTitle[76].SetFontName(_T("Arial"));
	myStcTitle[76].SetFontSize(14);
	myStcTitle[76].SetFontBold(TRUE);
	myStcTitle[76].SetTextColor(RGB_WHITE);
	myStcTitle[76].SetBkColor(RGB_DARKBLUE);

	for (i = 65; i < MAX_MENU01_STC_TITLE; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		if (i == 69 || i == 75)
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
	SetRgbStcDef();
	SetTitleStcDef();
	UpdateData();
	pView->DispMain(_T("정 지"), RGB_RED);
	EnableBtn(TRUE);

	GetDlgItem(IDC_BTN_ST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_ALL)->ShowWindow(SW_HIDE);

	GetCtrlPos();

	m_bTIM_DISP_MK_CNT = TRUE;
	SetTimer(TIM_DISP_MK_CNT, 300, NULL);

	myStcData[87].SetText(pDoc->WorkingInfo.Marking[0].sMarkingDisp1Toq); // IDC_STC_TQ_DISP1_VAL_L
	myStcData[88].SetText(pDoc->WorkingInfo.Marking[1].sMarkingDisp1Toq); // IDC_STC_TQ_DISP1_VAL_R
	myStcData[89].SetText(pDoc->WorkingInfo.Marking[0].sMarkingDisp2Toq); // IDC_STC_TQ_DISP2_VAL_L
	myStcData[90].SetText(pDoc->WorkingInfo.Marking[1].sMarkingDisp2Toq); // IDC_STC_TQ_DISP2_VAL_R

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
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu01::AtDlgShow()
{
	UpdateData();

	if (pDoc->WorkingInfo.LastJob.nMergingLayer == 0) 	// [0]:AOI-Up , [1]:AOI-Dn
	{
		myStcTitle[61].SetTextColor(RGB_WHITE);		// IDC_STC_UP
		myStcTitle[61].SetBkColor(RGB_RED);			// IDC_STC_UP
		myStcTitle[62].SetTextColor(RGB_WHITE);		// IDC_STC_DN
		myStcTitle[62].SetBkColor(RGB_DARKBLUE);	// IDC_STC_DN
	}
	else if (pDoc->WorkingInfo.LastJob.nMergingLayer == 1) 	// [0]:AOI-Up , [1]:AOI-Dn
	{
		myStcTitle[61].SetTextColor(RGB_WHITE);		// IDC_STC_UP
		myStcTitle[61].SetBkColor(RGB_DARKBLUE);	// IDC_STC_UP
		myStcTitle[62].SetTextColor(RGB_WHITE);		// IDC_STC_DN
		myStcTitle[62].SetBkColor(RGB_RED);			// IDC_STC_DN
	}

	myBtn[21].SetCheck(pDoc->WorkingInfo.Probing[0].bUse);
	myBtn[22].SetCheck(pDoc->WorkingInfo.Probing[0].bStopOnOpen);
}

void CDlgMenu01::AtDlgHide()
{
}

void CDlgMenu01::SetRgbStcDef()
{
	if (!pView->m_mgrReelmap) return;

	COLORREF rgbDef;

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_OPEN);
	myStcTitle[23].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_SHORT);
	myStcTitle[24].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_USHORT);
	myStcTitle[25].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_SPACE);
	myStcTitle[26].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_EXTRA);
	myStcTitle[27].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_PROTRUSION);
	myStcTitle[28].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_PINHOLE);
	myStcTitle[29].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_PAD);
	myStcTitle[30].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_HOLE_OPEN);
	myStcTitle[31].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_HOLE_MISS);
	myStcTitle[32].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_HOLE_POSITION);
	myStcTitle[33].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_HOLE_DEFECT);
	myStcTitle[34].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_NICK);
	myStcTitle[35].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_POI);
	myStcTitle[36].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_VH_OPEN);
	myStcTitle[37].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_VH_MISS);
	myStcTitle[38].SetBkColor(RGB_YELLOW);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_VH_POSITION);
	myStcTitle[39].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_VH_DEF);
	myStcTitle[40].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_LIGHT);
	myStcTitle[41].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_EDGE_NICK);
	myStcTitle[42].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_EDGE_PROT);
	myStcTitle[43].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_EDGE_SPACE);
	myStcTitle[44].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_USER_DEFINE_1);
	myStcTitle[45].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_NARROW);
	myStcTitle[46].SetBkColor(rgbDef);

	rgbDef = pView->m_mgrReelmap->GetRgbStcDef(DEF_WIDE);
	myStcTitle[47].SetBkColor(rgbDef);
}

void CDlgMenu01::SetTitleStcDef()
{
	if (!pView->m_mgrReelmap) return;

	myStcTitle[23].SetText(pView->m_mgrReelmap->GetKorDef(DEF_OPEN));
	myStcTitle[24].SetText(pView->m_mgrReelmap->GetKorDef(DEF_SHORT));
	myStcTitle[25].SetText(pView->m_mgrReelmap->GetKorDef(DEF_USHORT));
	myStcTitle[26].SetText(pView->m_mgrReelmap->GetKorDef(DEF_SPACE));
	myStcTitle[27].SetText(pView->m_mgrReelmap->GetKorDef(DEF_EXTRA));
	myStcTitle[28].SetText(pView->m_mgrReelmap->GetKorDef(DEF_PROTRUSION));
	myStcTitle[29].SetText(pView->m_mgrReelmap->GetKorDef(DEF_PINHOLE));
	myStcTitle[30].SetText(pView->m_mgrReelmap->GetKorDef(DEF_PAD));
	myStcTitle[31].SetText(pView->m_mgrReelmap->GetKorDef(DEF_HOLE_OPEN));
	myStcTitle[32].SetText(pView->m_mgrReelmap->GetKorDef(DEF_HOLE_MISS));
	myStcTitle[33].SetText(pView->m_mgrReelmap->GetKorDef(DEF_HOLE_POSITION));
	myStcTitle[34].SetText(pView->m_mgrReelmap->GetKorDef(DEF_HOLE_DEFECT));
	myStcTitle[35].SetText(pView->m_mgrReelmap->GetKorDef(DEF_NICK));
	myStcTitle[36].SetText(pView->m_mgrReelmap->GetKorDef(DEF_POI));
	myStcTitle[37].SetText(pView->m_mgrReelmap->GetKorDef(DEF_VH_OPEN));
	myStcTitle[38].SetText(pView->m_mgrReelmap->GetKorDef(DEF_VH_MISS));
	myStcTitle[39].SetText(pView->m_mgrReelmap->GetKorDef(DEF_VH_POSITION));
	myStcTitle[40].SetText(pView->m_mgrReelmap->GetKorDef(DEF_VH_DEF));
	myStcTitle[41].SetText(pView->m_mgrReelmap->GetKorDef(DEF_LIGHT));
	myStcTitle[42].SetText(pView->m_mgrReelmap->GetKorDef(DEF_EDGE_NICK));
	myStcTitle[43].SetText(pView->m_mgrReelmap->GetKorDef(DEF_EDGE_PROT));
	myStcTitle[44].SetText(pView->m_mgrReelmap->GetKorDef(DEF_EDGE_SPACE));
	myStcTitle[45].SetText(pView->m_mgrReelmap->GetKorDef(DEF_USER_DEFINE_1));
	myStcTitle[46].SetText(pView->m_mgrReelmap->GetKorDef(DEF_NARROW));
	myStcTitle[47].SetText(pView->m_mgrReelmap->GetKorDef(DEF_WIDE));
}

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
	pView->SetMkMenu01(_T("Info"), _T("Operator"), pDoc->WorkingInfo.LastJob.sSelUserName);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		myStcData[75].SetText(pDoc->WorkingInfo.LastJob.sLayerDn);		// 하면레이어
		pView->SetMkMenu01(_T("Info"), _T("LayerDn"), pDoc->WorkingInfo.LastJob.sLayerDn);
	}

	myStcData[1].SetText(pDoc->WorkingInfo.LastJob.sModelUp);		// 모델
	pView->SetMkMenu01(_T("Info"), _T("Model"), pDoc->WorkingInfo.LastJob.sModelUp);

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		myStcData[91].SetText(pDoc->m_sItsCode);					// ITS CODE
	else
		myStcData[91].SetText(_T(""));

	myStcData[3].SetText(pDoc->WorkingInfo.LastJob.sLotUp);			// 로트
	pView->SetMkMenu01(_T("Info"), _T("Lot"), pDoc->WorkingInfo.LastJob.sLotUp);
	myStcData[2].SetText(pDoc->WorkingInfo.LastJob.sLayerUp);		// 상면레이어
	pView->SetMkMenu01(_T("Info"), _T("LayerUp"), pDoc->WorkingInfo.LastJob.sLayerUp);

	double dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 1000.0;
	double dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen) * 1000.0;

	sVal.Format(_T("%.1f"), dTotLen / 1000.0);
	myStcData[4].SetText(sVal);			// 전체릴길이

	double dFdTotLen = 0.0;
	if(pView->m_mgrFeeding)
		dFdTotLen = pView->m_mgrFeeding->GetMkFdLen();

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
	pView->SetMkMenu01(_T("Data"), _T("VerifyLen"), pDoc->WorkingInfo.LastJob.sVerifyLen);

	//((CButton*)GetDlgItem(IDC_CHK_2LAYER))->SetCheck(pDoc->WorkingInfo.LastJob.bUse2Layer);

	if (pDoc->WorkingInfo.LastJob.bUse380mm)
		GetDlgItem(IDC_STC_380mm)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_STC_380mm)->ShowWindow(SW_HIDE);
}

void CDlgMenu01::SelMap(int nSel)
{
#ifdef TEST_MODE
	switch (nSel)
	{
	case UP:
		myBtn[12].SetCheck(TRUE);
		myBtn[13].SetCheck(FALSE);
		myBtn[14].SetCheck(FALSE);
		m_nSelRmap = RMAP_UP;
		break;
	case DN:
		myBtn[12].SetCheck(FALSE);
		myBtn[13].SetCheck(TRUE);
		myBtn[14].SetCheck(FALSE);
		m_nSelRmap = RMAP_DN;
		break;
	case ALL:
		myBtn[12].SetCheck(FALSE);
		myBtn[13].SetCheck(FALSE);
		myBtn[14].SetCheck(TRUE);
		if (pDoc->WorkingInfo.LastJob.nMergingLayer == 0)
			m_nSelRmap = RMAP_ALLUP;
		else if (pDoc->WorkingInfo.LastJob.nMergingLayer == 1)
			m_nSelRmap = RMAP_ALLDN;
		else
			m_nSelRmap = RMAP_ALLUP;
		break;
	}
	//OpenReelmap(m_nSelRmap);
	//DispReelmap(TEST_SHOT);	
#else
	CString str;// , sPath;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		switch (nSel)
		{
		case UP:
			myBtn[12].SetCheck(TRUE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(FALSE);
			m_nSelRmap = RMAP_UP;
			break;
		case DN:
			myBtn[12].SetCheck(FALSE);
			myBtn[13].SetCheck(TRUE);
			myBtn[14].SetCheck(FALSE);
			m_nSelRmap = RMAP_DN;
			break;
		case ALL:
			myBtn[12].SetCheck(FALSE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(TRUE);
			if (pDoc->WorkingInfo.LastJob.nMergingLayer == 0)
				m_nSelRmap = RMAP_ALLUP;
			else if (pDoc->WorkingInfo.LastJob.nMergingLayer == 1)
				m_nSelRmap = RMAP_ALLDN;
			else
				m_nSelRmap = RMAP_ALLUP;
			break;
		}
	}
	else
	{
		myBtn[12].SetCheck(TRUE);
		myBtn[13].SetCheck(FALSE);
		myBtn[14].SetCheck(FALSE);
		m_nSelRmap = RMAP_UP;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_nSelRmap != RMAP_ITS)
			m_nSelRmapPrev = m_nSelRmap;

		m_nSelRmap = RMAP_ITS;
	}

	SwitchReelmapDisp(m_nSelRmap);
#endif
}

void CDlgMenu01::SwitchReelmapDisp(int nSelRmap)
{
	if (!pView->m_mgrReelmap) return;
	pView->m_mgrReelmap->SwitchReelmapDisp(nSelRmap);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		switch (nSelRmap) // m_nSelRmap
		{
		case RMAP_UP:
			myBtn[12].SetCheck(TRUE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(FALSE);
			break;
		case RMAP_DN:
			myBtn[12].SetCheck(FALSE);
			myBtn[13].SetCheck(TRUE);
			myBtn[14].SetCheck(FALSE);
			break;
		case RMAP_ALLUP:
		case RMAP_ALLDN:
			myBtn[12].SetCheck(FALSE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(TRUE);
			break;
		case RMAP_ITS:
			myBtn[12].SetCheck(FALSE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(TRUE);
			break;
		}
	}
	else
	{
		m_pReelMapDisp = pView->m_mgrReelmap->m_pReelMapDisp;
	}
}

void CDlgMenu01::InitGL()
{
	if (m_pMyGL)
	{
		delete m_pMyGL;
		m_pMyGL = NULL;
	}

	if (!m_pMyGL)
	{
		m_pMyGL = new CMyGL(this);
		m_pMyGL->Init(IDC_STC_REELMAP_IMG, m_pReelMapDisp);
	}
	m_pMyGL->SetRgn();
	m_pMyGL->SetPcsDef();
}

void CDlgMenu01::ResetGL()
{
	if (m_pMyGL)
		m_pMyGL->ResetRgn();
}

void CDlgMenu01::ResetMkInfo()
{
	m_nIdxMkInfo[0] = 0;
	m_nIdxMkInfo[1] = 0;
	ClrCadImg();
	ClrDefImg();
	InitDefInfo();
	ClrInfo();


	int nCtrlIdCad, nCtrlIdDef;
	for (int nIdxMkInfo = 0; nIdxMkInfo < DEF_VIEW_IMG_NUMBER; nIdxMkInfo++)
	{
		switch (nIdxMkInfo)
		{
		case 0:
			nCtrlIdCad = IDC_PIC_CAD_001;
			nCtrlIdDef = IDC_PIC_DEF_001;
			break;
		case 1:
			nCtrlIdCad = IDC_PIC_CAD_002;
			nCtrlIdDef = IDC_PIC_DEF_002;
			break;
		case 2:
			nCtrlIdCad = IDC_PIC_CAD_003;
			nCtrlIdDef = IDC_PIC_DEF_003;
			break;
		case 3:
			nCtrlIdCad = IDC_PIC_CAD_004;
			nCtrlIdDef = IDC_PIC_DEF_004;
			break;
		case 4:
			nCtrlIdCad = IDC_PIC_CAD_005;
			nCtrlIdDef = IDC_PIC_DEF_005;
			break;
		case 5:
			nCtrlIdCad = IDC_PIC_CAD_006;
			nCtrlIdDef = IDC_PIC_DEF_006;
			break;
		case 6:
			nCtrlIdCad = IDC_PIC_CAD_007;
			nCtrlIdDef = IDC_PIC_DEF_007;
			break;
		case 7:
			nCtrlIdCad = IDC_PIC_CAD_008;
			nCtrlIdDef = IDC_PIC_DEF_008;
			break;
		case 8:
			nCtrlIdCad = IDC_PIC_CAD_009;
			nCtrlIdDef = IDC_PIC_DEF_009;
			break;
		case 9:
			nCtrlIdCad = IDC_PIC_CAD_010;
			nCtrlIdDef = IDC_PIC_DEF_010;
			break;
		case 10:
			nCtrlIdCad = IDC_PIC_CAD_011;
			nCtrlIdDef = IDC_PIC_DEF_011;
			break;
		case 11:
			nCtrlIdCad = IDC_PIC_CAD_012;
			nCtrlIdDef = IDC_PIC_DEF_012;
			break;
		}

		GetDlgItem(nCtrlIdCad)->RedrawWindow();
		GetDlgItem(nCtrlIdDef)->RedrawWindow();
	}

}

void CDlgMenu01::ResetMkInfoUp()
{
	m_nIdxMkInfo[0] = 0;
	ClrCadImgUp();
	ClrDefImgUp();
	InitDefInfo();
	ClrInfo();
}

void CDlgMenu01::ResetMkInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	m_nIdxMkInfo[1] = 0;
	ClrCadImgDn();
	ClrDefImgDn();
	InitDefInfo();
	ClrInfo();
}

void CDlgMenu01::EnableBtn(BOOL bEnable)
{
	m_bEnableMkStrip = bEnable;
	myBtn[7].EnableWindow(bEnable); // IDC_CHK_MK_1
	myBtn[8].EnableWindow(bEnable); // IDC_CHK_MK_2
	myBtn[9].EnableWindow(bEnable); // IDC_CHK_MK_3
	myBtn[10].EnableWindow(bEnable); // IDC_CHK_MK_4
}

void CDlgMenu01::GetCtrlPos()
{
	GetDlgItem(IDC_STC_UP)->GetWindowRect(&m_RectTitle[0]);
	GetDlgItem(IDC_STC_DN)->GetWindowRect(&m_RectTitle[1]);
	GetDlgItem(IDC_PIC_CAD_001)->GetWindowRect(&m_RectCAD[0]);
	GetDlgItem(IDC_PIC_CAD_002)->GetWindowRect(&m_RectCAD[1]);
	GetDlgItem(IDC_PIC_CAD_003)->GetWindowRect(&m_RectCAD[2]);
	GetDlgItem(IDC_PIC_CAD_004)->GetWindowRect(&m_RectCAD[3]);
	GetDlgItem(IDC_PIC_CAD_005)->GetWindowRect(&m_RectCAD[4]);
	GetDlgItem(IDC_PIC_CAD_006)->GetWindowRect(&m_RectCAD[5]);
	GetDlgItem(IDC_PIC_CAD_007)->GetWindowRect(&m_RectCAD[6]);
	GetDlgItem(IDC_PIC_CAD_008)->GetWindowRect(&m_RectCAD[7]);
	GetDlgItem(IDC_PIC_CAD_009)->GetWindowRect(&m_RectCAD[8]);
	GetDlgItem(IDC_PIC_CAD_010)->GetWindowRect(&m_RectCAD[9]);
	GetDlgItem(IDC_PIC_CAD_011)->GetWindowRect(&m_RectCAD[10]);
	GetDlgItem(IDC_PIC_CAD_012)->GetWindowRect(&m_RectCAD[11]);
	GetDlgItem(IDC_PIC_DEF_001)->GetWindowRect(&m_RectDef[0]);
	GetDlgItem(IDC_PIC_DEF_002)->GetWindowRect(&m_RectDef[1]);
	GetDlgItem(IDC_PIC_DEF_003)->GetWindowRect(&m_RectDef[2]);
	GetDlgItem(IDC_PIC_DEF_004)->GetWindowRect(&m_RectDef[3]);
	GetDlgItem(IDC_PIC_DEF_005)->GetWindowRect(&m_RectDef[4]);
	GetDlgItem(IDC_PIC_DEF_006)->GetWindowRect(&m_RectDef[5]);
	GetDlgItem(IDC_PIC_DEF_007)->GetWindowRect(&m_RectDef[6]);
	GetDlgItem(IDC_PIC_DEF_008)->GetWindowRect(&m_RectDef[7]);
	GetDlgItem(IDC_PIC_DEF_009)->GetWindowRect(&m_RectDef[8]);
	GetDlgItem(IDC_PIC_DEF_010)->GetWindowRect(&m_RectDef[9]);
	GetDlgItem(IDC_PIC_DEF_011)->GetWindowRect(&m_RectDef[10]);
	GetDlgItem(IDC_PIC_DEF_012)->GetWindowRect(&m_RectDef[11]);
	GetDlgItem(IDC_EDIT_DEF_INFO_001)->GetWindowRect(&m_RectInfo[0]);
	GetDlgItem(IDC_EDIT_DEF_INFO_002)->GetWindowRect(&m_RectInfo[1]);
	GetDlgItem(IDC_EDIT_DEF_INFO_003)->GetWindowRect(&m_RectInfo[2]);
	GetDlgItem(IDC_EDIT_DEF_INFO_004)->GetWindowRect(&m_RectInfo[3]);
	GetDlgItem(IDC_EDIT_DEF_INFO_005)->GetWindowRect(&m_RectInfo[4]);
	GetDlgItem(IDC_EDIT_DEF_INFO_006)->GetWindowRect(&m_RectInfo[5]);
	GetDlgItem(IDC_EDIT_DEF_INFO_007)->GetWindowRect(&m_RectInfo[6]);
	GetDlgItem(IDC_EDIT_DEF_INFO_008)->GetWindowRect(&m_RectInfo[7]);
	GetDlgItem(IDC_EDIT_DEF_INFO_009)->GetWindowRect(&m_RectInfo[8]);
	GetDlgItem(IDC_EDIT_DEF_INFO_010)->GetWindowRect(&m_RectInfo[9]);
	GetDlgItem(IDC_EDIT_DEF_INFO_011)->GetWindowRect(&m_RectInfo[10]);
	GetDlgItem(IDC_EDIT_DEF_INFO_012)->GetWindowRect(&m_RectInfo[11]);
}

void CDlgMenu01::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIM_DISP_DEF_IMG) // SetSerial() ---> DispMkInfo() ---> DispMkInfo(m_nSerial);
	{
		KillTimer(TIM_DISP_DEF_IMG);
		if (m_nSerialDispMkInfo > 0) // if (m_nSerial > 0)
		{
			DispMkInfo(m_nSerialDispMkInfo);	// DispMkInfo(m_nSerial);

			if (m_nIdxDef[0] < m_nDef[0] || m_nIdxDef[1] < m_nDef[1]) // m_nIdxDef(불량이미지 인덱스) , m_nDef[0] = pPcr->m_nTotDef; // m_nDef[up] : 릴맵 화면 표시 인덱스의 Display Def Num.
			{
				if (m_bTIM_DISP_DEF_IMG)
					SetTimer(TIM_DISP_DEF_IMG, 100, NULL);
			}
			else
				m_bTIM_DISP_DEF_IMG = FALSE;
		}
		else
			m_bTIM_DISP_DEF_IMG = FALSE;
	}
	if (nIDEvent == TIM_DISP_MK_INFO) // SetSerial() ---> if m_bTIM_DISP_DEF_IMG == FALSE then Wait to call SetTimer(TIM_DISP_MK_INFO)
	{
		KillTimer(TIM_DISP_MK_INFO);
		if (m_bTIM_DISP_DEF_IMG)
			SetTimer(TIM_DISP_MK_INFO, 100, NULL);
		else
			DispMkInfo();
	}
	if (nIDEvent == TIM_DISP_MK_CNT)
	{
		KillTimer(TIM_DISP_MK_CNT);
		DispMkCnt();
		DispTqVal();
		ChkMkLimit();
		ChkPartialSpd();

		if (pView->GetAoiUpVsStatus() && !GetDlgItem(IDC_STC_VS)->IsWindowVisible())
			GetDlgItem(IDC_STC_VS)->ShowWindow(SW_SHOW);
		else if (!pView->GetAoiUpVsStatus() && GetDlgItem(IDC_STC_VS)->IsWindowVisible())
			GetDlgItem(IDC_STC_VS)->ShowWindow(SW_HIDE);

		if (m_bTIM_DISP_MK_CNT)
			SetTimer(TIM_DISP_MK_CNT, 300, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgMenu01::DispMkInfo()	// m_bTIM_DISP_DEF_IMG == FALSE 일때까지 계속 호출함.
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		DispMkInfoUp();
		DispMkInfoDn();

		if (m_nDef[0] > 0 || m_nDef[1] > 0)
		{
			if (!m_bTIM_DISP_DEF_IMG)
			{
				m_nSerialDispMkInfo = m_nSerial;
				m_bTIM_DISP_DEF_IMG = TRUE;
				SetTimer(TIM_DISP_DEF_IMG, 10, NULL);
			}
			else
				SetTimer(TIM_DISP_MK_INFO, 10, NULL);
		}
	}
	else
	{
		DispMkInfoUp();

		if (m_nDef[0] > 0)
		{
			if (!m_bTIM_DISP_DEF_IMG)
			{
				m_nSerialDispMkInfo = m_nSerial;
				m_bTIM_DISP_DEF_IMG = TRUE;
				SetTimer(TIM_DISP_DEF_IMG, 10, NULL);
			}
			else
				SetTimer(TIM_DISP_MK_INFO, 10, NULL);
		}
	}
}

void CDlgMenu01::DispMkInfoUp()
{
	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapUp) return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nIdx = GetPcrIdx0(m_nSerial);
	m_nDef[0] = 0;
	CDataMarking* pPcr = pView->m_mgrReelmap->m_pReelMapUp->GetPcr(nIdx);
	if (!pPcr) return;

	int nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);
	if (pDoc->WorkingInfo.System.sMaxDispDefImg.IsEmpty())
		nMaxMaxDispDefImg = 0;
	else
		nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);

	if (nMaxMaxDispDefImg > 0 && pPcr->m_nTotDef - nMaxMaxDispDefImg > 0)
		m_nIdxDef[0] = pPcr->m_nTotDef - nMaxMaxDispDefImg; // 불량이미지 인덱스.
	else
		m_nIdxDef[0] = 0; // 불량이미지 인덱스.

	if (bDualTest)
	{
		if (m_nIdxMkInfo[0] > MENU01_STC_DEFINFO_HARF)
			m_nIdxMkInfo[0] = MENU01_STC_DEFINFO_HARF;
	}
	else
	{
		if (m_nIdxMkInfo[0] > MENU01_STC_DEFINFO_HARF * 2)
			m_nIdxMkInfo[0] = MENU01_STC_DEFINFO_HARF * 2;
	}

#ifdef TEST_MODE
	m_nDef[0] = m_nIdxMkInfo[0] + 15; // DefNum : 15
#else
	if (pPcr)
	{
		m_nDef[0] = pPcr->m_nTotDef; // m_nDef : m_nIdxMkInfo + Display Def Num.
		pPcr->m_nTotRealDef = 0;
	}
#endif
}

void CDlgMenu01::DispMkInfoDn()
{
	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapUp) return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest) return;

	int nIdx = GetPcrIdx1(m_nSerial);
	m_nDef[1] = 0;
	CDataMarking* pPcr = pView->m_mgrReelmap->m_pReelMapDn->GetPcr(nIdx);
	if (!pPcr) return;

	int nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);
	if (pDoc->WorkingInfo.System.sMaxDispDefImg.IsEmpty())
		nMaxMaxDispDefImg = 0;
	else
		nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);

	if (nMaxMaxDispDefImg > 0 && pPcr->m_nTotDef - nMaxMaxDispDefImg > 0)
		m_nIdxDef[1] = pPcr->m_nTotDef - nMaxMaxDispDefImg; // 불량이미지 인덱스.
	else
		m_nIdxDef[1] = 0; // 불량이미지 인덱스.

	if (m_nIdxMkInfo[1] > MENU01_STC_DEFINFO_HARF)
		m_nIdxMkInfo[1] = MENU01_STC_DEFINFO_HARF;
#ifdef TEST_MODE
	m_nDef[1] = m_nIdxMkInfo[1] + 15; // DefNum : 15
#else
	if (pPcr)
	{
		m_nDef[1] = pPcr->m_nTotDef; // m_nDef : m_nIdxMkInfo + Display Def Num.
		pPcr->m_nTotRealDef = 0;
	}
#endif
}

void CDlgMenu01::DispMkInfo(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.2"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	DispMkInfoUp(nSerial);
	if (bDualTest)
		DispMkInfoDn(nSerial);
}

void CDlgMenu01::DispMkInfoUp(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.3"));
		return;
	}

	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapUp) return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = GetPcrIdx0(nSerial);
	CDataMarking* pPcr = pView->m_mgrReelmap->m_pReelMapUp->GetPcr(nIdx);
	if (!pPcr) return;

#ifdef USE_VISION
	if (bDualTest)
	{
		int nIdxMkInfo = (m_nIdxMkInfo[0] < MENU01_STC_DEFINFO_HARF) ? m_nIdxMkInfo[0] : (MENU01_STC_DEFINFO_HARF - 1);

		int nDefImg;
		if (pPcr->m_pImg)
		{
			if (pPcr->m_pMk[m_nIdxDef[0]] != -2) // -2 (NoMarking)
			{
				if (m_nIdxDef[0] < pPcr->m_nTotDef)
				{
					if (m_nIdxMkInfo[0] >= MENU01_STC_DEFINFO_HARF)
						ShiftDefInfoUp();								
					ShowDispCadUp(nIdxMkInfo, nSerial, m_nIdxDef[0]);//pView->m_pVision[0]->ShowDispCad(nIdxMkInfo, nSerial, 0, m_nIdxDef[0]);
					ShowOvrCadUp(nIdxMkInfo, nSerial);//pView->m_pVision[0]->ShowOvrCad(nIdxMkInfo, nSerial);
					nDefImg = pPcr->m_pImg[m_nIdxDef[0]]; // 화면에 표시할 불량이미지 인덱스
					ShowDispDefUp(nIdxMkInfo, nSerial, nDefImg);//pView->m_pVision[0]->ShowDispDef(nIdxMkInfo, nSerial, 0, nDefImg);
					ShowDefInfoUp(nIdxMkInfo); // 화면의 IDC 인덱스
					WriteDefInfoUp(nSerial, nIdxMkInfo, m_nIdxDef[0], nDefImg); // (nSerial, 화면의 IDC 인덱스, 불량피스 인덱스, 불량이미지 인덱스)
					SaveCadImgUp(nSerial, nIdxMkInfo, nDefImg);
					m_nIdxMkInfo[0]++; // 화면의 IDC 인덱스
					m_nIdxDef[0]++; // 화면에 표시할 불량피스 인덱스 ( 0 ~ TotalDef )
					(pPcr->m_nTotRealDef)++;
				}
				else
					m_nIdxMkInfo[0]++;
			}
			else
				m_nIdxDef[0]++;
		}
	}
	else
	{
		int nIdxMkInfo = (m_nIdxMkInfo[0] < MENU01_STC_DEFINFO_HARF * 2) ? m_nIdxMkInfo[0] : (MENU01_STC_DEFINFO_HARF * 2 - 1);

		int nDefImg;
		if (pPcr->m_pImg)
		{
			if (pPcr->m_pMk[m_nIdxDef[0]] != -2) // -2 (NoMarking)
			{
				if (m_nIdxDef[0] < pPcr->m_nTotDef)
				{
					if (m_nIdxMkInfo[0] >= MENU01_STC_DEFINFO_HARF * 2)
						ShiftDefInfoUp();
					ShowDispCadUp(nIdxMkInfo, nSerial, m_nIdxDef[0]);//pView->m_pVision[0]->ShowDispCad(nIdxMkInfo, nSerial, 0, m_nIdxDef[0]);
					ShowOvrCadUp(nIdxMkInfo, nSerial);//pView->m_pVision[0]->ShowOvrCad(nIdxMkInfo, nSerial);
					nDefImg = pPcr->m_pImg[m_nIdxDef[0]];
					ShowDispDefUp(nIdxMkInfo, nSerial, nDefImg);//pView->m_pVision[0]->ShowDispDef(nIdxMkInfo, nSerial, 0, nDefImg);
					ShowDefInfoUp(nIdxMkInfo);
					WriteDefInfoUp(nSerial, nIdxMkInfo, m_nIdxDef[0], nDefImg);
					SaveCadImgUp(nSerial, nIdxMkInfo, nDefImg);
					m_nIdxMkInfo[0]++;
					m_nIdxDef[0]++;
					(pPcr->m_nTotRealDef)++;
				}
				else
					m_nIdxMkInfo[0]++;
			}
			else
				m_nIdxDef[0]++;
		}
	}
#endif
}

void CDlgMenu01::DispMkInfoDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.4"));
		return;
	}

	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapDn) return;
	int nIdx = GetPcrIdx1(nSerial);
	CDataMarking* pPcr = pView->m_mgrReelmap->m_pReelMapDn->GetPcr(nIdx);
	if (!pPcr) return;

#ifdef USE_VISION
	int nIdxMkInfo = (m_nIdxMkInfo[1] < MENU01_STC_DEFINFO_HARF) ? m_nIdxMkInfo[1] : (MENU01_STC_DEFINFO_HARF - 1);

	int nDefImg;
	if (pPcr->m_pImg)
	{
		if (pPcr->m_pMk[m_nIdxDef[1]] != -2) // -2 (NoMarking)
		{
			if (m_nIdxDef[1] < pPcr->m_nTotDef)
			{
				if (m_nIdxMkInfo[1] >= MENU01_STC_DEFINFO_HARF)
					ShiftDefInfoDn();
				ShowDispCadDn(nIdxMkInfo, nSerial,m_nIdxDef[1]); //pView->m_pVision[1]->ShowDispCad(nIdxMkInfo, nSerial, 1, m_nIdxDef[1]);
				ShowOvrCadDn(nIdxMkInfo, nSerial);//pView->m_pVision[1]->ShowOvrCad(nIdxMkInfo, nSerial);
				nDefImg = pPcr->m_pImg[m_nIdxDef[1]];
				ShowDispDefDn(nIdxMkInfo, nSerial, nDefImg);//pView->m_pVision[1]->ShowDispDef(nIdxMkInfo, nSerial, 1, nDefImg);
				ShowDefInfoDn(nIdxMkInfo);
				WriteDefInfoDn(nSerial, nIdxMkInfo, m_nIdxDef[1], nDefImg);
				SaveCadImgDn(nSerial, nIdxMkInfo, nDefImg);
				m_nIdxMkInfo[1]++;
				m_nIdxDef[1]++;
				(pPcr->m_nTotRealDef)++;
			}
			else
				m_nIdxMkInfo[1]++;
		}
		else
			m_nIdxDef[1]++;
	}
#endif
}

void CDlgMenu01::InitMkInfo()
{
	InitMkInfoUp();

	if (pDoc->WorkingInfo.LastJob.bDualTest) // 20220519
		InitMkInfoDn();
}

void CDlgMenu01::InitMkInfoUp()
{
#ifdef USE_VISION
	if (!pView->m_pVision[0])
		return;
#else
	return;
#endif

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;

	if (bDualTest)
	{
		for (int nIdxMkInfo = 0; nIdxMkInfo < DEF_VIEW_IMG_NUMBER / 2; nIdxMkInfo++)
		{
			switch (nIdxMkInfo)
			{
			case 0:
				nCtrlIdCad = IDC_PIC_CAD_001;
				nCtrlIdDef = IDC_PIC_DEF_001;
				break;
			case 1:
				nCtrlIdCad = IDC_PIC_CAD_002;
				nCtrlIdDef = IDC_PIC_DEF_002;
				break;
			case 2:
				nCtrlIdCad = IDC_PIC_CAD_003;
				nCtrlIdDef = IDC_PIC_DEF_003;
				break;
			case 3:
				nCtrlIdCad = IDC_PIC_CAD_004;
				nCtrlIdDef = IDC_PIC_DEF_004;
				break;
			case 4:
				nCtrlIdCad = IDC_PIC_CAD_005;
				nCtrlIdDef = IDC_PIC_DEF_005;
				break;
			case 5:
				nCtrlIdCad = IDC_PIC_CAD_006;
				nCtrlIdDef = IDC_PIC_DEF_006;
				break;
			}

			hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVision[0]->SelDispCad(hW, rt, nIdxMkInfo);
			pView->m_pVision[0]->SetOvrCadFontSz(nIdxMkInfo);

			hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
			pView->m_pVision[0]->SelDispDef(hW, rt, nIdxMkInfo);
#endif
		}
	}
	else
	{
		FreeMkInfoDn();
		for (int nIdxMkInfo = 0; nIdxMkInfo < DEF_VIEW_IMG_NUMBER; nIdxMkInfo++)
		{
			switch (nIdxMkInfo)
			{
			case 0:
				nCtrlIdCad = IDC_PIC_CAD_001;
				nCtrlIdDef = IDC_PIC_DEF_001;
				break;
			case 1:
				nCtrlIdCad = IDC_PIC_CAD_002;
				nCtrlIdDef = IDC_PIC_DEF_002;
				break;
			case 2:
				nCtrlIdCad = IDC_PIC_CAD_003;
				nCtrlIdDef = IDC_PIC_DEF_003;
				break;
			case 3:
				nCtrlIdCad = IDC_PIC_CAD_004;
				nCtrlIdDef = IDC_PIC_DEF_004;
				break;
			case 4:
				nCtrlIdCad = IDC_PIC_CAD_005;
				nCtrlIdDef = IDC_PIC_DEF_005;
				break;
			case 5:
				nCtrlIdCad = IDC_PIC_CAD_006;
				nCtrlIdDef = IDC_PIC_DEF_006;
				break;
			case 6:
				nCtrlIdCad = IDC_PIC_CAD_007;
				nCtrlIdDef = IDC_PIC_DEF_007;
				break;
			case 7:
				nCtrlIdCad = IDC_PIC_CAD_008;
				nCtrlIdDef = IDC_PIC_DEF_008;
				break;
			case 8:
				nCtrlIdCad = IDC_PIC_CAD_009;
				nCtrlIdDef = IDC_PIC_DEF_009;
				break;
			case 9:
				nCtrlIdCad = IDC_PIC_CAD_010;
				nCtrlIdDef = IDC_PIC_DEF_010;
				break;
			case 10:
				nCtrlIdCad = IDC_PIC_CAD_011;
				nCtrlIdDef = IDC_PIC_DEF_011;
				break;
			case 11:
				nCtrlIdCad = IDC_PIC_CAD_012;
				nCtrlIdDef = IDC_PIC_DEF_012;
				break;
			}

#ifdef USE_VISION
			hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
			pView->m_pVision[0]->SelDispCad(hW, rt, nIdxMkInfo);
			pView->m_pVision[0]->SetOvrCadFontSz(nIdxMkInfo);

			hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
			pView->m_pVision[0]->SelDispDef(hW, rt, nIdxMkInfo);
#endif
		}
	}

}

void CDlgMenu01::InitMkInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

#ifdef USE_VISION
	if (!pView->m_pVision[1])
		return;
#else
	return;
#endif

	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;

	FreeMkInfoUp();
	for (int nIdxMkInfo = 0; nIdxMkInfo < DEF_VIEW_IMG_NUMBER / 2; nIdxMkInfo++)
	{
		switch (nIdxMkInfo)
		{
		case 0:
			nCtrlIdCad = IDC_PIC_CAD_007;
			nCtrlIdDef = IDC_PIC_DEF_007;
			break;
		case 1:
			nCtrlIdCad = IDC_PIC_CAD_008;
			nCtrlIdDef = IDC_PIC_DEF_008;
			break;
		case 2:
			nCtrlIdCad = IDC_PIC_CAD_009;
			nCtrlIdDef = IDC_PIC_DEF_009;
			break;
		case 3:
			nCtrlIdCad = IDC_PIC_CAD_010;
			nCtrlIdDef = IDC_PIC_DEF_010;
			break;
		case 4:
			nCtrlIdCad = IDC_PIC_CAD_011;
			nCtrlIdDef = IDC_PIC_DEF_011;
			break;
		case 5:
			nCtrlIdCad = IDC_PIC_CAD_012;
			nCtrlIdDef = IDC_PIC_DEF_012;
			break;
		}

		hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
		GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
		pView->m_pVision[1]->SelDispCad(hW, rt, nIdxMkInfo);
		pView->m_pVision[1]->SetOvrCadFontSz(nIdxMkInfo);

		hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
		GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
		pView->m_pVision[1]->SelDispDef(hW, rt, nIdxMkInfo);
#endif
	}

}

void CDlgMenu01::SaveCadImgUp(int nSerial, int nIdxMkInfo, int nIdxImg) // (nSerial, 화면의 IDC 인덱스, 불량이미지 인덱스)
{
	pDoc->MakeImageDirUp(nSerial);

	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		nSerial,
		nIdxImg);

	if (pView->m_pVision[0])
		pView->m_pVision[0]->SaveCadImg(nIdxMkInfo, sPath);
}

void CDlgMenu01::SaveCadImgDn(int nSerial, int nIdxMkInfo, int nIdxImg) // (nSerial, 화면의 IDC 인덱스, 불량이미지 인덱스)
{
	pDoc->MakeImageDirDn(nSerial);

	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\CadImage\\%d\\%05d.tif"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		nSerial,
		nIdxImg);

	if (pView->m_pVision[1])
		pView->m_pVision[1]->SaveCadImg(nIdxMkInfo, sPath);
}

BOOL CDlgMenu01::SaveDefImgPosUp(int nSerial, int nIdxMkInfo, int nIdxImg) // (nSerial, 화면의 IDC 인덱스, 불량이미지 인덱스)
{
	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapUp) return 0;
	CPcsRgn* pPcsRgn = pView->m_mgrReelmap->m_Master[0].m_pPcsRgn;

	int nStrip = -1, nCol = -1, nRow = -1;
	int nPcrIdx = GetPcrIdx0(nSerial);
	CDataMarking* pPcr = pView->m_mgrReelmap->m_pReelMapUp->GetPcr(nPcrIdx);

	int nPcsIdx = pPcr->m_pDefPcs[m_nIdxDef[0]];
	int nDefCode = pPcr->m_pDefType[m_nIdxDef[0]];
	if (pPcsRgn)
		pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);

	CString strDefImgPathS, strDefImgPathD, strTemp;

	strDefImgPathS.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		nSerial,
		nIdxImg);

	strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		nSerial,
		nIdxImg, pView->m_mgrReelmap->GetKorDef(nDefCode), nStrip + 'A', nCol + 1, nRow + 1);

	CFileFind finder;
	if (finder.FindFile(strDefImgPathS))
	{
		if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
		{
			if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
			{
				strTemp.Format(_T("%s \r\n: Defect Image Up Position File Copy Fail"), strDefImgPathS);
				pView->MsgBox(strTemp);
				return FALSE;
			}
		}
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CDlgMenu01::SaveDefImgPosDn(int nSerial, int nIdxMkInfo, int nIdxImg) // (nSerial, 화면의 IDC 인덱스, 불량이미지 인덱스)
{
	if (!pView->m_mgrReelmap || !pView->m_mgrReelmap->m_pReelMapUp) return 0;
	CPcsRgn* pPcsRgn = pView->m_mgrReelmap->m_Master[0].m_pPcsRgn;

	int nStrip = -1, nCol = -1, nRow = -1;
	int nPcrIdx = GetPcrIdx1(nSerial);
	CDataMarking* pPcr = pView->m_mgrReelmap->m_pReelMapDn->GetPcr(nPcrIdx);

	int nPcsIdx = pPcr->m_pDefPcs[m_nIdxDef[1]];
	int nDefCode = pPcr->m_pDefType[m_nIdxDef[1]];
	if (pPcsRgn)
		pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);

	CString strDefImgPathS, strDefImgPathD, strTemp;

	strDefImgPathS.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		nSerial,
		nIdxImg);

	strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		nSerial,
		nIdxImg, pView->m_mgrReelmap->GetKorDef(nDefCode), nStrip + 'A', nCol + 1, nRow + 1);

	CFileFind finder;
	if (finder.FindFile(strDefImgPathS))
	{
		if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
		{
			if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
			{
				strTemp.Format(_T("%s \r\n: Defect Image Dn Position File Copy Fail"), strDefImgPathS);
				pView->MsgBox(strTemp);
				return FALSE;
			}
		}
	}
	else
		return FALSE;

	return TRUE;
}

void CDlgMenu01::WriteDefInfoUp(int nSerial, int nIdxText, int nIdxDef, int nIdxImg)
{
	CString sText = myStcDefInfo[nIdxText].GetText();
	COLORREF rgbDef = myStcDefInfo[nIdxText].GetBkColor();

	CString sPos = _T(""), sDef = _T("");
	int nPos = -1;

	nPos = sText.Find(_T('\r'));
	sDef = sText.Left(nPos);
	sDef.Trim();
	sPos = sText.Right(sText.GetLength() - nPos - 2);
	sPos.Trim();

	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\Disp.txt"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		nSerial);

	CString sItem, sData;
	sData.Format(_T("%d"), m_nDef[0]);
	::WritePrivateProfileString(_T("Info"), _T("TotalDef"), sData, sPath);

	int nMaxMaxDispDefImg = 0;
	if (!pDoc->WorkingInfo.System.sMaxDispDefImg.IsEmpty())
		nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);

	sData.Format(_T("%d"), nMaxMaxDispDefImg);
	::WritePrivateProfileString(_T("Info"), _T("MaxDisp"), sData, sPath);

	sItem.Format(_T("%d"), nIdxDef);
	sData.Format(_T("%d"), nIdxImg);
	::WritePrivateProfileString(sItem, _T("nImg"), sData, sPath);
	::WritePrivateProfileString(sItem, _T("TextDef"), sDef, sPath);
	::WritePrivateProfileString(sItem, _T("TextPos"), sPos, sPath);
	sData.Format(_T("%d"), rgbDef);
	::WritePrivateProfileString(sItem, _T("TextRGB"), sData, sPath);
}

void CDlgMenu01::WriteDefInfoDn(int nSerial, int nIdxText, int nIdxDef, int nIdxImg)
{
	CString sText = myStcDefInfo[MENU01_STC_DEFINFO_HARF + nIdxText].GetText();
	COLORREF rgbDef = myStcDefInfo[MENU01_STC_DEFINFO_HARF + nIdxText].GetBkColor();

	CString sPos = _T(""), sDef = _T("");
	int nPos = -1;

	nPos = sText.Find(_T('\r'));
	sDef = sText.Left(nPos);
	sDef.Trim();
	sPos = sText.Right(sText.GetLength() - nPos - 2);
	sPos.Trim();


	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\Disp.txt"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModelUp,
		pDoc->WorkingInfo.LastJob.sLotUp,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		nSerial);

	int nIdx = GetPcrIdx0(m_nSerial);
	CString sItem, sData;

	sData.Format(_T("%d"), pDoc->m_pPcr[2][nIdx]->m_nTotDef);
	::WritePrivateProfileString(_T("Info"), _T("MergedTotalDef"), sData, sPath);

	sData.Format(_T("%d"), m_nDef[1]);
	::WritePrivateProfileString(_T("Info"), _T("TotalDef"), sData, sPath);

	int nMaxMaxDispDefImg = 0;
	if (!pDoc->WorkingInfo.System.sMaxDispDefImg.IsEmpty())
		nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);

	sData.Format(_T("%d"), nMaxMaxDispDefImg);
	::WritePrivateProfileString(_T("Info"), _T("MaxDisp"), sData, sPath);

	sItem.Format(_T("%d"), nIdxDef);
	sData.Format(_T("%d"), nIdxImg);
	::WritePrivateProfileString(sItem, _T("nImg"), sData, sPath);
	::WritePrivateProfileString(sItem, _T("TextDef"), sDef, sPath);
	::WritePrivateProfileString(sItem, _T("TextPos"), sPos, sPath);
	sData.Format(_T("%d"), rgbDef);
	::WritePrivateProfileString(sItem, _T("TextRGB"), sData, sPath);
}

void CDlgMenu01::ShowDefInfoUp(int nIdx) // nIdx : 0 ~ 11 (12ea)
{
	if (!pView->m_bDrawGL_Menu01)
		return;
	if (!pDoc->m_pReelMap)
	{
		pView->MsgBox(_T("Not created Reelmap."));
		//AfxMessageBox(_T("Not created Reelmap."));
		return;
	}

	CString str;
	int nDefCode, nStrip, nCol, nRow;
	COLORREF rgbDef;

#ifdef TEST_MODE 
	// 	nDefCode = pDoc->m_pReelMap->pPcsDef[2][28];
	int nPcrIdx = GetPcrIdx0(m_nSerial);
	if (pPcr->m_pMk[m_nIdxDef[0]] == -2) // -2 (NoMarking)
		return;
	int nPcsIdx = pPcr->m_pDefPcs[m_nIdxDef[0]]; //m_pImg[m_nIdxDef];
	nDefCode = pPcr->m_pDefType[m_nIdxDef[0]];
	rgbDef = pDoc->m_pReelMap->m_rgbDef[nDefCode];
	if (pPcsRgn)
		pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
	// 	str.Format(_T("%s\r\n%d열 %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip+1, nCol+1, nRow+1);
	// 	str.Format(_T("%s"), pDoc->m_pReelMap->pMkInfo[nPcsIdx]);
	// 	str.Format(_T("%s"), pDoc->m_pReelMap->pMkInfo[28]);

#else
	int nPcrIdx = GetPcrIdx0(m_nSerial);
	// 	int nSelMkPnl = pDoc->m_pReelMap->m_nSelMarkingPnl;
	if (MODE_OUTER != pDoc->GetTestMode())
	{
		if (pPcr->m_pMk[m_nIdxDef[0]] == -2) // -2 (NoMarking)
			return;
	}
	else
	{
		if (pPcr->m_pMk[m_nIdxDef[0]] == -2) // -2 (NoMarking)
			return;
	}
	int nPcsIdx = pPcr->m_pDefPcs[m_nIdxDef[0]]; //m_pImg[m_nIdxDef];
	nDefCode = pPcr->m_pDefType[m_nIdxDef[0]];
	// 	nDefCode = pDoc->m_pReelMap->pPcsDef[nSelMkPnl][nPcsIdx];
	rgbDef = pDoc->m_pReelMap->m_rgbDef[nDefCode];
	// 	str.Format(_T("%s"), pDoc->m_pReelMap->pMkInfo[nPcsIdx]);
	if (pPcsRgn)
		pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
	// 	str.Format(_T("%s\r\n%d열 %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip+1, nCol+1, nRow+1);
#endif
	myStcDefInfo[nIdx].SetText(str);
	myStcDefInfo[nIdx].SetBkColor(rgbDef);
}

void CDlgMenu01::ShowDefInfoDn(int nIdx) // nIdx : 0 ~ 11 (12ea)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (!pView->m_bDrawGL_Menu01)
		return;
	if (!pDoc->m_pReelMap)
	{
		pView->MsgBox(_T("Not created Reelmap."));
		//AfxMessageBox(_T("Not created Reelmap."));
		return;
	}

	CString str;
	int nDefCode, nStrip, nCol, nRow;
	COLORREF rgbDef;

#ifdef TEST_MODE 
	int nPcrIdx = GetPcrIdx1(m_nSerial);
	if (pPcr->m_pMk[m_nIdxDef[1]] == -2) // -2 (NoMarking)
		return;
	int nPcsIdx = pPcr->m_pDefPcs[m_nIdxDef[1]]; //m_pImg[m_nIdxDef];
	nDefCode = pPcr->m_pDefType[m_nIdxDef[1]];
	// 	nDefCode = pDoc->m_pReelMap->pPcsDef[2][28];
	rgbDef = pDoc->m_pReelMap->m_rgbDef[nDefCode];
	if (pPcsRgn)
		pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
	// 	str.Format(_T("%s\r\n%d열 %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip+1, nCol+1, nRow+1);
	// 	str.Format(_T("%s"), pDoc->m_pReelMap->pMkInfo[nPcsIdx]);
	// 	str.Format(_T("%s"), pDoc->m_pReelMap->pMkInfo[28]);
#else
	int nPcrIdx = GetPcrIdx1(m_nSerial);
	// 	int nSelMkPnl = pDoc->m_pReelMap->m_nSelMarkingPnl;
	if (MODE_OUTER != pDoc->GetTestMode())
	{
		if (pPcr->m_pMk[m_nIdxDef[1]] == -2) // -2 (NoMarking)
			return;
	}
	else
	{
		if (pPcr->m_pMk[m_nIdxDef[1]] == -2) // -2 (NoMarking)
			return;
	}
	int nPcsIdx = pPcr->m_pDefPcs[m_nIdxDef[1]]; //m_pImg[m_nIdxDef];
	nDefCode = pPcr->m_pDefType[m_nIdxDef[1]];
	// 	nDefCode = pDoc->m_pReelMap->pPcsDef[nSelMkPnl][nPcsIdx];
	rgbDef = pDoc->m_pReelMap->m_rgbDef[nDefCode];
	if (pPcsRgn)
		pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
	// 	str.Format(_T("%s\r\n%d열 %d, %d"), pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip+1, nCol+1, nRow+1);
	// 	str.Format(_T("%s"), pDoc->m_pReelMap->pMkInfo[nPcsIdx]);
#endif
	myStcDefInfo[MENU01_STC_DEFINFO_HARF + nIdx].SetText(str);
	myStcDefInfo[MENU01_STC_DEFINFO_HARF + nIdx].SetBkColor(rgbDef);
}

void CDlgMenu01::ShiftDefInfoUp()
{
#ifdef USE_VISION
	if (!pView->m_pVision[0])
		return;

	pView->m_pVision[0]->ShiftDisp();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString str;
	COLORREF rgbDef;

	if (bDualTest)
	{
		for (int i = 0; i < MENU01_STC_DEFINFO_HARF - 1; i++)
		{
			str = myStcDefInfo[i + 1].GetText();
			rgbDef = myStcDefInfo[i + 1].GetBkColor();
			myStcDefInfo[i].SetText(str);
			myStcDefInfo[i].SetBkColor(rgbDef);
		}
	}
	else
	{
		for (int i = 0; i < MENU01_STC_DEFINFO_HARF * 2 - 1; i++)
		{
			str = myStcDefInfo[i + 1].GetText();
			rgbDef = myStcDefInfo[i + 1].GetBkColor();
			myStcDefInfo[i].SetText(str);
			myStcDefInfo[i].SetBkColor(rgbDef);
		}
	}
#endif
}

void CDlgMenu01::ShiftDefInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

#ifdef USE_VISION
	if (!pView->m_pVision[1])
		return;

	pView->m_pVision[1]->ShiftDisp();

	CString str;
	COLORREF rgbDef;
	for (int i = 0; i < MENU01_STC_DEFINFO_HARF - 1; i++)
	{
		str = myStcDefInfo[i + 1 + MENU01_STC_DEFINFO_HARF].GetText();
		rgbDef = myStcDefInfo[i + 1 + MENU01_STC_DEFINFO_HARF].GetBkColor();
		myStcDefInfo[i + MENU01_STC_DEFINFO_HARF].SetText(str);
		myStcDefInfo[i + MENU01_STC_DEFINFO_HARF].SetBkColor(rgbDef);
	}
#endif
}

void CDlgMenu01::InitCadImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
#ifdef USE_VISION
	if (pView->m_pVision[0])
	{
		//pView->m_pVision[0]->InitDispCad();
		pView->m_pVision[0]->InitCADBuf(0); // Top Side
	}

	if (bDualTest)
	{
		if (pView->m_pVision[1])
		{
			//pView->m_pVision[1]->InitDispCad();
			pView->m_pVision[1]->InitCADBuf(1); // Bottom Side
		}
	}
#endif
}

void CDlgMenu01::InitCadImgUp()
{
#ifdef USE_VISION
	if (pView->m_pVision[0])
	{
		//pView->m_pVision[0]->InitDispCad();
		pView->m_pVision[0]->InitCADBuf(0);
	}
#endif
}

void CDlgMenu01::InitCadImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

#ifdef USE_VISION
	if (pView->m_pVision[1])
	{
		//pView->m_pVision[1]->InitDispCad();
		pView->m_pVision[1]->InitCADBuf(1);
	}
#endif
}

void CDlgMenu01::InitDefImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

#ifdef USE_VISION
	if (pView->m_pVision[0])
		pView->m_pVision[0]->InitDispDef();

	if (bDualTest)
	{
		if (pView->m_pVision[1])
			pView->m_pVision[1]->InitDispDef();
	}
#endif
}

void CDlgMenu01::InitDefImgUp()
{
#ifdef USE_VISION
	if (pView->m_pVision[0])
		pView->m_pVision[0]->InitDispDef();
#endif
}

void CDlgMenu01::InitDefImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

#ifdef USE_VISION
	if (pView->m_pVision[1])
		pView->m_pVision[1]->InitDispDef();
#endif
}

void CDlgMenu01::InitDefInfo()
{
	for (int nIdx = 0; nIdx < DEF_VIEW_IMG_NUMBER; nIdx++)
	{
		myStcDefInfo[nIdx].SetText(_T(""));
		myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu01::InitDefInfoUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		for (int nIdx = 0; nIdx < DEF_VIEW_IMG_NUMBER / 2; nIdx++)
		{
			myStcDefInfo[nIdx].SetText(_T(""));
			myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
		}
	}
	else
	{
		for (int nIdx = 0; nIdx < DEF_VIEW_IMG_NUMBER; nIdx++)
		{
			myStcDefInfo[nIdx].SetText(_T(""));
			myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
		}
	}
}

void CDlgMenu01::InitDefInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	for (int nIdx = DEF_VIEW_IMG_NUMBER / 2; nIdx < DEF_VIEW_IMG_NUMBER; nIdx++)
	{
		myStcDefInfo[nIdx].SetText(_T(""));
		myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu01::ClrInfo()
{
	myStcData[5].SetText(_T(""));			// 전체진행율
	pView->SetMkMenu01(_T("Info"), _T("Total Work Ratio"), _T(""));
	myStcData[6].SetText(_T(""));			// 로트진행율
	pView->SetMkMenu01(_T("Info"), _T("Lot Work Ratio"), _T(""));
	myStcData[14].SetText(_T(""));			// 진행Lot시리얼
	pView->SetMkMenu01(_T("Info"), _T("VerifyImageNum"), _T(""));
	myStcData[7].SetText(_T(""));			// 전체속도
	pView->SetMkMenu01(_T("Info"), _T("Total Speed"), _T(""));
	myStcData[8].SetText(_T(""));			// 구간속도
	pView->SetMkMenu01(_T("Info"), _T("Patial Speed"), _T(""));
	myStcData[12].SetText(_T(""));			// 마킹부 : Distance (FdDone)
	pView->SetMkMenu01(_T("Info"), _T("DoneLengthMk"), _T(""));
	myStcData[13].SetText(_T(""));			// 검사부 Dn: Distance (FdDone)
	pView->SetMkMenu01(_T("Info"), _T("DoneLengthAoiDn"), _T(""));
	myStcData[74].SetText(_T(""));			// 검사부 Up: Distance (FdDone)
	pView->SetMkMenu01(_T("Info"), _T("DoneLengthAoiUp"), _T(""));
	myStcData[83].SetText(_T(""));			// 각인부: Distance (FdDone)
	pView->SetMkMenu01(_T("Info"), _T("DoneLengthEngrave"), _T(""));

	// < 전체 수율 >
	// 상면
	myStcData[15].SetText(_T("")); // IDC_STC_DEFECT_NUM_UP
	pView->SetMkMenu01(_T("Bed"), _T("Up"), _T(""));
	myStcData[16].SetText(_T("")); // IDC_STC_DEFECT_RATIO_UP
	pView->SetMkMenu01(_T("Bed Ratio"), _T("Up"), _T(""));
	myStcData[17].SetText(_T("")); // IDC_STC_GOOD_NUM_UP
	pView->SetMkMenu01(_T("Good"), _T("Up"), _T(""));
	myStcData[18].SetText(_T("")); // IDC_STC_GOOD_RATIO_UP
	pView->SetMkMenu01(_T("Good Ratio"), _T("Up"), _T(""));
	myStcData[19].SetText(_T("")); // IDC_STC_TOTAL_NUM_UP
	pView->SetMkMenu01(_T("Total Test"), _T("Up"), _T(""));

	// 하면
	myStcData[49].SetText(_T("")); // IDC_STC_DEFECT_NUM_DN
	pView->SetMkMenu01(_T("Bed"), _T("Dn"), _T(""));
	myStcData[50].SetText(_T("")); // IDC_STC_DEFECT_RATIO_DN
	pView->SetMkMenu01(_T("Bed Ratio"), _T("Dn"), _T(""));
	myStcData[51].SetText(_T("")); // IDC_STC_GOOD_NUM_DN
	pView->SetMkMenu01(_T("Good"), _T("Dn"), _T(""));
	myStcData[52].SetText(_T("")); // IDC_STC_GOOD_RATIO_DN
	pView->SetMkMenu01(_T("Good Ratio"), _T("Dn"), _T(""));
	myStcData[53].SetText(_T("")); // IDC_STC_TOTAL_NUM_DN
	pView->SetMkMenu01(_T("Total Test"), _T("Dn"), _T(""));

	// 전체
	myStcData[54].SetText(_T("")); // IDC_STC_DEFECT_NUM_ALL
	pView->SetMkMenu01(_T("Bed"), _T("Total"), _T(""));
	myStcData[55].SetText(_T("")); // IDC_STC_DEFECT_RATIO_ALL
	pView->SetMkMenu01(_T("Bed Ratio"), _T("Total"), _T(""));
	myStcData[56].SetText(_T("")); // IDC_STC_GOOD_NUM_ALL
	pView->SetMkMenu01(_T("Good"), _T("Total"), _T(""));
	myStcData[57].SetText(_T("")); // IDC_STC_GOOD_RATIO_ALL
	pView->SetMkMenu01(_T("Good Ratio"), _T("Total"), _T(""));
	myStcData[58].SetText(_T("")); // IDC_STC_TOTAL_NUM_ALL
	pView->SetMkMenu01(_T("Total Test"), _T("Total"), _T(""));

	// < 스트립 별 수율 >
	// 상면
	myStcData[59].SetText(_T("")); // DC_STC_GD_RA_1_UP
	pView->SetMkMenu01(_T("Yield Strip0"), _T("Up"), _T(""));
	myStcData[60].SetText(_T("")); // DC_STC_GD_RA_2_UP
	pView->SetMkMenu01(_T("Yield Strip1"), _T("Up"), _T(""));
	myStcData[61].SetText(_T("")); // DC_STC_GD_RA_3_UP
	pView->SetMkMenu01(_T("Yield Strip2"), _T("Up"), _T(""));
	myStcData[62].SetText(_T("")); // DC_STC_GD_RA_4_UP
	pView->SetMkMenu01(_T("Yield Strip3"), _T("Up"), _T(""));
	myStcData[63].SetText(_T("")); // IDC_STC_GD_RA_ALL_UP
	pView->SetMkMenu01(_T("Yield Total"), _T("Up"), _T(""));

	// 하면
	myStcData[64].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pView->SetMkMenu01(_T("Yield Strip0"), _T("Dn"), _T(""));
	myStcData[65].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pView->SetMkMenu01(_T("Yield Strip1"), _T("Dn"), _T(""));
	myStcData[66].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pView->SetMkMenu01(_T("Yield Strip2"), _T("Dn"), _T(""));
	myStcData[67].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pView->SetMkMenu01(_T("Yield Strip3"), _T("Dn"), _T(""));
	myStcData[68].SetText(_T("")); // IDC_STC_GD_RA_ALL_DN
	pView->SetMkMenu01(_T("Yield Total"), _T("Dn"), _T(""));

	// 상면 + 하면
	myStcData[69].SetText(_T("")); // IDC_STC_GD_RA_1_ALL
	pView->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[70].SetText(_T("")); // IDC_STC_GD_RA_2_ALL
	pView->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[71].SetText(_T("")); // IDC_STC_GD_RA_3_ALL
	pView->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[72].SetText(_T("")); // IDC_STC_GD_RA_4_ALL
	pView->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[73].SetText(_T("")); // IDC_STC_GD_RA_ALL_ALL
	pView->SetMkMenu01(_T("Yield Total"), _T("Total"), _T(""));

	// 불량 내역
	myStcData[24].SetText(_T("")); // IDC_STC_DEF_OPEN
	myStcData[25].SetText(_T("")); // IDC_STC_DEF_SHORT
	myStcData[26].SetText(_T("")); // IDC_STC_DEF_U_SHORT
	myStcData[27].SetText(_T("")); // IDC_STC_DEF_SPACE
	myStcData[28].SetText(_T("")); // IDC_STC_DEF_EXTRA
	myStcData[29].SetText(_T("")); // IDC_STC_DEF_PROT
	myStcData[30].SetText(_T("")); // IDC_STC_DEF_P_HOLE
	myStcData[31].SetText(_T("")); // IDC_STC_DEF_PAD
	myStcData[32].SetText(_T("")); // IDC_STC_DEF_H_OPEN
	myStcData[33].SetText(_T("")); // IDC_STC_DEF_H_MISS
	myStcData[34].SetText(_T("")); // IDC_STC_DEF_H_POS
	myStcData[35].SetText(_T("")); // IDC_STC_DEF_H_DEF
	myStcData[36].SetText(_T("")); // IDC_STC_DEF_NICK
	myStcData[37].SetText(_T("")); // IDC_STC_DEF_POI
	myStcData[38].SetText(_T("")); // IDC_STC_DEF_VH_OPEN
	myStcData[39].SetText(_T("")); // IDC_STC_DEF_VH_MISS
	myStcData[40].SetText(_T("")); // IDC_STC_DEF_VH_POS
	myStcData[41].SetText(_T("")); // IDC_STC_DEF_VH_DEF
	myStcData[42].SetText(_T("")); // IDC_STC_DEF_LIGHT
	myStcData[43].SetText(_T("")); // DEF_EDGE_NICK
	myStcData[44].SetText(_T("")); // DEF_EDGE_PROT
	myStcData[45].SetText(_T("")); // DEF_EDGE_SPACE
	myStcData[46].SetText(_T("")); // DEF_USER_DEFINE_1
	myStcData[47].SetText(_T("")); // DEF_NARROW
	myStcData[48].SetText(_T("")); // DEF_WIDE

	int i;
	CString sItem;
	for (int i = 1; i < MAX_DEF; i++)
	{
		sItem.Format(_T("%d"), i);
		pView->SetMkMenu01(_T("Defect"), sItem, _T(""));
	}

}

void CDlgMenu01::ClrCadImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

#ifdef USE_VISION
	if (pView->m_pVision[0])
		pView->m_pVision[0]->ClrDispCad();

	if (bDualTest)
	{
		if (pView->m_pVision[1])
			pView->m_pVision[1]->ClrDispCad();
	}
#endif
}

void CDlgMenu01::ClrCadImgUp()
{
#ifdef USE_VISION
	if (pView->m_pVision[0])
		pView->m_pVision[0]->ClrDispCad();
#endif
}

void CDlgMenu01::ClrCadImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

#ifdef USE_VISION
	if (pView->m_pVision[1])
		pView->m_pVision[1]->ClrDispCad();
#endif
}

void CDlgMenu01::ClrDefImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

#ifdef USE_VISION
	if (pView->m_pVision[0])
		pView->m_pVision[0]->ClrDispDef();

	if (bDualTest)
	{
		if (pView->m_pVision[1])
			pView->m_pVision[1]->ClrDispDef();
	}
#endif
}

void CDlgMenu01::ClrDefImgUp()
{
#ifdef USE_VISION
	if (pView->m_pVision[0])
		pView->m_pVision[0]->ClrDispDef();
#endif
}

void CDlgMenu01::ClrDefImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

#ifdef USE_VISION
	if (pView->m_pVision[1])
		pView->m_pVision[1]->ClrDispDef();
#endif
}

void CDlgMenu01::ShowDispCadUp(int nIdxMkInfo, int nSerial, int nIdxDef) // From 0 To 12...for Screen display.
{
	if (!pView->m_mgrPunch) return;
	pView->m_mgrPunch->ShowDispCadUp(nIdxMkInfo, nSerial, nIdxDef);
}

void CDlgMenu01::ShowDispCadDn(int nIdxMkInfo, int nSerial, int nIdxDef) // From 0 To 12...for Screen display.
{
	if (!pView->m_mgrPunch) return;
	pView->m_mgrPunch->ShowDispCadDn(nIdxMkInfo, nSerial, nIdxDef);
}

void CDlgMenu01::ShowOvrCadUp(int nIdxMkInfo, int nSerial) // From 0 To 12...for Screen display.
{
	if (!pView->m_mgrPunch) return;
	pView->m_mgrPunch->ShowDispCadUp(nIdxMkInfo, nSerial);
}

void CDlgMenu01::ShowOvrCadDn(int nIdxMkInfo, int nSerial) // From 0 To 12...for Screen display.
{
	if (!pView->m_mgrPunch) return;
	pView->m_mgrPunch->ShowDispCadDn(nIdxMkInfo, nSerial);
}

void CDlgMenu01::ShowDispDefUp(int nIdxMkInfo, int nSerial, int nDefPcs)
{
	if (!pView->m_mgrPunch) return;
	pView->m_mgrPunch->ShowDispDefUp(nIdxMkInfo, nSerial, 0, nDefPcs);
}

void CDlgMenu01::ShowDispDefDn(int nIdxMkInfo, int nSerial, int nDefPcs)
{
	if (!pView->m_mgrPunch) return;
	pView->m_mgrPunch->ShowDispDefDn(nIdxMkInfo, nSerial, 1, nDefPcs);
}

int CDlgMenu01::GetPcrIdx0(int nSerial, BOOL bNewLot)	// Punch-#0 : 릴맵화면 표시를 위한 Display buffer의 Shot 인덱스
{
	if (!pView->m_mgrReelmap) return 0;
	return pView->m_mgrReelmap->GetPcrIdx0(nSerial, bNewLot);
}

int CDlgMenu01::GetPcrIdx1(int nSerial, BOOL bNewLot)	// Punch-#1 :릴맵화면 표시를 위한 Display buffer의 Shot 인덱스
{
	if (!pView->m_mgrReelmap) return 0;
	return pView->m_mgrReelmap->GetPcrIdx1(nSerial, bNewLot);
}
