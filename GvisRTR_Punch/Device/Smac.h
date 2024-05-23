#if !defined(AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_)
#define AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Smac.h : header file
//
#include "Rs232.h"

#define TIMER_CHECK_END_CMD		110
#define TIMER_CHECK_ERROR_CODE	120

/////////////////////////////////////////////////////////////////////////////
// CSmac window

class CSmac : public CWnd
{
// Construction
	CWnd* m_pParent;
	CCriticalSection m_cs;

	CRs232 m_Rs232;
	BOOL m_bRs232Run;
	int m_nCh;

	CString m_strReceiveVoiceCoil;
	CString m_strSmacEndCmd;
	BOOL m_bTimerStop, m_bFeedingRollerUp;
	CString m_sRcvRs232;
	BOOL m_bReturnCmdEnd, m_bRunTimerCheckEnd;

	BOOL Send(CString str);
	char* StringToChar(CString str); // char* returned must be deleted... 

public:
	CSmac(CWnd* pParent =NULL);

// Attributes
public:

// Operations
public:
	afx_msg LRESULT OnReceiveRs232(WPARAM wP, LPARAM lP);

	void Init(int nCh);
	void Close();

	double GetSmacMeasureOfSurface();
	void SendStringToVoiceCoil(CString strSend);
	//BOOL SetWaitSmacCmdEnd(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	BOOL WaitSmacCmdEnd(CString strEndCmd); // Return Value is TRUE (OK), FALSE (NG)
	double GetSmacPosition();
	void SearchHomeSmac();
	int CheckSmacErrorCode();
	CString DisplaySmacErrorList(int nErrCode);
	void Wait(int imSecond);
	void CalcAveSmacHeight();
	void ResetSmac();
	CString GetSmacStatus();
	void MoveSmacFinalPos();
	void MoveSmacShiftPos();

	void SetMarkShiftData();
	void SetMarkFinalData();
	void SetMark();
	BOOL IsDoneMark();

	void ClearReceive();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSmac)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSmac();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSmac)
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SMAC_H__75FE4BE9_1514_4433_B92C_218B1E6CF6D8__INCLUDED_)
