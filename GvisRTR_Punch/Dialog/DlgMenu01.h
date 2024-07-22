#pragma once

#include "MyStatic.h"
#include "MyBtn.h"
#include "MyGL.h"
#include "../Process/ReelMap.h"

#define MAX_MENU01_STC_DEFINFO		12
#define MAX_MENU01_STC_TITLE		77
#define MAX_MENU01_STC_DATA			93
#define MAX_MENU01_BTN				23

#define TIM_DISP_DEF_IMG			100
#define TIM_DISP_MK_INFO			101
#define TIM_DISP_MK_CNT				102

// CDlgMenu01 ��ȭ �����Դϴ�.

class CDlgMenu01 : public CDialog
{
	DECLARE_DYNAMIC(CDlgMenu01)

	CRect* m_pRect;
	CReelMap* m_pReelMapDisp;
	CMyGL* m_pMyGL;

	CMyStatic myStcReelmap;
	CMyStatic myStcDefInfo[MAX_MENU01_STC_DEFINFO];
	CMyStatic myStcTitle[MAX_MENU01_STC_TITLE];
	CMyStatic myStcData[MAX_MENU01_STC_DATA];
	CMyBtn myBtn[MAX_MENU01_BTN];

	CRect m_RectTitle[2];
	CRect m_RectCAD[12];
	CRect m_RectDef[12];
	CRect m_RectInfo[12];

	int m_nSerial;
	int m_nSelRmap, m_nSelRmapPrev;
	BOOL m_bEnableMkStrip;
	BOOL m_bTIM_DISP_DEF_IMG;
	BOOL m_bTIM_DISP_MK_CNT;

	int m_nIdxDef[2];					// [0]:AOI-Up , [1]:AOI-Dn ; �ҷ��̹��� �ε���
	int m_nIdxMkInfo[2];				// [0]:AOI-Up , [1]:AOI-Dn ; �ҷ��̹��� ǥ����ġ �ε��� 
	int m_nDef[2];						// [0]:AOI-Up , [1]:AOI-Dn ; One Shot�� �� �ҷ���
	int m_nSerialDispMkInfo;

	BOOL Create();
	void LoadImg();
	void DelImg();

	void AtDlgShow();
	void AtDlgHide();

	void InitGui();
	void InitBtn();
	void InitStatic();
	void InitStcDefInf();
	void InitStcTitle();
	void InitStcData();

	void InitMkInfo();
	void InitMkInfoUp();
	void InitMkInfoDn();

	void InitDefImg();
	void InitDefImgUp();
	void InitDefImgDn();
	void InitDefInfo();
	void InitDefInfoUp();
	void InitDefInfoDn();

	void ClrInfo();
	void ClrCadImg();
	void ClrCadImgUp();
	void ClrCadImgDn();
	void ClrDefImg();
	void ClrDefImgUp();
	void ClrDefImgDn();

	void SetRgbStcDef();
	void SetTitleStcDef();

	void DispStTime();
	void DispEdTime();

	void ResetSerial();
	void ResetLastProc();

	void SwitchReelmapDisp(int nSelRmap);
	void EnableBtn(BOOL bEnable = TRUE);
	void GetCtrlPos();

	void DispMkInfo();
	void DispMkInfoUp();
	void DispMkInfoDn();
	void DispMkInfo(int nSerial);
	void DispMkInfoUp(int nSerial);
	void DispMkInfoDn(int nSerial);

	void SaveCadImgUp(int nSerial, int nIdxMkInfo, int nIdxImg); // (nSerial, ȭ���� IDC �ε���, �ҷ��̹��� �ε���)
	void SaveCadImgDn(int nSerial, int nIdxMkInfo, int nIdxImg); // (nSerial, ȭ���� IDC �ε���, �ҷ��̹��� �ε���)
	BOOL SaveDefImgPosUp(int nSerial, int nIdxMkInfo, int nIdxImg); // (nSerial, ȭ���� IDC �ε���, �ҷ��̹��� �ε���)
	BOOL SaveDefImgPosDn(int nSerial, int nIdxMkInfo, int nIdxImg); // (nSerial, ȭ���� IDC �ε���, �ҷ��̹��� �ε���)
	void WriteDefInfoUp(int nSerial, int nIdxText, int nIdxDef, int nIdxImg); // (nSerial, ȭ���� IDC �ε���, �ҷ��ǽ� �ε���, �ҷ��̹��� �ε���)
	void WriteDefInfoDn(int nSerial, int nIdxText, int nIdxDef, int nIdxImg); // (nSerial, ȭ���� IDC �ε���, �ҷ��ǽ� �ε���, �ҷ��̹��� �ε���)
	void ShowDefInfoUp(int nIdx); // nIdx : 0 ~ 11 (12ea)
	void ShowDefInfoDn(int nIdx); // nIdx : 0 ~ 11 (12ea)
	void ShiftDefInfoUp();
	void ShiftDefInfoDn();
	void ShowDispCadUp(int nIdxMkInfo, int nSerial, int nIdxDef); // From 0 To 12...for Screen display.
	void ShowDispCadDn(int nIdxMkInfo, int nSerial, int nIdxDef); // From 0 To 12...for Screen display.
	void ShowOvrCadUp(int nIdxMkInfo, int nSerial);
	void ShowOvrCadDn(int nIdxMkInfo, int nSerial);
	void ShowDispDefUp(int nIdxMkInfo, int nSerial, int nDefPcs);
	void ShowDispDefDn(int nIdxMkInfo, int nSerial, int nDefPcs);

	int GetPcrIdx0(int nSerial, BOOL bNewLot = FALSE);		// Up - ����ȭ�� ǥ�ø� ���� Display buffer�� Shot �ε���
	int GetPcrIdx1(int nSerial, BOOL bNewLot = FALSE);		// Dn - ����ȭ�� ǥ�ø� ���� Display buffer�� Shot �ε���

public:
	CDlgMenu01(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgMenu01();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MENU_01 };
#endif

	// Ư���Դϴ�.
public:
	BOOL m_bLastProc;

	// �۾��Դϴ�.
public:
	void ChkUserInfo(BOOL bOn);
	void EnableItsMode(BOOL bEnable);
	void DispMain(CString sMsg, COLORREF rgb = RGB_GREEN);
	CString GetRunTime();
	void DispRunTime();
	void DispLotTime();
	void DispLotEndSerial(int nSerial);
	void Reset();
	void ResetLotTime();
	void UpdateData();
	void SelMap(int nSel);
	void InitGL();
	void ResetGL();
	void ResetMkInfo();
	void ResetMkInfoUp();
	void ResetMkInfoDn();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedChkUserInfo();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
