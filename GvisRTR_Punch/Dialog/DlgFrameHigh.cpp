// DlgFrameHigh.cpp : ���� �����Դϴ�.
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

// CDlgFrameHigh ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgFrameHigh, CDialog)

CDlgFrameHigh::CDlgFrameHigh(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_FRAME_HIGH, pParent)
{

}

CDlgFrameHigh::~CDlgFrameHigh()
{
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
END_MESSAGE_MAP()


// CDlgFrameHigh �޽��� ó�����Դϴ�.

void CDlgFrameHigh::ChkMenu01()
{
	// TODO: Add your control notification handler code here
	//SetChk(IDC_CHK_MENU_01);
	pView->ShowDlg(IDD_DLG_MENU_01);
}

void CDlgFrameHigh::OnBnClickedChkMenu01()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//if (myChkMenu01.GetCheck())
	//{
	//	SetChk(IDC_CHK_MENU_01);
	//	return;
	//}
	ChkMenu01();
}


void CDlgFrameHigh::OnBnClickedChkMenu02()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CDlgFrameHigh::OnBnClickedChkMenu03()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CDlgFrameHigh::OnBnClickedChkMenu04()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CDlgFrameHigh::OnBnClickedChkMenu05()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CDlgFrameHigh::OnBnClickedChkMenu06()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
