// DlgMyPassword.cpp : 구현 파일입니다.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMyPassword.h"
#include "afxdialogex.h"

#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;


// CDlgMyPassword 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgMyPassword, CDialog)

CDlgMyPassword::CDlgMyPassword(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MY_PASSWORD, pParent)
{
	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_nSel = 0;
	//pView->m_bShowModalMyPassword = TRUE;
	m_nRtnVal = 0;
	m_strMsg = _T("");
	m_nCtrlId = 0;
	m_bTIM_DLG_MY_PASSWORD_Lock = FALSE;

	m_bFont = FALSE;
}

CDlgMyPassword::~CDlgMyPassword()
{
	AtDlgHide();

	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if (m_bFont)
	{
		if (m_Font.DeleteObject())
			m_bFont = FALSE;
	}

	//pView->m_bShowModalMyPassword = FALSE;
}

void CDlgMyPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMyPassword, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_SYM00, &CDlgMyPassword::OnBnClickedButtonSym00)
	ON_BN_CLICKED(IDC_BUTTON_SYM01, &CDlgMyPassword::OnBnClickedButtonSym01)
	ON_BN_CLICKED(IDC_BUTTON_SYM02, &CDlgMyPassword::OnBnClickedButtonSym02)
	ON_BN_CLICKED(IDC_BUTTON_SYM03, &CDlgMyPassword::OnBnClickedButtonSym03)
	ON_BN_CLICKED(IDC_BUTTON_SYM04, &CDlgMyPassword::OnBnClickedButtonSym04)
	ON_BN_CLICKED(IDC_BUTTON_SYM05, &CDlgMyPassword::OnBnClickedButtonSym05)
	ON_BN_CLICKED(IDC_BUTTON_SYM06, &CDlgMyPassword::OnBnClickedButtonSym06)
	ON_BN_CLICKED(IDC_BUTTON_SYM07, &CDlgMyPassword::OnBnClickedButtonSym07)
	ON_BN_CLICKED(IDC_BUTTON_SYM08, &CDlgMyPassword::OnBnClickedButtonSym08)
	ON_BN_CLICKED(IDC_BUTTON_SYM09, &CDlgMyPassword::OnBnClickedButtonSym09)
	ON_BN_CLICKED(IDC_BUTTON_SYM10, &CDlgMyPassword::OnBnClickedButtonSym10)
	ON_BN_CLICKED(IDC_BUTTON_SYM11, &CDlgMyPassword::OnBnClickedButtonSym11)
	ON_BN_CLICKED(IDC_BUTTON_NUM1, &CDlgMyPassword::OnBnClickedButtonNum1)
	ON_BN_CLICKED(IDC_BUTTON_NUM2, &CDlgMyPassword::OnBnClickedButtonNum2)
	ON_BN_CLICKED(IDC_BUTTON_NUM3, &CDlgMyPassword::OnBnClickedButtonNum3)
	ON_BN_CLICKED(IDC_BUTTON_NUM4, &CDlgMyPassword::OnBnClickedButtonNum4)
	ON_BN_CLICKED(IDC_BUTTON_NUM5, &CDlgMyPassword::OnBnClickedButtonNum5)
	ON_BN_CLICKED(IDC_BUTTON_NUM6, &CDlgMyPassword::OnBnClickedButtonNum6)
	ON_BN_CLICKED(IDC_BUTTON_NUM7, &CDlgMyPassword::OnBnClickedButtonNum7)
	ON_BN_CLICKED(IDC_BUTTON_NUM8, &CDlgMyPassword::OnBnClickedButtonNum8)
	ON_BN_CLICKED(IDC_BUTTON_NUM9, &CDlgMyPassword::OnBnClickedButtonNum9)
	ON_BN_CLICKED(IDC_BUTTON_NUM0, &CDlgMyPassword::OnBnClickedButtonNum0)
	ON_BN_CLICKED(IDC_BUTTON_MINUS, &CDlgMyPassword::OnBnClickedButtonMinus)
	ON_BN_CLICKED(IDC_BUTTON_EQUAL, &CDlgMyPassword::OnBnClickedButtonEqual)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_Q, &CDlgMyPassword::OnBnClickedButtonCharQ)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_W, &CDlgMyPassword::OnBnClickedButtonCharW)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_E, &CDlgMyPassword::OnBnClickedButtonCharE)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_R, &CDlgMyPassword::OnBnClickedButtonCharR)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_T, &CDlgMyPassword::OnBnClickedButtonCharT)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_Y, &CDlgMyPassword::OnBnClickedButtonCharY)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_U, &CDlgMyPassword::OnBnClickedButtonCharU)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_I, &CDlgMyPassword::OnBnClickedButtonCharI)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_O, &CDlgMyPassword::OnBnClickedButtonCharO)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_P, &CDlgMyPassword::OnBnClickedButtonCharP)
	ON_BN_CLICKED(IDC_BUTTON_ESC, &CDlgMyPassword::OnBnClickedButtonEsc)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_A, &CDlgMyPassword::OnBnClickedButtonCharA)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_S, &CDlgMyPassword::OnBnClickedButtonCharS)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_D, &CDlgMyPassword::OnBnClickedButtonCharD)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_F, &CDlgMyPassword::OnBnClickedButtonCharF)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_G, &CDlgMyPassword::OnBnClickedButtonCharG)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_H, &CDlgMyPassword::OnBnClickedButtonCharH)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_J, &CDlgMyPassword::OnBnClickedButtonCharJ)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_K, &CDlgMyPassword::OnBnClickedButtonCharK)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_L, &CDlgMyPassword::OnBnClickedButtonCharL)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_Z, &CDlgMyPassword::OnBnClickedButtonCharZ)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_X, &CDlgMyPassword::OnBnClickedButtonCharX)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_C, &CDlgMyPassword::OnBnClickedButtonCharC)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_V, &CDlgMyPassword::OnBnClickedButtonCharV)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_B, &CDlgMyPassword::OnBnClickedButtonCharB)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_N, &CDlgMyPassword::OnBnClickedButtonCharN)
	ON_BN_CLICKED(IDC_BUTTON_CHAR_M, &CDlgMyPassword::OnBnClickedButtonCharM)
	ON_BN_CLICKED(IDC_BUTTON_COMMA, &CDlgMyPassword::OnBnClickedButtonComma)
	ON_BN_CLICKED(IDC_BUTTON_DOT, &CDlgMyPassword::OnBnClickedButtonDot)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, &CDlgMyPassword::OnBnClickedButtonEnter)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CDlgMyPassword::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_SPACE, &CDlgMyPassword::OnBnClickedButtonSpace)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CDlgMyPassword::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CDlgMyPassword::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_BS, &CDlgMyPassword::OnBnClickedButtonBs)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgMyPassword::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_CHECK_SHIFT, &CDlgMyPassword::OnBnClickedCheckShift)
	ON_EN_SETFOCUS(IDC_EDIT_11, &CDlgMyPassword::OnSetfocusEdit11)
	ON_EN_SETFOCUS(IDC_EDIT_21, &CDlgMyPassword::OnSetfocusEdit21)
	ON_EN_CHANGE(IDC_EDIT_11, &CDlgMyPassword::OnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT_21, &CDlgMyPassword::OnChangeEdit21)
	ON_BN_CLICKED(IDC_BTN_00, &CDlgMyPassword::OnBnClickedBtn00)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgMyPassword 메시지 처리기입니다.


