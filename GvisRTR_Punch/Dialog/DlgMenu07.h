#pragma once


// CDlgMenu07 ��ȭ �����Դϴ�.

class CDlgMenu07 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu07)

public:
	CDlgMenu07(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu07();
	BOOL Create();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_07 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
