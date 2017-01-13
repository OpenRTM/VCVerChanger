/////////////////////////////////////////////////////////////////////////////
// Name			: TraceLog.h
// Create Date	: 2016.11.02
// Author		: Nobu Kawauchi
// Copyright (C) 2016 n-ando AIST All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////
#pragma once

class CTraceLog
{
public:
	CTraceLog(void);
	~CTraceLog(void);

	static void LogFileOpen(void);
	static void LogFileWrite(LPCTSTR lpcszBuf,...);
	static CString GetTimeString(void);
};