BOOL CDlgMyPassword::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	CString sData;

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			GetDlgItem(IDC_EDIT_21)->GetWindowText(sData);
			if(sData == pDoc->WorkingInfo.System.sPassword)
				m_nRtnVal = 1;

			OnOK();
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		}
	}	

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgMyPassword::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMyPassword::AtDlgShow()
{
	LoadImg();
}

void CDlgMyPassword::AtDlgHide()
{
	m_bTIM_DLG_MY_PASSWORD = FALSE;
	DelImg();
}

void CDlgMyPassword::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;
	myBtn.LoadBkImage(IMG_BTN_UP_DlgMyPassword, BTN_IMG_UP);
	myBtn.LoadBkImage(IMG_BTN_DN_DlgMyPassword, BTN_IMG_DN);
	
	myLabel.LoadBkImage(IMG_BK_LT_YELLOW);
}

void CDlgMyPassword::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	myBtn.DelImgList();
	myLabel.DelImgList();
}


BOOL CDlgMyPassword::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetFont(_T("굴림체"), 24, TRUE);

	InitPic();
	InitLabel();
	InitEdit();
	InitGroup();
	InitBtn();

	m_nSel = 0;
	GetDlgItem(IDC_EDIT_11)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgMyPassword::SetMsg(CString strMsg, int nCtrlId)
{
	m_strMsg = strMsg;
	m_nCtrlId = nCtrlId;
}

