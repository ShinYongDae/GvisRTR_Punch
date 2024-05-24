#pragma once

#include "MyStatic.h"
//#include "DlgUtil01.h"
#include "MyLabel.h"

#define MAX_MENU05_LABEL			2

// CDlgMenu05 대화 상자입니다.

class CDlgMenu05 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu05)

	CMyLabel myLabel[MAX_MENU05_LABEL];

	BOOL Create();
	void LoadImg();
	void DelImg();
	void InitGui();

public:
	CDlgMenu05(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMenu05();

	// 특성입니다.
public:


	// 작업입니다.
public:

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_05 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
