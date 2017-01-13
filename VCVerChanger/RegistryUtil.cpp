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
// �֐����FRegOpen
// �@�\�@�F���W�X�g���L�[�̃I�[�v��
// �����@�F����
// �߂�l�F����
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
// �֐����FRegClose
// �@�\�@�F���W�X�g���L�[�̃N���[�Y
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CRegistryUtil::RegClose(void)
{
	TRACE("RegClose : Registry close\n");
	m_RegKey.Close();
}

////////////////////////////////////////////////////////
// �֐����FReadEnv
// �@�\�@�F���W�X�g���G���g���ǂݍ���
// �����@�FLPCTSTR EnvName�@�F�G���g����
// �߂�l�F�G���g���l
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
// �֐����FWriteEnv
// �@�\�@�F���W�X�g���G���g����������
// �����@�FLPCTSTR EnvName�@: �G���g����
//         DWORD type       : �G���g���̌^
//         CString str      : �G���g���l
// �߂�l�Ftrue:����I���@false:�������݃G���[
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
// �֐����FDeleteEnv
// �@�\�@�F���W�X�g���G���g���폜
// �����@�FLPCTSTR EnvName�@: �G���g����
// �߂�l�Ftrue:����I���@false:�폜�G���[
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

