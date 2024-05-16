#pragma once
#include "afxwin.h"
class CManagerProcedure : public CWnd
{
	CWnd* m_pParent;

public:
	CManagerProcedure(CWnd* pParent = NULL);
	~CManagerProcedure();
};

