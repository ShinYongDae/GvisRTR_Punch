// DlgMenu03.cpp : ���� �����Դϴ�.
//

#include "../stdafx.h"
#include "../GvisRTR_Punch.h"
#include "DlgMenu03.h"
#include "afxdialogex.h"

#include "../MainFrm.h"
#include "../GvisRTR_PunchDoc.h"
#include "../GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
extern CGvisRTR_PunchView* pView;

// CDlgMenu03 ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgMenu03, CDialog)

CDlgMenu03::CDlgMenu03(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_MENU_03, pParent)
{
	m_pRect = NULL;
	Create();
}

CDlgMenu03::~CDlgMenu03()
{
	DelImg();

	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

void CDlgMenu03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMenu03, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDlgMenu03 �޽��� ó�����Դϴ�.

BOOL CDlgMenu03::Create()
{
	return CDialog::Create(CDlgMenu03::IDD);
}

void CDlgMenu03::LoadImg()
{
	int i;
	for (i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
		myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
	}

	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		switch (i)
		{
		case 0:		// Run-Main
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 33:	// Stop-Main
		case 49:	// LaserPoint-Punching
		case 50:	// LaserPoint-AOI��
		case 65:	// LaserPoint-AOI��
		case 82:	// LaserPoint-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_RED_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 34:	// �����غ�-Main
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 1:		// Reset-Main
		case 40:	// ����-Uncoiler
		case 47:	// ����-Recoiler
			myBtn[i].LoadBkImage(IMG_CIR_YLO_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_YLO_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 4:		// ��������-Recoiler
		case 9:		// ��������-Punching
		case 17:	// ��������-AOI��
		case 25:	// ��������-Uncoiler
		case 55:	// ��������-AOI��
		case 72:	// ��������-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRY_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 2:		// TorqueMotor-Punching
		case 3:		// TorqueMotor-AOI
		case 5:		// ��ȸ��-Recoiler
		case 6:		// ��ȸ��-Recoiler
		case 10:	// ��ȸ��-Punching
		case 11:	// ��ȸ��-Punching
		case 18:	// ��ȸ��-AOI��
		case 19:	// ��ȸ��-AOI��
		case 26:	// ��ȸ��-Uncoiler
		case 27:	// ��ȸ��-Uncoiler
		case 32:	// ���� ��ȸ��-Uncoiler
		case 39:	// ���� ��ȸ��-Uncoiler
		case 45:	// ���� ��ȸ��-Uncoiler
		case 46:	// ���� ��ȸ��-Uncoiler
		case 56:	// ��ȸ��-AOI��
		case 57:	// ��ȸ��-AOI��
		case 70:	// Core150mm-Recoiler
		case 71:	// Core150mm-Uncoiler
		case 73:	// ��ȸ��-Engraving
		case 74:	// ��ȸ��-Engraving
		case 83:	// TorqueMotor-Engraving
		case 85:	// �����ļ�����-Engrave
		case 86:	// �����ļ�����-AOI_DN
			myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 12:	// �ǵ�����-Punching
		case 13:	// ��ǰǪ��-Punching
		case 20:	// �ǵ�����-AOI��
		case 21:	// ��ǰǪ��-AOI��
		case 28:	// ��ǰ���ѻ��-Uncoiler
		case 29:	// Ŭ���ѷ����-Uncoiler
		case 31:	// ����ô-Uncoiler
		case 35:	// ��ǰô-Uncoiler
		case 36:	// Ŭ���ѷ��������-Uncoiler
		case 41:	// ��ǰô-Recoiler
		case 42:	// ��ǰ���ѻ��-Recoiler
		case 44:	// ����ô-Recoiler
		case 48:	// ���ѻ��-Punching
		case 51:	// �ǵ�Ŭ����-Punching
		case 52:	// �ټ�Ŭ����-Punching
		case 53:	// �ǵ�Ŭ����-AOI��
		case 54:	// �ټ�Ŭ����-AOI��
		case 58:	// �ǵ�����-AOI��
		case 59:	// ��ǰǪ��-AOI��
		case 62:	// �ǵ�Ŭ����-AOI��
		case 63:	// �ټ�Ŭ����-AOI��
		case 66:	// Rewinder����-Recoiler
		case 67:	// Rewinder��ǰ����-Recoiler
		case 75:	// �ǵ�����-Engraving
		case 76:	// ��ǰǪ��-Engraving
		case 79:	// �ǵ�Ŭ����-Engraving
		case 80:	// �ټ�Ŭ����-Engraving
		case 84:	// ���ѻ��-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_DGN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_DGN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 7:		// ��ǰ�����Ż�¿�-Recoiler
		case 8:		// ��ǰ����������-Recoiler
		case 14:	// ���̺��ο�-Punching
		case 15:	// ���̺�����-Punching
		case 22:	// ���̺��ο�-AOI��
		case 23:	// ���̺�����-AOI��
		case 30:	// ��ǰ�����Ż����-Uncoiler
		case 37:	// ��ǰ�����Ż�¿�-Uncoiler
		case 38:	// ��ǰ����������-Uncoiler
		case 43:	// ��ǰ�����Ż����-Recoiler
		case 60:	// ���̺��ο�-AOI��
		case 61:	// ���̺�����-AOI��
		case 77:	// ���̺��ο�-Engraving
		case 78:	// ���̺�����-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_PNK_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_PNK_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 16:	// ���ǳ��̼�-Punching
		case 24:	// ���ǳ��̼�-AOI��	
		case 64:	// ���ǳ��̼�-AOI��	
		case 81:	// ���ǳ��̼�-Engraving	
			myBtn[i].LoadBkImage(IMG_CIR_ORG_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 68:	// InductionMotor-Recoiler
		case 69:	// InductionMotor-Uncoiler	
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		}
	}

	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[34].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
}

void CDlgMenu03::DelImg()
{
	int i;

	for (i = 0; i < MAX_MENU03_LABEL; i++)
		myLabel[i].DelImgList();

	for (i = 0; i < MAX_MENU03_BTN; i++)
		myBtn[i].DelImgList();

	myStcTitle[33].DelImgList();
	myStcTitle[34].DelImgList();
	myStcTitle[35].DelImgList();
}

void CDlgMenu03::InitGui()
{
	InitStatic();
	InitBtn();
	InitGroup();
	InitLabel();

	LoadImg();
}

void CDlgMenu03::InitGroup()
{
	myGrp[0].SubclassDlgItem(IDC_GRP_00, this);
	myGrp[0].SetHwnd(this->GetSafeHwnd(), IDC_GRP_00);	// Main

	myGrp[1].SubclassDlgItem(IDC_GRP_1, this);
	myGrp[1].SetHwnd(this->GetSafeHwnd(), IDC_GRP_1);	// Recoiler

	myGrp[2].SubclassDlgItem(IDC_GRP_02, this);
	myGrp[2].SetHwnd(this->GetSafeHwnd(), IDC_GRP_02);	// Punching

	myGrp[3].SubclassDlgItem(IDC_GRP_4, this);
	myGrp[3].SetHwnd(this->GetSafeHwnd(), IDC_GRP_4);	// AOI��

	myGrp[4].SubclassDlgItem(IDC_GRP_03, this);
	myGrp[4].SetHwnd(this->GetSafeHwnd(), IDC_GRP_03);	// AOI��

	myGrp[5].SubclassDlgItem(IDC_GRP_04, this);
	myGrp[5].SetHwnd(this->GetSafeHwnd(), IDC_GRP_04);	// Uncoiler

	myGrp[6].SubclassDlgItem(IDC_GRP_05, this);
	myGrp[6].SetHwnd(this->GetSafeHwnd(), IDC_GRP_05);	// TorqueMotor

	myGrp[7].SubclassDlgItem(IDC_GRP_16, this);
	myGrp[7].SetHwnd(this->GetSafeHwnd(), IDC_GRP_16);	// InductionMotor

	myGrp[8].SubclassDlgItem(IDC_GRP_17, this);
	myGrp[8].SetHwnd(this->GetSafeHwnd(), IDC_GRP_17);	// Core150mm

	myGrp[9].SubclassDlgItem(IDC_GRP_3, this);
	myGrp[9].SetHwnd(this->GetSafeHwnd(), IDC_GRP_3);	// Engraving

	for (int i = 0; i < MAX_MENU03_GRP; i++)
	{
		myGrp[i].SetFontName(_T("Arial"));
		myGrp[i].SetFontSize(14);
		myGrp[i].SetFontBold(TRUE);
		myGrp[i].SetTextColor(RGB_NAVY);
		myGrp[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::InitBtn()
{
	// Main
	myBtn[0].SubclassDlgItem(IDC_CHK_0, this);			// Run
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_0);
	myBtn[0].SetBoarder(FALSE);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[34].SubclassDlgItem(IDC_CHK_34, this);		// �����غ�
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_CHK_34);
	myBtn[34].SetBoarder(FALSE);
	myBtn[34].SetBtnType(BTN_TYPE_CHECK);

	myBtn[33].SubclassDlgItem(IDC_CHK_33, this);		// Stop
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_CHK_33);
	myBtn[33].SetBoarder(FALSE);
	myBtn[33].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_1, this);			// Reset
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1);
	myBtn[1].SetBoarder(FALSE);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	// Torque Motor
	myBtn[2].SubclassDlgItem(IDC_CHK_2, this);			// ��Ī��
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2);
	myBtn[2].SetBoarder(FALSE);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_3, this);			// �˻��
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_3);
	myBtn[3].SetBoarder(FALSE);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[83].SubclassDlgItem(IDC_CHK_84, this);		// ���κ�
	myBtn[83].SetHwnd(this->GetSafeHwnd(), IDC_CHK_84);
	myBtn[83].SetBoarder(FALSE);
	myBtn[83].SetBtnType(BTN_TYPE_CHECK);

	// ������ ������ �ӵ�
	myBtn[85].SubclassDlgItem(IDC_CHK_87, this);		// ������ ������ �ӵ� - ���κ�
	myBtn[85].SetHwnd(this->GetSafeHwnd(), IDC_CHK_87);
	myBtn[85].SetBoarder(FALSE);
	myBtn[85].SetBtnType(BTN_TYPE_CHECK);

	myBtn[86].SubclassDlgItem(IDC_CHK_88, this);		// ������ ������ �ӵ� - AOI �ϸ�
	myBtn[86].SetHwnd(this->GetSafeHwnd(), IDC_CHK_88);
	myBtn[86].SetBoarder(FALSE);
	myBtn[86].SetBtnType(BTN_TYPE_CHECK);

	// Induction Motor
	myBtn[68].SubclassDlgItem(IDC_CHK_68, this);		// Induction Motor - Recoiler\r������ ON
	myBtn[68].SetHwnd(this->GetSafeHwnd(), IDC_CHK_68);
	myBtn[68].SetBoarder(FALSE);
	myBtn[68].SetBtnType(BTN_TYPE_CHECK);

	myBtn[69].SubclassDlgItem(IDC_CHK_69, this);		// Induction Motor - Uncoiler\r������ ON
	myBtn[69].SetHwnd(this->GetSafeHwnd(), IDC_CHK_69);
	myBtn[69].SetBoarder(FALSE);
	myBtn[69].SetBtnType(BTN_TYPE_CHECK);

	// Core 150mm
	myBtn[70].SubclassDlgItem(IDC_CHK_70, this);		// Core 150mm - Recoiler
	myBtn[70].SetHwnd(this->GetSafeHwnd(), IDC_CHK_70);
	myBtn[70].SetBoarder(FALSE);
	myBtn[70].SetBtnType(BTN_TYPE_CHECK);

	myBtn[71].SubclassDlgItem(IDC_CHK_71, this);		// Core 150mm - Uncoiler
	myBtn[71].SetHwnd(this->GetSafeHwnd(), IDC_CHK_71);
	myBtn[71].SetBoarder(FALSE);
	myBtn[71].SetBtnType(BTN_TYPE_CHECK);

	// Recoiler
	myBtn[4].SubclassDlgItem(IDC_CHK_4, this);			// ��������
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4);
	myBtn[4].SetBoarder(FALSE);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_5, this);			// ����ȸ��
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_5);
	myBtn[5].SetBoarder(FALSE);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_6, this);			// ��ȸ����
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_6);
	myBtn[6].SetBoarder(FALSE);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[41].SubclassDlgItem(IDC_CHK_41, this);		// ��ǰô\rŬ����
	myBtn[41].SetHwnd(this->GetSafeHwnd(), IDC_CHK_41);
	myBtn[41].SetBoarder(FALSE);
	myBtn[41].SetBtnType(BTN_TYPE_CHECK);

	myBtn[42].SubclassDlgItem(IDC_CHK_42, this);		// ��ǰ����\r���/�ϰ�
	myBtn[42].SetHwnd(this->GetSafeHwnd(), IDC_CHK_42);
	myBtn[42].SetBoarder(FALSE);
	myBtn[42].SetBtnType(BTN_TYPE_CHECK);

	myBtn[43].SubclassDlgItem(IDC_CHK_43, this);		// ��ǰ������(��)\r���/�ϰ�
	myBtn[43].SetHwnd(this->GetSafeHwnd(), IDC_CHK_43);
	myBtn[43].SetBoarder(FALSE);
	myBtn[43].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_7, this);			// ��ǰ������(��)\r���/�ϰ�
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_7);
	myBtn[7].SetBoarder(FALSE);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_8, this);			// ��ǰ������\r����
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_8);
	myBtn[8].SetBoarder(FALSE);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[44].SubclassDlgItem(IDC_CHK_44, this);		// ���� ô\rŬ����
	myBtn[44].SetHwnd(this->GetSafeHwnd(), IDC_CHK_44);
	myBtn[44].SetBoarder(FALSE);
	myBtn[44].SetBtnType(BTN_TYPE_CHECK);

	myBtn[45].SubclassDlgItem(IDC_CHK_45, this);		// ���� ��\r����ȸ��
	myBtn[45].SetHwnd(this->GetSafeHwnd(), IDC_CHK_45);
	myBtn[45].SetBoarder(FALSE);
	myBtn[45].SetBtnType(BTN_TYPE_CHECK);

	myBtn[46].SubclassDlgItem(IDC_CHK_46, this);		// ���� ��\r��ȸ����
	myBtn[46].SetHwnd(this->GetSafeHwnd(), IDC_CHK_46);
	myBtn[46].SetBoarder(FALSE);
	myBtn[46].SetBtnType(BTN_TYPE_DEFAULT);

	myBtn[47].SubclassDlgItem(IDC_CHK_47, this);		// Recoiler-����
	myBtn[47].SetHwnd(this->GetSafeHwnd(), IDC_CHK_47);
	myBtn[47].SetBoarder(FALSE);
	myBtn[47].SetBtnType(BTN_TYPE_CHECK);

	myBtn[66].SubclassDlgItem(IDC_CHK_66, this);		// Rewinder\r����
	myBtn[66].SetHwnd(this->GetSafeHwnd(), IDC_CHK_66);
	myBtn[66].SetBoarder(FALSE);
	myBtn[66].SetBtnType(BTN_TYPE_CHECK);

	myBtn[67].SubclassDlgItem(IDC_CHK_67, this);		// Rewinder\r��ǰ&&����
	myBtn[67].SetHwnd(this->GetSafeHwnd(), IDC_CHK_67);
	myBtn[67].SetBoarder(FALSE);
	myBtn[67].SetBtnType(BTN_TYPE_CHECK);

	// Punching
	myBtn[9].SubclassDlgItem(IDC_CHK_9, this);			// ��������
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_9);
	myBtn[9].SetBoarder(FALSE);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	myBtn[10].SubclassDlgItem(IDC_CHK_10, this);		// ����ȸ��
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_10);
	myBtn[10].SetBoarder(FALSE);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_11, this);		// ��ȸ����
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_11);
	myBtn[11].SetBoarder(FALSE);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_12, this);		// �ǵ�����
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_12);
	myBtn[12].SetBoarder(FALSE);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_13, this);		// ��ǰǪ��
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_13);
	myBtn[13].SetBoarder(FALSE);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_14, this);		// ���̺�\r��ο�
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_14);
	myBtn[14].SetBoarder(FALSE);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_CHK_15, this);		// ���̺�\r����
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_15);
	myBtn[15].SetBoarder(FALSE);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);


	myBtn[51].SubclassDlgItem(IDC_CHK_51, this);		// �ǵ�\rŬ����
	myBtn[51].SetHwnd(this->GetSafeHwnd(), IDC_CHK_51);
	myBtn[51].SetBoarder(FALSE);
	myBtn[51].SetBtnType(BTN_TYPE_CHECK);

	myBtn[52].SubclassDlgItem(IDC_CHK_52, this);		// �ټ�\rŬ����
	myBtn[52].SetHwnd(this->GetSafeHwnd(), IDC_CHK_52);
	myBtn[52].SetBoarder(FALSE);
	myBtn[52].SetBtnType(BTN_TYPE_CHECK);

	myBtn[16].SubclassDlgItem(IDC_CHK_16, this);		// ���ǳ�\r�̼�
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_16);
	myBtn[16].SetBoarder(FALSE);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[49].SubclassDlgItem(IDC_CHK_49, this);		// ������\r��ũ
	myBtn[49].SetHwnd(this->GetSafeHwnd(), IDC_CHK_49);
	myBtn[49].SetBoarder(FALSE);
	myBtn[49].SetBtnType(BTN_TYPE_CHECK);

	myBtn[48].SubclassDlgItem(IDC_CHK_48, this);		// ����\r��� / �ϰ�
	myBtn[48].SetHwnd(this->GetSafeHwnd(), IDC_CHK_48);
	myBtn[48].SetBoarder(FALSE);
	myBtn[48].SetBtnType(BTN_TYPE_CHECK);


	// AOI ���.	
	myBtn[17].SubclassDlgItem(IDC_CHK_17, this);		// ��������
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_17);
	myBtn[17].SetBoarder(FALSE);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_CHK_18, this);		// ����ȸ��
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_18);
	myBtn[18].SetBoarder(FALSE);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);

	myBtn[19].SubclassDlgItem(IDC_CHK_19, this);		// ��ȸ����
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_19);
	myBtn[19].SetBoarder(FALSE);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);

	myBtn[20].SubclassDlgItem(IDC_CHK_20, this);		// �ǵ�����
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_20);
	myBtn[20].SetBoarder(FALSE);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_21, this);		// ��ǰǪ��
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_21);
	myBtn[21].SetBoarder(FALSE);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_CHK_22, this);		// ���̺�\r��ο�
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_22);
	myBtn[22].SetBoarder(FALSE);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);

	myBtn[23].SubclassDlgItem(IDC_CHK_23, this);		// ���̺�\r����
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_23);
	myBtn[23].SetBoarder(FALSE);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);

	myBtn[53].SubclassDlgItem(IDC_CHK_53, this);		// �ǵ�\rŬ����
	myBtn[53].SetHwnd(this->GetSafeHwnd(), IDC_CHK_53);
	myBtn[53].SetBoarder(FALSE);
	myBtn[53].SetBtnType(BTN_TYPE_CHECK);

	myBtn[54].SubclassDlgItem(IDC_CHK_54, this);		// �ټ�\rŬ����
	myBtn[54].SetHwnd(this->GetSafeHwnd(), IDC_CHK_54);
	myBtn[54].SetBoarder(FALSE);
	myBtn[54].SetBtnType(BTN_TYPE_CHECK);

	myBtn[24].SubclassDlgItem(IDC_CHK_24, this);		// ���ǳ�\r�̼�
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_24);
	myBtn[24].SetBoarder(FALSE);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);

	myBtn[50].SubclassDlgItem(IDC_CHK_50, this);		// ������\r��ũ
	myBtn[50].SetHwnd(this->GetSafeHwnd(), IDC_CHK_50);
	myBtn[50].SetBoarder(FALSE);
	myBtn[50].SetBtnType(BTN_TYPE_CHECK);


	// Uncoiler
	myBtn[25].SubclassDlgItem(IDC_CHK_25, this);		// ��������
	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_25);
	myBtn[25].SetBoarder(FALSE);
	myBtn[25].SetBtnType(BTN_TYPE_CHECK);

	myBtn[26].SubclassDlgItem(IDC_CHK_26, this);		// ����ȸ��
	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_26);
	myBtn[26].SetBoarder(FALSE);
	myBtn[26].SetBtnType(BTN_TYPE_CHECK);

	myBtn[27].SubclassDlgItem(IDC_CHK_27, this);		// ��ȸ����
	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_27);
	myBtn[27].SetBoarder(FALSE);
	myBtn[27].SetBtnType(BTN_TYPE_CHECK);

	myBtn[28].SubclassDlgItem(IDC_CHK_28, this);		// ��ǰ����\r���/�ϰ�
	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_CHK_28);
	myBtn[28].SetBoarder(FALSE);
	myBtn[28].SetBtnType(BTN_TYPE_CHECK);

	myBtn[29].SubclassDlgItem(IDC_CHK_29, this);		// Ŭ���ѷ�\r���/�ϰ�
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_29);
	myBtn[29].SetBoarder(FALSE);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);

	myBtn[30].SubclassDlgItem(IDC_CHK_30, this);		// ��ǰ������(��)\r���/�ϰ�
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_30);
	myBtn[30].SetBoarder(FALSE);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);

	myBtn[31].SubclassDlgItem(IDC_CHK_31, this);		// ���� ô\rŬ����
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_31);
	myBtn[31].SetBoarder(FALSE);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);

	myBtn[32].SubclassDlgItem(IDC_CHK_32, this);		// ���� ��\r����ȸ��
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_32);
	myBtn[32].SetBoarder(FALSE);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);

	myBtn[35].SubclassDlgItem(IDC_CHK_35, this);		// ��ǰô\rŬ����
	myBtn[35].SetHwnd(this->GetSafeHwnd(), IDC_CHK_35);
	myBtn[35].SetBoarder(FALSE);
	myBtn[35].SetBtnType(BTN_TYPE_CHECK);

	myBtn[36].SubclassDlgItem(IDC_CHK_36, this);		// Ŭ���ѷ�����\r���/�ϰ�
	myBtn[36].SetHwnd(this->GetSafeHwnd(), IDC_CHK_36);
	myBtn[36].SetBoarder(FALSE);
	myBtn[36].SetBtnType(BTN_TYPE_CHECK);

	myBtn[37].SubclassDlgItem(IDC_CHK_37, this);		// ��ǰ������(��)\r���/�ϰ�
	myBtn[37].SetHwnd(this->GetSafeHwnd(), IDC_CHK_37);
	myBtn[37].SetBoarder(FALSE);
	myBtn[37].SetBtnType(BTN_TYPE_CHECK);

	myBtn[38].SubclassDlgItem(IDC_CHK_38, this);		// ��ǰ������\r����
	myBtn[38].SetHwnd(this->GetSafeHwnd(), IDC_CHK_38);
	myBtn[38].SetBoarder(FALSE);
	myBtn[38].SetBtnType(BTN_TYPE_CHECK);

	myBtn[39].SubclassDlgItem(IDC_CHK_39, this);		// ���� ��\r��ȸ����
	myBtn[39].SetHwnd(this->GetSafeHwnd(), IDC_CHK_39);
	myBtn[39].SetBoarder(FALSE);
	myBtn[39].SetBtnType(BTN_TYPE_CHECK);

	myBtn[40].SubclassDlgItem(IDC_CHK_40, this);		// Uncoiler-����
	myBtn[40].SetHwnd(this->GetSafeHwnd(), IDC_CHK_40);
	myBtn[40].SetBoarder(FALSE);
	myBtn[40].SetBtnType(BTN_TYPE_CHECK);


	// AOI �ϸ�.
	myBtn[55].SubclassDlgItem(IDC_CHK_55, this);		// ��������
	myBtn[55].SetHwnd(this->GetSafeHwnd(), IDC_CHK_55);
	myBtn[55].SetBoarder(FALSE);
	myBtn[55].SetBtnType(BTN_TYPE_CHECK);

	myBtn[56].SubclassDlgItem(IDC_CHK_56, this);		// ����ȸ��
	myBtn[56].SetHwnd(this->GetSafeHwnd(), IDC_CHK_56);
	myBtn[56].SetBoarder(FALSE);
	myBtn[56].SetBtnType(BTN_TYPE_CHECK);

	myBtn[57].SubclassDlgItem(IDC_CHK_57, this);		// ��ȸ����
	myBtn[57].SetHwnd(this->GetSafeHwnd(), IDC_CHK_57);
	myBtn[57].SetBoarder(FALSE);
	myBtn[57].SetBtnType(BTN_TYPE_CHECK);

	myBtn[58].SubclassDlgItem(IDC_CHK_58, this);		// �ǵ�����
	myBtn[58].SetHwnd(this->GetSafeHwnd(), IDC_CHK_58);
	myBtn[58].SetBoarder(FALSE);
	myBtn[58].SetBtnType(BTN_TYPE_CHECK);

	myBtn[59].SubclassDlgItem(IDC_CHK_59, this);		// ��ǰǪ��
	myBtn[59].SetHwnd(this->GetSafeHwnd(), IDC_CHK_59);
	myBtn[59].SetBoarder(FALSE);
	myBtn[59].SetBtnType(BTN_TYPE_CHECK);

	myBtn[60].SubclassDlgItem(IDC_CHK_60, this);		// ���̺�\r��ο�
	myBtn[60].SetHwnd(this->GetSafeHwnd(), IDC_CHK_60);
	myBtn[60].SetBoarder(FALSE);
	myBtn[60].SetBtnType(BTN_TYPE_CHECK);

	myBtn[61].SubclassDlgItem(IDC_CHK_61, this);		// ���̺�\r����
	myBtn[61].SetHwnd(this->GetSafeHwnd(), IDC_CHK_61);
	myBtn[61].SetBoarder(FALSE);
	myBtn[61].SetBtnType(BTN_TYPE_CHECK);

	myBtn[62].SubclassDlgItem(IDC_CHK_64, this);		// �ǵ�\rŬ����
	myBtn[62].SetHwnd(this->GetSafeHwnd(), IDC_CHK_64);
	myBtn[62].SetBoarder(FALSE);
	myBtn[62].SetBtnType(BTN_TYPE_CHECK);

	myBtn[63].SubclassDlgItem(IDC_CHK_65, this);		// �ټ�\rŬ����
	myBtn[63].SetHwnd(this->GetSafeHwnd(), IDC_CHK_65);
	myBtn[63].SetBoarder(FALSE);
	myBtn[63].SetBtnType(BTN_TYPE_CHECK);

	myBtn[64].SubclassDlgItem(IDC_CHK_62, this);		// ���ǳ�\r�̼�
	myBtn[64].SetHwnd(this->GetSafeHwnd(), IDC_CHK_62);
	myBtn[64].SetBoarder(FALSE);
	myBtn[64].SetBtnType(BTN_TYPE_CHECK);

	myBtn[65].SubclassDlgItem(IDC_CHK_63, this);		// ������\r��ũ
	myBtn[65].SetHwnd(this->GetSafeHwnd(), IDC_CHK_63);
	myBtn[65].SetBoarder(FALSE);
	myBtn[65].SetBtnType(BTN_TYPE_CHECK);


	// Engraving
	myBtn[72].SubclassDlgItem(IDC_CHK_72, this);		// ��������
	myBtn[72].SetHwnd(this->GetSafeHwnd(), IDC_CHK_72);
	myBtn[72].SetBoarder(FALSE);
	myBtn[72].SetBtnType(BTN_TYPE_CHECK);

	myBtn[73].SubclassDlgItem(IDC_CHK_73, this);		// ����ȸ��
	myBtn[73].SetHwnd(this->GetSafeHwnd(), IDC_CHK_73);
	myBtn[73].SetBoarder(FALSE);
	myBtn[73].SetBtnType(BTN_TYPE_CHECK);

	myBtn[74].SubclassDlgItem(IDC_CHK_74, this);		// ��ȸ����
	myBtn[74].SetHwnd(this->GetSafeHwnd(), IDC_CHK_74);
	myBtn[74].SetBoarder(FALSE);
	myBtn[74].SetBtnType(BTN_TYPE_CHECK);

	myBtn[75].SubclassDlgItem(IDC_CHK_75, this);		// �ǵ�����
	myBtn[75].SetHwnd(this->GetSafeHwnd(), IDC_CHK_75);
	myBtn[75].SetBoarder(FALSE);
	myBtn[75].SetBtnType(BTN_TYPE_CHECK);

	myBtn[76].SubclassDlgItem(IDC_CHK_76, this);		// ��ǰǪ��
	myBtn[76].SetHwnd(this->GetSafeHwnd(), IDC_CHK_76);
	myBtn[76].SetBoarder(FALSE);
	myBtn[76].SetBtnType(BTN_TYPE_CHECK);

	myBtn[77].SubclassDlgItem(IDC_CHK_77, this);		// ���̺�\r��ο�
	myBtn[77].SetHwnd(this->GetSafeHwnd(), IDC_CHK_77);
	myBtn[77].SetBoarder(FALSE);
	myBtn[77].SetBtnType(BTN_TYPE_CHECK);

	myBtn[78].SubclassDlgItem(IDC_CHK_78, this);		// ���̺�\r����
	myBtn[78].SetHwnd(this->GetSafeHwnd(), IDC_CHK_78);
	myBtn[78].SetBoarder(FALSE);
	myBtn[78].SetBtnType(BTN_TYPE_CHECK);

	myBtn[79].SubclassDlgItem(IDC_CHK_82, this);		// �ǵ�\rŬ����
	myBtn[79].SetHwnd(this->GetSafeHwnd(), IDC_CHK_82);
	myBtn[79].SetBoarder(FALSE);
	myBtn[79].SetBtnType(BTN_TYPE_CHECK);

	myBtn[80].SubclassDlgItem(IDC_CHK_83, this);		// �ټ�\rŬ����
	myBtn[80].SetHwnd(this->GetSafeHwnd(), IDC_CHK_83);
	myBtn[80].SetBoarder(FALSE);
	myBtn[80].SetBtnType(BTN_TYPE_CHECK);

	myBtn[81].SubclassDlgItem(IDC_CHK_79, this);		// ���ǳ�\r�̼�
	myBtn[81].SetHwnd(this->GetSafeHwnd(), IDC_CHK_79);
	myBtn[81].SetBoarder(FALSE);
	myBtn[81].SetBtnType(BTN_TYPE_CHECK);

	myBtn[82].SubclassDlgItem(IDC_CHK_81, this);		// ������\r��ũ
	myBtn[82].SetHwnd(this->GetSafeHwnd(), IDC_CHK_81);
	myBtn[82].SetBoarder(FALSE);
	myBtn[82].SetBtnType(BTN_TYPE_CHECK);

	myBtn[84].SubclassDlgItem(IDC_CHK_80, this);		// ����\r���/�ϰ�
	myBtn[84].SetHwnd(this->GetSafeHwnd(), IDC_CHK_80);
	myBtn[84].SetBoarder(FALSE);
	myBtn[84].SetBtnType(BTN_TYPE_CHECK);

	int i;
	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		myBtn[i].SetFont(_T("����ü"), 12, TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
	}
}

