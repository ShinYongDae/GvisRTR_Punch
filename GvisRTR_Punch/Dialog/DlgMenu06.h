#pragma once

#include "MyStatic.h"
#include "MyBtn.h"
#include "MyGL.h"

#define MAX_MENU06_STC_DEFINFO		12
#define MAX_MENU06_STC_TITLE		67
#define MAX_MENU06_STC_DATA			84
#define MAX_MENU06_BTN				23

// CDlgMenu06 대화 상자입니다.

class CDlgMenu06 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu06)

	CRect* m_pRect;

	CMyStatic myStcReelmap;
	CMyStatic myStcDefInfo[MAX_MENU06_STC_DEFINFO];
	CMyStatic myStcTitle[MAX_MENU06_STC_TITLE];
	CMyStatic myStcData[MAX_MENU06_STC_DATA];
	CMyBtn myBtn[MAX_MENU06_BTN];

	CRect m_RectTitle[2];
	CRect m_RectCAD[12];
	CRect m_RectDef[12];
	CRect m_RectInfo[12];

	BOOL Create();
	void LoadImg();
	void DelImg();
	void InitGui();
	void InitStatic();
	void InitStcDefInf();
	void InitStcTitle();
	void InitStcData();
	void InitBtn();

	void AtDlgShow();
	void AtDlgHide();

public:
	CDlgMenu06(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMenu06();

	// 특성입니다.
public:


	// 작업입니다.
public:

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_06 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
