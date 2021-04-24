#pragma once


class CChessClient : public CSocket
{
public:
	CChessClient();
	virtual ~CChessClient();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};


