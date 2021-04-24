
#include "pch.h"
#include "framework.h"
#include "Chess.h"
#include "ChildView.h"

#include "DlgCreateServer.h"
#include "DlgCreateClient.h"

#include "MainFrm.h"
#include "ChessPiece.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_RESTART, &CChildView::OnRestart)
	ON_COMMAND(ID_RANDOM, &CChildView::OnRandom)
	ON_COMMAND(ID_RANDOM_START, &CChildView::OnRandomStart)
	ON_COMMAND(ID_BACKWARD, &CChildView::OnBackward)
	ON_COMMAND(ID_FORWARD, &CChildView::OnForward)
	ON_COMMAND(ID_SAVEGAME, &CChildView::OnSaveGame)
	ON_COMMAND(ID_LOADGAME, &CChildView::OnLoadGame)
	ON_COMMAND(ID_SERVER_MAKE, &CChildView::OnServerCreate)
	ON_COMMAND(ID_SERVER_CONNECT, &CChildView::OnCreateClient)

	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_RBUTTONDBLCLK()
END_MESSAGE_MAP()


BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{

	CPaintDC dc(this); 


	if (this->Chess->is_Load_game == 1)
	{
		CString m_CurrentFileName = this->Chess->m_fileloadarray.GetAt(this->Chess->index_loadfile);
		dc.TextOutW(450, 60, _T("Puzzle:")+m_CurrentFileName);
	}
	if (this->Chess->To_start_timer == 1)
	{
		this->Chess->PaintChessBoard(dc);
		this->Chess->PaintChessPieces(dc);
		this->Chess->PaintTimerCurrentChessPiece(dc);
		CString TurnMsg = _T("Turn : ");


		if (Chess->GetTurn() == Chess::TEAM_WHITE) {
			TurnMsg += _T("White");
		}
		else {
			TurnMsg += _T("Black");
		}
		dc.TextOutW(450, 30, TurnMsg);
		return;
	}

	if (isRandGame == 1)
	{

		this->Chess->PaintChessBoard(dc);
		if (is_LButtonDown_rand_in_chessboard == 1)
		{
			this->Chess->PaintChessPieces(dc);
			this->Chess->PaintCurrentChessPiece(dc, this->Chess->pt_randgame_in_chessboard_current);
			this->Chess->PaintRandChessPieces(dc);
			return;
		}
		if (israndgameLbuttondown == 1)
		{
			CPen pen;
			pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
			CPen* old = (CPen*)dc.SelectObject(pen);
			dc.Rectangle(30 + this->Chess->pt_rand_get.x * 50, 450 + this->Chess->pt_rand_get.y * 50, 30 + (this->Chess->pt_rand_get.x+1) * 50, 450 + (this->Chess->pt_rand_get.y+1) * 50);
			dc.SelectObject(&old);
			this->Chess->PaintChessCurrentRandMouseMove(dc, this->Chess->pt_randgame_current_pos);
			
		}
		this->Chess->PaintRandChessPieces(dc);
		this->Chess->PaintChessPieces(dc);
		CString TurnMsg = _T("Turn : ");


		if (Chess->GetTurn() == Chess::TEAM_WHITE) {
			TurnMsg += _T("White");
		}
		else {
			TurnMsg += _T("Black");
		}
		dc.TextOutW(450, 30, TurnMsg);
		return;
	}

	if (is_LButtonDown == 1)
	{
		this->Chess->PaintChessBoard(dc);
		
		
		CPen pen,pen1;
		pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		CPen *old = (CPen *)dc.SelectObject(pen);
		CBrush bsBlockColor[2];
		CBrush* oldb1;
		CBrush* oldb2;
		bsBlockColor[0].CreateSolidBrush(RGB(200, 200, 200));
		bsBlockColor[1].CreateSolidBrush(RGB(255, 255, 255));
		if ((this->Chess->pt_first.x + this->Chess->pt_first.y) % 2 == 1)
			oldb1 = (CBrush*)dc.SelectObject(bsBlockColor[0]);
		else
			oldb1 = (CBrush*)dc.SelectObject(bsBlockColor[1]);
		dc.Rectangle(30 + this->Chess->pt_current_bock.x * 50, 30 + (this->Chess->pt_current_bock.y) * 50, 30 + (this->Chess->pt_current_bock.x + 1) * 50, 30 + (this->Chess->pt_current_bock.y + 1) * 50); 
		dc.SelectObject(&old);
		dc.SelectObject(&oldb1);

		if((this->Chess->pt_second.x+this->Chess->pt_second.y) % 2 ==1)
			oldb2 = (CBrush*)dc.SelectObject(bsBlockColor[0]);
		else
			oldb2 = (CBrush*)dc.SelectObject(bsBlockColor[1]);
		pen1.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
		CPen* old1 = (CPen*)dc.SelectObject(pen1);
		dc.Rectangle(30 + this->Chess->pt_LButtonUp_block.x * 50, 30 + (this->Chess->pt_LButtonUp_block.y) * 50, 30 + (this->Chess->pt_LButtonUp_block.x + 1) * 50, 30 + (this->Chess->pt_LButtonUp_block.y + 1) * 50);
		dc.SelectObject(&old1);
		dc.SelectObject(&oldb2);
		this->Chess->PaintChessPieces(dc);
		this->Chess->PaintCurrentChessPiece(dc,pt_MouseMove);
		CString TurnMsg = _T("Turn : ");


		if (Chess->GetTurn() == Chess::TEAM_WHITE) {
			TurnMsg += _T("White");
		}
		else {
			TurnMsg += _T("Black");
		}
		dc.TextOutW(450, 30, TurnMsg);
		return;
	}
	if (this->Chess->is_LButtonup_clicked == 1)
	{
		this->Chess->PaintChessBoard(dc);
		this->Chess->PaintChessPieces(dc);
		CPen pen, pen1;
		pen.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		CPen* old = (CPen*)dc.SelectObject(pen);
		CBrush bsBlockColor[2];
		CBrush* oldb1;
		CBrush* oldb2;
		bsBlockColor[0].CreateSolidBrush(RGB(200, 200, 200));
		bsBlockColor[1].CreateSolidBrush(RGB(255, 255, 255));
		if ((this->Chess->pt_first.x + this->Chess->pt_first.y) % 2 == 1)
			oldb1 = (CBrush*)dc.SelectObject(bsBlockColor[0]);
		else
			oldb1 = (CBrush*)dc.SelectObject(bsBlockColor[1]);
		dc.Rectangle(30 + this->Chess->pt_current_bock.x * 50, 30 + (this->Chess->pt_current_bock.y) * 50, 30 + (this->Chess->pt_current_bock.x + 1) * 50, 30 + (this->Chess->pt_current_bock.y + 1) * 50);
		dc.SelectObject(&old);
		dc.SelectObject(&oldb1);

		if ((this->Chess->pt_second.x + this->Chess->pt_second.y) % 2 == 1)
			oldb2 = (CBrush*)dc.SelectObject(bsBlockColor[0]);
		else
			oldb2 = (CBrush*)dc.SelectObject(bsBlockColor[1]);
		pen1.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
		CPen* old1 = (CPen*)dc.SelectObject(pen1);
		dc.Rectangle(30 + this->Chess->pt_LButtonUp_block.x * 50, 30 + (this->Chess->pt_LButtonUp_block.y) * 50, 30 + (this->Chess->pt_LButtonUp_block.x + 1) * 50, 30 + (this->Chess->pt_LButtonUp_block.y + 1) * 50);
		dc.SelectObject(&old1);
		dc.SelectObject(&oldb2);
	//	this->Chess->PaintCurrentChessPiece(dc, pt_MouseMove);
		this->Chess->PaintChessPieces(dc);
		CString TurnMsg = _T("Turn : ");
		if (this->Chess->is_end_game == 1)
		{
			//	AfxMessageBox(L"This puzzle is finished! Loading NextFile", MB_OK);
			this->Chess->pt_LButtonUp_block = CPoint(-100, -100);
			this->Chess->pt_current_bock = CPoint(-100, -100);
			if (this->Chess->index_loadfile == this->Chess->m_fileloadarray.GetSize() - 1)
			{
				AfxMessageBox(L"End Game!congratulation", MB_OK);
				return;
			}
			this->Chess->PaintChessBoard(dc);
			this->Chess->PaintChessPieces(dc);
			this->Chess->index_loadfile++;
			LoadFile(this->Chess->path_root);
			return;
		}

		if (Chess->GetTurn() == Chess::TEAM_WHITE) {
			TurnMsg += _T("White");
		}
		else {
			TurnMsg += _T("Black");
		}
		dc.TextOutW(450, 30, TurnMsg);
		
		return;
	}


	this->Chess->PaintChessBoard(dc);
	this->Chess->PaintChessPieces(dc);
	

	CString TurnMsg = _T("Turn : ");


	if (Chess->GetTurn() == Chess::TEAM_WHITE) {
		TurnMsg += _T("White");
	}
	else {
		TurnMsg += _T("Black");
	}
	dc.TextOutW(450, 30, TurnMsg);
	/*if (Chess->IsClientOpen()) {
		dc.TextOutW(450, 10, _T("Playing with other player."));
	}
	else if (Chess->IsServerOpen()) {
		dc.TextOutW(450, 10, _T("Waiting other player...."));
	}*/
}



