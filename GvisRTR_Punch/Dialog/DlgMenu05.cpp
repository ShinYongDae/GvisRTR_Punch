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

}

CDlgMenu05::~CDlgMenu05()
{
}

void CDlgMenu05::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu05, CDialog)
END_MESSAGE_MAP()


// CDlgMenu05 �޽��� ó�����Դϴ�.

BOOL CDlgMenu05::Create()
{
	return CDialog::Create(CDlgMenu05::IDD);
}
