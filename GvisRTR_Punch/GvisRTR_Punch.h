
// GvisRTR_Punch.h : GvisRTR_Punch ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CGvisRTR_PunchApp:
// �� Ŭ������ ������ ���ؼ��� GvisRTR_Punch.cpp�� �����Ͻʽÿ�.
//

class CGvisRTR_PunchApp : public CWinApp
{
public:
	CGvisRTR_PunchApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGvisRTR_PunchApp theApp;
