
// GvisRTR_PunchDoc.cpp : CGvisRTR_PunchDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// CGvisRTR_PunchDoc 생성/소멸

CGvisRTR_PunchDoc::CGvisRTR_PunchDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	pDoc = this;
	m_pFile = new CMyFile();

	m_bOffLogAuto = FALSE;
	m_bOffLogPLC = FALSE;

	m_nAoiCamInfoStrPcs[0] = 0;
	m_nAoiCamInfoStrPcs[1] = 0;

	LoadWorkingInfo();
}

CGvisRTR_PunchDoc::~CGvisRTR_PunchDoc()
{
	DestroyDoc();
}

BOOL CGvisRTR_PunchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

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
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGvisRTR_PunchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CGvisRTR_PunchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CGvisRTR_PunchDoc 진단

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


// CGvisRTR_PunchDoc 명령

BOOL CGvisRTR_PunchDoc::LoadWorkingInfo()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;
	pView->ClrDispMsg();

	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("UseDts"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bUseDts = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bUseDts = FALSE;

	// [System]

	if (0 < ::GetPrivateProfileString(_T("System"), _T("OffLogAuto"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bOffLogAuto = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bOffLogAuto = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("OffLogPLC"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bOffLogPLC = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bOffLogPLC = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("DebugGrabAlign"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bDebugGrabAlign = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bDebugGrabAlign = FALSE;

	//if (0 < ::GetPrivateProfileString(_T("DTS"), _T("UseIts"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
	//	m_bUseIts = _ttoi(szData) ? TRUE : FALSE;
	//else
	//	m_bUseIts = FALSE;

	// 20160926 - syd
	if (0 < ::GetPrivateProfileString(_T("System"), _T("USE_RTR_SHIFT_ADJUST"), NULL, szData, sizeof(szData), sPath))
	{
		m_bUseRTRYShiftAdjust = (BOOL)(_ttoi(szData) ? TRUE : FALSE);
	}
	else
		m_bUseRTRYShiftAdjust = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("RTR_SHIFT_ADJUST_RATIO"), NULL, szData, sizeof(szData), sPath))
	{
		m_dShiftAdjustRatio = (double)_ttof(szData);
	}
	else
		m_dShiftAdjustRatio = 0.5;


	if (0 < ::GetPrivateProfileString(_T("System"), _T("Password"), NULL, szData, sizeof(szData), sPath))
		pDoc->m_sPassword = CString(szData);
	else
		pDoc->m_sPassword = _T("gvis");

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MachineName"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sMcName = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("설비이름이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sMcName = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("CamSpecDir"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathCamSpecDir = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("CamMaster의 Spec디렉토리 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathCamSpecDir = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngravePath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEng = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("EngravePath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEng = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngraveCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEngCurrInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("EngraveCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEngCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngraveSignalInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEngSignalInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("EngraveSignalInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEngSignalInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("PunchingCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkCurrInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("PunchingCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkCurrInfo = CString(_T("C:\\PunchWork\\CurrentInfo.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("PunchingSignalInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkSignalInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("PunchingSignalInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkSignalInfo = CString(_T("C:\\PunchWork\\SignalInfo.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpStatusInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpStatusInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AoiUpStatusInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpStatusInfo = CString(_T("C:\\AOIWork\\Statusini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnStatusInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnStatusInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AoiDnStatusInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnStatusInfo = CString(_T("C:\\AOIWork\\Statusini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkWorkPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkWork = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("MonDispMainPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkWork = CString(_T("C:\\PunchWork"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MonDispMainPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMonDispMain = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("MonDispMainPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMonDispMain = CString(_T("C:\\PunchWork\\MonDispMain.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkMenu01Path"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkMenu01 = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("MkMenu01Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkMenu01 = CString(_T("C:\\PunchWork\\MkMenu01.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkMenu03Path"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkMenu03 = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("MkMenu03Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkMenu03 = CString(_T("C:\\PunchWork\\MkMenu03.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("MkInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkInfo = CString(_T("C:\\PunchWork\\MkInfoPath.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("PunchingCurrentInfoBufPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkCurrInfoBuf = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("PunchingCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkCurrInfoBuf = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngraveCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEngOffset = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("EngraveCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEngOffset = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIUpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpCamInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpCamInfo = CString(szData);
	else
	{
		//AfxMessageBox(_T("AOIUpCamInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpCamInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseDTS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseDTS = _ttoi(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("UseDTS가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.bUseDTS = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseITS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseITS = _ttoi(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("UseITS가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.bUseITS = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpDtsPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpDts = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIUpDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpDts = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpDefImgPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpDefImg = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIUpDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpDefImg = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpCurrInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AoiUpCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpOffset = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AoiUpCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUp VRSData Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpVrsData = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIUp VRSData 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpVrsData = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIDnPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnCamInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnCamInfo = CString(szData);
	else
	{
		//AfxMessageBox(_T("AOIDnCamInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnCamInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnDtsPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnDts = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIDnDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnDts = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnDefImgPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnDefImg = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIDnDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnDefImg = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnCurrInfo = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AoiDnCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnOffset = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AoiDnCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDn VRSData Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnVrsData = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOIDn VRSData 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnVrsData = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsShareUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsShareUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VrsShareUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsShareUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsBufferUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsBufUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VrsBufferUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsBufUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsShareDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsShareDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VrsShareDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsShareDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsBufferDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsBufDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VrsBufferDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsBufDn = CString(_T(""));
	}

	// Share folder For VS
	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsShareUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsShareUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VsShareUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsShareUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsBufferDummyUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsDummyBufUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VsBufferUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsDummyBufUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsShareDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsShareDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VsShareDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsShareDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsBufferDummyDnDirPath "), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsDummyBufDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VsBufferDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsDummyBufDn = CString(_T(""));
	}


	//if (0 < ::GetPrivateProfileString(_T("System"), _T("VsUp Data Folder"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.System.sPathVsShareUp = CString(szData);
	//else
	//{
	//	AfxMessageBox(_T("VSUp Share 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
	//	WorkingInfo.System.sPathVsShareUp = CString(_T(""));
	//}
	//
	//if (0 < ::GetPrivateProfileString(_T("System"), _T("VsDn Data Folder"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.System.sPathVsShareDn = CString(szData);
	//else
	//{
	//	AfxMessageBox(_T("VSDn Share 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
	//	WorkingInfo.System.sPathVsShareDn = CString(_T(""));
	//}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsOldFileDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathOldFile = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VRS 완료 파일 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathOldFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsOldFileDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathItsFile = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("ItsOldFileDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathItsFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsFileDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathIts = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("ItsFileDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathIts = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsOldFileDirIpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpPathOldFile = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("VRS 완료 파일 IpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sIpPathOldFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsOldFileDirIpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpPathItsFile = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("ITS 완료 파일 ItsOldFileDirIpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sIpPathItsFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsFileDirIpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpPathIts = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("ITS 완료 파일 ItsFileDirIpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sIpPathIts = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Sapp3Path"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathSapp3 = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Sapp3Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathSapp3 = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveLog"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveLog = _ttoi(szData);
	else
		WorkingInfo.System.bSaveLog = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NO_MARKING"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bNoMk = _ttoi(szData);	// 0 : 마킹모드, 1 : 비젼모드
	else
		WorkingInfo.System.bNoMk = FALSE;			// 0 : 마킹모드, 1 : 비젼모드

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveMkImage"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveMkImg = _ttoi(szData);
	else
		WorkingInfo.System.bSaveMkImg = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveGrabImage"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveGrabImg = _ttoi(szData);
	else
		WorkingInfo.System.bSaveGrabImg = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseStripPcsRgnBin"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bStripPcsRgnBin = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bStripPcsRgnBin = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("REVIEW_MARKING_LENGTH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sReViewMkLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 ReView가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sReViewMkLen = CString(_T(""));
	}

	//if (0 < ::GetPrivateProfileString(_T("System"), _T("USE_REVIEW_MARKING"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.System.bReViewMk = _ttoi(szData) ? TRUE : FALSE;
	//else
	//	WorkingInfo.System.bReViewMk = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MAX_DISP_DEF_IMAGE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sMaxDispDefImg = CString(szData);
	else
		WorkingInfo.System.sMaxDispDefImg = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Cam1Sn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sCamSn[0] = CString(szData);
	else
		WorkingInfo.System.sCamSn[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Cam2Sn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sCamSn[1] = CString(szData);
	else
		WorkingInfo.System.sCamSn[1] = CString(_T(""));

	// READER 2D BARCODE
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_CLIENT_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpClient[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sIpClient[ID_SR1000W] = CString(_T("192.168.100.99"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_SERVER_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpServer[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sIpServer[ID_SR1000W] = CString(_T("192.168.100.100"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_SR1000W] = CString(_T("9004"));

	// ENGRAVE PC
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_SERVER_ENGRAVE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpServer[ID_ENGRAVE] = CString(szData);
	else
		WorkingInfo.System.sIpServer[ID_ENGRAVE] = CString(_T("100.100.100.113"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_ENGRAVE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_ENGRAVE] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_ENGRAVE] = CString(_T("8800"));

	// PUNCH PC
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_CLIENT_PUNCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpClient[ID_PUNCH] = CString(szData);
	else
		WorkingInfo.System.sIpClient[ID_PUNCH] = CString(_T("100.100.101.225"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_PUNCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_PUNCH] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_PUNCH] = CString(_T("8800"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Delay Show Time"), NULL, szData, sizeof(szData), sPath))
		m_nDelayShow = _ttoi(szData);
	else
		m_nDelayShow = 500;

	// [Last Job]
	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("MkSt"), NULL, szData, sizeof(szData), sPath))
		pView->m_bMkSt = _ttoi(szData) > 0 ? TRUE : FALSE;
	else
		pView->m_bMkSt = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("MkStAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStAuto = _ttoi(szData);
	else
		pView->m_nMkStAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Process Code"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sProcessNum = CString(szData);
	else
	{
		WorkingInfo.LastJob.sProcessNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("ModelUp Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sModelUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("ModelUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sModelUp = CString(_T(""));
	}
	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("ModelDn Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sModelDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("ModelDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sModelDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerUp Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLayerUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("LayerUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLayerUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Test Mode"), NULL, szData, sizeof(szData), sPath)) // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
		WorkingInfo.LastJob.nTestMode = _ttoi(szData);
	else
		WorkingInfo.LastJob.nTestMode = 0;

	if (WorkingInfo.LastJob.nTestMode == MODE_INNER)
	{
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
	}
	else if (WorkingInfo.LastJob.nTestMode == MODE_OUTER)
	{
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), TRUE);
	}
	else
	{
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Dual AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDualTest = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDualTest = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("Use Dual AOI"), WorkingInfo.LastJob.bDualTest);

	if (WorkingInfo.LastJob.bDualTest)
	{
		if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerDn Name"), NULL, szData, sizeof(szData), sPath))
			WorkingInfo.LastJob.sLayerDn = CString(szData);
		else
		{
			pView->ClrDispMsg(); AfxMessageBox(_T("LayerDn가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
			WorkingInfo.LastJob.sLayerDn = CString(_T(""));
		}
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LotUp No"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("LotUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotUp = CString(_T(""));
	}
	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LotDn No"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("LotDn가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last SerialUp"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialUp = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("SerialUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last SerialDn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialDn = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("SerialDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last Serial Eng"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialEng = CString(szData);
	else
	{
		//AfxMessageBox(_T("Engrave Serial이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialEng = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sEngraveLastShot = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Engrave Last Shot이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sEngraveLastShot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCompletedSerialUp = CString(szData);
	else
	{
		//AfxMessageBox(_T("SerialUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sCompletedSerialUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCompletedSerialDn = CString(szData);
	else
	{
		//AfxMessageBox(_T("SerialDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sCompletedSerialDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Operator Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSelUserName = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("작업자가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSelUserName = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Reel Total Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sReelTotLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Reel 전체 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sReelTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("One Panel Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sOnePnlLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("한 판넬 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sOnePnlLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bLotSep = _tstoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bLotSep = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("SeparateLot"), WorkingInfo.LastJob.bLotSep);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotSepLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Lot분리 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotSepLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotCutPosLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Lot절단위치 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotCutPosLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTempPause = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bTempPause = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("TempStop"), WorkingInfo.LastJob.bTempPause);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Merging Layer"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nMergingLayer = _ttoi(szData);
	else
		WorkingInfo.LastJob.nMergingLayer = 0; 	// [0]:AOI-Up , [1]:AOI-Dn

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_bUseTempPause = WorkingInfo.LastJob.bTempPause;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sTempPauseLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("일시정지 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sTempPauseLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLmtTotYld = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("전체 수율 제한 범위가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLmtTotYld = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLmtPatlYld = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("부분 수율 제한 범위가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLmtPatlYld = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Continuous Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bContFixDef = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bContFixDef = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("FixBed"), pDoc->WorkingInfo.LastJob.bContFixDef);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sNumRangeFixDef = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("연속 고정불량 판정 누적 판넬수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sNumRangeFixDef = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sNumContFixDef = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("연속 고정불량 판정 발생 판넬수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sNumContFixDef = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sUltraSonicCleannerStTim = CString(szData);
	else
		WorkingInfo.LastJob.sUltraSonicCleannerStTim = CString(_T("5.0"));


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bRclDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bRclDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensRecoil"), pDoc->WorkingInfo.LastJob.bRclDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Marking Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensPunch"), pDoc->WorkingInfo.LastJob.bMkDrSen);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiUp Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiUpDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiUpDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensAoiUp"), pDoc->WorkingInfo.LastJob.bAoiUpDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiDn Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiDnDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiDnDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensAoiDn"), pDoc->WorkingInfo.LastJob.bAoiDnDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Uncoiler Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUclDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUclDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensUncoil"), pDoc->WorkingInfo.LastJob.bUclDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Engrave Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bEngvDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bEngvDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensEngrave"), pDoc->WorkingInfo.LastJob.bEngvDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Buffer Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bBufDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bBufDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use 380mm Roll"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUse380mm = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUse380mm = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Display Marked Piece"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDispMkPcs = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDispMkPcs = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Fix Defect Stop"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bStopFixDef = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bStopFixDef = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Marking Safty Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkSftySen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkSftySen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("SaftySensPunch"), pDoc->WorkingInfo.LastJob.bMkSftySen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Aoi Safty Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiSftySen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiSftySen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sStripOutRatio = CString(szData);
	else
		WorkingInfo.LastJob.sStripOutRatio = _T("20.0"); // Minimum 20%

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCustomNeedRatio = CString(szData);
	else
		WorkingInfo.LastJob.sCustomNeedRatio = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Partial Speed"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sPartialSpd = CString(szData);
	else
		WorkingInfo.LastJob.sPartialSpd = _T("10.0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Up Aoi CleanRoler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiUpCleanRoler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUseAoiUpCleanRoler = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("CleanRolerAoiUp"), pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Dn Aoi CleanRoler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiDnCleanRoler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUseAoiDnCleanRoler = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("CleanRolerAoiDn"), pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Its Code"), NULL, szData, sizeof(szData), sPath))
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = CString(szData);
	else
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Current ShotNum"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCurrentShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sCurrentShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Setting ShotNum"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSettingShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sSettingShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Alarm Time AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlarmTimeAoi = _ttoi(szData);
	else
		WorkingInfo.LastJob.nAlarmTimeAoi = 10800;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Alarm Time Puncking"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlarmTimePunch = _ttoi(szData);
	else
		WorkingInfo.LastJob.nAlarmTimePunch = 10800;


	// 	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Light Value"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.LastJob.sLightVal = CString(szData);
	// 	else
	// 		WorkingInfo.LastJob.sLightVal = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Light On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bLightOn = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bLightOn = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Jog Speed"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sJogSpd = CString(szData);
	else
		WorkingInfo.LastJob.sJogSpd = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bEngraveOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bEngraveOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Verify"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bVerify = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bVerify = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Verify Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sVerifyLen = CString(szData);
	else
		WorkingInfo.LastJob.sVerifyLen = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Verify Period"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nVerifyPeriod = _ttoi(szData);
	else
		WorkingInfo.LastJob.nVerifyPeriod = 0;

	//if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Review"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.LastJob.bReview = _ttoi(szData) ? TRUE : FALSE;
	//else
	//	WorkingInfo.LastJob.bReview = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Shot Num for Furge"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sFurgeNum = CString(szData);
	else
		WorkingInfo.LastJob.sFurgeNum = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Furge Num at Run"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sFurgeRun = CString(szData);
	else
		WorkingInfo.LastJob.sFurgeRun = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Mk Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sMkLastShot = CString(szData);
	else
		WorkingInfo.LastJob.sMkLastShot = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiUp Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sAoiLastShot[0] = CString(szData);
	else
		WorkingInfo.LastJob.sAoiLastShot[0] = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiDn Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sAoiLastShot[1] = CString(szData);
	else
		WorkingInfo.LastJob.sAoiLastShot[1] = _T("0");


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("One Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bOneMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bOneMetal = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("RecoilerCcw"), WorkingInfo.LastJob.bOneMetal);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Two Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTwoMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bTwoMetal = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("UncoilerCcw"), WorkingInfo.LastJob.bTwoMetal);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bSampleTest = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bSampleTest = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("Sample Test On"), WorkingInfo.LastJob.bSampleTest);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSampleTestShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sSampleTestShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Recoiler Core 150mm"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bCore150Recoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bCore150Recoiler = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Uncoiler Core 150mm"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bCore150Uncoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bCore150Uncoiler = FALSE;


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use 2Layer"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUse2Layer = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUse2Layer = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Order Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sEngraveOrderNum = CString(szData);
	else
		WorkingInfo.LastJob.sEngraveOrderNum = _T("");


	//if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.LastJob.sSampleTestShotNum = CString(szData);
	//else
	//	WorkingInfo.LastJob.sSampleTestShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Align Methode"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlignMethode = _ttoi(szData);
	else
		WorkingInfo.LastJob.nAlignMethode = TWO_POINT;

	switch (pDoc->WorkingInfo.LastJob.nAlignMethode)
	{
	case TWO_POINT:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), FALSE);
		break;
	case FOUR_POINT:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), TRUE);
		break;
	default:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), FALSE);
		break;
	}


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Engrave Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseEngraveUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bUseEngraveUltrasonic = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("UltrasonicEngrave"), pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiDn Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiDnUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bUseAoiDnUltrasonic = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("UltrasonicAoi"), pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Cleanner Vel"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bVelEngraveUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bVelEngraveUltrasonic = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner Vel"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bVelAoiDnUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bVelAoiDnUltrasonic = FALSE;


	// [Motion]
	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_ROLLAR_HOMMING_RUN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.bBufHomming = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Motion.bBufHomming = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdAdjOffSet = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량 보정 제한범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdAdjOffSet = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdOvrNum = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량 보정 연속 초과횟수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdOvrNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdErr = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량 보정 Error범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdErr = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdDist = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkJogVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding Jog 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkJogVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdAcc = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkJogAcc = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding Jog 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkJogAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_TOTAL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdTotLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 진행량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TOTAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTotVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 전체 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTotVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_PARTIAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkPatlVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 부분 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkPatlVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdDist = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiJogVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding Jog 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiJogVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdAcc = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiJogAcc = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding Jog 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiJogAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_TOTAL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdTotLen = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 진행량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TOTAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTotVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI 전체 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTotVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_PARTIAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiPatlVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI 부분 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiPatlVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosX[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Pin0 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosY[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Pin0 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosX[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Pin1 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosY[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Pin1 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_X0"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosX[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹0 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_Y0"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosY[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹0 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_X1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosX[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_Y1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosY[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MK_END_POSITION_X1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkEdPosX[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkEdPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MK_END_POSITION_Y1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkEdPosY[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkEdPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStBufPos = CString(szData);
	else
		WorkingInfo.Motion.sStBufPos = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_HOME_SPEED"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sBufHomeSpd = CString(szData);
	else
		WorkingInfo.Motion.sBufHomeSpd = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_HOME_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sBufHomeAcc = CString(szData);
	else
		WorkingInfo.Motion.sBufHomeAcc = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdLead = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdLead = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdMkAoiInitDist = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI에서 마킹부까지 진행거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdMkAoiInitDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdAoiAoiDistShot = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI(상)에서 AOI(하)까지 진행 Shot수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdAoiAoiDistShot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTq = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI 텐션 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTq = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 텐션 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVacOff = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVacOff = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ON_START_WITH_NEW_PRODUCT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.bStNewPrdt = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Motion.bStNewPrdt = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.Motion.bMkTq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	WorkingInfo.Motion.bMkTq = TRUE;

	// 	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.Motion.bAoiTq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	WorkingInfo.Motion.bAoiTq = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVacOff = CString(szData);
	else
		WorkingInfo.Motion.sMkFdVacOff = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVacOff = CString(szData);
	else
		WorkingInfo.Motion.sAoiFdVacOff = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("SAFE_ZONE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sSafeZone = CString(szData);
	else
		WorkingInfo.Motion.sSafeZone = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("COLLISION_LENGTH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sCollisionLength = CString(szData);
	else
		WorkingInfo.Motion.sCollisionLength = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("COLLISION_MARGIN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sCollisionMargin = CString(szData);
	else
		WorkingInfo.Motion.sCollisionMargin = CString(_T(""));

	// ENGRAVE ==========================================================================================================

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdBarcodeOffset = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdBarcodeOffset = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("AOI 검사위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PUNCHING_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdBarcodeOffset = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Punching 시작위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("INIT_POSITION_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sOffsetInitPos = CString(szData);
	else
	{
		//AfxMessageBox(_T("각인부, 검사부, 마킹부 offset 이송 값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sOffsetInitPos = CString(_T("0.0"));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdBarcodeOffsetVel = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("바코드 위치 이송 Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdBarcodeOffsetVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdBarcodeOffsetAcc = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("바코드 위치 이송 Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdBarcodeOffsetAcc = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_AUTO_LIMIT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("바코드 각인 위치 자동 보정 한계값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_MANUAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLaserAdjOffSetManual = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("바코드 각인 위치 수동 보정값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLaserAdjOffSetManual = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdLead = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdVacOff = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인부 롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인기 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveTq = CString(_T(""));
	}

	WorkingInfo.Motion.bEngraveTq = TRUE;	// "ENGRAVE_TENSION_SERVO_TORQUE_ON"


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdEngraveAoiInitDist = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인부에서 마킹부까지 진행거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdEngraveAoiInitDist = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_ERROR_VAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdErr = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding량 보정 Error범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdErr = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding량 보정 제한범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OVER_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdOvrNum = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding량 보정 연속 초과횟수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdOvrNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_START_BUFFER_POSITION"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveStBufPos = CString(szData);
	else
		WorkingInfo.Motion.sEngraveStBufPos = _T("");


	// [VoiceCoil0]

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].sPort = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일0 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[0].sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].sBaudRate = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일0 통신  Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[0].sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[0].bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[0].bLf = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalToq = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftToq = _tstoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftToq = FALSE;

	// [VoiceCoil1]

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].sPort = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일1 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[1].sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].sBaudRate = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일1 통신  Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[1].sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[1].bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[1].bLf = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalToq = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftToq = FALSE;

	// [Fluck RS232 DATA]
#ifdef USE_FLUCK
	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("USE_FLUCK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bUse = _ttoi(szData) ? TRUE : FALSE;
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Fluck 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.bUse = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CONTROL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sPort = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Fluck 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CONTROL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sBaudRate = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Fluck 통신 Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Fluck.bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Fluck.bLf = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("PROBING_THRESHOLD"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sThreshold = CString(szData);
	else
		WorkingInfo.Fluck.sThreshold = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("PROBING_REJECT_MK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sRejectMkNum = CString(szData);
	else
		WorkingInfo.Fluck.sRejectMkNum = CString(_T("20"));
#else

	if (0 < ::GetPrivateProfileString(_T("MarkingTest"), _T("TOTAL_REJECT_MK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sRejectMkNum = CString(szData);
	else
		WorkingInfo.Fluck.sRejectMkNum = CString(_T("20"));
#endif

	// [Light RS232 DATA]
	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sPort = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("조명 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sBaudRate = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("조명 통신 Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Light.bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Light.bLf = FALSE;

	// [Light0]

	if (0 < ::GetPrivateProfileString(_T("Light0"), _T("LIGHT_CHANNEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sCh[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("조명0의 Channel이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sCh[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sVal[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("조명0의 통신 밝기값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sVal[0] = CString(_T(""));
	}

	// [Light1]

	if (0 < ::GetPrivateProfileString(_T("Light1"), _T("LIGHT_CHANNEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sCh[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("조명1의 Channel이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sCh[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sVal[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("조명1의 통신 밝기값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sVal[1] = CString(_T(""));
	}



	// [Vision0]
	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("PIN_IMG_SIZE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sPinImgSz = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Pin 이미지 크기가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sPinImgSz = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("RESOLUTION_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sResX = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sResX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("RESOLUTION_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sResY = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sResY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sMkOffsetX = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sMkOffsetX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sMkOffsetY = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sMkOffsetY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_STD_SCORE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sStdScr = CString(szData);
	else
	{
		//		AfxMessageBox(_T("마킹 ALIGN_STD_SCORE이 설정되어 있지 않습니다."),MB_ICONWARNING|MB_OK);
		WorkingInfo.Vision[0].sStdScr = CString(_T("40"));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosX[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosY[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosX[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosY[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("PtMatchingScore"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sPtMatScr = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("패턴 Matching Score가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sPtMatScr = CString(_T(""));
	}

	// [Vision1]
	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("PIN_IMG_SIZE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sPinImgSz = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Pin 이미지 크기가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sPinImgSz = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("RESOLUTION_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sResX = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sResX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("RESOLUTION_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sResY = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sResY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sMkOffsetX = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sMkOffsetX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sMkOffsetY = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sMkOffsetY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_STD_SCORE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sStdScr = CString(szData);
	else
	{
		//		AfxMessageBox(_T("마킹 ALIGN_STD_SCORE이 설정되어 있지 않습니다."),MB_ICONWARNING|MB_OK);
		WorkingInfo.Vision[1].sStdScr = CString(_T("40"));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosX[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosY[0] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosX[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosY[1] = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("PtMatchingScore"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sPtMatScr = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("패턴 Matching Score가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sPtMatScr = CString(_T(""));
	}


	// [Lot]
	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Tick"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.dwStTick = _ttoi(szData);
	else
		WorkingInfo.Lot.dwStTick = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Year"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nYear = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Month"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nMonth = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Day"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nDay = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Hour"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nHour = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Minute"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nMin = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Second"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nSec = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nSec = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Year"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nYear = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Month"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nMonth = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Day"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nDay = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Hour"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nHour = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Minute"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nMin = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Second"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nSec = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nSec = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sTotalReelDist = CString(szData);
	else
		WorkingInfo.Lot.sTotalReelDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_SEPAREATE_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sSeparateDist = CString(szData);
	else
		WorkingInfo.Lot.sSeparateDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sCuttingDist = CString(szData);
	else
		WorkingInfo.Lot.sCuttingDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_STOP_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sStopDist = CString(szData);
	else
		WorkingInfo.Lot.sStopDist = CString(_T(""));


	// [Marking0]

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitPos = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitVel = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitAcc = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingPos = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingVel = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingAcc = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingToq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_DISP1_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingDisp1Toq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingDisp1Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_DISP2_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingDisp2Toq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingDisp2Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingToqOffset = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingToqOffset = CString(_T("0"));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[0] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[0] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[1] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[1] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[2] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[2] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[3] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[3] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sAverDist = CString(szData);
	else
		WorkingInfo.Marking[0].sAverDist = CString(_T(""));


	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("Marking Count"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].nMkCnt = _ttoi(szData);
	else
		WorkingInfo.Marking[0].nMkCnt = 0;

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("Marking Limit"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].nMkLimit = _ttoi(szData);
	else
		WorkingInfo.Marking[0].nMkLimit = 31536000;


	// [Marking1]

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitPos = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitVel = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitAcc = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingPos = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingVel = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingAcc = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingToq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_DISP1_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingDisp1Toq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingDisp1Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_DISP2_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingDisp2Toq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingDisp2Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingToqOffset = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingToqOffset = CString(_T("0"));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[0] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[0] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[1] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[1] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[2] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[2] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[3] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[3] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sAverDist = CString(szData);
	else
		WorkingInfo.Marking[1].sAverDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("Marking Count"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].nMkCnt = _ttoi(szData);
	else
		WorkingInfo.Marking[1].nMkCnt = 0;

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("Marking Limit"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].nMkLimit = _ttoi(szData);
	else
		WorkingInfo.Marking[1].nMkLimit = 31536000;



	if (0 < ::GetPrivateProfileString(_T("Elec Check"), _T("Use"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].bUse = WorkingInfo.Probing[1].bUse = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Probing[0].bUse = WorkingInfo.Probing[1].bUse = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Elec Check"), _T("StopOnOpen"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].bStopOnOpen = WorkingInfo.Probing[1].bStopOnOpen = _tstoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Probing[0].bStopOnOpen = WorkingInfo.Probing[1].bStopOnOpen = TRUE;


	// [ElecCheck0]

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitPos = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitVel = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitAcc = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingPos = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingVel = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingAcc = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingToq = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSX"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sMeasurePosX = CString(szData);
	else
		WorkingInfo.Probing[0].sMeasurePosX = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sMeasurePosY = CString(szData);
	else
		WorkingInfo.Probing[0].sMeasurePosY = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("REPEAT_TEST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sRepeatTestNum = CString(szData);
	else
		WorkingInfo.Probing[0].sRepeatTestNum = CString(_T("3"));


	// [ElecCheck1]

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitPos = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitVel = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitAcc = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingPos = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingVel = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingAcc = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingToq = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSX"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sMeasurePosX = CString(szData);
	else
		WorkingInfo.Probing[1].sMeasurePosX = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sMeasurePosY = CString(szData);
	else
		WorkingInfo.Probing[1].sMeasurePosY = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("REPEAT_TEST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sRepeatTestNum = CString(szData);
	else
		WorkingInfo.Probing[1].sRepeatTestNum = CString(_T("3"));


	// Sapp3code =====================================================================================

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("OPEN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] = _ttoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_OPEN] = 0;

	// 	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SHORT_USHORT"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT_USHORT] = _ttoi(szData);
	// 	else
	// 		WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT_USHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SHORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] = _ttoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_SHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("USHORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] = _ttoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_USHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("NICK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] = _tstoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_NICK] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SPACE_EXTRA_PROTRUSION"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] = _ttoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] = 0;

	// 	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PINHOLE_PAD"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE_PAD] = _ttoi(szData);
	// 	else
	// 		WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE_PAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PINHOLE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] = _ttoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_PINHOLE] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PAD"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] = _ttoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_PAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("HOPEN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] = _ttoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_HOPEN] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("HMISS_HPOS_HBAD"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] = _tstoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("VHOPEN_NOVH_VHALIGN_VHDEF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] = _tstoi(szData);
	else
		WorkingInfo.System.m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] = 0;


	return TRUE;
}

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
	pView->MpeWrite(_T("ML45026"), lData);	// 각인부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}

void CGvisRTR_PunchDoc::SetEngraveAoiDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sFdEngraveAoiInitDist = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), sData, sPath);
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(_T("ML45024"), lData);	// 각인부에서 AOI(상)까지 거리 (단위 mm * 1000)
}

void CGvisRTR_PunchDoc::SetEngBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sStEngBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_ENG_BUFFER_POSITION"), sData, sPath);
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(_T("ML45028"), lData);	// 각인부 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
}


double CGvisRTR_PunchDoc::GetOffsetInitPos()
{
	TCHAR szData[200];
	CString sPath = PATH_WORKING_INFO;
	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("INIT_POSITION_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sOffsetInitPos = CString(szData);
	else
	{
		//AfxMessageBox(_T("각인부, 검사부, 마킹부 offset 이송 값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sOffsetInitPos = CString(_T("0.0"));
	}

	return (_tstof(WorkingInfo.Motion.sOffsetInitPos));
}

BOOL CGvisRTR_PunchDoc::GetCurrentInfoSignal(int nIdxSig)
{
	TCHAR szData[200];
	CString sData, sIdx, sPath = WorkingInfo.System.sPathEngSignalInfo;

	if (sPath.IsEmpty())
		return FALSE;

	sIdx.Format(_T("%d"), nIdxSig);
	if (0 < ::GetPrivateProfileString(_T("Signal"), sIdx, NULL, szData, sizeof(szData), sPath))
		return (_ttoi(szData) > 0 ? TRUE : FALSE);

	return FALSE;
}

void CGvisRTR_PunchDoc::SetCurrentInfoSignal(int nIdxSig, BOOL bOn)
{
	CString sData, sIdx, sPath = WorkingInfo.System.sPathMkSignalInfo;

	if (sPath.IsEmpty())
		return;

	sIdx.Format(_T("%d"), nIdxSig);
	sData.Format(_T("%d"), bOn ? 1 : 0);
	::WritePrivateProfileString(_T("Signal"), sIdx, sData, sPath);
}

int CGvisRTR_PunchDoc::GetAoiUpAutoSerial()
{
	int nSerial = 0;
	CString sPath = WorkingInfo.System.sPathAoiUpStatusInfo;
	TCHAR szData[512];

	if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nSerial"), NULL, szData, sizeof(szData), sPath))
		nSerial = _ttoi(szData);
	else
		nSerial = -1;

	return nSerial;
}

void CGvisRTR_PunchDoc::SetAoiUpAutoSerial(int nSerial)
{
	CString sPath = WorkingInfo.System.sPathAoiUpStatusInfo;
	CString str;
	str.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Auto"), _T("nSerial"), str, sPath);
}

int CGvisRTR_PunchDoc::GetAoiUpAutoStep()
{
	TCHAR szData[512];
	CString sPath = WorkingInfo.System.sPathAoiUpStatusInfo;
	if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nStep"), NULL, szData, sizeof(szData), sPath))
		return _ttoi(szData);

	return (-1);
}

void CGvisRTR_PunchDoc::SetAoiUpAutoStep(int nStep)
{
	CString sPath = WorkingInfo.System.sPathAoiUpStatusInfo;
	CString str;
	str.Format(_T("%d"), nStep);
	::WritePrivateProfileString(_T("Auto"), _T("nStep"), str, sPath);
}

int CGvisRTR_PunchDoc::GetAoiDnAutoSerial()
{
	int nSerial = 0;
	CString sPath = WorkingInfo.System.sPathAoiDnStatusInfo;
	TCHAR szData[512];

	if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nSerial"), NULL, szData, sizeof(szData), sPath))
		nSerial = _ttoi(szData);
	else
		nSerial = -1;

	return nSerial;
}

void CGvisRTR_PunchDoc::SetAoiDnAutoSerial(int nSerial)
{
	CString sPath = WorkingInfo.System.sPathAoiDnStatusInfo;
	CString str;
	str.Format(_T("%d"), nSerial);
	::WritePrivateProfileString(_T("Auto"), _T("nSerial"), str, sPath);
}

int CGvisRTR_PunchDoc::GetAoiDnAutoStep()
{
	TCHAR szData[512];
	CString sPath = WorkingInfo.System.sPathAoiDnStatusInfo;
	if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nStep"), NULL, szData, sizeof(szData), sPath))
		return _ttoi(szData);

	return (-1);
}

void CGvisRTR_PunchDoc::SetAoiDnAutoStep(int nStep)
{
	CString sPath = WorkingInfo.System.sPathAoiDnStatusInfo;
	CString str;
	str.Format(_T("%d"), nStep);
	::WritePrivateProfileString(_T("Auto"), _T("nStep"), str, sPath);
}

CString CGvisRTR_PunchDoc::GetAoiUpAlarmRestartMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("11"), _T("27"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

CString CGvisRTR_PunchDoc::GetAoiUpAlarmReTestMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("20"), _T("9"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

CString CGvisRTR_PunchDoc::GetAoiDnAlarmRestartMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("11"), _T("28"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

CString CGvisRTR_PunchDoc::GetAoiDnAlarmReTestMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("20"), _T("10"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

BOOL CGvisRTR_PunchDoc::GetAoiUpVsStatus()
{
	BOOL bVsStatus = FALSE;
#ifndef TEST_MODE
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpCurrInfo;
	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current VS Status"), NULL, szData, sizeof(szData), sPath))
		bVsStatus = _tstoi(szData) > 0 ? TRUE : FALSE;
#endif
	return bVsStatus;
}

int CGvisRTR_PunchDoc::GetLastShotMk()	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.
{
	if (!pView->m_mgrStatus)
		return 0;

	stGeneral& General = (pView->m_mgrStatus->General);

	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nMkLastShot;

	BOOL bDualTest = WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (!nLastShot && General.bCont)
		{
			CFileFind FF;
			CString sPath, sRmapPath, sMsg;
			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				sPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLotUp,
					WorkingInfo.LastJob.sLayerDn);
			else
				sPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLotUp,
					WorkingInfo.LastJob.sLayerDn);

			sRmapPath.Format(_T("%s\\ReelMapDataDn.txt"), sPath);
			BOOL bExist = FF.FindFile(sRmapPath);

			if (!bExist) // Can not find file.
			{
				return 0; // Failed.
			}

			TCHAR szData[MAX_PATH];
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sRmapPath))
				nLastShot = _tstoi(szData);
			else
				nLastShot = 0; // Failed.
		}
	}
	else
	{
		if (!nLastShot && General.bCont)
		{
			CFileFind FF;
			CString sPath, sRmapPath, sMsg;
			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				sPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLotUp,
					WorkingInfo.LastJob.sLayerUp);
			else
				sPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModelUp,
					WorkingInfo.LastJob.sLotUp,
					WorkingInfo.LastJob.sLayerUp);

			sRmapPath.Format(_T("%s\\ReelMapDataUp.txt"), sPath);
			BOOL bExist = FF.FindFile(sRmapPath);

			if (!bExist) // Can not find file.
			{
				return 0; // Failed.
			}

			TCHAR szData[MAX_PATH];
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sRmapPath))
				nLastShot = _tstoi(szData);
			else
				nLastShot = 0; // Failed.
		}
	}

	return (nLastShot);
}

BOOL CGvisRTR_PunchDoc::GetAoiUpOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathAoiUpOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
		OfSt.x = _tstof(szData);
	else
	{
		OfSt.x = 0.0;
		bRtn = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
		OfSt.y = _tstof(szData);
	else
	{
		OfSt.y = 0.0;
		bRtn = FALSE;
	}

	return bRtn;
}

BOOL CGvisRTR_PunchDoc::GetAoiDnOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathAoiDnOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
		OfSt.x = -1.0*_tstof(szData);
	else
	{
		OfSt.x = 0.0;
		bRtn = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
		OfSt.y = _tstof(szData);
	else
	{
		OfSt.y = 0.0;
		bRtn = FALSE;
	}

	return bRtn;
}

BOOL CGvisRTR_PunchDoc::GetEngOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathEngOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

	if (sPath.IsEmpty())
		return FALSE;

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
		OfSt.x = _tstof(szData);
	else
	{
		OfSt.x = 0.0;
		bRtn = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
		OfSt.y = _tstof(szData);
	else
	{
		OfSt.y = 0.0;
		bRtn = FALSE;
	}

	return bRtn;
}

void CGvisRTR_PunchDoc::SetEngraveReaderDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.s2DEngLen = WorkingInfo.Motion.sEngraveFdBarcodeOffset = sData;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), sData, sPath);
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(_T("ML45090"), lData);	// 각인위치에서 2D 바코드 리더기까지의 Offset (단위 mm * 1000)
}


void CGvisRTR_PunchDoc::SetOnePnlLen(double dLen)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLen);
	WorkingInfo.LastJob.sOnePnlLen = sVal;
	WorkingInfo.Motion.sMkFdDist = WorkingInfo.Motion.sAoiFdDist = sVal;
	m_pReelMap->m_dPnlLen = dLen;
	::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), sVal, sPath);
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(_T("ML45032"), lData);	// 한 판넬 길이 (단위 mm * 1000)
}

double CGvisRTR_PunchDoc::GetOnePnlLen()
{
	return (_tstof(WorkingInfo.Motion.sMkFdDist));
}

void CGvisRTR_PunchDoc::SetOnePnlVel(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkFdVel = WorkingInfo.Motion.sAoiFdVel = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), sVal, sPath);
	long lData = (long)(dVel * 1000.0);
	pView->MpeWrite(_T("ML45034"), lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
}

void CGvisRTR_PunchDoc::SetFdJogVel(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkJogVel = WorkingInfo.Motion.sAoiJogVel = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), sVal, sPath);
	long lData = (long)(dVel * 1000.0);
	pView->MpeWrite(_T("ML45038"), lData);	// 연속공급 속도 (단위 mm/sec * 1000)
}

void CGvisRTR_PunchDoc::SetFdJogAcc(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkJogAcc = WorkingInfo.Motion.sAoiJogAcc = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), sVal, sPath);
	long lData = (long)(dVel * 1000.0);
	pView->MpeWrite(_T("ML45040"), lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)
}

double CGvisRTR_PunchDoc::GetOnePnlVel()
{
	return (_tstof(WorkingInfo.Motion.sMkFdVel));
}

double CGvisRTR_PunchDoc::GetFdJogVel()
{
	return (_tstof(WorkingInfo.Motion.sMkJogVel));
}

double CGvisRTR_PunchDoc::GetFdJogAcc()
{
	return (_tstof(WorkingInfo.Motion.sMkJogAcc));
}

void CGvisRTR_PunchDoc::SetOnePnlAcc(double dAcc)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dAcc);
	WorkingInfo.Motion.sMkFdAcc = WorkingInfo.Motion.sAoiFdAcc = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), sVal, sPath);
	long lData = (long)(dAcc * 1000.0);
	pView->MpeWrite(_T("ML45036"), lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)
}

double CGvisRTR_PunchDoc::GetOnePnlAcc()
{
	return (_tstof(WorkingInfo.Motion.sMkFdAcc));
}

void CGvisRTR_PunchDoc::SetAoiFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sAoiFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
	long lData = (long)(dPitch * 1000.0);
	pView->MpeWrite(_T("ML45012"), lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	pView->MpeWrite(_T("ML45020"), lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}

double CGvisRTR_PunchDoc::GetAoiFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sAoiFdLead));
}

void CGvisRTR_PunchDoc::SetMkFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sMkFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
	long lData = (long)(dPitch * 1000.0);
	pView->MpeWrite(_T("ML45014"), lData);	// 마킹부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}

double CGvisRTR_PunchDoc::GetMkFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sMkFdLead));
}

void CGvisRTR_PunchDoc::SetFdErrLmt(double dLmt)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLmt);
	WorkingInfo.Motion.sLmtFdErr = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), sVal, sPath);
}

double CGvisRTR_PunchDoc::GetFdErrLmt()
{
	return (_tstof(WorkingInfo.Motion.sLmtFdErr));
}

void CGvisRTR_PunchDoc::SetFdErrRng(double dRng)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dRng);
	WorkingInfo.Motion.sLmtFdAdjOffSet = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), sVal, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetFdDiffRng();	//_ItemInx::_FdDiffRng
	//#endif
}

double CGvisRTR_PunchDoc::GetFdErrRng()
{
	return (_tstof(WorkingInfo.Motion.sLmtFdAdjOffSet));
}

void CGvisRTR_PunchDoc::SetFdErrNum(int nNum)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%d"), nNum);
	WorkingInfo.Motion.sLmtFdOvrNum = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), sVal, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetFdDiffNum();	//_ItemInx::_FdDiffNum
	//#endif
}

int CGvisRTR_PunchDoc::GetFdErrNum()
{
	return (_tstoi(WorkingInfo.Motion.sLmtFdOvrNum));
}

void CGvisRTR_PunchDoc::SetBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sStBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(_T("ML45016"), lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetMkBuffInitPos();	//_ItemInx::_MkBuffInitPos
												//#endif
}

void CGvisRTR_PunchDoc::SetEngBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sStEngBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_ENG_BUFFER_POSITION"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(_T("ML45028"), lData);	// 각인부 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
#endif
}

void CGvisRTR_PunchDoc::SetBufInitPos(double dVel, double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sBufHomeSpd = sData;
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_HOME_SPEED"), sData, sPath);
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Motion.sBufHomeAcc = sData;
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_HOME_ACC"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetBuffInitPos()
{
	return (_tstof(WorkingInfo.Motion.sStBufPos));
}

void CGvisRTR_PunchDoc::SetAoiMkDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sFdMkAoiInitDist = sData;
	//::WritePrivateProfileString(_T("Motion"), _T("FEEDING_INIT_DIST_OFFSET"), sData, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(_T("ML45008"), lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetAoiMkLen();	//_ItemInx::_AoiMkLen
												//#endif
}

void CGvisRTR_PunchDoc::SetAoiAoiDist(int nShot)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nShot);
	WorkingInfo.Motion.sFdAoiAoiDistShot = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(nShot * 1000);
	pView->MpeWrite(_T("ML45010"), lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetAoiBuffShotNum();	//_ItemInx::_AoiBuffShotNum
												//#endif
}

double CGvisRTR_PunchDoc::GetAoiMkDist()
{
	return (_tstof(WorkingInfo.Motion.sFdMkAoiInitDist));
}

int CGvisRTR_PunchDoc::GetAoiAoiDist()
{
	return (_tstoi(WorkingInfo.Motion.sFdAoiAoiDistShot));
}

void CGvisRTR_PunchDoc::SetWaitPos1(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[0].sWaitPos = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetWaitPos1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitPos));
}

void CGvisRTR_PunchDoc::SetWaitVel1(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[0].sWaitVel = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetWaitVel1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitVel));
}

void CGvisRTR_PunchDoc::SetWaitAcc1(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[0].sWaitAcc = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetWaitAcc1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitAcc));
}

void CGvisRTR_PunchDoc::SetMarkingPos1(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[0].sMarkingPos = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingPos1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingPos));
}

void CGvisRTR_PunchDoc::SetMarkingVel1(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[0].sMarkingVel = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingVel1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingVel));
}

void CGvisRTR_PunchDoc::SetMarkingAcc1(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[0].sMarkingAcc = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingAcc1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingAcc));
}

void CGvisRTR_PunchDoc::SetMarkingToq1(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Marking[0].sMarkingToq = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), sData, sPath);
}

void CGvisRTR_PunchDoc::SetMarkingToq1Offset(double dToqOffset)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToqOffset);
	WorkingInfo.Marking[0].sMarkingToqOffset = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ_OFFSET"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingToq1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingToq));
}

void CGvisRTR_PunchDoc::SetPosX1_1(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[0] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX1Lf();	//_ItemInx::_MkHgtPosX1Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX1_1()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[0]));
}

void CGvisRTR_PunchDoc::SetPosY1_1(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[0] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY1Lf();	//_ItemInx::_MkHgtPosY1Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY1_1()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[0]));
}

void CGvisRTR_PunchDoc::SetPosX1_2(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[1] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX2Lf();	//_ItemInx::_MkHgtPosX2Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX1_2()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[1]));
}

void CGvisRTR_PunchDoc::SetPosY1_2(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[1] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY2Lf();	//_ItemInx::_MkHgtPosY2Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY1_2()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[1]));
}

void CGvisRTR_PunchDoc::SetPosX1_3(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[2] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX3Lf();	//_ItemInx::_MkHgtPosX3Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX1_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[2]));
}

void CGvisRTR_PunchDoc::SetPosY1_3(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[2] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY3Lf();	//_ItemInx::_MkHgtPosY3Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY1_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[2]));
}

void CGvisRTR_PunchDoc::SetPosX1_4(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[3] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX4Lf();	//_ItemInx::_MkHgtPosX4Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX1_4()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[3]));
}

void CGvisRTR_PunchDoc::SetPosY1_4(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[3] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY4Lf();	//_ItemInx::_MkHgtPosY4Lf
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY1_4()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[3]));
}

void CGvisRTR_PunchDoc::SetAverDist1(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Marking[0].sAverDist = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtAvgPosLf();	//_ItemInx::_MkHgtAvgPosLf
	//#endif
}

double CGvisRTR_PunchDoc::GetAverDist1()
{
	return (_tstof(WorkingInfo.Marking[0].sAverDist));
}

void CGvisRTR_PunchDoc::SetWaitPos2(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[1].sWaitPos = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetWaitPos2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitPos));
}

void CGvisRTR_PunchDoc::SetWaitVel2(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[1].sWaitVel = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetWaitVel2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitVel));
}

void CGvisRTR_PunchDoc::SetWaitAcc2(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[1].sWaitAcc = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetWaitAcc2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitAcc));
}

void CGvisRTR_PunchDoc::SetMarkingPos2(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[1].sMarkingPos = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingPos2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingPos));
}

void CGvisRTR_PunchDoc::SetMarkingVel2(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[1].sMarkingVel = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingVel2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingVel));
}

