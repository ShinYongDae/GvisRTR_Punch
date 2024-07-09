#pragma once
#include "afxwin.h"

#include "Process/ThreadTask.h"

#define TIM_INIT_THREAD			0
#define MAX_THREAD				39

class CManagerThread : public CWnd
{
	CWnd* m_pParent;

	BOOL m_bTIM_INIT_THREAD;
	int m_nStepInitThread;

	void Init();
	BOOL Create();

public:
	CManagerThread(CWnd* pParent = NULL);
	~CManagerThread();

	// 특성입니다.
public:
	DWORD m_dwThreadTick[MAX_THREAD];
	BOOL m_bThread[MAX_THREAD];
	CThreadTask m_Thread[MAX_THREAD];

	BOOL m_bTHREAD_MK[4];	// [0] Auto-Left, [1] Auto-Right, [2] Manual-Left, [3] Manual-Right

	int	m_nStepTHREAD_DISP_DEF;
	BOOL m_bTHREAD_DISP_DEF;
	int	m_nStepTHREAD_DISP_DEF_INNER;
	BOOL m_bTHREAD_DISP_DEF_INNER;

	int	m_nSerialTHREAD_UPDATAE_YIELD[2];	// [0] : Cam0, [1] : Cam1
	BOOL m_bTHREAD_UPDATAE_YIELD[2];		// [0] : Cam0, [1] : Cam1

	BOOL m_bTHREAD_SHIFT2MK;// [2];		// [0] : Cam0, [1] : Cam1
	BOOL m_bTHREAD_UPDATE_REELMAP_UP, m_bTHREAD_UPDATE_REELMAP_ALLUP;
	BOOL m_bTHREAD_UPDATE_REELMAP_DN, m_bTHREAD_UPDATE_REELMAP_ALLDN;
	BOOL m_bTHREAD_REELMAP_YIELD_UP, m_bTHREAD_REELMAP_YIELD_ALLUP;
	BOOL m_bTHREAD_REELMAP_YIELD_DN, m_bTHREAD_REELMAP_YIELD_ALLDN;
	BOOL m_bTHREAD_RELOAD_RST_UP, m_bTHREAD_RELOAD_RST_ALLUP;
	BOOL m_bTHREAD_RELOAD_RST_DN, m_bTHREAD_RELOAD_RST_ALLDN;
	BOOL m_bTHREAD_RELOAD_RST_UP_INNER, m_bTHREAD_RELOAD_RST_ALLUP_INNER;
	BOOL m_bTHREAD_RELOAD_RST_DN_INNER, m_bTHREAD_RELOAD_RST_ALLDN_INNER;
	BOOL m_bTHREAD_RELOAD_RST_ITS, m_bTHREAD_REELMAP_YIELD_ITS;

	BOOL m_bTHREAD_UPDATE_YIELD_UP, m_bTHREAD_UPDATE_YIELD_ALLUP;
	BOOL m_bTHREAD_UPDATE_YIELD_DN, m_bTHREAD_UPDATE_YIELD_ALLDN;
	BOOL m_bTHREAD_UPDATE_YIELD_ITS;
	BOOL m_bTHREAD_UPDATE_YIELD_INNER_UP, m_bTHREAD_UPDATE_YIELD_INNER_ALLUP;
	BOOL m_bTHREAD_UPDATE_YIELD_INNER_DN, m_bTHREAD_UPDATE_YIELD_INNER_ALLDN;
	int	m_nSnTHREAD_UPDATAE_YIELD;

	BOOL m_bTHREAD_MAKE_ITS_FILE_UP, m_bTHREAD_MAKE_ITS_FILE_DN;
	BOOL m_bTHREAD_UPDATE_REELMAP_INNER_UP, m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP;
	BOOL m_bTHREAD_UPDATE_REELMAP_INNER_DN, m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN;
	BOOL m_bTHREAD_UPDATE_REELMAP_ITS;


	// 작업입니다.
public:
	void Reset();
	BOOL InitAct();
	void ThreadKill();

	static UINT ThreadProc0(LPVOID lpContext); // DoMark0(), DoMark1()
	static UINT ThreadProc1(LPVOID lpContext); // ChkCollision()
	static UINT ThreadProc2(LPVOID lpContext); // DispDefImg()
	static UINT ThreadProc3(LPVOID lpContext); // UpdateYield()
	static UINT ThreadProc4(LPVOID lpContext); // RunShift2Mk()
	static UINT ThreadProc5(LPVOID lpContext); // GetCurrentInfoSignal()

