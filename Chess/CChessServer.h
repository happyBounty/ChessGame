#pragma once


class CChessServer : public CSocket
{
public:
	CChessServer();
	virtual ~CChessServer();
	virtual void OnAccept(int nErrorCode);
};