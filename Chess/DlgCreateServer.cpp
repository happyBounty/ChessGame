//

#include "pch.h"
#include "Chess.h"
#include "DlgCreateServer.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ChildView.h"


IMPLEMENT_DYNAMIC(DlgCreateServer, CDialog)

DlgCreateServer::DlgCreateServer(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CREATESERVER, pParent)
	, ctrl_Port(_T(""))
{

}

DlgCreateServer::~DlgCreateServer()
{
}

void DlgCreateServer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ctrl_Port);
}


BEGIN_MESSAGE_MAP(DlgCreateServer, CDialog)
//	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &DlgCreateServer::OnBnClickedOk)
END_MESSAGE_MAP()





void DlgCreateServer::OnBnClickedOk()
{
	//CDialog::OnOK();
	CChildView* ptView = ((CMainFrame*)AfxGetMainWnd())->GetView();
	
	UpdateData();

	EndDialog(ptView->Chess->CreateServer(_wtoi(ctrl_Port)));
}


BOOL DlgCreateServer::OnInitDialog()
{
	CDialog::OnInitDialog();



	return TRUE;  // return TRUE unless you set the focus to a control
}
