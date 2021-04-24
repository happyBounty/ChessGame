#include "pch.h"
#include "ChessGame.h"
#include "DlgPawnPromotion.h"

#include "MainFrm.h"
#include "ChildView.h"

using namespace Chess;

ChessBlock* Chess::ChessGame::GetChessBlock(CPoint pt)
{
	if (!IsRightPoint(pt))
		return nullptr;
	
	return Board[pt.y][pt.x].get();
}


void Chess::ChessGame::ClearAllMove()
{
	for (int iy = 0; iy < BLOCK_COUNT; iy++) {
		for (int ix = 0; ix < BLOCK_COUNT; ix++) {
			Board[iy][ix]->ClearMove();
		}
	}
}

void Chess::ChessGame::ClearAllCheck()
{
	for (int iy = 0; iy < BLOCK_COUNT; iy++) {
		for (int ix = 0; ix < BLOCK_COUNT; ix++) {
			Board[iy][ix]->SetCheck(false);
		}
	}
}

void Chess::ChessGame::GetAllFilesNames(CString path)
{

	WIN32_FIND_DATA fileData;
	memset(&fileData, 0, sizeof(WIN32_FIND_DATA));
	CString t = CString("*");
	path.AppendChar(t.GetAt(0));
//	AfxMessageBox(path, MB_OK);
	m_fileloadarray.RemoveAll();
	HANDLE handle = FindFirstFile(path, &fileData);
		
	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (_tcscmp(fileData.cFileName, _T(".")) != 0 && // ignore "." and ".."
				_tcscmp(fileData.cFileName, _T("..")) != 0)
			{
				//	AfxMessageBox(fileData.cFileName,MB_OK);
				CString str = fileData.cFileName;
				CString str_mp = CString("kmh");
				CString strstr = CString("");
				for (int i = str.GetLength() - 3; i < str.GetLength(); i++)
					strstr.AppendChar(str.GetAt(i));
				if (strstr == _T("kmh"))
				{
						m_fileloadarray.Add(fileData.cFileName);
					//AfxMessageBox(fileData.cFileName, MB_OK);
				}
			}
		} while (FindNextFile(handle, &fileData));

		FindClose(handle);
	}


	
}
bool Chess::ChessGame::IsCheck(int team)
{
	ClearAllCheck();

	CPoint ptIndex(0, 0);
	for (ptIndex.y = 0; ptIndex.y < BLOCK_COUNT; ptIndex.y++) {
		for (ptIndex.x = 0; ptIndex.x < BLOCK_COUNT; ptIndex.x++) {
			ChessBlock* ptCB = GetChessBlock(ptIndex);

			if (ptCB == nullptr)
				continue;

			if (ptCB->GetChessPieceTeam() == team)
				continue;

			ptCB->MovementChessPiece(true);
		}
	}

	for (ptIndex.y = 0; ptIndex.y < BLOCK_COUNT; ptIndex.y++) {
		for (ptIndex.x = 0; ptIndex.x < BLOCK_COUNT; ptIndex.x++) {
			ChessBlock* ptCB = GetChessBlock(ptIndex);
			if (ptCB == nullptr)
				continue;
			
			if (ptCB->GetChessPieceTeam() != team)
				continue;

			if (ptCB->GetChessPieceType() == PIECE_KING && ptCB->GetCheck())
				return true;
			
		}
	}

	return false;
}

bool Chess::ChessGame::IsCheckMate(int team)
{
	CChildView* ptView = ((CMainFrame*)AfxGetMainWnd())->GetView();

	ClearAllMove();
	CPoint ptIndex(0, 0);
	for (ptIndex.y = 0; ptIndex.y < BLOCK_COUNT; ptIndex.y++) {
		for (ptIndex.x = 0; ptIndex.x < BLOCK_COUNT; ptIndex.x++) {
			ChessBlock* ptCB = GetChessBlock(ptIndex);

			if (ptCB == nullptr)
				continue;

			if (ptCB->GetChessPieceTeam() != team)
				continue;

			ptCB->MovementChessPiece(false);
		}
	}

	for (ptIndex.y = 0; ptIndex.y < BLOCK_COUNT; ptIndex.y++) {
		for (ptIndex.x = 0; ptIndex.x < BLOCK_COUNT; ptIndex.x++) {
			ChessBlock* ptCB = GetChessBlock(ptIndex);
			if (ptCB == nullptr)
				continue;

			if (ptCB->GetMove()) 
				return false;
			
		}
	}
	
	return true;
}


