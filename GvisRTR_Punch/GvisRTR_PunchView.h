
// GvisRTR_PunchView.h : CGvisRTR_PunchView Ŭ������ �������̽�
//

#pragma once


class CGvisRTR_PunchView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CGvisRTR_PunchView();
	DECLARE_DYNCREATE(CGvisRTR_PunchView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_GVISRTR_PUNCH_FORM };
#endif

// Ư���Դϴ�.
public:
	CGvisRTR_PunchDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CGvisRTR_PunchView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GvisRTR_PunchView.cpp�� ����� ����
inline CGvisRTR_PunchDoc* CGvisRTR_PunchView::GetDocument() const
   { return reinterpret_cast<CGvisRTR_PunchDoc*>(m_pDocument); }
#endif

