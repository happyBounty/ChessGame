#pragma once

#include <vector>
#include <array>
#include <memory>

#include "ChessBlock.h"
#include "ChessPiece.h"
#include "Resource.h"

#include "CChessServer.h"
#include "CChessClient.h"

namespace Chess {

	using namespace std;

	class ChessGame
	{
	private:
		static const int BLOCK_COUNT = 8;
		array<array<unique_ptr<ChessBlock>, BLOCK_COUNT>,BLOCK_COUNT> Board;

		CBitmap bmChessPieces[2][PIECE_COUNT];
		CPoint ptStart;
		CPoint ptSelect;
		int iBlockSize;
		bool bMove;
		bool bEnPassant;
		bool bCheckMove;
		int turn;
		int PlayerTurn;
		unique_ptr<CChessServer> ptServer;
		unique_ptr<CChessClient> ptClinet;
	public:
		CArray<CString> m_fileloadarray;
		CString path_root;
		int index_loadfile = 0;
		CPoint pt_rand_get;
		CPoint pt_current_bock;
		CPoint pt_LButtonUp_block;
		CPoint pt_randgame_current_pos;
		CPoint pt_randgame_in_chessboard_current;
		CPoint pt_first;
		CPoint pt_second;
		CPoint pt_different_point_start;
		CPoint pt_different_point_end;
		CPoint pt_different_team_type;
		CPoint pt_timer_current;
		int is_LButtonup_clicked = 0;
		int current_block_team;
		int current_block_type;
		int current_block_selected = 0;
		int is_Rand_Game = 0;
		int To_start_timer = 0;
		CArray<CPoint**> my_list;
		int is_Load_game = 0;
		int is_end_game = 0;
		int index_chess_count;
		int timer_turn ;
		ChessGame(CPoint sp = { 0,0 });
		virtual ~ChessGame();

		void PaintChessPiece(CPaintDC& dc, CPoint pt);
		void PaintChessPieces(CPaintDC& dc);
		void PaintRandChessPieces(CPaintDC& dc);
		void PaintChessBoard(CPaintDC& dc);
		void PaintCurrentChessPiece(CPaintDC& dc,CPoint point);
		
		void StartGame();
		void ResumeGame(CPoint ** asdf);
		void OnSetBackPaint(CPoint** asdf);
		void RandomGame();
		void EndGame();
		void ChessBoardMessage(CPoint ptCursor);
		void ChessBoardMessageRand(CPoint ptCursor,int is_get_set);
		bool AddChessPiece(CPoint pt,int type,int team);
		void DeleteChessPiece(CPoint pt);

		CPoint GetSelectedPoint();

		ChessBlock* GetChessBlock(CPoint pt);
		void ClearAllMove();
		void ClearAllCheck();

		bool IsCheck(int team);
		bool IsCheckMate(int team);

		void IdentifyEnPassant(CPoint cpTo,CPoint cpFrom);
		bool GetEnPassant();

		bool IsRightPoint(CPoint pt) const;

		bool CreateServer(int Port);
		bool IsServerOpen();
		void CloseServer();

		bool ConnectClient(CString& IPAddress,int Port);
		bool AcceptClient();
		bool IsClientOpen();
		void CloseClient();

		bool SendCommand(CString Command);
		void HandleComand(CString Command);

		bool GetCheckMove();
		void SetCheckMove(bool bValue);

		int GetTurn();
		int GetPlayerTurn();
		CPoint** GetCurrentBlockData();
		void SetCurrentBlockData(int k);
		void OnLoadGameAfterEngine(CPoint ** q);
		int isEqual(CPoint** q, CPoint** qq);
		int Is_Legal_Rand_Chess_Piece(CPoint a);
		int IsKingHere();
		bool OnLButtonClickedFromView(CPoint point);
		void OnLButtonupFromView(CPoint point);
		void PaintChessCurrentRandMouseMove(CPaintDC &dc,CPoint point);
		void PaintTimerCurrentChessPiece(CPaintDC& dc);
		void myfunc(CPoint point);
		void SetMyTurn(int k);
		void GetAllFilesNames(CString path);
	};
}