void CGvisRTR_PunchDoc::SetMarkingAcc2(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[1].sMarkingAcc = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingAcc2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingAcc));
}

void CGvisRTR_PunchDoc::SetMarkingToq2(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Marking[1].sMarkingToq = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), sData, sPath);
}

void CGvisRTR_PunchDoc::SetMarkingToq2Offset(double dToqOffset)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToqOffset);
	WorkingInfo.Marking[1].sMarkingToqOffset = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ_OFFSET"), sData, sPath);
}

double CGvisRTR_PunchDoc::GetMarkingToq2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingToq));
}

void CGvisRTR_PunchDoc::SetPosX2_1(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[0] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX1Rt();	//_ItemInx::_MkHgtPosX1Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX2_1()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[0]));
}

void CGvisRTR_PunchDoc::SetPosY2_1(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[0] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY1Rt();	//_ItemInx::_MkHgtPosY1Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY2_1()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[0]));
}

void CGvisRTR_PunchDoc::SetPosX2_2(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[1] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX2Rt();	//_ItemInx::_MkHgtPosX2Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX2_2()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[1]));
}

void CGvisRTR_PunchDoc::SetPosY2_2(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[1] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY2Rt();	//_ItemInx::_MkHgtPosY2Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY2_2()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[1]));
}