void CDlgMyPassword::SetFont(CString srtFntName, int nSize, BOOL bBold)
{
	if (m_bFont)
	{
		if (m_Font.DeleteObject())
			m_bFont = FALSE;
	}

	LOGFONT lfCtrl = { 0 };
	lfCtrl.lfOrientation = 0;
	lfCtrl.lfEscapement = 0;

	lfCtrl.lfHeight = nSize;
	lfCtrl.lfWeight = bBold ? FW_BOLD : FW_NORMAL;

	lfCtrl.lfItalic = FALSE;
	lfCtrl.lfUnderline = FALSE;
	lfCtrl.lfStrikeOut = FALSE;

	lfCtrl.lfCharSet = DEFAULT_CHARSET;
	lfCtrl.lfQuality = DEFAULT_QUALITY;
	lfCtrl.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfCtrl.lfPitchAndFamily = DEFAULT_PITCH;
	_tcscpy(lfCtrl.lfFaceName, srtFntName);

	if (!m_bFont)
	{
		BOOL bCr = m_Font.CreateFontIndirect(&lfCtrl);
		if (bCr)
			m_bFont = TRUE;
	}
}

void CDlgMyPassword::KillFocus(int nID)
{
	SetTimer(TIM_KILL_FOCUS, 50, NULL);
}

void CDlgMyPassword::InitLabel()
{
	myLabel.SubclassDlgItem(IDC_STC_20, this);
	myLabel.SetTextColor(RGB_ORANGE);

	myLabel.SetFontName(_T("Arial"));
	myLabel.SetFontSize(28);
	myLabel.SetFontBold(TRUE);
	myLabel.SetBkColor(RGB_WHITE);
	myLabel.DrawBoarder();
}

void CDlgMyPassword::InitEdit()
{
	myEdit[0].SubclassDlgItem(IDC_EDIT_11, this);
	myEdit[1].SubclassDlgItem(IDC_EDIT_21, this);

	myEdit[0].SetFontName(_T("Arial"));
	myEdit[0].SetFontSize(FONT_SIZE_PW + LINE_SPACE_PW);
	myEdit[0].SetFontBold(TRUE);
	myEdit[0].SetTextColor(RGB_RED);
	myEdit[0].SetBkColor(RGB_LTYELLOW);
	myEdit[0].SetText(_T(""));
	myEdit[0].SetFont((CFont*)&myEdit[0].m_font, TRUE);
	KillFocus(IDC_EDIT_11);

	myEdit[1].SetFontName(_T("Arial"));
	myEdit[1].SetFontSize(32);
	myEdit[1].SetFontBold(TRUE);
	myEdit[1].SetTextColor(RGB_BLACK);
	myEdit[1].SetBkColor(RGB_WHITE);
	myEdit[1].SetFont((CFont*)&myEdit[1].m_font, TRUE);

}

void CDlgMyPassword::InitGroup()
{
	myGroup.SubclassDlgItem(IDC_GROUP_00, this);
	myGroup.SetHwnd(this->GetSafeHwnd(), IDC_GROUP_00);
	myGroup.SetBkColor(RGB_BLACK);
}

void CDlgMyPassword::InitBtn()
{
	myBtn.SubclassDlgItem(IDC_BTN_00, this);
	myBtn.SetHwnd(this->GetSafeHwnd(), IDC_BTN_00);
	myBtn.SetFont(_T("굴림체"), 24, TRUE);
	myBtn.SetTextColor(RGB_BLACK);

	GetDlgItem(IDC_BUTTON_SYM00)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM01)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM02)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM03)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM04)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM05)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM06)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM07)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM08)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM09)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM10)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SYM11)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM1)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM2)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM3)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM4)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM5)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM6)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM7)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM8)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM9)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_NUM0)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_MINUS)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_EQUAL)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_Q)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_W)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_E)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_R)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_T)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_Y)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_U)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_I)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_O)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_P)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_A)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_S)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_D)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_F)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_G)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_H)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_J)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_K)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_L)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_Z)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_X)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_C)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_V)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_B)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_N)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CHAR_M)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_ESC)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_ENTER)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_DOT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_CHECK_SHIFT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_CLEAR)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_SPACE)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_LEFT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_RIGHT)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_BS)->SetFont((CFont*)&m_Font, TRUE);
	GetDlgItem(IDC_BUTTON_DEL)->SetFont((CFont*)&m_Font, TRUE);
}