bool Chess::ChessGame::IsRightPoint(CPoint pt) const
{
	if ((pt.x >= 0 && pt.x < BLOCK_COUNT) && (pt.y >= 0 && pt.y < BLOCK_COUNT))
		return true;

	return false;
}

bool Chess::ChessGame::CreateServer(int Port)
{
	ptServer = make_unique<CChessServer>();
	if (ptServer->Create(Port, SOCK_STREAM) == false) {
		CloseServer();
		return true;
	}

	if (ptServer->Listen() == false) {
		CloseServer();
		return true;
	}

	return false;
}

bool Chess::ChessGame::IsServerOpen()
{
	return (bool)ptServer;
}

void Chess::ChessGame::CloseServer()
{
	if ((bool)ptServer) {
		CloseClient();
		ptServer->ShutDown();
		ptServer->Close();
		ptServer.release();
	}
}

bool Chess::ChessGame::ConnectClient(CString& IPAddress, int Port)
{
	ptClinet = make_unique<CChessClient>();

	ptClinet->Create(0, SOCK_STREAM);

	if (ptClinet->Connect(IPAddress.GetString(), Port)) {
		return true;
	}
	else {
		CloseClient();
		return false;
	}
}

bool Chess::ChessGame::AcceptClient()
{
	if ((bool)ptServer == false)
		return true;

	ptClinet = make_unique<CChessClient>();
	return ptServer->Accept(*ptClinet);
}

bool Chess::ChessGame::IsClientOpen()
{
	return (bool)this->ptClinet;
}

void Chess::ChessGame::CloseClient()
{
	if ((bool)this->ptClinet) {
		ptClinet->ShutDown();
		ptClinet->Close();
		ptClinet.release();
	}
}

bool Chess::ChessGame::SendCommand(CString Command)
{
	if(IsClientOpen() == false)
		return false;

	ptClinet->Send(Command.GetBuffer(), Command.GetLength() * 2);
	ptClinet->Send("/", 2);
	return true;
}

void Chess::ChessGame::HandleComand(CString Command)
{
	int curPos = 0;
	CString resToken;
	vector<CString> arguments;

	resToken = Command.Tokenize(_T(" "), curPos);
	while (resToken != _T("")) {
		arguments.emplace_back(resToken);
		resToken = Command.Tokenize(_T(" "), curPos);
	}

	SetCheckMove(true);

	if (arguments[0] == _T("DELETE")) {
		DeleteChessPiece(CPoint(_wtoi(arguments[1]), _wtoi(arguments[2])));
	}
	else if (arguments[0] == _T("ADD")) {
		AddChessPiece(CPoint(_wtoi(arguments[1]), _wtoi(arguments[2])), _wtoi(arguments[3]), _wtoi(arguments[4]));
	}
	else if (arguments[0] == _T("RESTART")) {
		EndGame();
		StartGame();
	}
	else if (arguments[0] == _T("ENDTURN")) {
		turn = !turn;
	}

	SetCheckMove(false);
}

bool Chess::ChessGame::GetCheckMove()
{
	return bCheckMove;
}

void Chess::ChessGame::SetCheckMove(bool bValue)
{
	bCheckMove = bValue;
}

int Chess::ChessGame::GetTurn()
{
	return turn;
}

int Chess::ChessGame::GetPlayerTurn()
{
	return PlayerTurn;
}

ChessGame::ChessGame(CPoint sp) : ptStart(sp), iBlockSize(50), bMove(false), turn(0), bEnPassant(false), bCheckMove(false)
								, PlayerTurn(0)
{
	for (int i = 0; i < 2; i++) 
		for(int i1 = 0;i1 < PIECE_COUNT;i1++)
			this->bmChessPieces[i][i1].LoadBitmapW(IDB_KING_WHITE + (i * 10) + i1);

	CPoint ptIndex(0, 0);
	for (ptIndex.y = 0; ptIndex.y < BLOCK_COUNT; ptIndex.y++) {
		for (ptIndex.x = 0; ptIndex.x < BLOCK_COUNT; ptIndex.x++) {
			Board[ptIndex.y][ptIndex.x] = make_unique<ChessBlock>(this, ptIndex);
		}
	}
}