void CGvisRTR_PunchDoc::SetPosX2_3(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[2] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX3Rt();	//_ItemInx::_MkHgtPosX3Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX2_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[2]));
}

void CGvisRTR_PunchDoc::SetPosY2_3(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[2] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY3Rt();	//_ItemInx::_MkHgtPosY3Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY2_3()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[2]));
}

void CGvisRTR_PunchDoc::SetPosX2_4(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[3] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosX4Rt();	//_ItemInx::_MkHgtPosX4Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosX2_4()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[3]));
}

void CGvisRTR_PunchDoc::SetPosY2_4(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[3] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtPosY4Rt();	//_ItemInx::_MkHgtPosY4Rt
	//#endif
}

double CGvisRTR_PunchDoc::GetPosY2_4()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[3]));
}

void CGvisRTR_PunchDoc::SetAverDist2(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Marking[1].sAverDist = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), sData, sPath);

	//#ifdef USE_ENGRAVE
	//	if (pView && pView->m_pEngrave)
	//		pView->m_pEngrave->SetMkHgtAvgPosRt();	//_ItemInx::_MkHgtAvgPosRt
	//#endif
}

double CGvisRTR_PunchDoc::GetAverDist2()
{
	return (_tstof(WorkingInfo.Marking[1].sAverDist));
}

