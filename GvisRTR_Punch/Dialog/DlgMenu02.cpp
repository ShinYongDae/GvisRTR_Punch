// DlgMenu02.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu02.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

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
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMenu02 메시지 처리기입니다.

BOOL CDlgMenu02::Create()
{
	return CDialog::Create(CDlgMenu02::IDD);
}


void CDlgMenu02::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
}

void CDlgMenu02::SetLight(int nVal)
{
	if (pView->m_mgrPunch)
	{
		pView->m_mgrPunch->SetLight(nVal);
		nVal = pView->m_mgrPunch->GetLight();
	}

	//if (nVal)
	//{
	//	if (!myBtn[0].GetCheck())
	//		myBtn[0].SetCheck(TRUE);
	//}
	//else
	//{
	//	if (myBtn[0].GetCheck())
	//		myBtn[0].SetCheck(FALSE);
	//}

	////	nVal = pView->m_mgrPunch->m_pLight->Get(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));

	//int nPos;
	//nPos = int(100.0*(1.0 - (nVal - 0.5) / 255.0));
	//m_LightSlider.SetPos(nPos);

	//CString str;
	//str.Format(_T("%d"), nVal);
	//myStcData[2].SetText(str);
}

void CDlgMenu02::SetLight2(int nVal)
{
	//if (nVal)
	//{
	//	if (!myBtn2[0].GetCheck())
	//		myBtn2[0].SetCheck(TRUE);
	//}
	//else
	//{
	//	if (myBtn2[0].GetCheck())
	//		myBtn2[0].SetCheck(FALSE);
	//}

	//int nPos;
	//nPos = int(100.0*(1.0 - (nVal - 0.5) / 255.0));
	//m_LightSlider2.SetPos(nPos);
	CString str;
	//str.Format(_T("%d"), nVal);
	//myStcData2[2].SetText(str);

	pDoc->WorkingInfo.Light.sVal[1] = str;
	::WritePrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), str, PATH_WORKING_INFO);
}

void CDlgMenu02::ResetLight()
{
	//if (pView->m_mgrPunch->m_pLight)
	//{
	//	if (myBtn[0].GetCheck())
	//		myBtn[0].SetCheck(FALSE);

	//	pView->m_mgrPunch->m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[0]));
	//}
}

void CDlgMenu02::ResetLight2()
{
	//if (pView->m_mgrPunch->m_pLight)
	//{
	//	if (myBtn2[0].GetCheck())
	//		myBtn2[0].SetCheck(FALSE);

	//	pView->m_mgrPunch->m_pLight->Reset(_tstoi(pDoc->WorkingInfo.Light.sCh[1]));
	//}
}
