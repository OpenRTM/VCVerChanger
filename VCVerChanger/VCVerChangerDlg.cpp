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


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CVCVerChangerDlg ダイアログ



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


// CVCVerChangerDlg メッセージ ハンドラー

BOOL CVCVerChangerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	Init();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CVCVerChangerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CVCVerChangerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

////////////////////////////////////////////////////////
// 関数名：Init
// 機能　：初期処理
// 引数　：無し
// 戻り値：無し
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
		
		//コンボボックス表示
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

		//コンボボックス表示
		m_arch.InsertString(-1, m_archInfo[i].BitType);
	}

	//32bit、64bitの両バージョンインストール状態
	//両方インストールしているか、否か
	//true:してる　false:してない
	m_32b64bInstFlg = false;

	//PATH中のOpenRTM-aist 32bit/64bit両定義状態
	//true:両方のパスが存在する　flase:存在しない
	m_32b64bDualDefineFlg = false;

	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("確認ボタンを押して下さい。");
}

////////////////////////////////////////////////////////
// 関数名：RegOpen
// 機能　：レジストリキーのオープン
// 引数　：無し
// 戻り値：無し
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
// 関数名：RegClose
// 機能　：レジストリキーのクローズ
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegClose()
{
	RegCloseKey(m_hkey);
	m_hkey = 0;
}

