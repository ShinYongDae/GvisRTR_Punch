#pragma once


// CDlgMenu05 대화 상자입니다.

class CDlgMenu05 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu05)

public:
	CDlgMenu05(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMenu05();
	BOOL Create();

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
};
