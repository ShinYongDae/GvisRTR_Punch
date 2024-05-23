#pragma once

#include "MyStatic.h"
#include "MyBtn.h"
#include "MyGL.h"

#define MAX_MENU01_STC_DEFINFO		12
#define MAX_MENU01_STC_TITLE		75
#define MAX_MENU01_STC_DATA			91
#define MAX_MENU01_BTN				23

// CDlgMenu01 대화 상자입니다.

class CDlgMenu01 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu01)

	CMyStatic myStcReelmap;
	CMyStatic myStcDefInfo[MAX_MENU01_STC_DEFINFO];
	CMyStatic myStcTitle[MAX_MENU01_STC_TITLE];
	CMyStatic myStcData[MAX_MENU01_STC_DATA];
	CMyBtn myBtn[MAX_MENU01_BTN];

public:
	CDlgMenu01(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMenu01();
	BOOL Create();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_01 };
#endif

	// 특성입니다.
public:


	// 작업입니다.
public:
	void ChkUserInfo(BOOL bOn);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
