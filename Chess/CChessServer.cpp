//

#include "pch.h"
#include "Chess.h"
#include "CChessServer.h"

#include "MainFrm.h"
#include "ChildView.h"

// CChessServer

CChessServer::CChessServer()
{

}

CChessServer::~CChessServer()
{

}



void CChessServer::OnAccept(int nErrorCode)
{
	CChildView* ptView = ((CMainFrame*)AfxGetMainWnd())->GetView();
	
	if (!ptView->Chess->AcceptClient()) {
		ptView->Chess->CloseClient();
		AfxMessageBox(_T("Failed to Connect with Client!"));
	}

	ptView->Chess->SendCommand(_T("RESTART"));
	ptView->Chess->HandleComand(_T("RESTART"));

	ptView->Invalidate();

	AfxMessageBox(_T("New Chellenger!"));
	
	CSocket::OnAccept(nErrorCode);
}
