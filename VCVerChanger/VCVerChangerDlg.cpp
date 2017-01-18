/////////////////////////////////////////////////////////////////////////////
// Name			: VCVerChangerDlg.cpp
// Description	: implementation of the CVCVerChangerDlg class
//
// To check and change the system environment variables of OpenRTM-aist.
//
// Create Date	: 2016.10.11
// Author		: Nobu Kawauchi
// Copyright (C) 2016 n-ando AIST All Rights Reserved.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxdialogex.h"
#include "VCVerChanger.h"
#include "VCVerChangerDlg.h"

#include "atlbase.h"

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
	InitMultilingual();
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
// �֐����FInitMultilingual
// �@�\�@�FPC�̌���ݒ�Ń{�^���\����������
// �@�\�@�F����ݒ�ɍ�����String Table�̐ݒ���g��
//   ���{��Ɖp��̐ݒ�̂ǂ�����g�����͎����Ŕ��f�����         
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::InitMultilingual()
{
	CString strText;
	CButton *button;

	strText.LoadString(IDS_BTN_UPDATE); 
	button = (CButton*)GetDlgItem(IDC_UPDATE);
	button->SetWindowText(strText);

	strText.LoadString(IDS_BTN_CHANGE); 
	button = (CButton*)GetDlgItem(IDC_ARCH_CHANGE);
	button->SetWindowText(strText);

	strText.LoadString(IDS_BTN_CHECK);
	button = (CButton*)GetDlgItem(ID_CHECK);
	button->SetWindowText(strText);

	strText.LoadString(IDS_BTN_END);
	button = (CButton*)GetDlgItem(IDCLOSE);
	button->SetWindowText(strText);

	strText.LoadString(IDS_DLG_TITLE);
	SetWindowText(strText);
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
	CString strText;

	GetDlgItem(IDC_UPDATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_VS_VER)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ARCH)->ShowWindow(FALSE);
	GetDlgItem(IDC_ARCH)->ShowWindow(FALSE);
	GetDlgItem(IDC_ARCH_CHANGE)->ShowWindow(FALSE);

	VISUAL_STUDIO vc_ver[] = {
		{"Visual C++ 17", "vc17"},
		{"Visual C++ 16", "vc16"},
		{"Visual C++ 15 2017", "vc15"},
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

	strText.LoadString(IDS_WARNING_1);
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(strText);
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

	TRACE("OnBnClickedCheck : �m�F�{�^������\n");
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("");

	m_RegistryUtil.RegOpen();

	//���W�X�g���ǂݏo��
	bRet = RegistryReadProc();
	if (!bRet)
	{
		m_RegistryUtil.RegClose();
		return;
	}

	//PATH��OpenRTM-aist��32bit/64bit�̗���`�����݂��邩�H
	bRet = PathDoubleDefinitionCheck(m_RtmPath);
	if(bRet)
	{
		TRACE("OnBnClickedCheck : OpenRTM-aist��PATH��32bit/64bit������`����Ă���\n");
		str.LoadString(IDS_WARNING_2);
		GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
		m_32b64bDualDefineFlg = true;
	}

	//�{�c�[������x���s����ƁAPATH�̒�`�͐����ς݂�
	//����̃o�[�W�����������o�ł��Ȃ��Ȃ�


	//���݊��ϐ��������Ă���p�X�̑��݊m�F
	if (!(::PathFileExistsA(m_OpenrtmDir) && ::PathIsDirectoryA(m_OpenrtmDir)))
	{
		//���݂��Ȃ��ꏊ���w���Ă���̂ŁA�ݒ�𐳂�
		CorrectSystemEnvValue();
        
		//���W�X�g���ǂݏo��
		bRet = RegistryReadProc();
		if (!bRet)
		{
			m_RegistryUtil.RegClose();
			return;
		}
	}

	m_RegistryUtil.RegClose();

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
// �֐����FFindStringFromTargetPath
// �@�\�@�Ftarget�̒�����w�蕶������܂ރp�X�𔲂��o��
//    �Y���̃p�X����������ꍇ�A��؂蕶����"\n"��";"��
//�@�@�Q�̃p�^�[����Ԃ��B(out1��out2�j
//�@�@target����w�蕶��������O�����c���out3�Ƃ��ĕԂ�
// ����
//   :in1  :str       :�w�蕶����
//   :in2  :target    :�Ώ�PATH
//   :out1 :outForGUI :��؂蕶��"\n"�iGUI�\���p�j
//   :out2 :outForReg :��؂蕶��";"�i���W�X�g�������p�j
//   :out3 :otherPath :target�̎w�蕶����ȊO�̕���
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::FindStringFromTargetPath(
				CString str,
				CString target,
				CString& outForGUI, 
				CString& outForReg,
				CString& otherPath)
{
	CString replaceStr, orgStr;
	int pos = 0;
	int findpos = 0;

	//�󂯎����target��path��";"�ŕ�������
	orgStr = target.Tokenize(";", pos);
	while (orgStr != "")
	{
		if (str == "OpenRTM-aist")
		{
			//%%�ϐ���W�J����
			replaceStr = ReplaceEnv(orgStr);
		}else{
			//%%�ϐ���W�J���Ȃ�
			replaceStr = orgStr;
		}
		//�w�蕶������܂ރp�X���������o��
		findpos = replaceStr.Find(str);
		if (findpos != -1)
		{
			//GUI�ł͕����̃p�X�͉��s�\��������
			outForGUI = outForGUI + replaceStr + "\n";
			//���W�X�g�������߂��p��%%�ϐ����p�A;�ŋ�؂�
			outForReg = outForReg + orgStr + ";";

		}else{
			//OpenRTM-aist�ȊO�̃p�X��otherPath�ɕۑ����Ă���
			//���W�X�g���ւ̏������ݎ��ɍ��킹�ď����߂�
			otherPath = otherPath + orgStr + ";";
		}
		orgStr = target.Tokenize(";", pos);
	}
	TRACE("FindStringFromTargetPath : otherPath : %s\n", otherPath);
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
	binPath.Format("%sbin\\%s", LPCTSTR(m_RtmRoot), LPCTSTR(m_VcVersion));

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
		TRACE("PathDoubleDefinitionCheck : OpenRTM-aist��32bit/64bit�̗����̃p�X�����݂���\n");
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
	TRACE("OnBnClickedClose : �I���{�^������\n");
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
	bool ret;

	TRACE("OnBnClickedUpdate : �X�V�{�^������\n");
	m_StatusMsg.LoadString(IDS_STATUS_1);
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

	m_RegistryUtil.RegOpen();

	//���W�X�g���G���g���@��������
	ret = m_RegistryUtil.WriteEnv("RTM_VC_VERSION", REG_SZ, m_VcVersion);	
	if (!ret)
	{
		RegistryEntryErr("RTM_VC_VERSIN", "Write");
		m_RegistryUtil.RegClose();
		return;
	}

	//WM_SETTINGCHANGE
	SettingChange();

	//����������̃��W�X�g����ǂݍ���
	ret = RegistryReadProc();
	if (!ret)
	{
		m_RegistryUtil.RegClose();
		return;
	}

	m_RegistryUtil.RegClose();

	Msg1.LoadString(IDS_STATUS_2);
	Msg2.LoadString(IDS_STATUS_3);
	Msg3.Format("%s%s%s", LPCTSTR(Msg1), LPCTSTR(m_VcVersion), LPCTSTR(Msg2));
	m_StatusMsg = Msg3;
  
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// �֐����FOnCtlColor
// �@�\�@�F�X�^�e�B�b�N�e�L�X�g�̕����F��Ԃɂ��邽��
// �@�@�@�@�{�֐����I�[�o�[���C�h����
////////////////////////////////////////////////////////
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
	TRACE("SettingChange : WM_SETTINGCHANGE���b�Z�[�W���M\n");
	LRESULT Ret = SendMessageTimeout(
		HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)("Environment"),
		SMTO_ABORTIFHUNG, 5000, &dwReturnValue);
	if (!Ret)
	{
		TRACE("SettingChange error. (%d)\n", Ret);
	}
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
	m_StatusMsg.LoadString(IDS_STATUS_1);
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
	}
	
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("");

	m_RegistryUtil.RegOpen();

	//���W�X�g���X�V����
	ret = RegistryWriteProc(m_archInfo[i].pFilePath);
	if (!ret)
	{
		m_RegistryUtil.RegClose();
		return;
	}
	
	//WM_SETTINGCHANGE
	SettingChange();

	//����������̃��W�X�g����ǂݍ���
	ret = RegistryReadProc();
	if (!ret)
	{
		m_RegistryUtil.RegClose();
		return;
	}

	m_RegistryUtil.RegClose();

	Msg1.LoadString(IDS_STATUS_4);
	Msg2.LoadString(IDS_STATUS_5);
	Msg3.Format("%s%s%s", LPCTSTR(Msg1), LPCTSTR(input), LPCTSTR(Msg2));
	m_StatusMsg = Msg3;
  
	UpdateData(FALSE);
}


