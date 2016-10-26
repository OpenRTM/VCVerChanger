/////////////////////////////////////////////////////////////////////////////
// Name			:VCVerChangerDlg.cpp
// Description	: implementation of the CVCVerChangerDlg class
//
// To check and change the system environment variables of OpenRTM-aist.
// To operate the registry.
//
// Create Date	: 2016.10.11
// Author		: Nobu Kawauchi
// Copyright (C) 2016 n-ando All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VCVerChanger.h"
#include "VCVerChangerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVCVerChangerDlg �_�C�A���O



CVCVerChangerDlg::CVCVerChangerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVCVerChangerDlg::IDD, pParent)
	, m_RtmBase(_T(""))
	, m_RtmRoot(_T(""))
	, m_RtmJavaRoot(_T(""))
	, m_OmniRoot(_T(""))
	, m_OpencvDir(_T(""))
	, m_OpenrtmDir(_T(""))
	, m_RtmPath(_T(""))
	, m_VcVersion(_T(""))
	, m_StatusMsg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVCVerChangerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RTM_BASE, m_RtmBase);
	DDX_Text(pDX, IDC_RTM_ROOT, m_RtmRoot);
	DDX_Text(pDX, IDC_RTM_JAVA_ROOT, m_RtmJavaRoot);
	DDX_Text(pDX, IDC_OMNI_ROOT, m_OmniRoot);
	DDX_Text(pDX, IDC_OPENCV_DIR, m_OpencvDir);
	DDX_Text(pDX, IDC_OPENRTM_DIR, m_OpenrtmDir);
	DDX_Text(pDX, IDC_RTM_PATH, m_RtmPath);
	DDX_Control(pDX, IDC_WARNING_MSG, m_WarningMsg);
	DDX_Text(pDX, IDC_VC_VER, m_VcVersion);
	DDX_Text(pDX, IDC_STATUS_MSG, m_StatusMsg);
	DDX_Control(pDX, IDC_ARCH, m_arch);
	DDX_Control(pDX, IDC_VS_VER, m_VSVer);
	DDX_Control(pDX, IDC_STATIC_ARCH, m_staticArch);
}

BEGIN_MESSAGE_MAP(CVCVerChangerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_CHECK, &CVCVerChangerDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDCLOSE, &CVCVerChangerDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_UPDATE, &CVCVerChangerDlg::OnBnClickedUpdate)
	ON_WM_CTLCOLOR()
	ON_WM_SETTINGCHANGE()
	ON_BN_CLICKED(IDC_ARCH_CHANGE, &CVCVerChangerDlg::OnClickedArchChange)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CVCVerChangerDlg ���b�Z�[�W �n���h���[

BOOL CVCVerChangerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	Init();

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CVCVerChangerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CVCVerChangerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CVCVerChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

////////////////////////////////////////////////////////
// �֐����FInit
// �@�\�@�F��������
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::Init()
{
	int i;

	GetDlgItem(IDC_UPDATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_VS_VER)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ARCH)->ShowWindow(FALSE);
	GetDlgItem(IDC_ARCH)->ShowWindow(FALSE);
	GetDlgItem(IDC_ARCH_CHANGE)->ShowWindow(FALSE);

	VISUAL_STUDIO vc_ver[] = {
		{"Visual C++ 17", "vc17"},
		{"Visual C++ 16", "vc16"},
		{"Visual C++ 15", "vc15"},
		{"Visual C++ 14 2015", "vc14"},
		{"Visual C++ 12 2013", "vc12"},
		{"Visual C++ 11 2012", "vc11"},
		{"Visual C++ 10 2010", "vc10"},
		{"Visual C++ 9 2008", "vc9"},
	};
	const int vcNum = sizeof vc_ver / sizeof vc_ver[0];

	m_VSInfo = new VISUAL_STUDIO[vcNum];
	for(i=0; i<vcNum; i++)
	{
		m_VSInfo[i] = vc_ver[i];
		
		//�R���{�{�b�N�X�\��
		m_VSVer.InsertString(-1, m_VSInfo[i].Release_Ver);
	}

	m_x86Path = "Program Files (x86)";
	m_x64Path = "Program Files";

	ARCH_INFO archInfo[] = {
		{"32bit", "x86", m_x86Path},
		{"64bit", "x64", m_x64Path}
	};

	m_archInfo = new ARCH_INFO[2];
	for(i=0; i<2; i++)
	{
		m_archInfo[i] = archInfo[i];

		//�R���{�{�b�N�X�\��
		m_arch.InsertString(-1, m_archInfo[i].BitType);
	}

	//32bit�A64bit�̗��o�[�W�����C���X�g�[�����
	//�����C���X�g�[�����Ă��邩�A�ۂ�
	//true:���Ă�@false:���ĂȂ�
	m_32b64bInstFlg = false;

	//PATH����OpenRTM-aist 32bit/64bit����`���
	//true:�����̃p�X�����݂���@flase:���݂��Ȃ�
	m_32b64bDualDefineFlg = false;

	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("�m�F�{�^���������ĉ������B");
}

////////////////////////////////////////////////////////
// �֐����FRegOpen
// �@�\�@�F���W�X�g���L�[�̃I�[�v��
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegOpen()
{	
	CString HKLMkey = "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
	LONG ret;

	m_hkey = 0;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, HKLMkey, 0, KEY_ALL_ACCESS, &m_hkey);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("RegOpenKeyEx Error.\n");
		return;
	}
}

