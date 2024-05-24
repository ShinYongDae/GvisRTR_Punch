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
	Create();
}

CDlgMenu03::~CDlgMenu03()
{
	DelImg();
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
	LoadImg();
}

BOOL CDlgMenu03::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	InitGui();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
