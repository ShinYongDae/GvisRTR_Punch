#pragma once


// CDlgMenu02 ��ȭ �����Դϴ�.

class CDlgMenu02 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu02)

public:
	CDlgMenu02(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu02();
	BOOL Create();

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:
	void SetLight(int nVal = -1);
	void SetLight2(int nVal = -1);
	void ResetLight();
	void ResetLight2();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_02 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
