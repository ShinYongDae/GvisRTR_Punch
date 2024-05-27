#pragma once

#include "MyBtn.h"
#include "MyLabel.h"
#include "MyStatic.h"
#include "MyGL.h"
#include "afxcmn.h"

#define MAX_MENU02_STC				23
#define MAX_MENU02_STC_DATA			17
#define MAX_MENU02_BTN				35

#define MAX_MENU02_STC_DUO			15
#define MAX_MENU02_STC_DATA_DUO		17
#define MAX_MENU02_BTN_DUO			29

// CDlgMenu02 ��ȭ �����Դϴ�.

class CDlgMenu02 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu02)

	CRect* m_pRect;

	CMyStatic myStcPcsImg;//, myStcPinImg;

	CMyBtn myBtn[MAX_MENU02_BTN];
	CMyLabel myStcTitle[MAX_MENU02_STC];
	CMyStatic myStcData[MAX_MENU02_STC_DATA];

	CMyBtn myBtn2[MAX_MENU02_BTN_DUO];
	CMyLabel myStcTitle2[MAX_MENU02_STC_DUO];
	CMyStatic myStcData2[MAX_MENU02_STC_DATA_DUO];


	BOOL Create();
	void LoadImg();
	void DelImg();
	void InitGui();
	void InitSlider();
	void InitStatic();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();
	void InitStaticDuo();
	void InitStcTitleDuo();
	void InitStcDataDuo();
	void InitBtnDuo();

	void AtDlgShow();
	void AtDlgHide();

public:
	CDlgMenu02(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu02();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:
	void SetLight(int nVal = -1);
	void SetLight2(int nVal = -1);
	void ResetLight();
	void ResetLight2();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_02 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	CSliderCtrl m_LightSlider, m_LightSlider2;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