void CGvisRTR_PunchDoc::SetTotalReelDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sTotalReelDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), sData, sPath);


	WorkingInfo.LastJob.sReelTotLen = sData;
	if (m_pReelMap)
		m_pReelMap->m_dTotLen = _tstof(sData) * 1000.0;
	::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	long lData = (long)(dDist * 1000.0);
	if (pView && pView->m_pMpe)
		pView->MpeWrite(_T("ML45000"), lData);	// 전체 Reel 길이 (단위 M * 1000)
#endif

													//#ifdef USE_ENGRAVE
													//	if (pView && pView->m_pEngrave)
													//		pView->m_pEngrave->SetTotReelLen();	//_ItemInx::_TotReelLen
													//#endif
}

double CGvisRTR_PunchDoc::GetTotalReelDist()
{
	return (_tstof(WorkingInfo.Lot.sTotalReelDist));
}

void CGvisRTR_PunchDoc::SetSeparateDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sSeparateDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_SEPAREATE_DIST"), sData, sPath);

	pDoc->WorkingInfo.LastJob.sLotSepLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dLotLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	long lData = (long)(dDist * 1000.0);
	pView->MpeWrite(_T("ML45002"), lData);	// Lot 분리 길이 (단위 M * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetLotCutLen();	//_ItemInx::_SetData
												//#endif
}