void CDlgMyPassword::InitPic()
{
	myPic.Load(IMG_BK_LT_RED);
}


void CDlgMyPassword::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CRect rect(0, 0, 0, 0);
	GetClientRect(&rect);
	myPic.Show(&dc, rect);
}



void CDlgMyPassword::OnBnClickedButtonSym00()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11, WM_CHAR, 0x21, 0);
	else if (m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21, WM_CHAR, 0x21, 0);
}


void CDlgMyPassword::OnBnClickedButtonSym01()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11, WM_CHAR, 0x40, 0);
	else if (m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21, WM_CHAR, 0x40, 0);
}


void CDlgMyPassword::OnBnClickedButtonSym02()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11, WM_CHAR, 0x23, 0);
	else if (m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21, WM_CHAR, 0x23, 0);
}


void CDlgMyPassword::OnBnClickedButtonSym03()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x24,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x24,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym04()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x25,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x25,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym05()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x5E,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x5E,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym06()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x26,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x26,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym07()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2A,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2A,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym08()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x28,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x28,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym09()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x29,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x29,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x7B,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x7B,0);	
}


void CDlgMyPassword::OnBnClickedButtonSym11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x7D,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x7D,0);	
}


void CDlgMyPassword::OnBnClickedButtonNum1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x31,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x31,0);
}


void CDlgMyPassword::OnBnClickedButtonNum2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x32,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x32,0);
}


void CDlgMyPassword::OnBnClickedButtonNum3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x33,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x33,0);
}


void CDlgMyPassword::OnBnClickedButtonNum4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x34,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x34,0);
}


void CDlgMyPassword::OnBnClickedButtonNum5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x35,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x35,0);
}


void CDlgMyPassword::OnBnClickedButtonNum6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x36,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x36,0);
}


void CDlgMyPassword::OnBnClickedButtonNum7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x37,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x37,0);
}


void CDlgMyPassword::OnBnClickedButtonNum8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x38,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x38,0);
}


void CDlgMyPassword::OnBnClickedButtonNum9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x39,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x39,0);
}

void CDlgMyPassword::OnBnClickedButtonNum0()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x30,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x30,0);
}


void CDlgMyPassword::OnBnClickedButtonMinus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2D,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2D,0);
}


void CDlgMyPassword::OnBnClickedButtonEqual()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x3D,0);		
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x3D,0);		
}


void CDlgMyPassword::OnBnClickedButtonCharQ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x51+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x51,0);
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x51+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x51,0);
	}
}


void CDlgMyPassword::OnBnClickedButtonCharW()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x57+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x57,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x57+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x57,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharE()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x45+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x45,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x45+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x45,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharR()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x52+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x52,0);			
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x52+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x52,0);			
	}
}


void CDlgMyPassword::OnBnClickedButtonCharT()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x54+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x54,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x54+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x54,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharY()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x59+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x59,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x59+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x59,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonCharU()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x55+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x55,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x55+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x55,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonCharI()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x49+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x49,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x49+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x49,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonCharO()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4F+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4F,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4F+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4F,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharP()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x50+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x50,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x50+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x50,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonEsc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgMyPassword::OnBnClickedButtonCharA()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x41+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x41,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x41+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x41,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharS()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x53+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x53,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x53+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x53,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharD()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x44+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x44,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x44+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x44,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonCharF()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x46+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x46,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x46+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x46,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharG()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x47+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x47,0);			
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x47+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x47,0);			
	}
}


void CDlgMyPassword::OnBnClickedButtonCharH()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x48+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x48,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x48+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x48,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonCharJ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4A,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4A,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharK()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4B+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4B,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4B+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4B,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonCharL()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4C+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4C,0);		
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4C+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4C,0);		
	}
}


