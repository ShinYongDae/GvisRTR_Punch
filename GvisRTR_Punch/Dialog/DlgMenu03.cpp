// DlgMenu03.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu03.h"
#include "afxdialogex.h"


// CDlgMenu03 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu03, CDialog)

CDlgMenu03::CDlgMenu03(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_03, pParent)
{

}

CDlgMenu03::~CDlgMenu03()
{
}

void CDlgMenu03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu03, CDialog)
END_MESSAGE_MAP()


// CDlgMenu03 �޽��� ó�����Դϴ�.

BOOL CDlgMenu03::Create()
{
	return CDialog::Create(CDlgMenu03::IDD);
}