////////////////////////////////////////////////////////
// �֐����FRegClose
// �@�\�@�F���W�X�g���L�[�̃N���[�Y
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegClose()
{
	RegCloseKey(m_hkey);
	m_hkey = 0;
}

////////////////////////////////////////////////////////
// �֐����FOnBnClickedCheck
// �@�\�@�F�m�F�{�^������
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnBnClickedCheck()
{
	CString path, str;
	int ret;
	bool bRet;

	TRACE("%s OnBnClickedCheck : �m�F�{�^������\n", GetTime());
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("");

	//���W�X�g���ǂݏo��
	bRet = RegistryReadProc();
	if (!bRet) return;

	//PATH��OpenRTM-aist��32bit/64bit�̗���`�����݂��邩�H
	bRet = PathDoubleDefinitionCheck(m_RtmPath);
	if(bRet)
	{		
		str = "OpenRTM-aist��PATH��32bit/64bit������`����Ă��܂��B�ؑփ{�^���łǂ��炩�ɐݒ肵�ĉ������B";
		GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
		m_32b64bDualDefineFlg = true;
	}

	//�{�c�[������x���s����ƁAPATH�̒�`�͐����ς݂�
	//����̃o�[�W�����������o�ł��Ȃ��Ȃ�
	//�f�B���N�g���̑��݊m�F�ŗ��o�[�W�����̃C���X�g�[��
	//��Ԃ��`�F�b�N����
	ret = InstalledArchCheck();
	if (ret != -1)
	{
		//���o�[�W�������C���X�g�[������Ă���
		m_arch.SetCurSel(ret);
		m_32b64bInstFlg = true;
	}

	if (m_32b64bInstFlg)
	{
		GetDlgItem(IDC_STATIC_ARCH)->ShowWindow(TRUE);
		GetDlgItem(IDC_ARCH)->ShowWindow(TRUE);
		GetDlgItem(IDC_ARCH_CHANGE)->ShowWindow(TRUE);
	}
	GetDlgItem(IDC_VS_VER)->EnableWindow(TRUE);
	GetDlgItem(IDC_UPDATE)->EnableWindow(TRUE);
	UpdateData(FALSE);	
}

