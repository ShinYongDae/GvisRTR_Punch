#pragma once

#include "MyBtn.h"
#include "MyStatic.h"

#define MAX_MENU04_STC				135
#define MAX_MENU04_STC_DATA			58
#define MAX_MENU04_BTN				12

// CDlgMenu04 대화 상자입니다.

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
	CDlgMenu04(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMenu04();

	// 특성입니다.
public:


	// 작업입니다.
public:

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_04 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
