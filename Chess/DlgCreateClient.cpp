//

#include "pch.h"
#include "Chess.h"
#include "DlgCreateClient.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ChildView.h"


IMPLEMENT_DYNAMIC(DlgCreateClient, CDialog)

DlgCreateClient::DlgCreateClient(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CREATECLIENT, pParent)
	, ctrl_Port(_T(""))
{

}

DlgCreateClient::~DlgCreateClient()
{
}

void DlgCreateClient::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, ctrl_Port);
	DDX_Control(pDX, IDC_IPADDRESS1, ctrl_IPAddress);
}


BEGIN_MESSAGE_MAP(DlgCreateClient, CDialog)
	ON_BN_CLICKED(IDOK, &DlgCreateClient::OnBnClickedOk)
END_MESSAGE_MAP()




BOOL DlgCreateClient::OnInitDialog()
{
	CDialog::OnInitDialog();


	ctrl_IPAddress.SetAddress(127, 0, 0, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void DlgCreateClient::OnBnClickedOk()
{
	//CDialog::OnOK();
	CChildView* ptView = ((CMainFrame*)AfxGetMainWnd())->GetView();

	BYTE fields[4];
	CString IPAddress;

	UpdateData();

	ctrl_IPAddress.GetAddress(fields[0], fields[1], fields[2], fields[3]);

	IPAddress.Format(_T("%d.%d.%d.%d"), fields[0], fields[1], fields[2], fields[3]);

	EndDialog(ptView->Chess->ConnectClient(IPAddress, _wtoi(ctrl_Port)));
}