void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	
	CWnd::OnLButtonDown(nFlags, point);
	this->Chess->is_LButtonup_clicked = 0;
	pt_MouseMove = point;
	this->Chess->pt_LButtonUp_block = CPoint(-100, -100);
	if (isRandGame == 1)
	{
		if (point.x >= 30 && point.x <= 330 && point.y >= 450 && point.y <= 550)
		{
			israndgameLbuttondown = 1;
			this->Chess->ChessBoardMessageRand(point, 1);
		}
		else if (point.x >= 30 && point.x <= 430 && point.y <= 430 && point.y >= 30)
		{

			if (this->Chess->OnLButtonClickedFromView(point) == true)
			{
				israndgameLbuttondown = 0;
				is_LButtonDown_rand_in_chessboard = 1;
				return;
			}
		}
		return;
	}
	if (point.x > 430 || point.y > 430)
		return;
	if (this->Chess->is_Load_game == 1)
	{
		if (this->Chess->is_end_game == 1)
		{
			AfxMessageBox(L"The Game is already finished", MB_OK);
			return;
		}
		if (this->Chess->OnLButtonClickedFromView(point) == true)
		{
			is_LButtonDown = 1;
			return;
		}
		return;
	}
	
	if(this->Chess->OnLButtonClickedFromView(point)==true)
		is_LButtonDown = 1;
	this->Invalidate();
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	this->Chess = std::make_unique<Chess::ChessGame>(CPoint(30, 30));
//	this->Chess->StartGame();
	OnRandom();
	return 0;
}


