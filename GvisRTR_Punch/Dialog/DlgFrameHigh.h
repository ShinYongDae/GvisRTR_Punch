#pragma once


// CDlgFrameHigh ��ȭ �����Դϴ�.

class CDlgFrameHigh : public CDialog
{
	DECLARE_DYNAMIC(CDlgFrameHigh)


public:
	CDlgFrameHigh(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgFrameHigh();
	BOOL Create();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FRAME_HIGH };
#endif

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:
	void ChkMenu01();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChkMenu01();
	afx_msg void OnBnClickedChkMenu02();
	afx_msg void OnBnClickedChkMenu03();
	afx_msg void OnBnClickedChkMenu04();
	afx_msg void OnBnClickedChkMenu05();
	afx_msg void OnBnClickedChkMenu06();
};
