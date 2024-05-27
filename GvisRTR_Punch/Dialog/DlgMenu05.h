#pragma once

#include "MyStatic.h"
#include "MyLabel.h"
//#include "DlgUtil01.h"

#define MAX_MENU05_LABEL			2

// CDlgMenu05 ��ȭ �����Դϴ�.

class CDlgMenu05 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu05)

	CRect* m_pRect;

	CMyLabel myLabel[MAX_MENU05_LABEL];
	CMyStatic myStcLot, myStcSerialSt, myStcSerialEd, myStcFromTo, myStcProc, myStcIts;

	BOOL Create();
	void LoadImg();
	void DelImg();
	void InitGui();
	void InitStc();
	void InitLabel();

	void AtDlgShow();
	void AtDlgHide();

public:
	CDlgMenu05(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu05();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_05 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