void CChildView::OnDestroy()
{
	CWnd::OnDestroy();
	Chess->CloseServer();
}


void CChildView::OnRestart()
{
	this->Chess->is_Load_game = 0;
	isRandGame = 0;
	this->Chess->is_end_game = 0;
	this->Chess->pt_LButtonUp_block = CPoint(-100, -100);
	this->Chess->pt_current_bock = CPoint(-100, -100);
	this->Chess->EndGame();
	this->Chess->StartGame();
	Chess->SendCommand(_T("RESTART"));
	pt_MouseMove = CPoint(-100, -100);
	Invalidate();
}

void CChildView::OnRandom()
{
	this->Chess->is_Load_game = 0;
	this->Chess->is_Rand_Game = 1;
	
	isRandGame = 1;
	this->Chess->is_end_game = 0;
	this->Chess->EndGame();
	this->Chess->RandomGame();
	Invalidate();
}

void CChildView::OnRandomStart()
{
/*	if (this->Chess->IsKingHere() != 1)
	{
		AfxMessageBox(L"No King", MB_OK);
		return;
	}*/
	this->Chess->is_Rand_Game = 0;
	this->Chess->is_Load_game = 0;
	this->Chess->is_end_game = 0;
	isRandGame = 0;
	israndgameLbuttondown = 0;
	is_LButtonDown_rand_in_chessboard = 0;
	this->Chess->my_list.RemoveAll();
	this->Chess->my_list.Add(this->Chess->GetCurrentBlockData());
	Invalidate();
}
void CChildView::OnBackward()
{
	if (this->Chess->is_Load_game == 1)
	{
		AfxMessageBox(L"Can't backward when playing loadgame", MB_OK);
		return;
	}
	count_chess_method = this->Chess->my_list.GetSize();
	if (count_chess_method == 1)
	{
		click_backward_button = 0;
		AfxMessageBox(L"Can't backward", MB_OK);
		return;
	}
	else
	{
		this->Chess->my_list.RemoveAt(count_chess_method-1);
		this->Chess->OnSetBackPaint(this->Chess->my_list.GetAt(count_chess_method-2));
	}
	Invalidate();
}
void CChildView::OnForward()
{
	
}
void CChildView::OnSaveGame()
{
	this->Chess->is_LButtonup_clicked = 0;
	this->Chess->is_Rand_Game = 0;
	isRandGame = 0;
	israndgameLbuttondown = 0;
	char strFilter[] = { "All Files (*.kmh)|*.kmh|" };
	CString agendaName, agendaPath;
	CFileDialog FileDlg(FALSE, CString(""), NULL,0,  CString(strFilter));
	CFile file;
	
	if (FileDlg.DoModal() == IDOK) // this is the line which gives the errors
	{
		agendaName = FileDlg.GetFileName();
		agendaPath = FileDlg.GetPathName();
		//	AfxMessageBox(FileDlg.GetPathName() , MB_OK);
	}

	if (file.Open((agendaPath), CFile::modeCreate | CFile::modeWrite))
	{
			int mlist_size = this->Chess->my_list.GetSize();
			CString str;
			for (int index_array = 0; index_array < mlist_size; index_array++)
			{
				CPoint** asdf = this->Chess->my_list.GetAt(index_array);
				for (int i = 0; i < 8; i++)
					for (int j = 0; j < 8; j++)
					{
						char buf[100];
						sprintf_s(buf, "%d\n%d\n", asdf[i][j].x, asdf[i][j].y);
						file.Write(&buf, sizeof(char) * strlen(buf));
						str = CString(buf);
						//	AfxMessageBox(str, MB_OK);
					}
			}
			
			
			file.Close();
			char temp[100];
			sprintf_s(temp, "%d\n", this->Chess->my_list.GetSize());
			CString ttt;
			char buf[100];
			CPoint** asdf;
			asdf = this->Chess->my_list.GetAt(0);
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					sprintf_s(buf, "%d %d, ", asdf[i][j].x, asdf[i][j].y);
					ttt += CString(buf);
				}
				ttt += CString("\n");
			}
			this->Chess->my_list.RemoveAll();
		}
		else
			AfxMessageBox(L"Failed", MB_OK);
		Invalidate();
}

