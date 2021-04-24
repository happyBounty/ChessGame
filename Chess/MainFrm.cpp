
//

#include "pch.h"
#include "framework.h"
#include "Chess.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,          
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CMainFrame::CMainFrame() noexcept
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndView.Create(nullptr, nullptr, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, nullptr))
	{
		TRACE0("Don't make view.\n");
		return -1;
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Don't make status bar.\n");
		return -1;     
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	m_wndMenu.LoadMenuW(IDR_MENU);
	this->SetMenu(&m_wndMenu);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.hMenu = NULL;

	return TRUE;
}


#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG



void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	CPoint WindowSizePoint(650, 650);
	lpMMI->ptMaxTrackSize = WindowSizePoint;
	lpMMI->ptMinTrackSize = WindowSizePoint;
	lpMMI->ptMaxSize = WindowSizePoint;
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

CChildView* CMainFrame::GetView()
{
	return &m_wndView;
}
