#pragma once



class DlgPawnPromotion : public CDialog
{
	DECLARE_DYNAMIC(DlgPawnPromotion)

public:
	DlgPawnPromotion(CWnd* pParent = nullptr);   
	virtual ~DlgPawnPromotion();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAWN_PROMOTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedQueen();
	afx_msg void OnBnClickedRook();
	afx_msg void OnBnClickedBishop();
	afx_msg void OnBnClickedKnight();
};
