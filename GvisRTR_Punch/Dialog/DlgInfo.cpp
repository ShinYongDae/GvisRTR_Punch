// DlgInfo.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgInfo.h"
#include "afxdialogex.h"


// CDlgInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInfo, CDialog)

CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_INFO, pParent)
{

}

CDlgInfo::~CDlgInfo()
{
}

BOOL CDlgInfo::Create()
{
	return CDialog::Create(CDlgInfo::IDD);
}

void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
END_MESSAGE_MAP()


// CDlgInfo 메시지 처리기입니다.