////////////////////////////////////////////////////////
// �֐����FCorrectSystemEnvValue
// �@�\�@�F�V�X�e�����ϐ��̒l�����ۂ̃C���X�g�[����
//         �������ݒ�ɂȂ�悤�ɐ����B
//         OpenRTM-aist��32bit/64bit�ł̐ؑւɂ��
//         ���ϐ��̐ݒ肪����ƍ����Ă��Ȃ��ꍇ��
//         �Ή�����B
// �����@�F����
// �߂�l�FOpenRTM-aist�̃C���X�g�[�����  
//         true:�C���X�g�[�����Ă���
//         false:�A���C���X�g�[������Ă���
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::CorrectSystemEnvValue()
{
	CString archPath, outpath;
	int findpos = 0;
	bool result = true;
	CString target = m_OpenrtmDir;

	//���݂̐ݒ�ƈقȂ�A�[�L�e�N�`���̃p�X���擾
	findpos = target.Find(m_x86Path);
	if (findpos != -1)
	{
		//�p�X��32bit�ݒ�ɂȂ��Ă���
		//64bit�ł̃p�X���擾����
		ChangeProgramFilesPath(m_OpenrtmDir, m_x64Path, outpath);
		archPath = m_x64Path;
	}else{
		//�p�X��64bit�ݒ�ɂȂ��Ă���
		//32bit�ł̃p�X���擾����
		ChangeProgramFilesPath(m_OpenrtmDir, m_x86Path, outpath);
		archPath = m_x86Path;
	}

	//�f�B���N�g�����݂��m�F����
	if(::PathFileExistsA(outpath) && ::PathIsDirectoryA(outpath))
	{
		//���݂��Ă���̂Ńp�X������������
		TRACE("CorrectSystemEnvValue : �V�X�e�����ϐ��𐳂����ݒ�ŏ�����\n");
		RegistryWriteProc(archPath);
	}else{
		//OpenRTM-aist�̓A���C���X�g�[������Ă���Ɣ��f
		//���W�X�g������폜����
		TRACE("CorrectSystemEnvValue : OpenRTM-aist�̓A���C���X�g�[������Ă���\n");
		RegistryDeleteProc("ALL");
		CleanResult();
		result = false;
	}

	//WM_SETTINGCHANGE
	SettingChange();

	return result;
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

	str = m_OpenrtmDir;
	//32bit�Ŋm�F
	findpos = str.Find(m_x86Path);
	if (findpos != -1)
	{
		//�p�X��32bit�ݒ�ɂȂ��Ă���
		TRACE("InstalledArchCheck : �p�X��32bit�ݒ�\n");
		tmp = str;
		tmp.Replace(m_x86Path, m_x64Path);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			TRACE("InstalledArchCheck : �p�X��32bit�ݒ��64bit�ł��C���X�g�[���ς�\n");
			ret = 0;
		}						
	}else{
		//�p�X��64bit�ݒ�ɂȂ��Ă���
		TRACE("InstalledArchCheck : �p�X��64bit�ݒ�\n");
		tmp = str;
		tmp.Replace(m_x64Path, m_x86Path);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			TRACE("InstalledArchCheck : �p�X��64bit�ݒ��32bit�ł��C���X�g�[���ς�\n");
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
//         CString toDir    :���̃f�B���N�g���֕ϊ�����
//         CString& outPath :�ϊ���̃p�X
// �߂�l�Ftrue :�ϊ�����@false:�ϊ��Ȃ�
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::ChangeProgramFilesPath(CString path, 
							CString toDir, CString& outPath)
{
	outPath = path;
	int findpos = 0;
	bool ret = false;

	findpos = path.Find(toDir+"\\");
	if (findpos==-1 && toDir==m_x86Path)
	{
		//64bit->32bit
		TRACE("ChangeProgramFilesPath : 64bit->32bit�֐ؑ�\n");
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
	if (findpos==-1 && toDir==m_x64Path)
	{
		//32bit->64bit
		TRACE("ChangeProgramFilesPath : 32bit->64bit�֐ؑ�\n");
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

   TRACE("DeleteUnnecessaryPath : �w��ȊO�̃p�X�����O����\n");
   //�����֓n�����path�́A���W�X�g���֏����߂����̂�
   //%�ϐ����W�J����Ă��Ȃ�
   TRACE("DeleteUnnecessaryPath : in(%s)\n", path);

   basisPath.Format("%s\\OpenRTM-aist", LPCTSTR(info->pFilePath));
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
// �֐����FRegistryReadProc
// �@�\�@�F���W�X�g���G���g���Ǎ�����
// �����@�F����
// �߂�l�Ftrue:����I���@false:�G���g������`
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::RegistryReadProc()
{
	CString EnvValue = "";
	CString path, str;
	CString outForGUI, outForReg, otherPath;
	int i;

	TRACE("RegistryReadProc: ���W�X�g���G���g���Ǎ�����\n");

	EnvValue = m_RegistryUtil.ReadEnv("RTM_VC_VERSION");
	if (EnvValue == "")
	{
		RegistryEntryReadErr();
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

	EnvValue = m_RegistryUtil.ReadEnv("RTM_BASE");
	if (EnvValue == "")
	{
		//���W�X�g������폜����
		RegistryDeleteProc("RTM_VC_VERSION");
		CleanResult();
		RegistryEntryReadErr();
		return false;
	}
	m_RtmBase = EnvValue;

	EnvValue = m_RegistryUtil.ReadEnv("RTM_ROOT");
	if (EnvValue == "")
	{
		RegistryEntryErr("RTM_ROOT", "Read");
		return false;
	}
	m_RtmRoot = EnvValue;

	EnvValue = m_RegistryUtil.ReadEnv("RTM_JAVA_ROOT");
	if (EnvValue == "")
	{
		RegistryEntryErr("RTM_JAVA_ROOT", "Read");
		return false;
	}
	m_RtmJavaRoot = EnvValue;

	EnvValue = m_RegistryUtil.ReadEnv("OMNI_ROOT");
	if (EnvValue == "")
	{
		RegistryEntryErr("OMNI_ROOT", "Read");
		return false;
	}
	//���W�X�g�������߂��p
	m_OmniRootForReg = EnvValue; 
	//GUI�\���p��%OMNI_ROOT%��W�J������
	m_OmniRoot = ReplaceEnv(EnvValue);

	EnvValue = m_RegistryUtil.ReadEnv("OpenCV_DIR");
	if (EnvValue == "")
	{
		RegistryEntryErr("OpenCV_DIR", "Read");
		return false;
	}
	m_OpencvDir = EnvValue;

	EnvValue = m_RegistryUtil.ReadEnv("OpenRTM_DIR");
	if (EnvValue == "")
	{
		RegistryEntryErr("OpenRTM_DIR", "Read");
		return false;
	}
	m_OpenrtmDir = EnvValue;

	EnvValue = m_RegistryUtil.ReadEnv("PATH");
	if (EnvValue == "")
	{
		RegistryEntryErr("PATH", "Read");
		return false;
	}

	//PATH����OpenRTM-aist���ݒ肵���p�X��؂�o��	
	outForGUI = outForReg = otherPath = "";
	FindStringFromTargetPath("OpenRTM-aist", EnvValue,
						outForGUI, outForReg, otherPath); 
	m_RtmPath = outForGUI;
	m_RtmPathForReg = outForReg;
	m_otherPath = otherPath;

	return true;
}

////////////////////////////////////////////////////////
// �֐����FReplaceEnv
// �@�\�@�F�V�X�e�����ϐ��l�Ɋ܂܂��%�ϐ�%��W�J����
//    RegQueryValueEx�̓��W�X�g���l�Ɋ܂܂��%%�ϐ���
//    �W�J���Ă���Ȃ����߁A�W�J������
// �����@�FCString Path�@�F�W�J�O�̃p�X
// �߂�l�F�W�J��̃p�X
////////////////////////////////////////////////////////
CString  CVCVerChangerDlg::ReplaceEnv(CString Path)
{
	CString str1, str2, str3, src;
    int VER_len, OMNI_len, CV_len; 
    
    CString tmp = "%RTM_VC_VERSION%";
	VER_len = tmp.GetLength();
	tmp = "%OMNI_ROOT%";
	OMNI_len = tmp.GetLength();
	tmp = "%OpenCV_DIR%";
	CV_len = tmp.GetLength();

	src = Path;
	int ptr = src.Find("%RTM_VC_VERSION%");
	if (ptr != -1)
	{
		str1 = src.Mid(0, ptr);
		str2 = src.Mid(ptr + VER_len);
		str3.Format("%s%s%s", LPCTSTR(str1), 
						LPCTSTR(m_VcVersion), LPCTSTR(str2));
		src = str3;
		TRACE("ReplaceEnv : %s\n", src);
	}

	ptr = src.Find("%OMNI_ROOT%");
	if (ptr != -1)
	{
		str1 = src.Mid(0, ptr);
		str2 = src.Mid(ptr + OMNI_len);
		str3.Format("%s%s%s", LPCTSTR(str1), 
						LPCTSTR(m_OmniRoot), LPCTSTR(str2));
		src = str3;
		TRACE("ReplaceEnv : %s\n", src);
	}

	ptr = src.Find("%OpenCV_DIR%");
	if (ptr != -1)
	{
		str1 = src.Mid(0, ptr);
		str2 = src.Mid(ptr + CV_len);
		str3.Format("%s%s%s", LPCTSTR(str1), 
						LPCTSTR(m_OpencvDir), LPCTSTR(str2));
		src = str3;
		TRACE("ReplaceEnv : %s\n", src);
	}	
	return src;
}

////////////////////////////////////////////////////////
// �֐����FRegistryWriteProc
// �@�\�@�F���W�X�g���X�V����
// �����@�F����
// �߂�l�F����  
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::RegistryWriteProc(CString toDir)
{
	CString outpath, allpath;
	bool ret, ret1;

	ret = ChangeProgramFilesPath(m_RtmBase, toDir, outpath);
	if (ret)
	{
		//���W�X�g���̏��������K�v
		TRACE("RegistryWriteProc : ���W�X�g���X�V����\n");
		ret1 = m_RegistryUtil.WriteEnv("RTM_BASE", REG_SZ, outpath);
		if (!ret1)
		{
			RegistryEntryErr("RTM_BASE", "Write");
			return false;
		}

		ChangeProgramFilesPath(m_RtmRoot, toDir, outpath);
		ret1 = m_RegistryUtil.WriteEnv("RTM_ROOT", REG_SZ, outpath);
		if (!ret1)
		{
			RegistryEntryErr("RTM_ROOT", "Write");
			return false;
		}
	
		ChangeProgramFilesPath(m_RtmJavaRoot, toDir, outpath);
		ret1 = m_RegistryUtil.WriteEnv("RTM_JAVA_ROOT", REG_SZ, outpath);
		if (!ret1)
		{
			RegistryEntryErr("RTM_JAVA_ROOT", "Write");
			return false;
		}

		ChangeProgramFilesPath(m_OmniRootForReg, toDir, outpath);
		ret1 = m_RegistryUtil.WriteEnv("OMNI_ROOT", REG_EXPAND_SZ, outpath);
		if (!ret1)
		{
			RegistryEntryErr("OMNI_ROOT", "Write");
			return false;
		}

		ChangeProgramFilesPath(m_OpencvDir, toDir, outpath);
		ret1 = m_RegistryUtil.WriteEnv("OpenCV_DIR", REG_SZ, outpath);
		if (!ret1)
		{
			RegistryEntryErr("OpenCV_DIR", "Write");
			return false;
		}

		ChangeProgramFilesPath(m_OpenrtmDir, toDir, outpath);
		ret1 = m_RegistryUtil.WriteEnv("OpenRTM_DIR", REG_SZ, outpath);
		if (!ret1)
		{
			RegistryEntryErr("OpenRTM_DIR", "Write");
			return false;
		}
	}

	ret = ChangeProgramFilesPath(m_RtmPathForReg, toDir, outpath);
	if (ret || m_32b64bDualDefineFlg)
	{
		TRACE("RegistryWriteProc : PATH�X�V\n");
		allpath = m_otherPath + outpath;
		ret1 = m_RegistryUtil.WriteEnv("PATH", REG_EXPAND_SZ, allpath);
		if (!ret1)
		{
			RegistryEntryErr("PATH", "Write");
			return false;
		}
		m_32b64bDualDefineFlg = false;
	}

	return true;
}

////////////////////////////////////////////////////////
// �֐����FRegistryDeleteProc
// �@�\�@�F���W�X�g���G���g���폜����
// �����@�FCString target :�ΏۃG���g��
//                         ALL���w�肷��ΑS�폜
// �߂�l�Ftrue:����I���@false:�G���g������`
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::RegistryDeleteProc(CString target)
{
	CString EnvValue = "";
	CString outForGUI, outForReg, otherPath;
	bool ret;

    TRACE("RegistryDeleteProc : ���W�X�g���G���g���폜����\n");
    
    ret = m_RegistryUtil.DeleteEnv("RTM_VC_VERSION");
	if (!ret)
	{
		RegistryEntryErr("RTM_VC_VERSION", "Delete");
		return false;
	}

	//�A���C���X�g�[�����Ă��c���Ă��܂��V�X�e�����ϐ��폜
	//�{�c�[���Œl�������������̂��폜���ꂸ�c���Ă��܂�
	//OpenRTM-aist��32/64bit�ł̂����ꂩ�̂݃C���X�g�[�����Ă���
	//�ꍇ�́ARTM_VC_VERSION���c��\��������
	//32/64bit�ł̗������C���X�g�[�����Ă���ꍇ�A�{�c�[���Őؑւ�
	//���s����ƑS�Ă̊��ϐ����c��\��������̂ŁA������ALL��
	//�w�肵�Ď��s���Ă���

	if (target == "ALL")
	{
		ret = m_RegistryUtil.DeleteEnv("RTM_BASE");
		if (!ret)
		{
			RegistryEntryErr("RTM_BASE", "Delete");
			return false;
		}
		ret = m_RegistryUtil.DeleteEnv("RTM_ROOT");
		if (!ret)
		{
			RegistryEntryErr("RTM_ROOT", "Delete");
			return false;
		}
		ret = m_RegistryUtil.DeleteEnv("RTM_JAVA_ROOT");
		if (!ret)
		{
			RegistryEntryErr("RTM_JAVA_ROOT", "Delete");
			return false;
		}
		ret = m_RegistryUtil.DeleteEnv("OMNI_ROOT");
		if (!ret)
		{
			RegistryEntryErr("OMNI_ROOT", "Delete");
			return false;
		}
		ret = m_RegistryUtil.DeleteEnv("OpenCV_DIR");
		if (!ret)
		{
			RegistryEntryErr("OpenCV_DIR", "Delete");
			return false;
		}
		ret = m_RegistryUtil.DeleteEnv("OpenRTM_DIR");
		if (!ret)
		{
			RegistryEntryErr("OpenRTM_DIR", "Delete");
			return false;
		}

		//PATH����OpenRTM-aist�𔲂��o�����c��������߂�
		EnvValue = m_RegistryUtil.ReadEnv("PATH");
		if (EnvValue == "")
		{
			RegistryEntryErr("PATH", "Read");
			return false;
		}
		//PATH��OpenRTM-aist���ݒ肵���p�X��؂�o��	
		outForGUI = outForReg = otherPath = "";
		FindStringFromTargetPath("OpenRTM-aist", EnvValue,
							outForGUI, outForReg, otherPath);
		if (outForReg != "")
		{
			//PATH�̒���OpenRTM-aist�̃p�X���c���Ă���̂ŁA�����
			//���������̂������߂�
			ret = m_RegistryUtil.WriteEnv("PATH", REG_EXPAND_SZ, otherPath);
			if (!ret)
			{
				RegistryEntryErr("PATH", "Write");
				return false;
			}
		}
	}
	return true;
}

////////////////////////////////////////////////////////
// �֐����FRegistryEntryReadErr
// �@�\�@�F���W�X�g���G���g���ǂݍ��݃G���[���̏���
//         OpenRTM-aist���C���X�g�[������Ă��Ȃ��\��
//         �������ꍇ�ɕ\�����郁�b�Z�[�W
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegistryEntryReadErr()
{
	CString str;

	TRACE("RegistryEntryReadErr : OpenRTM-aist�C���X�g�[���m�F���b�Z�[�W�\��\n");
	str.LoadStringA(IDS_WARNING_3);
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// �֐����FRegistryEntryErr
// �@�\�@�F���W�X�g���G���g��Read/Write�G���[���̏���
// �����@�FLPCTSTR EnvName�@�F�G���g����
//         LPCTSTR proc :����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegistryEntryErr(LPCTSTR EnvName, LPCTSTR proc)
{
	CString str;

	str.Format("%s : %s Error", EnvName, proc);
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
	UpdateData(FALSE);
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

	TRACE("OnDestroy\n");
	delete [] m_VSInfo;
	delete [] m_archInfo;
	m_VSInfo = NULL;
	m_archInfo = NULL;
}

////////////////////////////////////////////////////////
// �֐����FCleanResult
// �@�\�@�F�_�C�A���O��Result�\��������
// �����@�F����
// �߂�l�F����
////////////////////////////////////////////////////////
void CVCVerChangerDlg::CleanResult()
{
	TRACE("CleanResult : �_�C�A���O��Result�\��������\n");
	m_VcVersion = "";
	m_RtmBase = "";
	m_RtmRoot = "";
	m_RtmJavaRoot = "";
	m_OmniRoot = "";
	m_OpencvDir = "";
	m_OpenrtmDir = "";
	m_RtmPath = "";
	UpdateData(FALSE);
}