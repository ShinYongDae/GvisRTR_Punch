// DlgMenu02.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu02.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu02 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMenu02, CDialog)

CDlgMenu02::CDlgMenu02(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_02, pParent)
{
	m_pRect = NULL;
	Create();
}

CDlgMenu02::~CDlgMenu02()
{
	DelImg();
}

void CDlgMenu02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLD_LT, m_LightSlider);
	DDX_Control(pDX, IDC_SLD_LT2, m_LightSlider2);
}


BEGIN_MESSAGE_MAP(CDlgMenu02, CDialog)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgMenu02 메시지 처리기입니다.

BOOL CDlgMenu02::Create()
{
	return CDialog::Create(CDlgMenu02::IDD);
}

void CDlgMenu02::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU02_BTN; i++)
	{
		if (i == 6)
			continue;

		if (i == 1)
		{
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 2)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 6)
		{
		//	myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
		//	myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 7)
		{
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 8)
		{
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 9)
		{
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_DN);
		}
		else if (i != 19 && i != 24 && i != 25 && i != 26 && i != 27 && i != 28)
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu02, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu02, BTN_IMG_DN);
		}
	}

	for (i = 0; i < MAX_MENU02_BTN_DUO; i++)
	{
		if (i == 1)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 2)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 6)
		{
			//myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_UP);
			//myBtn2[i].LoadBkImage(IMG_JOG_UP_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 7)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_DN_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 8)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_LF_DlgMenu02, BTN_IMG_DN);
		}
		else if (i == 9)
		{
			myBtn2[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_JOG_RT_DlgMenu02, BTN_IMG_DN);
		}
		else
		{
			myBtn2[i].LoadBkImage(IMG_BTN_UP_DlgMenu02, BTN_IMG_UP);
			myBtn2[i].LoadBkImage(IMG_BTN_DN_DlgMenu02, BTN_IMG_DN);
		}
	}

}

void CDlgMenu02::DelImg()
{
	int i;
	for (i = 0; i < MAX_MENU02_BTN; i++)
	{
		if (i == 6)
			continue;

		if (i != 6 && i != 18 && i != 19 && i != 24 && i != 25)
			myBtn[i].DelImgList();
	}
	for (i = 0; i < MAX_MENU02_BTN_DUO; i++)
	{
		myBtn2[i].DelImgList();
	}
}


void CDlgMenu02::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
}

void CDlgMenu02::InitGui()
{
	InitSlider();

	InitStatic();
	InitBtn();

	InitStaticDuo();
	InitBtnDuo();

	LoadImg();
}

void CDlgMenu02::InitSlider()
{
	m_LightSlider.SetRange(0, 100);
	m_LightSlider.SetLineSize(1);
	m_LightSlider.SetPageSize(10);
	m_LightSlider.SetPos(100);
	m_LightSlider.SetTicFreq(10);

	m_LightSlider2.SetRange(0, 100);
	m_LightSlider2.SetLineSize(1);
	m_LightSlider2.SetPageSize(10);
	m_LightSlider2.SetPos(100);
	m_LightSlider2.SetTicFreq(10);
}

void CDlgMenu02::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgMenu02::InitStcTitle()
{
	myStcTitle[3].SubclassDlgItem(IDC_STC_RES, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_RES_X, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_RES_Y, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_MK_OFFSET, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_CAM_MK_X, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_CAM_MK_Y, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_MK_POS1, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_MK_POS2, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_ALN, this);
	myStcTitle[14].SubclassDlgItem(IDC_STC_ALN_X, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_ALN_Y, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_ALN_ANGL, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_ALN_SCR, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_ALN_SCR2, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_ALN3, this);

	for (int i = 0; i < MAX_MENU02_STC; i++)
	{
		if (i != 0 && i != 1 && i != 2 && i != 6 && i != 7 && i != 18 && i != 19 && i != 20)
		{
			myStcTitle[i].SetFontName(_T("Arial"));
			myStcTitle[i].SetFontSize(14);
			myStcTitle[i].SetFontBold(TRUE);
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_DLG_FRM);
		}
	}
}

