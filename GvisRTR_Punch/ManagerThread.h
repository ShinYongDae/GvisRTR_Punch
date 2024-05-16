#pragma once
#include "afxwin.h"
class CManagerThread : public CWnd
{
	CWnd* m_pParent;

public:
	CManagerThread(CWnd* pParent = NULL);
	~CManagerThread();
};

