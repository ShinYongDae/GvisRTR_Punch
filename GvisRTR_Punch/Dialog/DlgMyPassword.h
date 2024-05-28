#pragma once

#include "MyPic.h"
#include "MyBtn.h"
#include "MyGroup.h"
#include "MyLabel.h"
#include "MyEdit.h"

#define TIM_KILL_FOCUS				221
#define TIM_WATCHDOG_MYPASSWORD		222

#ifndef FONT_SIZE_PW
#define FONT_SIZE_PW	62
#endif
#ifndef LINE_SPACE_PW
#define LINE_SPACE_PW	20
#endif

// CDlgMyPassword 대화 상자입니다.

class CDlgMyPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgMyPassword)

	BOOL m_bFont;
	CFont m_Font;
	BOOL m_bLoadImg;
	CRect* m_pRect;
	CMyPic myPic;
	int m_nSel;
	CMyLabel myLabel;
	CMyEdit myEdit[2];
	CMyBtn myBtn;
	CMyGroup myGroup;
	CString m_strMsg;
	int m_nCtrlId;
	BOOL m_bTIM_DLG_MY_PASSWORD, m_bTIM_DLG_MY_PASSWORD_Lock;

	void AtDlgShow();
	void AtDlgHide();
	void InitPic();
	void LoadImg();
	void DelImg();
	void InitLabel();
	void InitBtn();
	void InitGroup();
	void InitEdit();
	void KillFocus(int nID);
	void SetFont(CString srtFntName, int nSize = 10, BOOL bBold = FALSE);


public:
	CDlgMyPassword(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgMyPassword();

	CWnd* m_pParent;
	int m_nRtnVal;

	void SetMsg(CString strMsg, int nCtrlId = 0);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MY_PASSWORD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonNum0();
	afx_msg void OnBnClickedButtonNum1();
	afx_msg void OnBnClickedButtonNum2();
	afx_msg void OnBnClickedButtonNum3();
	afx_msg void OnBnClickedButtonNum4();
	afx_msg void OnBnClickedButtonNum5();
	afx_msg void OnBnClickedButtonNum6();
	afx_msg void OnBnClickedButtonNum7();
	afx_msg void OnBnClickedButtonNum8();
	afx_msg void OnBnClickedButtonNum9();
	afx_msg void OnBnClickedButtonSym00();
	afx_msg void OnBnClickedButtonSym01();
	afx_msg void OnBnClickedButtonSym02();
	afx_msg void OnBnClickedButtonSym03();
	afx_msg void OnBnClickedButtonSym04();
	afx_msg void OnBnClickedButtonSym05();
	afx_msg void OnBnClickedButtonSym06();
	afx_msg void OnBnClickedButtonSym07();
	afx_msg void OnBnClickedButtonSym08();
	afx_msg void OnBnClickedButtonSym09();
	afx_msg void OnBnClickedButtonSym10();
	afx_msg void OnBnClickedButtonSym11();
	afx_msg void OnBnClickedButtonMinus();
	afx_msg void OnBnClickedButtonEqual();
	afx_msg void OnBnClickedButtonCharQ();
	afx_msg void OnBnClickedButtonCharW();
	afx_msg void OnBnClickedButtonCharE();
	afx_msg void OnBnClickedButtonCharR();
	afx_msg void OnBnClickedButtonCharT();
	afx_msg void OnBnClickedButtonCharY();
	afx_msg void OnBnClickedButtonCharU();
	afx_msg void OnBnClickedButtonCharI();
	afx_msg void OnBnClickedButtonCharO();
	afx_msg void OnBnClickedButtonCharP();
	afx_msg void OnBnClickedButtonEsc();
	afx_msg void OnBnClickedButtonCharA();
	afx_msg void OnBnClickedButtonCharS();
	afx_msg void OnBnClickedButtonCharD();
	afx_msg void OnBnClickedButtonCharF();
	afx_msg void OnBnClickedButtonCharG();
	afx_msg void OnBnClickedButtonCharH();
	afx_msg void OnBnClickedButtonCharJ();
	afx_msg void OnBnClickedButtonCharK();
	afx_msg void OnBnClickedButtonCharL();
	afx_msg void OnBnClickedButtonCharZ();
	afx_msg void OnBnClickedButtonCharX();
	afx_msg void OnBnClickedButtonCharC();
	afx_msg void OnBnClickedButtonCharV();
	afx_msg void OnBnClickedButtonCharB();
	afx_msg void OnBnClickedButtonCharN();
	afx_msg void OnBnClickedButtonCharM();
	afx_msg void OnBnClickedButtonComma();
	afx_msg void OnBnClickedButtonDot();
	afx_msg void OnBnClickedButtonEnter();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSpace();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonBs();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedCheckShift();
	afx_msg void OnSetfocusEdit11();
	afx_msg void OnSetfocusEdit21();
	afx_msg void OnChangeEdit11();
	afx_msg void OnChangeEdit21();
	afx_msg void OnBnClickedBtn00();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