void CDlgMenu02::InitStcData()
{
	myStcData[2].SubclassDlgItem(IDC_STC_LT_VAL, this);
	myStcData[3].SubclassDlgItem(IDC_STC_RESOLUTION_X, this);
	myStcData[4].SubclassDlgItem(IDC_STC_RESOLUTION_Y, this);
	myStcData[5].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_X, this);
	myStcData[6].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_Y, this);
	myStcData[7].SubclassDlgItem(IDC_STC_ALIGN_X, this);
	myStcData[8].SubclassDlgItem(IDC_STC_ALIGN_Y, this);
	myStcData[9].SubclassDlgItem(IDC_STC_ALIGN_AGL, this);
	myStcData[10].SubclassDlgItem(IDC_STC_ALIGN_SCR, this);
	myStcData[12].SubclassDlgItem(IDC_STC_ALIGN_STD_SCR, this);
	myStcData[13].SubclassDlgItem(IDC_STC_MK_POS1_1, this);
	myStcData[14].SubclassDlgItem(IDC_STC_MK_POS1_2, this);
	myStcData[15].SubclassDlgItem(IDC_STC_MK_POS1_3, this);
	myStcData[16].SubclassDlgItem(IDC_STC_MK_POS1_4, this);

	for (int i = 0; i < MAX_MENU02_STC_DATA; i++)
	{
		if (i != 0 && i != 1 && i != 11)
		{
			myStcData[i].SetFontName(_T("Arial"));
			if (i == 2)
				myStcData[i].SetFontSize(22);
			else
				myStcData[i].SetFontSize(14);
			myStcData[i].SetFontBold(TRUE);
			myStcData[i].SetTextColor(RGB_BLACK);
			myStcData[i].SetBkColor(RGB_WHITE);
		}
	}
}

void CDlgMenu02::InitStaticDuo()
{
	InitStcTitleDuo();
	InitStcDataDuo();
}

