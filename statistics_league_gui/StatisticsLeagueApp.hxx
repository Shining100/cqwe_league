
// statistics_league_gui.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStatisticsLeagueApp:
// �йش����ʵ�֣������ statistics_league_gui.cpp
//

class CStatisticsLeagueApp : public CWinApp
{
public:
	CStatisticsLeagueApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStatisticsLeagueApp theApp;