////////////////////////////////////////////////////////
// �֐����FRegistryReadProc
// �@�\�@�F���W�X�g���ǂݍ��ݏ���
// �����@�F����
// �߂�l�Ftrue:����I���@false:�G���g������`
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::RegistryReadProc()
{
	CString EnvValue = "";
	CString path, str;
	int i;

	RegOpen();

	EnvValue = ReadEnv("RTM_VC_VERSION");
	if (EnvValue == "")
	{
		RegistryEntryReadErr();
		RegClose();
		return false;
	}
	m_VcVersion = EnvValue;
	for(i=0; i<m_VSVer.GetCount(); i++)
	{
		if(m_VcVersion == m_VSInfo[i].Internal_Ver)
		{
			m_VSVer.SetCurSel(i);
			break;
		}
	}

	EnvValue = ReadEnv("RTM_BASE");
	if (EnvValue == "")
	{
		RegistryEntryReadErr();
		RegClose();
		return false;
	}
	m_RtmBase = EnvValue;

	EnvValue = ReadEnv("RTM_ROOT");
	if (EnvValue == "") return false;
	m_RtmRoot = EnvValue;

	EnvValue = ReadEnv("RTM_JAVA_ROOT");
	if (EnvValue == "") return false;
	m_RtmJavaRoot = EnvValue;

	EnvValue = ReadEnv("OMNI_ROOT");
	if (EnvValue == "") return false;
	//���W�X�g�������߂��p
	m_OmniRootForReg = EnvValue; 
	//GUI�\���p��%OMNI_ROOT%��W�J������
	m_OmniRoot = ReplaceEnv(EnvValue);

	EnvValue = ReadEnv("OpenCV_DIR");
	if (EnvValue == "") return false;
	m_OpencvDir = EnvValue;

	EnvValue = ReadEnv("OpenRTM_DIR");
	if (EnvValue == "") return false;
	m_OpenrtmDir = EnvValue;

	EnvValue = ReadEnv("Path");
	if (EnvValue == "") return false;
	//�ǂݍ��񂾏�Ԃ��o�b�N�A�b�v���Ă���
	m_orgPATH = EnvValue;
	TRACE("RegistryReadProc : m_orgPATH : %s\n", m_orgPATH);
	
	//OpenRTM-aist���ݒ肵���p�X��؂�o��
	CString str1, str2, str3;	
	GetOpenrtmPath(EnvValue, str1, str2, str3); 
	m_RtmPath = str1;
	m_RtmPathForReg = str2;
	m_otherPath = str3;

	RegClose();
	return true;
}

////////////////////////////////////////////////////////
// �֐����FRegistryEntryReadErr
// �@�\�@�F���W�X�g���G���g���ǂݍ��݃G���[���̏���
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegistryEntryReadErr()
{
	CString str;

	str = "OpenRTM-aist 1.1.2�ňȍ~�̃o�[�W�������C���X�g�[������Ă��邩�A�m�F���ĉ������B";
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// �֐����FReadEnv
// �@�\�@�F���W�X�g���G���g���ǂݍ���
// �����@�FLPCTSTR EnvName�@�F�G���g����
// �߂�l�F�G���g���l
////////////////////////////////////////////////////////
CString CVCVerChangerDlg::ReadEnv(LPCTSTR EnvName)
{
	LONG ret;
	LPDWORD type = 0;
	DWORD size;
	LPBYTE buff = NULL;
	CString retValue;

	ret = RegQueryValueEx(m_hkey, EnvName, NULL, type, NULL, &size);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("RegQueryValueEx Error.(%d)\n", ret);
		return NULL;
	}

	buff = new BYTE[size];
	memset(buff, 0, size);
	ret = RegQueryValueEx(m_hkey, EnvName, NULL, type, buff, &size);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("RegQueryValueEx Error.(%d)\n", ret);
		return NULL;
	}
	retValue = (LPCSTR)buff;
	delete [] buff;
	return retValue;
}

////////////////////////////////////////////////////////
// �֐����FWriteEnv
// �@�\�@�F���W�X�g���G���g����������
// �����@�FLPCTSTR EnvName�@�F�G���g����
// �߂�l�F�G���g���l
////////////////////////////////////////////////////////
LONG CVCVerChangerDlg::WriteEnv(LPCTSTR EnvName,
	DWORD type, CString str)
{
	LONG ret;
	rsize_t size;
	char* buff;
		
	size = str.GetLength() + 1;
	buff = new char[size];
	strcpy_s(buff, size, str);
	ret = RegSetValueEx(m_hkey, EnvName, NULL, type, (const BYTE*)(LPCTSTR)buff, size);
	if (ret != ERROR_SUCCESS)
	{
		TRACE("RegSetValueEx Error.(%d)\n", ret);
		return ret;
	}
	TRACE("WriteEnv : EnvName=%s (%s)\n", EnvName, buff);
	delete[] buff;

	return ret;
}

