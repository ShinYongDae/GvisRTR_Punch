#pragma once


// CDlgOption01 ��ȭ �����Դϴ�.

class CDlgOption01 : public CDialog
{
	DECLARE_DYNAMIC(CDlgOption01)

public:
	CDlgOption01(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgOption01();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OPTION_01 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
