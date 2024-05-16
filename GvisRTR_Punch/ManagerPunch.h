#pragma once
#include "afxwin.h"
class CManagerPunch : public CWnd
{
	CWnd* m_pParent;

public:
	CManagerPunch(CWnd* pParent = NULL);
	~CManagerPunch();
};

