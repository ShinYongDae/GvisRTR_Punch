// DlgMenu07.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu07.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu07 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu07, CDialog)

CDlgMenu07::CDlgMenu07(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_07, pParent)
{

}

CDlgMenu07::~CDlgMenu07()
{
}

void CDlgMenu07::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu07, CDialog)
END_MESSAGE_MAP()


// CDlgMenu07 �޽��� ó�����Դϴ�.

BOOL CDlgMenu07::Create()
{
	return CDialog::Create(CDlgMenu07::IDD);
}
