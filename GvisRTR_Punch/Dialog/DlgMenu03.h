#pragma once


// CDlgMenu03 ��ȭ �����Դϴ�.

class CDlgMenu03 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu03)

public:
	CDlgMenu03(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu03();
	BOOL Create();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_03 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
