
// GvisRTR_PunchDoc.h : CGvisRTR_PunchDoc 클래스의 인터페이스
//


#pragma once

#include "Global/GlobalDefine.h"
#include "Global/MyData.h"
#include "Process/MyFile.h"

#define PATH_LOG		_T("C:\Debuging\Log")
#define PATH_LOG_AUTO	_T("C:\Debuging\Log\Auto")
#define PATH_LOG_PLC	_T("C:\Debuging\Log\PLC")


class CGvisRTR_PunchDoc : public CDocument
{
	BOOL m_bOffLogAuto, m_bOffLogPLC;
	BOOL m_bUseDts, m_bUseIts;

	BOOL LoadWorkingInfo();
	void StringToChar(CString str, char* pCh); // char* returned must be deleted... 

protected: // serialization에서만 만들어집니다.
	CGvisRTR_PunchDoc();
	DECLARE_DYNCREATE(CGvisRTR_PunchDoc)

// 특성입니다.
public:
	stWorkingInfo WorkingInfo;
	stMkIo MkIo;
	CMyFile *m_pFile;

	int m_nAoiCamInfoStrPcs[2]; // [0] : Up, [1] : Dn
	BOOL m_bDebugGrabAlign;
	BOOL m_bUseRTRYShiftAdjust;
	double m_dShiftAdjustRatio;
	CString m_sPassword;
	int m_nDelayShow;
	BOOL m_bDoneChgLot;
	
	// For ITS
	BOOL m_bEngDualTest;
	CString m_sItsCode;
	CString m_sEngLotNum, m_sEngProcessNum;
	CString m_sEngModel, m_sEngLayerUp;
	CString m_sEngLayerDn;
	int m_nWritedItsSerial;

// 작업입니다.
public:
	void DestroyDoc();
	int GetTestMode();
	void SetTestMode(int nMode);
	double GetOffsetInitPos();

	// File communication for Engrave
	void SetMonDispMain(CString sDisp);

	void LogAuto(CString strMsg, int nType = 0);
	void LogPLC(CString strMsg, int nType = 0);

	void SetTotalReelDist(double dDist);
	double GetTotalReelDist();
	void SetSeparateDist(double dDist);
	double GetSeparateDist();
	void SetCuttingDist(double dDist);
	double GetCuttingDist();
	void SetStopDist(double dDist);
	double GetStopDist();
	void SaveLotTime(DWORD dwStTick);
	BOOL GetCurrentInfoSignal(int nIdxSig);
	void SetCurrentInfoSignal(int nIdxSig, BOOL bOn);
	int GetLastShotMk();
	void SetOnePnlLen(double dLen);
	double GetOnePnlLen();
	void SetOnePnlVel(double dVel);
	void SetFdJogVel(double dVel);
	void SetFdJogAcc(double dVel);
	double GetOnePnlVel();
	double GetFdJogVel();
	double GetFdJogAcc();
	BOOL GetCurrentInfoEng();
	int GetCurrentInfoEngShotNum();
	CString GetCamPxlRes();
	CString GetProcessNum();
	double GetOnePnlAcc();
	void SetOnePnlAcc(double dAcc);
	void SetFdErrLmt(double dLmt);
	void SetFdErrRng(double dRng);
	double GetFdErrRng();
	void SetFdErrNum(int nNum);
	int GetFdErrNum();
	void SetBufInitPos(double dPos);
	void SetBufInitPos(double dVel, double dAcc);
	double GetBuffInitPos();
	void MakeImageDirUp(int nSerial);
	void MakeImageDirDn(int nSerial);

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

	void DelItsAll(CString strPath);
	int GetItsDefCode(int nDefCode);
	BOOL Shift2Mk(int nSerial);

	void SetEngraveFdPitch(double dPitch);
	void SetEngraveAoiDist(double dLen);
	void SetEngBufInitPos(double dPos);
	void SetEngItsCode(CString sItsCode);
	BOOL GetEngOffset(CfPoint &OfSt);
	void SetEngraveReaderDist(double dLen);
	void SetEngraveToq(double dToq);
	double GetEngraveToq();

	BOOL GetPcrInfo(CString sPath, stModelInfo &stInfo);
	BOOL MakeMkDir();
	void GetMkInfo(); // About Engrave Signal
	void SetMkInfo(CString sMenu, CString sItem, BOOL bOn);
	void SetMkInfo(CString sMenu, CString sItem, CString sData);
	double GetMkFdPitch();
	double GetFdErrLmt();
	double GetMarkingToq();
	void SetMarkingToq(double dToq);

	void SetAOIToq(double dToq);
	double GetAOIToq();
	BOOL GetAoiUpOffset(CfPoint &OfSt);
	BOOL GetAoiDnOffset(CfPoint &OfSt);
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
	void SetAoiFdPitch(double dPitch);
	double GetAoiFdPitch();
	void SetAoiMkDist(double dPos);
	double GetAoiMkDist();
	void SetAoiAoiDist(int nShot);
	int GetAoiAoiDist();


	void SetWaitPos1(double dPos);
	double GetWaitPos1();
	void SetWaitVel1(double dVel);
	double GetWaitVel1();
	void SetWaitAcc1(double dAcc);
	double GetWaitAcc1();

	void SetMarkingPos1(double dPos);
	double GetMarkingPos1();
	void SetMarkingVel1(double dVel);
	double GetMarkingVel1();
	void SetMarkingAcc1(double dAcc);
	double GetMarkingAcc1();
	void SetMarkingToq1(double dToq);
	void SetMarkingToq1Offset(double dToqOffset);
	double GetMarkingToq1();

	void SetPosX1_1(double dPosX);
	double GetPosX1_1();
	void SetPosY1_1(double dPosY);
	double GetPosY1_1();

	void SetPosX1_2(double dPosX);
	double GetPosX1_2();
	void SetPosY1_2(double dPosY);
	double GetPosY1_2();

	void SetPosX1_3(double dPosX);
	double GetPosY1_3();
	void SetPosY1_3(double dPosY);
	double GetPosX1_3();

	void SetPosX1_4(double dPosX);
	double GetPosX1_4();
	void SetPosY1_4(double dPosY);
	double GetPosY1_4();

	void SetAverDist1(double dDist);
	double GetAverDist1();

	void SetWaitPos2(double dPos);
	double GetWaitPos2();
	void SetWaitVel2(double dVel);
	double GetWaitVel2();
	void SetWaitAcc2(double dAcc);
	double GetWaitAcc2();

	void SetMarkingPos2(double dPos);
	double GetMarkingPos2();
	void SetMarkingVel2(double dVel);
	double GetMarkingVel2();
	void SetMarkingAcc2(double dAcc);
	double GetMarkingAcc2();
	void SetMarkingToq2(double dToq);
	void SetMarkingToq2Offset(double dToqOffset);
	double GetMarkingToq2();

	void SetPosX2_1(double dPosX);
	double GetPosX2_1();
	void SetPosY2_1(double dPosY);
	double GetPosY2_1();

	void SetPosX2_2(double dPosX);
	double GetPosX2_2();
	void SetPosY2_2(double dPosY);
	double GetPosY2_2();

	void SetPosX2_3(double dPosX);
	double GetPosY2_3();
	void SetPosY2_3(double dPosY);
	double GetPosX2_3();

	void SetPosX2_4(double dPosX);
	double GetPosX2_4();
	void SetPosY2_4(double dPosY);
	double GetPosY2_4();

	void SetAverDist2(double dDist);
	double GetAverDist2();

	BOOL DirectoryExists(LPCTSTR szPath);

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
