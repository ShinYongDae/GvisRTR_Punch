// DlgMenu04.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu04.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu04 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu04, CDialog)

CDlgMenu04::CDlgMenu04(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_04, pParent)
{

}

CDlgMenu04::~CDlgMenu04()
{
}

void CDlgMenu04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu04, CDialog)
END_MESSAGE_MAP()


// CDlgMenu04 �޽��� ó�����Դϴ�.

BOOL CDlgMenu04::Create()
{
	return CDialog::Create(CDlgMenu04::IDD);
}
