#pragma once


// CDlgMenu04 ��ȭ �����Դϴ�.

class CDlgMenu04 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu04)

public:
	CDlgMenu04(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu04();
	BOOL Create();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_04 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
