
// GvisRTR_Punch.h : GvisRTR_Punch 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include "SingleInstance.h"


// CGvisRTR_PunchApp:
// 이 클래스의 구현에 대해서는 GvisRTR_Punch.cpp을 참조하십시오.
//

class CGvisRTR_PunchApp : public CWinApp
{
	CString m_strHostAddress, m_strHostPort;

	void wsaStartup();
	void wsaEndup();

public:
	CGvisRTR_PunchApp();
	~CGvisRTR_PunchApp();

	CSingleInstance m_singleInstance;
	DWORD KillProcess(CString strProcName);

	CString GetHostAddress();
	CString GetHostPort();
	void SetHostAddress(CString sAddr);
	void SetHostPort(CString sPort);


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGvisRTR_PunchApp theApp;
