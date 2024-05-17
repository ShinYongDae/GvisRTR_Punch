#pragma once


// CDlgFrameHigh 대화 상자입니다.

class CDlgFrameHigh : public CDialog
{
	DECLARE_DYNAMIC(CDlgFrameHigh)


public:
	CDlgFrameHigh(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgFrameHigh();
	BOOL Create();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FRAME_HIGH };
#endif

	// 특성입니다.
public:


	// 작업입니다.
public:
	void ChkMenu01();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChkMenu01();
	afx_msg void OnBnClickedChkMenu02();
	afx_msg void OnBnClickedChkMenu03();
	afx_msg void OnBnClickedChkMenu04();
	afx_msg void OnBnClickedChkMenu05();
	afx_msg void OnBnClickedChkMenu06();
};
