#pragma once

#include <memory>

#include "ChessPiece.h"

namespace Chess {

	class ChessBlock
	{
	private:
		ChessGame* ptCG;
		const CPoint cpPos;
		int Moveable;
		bool bCheck;
		std::unique_ptr<ChessPiece> cp;
	public:
		ChessBlock(ChessGame* cg, CPoint pt);
		ChessBlock(ChessBlock& cb);
		int GetChessPieceType() const;
		int GetChessPieceTeam() const;
		void SetChessPieceType(int type);
		void SetChessPieceTeam(int team);
		ChessGame* GetGame() const;
		const ChessPiece* GetPiece() const;
		CPoint GetPos() const;

		bool IsHaveChessPiece() const;

		bool AddChessPiece(int type,int team);
		void DeleteChessPiece();
		bool MoveChessPiece(CPoint cpTo,bool bCheck = false);

		void SetMove(int value, ChessBlock* ptCB = nullptr);
		void SetCheck(int value, ChessBlock* ptCB = nullptr);
		int GetMove() const;
		bool GetCheck() const;

		void ClearMove();

		void SetChessGame(ChessGame* ptCG);

		void MovementChessPiece(bool bChecking);
	};

}