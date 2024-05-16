
// GvisRTR_PunchView.cpp : CGvisRTR_PunchView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "GvisRTR_Punch.h"
#endif

#include "MainFrm.h"
#include "GvisRTR_PunchDoc.h"
#include "GvisRTR_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisRTR_PunchDoc* pDoc;
CGvisRTR_PunchView* pView;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGvisRTR_PunchView

IMPLEMENT_DYNCREATE(CGvisRTR_PunchView, CFormView)

BEGIN_MESSAGE_MAP(CGvisRTR_PunchView, CFormView)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CGvisRTR_PunchView ����/�Ҹ�

CGvisRTR_PunchView::CGvisRTR_PunchView()
	: CFormView(IDD_GVISRTR_PUNCH_FORM)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	pView = this;
	m_bTIM_INIT_VIEW = FALSE;

}

CGvisRTR_PunchView::~CGvisRTR_PunchView()
{
	m_bTIM_INIT_VIEW = FALSE;

	InitDispMsg();
}

void CGvisRTR_PunchView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CGvisRTR_PunchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CGvisRTR_PunchView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	if (!m_bTIM_INIT_VIEW)
	{
		m_nStepInitView = 0;
		m_bTIM_INIT_VIEW = TRUE;
		SetTimer(TIM_INIT_VIEW, 300, NULL);
	}
}


// CGvisRTR_PunchView ����

#ifdef _DEBUG
void CGvisRTR_PunchView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGvisRTR_PunchView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGvisRTR_PunchDoc* CGvisRTR_PunchView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGvisRTR_PunchDoc)));
	return (CGvisRTR_PunchDoc*)m_pDocument;
}
#endif //_DEBUG


// CGvisRTR_PunchView �޽��� ó����
void CGvisRTR_PunchView::InitMgr()
{
	InitMgrProcedure();
	InitMgrPunch();			//HwInit();	
	InitMgrReelmap();
	InitMgrThread();
}

void CGvisRTR_PunchView::InitMgrPunch()
{
	if (m_mgrPunch)
	{
		delete m_mgrPunch;
		m_mgrPunch = NULL;
	}
	m_mgrPunch = new CManagerPunch(this);
	//m_mgrPunch->Init();
}

void CGvisRTR_PunchView::InitMgrProcedure()
{
	if (m_mgrProcedure)
	{
		delete m_mgrProcedure;
		m_mgrProcedure = NULL;
	}
	m_mgrProcedure = new CManagerProcedure(this);
	//m_mgrProcedure->Init();
}

void CGvisRTR_PunchView::InitMgrReelmap()
{
	if (m_mgrReelmap)
	{
		delete m_mgrReelmap;
		m_mgrReelmap = NULL;
	}
	m_mgrReelmap = new CManagerReelmap(this);
	//m_mgrReelmap->Init();
}

void CGvisRTR_PunchView::InitMgrThread()
{
	if (m_mgrThread)
	{
		delete m_mgrThread;
		m_mgrThread = NULL;
	}
	m_mgrThread = new CManagerThread(this);
	//m_mgrThread->Init();
}

void CGvisRTR_PunchView::InitDispMsg()
{
	m_pDlgMyMsg = NULL;
	m_pDlgMsgBox = NULL;
}

LONG CGvisRTR_PunchView::OnQuitDispMsg(UINT wParam, LONG lParam)
{
	if (m_pDlgMsgBox)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}

	return 0L;
}

void CGvisRTR_PunchView::ClrDispMsg()
{
	OnQuitDispMsg(NULL, NULL);
}

void CGvisRTR_PunchView::DoDispMsg(CString strMsg, CString strTitle, COLORREF color, DWORD dwDispTime, BOOL bOverWrite)
{
	if (dwDispTime == 0)
	{
		dwDispTime = 24 * 3600 * 1000;
	}

	if (m_pDlgMsgBox != NULL)
	{
		if (bOverWrite)
		{
			if (m_pDlgMsgBox)
				m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
		}
		if (m_pDlgMsgBox)
			m_pDlgMsgBox->ShowWindow(SW_SHOW);
		if (m_pDlgMsgBox)
			m_pDlgMsgBox->SetFocus();
		if (m_pDlgMsgBox)
			((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
	}
	else
	{
		m_pDlgMsgBox = new CDlgMsgBox(this, strTitle, strMsg, dwDispTime, color);
		if (m_pDlgMsgBox->GetSafeHwnd() == 0)
		{
			m_pDlgMsgBox->Create();
			m_pDlgMsgBox->ShowWindow(SW_SHOW);
			m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
			m_pDlgMsgBox->SetFocus();
			((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
		}
	}
}

void CGvisRTR_PunchView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nIDEvent == TIM_INIT_VIEW)
	{
		KillTimer(TIM_INIT_VIEW);

		switch (m_nStepInitView)
		{
		case 0:
			m_nStepInitView++;
			break;
		case 1:
			m_nStepInitView++;
			DoDispMsg(_T("���α׷��� �ʱ�ȭ�մϴ�."), _T("�˸�"), RGB_GREEN, DELAY_TIME_MSG);
			InitMgr();
			break;
		}
	}

	CFormView::OnTimer(nIDEvent);
}