void CChildView::OnLoadGame()
{
	
	char strFilter[] = { "All Files (*.kmh)|*.kmh|" };
	this->Chess->my_list.RemoveAll();
	this->Chess->index_chess_count = 0;
	this->Chess->is_end_game = 0;
	this->Chess->is_LButtonup_clicked = 0;
	this->Chess->index_loadfile = 0;
	isRandGame = 0;
	this->Chess->is_Load_game = 1;
	this->Chess->is_Rand_Game = 0;
	this->Chess->EndGame();
	CString agendaName, agendaPath;
	CFileDialog FileDlg(TRUE, CString(""), NULL, 0, CString(strFilter));
	CStdioFile file;
	if (FileDlg.DoModal() == IDOK) // this is the line which gives the errors
	{
		agendaName = FileDlg.GetFileName();
		agendaPath = FileDlg.GetPathName();
	}
	else
		return;
	
	CString path;
	this->Chess->path_root = CString("");
	for (int len = 0; len < agendaPath.GetLength()-agendaName.GetLength(); len++)
	{
		path.AppendChar(agendaPath.GetAt(len));
		this->Chess->path_root.AppendChar(agendaPath.GetAt(len));
	}
	this->Chess->GetAllFilesNames(path);
	char fjfj[100];
	CString s=CString("");
	for (int i = 0; i < agendaName.GetLength(); i++)
	{
		CString p=CString("");
		p.AppendChar(agendaName.GetAt(i));
		int is_number = _ttoi(p);
		
		if (is_number > 0 && is_number <= 9)
			s.AppendChar(agendaName.GetAt(i));
	}
	

	//	char asdfasdf[100];
	//	sprintf_s(asdfasdf, "%d", ttttt);
	//	AfxMessageBox(CString(asdfasdf), MB_OK);
	//	return;
	
	
	int kkk = _ttoi(s);
	
	
	
	this->Chess->index_loadfile = kkk - 1;

	LoadFile(this->Chess->path_root);

}

