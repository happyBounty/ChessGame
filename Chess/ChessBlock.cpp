#include "pch.h"
#include "ChessGame.h"
#include "ChessBlock.h"

using namespace Chess;

ChessBlock::ChessBlock(ChessGame* cg, CPoint pt) : cp(nullptr), ptCG(cg), cpPos(pt),Moveable(false),bCheck(false)
{

}

ChessBlock::ChessBlock(ChessBlock& cb) : cp(nullptr), ptCG(cb.ptCG), cpPos(cb.cpPos), Moveable(false), bCheck(false)
{
	if(cb.cp != nullptr)
		cp.reset(cb.cp->CopyChessPiece());
}

int Chess::ChessBlock::GetChessPieceType() const
{
	if (this->cp == false)
		return -1;

	return this->cp->GetType();
}
void Chess::ChessBlock::SetChessPieceType(int type)
{
	this->cp->SetType(type);
}

void Chess::ChessBlock::SetChessPieceTeam(int team)
{
	this->cp->SetTeam(team);
}

int Chess::ChessBlock::GetChessPieceTeam() const
{
	if (cp.get() == nullptr)
		return -1;

	return this->cp->GetTeam();
}


CPoint Chess::ChessBlock::GetPos() const
{
	return cpPos;
}

ChessGame* Chess::ChessBlock::GetGame() const
{
	return this->ptCG;
}

const ChessPiece* Chess::ChessBlock::GetPiece() const
{
	return cp.get();
}

bool Chess::ChessBlock::IsHaveChessPiece() const
{
	if (cp.get() == nullptr)
		return false;
	else
		return true;
}

bool ChessBlock::AddChessPiece(int type, int team)
{
	switch (type) {
	case PIECE_KING:
		this->cp = std::make_unique<King>(this,type,team);
		break;
	case PIECE_QUEEN:
		this->cp = std::make_unique<Queen>(this, type, team);
		break;
	case PIECE_ROOK:
		this->cp = std::make_unique<Rook>(this, type, team);
		break;
	case PIECE_BISHOP:
		this->cp = std::make_unique<Bishop>(this, type, team);
		break;
	case PIECE_KNIGHT:
		this->cp = std::make_unique<Knight>(this, type, team);
		break;
	case PIECE_PAWN:
		this->cp = std::make_unique<Pawn>(this, type, team);
		break;
	default:
		return false;
	}

	if (ptCG->GetCheckMove() == false) {
		CString cmd;
		cmd.Format(_T("ADD %d %d %d %d"), this->cpPos.x, this->cpPos.y,type,team);
		ptCG->SendCommand(cmd);
	}

	return true;
}

void Chess::ChessBlock::DeleteChessPiece()
{
	cp.release();

	if (ptCG->GetCheckMove() == false) {
		CString cmd;
		cmd.Format(_T("DELETE %d %d"), this->cpPos.x, this->cpPos.y);
		ptCG->SendCommand(cmd);
	}
}

bool Chess::ChessBlock::MoveChessPiece(CPoint cpTo, bool bCheck)
{
	if ((bool)this->cp == false)
		return false;

	if(ptCG->IsRightPoint(cpTo) == false)
		return false;

	ChessBlock* ptTo = ptCG->GetChessBlock(cpTo);
	InterfaceMoved* ifMoved = dynamic_cast<InterfaceMoved * >(cp.get());
	bool bMoved = false;

	if (ifMoved != nullptr)
		bMoved = ifMoved->IsMoved();

	ptTo->DeleteChessPiece();
	ptTo->AddChessPiece(GetChessPieceType(), GetChessPieceTeam());
	DeleteChessPiece();

	ifMoved = dynamic_cast<InterfaceMoved*>(ptTo->cp.get());
	if (ifMoved != nullptr)
		ifMoved->SetMove(bMoved);

	if (bCheck == false) {
		InterfaceMoved* ifMoved = nullptr;

		if (ifMoved = dynamic_cast<InterfaceMoved*>(ptTo->cp.get())) {
			ifMoved->SetMove(true);
		}
	}

	return true;
}


void Chess::ChessBlock::SetMove(int value, ChessBlock* ptCB)
{
	int ttype = GetChessPieceType();
	int tteam = GetChessPieceTeam();
	
	if (value == 3 || value == 2) {
		Moveable = value;
		return;
	}

	if (ptCB->GetChessPieceTeam() == GetChessPieceTeam())
		return;

	ptCG->SetCheckMove(true);

	ptCB->MoveChessPiece(cpPos,true);

	if (ptCG->IsCheck(GetChessPieceTeam())) {
		MoveChessPiece(ptCB->cpPos, true);
		ptCG->AddChessPiece(cpPos, ttype, tteam);
		ptCG->SetCheckMove(false);
		return;
	}
	MoveChessPiece(ptCB->cpPos, true);
	ptCG->AddChessPiece(cpPos, ttype, tteam);

	ptCG->SetCheckMove(false);
	Moveable = value;
}

void Chess::ChessBlock::SetCheck(int value, ChessBlock* ptCB)
{
	bCheck = value;
}

int Chess::ChessBlock::GetMove() const
{
	return Moveable;
}

bool Chess::ChessBlock::GetCheck() const
{
	return bCheck;
}

void Chess::ChessBlock::ClearMove()
{
	Moveable = false;
}

void Chess::ChessBlock::SetChessGame(ChessGame* ptCG)
{
	this->ptCG = ptCG;
}

void Chess::ChessBlock::MovementChessPiece(bool bChecking)
{
	if ((bool)cp == false)
		return;

	if (bChecking == false)
		cp->Movement(&ChessBlock::SetMove);
	else
		cp->Movement(&ChessBlock::SetCheck);
}