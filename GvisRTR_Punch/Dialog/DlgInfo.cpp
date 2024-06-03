// DlgInfo.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgInfo.h"
#include "afxdialogex.h"


#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgInfo, CDialog)

CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_INFO, pParent)
{

}

CDlgInfo::~CDlgInfo()
{
	DelImg();
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
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgInfo 메시지 처리기입니다.


void CDlgInfo::LoadImg()
{
	int i;
	for (i = 0; i < MAX_INFO_BTN; i++)
	{
		myBtn[i].LoadBkImage(IMG_BTN_UP_DlgInfo, BTN_IMG_UP);
		myBtn[i].LoadBkImage(IMG_BTN_DN_DlgInfo, BTN_IMG_DN);
	}

	myBtnExit.LoadBkImage(MSG_BTN_BK_UP, BTN_IMG_UP);
	myBtnExit.LoadBkImage(MSG_BTN_BK_DN, BTN_IMG_DN);
	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_UP, CSize(40, 40), BTN_POS_LEFT);
	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_DN, CSize(40, 40), BTN_POS_LEFT);
}

void CDlgInfo::DelImg()
{
	int i;
	for (i = 0; i < MAX_INFO_BTN; i++)
		myBtn[i].DelImgList();

	myBtnExit.DelImgList();
}

void CDlgInfo::InitGui()
{
	InitStatic();
	InitBtn();

	LoadImg();
	GetDlgItem(IDC_STC_181)->SetWindowText(_T("")); // 샘플검사 Shot수
}

