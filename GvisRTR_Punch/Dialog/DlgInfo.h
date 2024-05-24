#pragma once

#include "MyBtn.h"
#include "MyStatic.h"

#define MAX_INFO_STC				67
#define MAX_INFO_STC_DATA			21
#define MAX_INFO_BTN				25

// CDlgInfo ��ȭ �����Դϴ�.

class CDlgInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgInfo)

	CMyBtn myBtn[MAX_INFO_BTN], myBtnExit;
	CMyStatic myStcTitle[MAX_INFO_STC];
	CMyStatic myStcData[MAX_INFO_STC_DATA];

	void LoadImg();
	void DelImg();
	BOOL Create();
	void InitGui();

public:
	CDlgInfo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgInfo();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