////////////////////////////////////////////////////////
// �֐����FReplaceEnv
// �@�\�@�F�V�X�e�����ϐ��l�Ɋ܂܂��%%�ϐ���W�J����
//    RegQueryValueEx�̓��W�X�g���l�Ɋ܂܂��%%�ϐ���
//    �W�J���Ă���Ȃ����߁A�W�J������
// �����@�FCString Path�@�F�W�J�O�̃p�X
// �߂�l�F�W�J��̃p�X
////////////////////////////////////////////////////////
CString  CVCVerChangerDlg::ReplaceEnv(CString Path)
{
	CString str1, str2, str3, src;
	src = Path;
	int ptr = src.Find("%RTM_VC_VERSION%");
	while (ptr != -1)
	{
		str1 = src.Mid(0, ptr);
		str2 = src.Mid(ptr + 16);
		str3.Format("%s%s%s", str1, m_VcVersion, str2);
		src = str3;
		ptr = src.Find("%RTM_VC_VERSION%");
		if (ptr < 16) break;
	}

	ptr = src.Find("%OMNI_ROOT%");
	while (ptr != -1)
	{
		str1 = src.Mid(0, ptr);
		str2 = src.Mid(ptr + 11);
		str3.Format("%s%s%s", str1, m_OmniRoot, str2);
		src = str3;
		ptr = src.Find("%OMNI_ROOT%");
		if (ptr < 11) break;
	}

	ptr = src.Find("%OpenCV_DIR%");
	while (ptr != -1)
	{
		str1 = src.Mid(0, ptr);
		str2 = src.Mid(ptr + 12);
		str3.Format("%s%s%s", str1, m_OpencvDir, str2);
		src = str3;
		ptr = src.Find("%OpenCV_DIRT%");
		if (ptr < 10) break;
	}
	return src;
}

////////////////////////////////////////////////////////
// �֐����FGetOpenrtmPath
// �@�\�@�FPATH�̒�����uOpenRTM-aist�v���܂ނ��̂𔲂��o��
//   �uOpenRTM-aist�v�̃p�X�͋�؂蕶����"\n"��";"�̂Q��
//�@�@�p�^�[����Ԃ��B(out1��out2�j
//�@�@PATH����uOpenRTM-aist�v�̃p�X�����������̂�out3�Ƃ���
//    �Ԃ��B
// �����@�Fin :Path�@�FPATH�̒l
//    :out1 :outForGUI :��؂蕶��"\n"�iGUI�\���p�j
//    :out2 :outForReg :��؂蕶��";"�i���W�X�g�������p�j
//    :out3 :otherPath :���W�X�g����Path�����߂����Ɏg�p
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::GetOpenrtmPath(
				CString Path, 
				CString& outForGUI, 
				CString& outForReg,
				CString& otherPath)
{
	CString string, substr;
	int pos = 0;
	int findpos = 0;

	//�󂯎����path��";"�ŕ�������
	substr = Path.Tokenize(";", pos);
	while (substr != "")
	{
		//%%�ϐ���W�J����
		string = ReplaceEnv(substr);
		//OpenRTM-aist���܂ރp�X���������o��
		findpos = string.Find("OpenRTM-aist");
		if (findpos != -1)
		{
			TRACE("GetOpenrtmPath : string : %s\n", string);
			//GUI�ł͕����̃p�X�͉��s�\��������
			outForGUI = outForGUI + string + "\n";
			//���W�X�g�������߂��p��%%�ϐ����p�A;�ŋ�؂�
			outForReg = outForReg + substr + ";";

		}else{
			//OpenRTM-aist�ȊO�̃p�X��otherPath�ɕۑ����Ă���
			//���W�X�g���ւ̏������ݎ��ɍ��킹�ď����߂�
			otherPath = otherPath + substr + ";";
		}
		substr = Path.Tokenize(";", pos);
	}
	TRACE("GetOpenrtmPath : otherPath : %s\n", otherPath);
}

