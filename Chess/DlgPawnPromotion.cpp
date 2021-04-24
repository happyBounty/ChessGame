
#include "pch.h"
#include "Chess.h"
#include "DlgPawnPromotion.h"
#include "afxdialogex.h"

#include "ChessPiece.h"



IMPLEMENT_DYNAMIC(DlgPawnPromotion, CDialog)

DlgPawnPromotion::DlgPawnPromotion(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PAWN_PROMOTION, pParent)
{

}

DlgPawnPromotion::~DlgPawnPromotion()
{
}

void DlgPawnPromotion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgPawnPromotion, CDialog)
	ON_BN_CLICKED(IDC_QUEEN, &DlgPawnPromotion::OnBnClickedQueen)
	ON_BN_CLICKED(IDC_ROOK, &DlgPawnPromotion::OnBnClickedRook)
	ON_BN_CLICKED(IDC_BISHOP, &DlgPawnPromotion::OnBnClickedBishop)
	ON_BN_CLICKED(IDC_KNIGHT, &DlgPawnPromotion::OnBnClickedKnight)
END_MESSAGE_MAP()




void DlgPawnPromotion::OnBnClickedQueen()
{
	EndDialog(Chess::PIECE_QUEEN);
}


void DlgPawnPromotion::OnBnClickedRook()
{
	EndDialog(Chess::PIECE_ROOK);
}


void DlgPawnPromotion::OnBnClickedBishop()
{
	EndDialog(Chess::PIECE_BISHOP);
}


void DlgPawnPromotion::OnBnClickedKnight()
{
	EndDialog(Chess::PIECE_KNIGHT);
}
