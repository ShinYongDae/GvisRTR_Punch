#pragma once


// CDlgMenu01 대화 상자입니다.

class CDlgMenu01 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu01)

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
