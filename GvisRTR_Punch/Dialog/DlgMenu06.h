#pragma once


// CDlgMenu06 ��ȭ �����Դϴ�.

class CDlgMenu06 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu06)

public:
	CDlgMenu06(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu06();
	BOOL Create();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_06 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