CPoint** CChildView::setMemory()
{
	CPoint **q = new CPoint * [8];
	for (int i = 0; i < 8; i++)
		q[i] = new CPoint[8];
	return q;
}
void CChildView::OnServerCreate()
{
	DlgCreateServer dlgServer;
	CChildView* ptView = ((CMainFrame*)AfxGetMainWnd())->GetView();

	if (ptView->Chess->IsServerOpen()) {
		AfxMessageBox(_T("Server is already opened!"));
		return;
	}
	
	if (dlgServer.DoModal())
		AfxMessageBox(_T("Failed to create server"));

	Invalidate();
}

void CChildView::OnCreateClient()
{
	DlgCreateClient dlgClient;

	if (Chess->IsServerOpen()) {
		AfxMessageBox(_T("Server is already opened!"));
		return;
	}

	if (Chess->IsClientOpen()) {
		AfxMessageBox(_T("Client is already connected!"));
		return;
	}

	if (dlgClient.DoModal() == false)
		AfxMessageBox(_T("Failed to connect to server"));

	Invalidate();
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
//	AfxMessageBox(_T("MouseMoving."));
	if (is_LButtonDown == 1)
	{
		//	CPaintDC dc(this);
		//	this->Chess->PaintCurrentChessPiece(dc, point);
		pt_MouseMove = point;
		//	this->Invalidate(false);

		this->InvalidateRect(CRect(point.x - 40, point.y - 40, point.x + 40, point.y + 40), true);
		return;
	}
	if (israndgameLbuttondown == 1)
	{
		this->Chess->pt_randgame_current_pos = point;
		this->InvalidateRect(CRect(point.x - 40, point.y - 40, point.x + 40, point.y + 40), true);
	}
	if (is_LButtonDown_rand_in_chessboard == 1)
	{
		this->Chess->pt_randgame_in_chessboard_current = point;
		this->InvalidateRect(CRect(point.x - 40, point.y - 40, point.x + 40, point.y + 40), true);
	}
	CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (is_LButtonDown == 1)
	{
		is_LButtonDown = 0;
	//	this->Chess->SetCurrentBlockData(this->Chess->index_chess_count - 1);
		this->Chess->OnLButtonupFromView(point);
		if (this->Chess->To_start_timer == 1)
		{
			UINT n_Timer = SetTimer(100, 100, NULL);
			this->Chess->SetMyTurn(this->Chess->timer_turn);
			this->Chess->pt_timer_current.x = this->Chess->pt_different_point_start.x*50+30;
			this->Chess->pt_timer_current.y = this->Chess->pt_different_point_start.y * 50 + 30;
		}
		
		this->Chess->is_LButtonup_clicked = 1;
		this->Invalidate();
		return;
	}
	if (israndgameLbuttondown == 1)
	{
		int CPprocedure[6] = { 0,1,2,3,4,5 };
		
		CPoint ptCursor = point - CPoint(30, 30);

		ptCursor.x /= 50;
		ptCursor.y /= 50;
		if (ptCursor.x < 8 && ptCursor.y < 8)
		{
			this->Chess->AddChessPiece(ptCursor, CPprocedure[this->Chess->pt_rand_get.x], this->Chess->pt_rand_get.y);
		}
		this->Invalidate();
		israndgameLbuttondown = 0;
		return;
	}
	if (is_LButtonDown_rand_in_chessboard == 1)
	{
		this->Chess->OnLButtonupFromView(point);
		is_LButtonDown_rand_in_chessboard = 0;
		
		this->Invalidate();
		return;
	}

	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 100)
	{
	//	AfxMessageBox(L"asdf", MB_OK);
		//if()
		if (index_timer_count == 10)
		{
			this->Chess->To_start_timer = 0;
			KillTimer(100);
			
			index_timer_count = 0;
			this->Chess->SetCurrentBlockData(this->Chess->index_chess_count);
			this->Chess->SetMyTurn(!this->Chess->timer_turn);
			this->Invalidate();
			if (this->Chess->index_chess_count == this->Chess->my_list.GetSize() - 1)
			{
				if (this->Chess->index_loadfile == this->Chess->m_fileloadarray.GetSize() - 1)
				{
					AfxMessageBox(L"End Game!congratulation", MB_OK);
					return;
				}
				else
				{
					this->Chess->index_loadfile++;
					LoadFile(this->Chess->path_root);
				}
			}
			return;
		}
		int distance_x = (this->Chess->pt_different_point_end.x - this->Chess->pt_different_point_start.x)*50/10;
		int distance_y = (this->Chess->pt_different_point_end.y - this->Chess->pt_different_point_start.y)*50/10;
		this->Chess->pt_timer_current.x = this->Chess->pt_different_point_start.x*50 + 30 + distance_x * index_timer_count;
		this->Chess->pt_timer_current.y = this->Chess->pt_different_point_start.y*50 + 30 + distance_y * index_timer_count;
		index_timer_count++;
		
		this->Invalidate();
	}
	CWnd::OnTimer(nIDEvent);
}


