// DlgMenu04.cpp : 구현 파일입니다.
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

// CDlgMenu04 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMenu04, CDialog)

CDlgMenu04::CDlgMenu04(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_04, pParent)
{
	m_pRect = NULL;
	Create();
}

CDlgMenu04::~CDlgMenu04()
{
	DelImg();

	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

void CDlgMenu04::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu04, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgMenu04 메시지 처리기입니다.

BOOL CDlgMenu04::Create()
{
	return CDialog::Create(CDlgMenu04::IDD);
}

void CDlgMenu04::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU04_BTN; i++)
	{
		if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4)
			continue;

		switch (i)
		{
		case 0:
		case 1:
		case 2:
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu04, BTN_IMG_DN);
			break;
		case 5:
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_UP_DlgMenu04, BTN_IMG_DN);
			break;
		case 6:
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_JOG_DN_DlgMenu04, BTN_IMG_DN);
			break;
		default:
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu04, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu04, BTN_IMG_DN);
			break;
		}
	}
}

void CDlgMenu04::DelImg()
{
	int i;
	for (i = 0; i < MAX_MENU04_BTN; i++)
	{
		if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4)
			continue;

		myBtn[i].DelImgList();
	}
}

void CDlgMenu04::InitGui()
{
	InitStatic();
	InitBtn();

	LoadImg();
}