double CGvisRTR_PunchDoc::GetSeparateDist()
{
	return (_tstof(WorkingInfo.Lot.sSeparateDist));
}

void CGvisRTR_PunchDoc::SetCuttingDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sCuttingDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), sData, sPath);

	pDoc->WorkingInfo.LastJob.sLotCutPosLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dLotCutPosLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	//pView->IoWrite("ML45004", long(_tstof(sVal)*1000.0));	// Lot 분리 후 절단위치 (단위 M * 1000)
	// 	pView->MpeWrite(_T("ML45004", long(_tstof(sVal)*1000.0));

	long lData = (long)(dDist * 1000.0);
	pView->MpeWrite(_T("ML45004"), lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetLotCutPosLen();	//_ItemInx::_LotCutPosLen
												//#endif
}

double CGvisRTR_PunchDoc::GetCuttingDist()
{
	return (_tstof(WorkingInfo.Lot.sCuttingDist));
}

void CGvisRTR_PunchDoc::SetStopDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sStopDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_STOP_DIST"), sData, sPath);


	pDoc->WorkingInfo.LastJob.sTempPauseLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dTempPauseLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), sData, PATH_WORKING_INFO);

#ifdef USE_MPE
	//pView->IoWrite("ML45006", long(_tstof(sVal)*1000.0));	// 일시정지 길이 (단위 M * 1000)
	// 	pView->MpeWrite(_T("ML45006"), long(_tstof(sVal)*1000.0));

	long lData = (long)(dDist * 1000.0);
	pView->MpeWrite(_T("ML45006"), lData);	// 일시정지 길이 (단위 M * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetTempStopLen();	//_ItemInx::_TempStopLen
												//#endif
}

