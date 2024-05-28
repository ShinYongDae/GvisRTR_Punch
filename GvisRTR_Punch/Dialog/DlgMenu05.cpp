// DlgMenu05.cpp : ���� �����Դϴ�.
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

// CDlgMenu05 ��ȭ �����Դϴ�.

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


// CDlgMenu05 �޽��� ó�����Դϴ�.

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
	myLabel[0].SubclassDlgItem(IDC_STC_SIG00, this);	// ����
	myLabel[1].SubclassDlgItem(IDC_STC_SIG01, this);	// ����

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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitGui();

	GetDlgItem(IDC_BTN_MES)->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlgMenu05::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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

