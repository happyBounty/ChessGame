#include "pch.h"
#include "ChessPiece.h"
#include "ChessGame.h"

using namespace Chess;

CPoint ChessPiece::Foward(CPoint pt, int team)
{
	CPoint cpTemp = GetBlock()->GetPos();

	cpTemp.x += pt.x;
	if (team == TEAM_WHITE)
		cpTemp.y -= pt.y;
	else
		cpTemp.y += pt.y;

	return cpTemp;
}

ChessPiece::ChessPiece(ChessBlock* ptCB, int type,int team) : type(type),team(team),ptCB(ptCB)
{

}

int Chess::ChessPiece::GetType() const
{
	if (this == nullptr)
		return -1;

	return this->type;
}

int Chess::ChessPiece::GetTeam() const
{
	if (this == nullptr)
		return -1;

	return this->team;
}
void Chess::ChessPiece::SetTeam(int te)
{
	if (this == nullptr)
		return ;
	this->team = te;
}

void Chess::ChessPiece::SetType(int ty)
{
//	this->type = ty;
}


ChessBlock* Chess::ChessPiece::GetBlock()
{
	return this->ptCB;
}

void Chess::ChessPiece::Movement( SetFunc Func)
{

}

ChessPiece* Chess::ChessPiece::CopyChessPiece()
{
	return nullptr;
}


King::King(ChessBlock* ptCB, int type, int team) : ChessPiece(ptCB,type,team),bMove(false)
{
}

bool Chess::King::IsMoved() const
{
	return bMove;
}

void Chess::King::SetMove(bool bMove)
{
	this->bMove = bMove;
}

King Chess::King::operator=(const King& kp)
{
	this->bMove = kp.bMove;
	return *this;
}

void Chess::King::Movement(SetFunc Func)
{
	int tteam = ChessPiece::GetTeam();
	int tvalue = ChessPiece::GetType();

	ChessGame& cg = *GetBlock()->GetGame();
	CPoint ptPos = GetBlock()->GetPos();

	CPoint ptMove[8] = {
		{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}
	};

	ChessBlock* ptTmp = nullptr;

	for (int i = 0; i < 8; i++) {
		if (ptTmp = cg.GetChessBlock(ptPos + ptMove[i]))
			(ptTmp->*Func)(1,GetBlock());
	}

	if (IsMoved())
		return;

	//i = 0 : KingSide i = 1 : QueenSide
	for (int i = 0; i <= 1; i++) {
		
		const Rook* rkTmp = dynamic_cast<const Rook*>(cg.GetChessBlock(CPoint(i ? 0 : 7, tteam ? 0 : 7))->GetPiece());

		if (rkTmp == nullptr || rkTmp->IsMoved())
			continue;

		for (int i1 = 1; i1 < 3; i1++) {
			ChessBlock* cbCastling = cg.GetChessBlock(Foward(CPoint(i ? -i1 : i1, 0), tteam));
			const ChessPiece* cpTmp = dynamic_cast<const ChessPiece*>(cbCastling->GetPiece());

			if (cpTmp != nullptr)
				break;

			if (cbCastling->GetCheck())
				break;

			if (i1 == 2) {
				(cbCastling->*Func)(2, GetBlock());
				break;
			}
		}
	}
}

ChessPiece* Chess::King::CopyChessPiece()
{
	return new King(GetBlock(), ChessPiece::GetType(), ChessPiece::GetTeam());
}


Queen::Queen(ChessBlock* ptCB, int type, int team) : ChessPiece(ptCB, type,team)
{

}

void Chess::Queen::Movement(SetFunc Func)
{
	int tteam = ChessPiece::GetTeam();
	int tvalue = ChessPiece::GetType();

	ChessGame& cg = *GetBlock()->GetGame();
	CPoint ptPos = GetBlock()->GetPos();

	CPoint ptMove[8] = {
		{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}
	};

	ChessBlock* ptTmp = nullptr;

	for (int i = 0; i < 8; i++) {
		for (CPoint ptIndex = ptPos + ptMove[i]; cg.IsRightPoint(ptIndex); ptIndex += ptMove[i]) {
			if (ptTmp = cg.GetChessBlock(ptIndex)) {
				(ptTmp->*Func)(1,GetBlock());

				if (ptTmp->IsHaveChessPiece() == true)
					break;
			}
		}
	}
}

ChessPiece* Chess::Queen::CopyChessPiece()
{
	return new Queen(GetBlock(), ChessPiece::GetType(), ChessPiece::GetTeam());
}

Rook::Rook(ChessBlock* ptCB, int type, int team) : ChessPiece(ptCB, type, team), bMove(false)
{

}

bool Chess::Rook::IsMoved() const
{
	return bMove;
}

void Chess::Rook::SetMove(bool bMove)
{
	this->bMove = bMove;
}

