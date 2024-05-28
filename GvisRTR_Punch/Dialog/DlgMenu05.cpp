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
	m_pRect = NULL;
	Create();
}

CDlgMenu05::~CDlgMenu05()
{
	DelImg();

	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

void CDlgMenu05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu05, CDialog)
	ON_WM_SHOWWINDOW()
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
	InitLabel();
	InitStc();

	LoadImg();
}

void CDlgMenu05::InitStc()
{
	myStcLot.SubclassDlgItem(IDC_STC_LOT, this);
	myStcLot.SetFontName(_T("Arial"));
	myStcLot.SetFontSize(14);
	myStcLot.SetFontBold(TRUE);
	myStcLot.SetTextColor(RGB_BLACK);
	myStcLot.SetBkColor(RGB_WHITE);

	myStcSerialSt.SubclassDlgItem(IDC_STC_SERIAL_ST, this);
	myStcSerialSt.SetFontName(_T("Arial"));
	myStcSerialSt.SetFontSize(20);
	myStcSerialSt.SetFontBold(TRUE);
	myStcSerialSt.SetTextColor(RGB_RED);
	myStcSerialSt.SetBkColor(RGB_WHITE);

	myStcSerialEd.SubclassDlgItem(IDC_STC_SERIAL_ED, this);
	myStcSerialEd.SetFontName(_T("Arial"));
	myStcSerialEd.SetFontSize(20);
	myStcSerialEd.SetFontBold(TRUE);
	myStcSerialEd.SetTextColor(RGB_RED);
	myStcSerialEd.SetBkColor(RGB_WHITE);

	myStcFromTo.SubclassDlgItem(IDC_STATIC_FROMTO, this);
	myStcFromTo.SetFontName(_T("Arial"));
	myStcFromTo.SetFontSize(26);
	myStcFromTo.SetFontBold(TRUE);
	myStcFromTo.SetTextColor(RGB_BLACK);
	myStcFromTo.SetBkColor(RGB_DLG_FRM);

	myStcProc.SubclassDlgItem(IDC_STC_PROC, this);
	myStcProc.SetFontName(_T("Arial"));
	myStcProc.SetFontSize(14);
	myStcProc.SetFontBold(TRUE);
	myStcProc.SetTextColor(RGB_BLACK);
	myStcProc.SetBkColor(RGB_WHITE);

	myStcIts.SubclassDlgItem(IDC_STC_ITS, this);
	myStcIts.SetFontName(_T("Arial"));
	myStcIts.SetFontSize(14);
	myStcIts.SetFontBold(TRUE);
	myStcIts.SetTextColor(RGB_BLACK);
	myStcIts.SetBkColor(RGB_WHITE);
}

void CDlgMenu05::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_SIG00, this);	// 내층
	myLabel[1].SubclassDlgItem(IDC_STC_SIG01, this);	// 외층

	for (int i = 0; i < MAX_MENU05_LABEL; i++)
	{
		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(18);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_DARKRED);
		myLabel[i].SetImageBk(LBL_IMG_UP);
	}
}

BOOL CDlgMenu05::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	GetDlgItem(IDC_BTN_MES)->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgMenu05::OnShowWindow(BOOL bShow, UINT nStatus)
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

void CDlgMenu05::AtDlgShow()
{
	//ModifyModelData();
	//((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SelectString(0, m_sModel);
	//((CComboBox*)GetDlgItem(IDC_COMBO_MODEL))->SetWindowText(m_sModel);
	//ModifyLotData();

	//DispTestMode();
	//DispItsCode();
}

void CDlgMenu05::AtDlgHide()
{

}

