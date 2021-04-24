#pragma once



class DlgCreateClient : public CDialog
{
	DECLARE_DYNAMIC(DlgCreateClient)

public:
	DlgCreateClient(CWnd* pParent = nullptr);   
	virtual ~DlgCreateClient();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATECLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString ctrl_Port;
	CIPAddressCtrl ctrl_IPAddress;
	afx_msg void OnBnClickedOk();
};