double CGvisRTR_PunchDoc::GetStopDist()
{
	return (_tstof(WorkingInfo.Lot.sStopDist));
}

void CGvisRTR_PunchDoc::SetAOIToq(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sAoiTq = sData;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dToq * 1000.0);
	pView->MpeWrite(_T("ML45042"), lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetAoiTqVal();	//_ItemInx::_AoiTqVal
												//#endif
}

double CGvisRTR_PunchDoc::GetAOIToq()
{
	return (_tstof(WorkingInfo.Motion.sAoiTq));
}

void CGvisRTR_PunchDoc::SetMarkingToq(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sMkTq = sData;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), sData, sPath);
#ifdef USE_MPE
	long lData = (long)(dToq * 1000.0);
	pView->MpeWrite(_T("ML45044"), lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000)
#endif

												//#ifdef USE_ENGRAVE
												//	if (pView && pView->m_pEngrave)
												//		pView->m_pEngrave->SetMkTqVal();	//_ItemInx::_MkTqVal
												//#endif
}

double CGvisRTR_PunchDoc::GetEngraveToq()
{
	return (_tstof(WorkingInfo.Motion.sEngraveTq));
}

BOOL CGvisRTR_PunchDoc::GetCurrentInfoEng()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sPath = WorkingInfo.System.sPathEngCurrInfo;
	TCHAR szData[512];
	BOOL bRtn = FALSE;

