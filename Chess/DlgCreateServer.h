#pragma once



class DlgCreateServer : public CDialog
{
	DECLARE_DYNAMIC(DlgCreateServer)

public:
	DlgCreateServer(CWnd* pParent = nullptr);  
	virtual ~DlgCreateServer();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATESERVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
//	CIPAddressCtrl ctrlIPAddress;
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CString ctrl_Port;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
