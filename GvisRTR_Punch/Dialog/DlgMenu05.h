#pragma once


// CDlgMenu05 ��ȭ �����Դϴ�.

class CDlgMenu05 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu05)

public:
	CDlgMenu05(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu05();
	BOOL Create();

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
};
