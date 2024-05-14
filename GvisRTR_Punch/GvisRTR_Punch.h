
// GvisRTR_Punch.h : GvisRTR_Punch ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include "SingleInstance.h"


// CGvisRTR_PunchApp:
// �� Ŭ������ ������ ���ؼ��� GvisRTR_Punch.cpp�� �����Ͻʽÿ�.
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


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGvisRTR_PunchApp theApp;
