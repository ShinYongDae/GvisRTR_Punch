
// GvisRTR_PunchDoc.h : CGvisRTR_PunchDoc 클래스의 인터페이스
//


#pragma once

#include "Global/GlobalDefine.h"
#include "Process/MyFile.h"


class CGvisRTR_PunchDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CGvisRTR_PunchDoc();
	DECLARE_DYNCREATE(CGvisRTR_PunchDoc)

// 특성입니다.
public:
	stWorkingInfo WorkingInfo;
	stMkIo MkIo;
	CMyFile *m_pFile;


// 작업입니다.
public:
	void DestroyDoc();

	int GetTestMode();
	void SetTestMode(int nMode);

	// File communication for Engrave
	void SetMkInfo(CString sMenu, CString sItem, BOOL bOn);
	void SetMkInfo(CString sMenu, CString sItem, CString sData);
	void SetMonDispMain(CString sDisp);

	void LogAuto(CString strMsg, int nType = 0);
	void LogPLC(CString strMsg, int nType = 0);

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CGvisRTR_PunchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