void CDlgMenu04::InitBtn()
{
	//myBtn[0].SubclassDlgItem(IDC_CHK_0, this);
	//myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_0);
	//myBtn[0].SetBoarder(FALSE);
	//myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[1].SubclassDlgItem(IDC_CHK_1, this);
	//myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1);
	//myBtn[1].SetBoarder(FALSE);
	//myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[2].SubclassDlgItem(IDC_CHK_2, this);
	//myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2);
	//myBtn[2].SetBoarder(FALSE);
	//myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[3].SubclassDlgItem(IDC_CHK_10, this);
	//myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_10);
	//myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[4].SubclassDlgItem(IDC_CHK_11, this);
	//myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_11);
	//myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_BTN_BUFF_UP2, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_UP2);
	myBtn[5].SetBoarder(FALSE);

	myBtn[6].SubclassDlgItem(IDC_BTN_BUFF_DN2, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_DN2);
	myBtn[6].SetBoarder(FALSE);

	myBtn[7].SubclassDlgItem(IDC_BTN_BUFF_HOME2, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_HOME2);

	myBtn[8].SubclassDlgItem(IDC_BTN_BUFF_INIT_MOVE2, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_INIT_MOVE2);

	myBtn[9].SubclassDlgItem(IDC_BTN_BUFF_INIT_SAVE2, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_BTN_BUFF_INIT_SAVE2);

	myBtn[10].SubclassDlgItem(IDC_BTN_RESET_MK_L, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_BTN_RESET_MK_L);

	myBtn[11].SubclassDlgItem(IDC_BTN_RESET_MK_R, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_BTN_RESET_MK_R);

	int i;
	for (i = 0; i < MAX_MENU04_BTN; i++)
	{
		if (i==0||i==1||i==2||i==3||i==4)
			continue;

		myBtn[i].SetFont(_T("굴림체"), 16, TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
		//myBtn[i].SetBtnType(BTN_TYPE_CHECK);
	}
}

void CDlgMenu04::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgMenu04::InitStcTitle()
{
	myStcTitle[0].SubclassDlgItem(IDC_STC_000, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_001, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_002, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_003, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_005, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_006, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_008, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_009, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_011, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_012, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_013, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_015, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_016, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_017, this);
	myStcTitle[14].SubclassDlgItem(IDC_STC_019, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_020, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_022, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_023, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_025, this);
	myStcTitle[19].SubclassDlgItem(IDC_STC_100, this);
	myStcTitle[20].SubclassDlgItem(IDC_STC_101, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_102, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_103, this);
	myStcTitle[23].SubclassDlgItem(IDC_STC_105, this);
	myStcTitle[24].SubclassDlgItem(IDC_STC_200, this);
	myStcTitle[25].SubclassDlgItem(IDC_STC_201, this);
	myStcTitle[26].SubclassDlgItem(IDC_STC_202, this);
	myStcTitle[27].SubclassDlgItem(IDC_STC_203, this);
	myStcTitle[28].SubclassDlgItem(IDC_STC_205, this);
	//myStcTitle[29].SubclassDlgItem(IDC_STC_00_3, this);
	//myStcTitle[30].SubclassDlgItem(IDC_STC_00_4, this);
	//myStcTitle[31].SubclassDlgItem(IDC_STC_00_5, this);
	myStcTitle[32].SubclassDlgItem(IDC_STC_24, this);
	myStcTitle[33].SubclassDlgItem(IDC_STC_21, this);
	myStcTitle[34].SubclassDlgItem(IDC_STC_23, this);

	myStcTitle[35].SubclassDlgItem(IDC_STC_106, this);
	myStcTitle[36].SubclassDlgItem(IDC_STC_107, this);
	myStcTitle[37].SubclassDlgItem(IDC_STC_109, this);

	myStcTitle[38].SubclassDlgItem(IDC_STC_206, this);
	myStcTitle[39].SubclassDlgItem(IDC_STC_207, this);
	myStcTitle[40].SubclassDlgItem(IDC_STC_208, this);
	myStcTitle[41].SubclassDlgItem(IDC_STC_210, this);

	myStcTitle[42].SubclassDlgItem(IDC_STC_6, this);
	myStcTitle[43].SubclassDlgItem(IDC_STC_26, this);
	myStcTitle[44].SubclassDlgItem(IDC_STC_27, this);
	myStcTitle[45].SubclassDlgItem(IDC_STC_29, this);
	myStcTitle[46].SubclassDlgItem(IDC_STC_38, this);

	myStcTitle[47].SubclassDlgItem(IDC_STC_BUF_ROL2, this);
	myStcTitle[48].SubclassDlgItem(IDC_STC_BUF_HI2, this);

	myStcTitle[49].SubclassDlgItem(IDC_STC_8, this);
	myStcTitle[50].SubclassDlgItem(IDC_STC_9, this);
	myStcTitle[51].SubclassDlgItem(IDC_STC_50, this);
	myStcTitle[52].SubclassDlgItem(IDC_STC_39, this);
	myStcTitle[53].SubclassDlgItem(IDC_STC_49, this);

	myStcTitle[54].SubclassDlgItem(IDC_STC_54, this);
	myStcTitle[55].SubclassDlgItem(IDC_STC_51, this);
	myStcTitle[56].SubclassDlgItem(IDC_STC_53, this);

	myStcTitle[57].SubclassDlgItem(IDC_STC_145, this);
	myStcTitle[58].SubclassDlgItem(IDC_STC_155, this);
	myStcTitle[59].SubclassDlgItem(IDC_STC_90, this);

	myStcTitle[60].SubclassDlgItem(IDC_STC_146, this);
	myStcTitle[61].SubclassDlgItem(IDC_STC_156, this);
	myStcTitle[62].SubclassDlgItem(IDC_STC_91, this);

	myStcTitle[63].SubclassDlgItem(IDC_STC_150, this);
	myStcTitle[64].SubclassDlgItem(IDC_STC_147, this);
	myStcTitle[65].SubclassDlgItem(IDC_STC_154, this);
	myStcTitle[66].SubclassDlgItem(IDC_STC_151, this);
	myStcTitle[67].SubclassDlgItem(IDC_STC_160, this);
	myStcTitle[68].SubclassDlgItem(IDC_STC_157, this);
	myStcTitle[69].SubclassDlgItem(IDC_STC_167, this);
	myStcTitle[70].SubclassDlgItem(IDC_STC_161, this);
	myStcTitle[71].SubclassDlgItem(IDC_STC_168, this);
	myStcTitle[72].SubclassDlgItem(IDC_STC_164, this);
	myStcTitle[73].SubclassDlgItem(IDC_STC_172, this);
	myStcTitle[74].SubclassDlgItem(IDC_STC_169, this);
	myStcTitle[75].SubclassDlgItem(IDC_STC_60, this);
	myStcTitle[76].SubclassDlgItem(IDC_STC_57, this);
	myStcTitle[77].SubclassDlgItem(IDC_STC_61, this);
	myStcTitle[78].SubclassDlgItem(IDC_STC_64, this);
	myStcTitle[79].SubclassDlgItem(IDC_STC_67, this);
	myStcTitle[80].SubclassDlgItem(IDC_STC_73, this);
	myStcTitle[81].SubclassDlgItem(IDC_STC_70, this);
	myStcTitle[82].SubclassDlgItem(IDC_STC_74, this);
	myStcTitle[83].SubclassDlgItem(IDC_STC_77, this);
	myStcTitle[84].SubclassDlgItem(IDC_STC_80, this);
	myStcTitle[85].SubclassDlgItem(IDC_STC_83, this);
	myStcTitle[86].SubclassDlgItem(IDC_STC_95, this);
	myStcTitle[87].SubclassDlgItem(IDC_STC_92, this);
	myStcTitle[88].SubclassDlgItem(IDC_STC_96, this);
	myStcTitle[89].SubclassDlgItem(IDC_STC_99, this);
	myStcTitle[90].SubclassDlgItem(IDC_STC_115, this);
	myStcTitle[91].SubclassDlgItem(IDC_STC_112, this);
	myStcTitle[92].SubclassDlgItem(IDC_STC_116, this);
	myStcTitle[93].SubclassDlgItem(IDC_STC_119, this);
	myStcTitle[94].SubclassDlgItem(IDC_STC_122, this);
	myStcTitle[95].SubclassDlgItem(IDC_STC_128, this);
	myStcTitle[96].SubclassDlgItem(IDC_STC_125, this);
	myStcTitle[97].SubclassDlgItem(IDC_STC_129, this);
	myStcTitle[98].SubclassDlgItem(IDC_STC_132, this);
	myStcTitle[99].SubclassDlgItem(IDC_STC_135, this);
	myStcTitle[100].SubclassDlgItem(IDC_STC_138, this);

	myStcTitle[101].SubclassDlgItem(IDC_STC_149, this);
	myStcTitle[102].SubclassDlgItem(IDC_STC_153, this);
	myStcTitle[103].SubclassDlgItem(IDC_STC_159, this);
	myStcTitle[104].SubclassDlgItem(IDC_STC_163, this);
	myStcTitle[105].SubclassDlgItem(IDC_STC_166, this);
	myStcTitle[106].SubclassDlgItem(IDC_STC_171, this);
	myStcTitle[107].SubclassDlgItem(IDC_STC_56, this);
	myStcTitle[108].SubclassDlgItem(IDC_STC_59, this);
	myStcTitle[109].SubclassDlgItem(IDC_STC_63, this);
	myStcTitle[110].SubclassDlgItem(IDC_STC_66, this);
	myStcTitle[111].SubclassDlgItem(IDC_STC_69, this);
	myStcTitle[112].SubclassDlgItem(IDC_STC_72, this);
	myStcTitle[113].SubclassDlgItem(IDC_STC_76, this);
	myStcTitle[114].SubclassDlgItem(IDC_STC_79, this);
	myStcTitle[115].SubclassDlgItem(IDC_STC_82, this);
	myStcTitle[116].SubclassDlgItem(IDC_STC_85, this);
	myStcTitle[117].SubclassDlgItem(IDC_STC_94, this);
	myStcTitle[118].SubclassDlgItem(IDC_STC_98, this);
	myStcTitle[119].SubclassDlgItem(IDC_STC_111, this);
	myStcTitle[120].SubclassDlgItem(IDC_STC_114, this);
	myStcTitle[121].SubclassDlgItem(IDC_STC_118, this);
	myStcTitle[122].SubclassDlgItem(IDC_STC_121, this);
	myStcTitle[123].SubclassDlgItem(IDC_STC_124, this);
	myStcTitle[124].SubclassDlgItem(IDC_STC_127, this);
	myStcTitle[125].SubclassDlgItem(IDC_STC_131, this);
	myStcTitle[126].SubclassDlgItem(IDC_STC_134, this);
	myStcTitle[127].SubclassDlgItem(IDC_STC_137, this);
	myStcTitle[128].SubclassDlgItem(IDC_STC_140, this);

	myStcTitle[129].SubclassDlgItem(IDC_STC_211, this);
	myStcTitle[130].SubclassDlgItem(IDC_STC_213, this);

	myStcTitle[131].SubclassDlgItem(IDC_STC_179, this);
	myStcTitle[132].SubclassDlgItem(IDC_STC_214, this);
	myStcTitle[133].SubclassDlgItem(IDC_STC_216, this);
	myStcTitle[134].SubclassDlgItem(IDC_STC_219, this);

	for (int i = 0; i < MAX_MENU04_STC; i++)
	{
		if(i==29 || i==30 || i==31)
			continue;

		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);
		switch (i)
		{
		case 0:
		case 19:
		case 24:
		case 38:
		case 49:
		case 57:
		case 58:
		case 59:
		case 131:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 1:
		case 20:
		case 25:
		case 39:
		case 50:
		case 60:
		case 61:
		case 62:
			myStcTitle[i].SetTextColor(RGB_WHITE);
			myStcTitle[i].SetBkColor(RGB_DARKGREEN);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 4:
		case 6:
		case 8:
		case 11:
		case 14:
		case 16:
		case 18:
		case 23:
		case 28:
		case 34:
		case 37:
		case 41:
		case 44:
		case 46:
		case 53:
		case 56:
		case 101:
		case 102:
		case 103:
		case 104:
		case 105:
		case 106:
		case 107:
		case 108:
		case 109:
		case 110:
		case 111:
		case 112:
		case 113:
		case 114:
		case 115:
		case 116:
		case 117:
		case 118:
		case 119:
		case 120:
		case 121:
		case 122:
		case 123:
		case 124:
		case 125:
		case 126:
		case 127:
		case 128:
		case 130:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(FALSE);
			break;
		case 29:
		case 30:
		case 31:
		case 47:
		case 48:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_DLG_FRM);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		default:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTGRAY);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		}
	}
}

