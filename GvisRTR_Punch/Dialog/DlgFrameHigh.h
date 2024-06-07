#pragma once

#include "MyBtn.h"
#include "MyPic.h"
#include "MyLabel.h"
#include "MyStatic.h"

#define MAX_FRMHIGH_LABEL	5
#define MAX_FRMHIGH_STC		11

// CDlgFrameHigh ��ȭ �����Դϴ�.

class CDlgFrameHigh : public CDialog
{
	DECLARE_DYNAMIC(CDlgFrameHigh)

	CRect* m_pRect;

	CMyPic myPic;
	CMyLabel myLabel[MAX_FRMHIGH_LABEL];
	CMyBtn myChkMenu01, myChkMenu02, myChkMenu03, myChkMenu04, myChkMenu05, myChkMenu06;
	CMyStatic myStc[MAX_FRMHIGH_STC];

	int m_nMkLastShot, m_nAoiLastShot[2], m_nEngraveLastShot; // [Up/Dn]


	void LoadImg();
	void DelImg();
	BOOL Create();
	void InitGui();
	void InitStatic();
	void InitPic();
	void InitLabel();
	void InitBtn();

	void AtDlgShow();
	void AtDlgHide();

	void SetChk(int nID);

public:
	CDlgFrameHigh(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgFrameHigh();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FRAME_HIGH };
#endif

	// Ư���Դϴ�.
public:


	// �۾��Դϴ�.
public:
	void ChkMenu01();
	void ChkMenu02();
	void ChkMenu03();
	void ChkMenu04();
	void ChkMenu05();
	void ChkMenu06();

	int GetLastShotMk();
	void SetMkLastShot(int nSerial);

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
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