#ifdef TEST_MODE
	return bRtn;
#endif

	if (sPath.IsEmpty() || (GetTestMode() != MODE_INNER && GetTestMode() != MODE_OUTER))
		return bRtn;

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Dual Test"), NULL, szData, sizeof(szData), sPath))
		m_bEngDualTest = _ttoi(szData) > 0 ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Its Code"), NULL, szData, sizeof(szData), sPath))
		m_sItsCode = CString(szData);

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Lot"), NULL, szData, sizeof(szData), sPath))
		m_sEngLotNum = CString(szData);
	//WorkingInfo.LastJob.sLotUp = CString(szData);

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Process Unit Code"), NULL, szData, sizeof(szData), sPath))
		m_sEngProcessNum = CString(szData);
	//WorkingInfo.LastJob.sProcessNum = CString(szData);

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Model Up"), NULL, szData, sizeof(szData), sPath))
		m_sEngModel = CString(szData);
	//WorkingInfo.LastJob.sModelUp = CString(szData);

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Layer Up"), NULL, szData, sizeof(szData), sPath))
		m_sEngLayerUp = CString(szData);
	//WorkingInfo.LastJob.sLayerUp = CString(szData);

	if (bDualTest)
	{
		WorkingInfo.LastJob.sLotDn = WorkingInfo.LastJob.sLotUp;

		//if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Model Dn"), NULL, szData, sizeof(szData), sPath))
		//WorkingInfo.LastJob.sModelDn = CString(szData);

		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Layer Dn"), NULL, szData, sizeof(szData), sPath))
			m_sEngLayerDn = CString(szData);
		//WorkingInfo.LastJob.sLayerDn = CString(szData);
	}

	if (m_sItsCode.IsEmpty() || m_sEngLotNum.IsEmpty() || m_sEngModel.IsEmpty() || m_sEngLayerUp.IsEmpty())
		return bRtn;

	return TRUE;
}

