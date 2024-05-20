// DlgMenu07.cpp : 구현 파일입니다.
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

// CDlgMenu07 대화 상자입니다.

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


// CDlgMenu07 메시지 처리기입니다.

BOOL CDlgMenu07::Create()
{
	return CDialog::Create(CDlgMenu07::IDD);
}
