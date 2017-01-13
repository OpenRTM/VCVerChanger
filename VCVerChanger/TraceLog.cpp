/////////////////////////////////////////////////////////////////////////////
// Name			: TraceLog.cpp
// Description	: implementation of the TraceLog class
// Create Date	: 2016.11.02
// Author		: Nobu Kawauchi
// Copyright (C) 2016 n-ando AIST All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "TraceLog.h"
#include <sys/timeb.h>

#define LOGDIR ".\\Log"
#define LOG_DEF_FLAG	(CFile::modeCreate | CFile::modeWrite)

static CFile*	m_pCFile;

CTraceLog::CTraceLog(void)
{
	m_pCFile = NULL;
}

CTraceLog::~CTraceLog(void)
{
	if( m_pCFile )
	{
		m_pCFile->Close();
		delete m_pCFile;
		m_pCFile = NULL;
	}
}

////////////////////////////////////////////////////////
// 関数名：LogFileOpen
// 機能　：ログファイルを開く
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CTraceLog::LogFileOpen(void)
{
	time_t lTime;
	struct tm pTime;							// 作成時刻
	CString strFilename = "";
	char	buf[1024];
	
	CreateDirectory((LPCTSTR)LOGDIR, NULL);

	time( &lTime );								// long 整数として時刻を取得
	localtime_s( &pTime, &lTime );

	//ログファイル名は、日付＋時間　例）TRACE1012_1753.txt
	strFilename.Format("TRACE%02d%02d_%02d%02d%02d",
		pTime.tm_mon+1, pTime.tm_mday,
		pTime.tm_hour, pTime.tm_min, pTime.tm_sec );
	sprintf_s(buf, "%s\\%s.%s", LOGDIR, LPCTSTR(strFilename), "log");
	
	m_pCFile = new CFile( buf, LOG_DEF_FLAG );
}

////////////////////////////////////////////////////////
// 関数名：LogFileWrite
// 機能　：デバッグ情報をログファイルに出力する
// 引数　：LPCTSTR lpcszBuf,...: TRACEコマンドと同様
// 戻り値：無し
// 補足  ：#defineで定義することによりTRACE文を乗っ取って
//         ログファイルに出力する。
////////////////////////////////////////////////////////
void CTraceLog::LogFileWrite(LPCTSTR lpcszBuf,...)
{
	static char szMsg[4096];
	CString strLog;
	CString strTime;

	if( !m_pCFile ){
		LogFileOpen();
	}

	va_list arglist;
	va_start(arglist, lpcszBuf);
	vsprintf_s(szMsg, lpcszBuf, arglist);
	int nLength = strlen(szMsg);
	va_end(arglist);
	ASSERT ( nLength < 4096 );	
	
	strTime = GetTimeString();
	strLog.Format("%s %s", LPCTSTR(strTime), szMsg);
	m_pCFile->Write(strLog, strlen(strLog));
	m_pCFile->Flush();
}

////////////////////////////////////////////////////////
// 関数名：GetTimeString
// 機能　：hh:mm:ss.xxx の形式でミリ秒単位の時刻を取する
// 引数　：無し
// 戻り値：時刻
////////////////////////////////////////////////////////
CString CTraceLog::GetTimeString(void)
{
	char szBuf[64];
	CString Time;

	_strtime_s( szBuf );	// 時刻の取得
	struct timeb tstruct;
	ftime( &tstruct );		// ミリ秒の取得
	
	Time.Format("%s.%03d",szBuf,tstruct.millitm);
	return Time;
}