void CDlgMenu03::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_SIG1, this);
	myLabel[1].SubclassDlgItem(IDC_STC_SIG2, this);
	myLabel[2].SubclassDlgItem(IDC_STC_SIG3, this);
	myLabel[3].SubclassDlgItem(IDC_STC_SIG4, this);
	myLabel[4].SubclassDlgItem(IDC_STC_SIG5, this);

	for (int i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(18);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_DARKRED);
		myLabel[i].SetImageBk(LBL_IMG_UP);
	}
}

void CDlgMenu03::InitStatic()
{
	// Main
	myStcTitle[0].SubclassDlgItem(IDC_STC_000_, this);		// Run
	myStcTitle[1].SubclassDlgItem(IDC_STC_01_, this);		// Reset
	myStcTitle[39].SubclassDlgItem(IDC_STC_000_3, this);	// Stop
	myStcTitle[40].SubclassDlgItem(IDC_STC_000_4, this);	// �����غ�

	myStcTitle[33].SubclassDlgItem(IDC_STC_02, this);		// LED-�ڵ� 
	myStcTitle[34].SubclassDlgItem(IDC_STC_03, this);		// LED-1ȸ����
	myStcTitle[35].SubclassDlgItem(IDC_STC_04, this);		// LED-���� 

	myStcTitle[36].SubclassDlgItem(IDC_STC_02_, this);		// �ڵ�
	myStcTitle[37].SubclassDlgItem(IDC_STC_03_, this);		// 1ȸ����
	myStcTitle[38].SubclassDlgItem(IDC_STC_04_, this);		// ����

															// TorqueMotor
	myStcTitle[2].SubclassDlgItem(IDC_STC_000_2, this);		// ��Ī��
	myStcTitle[3].SubclassDlgItem(IDC_STC_01_2, this);		// �˻��
	myStcTitle[78].SubclassDlgItem(IDC_STC_000_9, this);	// ���κ�

															// InductionMotor
	myStcTitle[74].SubclassDlgItem(IDC_STC_000_6, this);	// Recoiler������
	myStcTitle[75].SubclassDlgItem(IDC_STC_01_3, this);		// Uncoiler������

															// Core150mm
	myStcTitle[76].SubclassDlgItem(IDC_STC_000_7, this);	// Recoiler
	myStcTitle[77].SubclassDlgItem(IDC_STC_01_4, this);		// Uncoiler

															// Recoiler
	myStcTitle[4].SubclassDlgItem(IDC_STC_10_, this);		// ��������
	myStcTitle[5].SubclassDlgItem(IDC_STC_12_2, this);		// ��ȸ��
	myStcTitle[6].SubclassDlgItem(IDC_STC_12_, this);		// ��ȸ��
	myStcTitle[7].SubclassDlgItem(IDC_STC_12_4, this);		// ��ǰ������(��)���
	myStcTitle[8].SubclassDlgItem(IDC_STC_12_3, this);		// ��ǰ����������
	myStcTitle[47].SubclassDlgItem(IDC_STC_12_32, this);	// ��ǰôŬ����
	myStcTitle[48].SubclassDlgItem(IDC_STC_12_33, this);	// ��ǰ���ѻ��
	myStcTitle[49].SubclassDlgItem(IDC_STC_12_34, this);	// ��ǰ������(��)���
	myStcTitle[50].SubclassDlgItem(IDC_STC_12_36, this);	// ����ôŬ����
	myStcTitle[51].SubclassDlgItem(IDC_STC_12_35, this);	// ��������ȸ��
	myStcTitle[52].SubclassDlgItem(IDC_STC_12_37, this);	// �����ٿ�ȸ��
	myStcTitle[53].SubclassDlgItem(IDC_STC_12_38, this);	// ����
	myStcTitle[72].SubclassDlgItem(IDC_STC_12_55, this);	// Rewinder����
	myStcTitle[73].SubclassDlgItem(IDC_STC_12_56, this);	// Rewinder��ǰ����

															// Punching
	myStcTitle[9].SubclassDlgItem(IDC_STC_20_, this);		// ��������
	myStcTitle[10].SubclassDlgItem(IDC_STC_12_6, this);		// ��ȸ��
	myStcTitle[11].SubclassDlgItem(IDC_STC_12_5, this);		// ��ȸ��
	myStcTitle[12].SubclassDlgItem(IDC_STC_12_8, this);		// �ǵ�����
	myStcTitle[13].SubclassDlgItem(IDC_STC_12_7, this);		// ��ǰǪ��
	myStcTitle[14].SubclassDlgItem(IDC_STC_12_10, this);	// ���̺��ο�
	myStcTitle[15].SubclassDlgItem(IDC_STC_12_9, this);		// ���̺�����
	myStcTitle[16].SubclassDlgItem(IDC_STC_12_12, this);	// ���ǳ��̼�
	myStcTitle[54].SubclassDlgItem(IDC_STC_000_5, this);	// ���ѻ��
	myStcTitle[55].SubclassDlgItem(IDC_STC_12_39, this);	// ��������ũ
	myStcTitle[57].SubclassDlgItem(IDC_STC_12_42, this);	// �ǵ�Ŭ����
	myStcTitle[58].SubclassDlgItem(IDC_STC_12_41, this);	// �ټ�Ŭ����

															// AOI��
	myStcTitle[61].SubclassDlgItem(IDC_STC_30_2, this);		// ��������
	myStcTitle[62].SubclassDlgItem(IDC_STC_12_46, this);	// ��ȸ��
	myStcTitle[63].SubclassDlgItem(IDC_STC_12_45, this);	// ��ȸ��
	myStcTitle[64].SubclassDlgItem(IDC_STC_12_48, this);	// �ǵ�����
	myStcTitle[65].SubclassDlgItem(IDC_STC_12_47, this);	// ��ǰǪ��
	myStcTitle[66].SubclassDlgItem(IDC_STC_12_50, this);	// ���̺��ο�
	myStcTitle[67].SubclassDlgItem(IDC_STC_12_49, this);	// ���̺�����
	myStcTitle[68].SubclassDlgItem(IDC_STC_12_54, this);	// �ǵ�Ŭ����
	myStcTitle[69].SubclassDlgItem(IDC_STC_12_53, this);	// �ټ�Ŭ����
	myStcTitle[70].SubclassDlgItem(IDC_STC_12_51, this);	// ���ǳ��̼�
	myStcTitle[71].SubclassDlgItem(IDC_STC_12_52, this);	// ��������ũ

															// AOI��
	myStcTitle[17].SubclassDlgItem(IDC_STC_30_, this);		// ��������
	myStcTitle[18].SubclassDlgItem(IDC_STC_12_13, this);	// ��ȸ��
	myStcTitle[19].SubclassDlgItem(IDC_STC_12_11, this);	// ��ȸ��
	myStcTitle[20].SubclassDlgItem(IDC_STC_12_15, this);	// �ǵ�����
	myStcTitle[21].SubclassDlgItem(IDC_STC_12_14, this);	// ��ǰǪ��
	myStcTitle[22].SubclassDlgItem(IDC_STC_12_17, this);	// ���̺��ο�
	myStcTitle[23].SubclassDlgItem(IDC_STC_12_16, this);	// ���̺�����
	myStcTitle[24].SubclassDlgItem(IDC_STC_12_19, this);	// ���ǳ��̼�
	myStcTitle[56].SubclassDlgItem(IDC_STC_12_40, this);	// ��������ũ
	myStcTitle[59].SubclassDlgItem(IDC_STC_12_44, this);	// �ǵ�Ŭ����
	myStcTitle[60].SubclassDlgItem(IDC_STC_12_43, this);	// �ټ�Ŭ����

															// Uncoiler
	myStcTitle[25].SubclassDlgItem(IDC_STC_40_, this);		// ��������
	myStcTitle[26].SubclassDlgItem(IDC_STC_12_20, this);	// ��ȸ��
	myStcTitle[27].SubclassDlgItem(IDC_STC_12_18, this);	// ��ȸ��
	myStcTitle[28].SubclassDlgItem(IDC_STC_12_22, this);	// ��ǰ���ѻ��
	myStcTitle[29].SubclassDlgItem(IDC_STC_12_21, this);	// Ŭ���ѷ����
	myStcTitle[30].SubclassDlgItem(IDC_STC_12_24, this);	// ��ǰ������(��)���
	myStcTitle[31].SubclassDlgItem(IDC_STC_12_25, this);	// ����ôŬ����
	myStcTitle[32].SubclassDlgItem(IDC_STC_12_23, this);	// ��������ȸ��
	myStcTitle[41].SubclassDlgItem(IDC_STC_12_26, this);	// ��ǰôŬ����
	myStcTitle[42].SubclassDlgItem(IDC_STC_12_27, this);	// Ŭ���ѷ��������
	myStcTitle[43].SubclassDlgItem(IDC_STC_12_28, this);	// ��ǰ������(��)���
	myStcTitle[44].SubclassDlgItem(IDC_STC_12_29, this);	// ��ǰ����������
	myStcTitle[45].SubclassDlgItem(IDC_STC_12_30, this);	// �����ٿ�ȸ��
	myStcTitle[46].SubclassDlgItem(IDC_STC_12_31, this);	// ����

															// Engraving
	myStcTitle[79].SubclassDlgItem(IDC_STC_20_2, this);		// ��������
	myStcTitle[80].SubclassDlgItem(IDC_STC_12_58, this);	// ��ȸ��
	myStcTitle[81].SubclassDlgItem(IDC_STC_12_57, this);	// ��ȸ��
	myStcTitle[82].SubclassDlgItem(IDC_STC_12_60, this);	// �ǵ�����
	myStcTitle[83].SubclassDlgItem(IDC_STC_12_59, this);	// ��ǰǪ��
	myStcTitle[84].SubclassDlgItem(IDC_STC_12_62, this);	// ���̺��ο�
	myStcTitle[85].SubclassDlgItem(IDC_STC_12_61, this);	// ���̺�����
	myStcTitle[86].SubclassDlgItem(IDC_STC_12_63, this);	// ���ǳ��̼�
	myStcTitle[87].SubclassDlgItem(IDC_STC_000_8, this);	// ���ѻ��
	myStcTitle[88].SubclassDlgItem(IDC_STC_12_64, this);	// ��������ũ
	myStcTitle[89].SubclassDlgItem(IDC_STC_12_66, this);	// �ǵ�Ŭ����
	myStcTitle[90].SubclassDlgItem(IDC_STC_12_65, this);	// �ټ�Ŭ����

															// ������
	myStcTitle[91].SubclassDlgItem(IDC_STC_000_10, this);	// ���κ� ������
	myStcTitle[92].SubclassDlgItem(IDC_STC_000_11, this);	// AOI(��) ������

	for (int i = 0; i < MAX_MENU03_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_DLG_FRM);
	}
}

BOOL CDlgMenu03::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CDlgMenu03::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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

void CDlgMenu03::AtDlgShow()
{
	//SetTimer(TIM_CHK_MREG, 300, NULL);
	//UpdateSignal();
}

void CDlgMenu03::AtDlgHide()
{
}

void CDlgMenu03::SetLed(int nIdx, BOOL bOn)
{
	if (bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_DN)
		myLabel[nIdx].SetImageBk(LBL_IMG_DN);
	else if (!bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_UP)
		myLabel[nIdx].SetImageBk(LBL_IMG_UP);
}


