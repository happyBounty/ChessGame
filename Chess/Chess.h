
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"     



class CChessApp : public CWinApp
{
public:
	CChessApp() noexcept;


public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();


public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CChessApp theApp;