////////////////////////////////////////////////////////
// �֐����FPathDoubleDefinitionCheck
// �@�\�@�FPATH��OpenRTM-aist��32bit/64bit�̗���`������
//         ���Ă��邩�`�F�b�N����
// �����@�FCString Path�@�FPATH�̒l
// �߂�l�Ftrue: 32/64bit�̗���`������@�@false:�Ȃ�
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::PathDoubleDefinitionCheck(CString Path)
{
	bool ret = false;
	int findpos = 0;
	CString binPath;

	//32bit/64bit�̗��o�[�W�������C���X�g�[����A
	//�V�X�e�����ϐ����������Ă��Ȃ���΁APATH�ɉ��L��
	//��`�����݂���
	//C:\Program Files\OpenRTM-aist\1.1.2\bin\%RTM_VC_VERSION%
	//C:\Program Files (x86)\OpenRTM-aist\1.1.2\bin\%RTM_VC_VERSION%
	//���̏�Ԃ��ǂ������`�F�b�N����

	//%RTM_ROOT%bin\%RTM_VC_VERSION%�̃p�X�𐶐�
	binPath.Format("%sbin\\%s", m_RtmRoot, m_VcVersion);

	//�w��̃p�X�Ɋ܂܂��Program Files��؂�ւ���
	//"Program Files (x86)" -> "Program Files"
	//"Program Files" -> "Program Files (x86)"
	findpos = binPath.Find(m_x86Path);
	if (findpos != -1)
	{
		binPath.Replace(m_x86Path, m_x64Path);
	}else{
		binPath.Replace(m_x64Path, m_x86Path);
	}

	//binPath��PATH�ɒ�`����Ă��邩�H
	findpos = Path.Find(binPath);
	if (findpos != -1)
	{
		ret = true;
	}
	return ret;
}

////////////////////////////////////////////////////////
// �֐����FOnBnClickedClose
// �@�\�@�F�I���{�^������
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnBnClickedClose()
{
	delete [] m_VSInfo;
	delete [] m_archInfo;
	m_VSInfo = NULL;
	m_archInfo = NULL;
	EndDialog(IDOK);
}

////////////////////////////////////////////////////////
// �֐����FOnBnClickedUpdate
// �@�\�@�F�X�V�{�^������
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnBnClickedUpdate()
{
	CString Msg1, Msg2, Msg3, input;
	int i;

	TRACE("%s OnBnClickedUpdate : �X�V�{�^������\n", GetTime());
	m_StatusMsg = "������...";
	UpdateData(FALSE);

	GetDlgItem(IDC_VS_VER)->GetWindowTextA(input);
	for(i=0; i<m_VSVer.GetCount(); i++)
	{
		if(input == m_VSInfo[i].Release_Ver)
		{
			m_VcVersion = m_VSInfo[i].Internal_Ver;
			break;
		}
	}
	TRACE("OnBnClickedUpdate : Combo Box�̓��͒l : m_VcVersion : %s\n", m_VcVersion);

	RegOpen();

	//���W�X�g���G���g���@��������
	WriteEnv("RTM_VC_VERSION", REG_SZ, m_VcVersion);

	RegClose();

	//WM_SETTINGCHANGE
	SettingChange();

	//Sleep(300);

	//����������̃��W�X�g����ǂݍ���
	//OnBnClickedCheck();
	RegistryReadProc();
	  
	Msg1 = "�X�V��̊m�F�����BOMNI_ROOT, PATH�Ɋ܂܂��RTM_VC_VERSION�̒l���u ";
	Msg2 = " �v�ɂȂ�܂����B";
	Msg3.Format("%s%s%s", Msg1, m_VcVersion, Msg2);
	m_StatusMsg = Msg3;
  
	UpdateData(FALSE);
}


HBRUSH CVCVerChangerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// �X�^�e�B�b�N�e�L�X�g�̐F�ύX
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (*pWnd == m_WarningMsg)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}		

	// TODO: ����l���g�p�������Ȃ��ꍇ�͕ʂ̃u���V��Ԃ��܂��B
	return hbr;
}


////////////////////////////////////////////////////////
// �֐����FSettingChange
// �@�\�@�FWM_SETTINGCHANGE���b�Z�[�W���M
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::SettingChange()
{
	DWORD dwReturnValue;
	LRESULT Ret = SendMessageTimeout(
		HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)("Environment"),
		SMTO_ABORTIFHUNG, 5000, &dwReturnValue);
	if (!Ret)
	{
		TRACE("SettingChange error. (%d)\n", Ret);
	}
}

CString CVCVerChangerDlg::GetTime()
{
	CString time;
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	time.Format("%u%u%u_%u%u%u.%u",
		systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond,
		systime.wMilliseconds);

	return time;
}

