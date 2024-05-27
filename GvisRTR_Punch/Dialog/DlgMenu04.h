#pragma once

#include "MyBtn.h"
#include "MyStatic.h"

#define MAX_MENU04_STC				135
#define MAX_MENU04_STC_DATA			58
#define MAX_MENU04_BTN				12

// CDlgMenu04 ��ȭ �����Դϴ�.

class CDlgMenu04 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu04)

	CRect* m_pRect;

	CMyBtn myBtn[MAX_MENU04_BTN];
	CMyStatic myStcTitle[MAX_MENU04_STC];
	CMyStatic myStcData[MAX_MENU04_STC_DATA];

	BOOL Create();
	void LoadImg();
	void DelImg();
	void InitGui();
	void InitStatic();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();

	void AtDlgShow();
	void AtDlgHide();

public:
	CDlgMenu04(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu04();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_04 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
