
// VCVerChanger.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CVCVerChangerApp:
// このクラスの実装については、VCVerChanger.cpp を参照してください。
//

class CVCVerChangerApp : public CWinApp
{
public:
	CVCVerChangerApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CVCVerChangerApp theApp;