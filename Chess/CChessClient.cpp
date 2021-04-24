
#include "pch.h"
#include "Chess.h"
#include "CChessClient.h"

#include "MainFrm.h"
#include "ChildView.h"

// CChessClient

CChessClient::CChessClient()
{

}

CChessClient::~CChessClient()
{

}




void CChessClient::OnClose(int nErrorCode)
{
	CChildView* ptView = ((CMainFrame*)AfxGetMainWnd())->GetView();

	ptView->Chess->CloseClient();

	ptView->Invalidate();

	CSocket::OnClose(nErrorCode);
}


void CChessClient::OnReceive(int nErrorCode)
{

	TCHAR szReceived[1024] = _T("");
	CString Commands;
	CChildView* ptView = ((CMainFrame*)AfxGetMainWnd())->GetView();

	if (Receive(szReceived, sizeof(szReceived)) > 0) {
		Commands = szReceived;
		int curPos = 0;
		CString cmd;

		cmd = Commands.Tokenize(_T("/"), curPos);
		while (cmd != _T("")) {
			ptView->Chess->HandleComand(cmd);
			cmd = Commands.Tokenize(_T("/"), curPos);
		}
	}

	ptView->Invalidate();

	CSocket::OnReceive(nErrorCode);
}
