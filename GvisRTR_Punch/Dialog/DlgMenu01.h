#pragma once

#include "MyStatic.h"
#include "MyBtn.h"
#include "MyGL.h"

#define MAX_MENU01_STC_DEFINFO		12
#define MAX_MENU01_STC_TITLE		75
#define MAX_MENU01_STC_DATA			91
#define MAX_MENU01_BTN				23

// CDlgMenu01 ��ȭ �����Դϴ�.

class CDlgMenu01 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu01)

	CMyStatic myStcReelmap;
	CMyStatic myStcDefInfo[MAX_MENU01_STC_DEFINFO];
	CMyStatic myStcTitle[MAX_MENU01_STC_TITLE];
	CMyStatic myStcData[MAX_MENU01_STC_DATA];
	CMyBtn myBtn[MAX_MENU01_BTN];

public:
	CDlgMenu01(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu01();
	BOOL Create();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_01 };
#endif

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:
	void ChkUserInfo(BOOL bOn);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
