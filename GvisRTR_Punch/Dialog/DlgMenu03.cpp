// DlgMenu03.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu03.h"
#include "afxdialogex.h"


// CDlgMenu03 대화 상자입니다.

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


// CDlgMenu03 메시지 처리기입니다.

BOOL CDlgMenu03::Create()
{
	return CDialog::Create(CDlgMenu03::IDD);
}
