
//


#pragma once

#include <memory>

#include "ChessGame.h"


class CChildView : public CWnd
{
public:
	CChildView();
public:
	int isRandGame = 0;
	int count_chess_method = 0;
	int click_backward_button = 0;
	int is_LButtonDown = 0;
	int israndgameLbuttondown = 0;
	int is_LButtonDown_rand_in_chessboard = 0;
	CPoint pt_MouseMove;
	int index_timer_count = 0;
	int Load_turn = 1;
	CPoint first_load_team_type;
	
public:
	std::unique_ptr<Chess::ChessGame> Chess;
public:
	CPoint** setMemory();
	
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual ~CChildView();

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRestart();
	afx_msg void OnServerCreate();
	afx_msg void OnCreateClient();
	afx_msg void OnRandom();
	afx_msg void OnRandomStart();
	afx_msg void OnSaveGame();
	afx_msg void OnLoadGame();
	afx_msg void OnBackward();
	afx_msg void OnForward();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void LoadFile(CString path);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
};