void CDlgMenu02::InitStcTitleDuo()
{
	myStcTitle2[0].SubclassDlgItem(IDC_STC_RES2, this);
	myStcTitle2[1].SubclassDlgItem(IDC_STC_RES_X9, this);
	myStcTitle2[2].SubclassDlgItem(IDC_STC_RES_Y4, this);
	myStcTitle2[3].SubclassDlgItem(IDC_STC_MK_OFFSET4, this);
	myStcTitle2[4].SubclassDlgItem(IDC_STC_CAM_MK_X2, this);
	myStcTitle2[5].SubclassDlgItem(IDC_STC_CAM_MK_Y2, this);
	myStcTitle2[6].SubclassDlgItem(IDC_STC_MK_POS3, this);
	myStcTitle2[7].SubclassDlgItem(IDC_STC_MK_POS4, this);
	myStcTitle2[8].SubclassDlgItem(IDC_STC_ALN2, this);
	myStcTitle2[9].SubclassDlgItem(IDC_STC_ALN_X2, this);
	myStcTitle2[10].SubclassDlgItem(IDC_STC_ALN_Y2, this);
	myStcTitle2[11].SubclassDlgItem(IDC_STC_ALN_ANGL2, this);
	myStcTitle2[12].SubclassDlgItem(IDC_STC_ALN_SCR3, this);
	myStcTitle2[13].SubclassDlgItem(IDC_STC_ALN_SCR4, this);
	myStcTitle2[14].SubclassDlgItem(IDC_STC_ALN4, this);

	for (int i = 0; i < MAX_MENU02_STC_DUO; i++)
	{
		myStcTitle2[i].SetFontName(_T("Arial"));
		myStcTitle2[i].SetFontSize(14);
		myStcTitle2[i].SetFontBold(TRUE);
		myStcTitle2[i].SetTextColor(RGB_NAVY);
		myStcTitle2[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu02::InitStcDataDuo()
{
	myStcData2[2].SubclassDlgItem(IDC_STC_LT_VAL2, this);
	myStcData2[3].SubclassDlgItem(IDC_STC_RESOLUTION_X2, this);
	myStcData2[4].SubclassDlgItem(IDC_STC_RESOLUTION_Y2, this);
	myStcData2[5].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_X2, this);
	myStcData2[6].SubclassDlgItem(IDC_STC_CAM_MK_OFFSET_Y2, this);
	myStcData2[7].SubclassDlgItem(IDC_STC_ALIGN_X2, this);
	myStcData2[8].SubclassDlgItem(IDC_STC_ALIGN_Y2, this);
	myStcData2[9].SubclassDlgItem(IDC_STC_ALIGN_AGL2, this);
	myStcData2[10].SubclassDlgItem(IDC_STC_ALIGN_SCR2, this);
	myStcData2[12].SubclassDlgItem(IDC_STC_ALIGN_STD_SCR2, this);

	myStcData2[13].SubclassDlgItem(IDC_STC_MK_POS2_1, this);
	myStcData2[14].SubclassDlgItem(IDC_STC_MK_POS2_2, this);
	myStcData2[15].SubclassDlgItem(IDC_STC_MK_POS2_3, this);
	myStcData2[16].SubclassDlgItem(IDC_STC_MK_POS2_4, this);

	for (int i = 0; i < MAX_MENU02_STC_DATA_DUO; i++)
	{
		if (i != 0 && i != 1 && i != 11)
		{
			myStcData2[i].SetFontName(_T("Arial"));
			if (i == 2)
				myStcData2[i].SetFontSize(22);
			else
				myStcData2[i].SetFontSize(14);
			myStcData2[i].SetFontBold(TRUE);
			myStcData2[i].SetTextColor(RGB_BLACK);
			myStcData2[i].SetBkColor(RGB_WHITE);
		}
	}
}

void CDlgMenu02::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_LT_ON, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LT_ON);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_BTN_LT_UP, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_UP);
	myBtn[1].SetBoarder(FALSE);

	myBtn[2].SubclassDlgItem(IDC_BTN_LT_DN, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_DN);
	myBtn[2].SetBoarder(FALSE);

	myBtn[3].SubclassDlgItem(IDC_BTN_PIN_MOVE, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_MOVE);

	myBtn[4].SubclassDlgItem(IDC_BTN_PIN_SAVE, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_SAVE);

	myBtn[5].SubclassDlgItem(IDC_BTN_HOME_MOVE, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_HOME_MOVE);

	//myBtn[6].SubclassDlgItem(IDC_BTN_JOG_UP, this);
	//myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_UP);
	//myBtn[6].SetBoarder(FALSE);

	myBtn[7].SubclassDlgItem(IDC_BTN_JOG_DN, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_DN);
	myBtn[7].SetBoarder(FALSE);

	myBtn[8].SubclassDlgItem(IDC_BTN_JOG_LF, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_LF);
	myBtn[8].SetBoarder(FALSE);

	myBtn[9].SubclassDlgItem(IDC_BTN_JOG_RT, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_RT);
	myBtn[9].SetBoarder(FALSE);

	myBtn[10].SubclassDlgItem(IDC_CHK_JOG_VF, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VF);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_JOG_F, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_F);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_JOG_N, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_N);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_JOG_S, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_S);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_JOG_VS, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VS);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_BTN_ALIGN, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN);

	myBtn[16].SubclassDlgItem(IDC_CHK_ELEC_TEST, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ELEC_TEST);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[17].SubclassDlgItem(IDC_CHK_RES_POS_ST, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RES_POS_ST);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_BTN_START_SAVE, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_BTN_START_SAVE);

	myBtn[20].SubclassDlgItem(IDC_CHK_MK_OFFSET_ST, this);
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ST);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_MK_OFFSET_ED, this);
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ED);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_BTN_ALIGN_MOVE, this);
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN_MOVE);

	myBtn[23].SubclassDlgItem(IDC_BTN_GRAB, this);
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GRAB);

	myBtn[29].SubclassDlgItem(IDC_CHK_MK_POS1_1, this);
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_1);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);

	myBtn[30].SubclassDlgItem(IDC_CHK_MK_POS1_2, this);
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_2);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);

	myBtn[31].SubclassDlgItem(IDC_CHK_MK_POS1_3, this);
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_3);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);

	myBtn[32].SubclassDlgItem(IDC_CHK_MK_POS1_4, this);
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS1_4);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);

	myBtn[33].SubclassDlgItem(IDC_BTN_MK_POS1_MOVE, this);
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS1_MOVE);

	myBtn[34].SubclassDlgItem(IDC_BTN_MK_POS1_MEAS, this);
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS1_MEAS);

	int i;
	for (i = 0; i < MAX_MENU02_BTN; i++)
	{
		if (i == 6)
			continue;

		if (i != 19 && i != 24 && i != 25 && i != 26 && i != 27 && i != 28)
		{
			myBtn[i].SetFont(_T("굴림체"), 16, TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
		}
	}

	myBtn[20].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
	myBtn[21].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
}