////////////////////////////////////////////////////////
// �֐����FOnClickedArchChange
// �@�\�@�F�ؑփ{�^������
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnClickedArchChange()
{
	int i, index = 0;
	CString input, outpath, Msg1, Msg2, Msg3;
	CString str;
	bool ret;

	TRACE("OnClickedArchChange : �ؑփ{�^������\n");
	m_StatusMsg = "������...";
	UpdateData(FALSE);
	GetDlgItem(IDC_ARCH)->GetWindowTextA(input);
	TRACE("OnClickedArchChange : Combo Box�̓��͒l : %s\n", input);

	for (i=0; i<m_arch.GetCount(); i++)
	{
		if (input == m_archInfo[i].BitType)
		{
			break;
		}
	}

	//Path�̐ݒ��32bit, 64bit�p�̃p�X�����݂��Ă����
	//�w��ȊO�̃p�X�͏��O����
	if (m_32b64bDualDefineFlg)
	{
		m_RtmPathForReg = DeleteUnnecessaryPath(
					m_RtmPathForReg, ";", &m_archInfo[i]);
		m_32b64bDualDefineFlg = false;
	}
	
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("");

	RegOpen();

	ret = ChangeProgramFilesPath(m_RtmBase, &m_archInfo[i], outpath);
	if (ret)
	{
		//���W�X�g���̏��������K�v
		WriteEnv("RTM_BASE", REG_SZ, outpath);

		ChangeProgramFilesPath(m_RtmRoot, &m_archInfo[i], outpath);
		WriteEnv("RTM_ROOT", REG_SZ, outpath);
	
		ChangeProgramFilesPath(m_RtmJavaRoot, &m_archInfo[i], outpath);
		WriteEnv("RTM_JAVA_ROOT", REG_SZ, outpath);

		ChangeProgramFilesPath(m_OmniRootForReg, &m_archInfo[i], outpath);
		WriteEnv("OMNI_ROOT", REG_EXPAND_SZ, outpath);

		ChangeProgramFilesPath(m_OpencvDir, &m_archInfo[i], outpath);
		WriteEnv("OpenCV_DIR", REG_SZ, outpath);

		ChangeProgramFilesPath(m_OpenrtmDir, &m_archInfo[i], outpath);
		WriteEnv("OpenRTM_DIR", REG_SZ, outpath);
	}

	ChangeProgramFilesPath(m_RtmPathForReg, &m_archInfo[i], outpath);
	str = m_otherPath + outpath;
	TRACE("OnClickedArchChange : str :%s\n", str);
	WriteEnv("Path", REG_EXPAND_SZ, str);

	RegClose();

	//WM_SETTINGCHANGE
	SettingChange();

	//����������̃��W�X�g����ǂݍ���
	RegistryReadProc();

	Msg1 = "�ؑ֌�̊m�F�����B�u ";
	Msg2 = " �v�p�̐ݒ�ɂȂ�܂����B";
	Msg3.Format("%s%s%s", Msg1, input, Msg2);
	m_StatusMsg = Msg3;
  
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// �֐����FInstalledArchCheck
// �@�\�@�FOpenRTM-aist�̃A�[�L�e�N�`���𒲂ׂ�
//         32bit/64bit�ł̗������C���X�g�[������Ă�����
//         �ؑ։\�Ƃ��锻�f�ɗ��p
// �����@�F����
// �߂�l�F-1:�ǂ��炩����  
//          0:�����C���X�g�[���ς݂Ō��݂̐ݒ��32bit
//          1:�����C���X�g�[���ς݂Ō��݂̐ݒ��64bit
////////////////////////////////////////////////////////
int CVCVerChangerDlg::InstalledArchCheck()
{
	CString str, tmp;	
	int findpos = 0;
	int ret = -1;

	//OpenRTM_DIR�̃f�B���N�g�����݊m�F��
	//�C���X�g�[����Ԃ𔻒f����
	str = m_OpenrtmDir;
	TRACE("str : %s\n", str);
	//32bit�Ŋm�F
	findpos = str.Find(m_x86Path);
	if (findpos != -1)
	{
		//�p�X��32bit�ݒ�ɂȂ��Ă���		
		tmp = str;
		tmp.Replace(m_x86Path, m_x64Path);
		TRACE("tmp : %s\n", tmp);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			//64bit�ł����݂��Ă���
			ret = 0;
		}						
	}else{
		//�p�X��64bit�ݒ�ɂȂ��Ă���
		tmp = str;
		tmp.Replace(m_x64Path, m_x86Path);
		TRACE("tmp : %s\n", tmp);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			//32bit�ł����݂��Ă���
			ret = 1;
		}
	}
	return ret;
}

