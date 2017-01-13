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
// �֐����FLogFileOpen
// �@�\�@�F���O�t�@�C�����J��
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CTraceLog::LogFileOpen(void)
{
	time_t lTime;
	struct tm pTime;							// �쐬����
	CString strFilename = "";
	char	buf[1024];
	
	CreateDirectory((LPCTSTR)LOGDIR, NULL);

	time( &lTime );								// long �����Ƃ��Ď������擾
	localtime_s( &pTime, &lTime );

	//���O�t�@�C�����́A���t�{���ԁ@��jTRACE1012_1753.txt
	strFilename.Format("TRACE%02d%02d_%02d%02d%02d",
		pTime.tm_mon+1, pTime.tm_mday,
		pTime.tm_hour, pTime.tm_min, pTime.tm_sec );
	sprintf_s(buf, "%s\\%s.%s", LOGDIR, LPCTSTR(strFilename), "log");
	
	m_pCFile = new CFile( buf, LOG_DEF_FLAG );
}

////////////////////////////////////////////////////////
// �֐����FLogFileWrite
// �@�\�@�F�f�o�b�O�������O�t�@�C���ɏo�͂���
// �����@�FLPCTSTR lpcszBuf,...: TRACE�R�}���h�Ɠ��l
// �߂�l�F����
// �⑫  �F#define�Œ�`���邱�Ƃɂ��TRACE������������
//         ���O�t�@�C���ɏo�͂���B
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
// �֐����FGetTimeString
// �@�\�@�Fhh:mm:ss.xxx �̌`���Ń~���b�P�ʂ̎������悷��
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
CString CTraceLog::GetTimeString(void)
{
	char szBuf[64];
	CString Time;

	_strtime_s( szBuf );	// �����̎擾
	struct timeb tstruct;
	ftime( &tstruct );		// �~���b�̎擾
	
	Time.Format("%s.%03d",szBuf,tstruct.millitm);
	return Time;
}
