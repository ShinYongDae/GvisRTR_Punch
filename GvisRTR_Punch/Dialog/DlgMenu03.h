#pragma once

#include "MyBtn.h"
#include "MyLabel.h"
#include "MyGroup.h"

#define MAX_MENU03_STC				93
#define MAX_MENU03_BTN				87
#define MAX_MENU03_GRP				10
#define MAX_MENU03_LABEL			5

// CDlgMenu03 ��ȭ �����Դϴ�.

class CDlgMenu03 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu03)

	CRect* m_pRect;

	CMyBtn myBtn[MAX_MENU03_BTN];
	CMyLabel myStcTitle[MAX_MENU03_STC];
	CMyGroup myGrp[MAX_MENU03_GRP];
	CMyLabel myLabel[MAX_MENU03_LABEL];

	BOOL Create();
	void LoadImg();
	void DelImg();
	void InitGui();
	void InitStatic();
	void InitBtn();
	void InitGroup();
	void InitLabel();

	void AtDlgShow();
	void AtDlgHide();

public:
	CDlgMenu03(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu03();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_03 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
