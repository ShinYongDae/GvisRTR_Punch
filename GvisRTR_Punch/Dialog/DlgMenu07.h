#pragma once


// CDlgMenu07 대화 상자입니다.

class CDlgMenu07 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu07)

public:
	CDlgMenu07(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMenu07();
	BOOL Create();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_07 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