void Chess::Rook::Movement( SetFunc Func)
{
	int tteam = ChessPiece::GetTeam();
	int tvalue = ChessPiece::GetType();

	ChessGame& cg = *GetBlock()->GetGame();
	CPoint ptPos = GetBlock()->GetPos();


	CPoint ptMove[4] = {
		{1,0},{-1,0},{0,1},{0,-1}
	};

	ChessBlock* ptTmp = nullptr;

	for (int i = 0; i < 4; i++) {
		for (CPoint ptIndex = ptPos + ptMove[i]; cg.IsRightPoint(ptIndex); ptIndex += ptMove[i]) {
			if (ptTmp = cg.GetChessBlock(ptIndex)) {
				(ptTmp->*Func)(1, GetBlock());

				if (ptTmp->IsHaveChessPiece() == true)
					break;
			}
		}
	}
}

ChessPiece* Chess::Rook::CopyChessPiece()
{
	return new Rook(GetBlock(), ChessPiece::GetType(), ChessPiece::GetTeam());
}

Bishop::Bishop(ChessBlock* ptCB, int type, int team) : ChessPiece(ptCB, type,team)
{

}

void Chess::Bishop::Movement( SetFunc Func)
{
	int tteam = ChessPiece::GetTeam();
	int tvalue = ChessPiece::GetType();

	ChessGame& cg = *GetBlock()->GetGame();
	CPoint ptPos = GetBlock()->GetPos();


	CPoint ptMove[4] = { 
		{1,1},{1,-1},{-1,1},{-1,-1}
	};

	ChessBlock* ptTmp = nullptr;

	for (int i = 0; i < 4; i++) {
		for (CPoint ptIndex = ptPos + ptMove[i]; cg.IsRightPoint(ptIndex); ptIndex += ptMove[i]) {
			if (ptTmp = cg.GetChessBlock(ptIndex)) {
				(ptTmp->*Func)(1, GetBlock());

				if (ptTmp->IsHaveChessPiece() == true)
					break;
			}
		}
	}
}

ChessPiece* Chess::Bishop::CopyChessPiece()
{
	return new Bishop(GetBlock(), ChessPiece::GetType(), ChessPiece::GetTeam());
}


Knight::Knight(ChessBlock* ptCB, int type, int team) : ChessPiece(ptCB, type,team)
{

}

void Chess::Knight::Movement(SetFunc Func)
{
	int tteam = ChessPiece::GetTeam();
	int tvalue = ChessPiece::GetType();

	ChessGame& cg = *GetBlock()->GetGame();
	CPoint ptPos = GetBlock()->GetPos();


	CPoint ptMove[8]= {
		{2,1},{-2,1},{2,-1},{-2,-1},
		{1,2},{-1,2},{1,-2},{-1,-2}
	};

	ChessBlock* ptTmp = nullptr;

	for (int i = 0; i < 8; i++) {
		if(ptTmp = cg.GetChessBlock(ptPos + ptMove[i]))
			(ptTmp->*Func)( 1, GetBlock());
	}
}

ChessPiece* Chess::Knight::CopyChessPiece()
{
	return new Knight(GetBlock(), ChessPiece::GetType(), ChessPiece::GetTeam());
}

Pawn::Pawn(ChessBlock* ptCB, int type, int team) : ChessPiece(ptCB, type,team)
{
}

void Chess::Pawn::Movement( SetFunc Func)
{
	int tteam = ChessPiece::GetTeam();
	int tvalue = ChessPiece::GetType();

	ChessGame* cg = GetBlock()->GetGame();
	CPoint ptPos = GetBlock()->GetPos();

	ChessBlock* ptTmp = nullptr;
	
	if (cg->GetEnPassant()) {
		for (int i = -1; i <= 1; i += 2) {
			if (ptTmp = cg->GetChessBlock(Foward(CPoint(i, 0), tteam))) {

				if (ptTmp->GetChessPieceTeam() != tteam && ptTmp->GetChessPieceType() == PIECE_PAWN) {
					(cg->GetChessBlock(Foward(CPoint(i, 1), tteam))->*Func)(2, GetBlock());
				}
			}
		}
	}

	if (ptTmp = cg->GetChessBlock(Foward(CPoint(0, 1), tteam))) {
		if (ptTmp->IsHaveChessPiece() == false) {
			(ptTmp->*Func)(1, GetBlock());

			if (ptTmp = cg->GetChessBlock(Foward(CPoint(0, 2), tteam))) {

				if (ptTmp != nullptr && (ptPos.y == 1 || ptPos.y == 6) && ptTmp->IsHaveChessPiece() == false)
					(ptTmp->*Func)(1, GetBlock());
			}
		}
	}

	for (int i = -1; i <= 1; i += 2) {
		if (ptTmp = cg->GetChessBlock(Foward(CPoint(i, 1), tteam))) {

			if (ptTmp->IsHaveChessPiece() == true) {
				(ptTmp->*Func)(1, GetBlock());
			}
		}
	}
}

ChessPiece* Chess::Pawn::CopyChessPiece()
{
	return new Pawn(GetBlock(), ChessPiece::GetType(),ChessPiece::GetTeam());
}