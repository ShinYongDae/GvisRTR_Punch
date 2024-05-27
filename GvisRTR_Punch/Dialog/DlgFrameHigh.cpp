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
	m_pRect = NULL;

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
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgFrameHigh 메시지 처리기입니다.

void CDlgFrameHigh::LoadImg()
{
	//myChkMenu01.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu01.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu01.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	//myChkMenu02.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu02.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu02.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	//myChkMenu03.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu03.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu03.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	//myChkMenu04.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu04.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu04.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	//myChkMenu05.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
	myChkMenu05.LoadBkImage(IMG_BTN_UP_DlgFrameHigh, BTN_IMG_UP);
	myChkMenu05.LoadBkImage(IMG_BTN_DN_DlgFrameHigh, BTN_IMG_DN);

	//myChkMenu06.LoadImage(ICO_CHK_DlgFrameHigh, BTN_IMG_DN, CSize(30, 30));
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

void CDlgFrameHigh::InitGui()
{
	InitLabel();
	InitBtn();
	InitPic();
	InitStatic();

	LoadImg();

	myStc[6].SetText(pDoc->WorkingInfo.LastJob.sMkLastShot);
	//m_nMkLastShot = _tstoi(pDoc->WorkingInfo.LastJob.sMkLastShot);
}

void CDlgFrameHigh::InitStatic()
{
	myStc[0].SubclassDlgItem(IDC_STC_0_0, this);	// Feeding Offset
	myStc[1].SubclassDlgItem(IDC_STC_0_1, this);	// MK
	myStc[2].SubclassDlgItem(IDC_STC_0_2, this);	// MK-Value
	myStc[3].SubclassDlgItem(IDC_STC_0_3, this);	// DN
	myStc[4].SubclassDlgItem(IDC_STC_0_4, this);	// DN-Value

	myStc[5].SubclassDlgItem(IDC_STC_0_5, this);	// Last
	myStc[6].SubclassDlgItem(IDC_STC_0_6, this);	// Last-Value

	myStc[7].SubclassDlgItem(IDC_STC_0_7, this);	// UP
	myStc[8].SubclassDlgItem(IDC_STC_0_8, this);	// UP-Value

	myStc[9].SubclassDlgItem(IDC_STC_0_9, this);	// LS
	myStc[10].SubclassDlgItem(IDC_STC_0_10, this);	// LS-Value

	for (int i = 0; i < MAX_FRMHIGH_STC; i++)
	{
		myStc[i].SetFontName(_T("Arial"));
		myStc[i].SetFontBold(TRUE);
		switch (i)
		{
		case 0:
		case 5:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_NAVY);
			myStc[i].SetBkColor(RGB_ORANGE);
			break;
		case 1:
		case 3:
		case 7:
		case 9:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_NAVY);
			myStc[i].SetBkColor(RGB_LTYELLOW);
			break;
		case 2:
		case 4:
		case 8:
		case 10:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_RED);
			myStc[i].SetBkColor(RGB_WHITE);
			break;
		case 6:
			myStc[i].SetFontSize(14);
			myStc[i].SetTextColor(RGB_RED);
			myStc[i].SetBkColor(RGB_WHITE);
			break;
		}
	}

}

void CDlgFrameHigh::InitPic()
{
	myPic.Load(IMG_BK_DlgFrameHigh);
}

void CDlgFrameHigh::InitBtn()
{
	myChkMenu01.SetFont(_T("굴림체"), 16, TRUE);
	myChkMenu01.SetTextColor(RGB_BLACK);
	myChkMenu01.SubclassDlgItem(IDC_CHK_MENU_01, this);
	myChkMenu01.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_01);

	myChkMenu02.SetFont(_T("굴림체"), 16, TRUE);
	myChkMenu02.SetTextColor(RGB_BLACK);
	myChkMenu02.SubclassDlgItem(IDC_CHK_MENU_02, this);
	myChkMenu02.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_02);

	myChkMenu03.SetFont(_T("굴림체"), 16, TRUE);
	myChkMenu03.SetTextColor(RGB_BLACK);
	myChkMenu03.SubclassDlgItem(IDC_CHK_MENU_03, this);
	myChkMenu03.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_03);

	myChkMenu04.SetFont(_T("굴림체"), 16, TRUE);
	myChkMenu04.SetTextColor(RGB_BLACK);
	myChkMenu04.SubclassDlgItem(IDC_CHK_MENU_04, this);
	myChkMenu04.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_04);

	myChkMenu05.SetFont(_T("굴림체"), 16, TRUE);
	myChkMenu05.SetTextColor(RGB_BLACK);
	myChkMenu05.SubclassDlgItem(IDC_CHK_MENU_05, this);
	myChkMenu05.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_05);

	myChkMenu06.SetFont(_T("굴림체"), 16, TRUE);
	myChkMenu06.SetTextColor(RGB_BLACK);
	myChkMenu06.SubclassDlgItem(IDC_CHK_MENU_06, this);
	myChkMenu06.SetHwnd(this->GetSafeHwnd(), IDC_CHK_MENU_06);
}

void CDlgFrameHigh::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_SIG00, this);	// pDoc->m_pSliceIo[10] & (0x01<<1);		// Out - 검사부 검사 테이블 진공 SOL
	//myLabel[1].SubclassDlgItem(IDC_STC_SIG01, this);	// pDoc->m_pSliceIo[10] & (0x01<<0);		// Out - 검사부 검사 시작
	//myLabel[2].SubclassDlgItem(IDC_STC_SIG02, this);	// pDoc->m_pSliceIo[5] & (0x01<<1));		// In - 검사부 테이블 진공 완료
	myLabel[3].SubclassDlgItem(IDC_STC_SIG03, this);	// pDoc->m_pSliceIo[5] & (0x01<<0));		// In - 검사부 검사 완료
	myLabel[4].SubclassDlgItem(IDC_STC_SIG04, this);	// pDoc->m_pSliceIo[5] & (0x01<<0));		// In - 검사부 검사 완료

	for (int i = 0; i < MAX_FRMHIGH_LABEL; i++)
	{
		if( i==1 || i==2 )
			continue;

		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(18);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_DARKRED);
		myLabel[i].SetImageBk(LBL_IMG_UP);
	}
}

BOOL CDlgFrameHigh::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgFrameHigh::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_pRect)
	{
		m_pRect = new CRect;

		this->GetClientRect(m_pRect);
		m_pRect->top = 22;
		m_pRect->bottom += 22;
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


void CDlgFrameHigh::AtDlgShow()
{
	LoadImg();
}

void CDlgFrameHigh::AtDlgHide()
{

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