////////////////////////////////////////////////////////
// 関数名：OnBnClickedCheck
// 機能　：確認ボタン処理
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnBnClickedCheck()
{
	CString path, str;
	int ret;
	bool bRet;

	TRACE("%s OnBnClickedCheck : 確認ボタン処理\n", GetTime());
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("");

	//レジストリ読み出し
	bRet = RegistryReadProc();
	if (!bRet) return;

	//PATHにOpenRTM-aistの32bit/64bitの両定義が存在するか？
	bRet = PathDoubleDefinitionCheck(m_RtmPath);
	if(bRet)
	{		
		str = "OpenRTM-aistのPATHが32bit/64bit両方定義されています。切替ボタンでどちらかに設定して下さい。";
		GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
		m_32b64bDualDefineFlg = true;
	}

	//本ツールを一度実行すると、PATHの定義は整理済みで
	//一方のバージョンしか検出できなくなる
	//ディレクトリの存在確認で両バージョンのインストール
	//状態をチェックする
	ret = InstalledArchCheck();
	if (ret != -1)
	{
		//両バージョンがインストールされている
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
// 関数名：RegistryReadProc
// 機能　：レジストリ読み込み処理
// 引数　：無し
// 戻り値：true:正常終了　false:エントリ未定義
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
	//レジストリ書き戻し用
	m_OmniRootForReg = EnvValue; 
	//GUI表示用は%OMNI_ROOT%を展開させる
	m_OmniRoot = ReplaceEnv(EnvValue);

	EnvValue = ReadEnv("OpenCV_DIR");
	if (EnvValue == "") return false;
	m_OpencvDir = EnvValue;

	EnvValue = ReadEnv("OpenRTM_DIR");
	if (EnvValue == "") return false;
	m_OpenrtmDir = EnvValue;

	EnvValue = ReadEnv("Path");
	if (EnvValue == "") return false;
	//読み込んだ状態をバックアップしておく
	m_orgPATH = EnvValue;
	TRACE("RegistryReadProc : m_orgPATH : %s\n", m_orgPATH);
	
	//OpenRTM-aistが設定したパスを切り出す
	CString str1, str2, str3;	
	GetOpenrtmPath(EnvValue, str1, str2, str3); 
	m_RtmPath = str1;
	m_RtmPathForReg = str2;
	m_otherPath = str3;

	RegClose();
	return true;
}

////////////////////////////////////////////////////////
// 関数名：RegistryEntryReadErr
// 機能　：レジストリエントリ読み込みエラー時の処理
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegistryEntryReadErr()
{
	CString str;

	str = "OpenRTM-aist 1.1.2版以降のバージョンがインストールされているか、確認して下さい。";
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// 関数名：ReadEnv
// 機能　：レジストリエントリ読み込み
// 引数　：LPCTSTR EnvName　：エントリ名
// 戻り値：エントリ値
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
// 関数名：WriteEnv
// 機能　：レジストリエントリ書き込み
// 引数　：LPCTSTR EnvName　：エントリ名
// 戻り値：エントリ値
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
// 関数名：ReplaceEnv
// 機能　：システム環境変数値に含まれる%%変数を展開する
//    RegQueryValueExはレジストリ値に含まれる%%変数を
//    展開してくれないため、展開させる
// 引数　：CString Path　：展開前のパス
// 戻り値：展開後のパス
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
// 関数名：GetOpenrtmPath
// 機能　：PATHの中から「OpenRTM-aist」を含むものを抜き出す
//   「OpenRTM-aist」のパスは区切り文字を"\n"と";"の２つの
//　　パターンを返す。(out1とout2）
//　　PATHから「OpenRTM-aist」のパスを除いたものをout3として
//    返す。
// 引数　：in :Path　：PATHの値
//    :out1 :outForGUI :区切り文字"\n"（GUI表示用）
//    :out2 :outForReg :区切り文字";"（レジストリ書込用）
//    :out3 :otherPath :レジストリのPath書き戻し時に使用
// 戻り値：無し
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

	//受け取ったpathを";"で分割する
	substr = Path.Tokenize(";", pos);
	while (substr != "")
	{
		//%%変数を展開する
		string = ReplaceEnv(substr);
		//OpenRTM-aistを含むパスだけ抜き出す
		findpos = string.Find("OpenRTM-aist");
		if (findpos != -1)
		{
			TRACE("GetOpenrtmPath : string : %s\n", string);
			//GUIでは複数のパスは改行表示させる
			outForGUI = outForGUI + string + "\n";
			//レジストリ書き戻し用は%%変数利用、;で区切る
			outForReg = outForReg + substr + ";";

		}else{
			//OpenRTM-aist以外のパスはotherPathに保存しておき
			//レジストリへの書き込み時に合わせて書き戻す
			otherPath = otherPath + substr + ";";
		}
		substr = Path.Tokenize(";", pos);
	}
	TRACE("GetOpenrtmPath : otherPath : %s\n", otherPath);
}

////////////////////////////////////////////////////////
// 関数名：PathDoubleDefinitionCheck
// 機能　：PATHにOpenRTM-aistの32bit/64bitの両定義が存在
//         しているかチェックする
// 引数　：CString Path　：PATHの値
// 戻り値：true: 32/64bitの両定義がある　　false:ない
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::PathDoubleDefinitionCheck(CString Path)
{
	bool ret = false;
	int findpos = 0;
	CString binPath;

	//32bit/64bitの両バージョンをインストール後、
	//システム環境変数をいじっていなければ、PATHに下記の
	//定義が存在する
	//C:\Program Files\OpenRTM-aist\1.1.2\bin\%RTM_VC_VERSION%
	//C:\Program Files (x86)\OpenRTM-aist\1.1.2\bin\%RTM_VC_VERSION%
	//この状態かどうかをチェックする

	//%RTM_ROOT%bin\%RTM_VC_VERSION%のパスを生成
	binPath.Format("%sbin\\%s", m_RtmRoot, m_VcVersion);

	//指定のパスに含まれるProgram Filesを切り替える
	//"Program Files (x86)" -> "Program Files"
	//"Program Files" -> "Program Files (x86)"
	findpos = binPath.Find(m_x86Path);
	if (findpos != -1)
	{
		binPath.Replace(m_x86Path, m_x64Path);
	}else{
		binPath.Replace(m_x64Path, m_x86Path);
	}

	//binPathはPATHに定義されているか？
	findpos = Path.Find(binPath);
	if (findpos != -1)
	{
		ret = true;
	}
	return ret;
}

////////////////////////////////////////////////////////
// 関数名：OnBnClickedClose
// 機能　：終了ボタン処理
// 引数　：無し
// 戻り値：無し
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
// 関数名：OnBnClickedUpdate
// 機能　：更新ボタン処理
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnBnClickedUpdate()
{
	CString Msg1, Msg2, Msg3, input;
	int i;

	TRACE("%s OnBnClickedUpdate : 更新ボタン処理\n", GetTime());
	m_StatusMsg = "処理中...";
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
	TRACE("OnBnClickedUpdate : Combo Boxの入力値 : m_VcVersion : %s\n", m_VcVersion);

	RegOpen();

	//レジストリエントリ　書き込み
	WriteEnv("RTM_VC_VERSION", REG_SZ, m_VcVersion);

	RegClose();

	//WM_SETTINGCHANGE
	SettingChange();

	//Sleep(300);

	//書き換え後のレジストリを読み込む
	//OnBnClickedCheck();
	RegistryReadProc();
	  
	Msg1 = "更新後の確認完了。OMNI_ROOT, PATHに含まれるRTM_VC_VERSIONの値が「 ";
	Msg2 = " 」になりました。";
	Msg3.Format("%s%s%s", Msg1, m_VcVersion, Msg2);
	m_StatusMsg = Msg3;
  
	UpdateData(FALSE);
}


HBRUSH CVCVerChangerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// スタティックテキストの色変更
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (*pWnd == m_WarningMsg)
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
	}		

	// TODO: 既定値を使用したくない場合は別のブラシを返します。
	return hbr;
}


