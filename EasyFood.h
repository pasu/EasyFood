
// EasyFood.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEasyFoodApp:
// �йش����ʵ�֣������ EasyFood.cpp
//

class CEasyFoodApp : public CWinApp
{
public:
	CEasyFoodApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEasyFoodApp theApp;