void CDlgMyPassword::OnBnClickedButtonCharZ()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x5A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x5A,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x5A+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x5A,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharX()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x58+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x58,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x58+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x58,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharC()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x43+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x43,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x43+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x43,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharV()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x56+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x56,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x56+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x56,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharB()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x42+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x42,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x42+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x42,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharN()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4E + 0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4E,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4E + 0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4E,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonCharM()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4D+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x4D,0);	
	}
	else if(m_nSel == 1)
	{
		if(((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck())
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4D+0x20,0);
		else
			SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x4D,0);	
	}
}


void CDlgMyPassword::OnBnClickedButtonComma()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2C,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2C,0);	
}


void CDlgMyPassword::OnBnClickedButtonDot()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x2E,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x2E,0);
}


void CDlgMyPassword::OnBnClickedButtonEnter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sData;
	GetDlgItem(IDC_EDIT_21)->GetWindowText(sData);
	if(sData == pDoc->WorkingInfo.System.sPassword)
		m_nRtnVal = 1;

	OnOK();
}


void CDlgMyPassword::OnBnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		GetDlgItem(IDC_EDIT_11)->SetWindowText(_T(""));
	else if(m_nSel == 1)
		GetDlgItem(IDC_EDIT_21)->SetWindowText(_T(""));
}


void CDlgMyPassword::OnBnClickedButtonSpace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,0x20,0);	
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,0x20,0);	
}


void CDlgMyPassword::OnBnClickedButtonLeft()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_KEYDOWN,VK_LEFT,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_KEYDOWN,VK_LEFT,0);
}


void CDlgMyPassword::OnBnClickedButtonRight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_KEYDOWN,VK_RIGHT,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_KEYDOWN,VK_RIGHT,0);
}


void CDlgMyPassword::OnBnClickedButtonBs()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_CHAR,VK_BACK,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_CHAR,VK_BACK,0);
}


void CDlgMyPassword::OnBnClickedButtonDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_nSel == 0)
		SendDlgItemMessage(IDC_EDIT_11,WM_KEYDOWN,VK_DELETE,0);
	else if(m_nSel == 1)
		SendDlgItemMessage(IDC_EDIT_21,WM_KEYDOWN,VK_DELETE,0);
}


