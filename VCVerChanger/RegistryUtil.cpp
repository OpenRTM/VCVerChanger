/////////////////////////////////////////////////////////////////////////////
// Name			: RegistryUtil.cpp
// Description	: implementation of the CRegistryUtil class
//
// To operate the registry.
//
// Create Date	: 2017.01.10
// Author		: Nobu Kawauchi
// Copyright (C) 2017 n-ando AIST All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "RegistryUtil.h"

CRegistryUtil::CRegistryUtil(void)
{
}

CRegistryUtil::~CRegistryUtil(void)
{
}

////////////////////////////////////////////////////////
// 関数名：RegOpen
// 機能　：レジストリキーのオープン
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CRegistryUtil::RegOpen(void)
{	
	CString HKLMkey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
	LONG ret;

	TRACE("RegOpen : Registry Open\n");
    ret = m_RegKey.Open(HKEY_LOCAL_MACHINE, HKLMkey, KEY_ALL_ACCESS);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("Registry Open Error.\n");
		return;
	}
}

////////////////////////////////////////////////////////
// 関数名：RegClose
// 機能　：レジストリキーのクローズ
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CRegistryUtil::RegClose(void)
{
	TRACE("RegClose : Registry close\n");
	m_RegKey.Close();
}

////////////////////////////////////////////////////////
// 関数名：ReadEnv
// 機能　：レジストリエントリ読み込み
// 引数　：LPCTSTR EnvName　：エントリ名
// 戻り値：エントリ値
////////////////////////////////////////////////////////
CString CRegistryUtil::ReadEnv(LPCTSTR EnvName)
{
	LONG ret;
   	DWORD size;
	CString retValue, buff;
    ULONG buffLen;
    
    ret = m_RegKey.QueryValue(EnvName, NULL, NULL, &size);    
	if (ret != ERROR_SUCCESS)
	{
		TRACE("ReadEnv : QueryValue(%s) Error.(%d)\n", EnvName, ret);
		return NULL;
	}

	buffLen = size;
	ret = m_RegKey.QueryStringValue(EnvName, buff.GetBuffer(size + 1), &buffLen);
	retValue = buff;
	buff.ReleaseBuffer();
	if (ret != ERROR_SUCCESS)
	{
		TRACE("ReadEnv : QueryStringValue(%s) Error.(%d)\n", EnvName, ret);
		return NULL;
	}
	TRACE("ReadEnv : EnvName=%s (%s)\n", EnvName, retValue);

	return retValue;
}

////////////////////////////////////////////////////////
// 関数名：WriteEnv
// 機能　：レジストリエントリ書き込み
// 引数　：LPCTSTR EnvName　: エントリ名
//         DWORD type       : エントリの型
//         CString str      : エントリ値
// 戻り値：true:正常終了　false:書き込みエラー
////////////////////////////////////////////////////////
bool CRegistryUtil::WriteEnv(LPCTSTR EnvName,
	DWORD type, CString str)
{
	LONG ret;

	ret = m_RegKey.SetStringValue(EnvName, str, type);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("WriteEnv : SetStringValue(%s) Error.(%d)\n", EnvName, ret);
		return false;
	}
	TRACE("WriteEnv : EnvName=%s (%s)\n", EnvName, str);

	return true;
}

////////////////////////////////////////////////////////
// 関数名：DeleteEnv
// 機能　：レジストリエントリ削除
// 引数　：LPCTSTR EnvName　: エントリ名
// 戻り値：true:正常終了　false:削除エラー
////////////////////////////////////////////////////////
bool CRegistryUtil::DeleteEnv(LPCTSTR EnvName)
{
	LONG ret;

	ret = m_RegKey.DeleteValue(EnvName);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("DeleteEnv : DeleteValue(%s) Error.(%d)\n", EnvName, ret);
		return false;
	}
	TRACE("DeleteEnv : EnvName=%s\n", EnvName);
	return true;
}

