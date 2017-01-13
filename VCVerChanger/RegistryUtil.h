/////////////////////////////////////////////////////////////////////////////
// Name			: RegistryUtil.h
// Create Date	: 2017.01.10
// Author		: Nobu Kawauchi
// Copyright (C) 2017 n-ando AIST All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "atlbase.h"

class CRegistryUtil
{
public:
	CRegistryUtil(void);
	~CRegistryUtil(void);
    
    //���W�X�g���L�[��open/close
	void    RegOpen(void);			
	void	RegClose(void);
    
	//���W�X�g���G���g����read/write
	CString ReadEnv(LPCTSTR EnvName);
	bool	WriteEnv(LPCTSTR EnvName, DWORD type, CString str);
	bool	DeleteEnv(LPCTSTR EnvName);

private:    
    CRegKey	m_RegKey;
};

