
// GvisRTR_PunchDoc.h : CGvisRTR_PunchDoc Ŭ������ �������̽�
//


#pragma once

#include "Global/GlobalDefine.h"
#include "Global/MyData.h"
#include "Process/MyFile.h"


class CGvisRTR_PunchDoc : public CDocument
{
	BOOL m_bOffLogAuto, m_bOffLogPLC;
	BOOL m_bUseDts, m_bUseIts;

	BOOL LoadWorkingInfo();

protected: // serialization������ ��������ϴ�.
	CGvisRTR_PunchDoc();
	DECLARE_DYNCREATE(CGvisRTR_PunchDoc)

// Ư���Դϴ�.
public:
	stWorkingInfo WorkingInfo;
	stMkIo MkIo;
	CMyFile *m_pFile;

	int m_nAoiCamInfoStrPcs[2]; // [0] : Up, [1] : Dn
	
	// For ITS
	BOOL m_bEngDualTest;
	CString m_sItsCode;
	CString m_sEngLotNum, m_sEngProcessNum;
	CString m_sEngModel, m_sEngLayerUp;
	CString m_sEngLayerDn;
	int m_nWritedItsSerial;

// �۾��Դϴ�.
public:
	void DestroyDoc();

	int GetTestMode();
	void SetTestMode(int nMode);

	double GetOffsetInitPos();

	// File communication for Engrave
	void GetMkInfo(); // About Engrave Signal
	void SetMkInfo(CString sMenu, CString sItem, BOOL bOn);
	void SetMkInfo(CString sMenu, CString sItem, CString sData);
	void SetMonDispMain(CString sDisp);
	void SetEngItsCode(CString sItsCode);

	void LogAuto(CString strMsg, int nType = 0);
	void LogPLC(CString strMsg, int nType = 0);

	void SaveLotTime(DWORD dwStTick);
	void SetEngraveFdPitch(double dPitch);
	void SetEngraveAoiDist(double dLen);
	void SetEngBufInitPos(double dPos);
	BOOL GetCurrentInfoSignal(int nIdxSig);
	void SetCurrentInfoSignal(int nIdxSig, BOOL bOn);
	int GetAoiUpAutoSerial();
	void SetAoiUpAutoSerial(int nSerial);
	int GetAoiUpAutoStep();
	void SetAoiUpAutoStep(int nStep);
	int GetAoiDnAutoSerial();
	void SetAoiDnAutoSerial(int nSerial);
	int GetAoiDnAutoStep();
	void SetAoiDnAutoStep(int nStep);
	CString GetAoiUpAlarmRestartMsg();
	CString GetAoiUpAlarmReTestMsg();
	CString GetAoiDnAlarmRestartMsg();
	CString GetAoiDnAlarmReTestMsg();
	BOOL GetAoiUpVsStatus();
	int GetLastShotMk();
	BOOL GetAoiUpOffset(CfPoint &OfSt);
	BOOL GetAoiDnOffset(CfPoint &OfSt);
	BOOL GetEngOffset(CfPoint &OfSt);
	void SetEngraveReaderDist(double dLen);
	void SetOnePnlLen(double dLen);
	double GetOnePnlLen();
	void SetOnePnlVel(double dVel);
	void SetFdJogVel(double dVel);
	void SetFdJogAcc(double dVel);
	double GetOnePnlVel();
	double GetFdJogVel();
	double GetEngraveToq();
	BOOL GetCurrentInfoEng();
	int GetCurrentInfoEngShotNum();
	CString GetCamPxlRes();
	CString GetProcessNum();

	int CopyPcrAll();
	int CopyPcrUp();
	int CopyPcrDn();
	void DelPcrAll();
	void DelPcrUp();
	void DelPcrDn();
	void DelSharePcr();
	void DelSharePcrUp();
	void DelSharePcrDn();
	void DelShareVsPcrUp();
	void DelShareVsPcrDn();

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