void CDlgMenu04::InitStcData()
{
	int i;
	myStcData[0].SubclassDlgItem(IDC_STC_004, this);
	myStcData[1].SubclassDlgItem(IDC_STC_007, this);
	myStcData[2].SubclassDlgItem(IDC_STC_010, this);
	myStcData[3].SubclassDlgItem(IDC_STC_014, this);
	myStcData[4].SubclassDlgItem(IDC_STC_018, this);
	myStcData[5].SubclassDlgItem(IDC_STC_021, this);
	myStcData[6].SubclassDlgItem(IDC_STC_024, this);
	myStcData[7].SubclassDlgItem(IDC_STC_104, this);
	myStcData[8].SubclassDlgItem(IDC_STC_204, this);
	myStcData[9].SubclassDlgItem(IDC_STC_22, this);
	myStcData[10].SubclassDlgItem(IDC_STC_108, this);
	myStcData[11].SubclassDlgItem(IDC_STC_209, this);
	myStcData[12].SubclassDlgItem(IDC_STC_25, this);
	myStcData[13].SubclassDlgItem(IDC_STC_28, this);
	myStcData[14].SubclassDlgItem(IDC_STC_BUF_POS2, this);
	myStcData[15].SubclassDlgItem(IDC_STC_48, this);
	myStcData[16].SubclassDlgItem(IDC_STC_52, this);

	myStcData[17].SubclassDlgItem(IDC_STC_148, this);
	myStcData[18].SubclassDlgItem(IDC_STC_152, this);
	myStcData[19].SubclassDlgItem(IDC_STC_158, this);
	myStcData[20].SubclassDlgItem(IDC_STC_162, this);
	myStcData[21].SubclassDlgItem(IDC_STC_165, this);
	myStcData[22].SubclassDlgItem(IDC_STC_170, this);
	myStcData[23].SubclassDlgItem(IDC_STC_55, this);
	myStcData[24].SubclassDlgItem(IDC_STC_58, this);
	myStcData[25].SubclassDlgItem(IDC_STC_62, this);
	myStcData[26].SubclassDlgItem(IDC_STC_65, this);
	myStcData[27].SubclassDlgItem(IDC_STC_68, this);
	myStcData[28].SubclassDlgItem(IDC_STC_71, this);
	myStcData[29].SubclassDlgItem(IDC_STC_86, this);
	myStcData[30].SubclassDlgItem(IDC_STC_75, this);
	myStcData[31].SubclassDlgItem(IDC_STC_87, this);
	myStcData[32].SubclassDlgItem(IDC_STC_78, this);
	myStcData[33].SubclassDlgItem(IDC_STC_88, this);
	myStcData[34].SubclassDlgItem(IDC_STC_81, this);
	myStcData[35].SubclassDlgItem(IDC_STC_89, this);
	myStcData[36].SubclassDlgItem(IDC_STC_84, this);
	myStcData[37].SubclassDlgItem(IDC_STC_93, this);
	myStcData[38].SubclassDlgItem(IDC_STC_97, this);
	myStcData[39].SubclassDlgItem(IDC_STC_110, this);
	myStcData[40].SubclassDlgItem(IDC_STC_113, this);
	myStcData[41].SubclassDlgItem(IDC_STC_117, this);
	myStcData[42].SubclassDlgItem(IDC_STC_120, this);
	myStcData[43].SubclassDlgItem(IDC_STC_123, this);
	myStcData[44].SubclassDlgItem(IDC_STC_126, this);
	myStcData[45].SubclassDlgItem(IDC_STC_141, this);
	myStcData[46].SubclassDlgItem(IDC_STC_130, this);
	myStcData[47].SubclassDlgItem(IDC_STC_142, this);
	myStcData[48].SubclassDlgItem(IDC_STC_133, this);
	myStcData[49].SubclassDlgItem(IDC_STC_143, this);
	myStcData[50].SubclassDlgItem(IDC_STC_136, this);
	myStcData[51].SubclassDlgItem(IDC_STC_144, this);
	myStcData[52].SubclassDlgItem(IDC_STC_139, this);

	myStcData[53].SubclassDlgItem(IDC_STC_212, this);
	for (i = 0; i < 54; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(20);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}

	myStcData[54].SubclassDlgItem(IDC_STC_215, this);
	myStcData[55].SubclassDlgItem(IDC_STC_221, this);

	myStcData[56].SubclassDlgItem(IDC_STC_220, this);
	myStcData[57].SubclassDlgItem(IDC_STC_222, this);

	for (i = 54; i < MAX_MENU04_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(15);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

BOOL CDlgMenu04::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMenu04::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_pRect)
	{
		m_pRect = new CRect;

		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if (bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu04::AtDlgShow()
{
	//Disp();
}

void CDlgMenu04::AtDlgHide()
{
}