void CDlgMyPassword::OnBnClickedCheckShift()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK_SHIFT))->GetCheck();
	if (bOn)
	{
		GetDlgItem(IDC_BUTTON_CHAR_Q)->SetWindowText(_T("q"));
		GetDlgItem(IDC_BUTTON_CHAR_W)->SetWindowText(_T("w"));
		GetDlgItem(IDC_BUTTON_CHAR_E)->SetWindowText(_T("e"));
		GetDlgItem(IDC_BUTTON_CHAR_R)->SetWindowText(_T("r"));
		GetDlgItem(IDC_BUTTON_CHAR_T)->SetWindowText(_T("t"));
		GetDlgItem(IDC_BUTTON_CHAR_Y)->SetWindowText(_T("y"));
		GetDlgItem(IDC_BUTTON_CHAR_U)->SetWindowText(_T("u"));
		GetDlgItem(IDC_BUTTON_CHAR_I)->SetWindowText(_T("i"));
		GetDlgItem(IDC_BUTTON_CHAR_O)->SetWindowText(_T("o"));
		GetDlgItem(IDC_BUTTON_CHAR_P)->SetWindowText(_T("p"));
		GetDlgItem(IDC_BUTTON_CHAR_A)->SetWindowText(_T("a"));
		GetDlgItem(IDC_BUTTON_CHAR_S)->SetWindowText(_T("s"));
		GetDlgItem(IDC_BUTTON_CHAR_D)->SetWindowText(_T("d"));
		GetDlgItem(IDC_BUTTON_CHAR_F)->SetWindowText(_T("f"));
		GetDlgItem(IDC_BUTTON_CHAR_G)->SetWindowText(_T("g"));
		GetDlgItem(IDC_BUTTON_CHAR_H)->SetWindowText(_T("h"));
		GetDlgItem(IDC_BUTTON_CHAR_J)->SetWindowText(_T("j"));
		GetDlgItem(IDC_BUTTON_CHAR_K)->SetWindowText(_T("k"));
		GetDlgItem(IDC_BUTTON_CHAR_L)->SetWindowText(_T("l"));
		GetDlgItem(IDC_BUTTON_CHAR_Z)->SetWindowText(_T("z"));
		GetDlgItem(IDC_BUTTON_CHAR_X)->SetWindowText(_T("x"));
		GetDlgItem(IDC_BUTTON_CHAR_C)->SetWindowText(_T("c"));
		GetDlgItem(IDC_BUTTON_CHAR_V)->SetWindowText(_T("v"));
		GetDlgItem(IDC_BUTTON_CHAR_B)->SetWindowText(_T("b"));
		GetDlgItem(IDC_BUTTON_CHAR_N)->SetWindowText(_T("n"));
		GetDlgItem(IDC_BUTTON_CHAR_M)->SetWindowText(_T("m"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_CHAR_Q)->SetWindowText(_T("Q"));
		GetDlgItem(IDC_BUTTON_CHAR_W)->SetWindowText(_T("W"));
		GetDlgItem(IDC_BUTTON_CHAR_E)->SetWindowText(_T("E"));
		GetDlgItem(IDC_BUTTON_CHAR_R)->SetWindowText(_T("R"));
		GetDlgItem(IDC_BUTTON_CHAR_T)->SetWindowText(_T("T"));
		GetDlgItem(IDC_BUTTON_CHAR_Y)->SetWindowText(_T("Y"));
		GetDlgItem(IDC_BUTTON_CHAR_U)->SetWindowText(_T("U"));
		GetDlgItem(IDC_BUTTON_CHAR_I)->SetWindowText(_T("I"));
		GetDlgItem(IDC_BUTTON_CHAR_O)->SetWindowText(_T("O"));
		GetDlgItem(IDC_BUTTON_CHAR_P)->SetWindowText(_T("P"));
		GetDlgItem(IDC_BUTTON_CHAR_A)->SetWindowText(_T("A"));
		GetDlgItem(IDC_BUTTON_CHAR_S)->SetWindowText(_T("S"));
		GetDlgItem(IDC_BUTTON_CHAR_D)->SetWindowText(_T("D"));
		GetDlgItem(IDC_BUTTON_CHAR_F)->SetWindowText(_T("F"));
		GetDlgItem(IDC_BUTTON_CHAR_G)->SetWindowText(_T("G"));
		GetDlgItem(IDC_BUTTON_CHAR_H)->SetWindowText(_T("H"));
		GetDlgItem(IDC_BUTTON_CHAR_J)->SetWindowText(_T("J"));
		GetDlgItem(IDC_BUTTON_CHAR_K)->SetWindowText(_T("K"));
		GetDlgItem(IDC_BUTTON_CHAR_L)->SetWindowText(_T("L"));
		GetDlgItem(IDC_BUTTON_CHAR_Z)->SetWindowText(_T("Z"));
		GetDlgItem(IDC_BUTTON_CHAR_X)->SetWindowText(_T("X"));
		GetDlgItem(IDC_BUTTON_CHAR_C)->SetWindowText(_T("C"));
		GetDlgItem(IDC_BUTTON_CHAR_V)->SetWindowText(_T("V"));
		GetDlgItem(IDC_BUTTON_CHAR_B)->SetWindowText(_T("B"));
		GetDlgItem(IDC_BUTTON_CHAR_N)->SetWindowText(_T("N"));
		GetDlgItem(IDC_BUTTON_CHAR_M)->SetWindowText(_T("M"));
	}
}


void CDlgMyPassword::OnSetfocusEdit11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSel = 0;
}


void CDlgMyPassword::OnSetfocusEdit21()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nSel = 1;
}


void CDlgMyPassword::OnChangeEdit11()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	GetDlgItem(IDC_EDIT_11)->GetWindowText(str);
}


void CDlgMyPassword::OnChangeEdit21()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	GetDlgItem(IDC_EDIT_21)->GetWindowText(str);
}


void CDlgMyPassword::OnBnClickedBtn00()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sData;
	GetDlgItem(IDC_EDIT_21)->GetWindowText(sData);
	if (sData == pDoc->WorkingInfo.System.sPassword)
		m_nRtnVal = 1;

	OnOK();
}


void CDlgMyPassword::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIM_KILL_FOCUS)
	{
		KillTimer(TIM_KILL_FOCUS);

		myEdit[0].SetFontSize(FONT_SIZE_PW);
		myEdit[0].SetText(m_strMsg);
		GetDlgItem(IDC_BTN_00)->SetFocus();
	}

	CDialog::OnTimer(nIDEvent);
}
