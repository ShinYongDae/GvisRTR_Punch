#pragma once

#include "MyStatic.h"
#include "MyBtn.h"
#include "MyGL.h"

#define MAX_MENU07_STC_DEFINFO		12
#define MAX_MENU07_STC_TITLE		67
#define MAX_MENU07_STC_DATA			84
#define MAX_MENU07_BTN				23

// CDlgMenu07 ��ȭ �����Դϴ�.

class CDlgMenu07 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu07)

	CMyStatic myStcReelmap;
	CMyStatic myStcDefInfo[MAX_MENU07_STC_DEFINFO];
	CMyStatic myStcTitle[MAX_MENU07_STC_TITLE];
	CMyStatic myStcData[MAX_MENU07_STC_DATA];
	CMyBtn myBtn[MAX_MENU07_BTN];

	CRect m_RectTitle[2];
	CRect m_RectCAD[12];
	CRect m_RectDef[12];
	CRect m_RectInfo[12];


	BOOL Create();
	void LoadImg();
	void DelImg();
	void InitGui();

public:
	CDlgMenu07(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu07();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_07 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