ChessGame::~ChessGame() {

}
void ChessGame::PaintCurrentChessPiece(CPaintDC& dc,CPoint point)
{
	CBitmap* btOld;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	btOld = (CBitmap*)memDC.SelectObject(&bmChessPieces[current_block_team][current_block_type]);

	dc.TransparentBlt(point.x-25, point.y-25, 50, 50, &memDC, 0, 0, 100, 100, RGB(255, 255, 255));

	memDC.SelectObject(btOld);
//	dc.Rectangle(0, 0, point.x, point.y);
}
void ChessGame::PaintChessBoard(CPaintDC& dc)
{
	CBrush bsBlockColor[5];
	CBrush* bsOld;
	CPoint ptSize = { iBlockSize,iBlockSize };
	bool bBoard = false;
	
	for (int i = 0;i<BLOCK_COUNT;i++)
	{ 
		CString str;
		char buf[100];
		sprintf_s(buf, "%d", 8-i);
		str = CString(buf);
	//	dc.DrawText()
		dc.TextOutW(10, 30+i*50+20, str);
		char buf1[9] = "abcdefgh";
		
		dc.TextOutW(30 + i * 50 + 20, 435,CString(buf1[i]) );
	}
	
	bsBlockColor[0].CreateSolidBrush(RGB(200, 200, 200));
	bsBlockColor[1].CreateSolidBrush(RGB(255, 255, 255));
	bsBlockColor[2].CreateSolidBrush(RGB(0, 255, 0));
	bsBlockColor[3].CreateSolidBrush(RGB(100, 190, 240));
	bsBlockColor[4].CreateSolidBrush(RGB(150, 100, 255));

	bsOld = (CBrush*)dc.SelectObject(&bsBlockColor[0]);

	for (int iy = 0; iy < BLOCK_COUNT; iy++) {
		for (int ix = 0; ix < BLOCK_COUNT; ix++) {
			ChessBlock& rfCB = *GetChessBlock(CPoint(ix, iy));
			if (rfCB.GetMove() == 1 && bMove == true)
				dc.SelectObject(&bsBlockColor[2]);
			else if (rfCB.GetMove() == 2 && bMove == true)
				dc.SelectObject(&bsBlockColor[4]);
			else if(rfCB.GetMove() == 3 && bMove == true)
				dc.SelectObject(&bsBlockColor[3]);
			else if (bBoard)
				dc.SelectObject(&bsBlockColor[0]);
			else
				dc.SelectObject(&bsBlockColor[1]);


			CPoint ptTemp = CPoint(ix * iBlockSize, iy * iBlockSize) + ptStart;

			dc.Rectangle(CRect(ptTemp, ptTemp + ptSize));

			bBoard = !bBoard;
		}
		bBoard = !bBoard;
	}

	dc.SelectObject(*bsOld);
}



void Chess::ChessGame::PaintChessPiece(CPaintDC& dc, CPoint pt) 
{
	CBitmap* btOld;
	CDC memDC;
	ChessBlock& rfCB = *Board[pt.y][pt.x];

	if (rfCB.IsHaveChessPiece() == false)
		return;

	memDC.CreateCompatibleDC(&dc);
	btOld = (CBitmap*)memDC.SelectObject(&bmChessPieces[rfCB.GetChessPieceTeam()][rfCB.GetChessPieceType()]);

	dc.TransparentBlt(ptStart.x + (pt.x * 50), ptStart.y + ( pt.y * 50), 50, 50,&memDC, 0, 0, 100, 100, RGB(255, 255, 255));

	memDC.SelectObject(btOld);
}
CPoint** Chess::ChessGame::GetCurrentBlockData()
{
	CPoint** save_array;
	save_array = new CPoint*[BLOCK_COUNT];
//	save_array_type = new int* [BLOCK_COUNT];
	
	for (int i = 0; i < BLOCK_COUNT; i++)
	{
		
		save_array[i] = new CPoint[BLOCK_COUNT];
	}
	for (int iy = 0; iy < BLOCK_COUNT; iy++) {
		for (int ix = 0; ix < BLOCK_COUNT; ix++) {
			ChessBlock& rfCB = *Board[iy][ix];
			save_array[iy][ix].x = rfCB.GetChessPieceTeam();
			save_array[iy][ix].y = rfCB.GetChessPieceType();
		}
	}
	return save_array;
}

