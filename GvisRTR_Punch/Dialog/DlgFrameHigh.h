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

	CMyPic myPic;
	CMyLabel myLabel[MAX_FRMHIGH_LABEL];
	CMyBtn myChkMenu01, myChkMenu02, myChkMenu03, myChkMenu04, myChkMenu05, myChkMenu06;
	CMyStatic myStc[MAX_FRMHIGH_STC];


	void LoadImg();
	void DelImg();
	BOOL Create();
	void InitGui();

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
};
