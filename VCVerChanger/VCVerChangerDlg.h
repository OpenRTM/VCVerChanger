/////////////////////////////////////////////////////////////////////////////
// Name			: VCVerChangerDlg.h
// Create Date	: 2016.10.11
// Author		: Nobu Kawauchi
// Copyright (C) 2016 n-ando AIST All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////
#pragma once
#include "afxwin.h"
#include "RegistryUtil.h"

typedef struct VISUAL_STUDIO
{
	CString Release_Ver;	//"Visual C++ 14 2015", ...	
	CString Internal_Ver;	//vc14, ...	
}VISUAL_STUDIO;

typedef struct ARCH_INFO
{
	CString BitType;	//32bit, 64bit
	CString CV_ARCH;	//x86, x64
	CString pFilePath;	//Program Files (x86), Program Files
}ARCH_INFO;

// CVCVerChangerDlg �_�C�A���O
class CVCVerChangerDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CVCVerChangerDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
enum { IDD = IDD_VCVERCHANGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g

// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedUpdate();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedArchChange();
	afx_msg void OnDestroy();

private:
	void	InitMultilingual();
	void	Init();
	bool	RegistryReadProc(); //���W�X�g���E�G���g���Ǎ�����
	bool	RegistryWriteProc(CString toDir); //���W�X�g���E�G���g���������ݏ���
	bool	RegistryDeleteProc(CString target); //���W�X�g���E�G���g���폜����
	void	RegistryEntryReadErr();
	void	RegistryEntryErr(LPCTSTR EnvName, LPCTSTR proc);

	//target�̒�����w�蕶������܂ރp�X�𔲂��o��
	int	FindStringFromTargetPath(
				CString str,
				CString target,
				CString& outForGUI, 
				CString& outForReg,
				CString& otherPath);
	
	bool ReplaceEnv(CString Path, CString& outPath);	//�V�X�e�����ϐ��l�Ɋ܂܂��%�ϐ�%��W�J����
	void    SettingChange();			//WM_SETTINGCHANGE���b�Z�[�W���M
	int		InstalledArchCheck();		//OpenRTM-aist�̃A�[�L�e�N�`���𒲂ׂ�
	bool	CorrectSystemEnvValue();	//OpenRTM-aist�̃C���X�g�[���󋵂𒲂ׂ�
	void	CleanResult();				//�_�C�A���O��Result�\��������

	//�w��̃p�X�Ɋ܂܂��Program Files��؂�ւ���
	bool	ChangeProgramFilesPath(CString path, 
					CString toDir, CString& outPath);

	//PATH��OpenRTM-aist��32bit/64bit�̗���`�����݃`�F�b�N
	int 	PathDoubleDefinitionCheck(CString Path);

	//ARCH_INFO�\���̂Ŏw�肳�ꂽ�^�C�v(32bit/64bit)�ȊO�̃p�X�����O����
	CString	DeleteUnnecessaryPath(CString path, 
				CString delimit, ARCH_INFO* info);
    
	bool	OrganizePath();

	//�uvc**�v�̕�������u%RTM_VC_VERSION%�v�ɒu��������
	bool	ReplaceFromVCxxToRTMVCVERSION(CString path, CString& outPath);

	CString m_VcVersion;
	CString m_RtmBase;
	CString m_RtmRoot;
	CString m_RtmJavaRoot;
	CString m_OmniRoot;			//Dialog�\���p
	CString m_OmniRootForReg;	//���W�X�g�������p�i%�ϐ����܂ށj
	CString m_OpencvDir;
	CString m_OpenrtmDir;
	CString m_RtmPath;			//Dialog�\���p
	CString m_RtmPathForReg;	//���W�X�g�������p�i%�ϐ����܂ށj
	CString m_otherPath;		//OpenRTM-aist�ȊO��PATH�ݒ�
	CStatic m_WarningMsg;
	CStatic m_staticArch;
	CString m_LogPath;

	CRegistryUtil m_RegistryUtil;
	CString m_StatusMsg;
	CComboBox m_VSVer;
	CComboBox m_arch;
	CString m_x86Path;
	CString m_x64Path;

	VISUAL_STUDIO*	m_VSInfo;
	ARCH_INFO*		m_archInfo;

	//PATH����OpenRTM-aist 32bit/64bit����`��Ԃ������t���O
	bool m_32b64bDualDefineFlg;	//true:�����̃p�X�����݂���@flase:���݂��Ȃ�

	//OpenRTM-aist 32bit/64bit�C���X�g�[����Ԃ������t���O
	bool m_32b64bInstFlg;	//true:�����C���X�g�[������Ă���@flase:����̃o�[�W�����̂�
	
};
