// DlgMenu06.cpp : ���� �����Դϴ�.
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

// CDlgMenu06 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu06, CDialog)

CDlgMenu06::CDlgMenu06(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_06, pParent)
{

}

CDlgMenu06::~CDlgMenu06()
{
}

void CDlgMenu06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu06, CDialog)
END_MESSAGE_MAP()


// CDlgMenu06 �޽��� ó�����Դϴ�.

BOOL CDlgMenu06::Create()
{
	return CDialog::Create(CDlgMenu06::IDD);
}