void CDlgInfo::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_000, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_000);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_001, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_001);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	myBtn[2].SubclassDlgItem(IDC_CHK_002, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_002);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_003, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_003);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[4].SubclassDlgItem(IDC_CHK_004, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_004);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_005, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_005);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_006, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_006);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_007, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_007);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_008, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_008);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[9].SubclassDlgItem(IDC_CHK_009, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_009);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	myBtn[10].SubclassDlgItem(IDC_CHK_010, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_010);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_011, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_011);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_USE_AOI_DUAL, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_DUAL);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_USE_AOI_SINGLE, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_SINGLE);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_SAMPLE_TEST, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_SAMPLE_TEST);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_CHK_ONE_METAL, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ONE_METAL);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);

	myBtn[16].SubclassDlgItem(IDC_CHK_TWO_METAL, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_TWO_METAL);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[17].SubclassDlgItem(IDC_CHK_2_POINT_ALIGN, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2_POINT_ALIGN);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_CHK_4_POINT_ALIGN, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4_POINT_ALIGN);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);

	myBtn[19].SubclassDlgItem(IDC_CHK_85, this); //하면AOI 클린롤러 
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_85);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);

	myBtn[20].SubclassDlgItem(IDC_CHK_86, this); //상면AOI 클린롤러 
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_86);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_1187, this); //AOI초음파세정기
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1187);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_CHK_1188, this); //각인부초음파세정기
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1188);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);

	myBtn[23].SubclassDlgItem(IDC_CHK_USE_AOI_INNER, this); //WORK_MODE : MODE_INNER
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_INNER);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);

	myBtn[24].SubclassDlgItem(IDC_CHK_USE_AOI_OUTER, this); //WORK_MODE : MODE_OUTER
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_OUTER);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);


	int i;
	for (i = 0; i < MAX_INFO_BTN; i++)
	{
		if (0 == i || 12 == i || 13 == i || 14 == i || 23 == i || 24 == i)
		{
			myBtn[i].SetFont(_T("굴림체"), 16, TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
		}

		if (15 == i || 16 == i)
		{
			myBtn[i].SetFont(_T("돋움체"), 22, TRUE);
		}

		if (0 != i && 12 != i && 13 != i && 14 != i && 15 != i && 16 != i && 23 != i && 24 != i)
		{
			myBtn[i].SetFont(_T("굴림체"), 16, TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
			myBtn[i].SetText(_T("사용\rOff"), BTN_UP);
			myBtn[i].SetText(_T("사용\rON"), BTN_DN);
		}
	}

	myBtnExit.SubclassDlgItem(IDC_BTN_EXIT, this);
	myBtnExit.SetHwnd(this->GetSafeHwnd(), IDC_BTN_EXIT);
	myBtnExit.SetFont(_T("굴림체"), 24, TRUE);
	myBtnExit.SetTextColor(RGB_BLACK);
}

void CDlgInfo::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgInfo::InitStcTitle()
{
	myStcTitle[0].SubclassDlgItem(IDC_STC_0000, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_0001, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_0002, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_0004, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_0005, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_0006, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_0007, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_0009, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_0010, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_0011, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_0013, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_0014, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_0015, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_0017, this);
	myStcTitle[14].SubclassDlgItem(IDC_STC_0018, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_0019, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_0021, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_0022, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_0023, this);
	myStcTitle[19].SubclassDlgItem(IDC_STC_0025, this);
	myStcTitle[20].SubclassDlgItem(IDC_STC_0026, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_0027, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_0028, this);
	myStcTitle[23].SubclassDlgItem(IDC_STC_0029, this);
	myStcTitle[24].SubclassDlgItem(IDC_STC_0031, this);
	myStcTitle[25].SubclassDlgItem(IDC_STC_0032, this);
	myStcTitle[26].SubclassDlgItem(IDC_STC_0033, this);
	myStcTitle[27].SubclassDlgItem(IDC_STC_0035, this);
	myStcTitle[28].SubclassDlgItem(IDC_STC_0036, this);
	myStcTitle[29].SubclassDlgItem(IDC_STC_0038, this);
	myStcTitle[30].SubclassDlgItem(IDC_STC_0039, this);
	myStcTitle[31].SubclassDlgItem(IDC_STC_0040, this);
	myStcTitle[32].SubclassDlgItem(IDC_STC_0041, this);
	myStcTitle[33].SubclassDlgItem(IDC_STC_0042, this);
	myStcTitle[34].SubclassDlgItem(IDC_STC_0043, this);
	myStcTitle[35].SubclassDlgItem(IDC_STC_0044, this);
	myStcTitle[36].SubclassDlgItem(IDC_STC_0045, this);
	myStcTitle[37].SubclassDlgItem(IDC_STC_0046, this);
	myStcTitle[38].SubclassDlgItem(IDC_STC_59, this);
	myStcTitle[39].SubclassDlgItem(IDC_STC_173, this);
	myStcTitle[40].SubclassDlgItem(IDC_STC_175, this);
	myStcTitle[41].SubclassDlgItem(IDC_STC_176, this);
	myStcTitle[42].SubclassDlgItem(IDC_STC_177, this);
	myStcTitle[43].SubclassDlgItem(IDC_STC_38, this);
	myStcTitle[44].SubclassDlgItem(IDC_STC_62, this);
	myStcTitle[45].SubclassDlgItem(IDC_STC_60, this);
	myStcTitle[46].SubclassDlgItem(IDC_STC_63, this);
	myStcTitle[47].SubclassDlgItem(IDC_STC_64, this);
	myStcTitle[48].SubclassDlgItem(IDC_STC_65, this);
	myStcTitle[49].SubclassDlgItem(IDC_STC_1145, this); //하면AOI 클린롤러
	myStcTitle[50].SubclassDlgItem(IDC_STC_1146, this); //상면AOI 클린롤러
	myStcTitle[51].SubclassDlgItem(IDC_STC_1147, this); //AOI초음파세정기
	myStcTitle[52].SubclassDlgItem(IDC_STC_1148, this); //각인부초음파세정기

	myStcTitle[53].SubclassDlgItem(IDC_STC_30, this); //초음파세정기 동작
	myStcTitle[54].SubclassDlgItem(IDC_STC_31, this); //검사시작 후
	myStcTitle[55].SubclassDlgItem(IDC_STC_33, this); //초

	myStcTitle[56].SubclassDlgItem(IDC_STC_73, this); //고객출하수율
	myStcTitle[57].SubclassDlgItem(IDC_STC_182, this); //양품율
	myStcTitle[58].SubclassDlgItem(IDC_STC_184, this); //%

	myStcTitle[59].SubclassDlgItem(IDC_STC_34, this); //불량 확인
	myStcTitle[60].SubclassDlgItem(IDC_STC_35, this); //Period
	myStcTitle[61].SubclassDlgItem(IDC_STC_37, this); //Shot

	myStcTitle[62].SubclassDlgItem(IDC_STC_16, this); //ITS코드 라벨
	myStcTitle[63].SubclassDlgItem(IDC_STC_40, this); //Shot수 현재값
	myStcTitle[64].SubclassDlgItem(IDC_STC_42, this); //Shot수 설정값

	myStcTitle[65].SubclassDlgItem(IDC_STC_185, this); //검사부 AOI 상하면 재작업 알람 시간
	myStcTitle[66].SubclassDlgItem(IDC_STC_186, this); //마킹부 재작업 알람 시간

	for (int i = 0; i < MAX_INFO_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);

		switch (i)
		{
		case 0:
		case 3:
		case 20:
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 1:
		case 4:
		case 21:
		case 47:
			myStcTitle[i].SetTextColor(RGB_WHITE);
			myStcTitle[i].SetBkColor(RGB_DARKGREEN);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 10:
		case 43:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(FALSE);
			break;
		case 7:
		case 13:
		case 16:
		case 19:
		case 24:
		case 27:
		case 40:
		case 55:
		case 58:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		default:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTYELLOW);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		}
	}
}