	static UINT ThreadProc6(LPVOID lpContext); // UpdateRMapUp()
	static UINT ThreadProc7(LPVOID lpContext); // UpdateRMapDn()
	static UINT ThreadProc8(LPVOID lpContext); // UpdateRMapAllUp()
	static UINT ThreadProc9(LPVOID lpContext); // UpdateRMapAllDn()

	static UINT ThreadProc10(LPVOID lpContext); // UpdateReelmapYieldUp()
	static UINT ThreadProc11(LPVOID lpContext); // UpdateReelmapYieldDn()
	static UINT ThreadProc12(LPVOID lpContext); // UpdateReelmapYieldAllUp()
	static UINT ThreadProc13(LPVOID lpContext); // UpdateReelmapYieldAllDn()

	static UINT ThreadProc14(LPVOID lpContext); // ReloadReelmapUp()
	static UINT ThreadProc15(LPVOID lpContext); // ReloadReelmapDn()
	static UINT ThreadProc16(LPVOID lpContext); // ReloadReelmapAllUp()
	static UINT ThreadProc17(LPVOID lpContext); // ReloadReelmapAllDn()
	static UINT ThreadProc18(LPVOID lpContext); // WriteReelmapIts()
	static UINT ThreadProc19(LPVOID lpContext); // DispDefImgInner()
	static UINT ThreadProc20(LPVOID lpContext); // UpdateReelmapYieldIts()
	static UINT ThreadProc21(LPVOID lpContext); // ReloadReelmapUpInner()
	static UINT ThreadProc22(LPVOID lpContext); // ReloadReelmapDnInner()
	static UINT ThreadProc23(LPVOID lpContext); // ReloadReelmapAllUpInner()
	static UINT ThreadProc24(LPVOID lpContext); // ReloadReelmapAllDnInner()
	static UINT ThreadProc25(LPVOID lpContext); // ReloadReelmapIts()

	static UINT ThreadProc26(LPVOID lpContext); // UpdateRMapInnerUp()
	static UINT ThreadProc27(LPVOID lpContext); // UpdateRMapInnerDn()
	static UINT ThreadProc28(LPVOID lpContext); // UpdateRMapInnerAllUp()
	static UINT ThreadProc29(LPVOID lpContext); // UpdateRMapInnerAllDn()

	static UINT ThreadProc30(LPVOID lpContext); // UpdateYieldUp()
	static UINT ThreadProc31(LPVOID lpContext); // UpdateYieldDn()
	static UINT ThreadProc32(LPVOID lpContext); // UpdateYieldAllUp()
	static UINT ThreadProc33(LPVOID lpContext); // UpdateYieldAllDn()

	static UINT ThreadProc34(LPVOID lpContext); // UpdateYieldInnerUp()
	static UINT ThreadProc35(LPVOID lpContext); // UpdateYieldInnerDn()
	static UINT ThreadProc36(LPVOID lpContext); // UpdateYieldInnerAllUp()
	static UINT ThreadProc37(LPVOID lpContext); // UpdateYieldInnerAllDn()
	static UINT ThreadProc38(LPVOID lpContext); // UpdateYieldIts()

	void GetEnc();
	BOOL IsRunAxisX();
	void EStop();

	BOOL ChkCollision();
	BOOL GetCollision(int nCam);

	void SetPriority();

	BOOL GetDoneMk(int nCam);
	void SetDoneMk(int nCam);

	int GetBufUpSerial(int nCam);
	int GetBufDnSerial(int nCam);

	void DoMark0();
	void DoMark1();
	void DoMark0Its();
	void DoMark1Its();
	void DoReject0();
	void DoReject1();

	void UpdateYield(int nSerial);
	void UpdateYieldUp(int nSerial);
	void UpdateYieldDn(int nSerial);
	void UpdateYieldAllUp(int nSerial);
	void UpdateYieldAllDn(int nSerial);

	void UpdateRMapInnerUp();
	void UpdateRMapInnerDn();
	void UpdateRMapInnerAllUp();
	void UpdateRMapInnerAllDn();

	BOOL GetCurrentInfoSignal();
	void DispDefImg();


	// 생성된 메시지 맵 함수
protected:

public:

	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

