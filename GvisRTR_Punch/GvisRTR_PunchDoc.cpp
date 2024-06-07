
// GvisRTR_PunchDoc.cpp : CGvisRTR_PunchDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "GvisRTR_Punch.h"
#endif

#include "GvisRTR_PunchDoc.h"

#include <propkey.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "GvisRTR_PunchView.h"
extern CGvisRTR_PunchView* pView;

CGvisRTR_PunchDoc* pDoc;

// CGvisRTR_PunchDoc

IMPLEMENT_DYNCREATE(CGvisRTR_PunchDoc, CDocument)

BEGIN_MESSAGE_MAP(CGvisRTR_PunchDoc, CDocument)
END_MESSAGE_MAP()


// CGvisRTR_PunchDoc ����/�Ҹ�

CGvisRTR_PunchDoc::CGvisRTR_PunchDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	pDoc = this;
	m_pFile = new CMyFile();

}

CGvisRTR_PunchDoc::~CGvisRTR_PunchDoc()
{
	DestroyDoc();
}

BOOL CGvisRTR_PunchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}




// CGvisRTR_PunchDoc serialization
void CGvisRTR_PunchDoc::DestroyDoc()
{
	if (m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}
}



void CGvisRTR_PunchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CGvisRTR_PunchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CGvisRTR_PunchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGvisRTR_PunchDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGvisRTR_PunchDoc ����

#ifdef _DEBUG
void CGvisRTR_PunchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGvisRTR_PunchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGvisRTR_PunchDoc ���

int CGvisRTR_PunchDoc::GetTestMode()
{
	return WorkingInfo.LastJob.nTestMode;
}

void CGvisRTR_PunchDoc::SetTestMode(int nMode)
{
	WorkingInfo.LastJob.nTestMode = nMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2 .

	CString sData;
	sData.Format(_T("%d"), nMode);
	::WritePrivateProfileString(_T("Last Job"), _T("Test Mode"), sData, PATH_WORKING_INFO);

	CString sPath = WorkingInfo.System.sPathMkCurrInfo;

	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(_T("Infomation"), _T("Test Mode"), sData, sPath);

	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), sPath);
	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), WorkingInfo.System.sPathMkCurrInfo);
}

void CGvisRTR_PunchDoc::GetMkInfo() // About Engrave Signal
{
	CString sPath = WorkingInfo.System.sPathMkInfo;
	TCHAR szData[512];

	if (sPath.IsEmpty())
		return;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DispContRun"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDispContRun = (_ttoi(szData) > 0) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDispContRun = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DispLotEnd"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDispLotEnd = (_ttoi(szData) > 0) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDispLotEnd = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Use Dual AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDualTest = (_ttoi(szData) > 0) ? TRUE : FALSE;

	WorkingInfo.LastJob.nTestMode = MODE_NONE;
	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Inner Test On"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			WorkingInfo.LastJob.nTestMode = MODE_INNER;
	}

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Outer Test On"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			WorkingInfo.LastJob.nTestMode = MODE_OUTER;
	}

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Sample Test On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bSampleTest = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("UncoilerCcw"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTwoMetal = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("RecoilerCcw"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bOneMetal = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("2PtAlign"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			WorkingInfo.LastJob.nAlignMethode = TWO_POINT;
	}

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("4PtAlign"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			WorkingInfo.LastJob.nAlignMethode = FOUR_POINT;
		else
			WorkingInfo.LastJob.nAlignMethode = TWO_POINT;
	}
	else
		WorkingInfo.LastJob.nAlignMethode = TWO_POINT;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensRecoil"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bRclDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensPunch"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensAoiUp"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensAoiDn"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensEngrave"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bEngvDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensUncoil"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUclDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("SaftySensPunch"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bMkSftySen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("CleanRolerAoiUp"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("CleanRolerAoiDn"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("UltrasonicAoi"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("UltrasonicEngrave"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("TempStop"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bTempPause = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("SeparateLot"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bLotSep = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("FixBed"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bContFixDef = (_ttoi(szData) > 0) ? TRUE : FALSE;

}

void CGvisRTR_PunchDoc::SetMkInfo(CString sMenu, CString sItem, BOOL bOn)
{
	CString sPath = WorkingInfo.System.sPathMkInfo;
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), bOn > 0 ? 1 : 0);
	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CGvisRTR_PunchDoc::SetMkInfo(CString sMenu, CString sItem, CString sData)
{
	CString sPath = WorkingInfo.System.sPathMkInfo;

	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CGvisRTR_PunchDoc::SetMonDispMain(CString sDisp)
{
	CString sPath = WorkingInfo.System.sPathMonDispMain;
	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(_T("Info"), _T("Disp"), sDisp, sPath);
}

// Write Log for Auto
void CGvisRTR_PunchDoc::LogAuto(CString strMsg, int nType)
{
	if (m_bOffLogAuto)
		return;

	CSafeLockDoc lock(&g_LogLockAuto);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, PATH_LOG);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	_stprintf(szPath, PATH_LOG_AUTO);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s: "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r\n");
	strContents += _T("\r\n");

	CFile file;

	if (file.Open(szFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) == 0)
		return;

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	file.Write(cameraKey, nLenth);
	file.Flush();
	file.Close();
}

// Write Log for PLC
void CGvisRTR_PunchDoc::LogPLC(CString strMsg, int nType)
{
	if (m_bOffLogPLC)
		return;

	CSafeLockDoc lock(&g_LogLockPLC);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, PATH_LOG);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	_stprintf(szPath, PATH_LOG_PLC);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s: "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r\n");
	strContents += _T("\r\n");

	CFile file;

	if (file.Open(szFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) == 0)
		return;

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	file.Write(cameraKey, nLenth);
	file.Flush();
	file.Close();
}

void CGvisRTR_PunchDoc::SetEngItsCode(CString sItsCode)
{
	WorkingInfo.LastJob.sEngItsCode = sItsCode;
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Its Code"), sItsCode, PATH_WORKING_INFO);
}

void CGvisRTR_PunchDoc::SaveLotTime(DWORD dwStTick)
{
	CString sData, sPath = PATH_WORKING_INFO;

	// [Lot]
	WorkingInfo.Lot.dwStTick = dwStTick;
	sData.Format(_T("%d"), dwStTick);
	::WritePrivateProfileString(_T("Lot"), _T("Start Tick"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("Start Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("Start Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("Start Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("Start Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("Start Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("Start Second"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("End Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("End Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("End Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("End Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("End Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("End Second"), sData, sPath);
}

void CGvisRTR_PunchDoc::SetEngraveFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sEngraveFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
	long lData = (long)(dPitch * 1000.0);
	pView->MpeWrite(_T("ML45026"), lData);	// ���κ� Feeding �ѷ� Lead Pitch (���� mm * 1000)
}

void CGvisRTR_PunchDoc::SetEngraveAoiDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sFdEngraveAoiInitDist = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), sData, sPath);
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(_T("ML45024"), lData);	// ���κο��� AOI(��)���� �Ÿ� (���� mm * 1000)
}

void CGvisRTR_PunchDoc::SetEngBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sStEngBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_ENG_BUFFER_POSITION"), sData, sPath);
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(_T("ML45028"), lData);	// ���κ� ���� ���� ���� �ѷ� �ʱ���ġ(���� mm * 1000)
}