void CDlgMenu02::InitBtnDuo()
{
	myBtn2[0].SubclassDlgItem(IDC_CHK_LT_ON2, this);
	myBtn2[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LT_ON2);
	myBtn2[0].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[1].SubclassDlgItem(IDC_BTN_LT_UP2, this);
	myBtn2[1].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_UP2);
	myBtn2[1].SetBoarder(FALSE);
	myBtn2[2].SubclassDlgItem(IDC_BTN_LT_DN2, this);
	myBtn2[2].SetHwnd(this->GetSafeHwnd(), IDC_BTN_LT_DN2);
	myBtn2[2].SetBoarder(FALSE);
	myBtn2[3].SubclassDlgItem(IDC_BTN_PIN_MOVE2, this);
	myBtn2[3].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_MOVE2);
	myBtn2[4].SubclassDlgItem(IDC_BTN_PIN_SAVE2, this);
	myBtn2[4].SetHwnd(this->GetSafeHwnd(), IDC_BTN_PIN_SAVE2);
	myBtn2[5].SubclassDlgItem(IDC_BTN_HOME_MOVE2, this);
	myBtn2[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_HOME_MOVE2);
	myBtn2[6].SubclassDlgItem(IDC_BTN_JOG_UP2, this);
	myBtn2[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_UP2);
	myBtn2[6].SetBoarder(FALSE);
	myBtn2[7].SubclassDlgItem(IDC_BTN_JOG_DN2, this);
	myBtn2[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_DN2);
	myBtn2[7].SetBoarder(FALSE);
	myBtn2[8].SubclassDlgItem(IDC_BTN_JOG_LF2, this);
	myBtn2[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_LF2);
	myBtn2[8].SetBoarder(FALSE);
	myBtn2[9].SubclassDlgItem(IDC_BTN_JOG_RT2, this);
	myBtn2[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_JOG_RT2);
	myBtn2[9].SetBoarder(FALSE);
	myBtn2[10].SubclassDlgItem(IDC_CHK_JOG_VF2, this);
	myBtn2[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VF2);
	myBtn2[10].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[11].SubclassDlgItem(IDC_CHK_JOG_F2, this);
	myBtn2[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_F2);
	myBtn2[11].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[12].SubclassDlgItem(IDC_CHK_JOG_N2, this);
	myBtn2[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_N2);
	myBtn2[12].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[13].SubclassDlgItem(IDC_CHK_JOG_S2, this);
	myBtn2[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_S2);
	myBtn2[13].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[14].SubclassDlgItem(IDC_CHK_JOG_VS2, this);
	myBtn2[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_JOG_VS2);
	myBtn2[14].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[15].SubclassDlgItem(IDC_BTN_ALIGN2, this);
	myBtn2[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN2);
	myBtn2[16].SubclassDlgItem(IDC_CHK_MARKING_TEST2, this);
	myBtn2[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MARKING_TEST2);
	myBtn2[16].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[17].SubclassDlgItem(IDC_CHK_RES_POS_ST2, this);
	myBtn2[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RES_POS_ST2);
	myBtn2[17].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[18].SubclassDlgItem(IDC_BTN_START_SAVE2, this);
	myBtn2[18].SetHwnd(this->GetSafeHwnd(), IDC_BTN_START_SAVE2);
	myBtn2[19].SubclassDlgItem(IDC_CHK_MK_OFFSET_ST2, this);
	myBtn2[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ST2);
	myBtn2[19].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[20].SubclassDlgItem(IDC_CHK_MK_OFFSET_ED2, this);
	myBtn2[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_OFFSET_ED2);
	myBtn2[20].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[21].SubclassDlgItem(IDC_BTN_ALIGN_MOVE2, this);
	myBtn2[21].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ALIGN_MOVE2);
	myBtn2[22].SubclassDlgItem(IDC_BTN_GRAB2, this);
	myBtn2[22].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GRAB2);

	myBtn2[23].SubclassDlgItem(IDC_BTN_MK_POS2_MEAS, this);
	myBtn2[23].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS2_MEAS);
	myBtn2[24].SubclassDlgItem(IDC_CHK_MK_POS2_1, this);
	myBtn2[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_1);
	myBtn2[24].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[25].SubclassDlgItem(IDC_CHK_MK_POS2_2, this);
	myBtn2[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_2);
	myBtn2[25].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[26].SubclassDlgItem(IDC_CHK_MK_POS2_3, this);
	myBtn2[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_3);
	myBtn2[26].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[27].SubclassDlgItem(IDC_CHK_MK_POS2_4, this);
	myBtn2[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_POS2_4);
	myBtn2[27].SetBtnType(BTN_TYPE_CHECK);
	myBtn2[28].SubclassDlgItem(IDC_BTN_MK_POS2_MOVE, this);
	myBtn2[28].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_POS2_MOVE);

	int i;
	for (i = 0; i < MAX_MENU02_BTN_DUO; i++)
	{
		myBtn2[i].SetFont(_T("굴림체"), 16, TRUE);
		myBtn2[i].SetTextColor(RGB_BLACK);
	}

	myBtn2[19].EnableWindow(TRUE);	// IDC_CHK_MK_OFFSET_ST
	myBtn2[20].EnableWindow(FALSE);	// IDC_CHK_MK_OFFSET_ED
}

