
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

class CMainFrame : public CFrameWnd
{
	CString m_sDispStatusBar[10];

protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void DispStatusBar(CString strMsg, int nStatusBarID);

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL DestroyWindow();
	afx_msg void OnAppMinimize();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};