void Chess::ChessGame::SetCurrentBlockData(int k)
{
	EndGame();
	for (int i = 0; i < BLOCK_COUNT; i++) {
		for (int j = 0; j < BLOCK_COUNT; j++) {
			CPoint** asdf = my_list.GetAt(k);
			AddChessPiece(CPoint(j, i), asdf[i][j].y, asdf[i][j].x); 
			//rfCB.SetChessPieceType(data[iy][ix].y);
		}
	}
	
	
}

void Chess::ChessGame::PaintChessPieces(CPaintDC& dc)
{
	for (int iy = 0; iy < BLOCK_COUNT; iy++) {
		for (int ix = 0; ix < BLOCK_COUNT; ix++) {
			PaintChessPiece(dc, CPoint(ix, iy));
		}
	}
}


void Chess::ChessGame::PaintRandChessPieces(CPaintDC& dc)
{
	CBitmap* btOld;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	int CPprocedure[6] = { 0,1,2,3,4,5};
	for(int i =8;i<=9;i++)
		for (int j = 0; j < 6; j++)
		{
			CPoint pt = CPoint(j, i);
		//	ChessBlock& rfCB = *Board[pt.y][pt.x];
			
			btOld = (CBitmap*)memDC.SelectObject(&bmChessPieces[i-8][CPprocedure[j]]);

			dc.TransparentBlt(ptStart.x + (pt.x * 50), ptStart.y + (pt.y * 50)+20, 50, 50, &memDC, 0, 0, 100, 100, RGB(255, 255, 255));

			memDC.SelectObject(btOld);
		}
	
}

void ChessGame::StartGame()
{
	
	my_list.RemoveAll();
	CString str;
	int CPprocedure[BLOCK_COUNT] = { 2,4,3,1,0,3,4,2 };
	for (int i = 0; i < BLOCK_COUNT; i++) {
		AddChessPiece(CPoint(i, 0), CPprocedure[i], 1);
		AddChessPiece(CPoint(i, 7), CPprocedure[i], 0);
		AddChessPiece(CPoint(i, 1), PIECE_PAWN, 1);
		AddChessPiece(CPoint(i, 6), PIECE_PAWN, 0);
	}
	my_list.Add(GetCurrentBlockData());
	turn = TEAM_WHITE;
}
void ChessGame::RandomGame()
{
	EndGame();
}
void ChessGame::OnSetBackPaint(CPoint** asdf)
{
	CString str;
	EndGame();
	int CPprocedure[BLOCK_COUNT] = { 2,4,3,1,0,3,4,2 };
	for (int i = 0; i < BLOCK_COUNT; i++) {
		for (int j = 0; j < BLOCK_COUNT; j++)
		{
			AddChessPiece(CPoint(j, i), asdf[i][j].y, asdf[i][j].x);
		}
	}
}
void ChessGame::ResumeGame(CPoint** asdf)
{
	CString str;
	EndGame();

	int CPprocedure[BLOCK_COUNT] = { 2,4,3,1,0,3,4,2 };
	for (int i = 0; i < BLOCK_COUNT; i++) {
		for (int j = 0; j < BLOCK_COUNT; j++)
		{
			AddChessPiece(CPoint(j, i), asdf[i][j].y, asdf[i][j].x);
		}
	//	AddChessPiece(CPoint(i, 0), CPprocedure[i], 1);
	//	AddChessPiece(CPoint(i, 7), CPprocedure[i], 0);
	//	AddChessPiece(CPoint(i, 1), PIECE_PAWN, 1);
	//	AddChessPiece(CPoint(i, 6), PIECE_PAWN, 0);
	}
//	turn = TEAM_WHITE;
}

