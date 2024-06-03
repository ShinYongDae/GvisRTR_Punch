
// GvisRTR_PunchDoc.h : CGvisRTR_PunchDoc Ŭ������ �������̽�
//


#pragma once

#include "Global/GlobalDefine.h"
#include "Process/MyFile.h"


class CGvisRTR_PunchDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CGvisRTR_PunchDoc();
	DECLARE_DYNCREATE(CGvisRTR_PunchDoc)

// Ư���Դϴ�.
public:
	stWorkingInfo WorkingInfo;
	stMkIo MkIo;
	CMyFile *m_pFile;


// �۾��Դϴ�.
public:
	void DestroyDoc();

	int GetTestMode();
	void SetTestMode(int nMode);

	// File communication for Engrave
	void SetMkInfo(CString sMenu, CString sItem, BOOL bOn);
	void SetMkInfo(CString sMenu, CString sItem, CString sData);
	void SetMonDispMain(CString sDisp);
	void SetEngItsCode(CString sItsCode);

	void LogAuto(CString strMsg, int nType = 0);
	void LogPLC(CString strMsg, int nType = 0);

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CGvisRTR_PunchDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