////////////////////////////////////////////////////////
// 関数名：SettingChange
// 機能　：WM_SETTINGCHANGEメッセージ送信
// 引数　：無し
// 戻り値：無し
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
// 関数名：OnClickedArchChange
// 機能　：切替ボタン処理
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnClickedArchChange()
{
	int i, index = 0;
	CString input, outpath, Msg1, Msg2, Msg3;
	CString str;
	bool ret;

	TRACE("OnClickedArchChange : 切替ボタン処理\n");
	m_StatusMsg = "処理中...";
	UpdateData(FALSE);
	GetDlgItem(IDC_ARCH)->GetWindowTextA(input);
	TRACE("OnClickedArchChange : Combo Boxの入力値 : %s\n", input);

	for (i=0; i<m_arch.GetCount(); i++)
	{
		if (input == m_archInfo[i].BitType)
		{
			break;
		}
	}

	//Pathの設定に32bit, 64bit用のパスが混在していれば
	//指定以外のパスは除外する
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
		//レジストリの書換えが必要
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

	//書き換え後のレジストリを読み込む
	RegistryReadProc();

	Msg1 = "切替後の確認完了。「 ";
	Msg2 = " 」用の設定になりました。";
	Msg3.Format("%s%s%s", Msg1, input, Msg2);
	m_StatusMsg = Msg3;
  
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// 関数名：InstalledArchCheck
// 機能　：OpenRTM-aistのアーキテクチャを調べる
//         32bit/64bit版の両方がインストールされていたら
//         切替可能とする判断に利用
// 引数　：無し
// 戻り値：-1:どちらかだけ  
//          0:両方インストール済みで現在の設定は32bit
//          1:両方インストール済みで現在の設定は64bit
////////////////////////////////////////////////////////
int CVCVerChangerDlg::InstalledArchCheck()
{
	CString str, tmp;	
	int findpos = 0;
	int ret = -1;

	//OpenRTM_DIRのディレクトリ存在確認で
	//インストール状態を判断する
	str = m_OpenrtmDir;
	TRACE("str : %s\n", str);
	//32bit版確認
	findpos = str.Find(m_x86Path);
	if (findpos != -1)
	{
		//パスは32bit設定になっている		
		tmp = str;
		tmp.Replace(m_x86Path, m_x64Path);
		TRACE("tmp : %s\n", tmp);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			//64bit版が存在している
			ret = 0;
		}						
	}else{
		//パスは64bit設定になっている
		tmp = str;
		tmp.Replace(m_x64Path, m_x86Path);
		TRACE("tmp : %s\n", tmp);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			//32bit版が存在している
			ret = 1;
		}
	}
	return ret;
}

////////////////////////////////////////////////////////
// 関数名：ChangeProgramFilesPath
// 機能　：指定のパスに含まれるProgram Filesを切り替える
//         "Program Files (x86)" -> "Program Files"
//         "Program Files" -> "Program Files (x86)"
// 引数　：CString path     :対象のパス
//         ARCH_INFO* info  :指定ARCH情報
//         CString& outPath :変換後のパス
// 戻り値：true :変換必要　false:変換不要
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
		
		//%OpenCV_DIR%が含まれているか？
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

		//%OpenCV_DIR%が含まれているか？
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
// 関数名：DeleteUnnecessaryPath
// 機能　：ARCH_INFO構造体で指定されたタイプ(32bit/64bit)
//         以外のパスを除外する
// 引数　：path　  : 調査対象パス
//         delimit : 区切り文字
//         info　　: ARCH_INFO構造体　
// 戻り値：整理後のパス
////////////////////////////////////////////////////////
CString CVCVerChangerDlg::DeleteUnnecessaryPath(
			CString path, CString delimit, ARCH_INFO* info)
{
   CString substr, basisPath, cvarch;
   int pos = 0;
   int findpos = 0;
   CString outPath = "";

   //ここへ渡されるpathは、レジストリへ書き戻すもので
   //%変数が展開されていない
   TRACE("DeleteUnnecessaryPath : in(%s)\n", path);

   basisPath.Format("%s\\OpenRTM-aist", info->pFilePath);
   cvarch = "%OpenCV_DIR%" + info->CV_ARCH;

   //受け取ったpathを指定の区切り文字で分割する
   substr = path.Tokenize(delimit, pos);
   while (substr != "")
   {
	   //指定パターンを含むパスだけ抜き出す
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
// 関数名：OnDestroy
// 機能　：ダイアログを閉じる際の処理
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete [] m_VSInfo;
	delete [] m_archInfo;
	m_VSInfo = NULL;
	m_archInfo = NULL;
}