////////////////////////////////////////////////////////
// �֐����FChangeProgramFilesPath
// �@�\�@�F�w��̃p�X�Ɋ܂܂��Program Files��؂�ւ���
//         "Program Files (x86)" -> "Program Files"
//         "Program Files" -> "Program Files (x86)"
// �����@�FCString path     :�Ώۂ̃p�X
//         ARCH_INFO* info  :�w��ARCH���
//         CString& outPath :�ϊ���̃p�X
// �߂�l�Ftrue :�ϊ��K�v�@false:�ϊ��s�v
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::ChangeProgramFilesPath(CString path, 
							ARCH_INFO* info, CString& outPath)
{
	outPath = path;
	CString newPath = info->pFilePath;
	int findpos = 0;
	bool ret = false;

	findpos = path.Find(newPath+"\\");
	if (findpos==-1 && newPath==m_x86Path)
	{
		//64bit->32bit
		outPath.Replace(m_x64Path, m_x86Path);
		
		//%OpenCV_DIR%���܂܂�Ă��邩�H
		findpos = path.Find("%OpenCV_DIR%");
		if (findpos != -1)
		{
			//64bit->32bit
			outPath.Replace("%OpenCV_DIR%x64", "%OpenCV_DIR%x86");
		}
		ret = true;
	}
	if (findpos==-1 && newPath==m_x64Path)
	{
		//32bit->64bit
		outPath.Replace(m_x86Path, m_x64Path);

		//%OpenCV_DIR%���܂܂�Ă��邩�H
		findpos = path.Find("%OpenCV_DIR%");
		if (findpos != -1)
		{
			//32bit->64bit
			outPath.Replace("%OpenCV_DIR%x86", "%OpenCV_DIR%x64");
		}
		ret = true;
	}
	TRACE("ChangeProgramFilesPath : inPath=%s : outPath=%s\n", path, outPath);
	return ret;
}

////////////////////////////////////////////////////////
// �֐����FDeleteUnnecessaryPath
// �@�\�@�FARCH_INFO�\���̂Ŏw�肳�ꂽ�^�C�v(32bit/64bit)
//         �ȊO�̃p�X�����O����
// �����@�Fpath�@  : �����Ώۃp�X
//         delimit : ��؂蕶��
//         info�@�@: ARCH_INFO�\���́@
// �߂�l�F������̃p�X
////////////////////////////////////////////////////////
CString CVCVerChangerDlg::DeleteUnnecessaryPath(
			CString path, CString delimit, ARCH_INFO* info)
{
   CString substr, basisPath, cvarch;
   int pos = 0;
   int findpos = 0;
   CString outPath = "";

   //�����֓n�����path�́A���W�X�g���֏����߂����̂�
   //%�ϐ����W�J����Ă��Ȃ�
   TRACE("DeleteUnnecessaryPath : in(%s)\n", path);

   basisPath.Format("%s\\OpenRTM-aist", info->pFilePath);
   cvarch = "%OpenCV_DIR%" + info->CV_ARCH;

   //�󂯎����path���w��̋�؂蕶���ŕ�������
   substr = path.Tokenize(delimit, pos);
   while (substr != "")
   {
	   //�w��p�^�[�����܂ރp�X���������o��
	   findpos = substr.Find(basisPath);
	   if (findpos != -1)
	   {
		   outPath += substr + delimit;
	   }

	   findpos = substr.Find(cvarch);
	   if (findpos != -1)
	   {
		   outPath += substr + delimit;
	   }

	    findpos = substr.Find("%OMNI_ROOT%");
	   if (findpos != -1)
	   {
		   outPath += substr + delimit;
	   }
	   substr = path.Tokenize(delimit, pos);
   }
   TRACE("DeleteUnnecessaryPath : out(%s)\n", outPath);
   return outPath;
}

////////////////////////////////////////////////////////
// �֐����FOnDestroy
// �@�\�@�F�_�C�A���O�����ۂ̏���
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete [] m_VSInfo;
	delete [] m_archInfo;
	m_VSInfo = NULL;
	m_archInfo = NULL;
}
