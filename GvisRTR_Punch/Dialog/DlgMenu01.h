#pragma once


// CDlgMenu01 ��ȭ �����Դϴ�.

class CDlgMenu01 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu01)

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
