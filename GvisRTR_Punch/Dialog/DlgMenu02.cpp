// DlgMenu02.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu02.h"
#include "afxdialogex.h"


// CDlgMenu02 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMenu02, CDialog)

CDlgMenu02::CDlgMenu02(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_02, pParent)
{

}

CDlgMenu02::~CDlgMenu02()
{
}

void CDlgMenu02::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu02, CDialog)
END_MESSAGE_MAP()


// CDlgMenu02 메시지 처리기입니다.

BOOL CDlgMenu02::Create()
{
	return CDialog::Create(CDlgMenu02::IDD);
}