void CChildView::LoadFile(CString path)
{
	// TODO: Add your implementation code here.

	char strFilter[] = { "Text Files (*.txt)|*.txt|" };
	this->Chess->my_list.RemoveAll();
	this->Chess->index_chess_count = 0;
	this->Chess->is_end_game = 0;
	this->Chess->is_LButtonup_clicked = 0;
	isRandGame = 0;
	this->Chess->is_Rand_Game = 0;
	this->Chess->EndGame();
	CString agendaName, agendaPath;
//	CFileDialog FileDlg(TRUE, CString(".txt"), NULL, 0, CString(strFilter));
	CStdioFile file;

	CString m_CurrentFileName = this->Chess->m_fileloadarray.GetAt(this->Chess->index_loadfile);
	for (int len = 0; len < m_CurrentFileName.GetLength(); len++)
	{
		path.AppendChar(m_CurrentFileName.GetAt(len));
	}

	if (file.Open(path, CFile::modeRead))
	{
		CString buffer;
		CString result;
		int i = 0, j = 0;
		int k = 0;
		int temp = 0;
		int tt = 0;
		char buf;
		CString stext;

		CPoint** asdf = setMemory();
		while (file.ReadString(stext) != FALSE)
		{
			if (j == 8)
			{
				i++;
				j = 0;
			}
			if (i == 8)
			{
				this->Chess->my_list.Add(asdf);
				asdf = setMemory();
				i = 0;
			}
			buffer += stext;

			buffer += "\n";
			if (k == 0)
			{
				char fjfj[100];
				asdf[i][j].x = _ttoi(stext);
				sprintf_s(fjfj, "%d ", asdf[i][j].x);
				//AfxMessageBox(CString(fjfj), MB_OK);
			}
			else
			{
				asdf[i][j].y = _ttoi(stext);
				char fjfj[100];
				sprintf_s(fjfj, "%d ", asdf[i][j].y);
				//AfxMessageBox(CString(fjfj), MB_OK);
			}
			if (k == 1)
			{
				j++;
			}
			k++;
			k = k % 2;

		}

		this->Chess->my_list.Add(asdf);
		file.Close();
		this->Chess->is_Load_game = 1;
		//	this->Chess->index_chess_count = 0;
		char qwer[100];
		sprintf_s(qwer, "%d\n", this->Chess->my_list.GetSize());
		//	AfxMessageBox(CString(qwer), MB_OK);
		this->Chess->SetCurrentBlockData(0);
		//-------------------------------
		CPoint** pt1=this->Chess->my_list.GetAt(0);
		CPoint** pt2= this->Chess->my_list.GetAt(1);
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
			{
				if (pt1[i][j].x != pt2[i][j].x && pt1[i][j].y != pt2[i][j].y)
				{
					if (pt2[i][j].x != -1 && pt2[i][j].y != -1)
					{
						first_load_team_type = CPoint(pt2[i][j].x, pt2[i][j].y);
					}
				}
			}
		if (first_load_team_type.x == 1)
			this->Chess->SetMyTurn(0);
		else
			this->Chess->SetMyTurn(1);
	}
	else
	{
		AfxMessageBox(_T("Failed"), MB_OK);
		return;
	}
	//	this->Chess->EndGame();



	Invalidate();

}


void CChildView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (this->Chess->is_Load_game == 1 && this->Chess->index_chess_count==0 && this->Chess->GetTurn()==Chess::TEAM_BLACK)
	{
		this->Chess->myfunc(point);
		if (this->Chess->To_start_timer == 1)
		{
			UINT n_Timer = SetTimer(100, 100, NULL);
			index_timer_count = 0;
			this->Chess->SetMyTurn(this->Chess->timer_turn);
			this->Chess->pt_timer_current.x = this->Chess->pt_different_point_start.x * 50 + 30;
			this->Chess->pt_timer_current.y = this->Chess->pt_different_point_start.y * 50 + 30;
		}
		this->Invalidate();
	}
	CWnd::OnRButtonDblClk(nFlags, point);
}