int CGvisRTR_PunchDoc::GetCurrentInfoEngShotNum()
{
	CString sPath = WorkingInfo.System.sPathEngCurrInfo;
	TCHAR szData[512];
#ifdef TEST_MODE
	return 0;
#endif

	if (sPath.IsEmpty())
		return 0;

	if (0 < ::GetPrivateProfileString(_T("Work"), _T("Shot Num"), NULL, szData, sizeof(szData), sPath))
		return(_ttoi(szData));

	return 0;
}

int CGvisRTR_PunchDoc::CopyPcrAll()  // from share to buffer ; return : Serial
{
	int nS0, nS1;
	nS0 = CopyPcrUp();
	nS1 = CopyPcrDn();
	if (nS0 > 0 || nS1 > 0)
		return TRUE;
	return FALSE;
}

int CGvisRTR_PunchDoc::CopyPcrUp()  // return : Serial
{
	int nSerial;
	CString sSrc = WorkingInfo.System.sPathVrsShareUp;
	CString sDest = WorkingInfo.System.sPathVrsBufUp;

	if (m_pFile)
		nSerial = m_pFile->CopyPcrAll(sSrc, sDest);

	return nSerial;
}

int CGvisRTR_PunchDoc::CopyPcrDn()  // return : Serial
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nSerial;
	CString sSrc = WorkingInfo.System.sPathVrsShareDn;
	CString sDest = WorkingInfo.System.sPathVrsBufDn;

	if (m_pFile)
		nSerial = m_pFile->CopyPcrAll(sSrc, sDest);

	return nSerial;
}

void CGvisRTR_PunchDoc::DelSharePcr()
{
	DelSharePcrUp();
	DelSharePcrDn();
}

void CGvisRTR_PunchDoc::DelSharePcrUp()
{
	CString sPath = WorkingInfo.System.sPathVrsShareUp;

	if (m_pFile)
	{
		while (m_pFile->IsPcrExist(sPath))
		{
			m_pFile->DelPcrAll(sPath);
			Sleep(30);
		}
	}
}

void CGvisRTR_PunchDoc::DelSharePcrDn()
{
	CString sPath = WorkingInfo.System.sPathVrsShareDn;

	while (m_pFile->IsPcrExist(sPath))
	{
		if (m_pFile)
			m_pFile->DelPcrAll(sPath);
		Sleep(30);
	}
}

void CGvisRTR_PunchDoc::DelShareVsPcrUp()
{
	CString sPath = WorkingInfo.System.sPathVsShareUp;

	if (m_pFile)
	{
		while (m_pFile->IsPcrExist(sPath))
		{
			m_pFile->DelPcrAll(sPath);
			Sleep(30);
		}
	}
}

void CGvisRTR_PunchDoc::DelShareVsPcrDn()
{
	CString sPath = WorkingInfo.System.sPathVsShareDn;

	while (m_pFile->IsPcrExist(sPath))
	{
		if (m_pFile)
			m_pFile->DelPcrAll(sPath);
		Sleep(30);
	}
}

void CGvisRTR_PunchDoc::DelPcrAll()
{
	DelSharePcrUp();
	DelSharePcrDn();
	DelShareVsPcrUp();
	DelShareVsPcrDn();

	DelPcrUp();
	DelPcrDn();

	pView->m_bIsBuf[0] = FALSE;
	pView->m_bIsBuf[1] = FALSE;

}

void CGvisRTR_PunchDoc::DelPcrUp()
{
	CString sPath;

	if (m_pFile)
	{
		sPath = WorkingInfo.System.sPathVrsShareUp;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVrsBufUp;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsShareUp;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsDummyBufUp;
		m_pFile->DelPcrAll(sPath);
	}
}

void CGvisRTR_PunchDoc::DelPcrDn()
{
	CString sPath;

	if (m_pFile)
	{
		sPath = WorkingInfo.System.sPathVrsShareDn;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVrsBufDn;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsShareDn;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsDummyBufDn;
		m_pFile->DelPcrAll(sPath);
	}
}

CString CGvisRTR_PunchDoc::GetCamPxlRes()
{
	CString sRes = _T("");
	CString sPath;
#ifdef TEST_MODE
	sPath = PATH_PIN_IMG_;
#else
	if (pDoc->WorkingInfo.System.sPathCamSpecDir.Right(1) != "\\")
		sPath.Format(_T("%s\\%s\\%s.mst"), pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerUp);
	else
		sPath.Format(_T("%s%s\\%s.mst"), pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModelUp, pDoc->WorkingInfo.LastJob.sLayerUp);
#endif

	int nPos = sPath.ReverseFind('-');
	if (nPos > 0)
	{
		sRes = sPath.Right(sPath.GetLength() - (nPos + 1));
		nPos = sRes.ReverseFind('.');
		if (nPos > 0)
			sRes = sRes.Left(nPos);
		WorkingInfo.Vision[0].sCamPxlRes = sRes;
		WorkingInfo.Vision[1].sCamPxlRes = sRes;
	}
	return sRes;
}

CString CGvisRTR_PunchDoc::GetProcessNum()
{
	CString sPath = WorkingInfo.System.sPathAoiUpCurrInfo;
	TCHAR szData[200];
	CString sCode = _T("");

	if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Process Unit Code"), NULL, szData, sizeof(szData), sPath))
		sCode = CString(szData);

	return sCode;
}
