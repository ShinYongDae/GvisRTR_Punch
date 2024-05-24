#pragma once

#include "MyBtn.h"
#include "MyStatic.h"

#define MAX_INFO_STC				67
#define MAX_INFO_STC_DATA			21
#define MAX_INFO_BTN				25

// CDlgInfo 대화 상자입니다.

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
	CDlgInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgInfo();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_INFO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