void Chess::ChessGame::EndGame()
{
	CPoint cpTmp(0, 0);

	for (cpTmp.y = 0; cpTmp.y < BLOCK_COUNT; cpTmp.y++) {
		for (cpTmp.x = 0; cpTmp.x < BLOCK_COUNT; cpTmp.x++) {
			GetChessBlock(cpTmp)->DeleteChessPiece();
		}
	}
}
int ChessGame::IsKingHere()
{
	CPoint** temp = GetCurrentBlockData();
	int count_white = 0, count_black = 0;
	for(int i=0;i<8;i++)
		for (int j = 0; j < 8; j++)
		{
			if (temp[i][j].x == 0 && temp[i][j].y == 0)
				count_white++;
			if (temp[i][j].x == 1 && temp[i][j].y == 0)
				count_black++;
		}
	if (count_white == 1 && count_black == 1)
		return 1;
	return 0;
}
int ChessGame::Is_Legal_Rand_Chess_Piece(CPoint a)
{
	int i, j;
	CPoint** temp = GetCurrentBlockData();
	int count = 0;
	for(i=0;i<8;i++)
		for (j = 0; j < 8; j++)
		{
			if (temp[i][j].x == a.x && temp[i][j].y == a.y)
				count++;
		}
	switch (a.y)
	{
	case 0:
		if (count >= 1)
			return 1;
		break;
	case 1:
		if (count >= 1)
			return 1;
		break;
	case 2:
		if (count >= 2)
			return 1;
		break;
	case 3:
		if (count >= 2)
			return 1;
		break;
	case 4:
		if (count >= 2)
			return 1;
	case 5:
		if (count >= 8)
			return 1;
		break;
	}
	return 100;
}
void ChessGame::ChessBoardMessageRand(CPoint ptCursor,int is_get_set)
{
	int CPprocedure[6] = { 0,1,2,3,4,5 };
	ptCursor -= ptStart;
	
	if (is_get_set == 1)
	{
		ptCursor.y -= 20;
		ptCursor.x /= iBlockSize;
		ptCursor.y /= iBlockSize;
		ptCursor.y -= 8;
		pt_rand_get = ptCursor;
	}
	else
	{
	/*	if (Is_Legal_Rand_Chess_Piece(CPoint( pt_rand_get.y, CPprocedure[pt_rand_get.x]))==1)
		{
			AfxMessageBox(L"ilegal");
			return;
		}*/
		ptCursor.x /= iBlockSize;
		ptCursor.y /= iBlockSize;
		AddChessPiece(ptCursor, CPprocedure[pt_rand_get.x],pt_rand_get.y);
		
	}
	
}
void ChessGame::PaintChessCurrentRandMouseMove(CPaintDC &dc,CPoint point)
{
	CBitmap* btOld;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	btOld = (CBitmap*)memDC.SelectObject(&bmChessPieces[pt_rand_get.y][pt_rand_get.x]);

	dc.TransparentBlt(point.x - 25, point.y - 25, 50, 50, &memDC, 0, 0, 100, 100, RGB(255, 255, 255));

	memDC.SelectObject(btOld);
}
bool ChessGame::OnLButtonClickedFromView(CPoint ptCursor)
{

	ptCursor -= ptStart;

	ptCursor.x /= iBlockSize;
	ptCursor.y /= iBlockSize;
	pt_first = ptCursor;
	ChessBlock& rfCB = *GetChessBlock(ptCursor);
	if (rfCB.IsHaveChessPiece()==false)
	{
		return false;
	}
	current_block_selected = 1;
	current_block_team = rfCB.GetChessPieceTeam();
	current_block_type = rfCB.GetChessPieceType();
	pt_current_bock = ptCursor;
	DeleteChessPiece(pt_current_bock);
	return true;
}
void ChessGame::OnLButtonupFromView(CPoint ptCursor)
{

	ptCursor -= ptStart;

	ptCursor.x /= iBlockSize;
	ptCursor.y /= iBlockSize;
	pt_second = ptCursor;
	if (ptCursor.x > 7 || ptCursor.y > 7)
	{
		if (is_Rand_Game == 1)
			return;
		pt_LButtonUp_block = pt_current_bock;
		AddChessPiece(pt_current_bock, current_block_type, current_block_team);
		return;
	}
	
	if (current_block_selected == 1)
	{
		SetMyTurn(current_block_team);
		ChessBlock& rfCB = *GetChessBlock(ptCursor);
		if (rfCB.IsHaveChessPiece() == true)
		{
			if (is_Rand_Game == 1)
			{
				DeleteChessPiece(ptCursor);
				AddChessPiece(ptCursor, current_block_type, current_block_team);
				pt_LButtonUp_block = ptCursor; 
				current_block_selected = 0;
				return;
			}
			if (current_block_team != rfCB.GetChessPieceTeam())
			{
				DeleteChessPiece(ptCursor);
				AddChessPiece(ptCursor, current_block_type, current_block_team);
			}
		}
		else
		{
			AddChessPiece(ptCursor, current_block_type, current_block_team);
		}
		
		pt_LButtonUp_block = ptCursor;
		CPoint** point = GetCurrentBlockData();
		current_block_selected = 0;
		if (is_Rand_Game == 1)
		{
			return;
		}
		is_LButtonup_clicked = 1;
		if (is_Load_game == 1)
		{
		
			if (my_list.GetSize() == 1)
			{
				AfxMessageBox(L"Only One chess method here!", MB_OK);
				SetCurrentBlockData(index_chess_count);
				bMove = false;
				return;
			}
			CPoint** qq;
			CPoint** temp = GetCurrentBlockData();
			qq = my_list.GetAt(index_chess_count + 1);
			if (isEqual(qq, temp) == 1)
			{

				CString s = L"Right Move";

				CStatusBar* p =
					(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);


				p->SetPaneText(0, s);
				index_chess_count += 2;
				if (index_chess_count >= my_list.GetSize() )
				{
					SetCurrentBlockData(my_list.GetSize() - 1);
					bMove = false;
				//	AfxMessageBox(L"EndGame! Congratulations", MB_OK);
					is_end_game = 1;
					pt_LButtonUp_block = CPoint(-100, -100);
					pt_current_bock = CPoint(-100, -100);
					return;
				}
			//	SetCurrentBlockData(index_chess_count);
				//--------------------------------------------------------------------
				To_start_timer = 1;
				if (current_block_team == 1)
				{
					SetMyTurn(0);
					timer_turn = 1;
				}
				else
				{
					SetMyTurn(1);
					timer_turn = 0;
				}
				CPoint** pt_1 = my_list.GetAt(index_chess_count - 1);
				CPoint ** pt_2 = my_list.GetAt(index_chess_count);
				for(int i = 0;i<BLOCK_COUNT;i++)
					for (int j = 0; j < BLOCK_COUNT; j++)
					{
						if (pt_1[i][j].x != pt_2[i][j].x && pt_1[i][j].y != pt_2[i][j].y)
						{
							if (pt_2[i][j].x != -1 && pt_2[i][j].y != -1)
							{
								pt_different_point_end = CPoint(i, j);
								pt_different_team_type = CPoint(pt_2[i][j].x, pt_2[i][j].y);
							}
							else if (pt_2[i][j].x == -1 && pt_2[i][j].y == -1)
								pt_different_point_start = CPoint(i, j);
						}
					}

				char temp_message[100];
				sprintf_s(temp_message,"start:%d,%d\nend:%d,%d\nteam_type%d,%d\n ", pt_different_point_start.x, pt_different_point_start.y, pt_different_point_end.x, pt_different_point_end.y, pt_different_team_type.x, pt_different_team_type.y);
			//	AfxMessageBox(CString(temp_message), MB_OK);

				pt_1[pt_different_point_start.x][pt_different_point_start.y] = CPoint(-1, -1);
				ResumeGame(pt_1);
			//	DeleteChessPiece(pt_different_point_start);
			//	AddChessPiece(pt_different_point_start, -1, -1);
				
				//-----------------------------------------------------
				bMove = false;
				return;
			}
			else
			{
				CString s = L"Wrong Move";
				
				CStatusBar* p =
					(CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
				p->SetPaneText(0, s);

				SetCurrentBlockData(index_chess_count);
				bMove = false;
				return;
			}
			return;
		}
		
		if(pt_current_bock.x!= ptCursor.x|| pt_current_bock.y != ptCursor.y)
			my_list.Add(point);
	}
}
void ChessGame::ChessBoardMessage(CPoint ptCursor)
{
	if (is_Load_game == 1)
	{
		turn = TEAM_WHITE;
	}
	if (ptCursor == CPoint(0, 0))
	{
		turn = !turn;
		return;
	}
	ptCursor -= ptStart;

	ptCursor.x /= iBlockSize;
	ptCursor.y /= iBlockSize;

	//----------------------------------
	CPoint** q = GetCurrentBlockData();
	//----------------------------------
	if (ptCursor.x < 0 || ptCursor.x >= BLOCK_COUNT
		|| ptCursor.y < 0 || ptCursor.y >= BLOCK_COUNT)
		return;

	ChessBlock& rfCB = *GetChessBlock(ptCursor);

	if (bMove == false) {
		if (rfCB.IsHaveChessPiece() == false)
			return;

		this->ptSelect = ptCursor;

		if (turn != GetChessBlock(ptSelect)->GetChessPieceTeam())
			return;

		IsCheck(GetChessBlock(ptSelect)->GetChessPieceTeam());

		rfCB.MovementChessPiece(false);

		CPoint cpTmp(0, 0);

		for (cpTmp.y = 0; cpTmp.y < BLOCK_COUNT;cpTmp.y++) {
			for (cpTmp.x = 0; cpTmp.x < BLOCK_COUNT;cpTmp.x++) {
				if (GetChessBlock(cpTmp)->GetMove() == 1) {
					rfCB.SetMove(3);
					bMove = true;
					return;
				}
			}
		}
	}
	else {

		if (ptCursor == ptSelect) {
			ClearAllMove();
			bMove = false;
		}
		if (rfCB.GetMove() == 0)
			return;

		//-----------------------------------------------------------
			
		//-------------------------------------------------------------
		GetChessBlock(ptSelect)->MoveChessPiece(ptCursor);

		ChessBlock* cbTmp = GetChessBlock(ptCursor);

		//pawn promotion
		if (cbTmp->GetChessPieceType() == PIECE_PAWN && (ptCursor.y == 0 || ptCursor.y == 7)) {
			DlgPawnPromotion DlgPromotion;
			int iDlgResult = DlgPromotion.DoModal();
			int tteam = cbTmp->GetChessPieceTeam();

			cbTmp->DeleteChessPiece();
			cbTmp->AddChessPiece(iDlgResult, tteam);
		}

		//king castling
		if (cbTmp->GetChessPieceType() == PIECE_KING && cbTmp->GetMove() == 2) {
			CPoint ptCastling(ptCursor.x == 6? 7 : 0, ptCursor.y);

			GetChessBlock(ptCastling)->MoveChessPiece(ptCastling + CPoint(ptCastling.x == 7 ? -2 : 3, 0));
		}

		IdentifyEnPassant(ptCursor, this->ptSelect);

		if (IsCheckMate(!GetChessBlock(ptCursor)->GetChessPieceTeam()))
			AfxMessageBox(_T("CheckMate!!"));
		else if (IsCheck(!GetChessBlock(ptCursor)->GetChessPieceTeam()))
			AfxMessageBox(_T("Check!!"));

		ClearAllMove();
		if (is_Load_game == 1)
		{
			
			if (my_list.GetSize() == 1)
			{
				AfxMessageBox(L"Only One chess method here!", MB_OK);
				SetCurrentBlockData(index_chess_count);
				bMove = false;
				return;
			}
			CPoint** qq;
			CPoint **temp = GetCurrentBlockData();
			qq = my_list.GetAt(index_chess_count + 1);
//			if(i==8&&j==8)
			
			if(isEqual(qq,temp)==1)
			{
					
				AfxMessageBox(L"Right step!", MB_OK);
				//if (index_chess_count == 0)
				index_chess_count += 2;
				
			/*	if (index_chess_count >= my_list.GetSize())
				{
					AfxMessageBox(L"End Game!congratulation", MB_OK);
					SetCurrentBlockData(my_list.GetSize());
					bMove = false;
					return;
				}*/

				if (index_chess_count >= my_list.GetSize())
				{
					SetCurrentBlockData(my_list.GetSize()-1);
					bMove = false;
					is_end_game = 1;
					
					return;
				}
				SetCurrentBlockData(index_chess_count);
				bMove = false;
				return;
			}
			else
			{
				AfxMessageBox(L"Wrong step!", MB_ICONSTOP);
				SetCurrentBlockData(index_chess_count);
				bMove = false;
				return;
			}
			return;
		}
		SendCommand(_T("ENDTURN"));
		bMove = false;
		turn = !turn;
		q = GetCurrentBlockData();
		my_list.Add(q);
	}
}
int Chess::ChessGame::isEqual(CPoint** q, CPoint** qq)
{
	int i; int j; int k = 0;
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			if (q[i][j].x == qq[i][j].x && q[i][j].y == qq[i][j].y)
				k++;
	if (k == 64)
		return 1;
	else
		return 0;

}
void Chess::ChessGame::OnLoadGameAfterEngine(CPoint ** q)
{


}
void Chess::ChessGame::IdentifyEnPassant(CPoint cpTo, CPoint cpFrom)
{
	if (bEnPassant == true && GetChessBlock(cpTo)->GetMove() == 2) {
		ChessBlock* ptCB = GetChessBlock(CPoint(cpTo.x,cpFrom.y));
		ptCB->DeleteChessPiece();
	}

	bEnPassant = false;

	if (GetChessBlock(cpTo)->GetChessPieceType() != PIECE_PAWN)
		return;
	if (abs(cpTo.y - cpFrom.y) != 2)
		return;

	for (int i = -1; i <= 1; i += 2) {
		ChessBlock* ptCB = GetChessBlock(cpTo + CPoint(i, 0));
		if (ptCB == nullptr)
			continue;
		
		if(ptCB->GetChessPieceTeam() != GetChessBlock(cpTo)->GetChessPieceTeam() &&
			ptCB->GetChessPieceType() == PIECE_PAWN)
			bEnPassant = true;
	}
}

bool Chess::ChessGame::GetEnPassant()
{
	return bEnPassant;
}

bool ChessGame::AddChessPiece(CPoint pt, int type, int team)
{
	Board[pt.y][pt.x]->AddChessPiece(type, team);
	return false;
}

void Chess::ChessGame::DeleteChessPiece(CPoint pt)
{
	Board[pt.y][pt.x]->DeleteChessPiece();
}

CPoint Chess::ChessGame::GetSelectedPoint()
{
	return ptSelect;
}

void Chess::ChessGame::PaintTimerCurrentChessPiece(CPaintDC& dc)
{
	CBitmap* btOld;
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	btOld = (CBitmap*)memDC.SelectObject(&bmChessPieces[pt_different_team_type.x][pt_different_team_type.y]);

	dc.TransparentBlt(pt_timer_current.y, pt_timer_current.x, 50, 50, &memDC, 0, 0, 100, 100, RGB(255, 255, 255));

	memDC.SelectObject(btOld);
}

void Chess::ChessGame::SetMyTurn(int k)
{
	if (k == 1)
		turn = TEAM_WHITE;
	else
		turn = TEAM_BLACK;
}
void Chess::ChessGame::myfunc(CPoint point)
{
	
		
		To_start_timer = 1;
		CPoint** pt_1 = my_list.GetAt(index_chess_count);
		CPoint** pt_2 = my_list.GetAt(index_chess_count+1);
		for (int i = 0; i < BLOCK_COUNT; i++)
			for (int j = 0; j < BLOCK_COUNT; j++)
			{
				if (pt_1[i][j].x != pt_2[i][j].x && pt_1[i][j].y != pt_2[i][j].y)
				{
					if (pt_2[i][j].x != -1 && pt_2[i][j].y != -1)
					{
						pt_different_point_end = CPoint(i, j);
						pt_different_team_type = CPoint(pt_2[i][j].x, pt_2[i][j].y);
					}
					else if (pt_2[i][j].x == -1 && pt_2[i][j].y == -1)
						pt_different_point_start = CPoint(i, j);
				}
			}

		char temp_message[100];
		sprintf_s(temp_message, "start:%d,%d\nend:%d,%d\nteam_type%d,%d\n ", pt_different_point_start.x, pt_different_point_start.y, pt_different_point_end.x, pt_different_point_end.y, pt_different_team_type.x, pt_different_team_type.y);
		//	AfxMessageBox(CString(temp_message), MB_OK);

		pt_1[pt_different_point_start.x][pt_different_point_start.y] = CPoint(-1, -1);
		ResumeGame(pt_1);
		//	DeleteChessPiece(pt_different_point_start);
		//	AddChessPiece(pt_different_point_start, -1, -1);
		index_chess_count++;
			//-----------------------------------------------------
		bMove = false;
		return;
}