BOOL CDlgMenu02::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMenu02::OnShowWindow(BOOL bShow, UINT nStatus)
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

void CDlgMenu02::AtDlgShow()
{
	//CString str;
	//myStcData[5].SetText(pDoc->WorkingInfo.Vision[0].sMkOffsetX);
	//myStcData[6].SetText(pDoc->WorkingInfo.Vision[0].sMkOffsetY);

	//myStcData[12].SetText(pDoc->WorkingInfo.Vision[0].sStdScr);

	//myStcData2[5].SetText(pDoc->WorkingInfo.Vision[1].sMkOffsetX);
	//myStcData2[6].SetText(pDoc->WorkingInfo.Vision[1].sMkOffsetY);

	//myStcData2[12].SetText(pDoc->WorkingInfo.Vision[1].sStdScr);

	//if (myBtn2[16].GetCheck())
	//{
	//	m_lChk = 1;
	//	SetTimer(TIM_SHOW_MK_TEST, 50, NULL);
	//}
	//if (myBtn[16].GetCheck())
	//{
	//	m_lChk = 2;
	//	SetTimer(TIM_SHOW_ELEC_TEST, 50, NULL);
	//}

	//SetJogSpd();
	//SetJogSpd2();

	//m_nMoveAlign[0] = 0;
	//m_nMoveAlign[1] = 0;
}

void CDlgMenu02::AtDlgHide()
{
	//if (m_pDlgUtil03)
	//{
	//	if (m_pDlgUtil03->IsWindowVisible())
	//	{
	//		m_pDlgUtil03->ShowWindow(SW_HIDE);
	//	}
	//}
}

void CDlgMenu02::SetLight(int nVal)
{
	if (pView->m_mgrPunch)
	{
		pView->m_mgrPunch->SetLight(nVal);
		nVal = pView->m_mgrPunch->GetLight();
	}

	if (nVal)
	{
		if (!myBtn[0].GetCheck())
			myBtn[0].SetCheck(TRUE);
	}
	else
	{
		if (myBtn[0].GetCheck())
			myBtn[0].SetCheck(FALSE);
	}

	int nPos;
	nPos = int(100.0*(1.0 - (nVal - 0.5) / 255.0));
	m_LightSlider.SetPos(nPos);

	CString str;
	str.Format(_T("%d"), nVal);
	myStcData[2].SetText(str);
}

void CDlgMenu02::SetLight2(int nVal)
{
	if (nVal)
	{
		if (!myBtn2[0].GetCheck())
			myBtn2[0].SetCheck(TRUE);
	}
	else
	{
		if (myBtn2[0].GetCheck())
			myBtn2[0].SetCheck(FALSE);
	}

	int nPos;
	nPos = int(100.0*(1.0 - (nVal - 0.5) / 255.0));
	m_LightSlider2.SetPos(nPos);
	CString str;
	str.Format(_T("%d"), nVal);
	myStcData2[2].SetText(str);

	pDoc->WorkingInfo.Light.sVal[1] = str;
	::WritePrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);
}

void CDlgMenu02::ResetLight()
{
	//if (pView->m_mgrPunch->m_pLight)
	//{
	//	if (myBtn[0].GetCheck())
	//		myBtn[0].SetCheck(FALSE);

	//	pView->m_mgrPunch->m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
	//}
}

void CDlgMenu02::ResetLight2()
{
	//if (pView->m_mgrPunch->m_pLight)
	//{
	//	if (myBtn2[0].GetCheck())
	//		myBtn2[0].SetCheck(FALSE);

	//	pView->m_mgrPunch->m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));
	//}
}