void CDlgInfo::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_0003, this);
	myStcData[1].SubclassDlgItem(IDC_STC_0008, this);
	myStcData[2].SubclassDlgItem(IDC_STC_0012, this);
	myStcData[3].SubclassDlgItem(IDC_STC_0016, this);
	myStcData[4].SubclassDlgItem(IDC_STC_0020, this);
	myStcData[5].SubclassDlgItem(IDC_STC_0024, this);
	myStcData[6].SubclassDlgItem(IDC_STC_0030, this);
	myStcData[7].SubclassDlgItem(IDC_STC_0034, this);
	myStcData[8].SubclassDlgItem(IDC_STC_0037, this);
	myStcData[9].SubclassDlgItem(IDC_STC_174, this);
	myStcData[10].SubclassDlgItem(IDC_STC_178, this);
	myStcData[11].SubclassDlgItem(IDC_STC_61, this);
	myStcData[12].SubclassDlgItem(IDC_STC_181, this);

	myStcData[13].SubclassDlgItem(IDC_STC_32, this); // 초음파세정기 동작 검사시작 후 시작시간 [초]
	myStcData[14].SubclassDlgItem(IDC_STC_183, this); // 고객출하수율

	myStcData[15].SubclassDlgItem(IDC_STC_36, this); // 불량 확인 Period [Shot]

	myStcData[16].SubclassDlgItem(IDC_STC_17, this); // ITS코드
	myStcData[17].SubclassDlgItem(IDC_STC_41, this); // Shot수 현재값
	myStcData[18].SubclassDlgItem(IDC_STC_43, this); // Shot수 설정값

	myStcData[19].SubclassDlgItem(IDC_STC_82, this); // 검사부 AOI 상하면 재작업 알람 시간
	myStcData[20].SubclassDlgItem(IDC_STC_83, this); // 마킹부 재작업 알람 시간

	for (int i = 0; i < MAX_INFO_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(20);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

BOOL CDlgInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();
	Disp();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HBRUSH CDlgInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if (nCtlColor == CTLCOLOR_DLG) 
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(RGB(0, 0, 0));
	}

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CDlgInfo::Disp()
{
	CString str;

	myStcData[0].SetText(pDoc->WorkingInfo.LastJob.sSelUserName);
	myStcData[1].SetText(pDoc->WorkingInfo.LastJob.sReelTotLen);
	myStcData[2].SetText(pDoc->WorkingInfo.LastJob.sOnePnlLen);	// pDoc->WorkingInfo.Motion.sMkFdDist
	myStcData[3].SetText(pDoc->WorkingInfo.LastJob.sLotSepLen);
	myStcData[4].SetText(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
	myStcData[5].SetText(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	myStcData[6].SetText(pDoc->WorkingInfo.LastJob.sLmtTotYld);
	myStcData[7].SetText(pDoc->WorkingInfo.LastJob.sLmtPatlYld);
	myStcData[8].SetText(pDoc->WorkingInfo.LastJob.sNumContFixDef);
	myStcData[9].SetText(pDoc->WorkingInfo.LastJob.sStripOutRatio);
	myStcData[10].SetText(pDoc->WorkingInfo.LastJob.sPartialSpd);
	myStcData[11].SetText(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
	myStcData[12].SetText(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	myStcData[13].SetText(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim);
	myStcData[14].SetText(pDoc->WorkingInfo.LastJob.sCustomNeedRatio);
	str.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nVerifyPeriod);
	myStcData[15].SetText(str);
	myStcData[16].SetText(pDoc->WorkingInfo.LastJob.sEngItsCode);		// IDC_STC_17	ITS코드
	myStcData[17].SetText(pDoc->WorkingInfo.LastJob.sCurrentShotNum);	// IDC_STC_41	Shot수 현재값
	myStcData[18].SetText(pDoc->WorkingInfo.LastJob.sSettingShotNum);	// IDC_STC_43	Shot수 설정값
	str.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlarmTimeAoi);
	myStcData[19].SetText(str);											// IDC_STC_82	검사부 AOI 상하면 재작업 알람 시간
	str.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlarmTimePunch);
	myStcData[20].SetText(str);											// IDC_STC_83	마킹부 재작업 알람 시간

	if (pDoc->WorkingInfo.LastJob.bLotSep)
		myBtn[1].SetCheck(TRUE);
	else
		myBtn[1].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bTempPause)
		myBtn[2].SetCheck(TRUE);
	else
		myBtn[2].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bContFixDef)
		myBtn[3].SetCheck(TRUE);
	else
		myBtn[3].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bRclDrSen)
		myBtn[4].SetCheck(TRUE);
	else
		myBtn[4].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bAoiUpDrSen)
		myBtn[5].SetCheck(TRUE);
	else
		myBtn[5].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bMkDrSen)
		myBtn[6].SetCheck(TRUE);
	else
		myBtn[6].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUclDrSen)
		myBtn[7].SetCheck(TRUE);
	else
		myBtn[7].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUse380mm)
		myBtn[8].SetCheck(TRUE);
	else
		myBtn[8].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bStopFixDef)
		myBtn[9].SetCheck(TRUE);
	else
		myBtn[9].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bMkSftySen)
		myBtn[10].SetCheck(TRUE);
	else
		myBtn[10].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen)
		myBtn[11].SetCheck(TRUE);
	else
		myBtn[11].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bDualTest)
	{
		myBtn[12].SetCheck(TRUE);
		myBtn[13].SetCheck(FALSE);
	}
	else
	{
		myBtn[12].SetCheck(FALSE);
		myBtn[13].SetCheck(TRUE);
	}

	if (pDoc->WorkingInfo.LastJob.bSampleTest)
		myBtn[14].SetCheck(TRUE);
	else
		myBtn[14].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bOneMetal)
	{
		myBtn[15].SetCheck(TRUE);
		myBtn[15].SetWindowText(_T("Recoiler\r역방향"));
		myBtn[15].SetTextColor(RGB_DARKRED);
	}
	else
	{
		myBtn[15].SetCheck(FALSE);
		myBtn[15].SetWindowText(_T("Recoiler\r정방향"));
		myBtn[15].SetTextColor(RGB_BLUE);
	}

	if (pDoc->WorkingInfo.LastJob.bTwoMetal)
	{
		myBtn[16].SetCheck(TRUE);
		myBtn[16].SetWindowText(_T("Uncoiler\r역방향"));
		myBtn[16].SetTextColor(RGB_DARKRED);
	}
	else
	{
		myBtn[16].SetCheck(FALSE);
		myBtn[16].SetWindowText(_T("Uncoiler\r정방향"));
		myBtn[16].SetTextColor(RGB_BLUE);
	}

	if (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler)
		myBtn[19].SetCheck(TRUE);
	else
		myBtn[19].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler)
		myBtn[20].SetCheck(TRUE);
	else
		myBtn[20].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic) //AOI초음파세정기
		myBtn[21].SetCheck(TRUE);
	else
		myBtn[21].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic) //각인부초음파세정기
		myBtn[22].SetCheck(TRUE);
	else
		myBtn[22].SetCheck(FALSE);

	switch (pDoc->WorkingInfo.LastJob.nTestMode) // GetTestMode()
	{
	case MODE_NONE:		// 0 
		myBtn[23].SetCheck(FALSE); // IDC_CHK_USE_AOI_INNER
		myBtn[24].SetCheck(FALSE); // IDC_CHK_USE_AOI_OUTER
		if (pView)
			pView->EnableItsMode(FALSE);
		break;
	case MODE_INNER:	// 1 
		myBtn[23].SetCheck(TRUE); // IDC_CHK_USE_AOI_INNER
		myBtn[24].SetCheck(FALSE); // IDC_CHK_USE_AOI_OUTER
		if (pView)
			pView->EnableItsMode(FALSE);
		break;
	case MODE_OUTER:	// 2 
		myBtn[23].SetCheck(FALSE); // IDC_CHK_USE_AOI_INNER
		myBtn[24].SetCheck(TRUE); // IDC_CHK_USE_AOI_OUTER
		if (pView)
			pView->EnableItsMode(TRUE);
		break;
	}

	if (pView)
	{
		str.Format(_T("%d"), pView->GetMpeData(2, 7)); // Shot수 현재값
		myStcData[17].SetWindowText(str);

		str.Format(_T("%d"), pView->GetMpeData(7, 6)); //Shot수 설정값
		myStcData[18].SetWindowText(str);

		str.Format(_T("%d"), pView->GetMpeData(0, 10)); //검사부 AOI 상[10]하[11]면 재작업 알람 시간
		myStcData[19].SetWindowText(str);

		str.Format(_T("%d"), pView->GetMpeData(0, 12)); //마킹부[12] 재작업 알람 시간
		myStcData[20].SetWindowText(str);
	}

	if (pView->IsAuto())
	{
		GetDlgItem(IDC_CHK_ONE_METAL)->EnableWindow(FALSE); // myBtn[15] IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
		GetDlgItem(IDC_CHK_TWO_METAL)->EnableWindow(FALSE); // myBtn[16] IDC_CHK_TWO_METAL - Uncoiler\r정방향 CW : FALSE
	}
	else
	{
		GetDlgItem(IDC_CHK_ONE_METAL)->EnableWindow(TRUE); // myBtn[15] IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
		GetDlgItem(IDC_CHK_TWO_METAL)->EnableWindow(TRUE); // myBtn[16] IDC_CHK_TWO_METAL - Uncoiler\r정방향 CW : FALSE
	}

}

