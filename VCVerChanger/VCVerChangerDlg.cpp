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
	, m_LogPath(_T(""))
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
	DDX_Text(pDX, IDC_LOG_PATH, m_LogPath);
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

	//ダイアログを最上位に表示する
	SetWindowPos(&wndNoTopMost , 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	InitMultilingual();
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
// 関数名：InitMultilingual
// 機能　：PCの言語設定でボタン表示を初期化
// 機能　：言語設定に合ったString Tableの設定を使う
//   日本語と英語の設定のどちらを使うかは自動で判断される         
// 引数　：無し
// 戻り値：無し
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
// 関数名：Init
// 機能　：初期処理
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::Init()
{
	int i;
	CString strText;
	char	path[MAX_PATH];

	GetDlgItem(IDC_UPDATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_VS_VER)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_ARCH)->ShowWindow(FALSE);
	GetDlgItem(IDC_ARCH)->ShowWindow(FALSE);
	GetDlgItem(IDC_ARCH_CHANGE)->ShowWindow(FALSE);

	VISUAL_STUDIO vc_ver[] = {
		{"Visual Studio 17", "vc17"},
		{"Visual Studio 16 2019", "vc14"},
		{"Visual Studio 15 2017", "vc14"},
		{"Visual Studio 14 2015", "vc14"},
		{"Visual Studio 12 2013", "vc12"},
		{"Visual Studio 11 2012", "vc11"},
		{"Visual Studio 10 2010", "vc10"},
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

	//ログファイルパス表示
	GetTempPath(MAX_PATH, path);
	m_LogPath.Format("%s\\VCVerChanger\\TRACE.log", path);
	GetDlgItem(IDC_LOG_PATH)->SetWindowTextA(m_LogPath);

	strText.LoadString(IDS_WARNING_1);
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(strText);
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

	TRACE("OnBnClickedCheck : 確認ボタン処理\n");
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("");

	m_RegistryUtil.RegOpen();

	//レジストリ読み出し
	bRet = RegistryReadProc();
	if (!bRet)
	{
		m_RegistryUtil.RegClose();
		return;
	}

	//PATHにOpenRTM-aistの32bit/64bitの両定義が存在するか？
	ret = PathDoubleDefinitionCheck(m_RtmPath);
	if (ret == 0)
	{
		TRACE("OnBnClickedCheck : OpenRTM-aistのPATHが32bit/64bit両方定義されている\n");
		str.LoadString(IDS_WARNING_2);
		GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
		m_32b64bDualDefineFlg = true;
	}
	if (ret == -2)
	{
		//PATHに32bit/64bit両方定義されていて、少なくとも一方はアンインストールされている
		bRet = OrganizePath();
		if(!bRet) return;
	}

	//本ツールを一度実行すると、PATHの定義は整理済みで
	//一方のバージョンしか検出できなくなる


	if (ret != -3)
	{
		//ProgramFilesにインストールされている
		//現在環境変数が示しているパスの存在確認
		if (!(::PathFileExistsA(m_OpenrtmDir) && ::PathIsDirectoryA(m_OpenrtmDir)))
		{
			//存在しない場所を指しているので、設定を正す
			CorrectSystemEnvValue();
        
			//レジストリ読み出し
			bRet = RegistryReadProc();
			if (!bRet)
			{
				m_RegistryUtil.RegClose();
				return;
			}
		}
	}

	m_RegistryUtil.RegClose();

	if (ret == -3)
	{
		//任意のディレクトリにインストールされているので
		//32bit版、64bit版のパスを確定できないため
		//両バージョンのインストール状態はチェックできない
		str.LoadString(IDS_WARNING_4);
		GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
		UpdateData(FALSE);
		return;
	}

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
// 関数名：FindStringFromTargetPath
// 機能　：targetの中から指定文字列を含むパスを抜き出す
//    該当のパスが複数ある場合、区切り文字を"\n"と";"の
//　　２つのパターンを返す。(out1とout2）
//　　targetから指定文字列を除外した残りをout3として返す
// 引数
//   :in1  :str       :指定文字列
//   :in2  :target    :対象PATH
//   :out1 :outForGUI :区切り文字"\n"（GUI表示用）
//   :out2 :outForReg :区切り文字";"（レジストリ書込用）
//   :out3 :otherPath :targetの指定文字列以外の部分
// 戻り値：PATHの編集状態。編集ありはレジストリに書き込む
//         必要がある。
//         0:なし -1:あり
////////////////////////////////////////////////////////
int CVCVerChangerDlg::FindStringFromTargetPath(
				CString str,
				CString target,
				CString& outForGUI, 
				CString& outForReg,
				CString& otherPath)
{
	CString replaceStr, orgStr, cnvStr, cnvStr2;
	CString openrtmGUIPath[10], openrtmRegPath[10];
	CString fixedGUIPath[10], fixedRegPath[10];
	int i,j;
	int pos, findpos, retVal, fixedPathCount, openrtmPathCount;
	bool repRet;

	pos = findpos = retVal = fixedPathCount = openrtmPathCount = 0;

	//受け取ったtargetのpathを";"で分割する
	orgStr = target.Tokenize(";", pos);
	while (orgStr != "")
	{
		if (str == "OpenRTM-aist")
		{
			//%%変数を展開する
			repRet = ReplaceEnv(orgStr, replaceStr);
		}else{
			//%%変数を展開しない
			replaceStr = orgStr;
		}
			
		//指定文字列を含むパスだけ抜き出す
		findpos = replaceStr.Find(str);
		if (findpos != -1)
		{
			//パスの存在確認
			if (::PathFileExistsA(replaceStr) && ::PathIsDirectoryA(replaceStr))
			{
				//パスは存在している
				if (repRet)
				{
					//%%変数が含まれていて展開した
					//GUIでは複数のパスは改行表示させる
					outForGUI = outForGUI + replaceStr + "\n";
					openrtmGUIPath[openrtmPathCount] = replaceStr;
					//レジストリ書き戻し用は%%変数利用、;で区切る
					outForReg = outForReg + orgStr + ";";
					openrtmRegPath[openrtmPathCount] = orgStr;
					openrtmPathCount++;
				}else{
					//%%変数が含まれていないため展開していない
					TRACE("FindStringFromTargetPath : %%変数が展開されて登録されている(%s)\n", replaceStr);
					//「vc**」の文字列を「%RTM_VC_VERSION%」に置き換える
					ReplaceFromVCxxToRTMVCVERSION(replaceStr, cnvStr);
					//改めて%%変数を展開する
					ReplaceEnv(cnvStr, cnvStr2);
					fixedGUIPath[fixedPathCount] = cnvStr2;
					fixedRegPath[fixedPathCount] = cnvStr;

					retVal = -1;
					fixedPathCount++;
				}
			}else{
				//存在しないパス
				TRACE("FindStringFromTargetPath : 存在しないパスなので除外する(%s)\n", replaceStr);
				retVal = -1;
			}
		}else{
			//OpenRTM-aist以外のパスはotherPathに保存しておき
			//レジストリへの書き込み時に合わせて書き戻す
			otherPath = otherPath + orgStr + ";";
		}
			
		orgStr = target.Tokenize(";", pos);
	}
	if (fixedPathCount > 0)
	{
		//OpenRTM-aistを含むパスで%%変数が展開された形で登録されている
		//ものがあったということ。%%変数を使うように修正したので
		//ダブりがあるかどうかチェックする。ダブっていれば削除する。
		for (i=0; i<fixedPathCount; i++)
		{
			for (j=0; j<openrtmPathCount; j++)
			{
				if (fixedGUIPath[i] == openrtmGUIPath[j])
				{
					TRACE("FindStringFromTargetPath : このパスはダブっているのでレジストリから削除する(%s)\n", fixedRegPath[i]);
					fixedGUIPath[i] = "";
					break;
				}
			}
			if (fixedGUIPath[i] != "")
			{
				TRACE("FindStringFromTargetPath : %%変数を使うように修正したパスをレジストリに登録する(%s)\n", fixedRegPath[i]);
				//GUIでは複数のパスは改行表示させる
				outForGUI = outForGUI + fixedGUIPath[i] + "\n";
				//レジストリ書き戻し用は%%変数利用、;で区切る
				outForReg = outForReg + fixedRegPath[i] + ";";
			}
		}
		
	}
	TRACE("FindStringFromTargetPath : otherPath : %s\n", otherPath);
	return retVal;
}

////////////////////////////////////////////////////////
// 関数名：ReplaceFromVCxxToRTMVCVERSION
// 機能　：「vc**」の文字列を「%RTM_VC_VERSION%」に置き換える
// 引数　：CString path     :対象のパス
//         CString& outPath :変換後のパス
// 戻り値：true :正常　false:異常
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::ReplaceFromVCxxToRTMVCVERSION(CString path, CString& outPath)
{
	bool ret = true;
	int i, pos;
	CString vc_ver;

	for(i=0; i<m_VSVer.GetCount(); i++)
	{
		vc_ver = m_VSInfo[i].Internal_Ver;
		pos = path.Find(vc_ver);
		if (pos != -1 )
		{
			outPath = path;
			outPath.Replace(vc_ver, "%RTM_VC_VERSION%");
			TRACE("ReplaceFromVCxxToRTMVCVERSION : Replace to (%s)\n", outPath);
			break;
		}
	}
	return ret;
}

////////////////////////////////////////////////////////
// 関数名：PathDoubleDefinitionCheck
// 機能　：PATHにOpenRTM-aistの32bit/64bitの両定義が存在
//         しているかチェックする
// 引数　：CString Path　：PATHの値
// 戻り値：0: 32/64bitの両定義がある
//        -1: 一方の定義のみ
// 　　　 -2: 両定義が残っているがアンインストールされている
//        -3: 任意のディレクトリにインストールされている
////////////////////////////////////////////////////////
int CVCVerChangerDlg::PathDoubleDefinitionCheck(CString Path)
{
	int ret = -1;
	int findpos = 0;
	CString binPath;
	int existCnt = 0;

	//32bit/64bitの両バージョンをインストール後、
	//システム環境変数をいじっていなければ、PATHに下記の
	//定義が存在する
	//C:\Program Files\OpenRTM-aist\1.1.2\bin\%RTM_VC_VERSION%
	//C:\Program Files (x86)\OpenRTM-aist\1.1.2\bin\%RTM_VC_VERSION%
	//この状態かどうかをチェックする
	//（2018/05 この判断ではまずいと判明. confファイルのバックアップなどが
	//　残っている場合があるので）
  
	//bin\%RTM_VC_VERSION%にはconfファイルがあり、ユーザによりバックアップ
	//ファイル等が作成されるとアンインストール後も残ってしまう。
	//このため判定には下記のPATHを利用する
	//C:\Program Files (x86)\OpenRTM-aist\1.2.0\omniORB\4.2.2_%RTM_VC_VERSION%\bin\x86_win32
	//C:\Program Files\OpenRTM-aist\1.2.0\omniORB\4.2.2_%RTM_VC_VERSION%\bin\x86_win32

	//%OMNI_ROOT%bin\x86_win32のパスを生成
	binPath.Format("%sbin\\x86_win32", LPCTSTR(m_OmniRoot));
	//パスの存在確認
	if (::PathFileExistsA(binPath) && ::PathIsDirectoryA(binPath))
	{
		existCnt++;
	}

	//指定のパスに含まれるProgram Filesを切り替える
	//"Program Files (x86)" -> "Program Files"
	//"Program Files" -> "Program Files (x86)"
	findpos = binPath.Find(m_x86Path);
	if (findpos != -1)
	{
		binPath.Replace(m_x86Path, m_x64Path);
	}else{
		findpos = binPath.Find(m_x64Path);
		if (findpos != -1)
		{
			binPath.Replace(m_x64Path, m_x86Path);
		}else{
			TRACE("PathDoubleDefinitionCheck : Program Files下ではない任意のディレクトリにインストールされている\n");
			ret = -3;
			return ret;
		}
	}
	//パスの存在確認
	if (::PathFileExistsA(binPath) && ::PathIsDirectoryA(binPath))
	{
		existCnt++;
	}

	//binPathはPATHに定義されているか？
	findpos = Path.Find(binPath);
	if (findpos!=-1)
	{
		if (existCnt==2)
		{
			TRACE("PathDoubleDefinitionCheck : OpenRTM-aistの32bit/64bitの両方のパスが存在する\n");
			ret = 0;
		}else{
			TRACE("PathDoubleDefinitionCheck : OpenRTM-aistの32bit/64bitの両方のパスが存在するが、どちらかはアンインストールされている\n");
			ret = -2;
		}
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
	TRACE("OnBnClickedClose : 終了ボタン処理\n");
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
	bool ret;

	TRACE("OnBnClickedUpdate : 更新ボタン処理\n");
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
	TRACE("OnBnClickedUpdate : Combo Boxの入力値 : m_VcVersion : %s\n", m_VcVersion);

	m_RegistryUtil.RegOpen();

	//レジストリエントリ　書き込み
	ret = m_RegistryUtil.WriteEnv("RTM_VC_VERSION", REG_SZ, m_VcVersion);	
	if (!ret)
	{
		RegistryEntryErr("RTM_VC_VERSIN", "Write");
		m_RegistryUtil.RegClose();
		return;
	}

	//WM_SETTINGCHANGE
	SettingChange();

	//書き換え後のレジストリを読み込む
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
	TRACE("OnBnClickedUpdate : %s\n", m_StatusMsg);
  
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// 関数名：OnCtlColor
// 機能　：スタティックテキストの文字色を赤にするため
// 　　　　本関数をオーバーライドする
////////////////////////////////////////////////////////
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
	PDWORD_PTR ReturnValue = NULL;
	TRACE("SettingChange : WM_SETTINGCHANGEメッセージ送信\n");
	LRESULT Ret = SendMessageTimeout(
		HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)("Environment"),
		SMTO_ABORTIFHUNG, 5000, ReturnValue);
	if (!Ret)
	{
		TRACE("SettingChange error. (%d)\n", Ret);
	}
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
	m_StatusMsg.LoadString(IDS_STATUS_1);
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
	}
	
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA("");

	m_RegistryUtil.RegOpen();

	//レジストリ更新処理
	ret = RegistryWriteProc(m_archInfo[i].pFilePath);
	if (!ret)
	{
		m_RegistryUtil.RegClose();
		return;
	}
	
	//WM_SETTINGCHANGE
	SettingChange();

	//書き換え後のレジストリを読み込む
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
	TRACE("OnClickedArchChange : %s\n", m_StatusMsg);
  
	UpdateData(FALSE);
}


////////////////////////////////////////////////////////
// 関数名：CorrectSystemEnvValue
// 機能　：システム環境変数の値を実際のインストール状況
//         合った設定になるように正す。
//         OpenRTM-aistの32bit/64bit版の切替により
//         環境変数の設定が現状と合っていない場合へ
//         対応する。
// 引数　：無し
// 戻り値：OpenRTM-aistのインストール状態  
//         true:インストールしている
//         false:アンインストールされている
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::CorrectSystemEnvValue()
{
	CString archPath, outpath;
	int findpos = 0;
	bool result = true;
	CString target = m_OpenrtmDir;

	//現在の設定と異なるアーキテクチャのパスを取得
	findpos = target.Find(m_x86Path);
	if (findpos != -1)
	{
		//パスは32bit設定になっている
		//64bit版のパスを取得する
		ChangeProgramFilesPath(m_OpenrtmDir, m_x64Path, outpath);
		archPath = m_x64Path;
	}else{
		//パスは64bit設定になっている
		//32bit版のパスを取得する
		ChangeProgramFilesPath(m_OpenrtmDir, m_x86Path, outpath);
		archPath = m_x86Path;
	}

	//ディレクトリ存在を確認する
	if(::PathFileExistsA(outpath) && ::PathIsDirectoryA(outpath))
	{
		//存在しているのでパスを書き換える
		TRACE("CorrectSystemEnvValue : システム環境変数を正しい設定で書換え\n");
		RegistryWriteProc(archPath);
	}else{
		//OpenRTM-aistはアンインストールされていると判断
		//レジストリから削除する
		TRACE("CorrectSystemEnvValue : OpenRTM-aistはアンインストールされている\n");
		RegistryDeleteProc("ALL");
		CleanResult();
		result = false;
	}

	//WM_SETTINGCHANGE
	SettingChange();

	return result;
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

	//confファイルのバックアップなどがある場合、アンインストール後も
	//OpenRTM-aistディレクトリが残る場合がある。そのためOMNI_ROOTで
	//示しているディレクトリを存在確認に利用する

	str = m_OmniRoot;
	//32bit版確認
	findpos = str.Find(m_x86Path);
	if (findpos != -1)
	{
		//パスは32bit設定になっている
		TRACE("InstalledArchCheck : パスは32bit設定\n");
		tmp = str;
		tmp.Replace(m_x86Path, m_x64Path);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			TRACE("InstalledArchCheck : パスは32bit設定で64bit版もインストール済み\n");
			ret = 0;
		}						
	}else{
		//パスは64bit設定になっている
		TRACE("InstalledArchCheck : パスは64bit設定\n");
		tmp = str;
		tmp.Replace(m_x64Path, m_x86Path);
		if (::PathFileExistsA(tmp) && ::PathIsDirectoryA(tmp))
		{
			TRACE("InstalledArchCheck : パスは64bit設定で32bit版もインストール済み\n");
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
//         CString toDir    :このディレクトリへ変換する
//         CString& outPath :変換後のパス
// 戻り値：true :変換あり　false:変換なし
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
		TRACE("ChangeProgramFilesPath : 64bit->32bitへ切替\n");
		outPath.Replace(m_x64Path, m_x86Path);
		
		//OpenCVが含まれているか？
		findpos = path.Find("OpenCV");
		if (findpos != -1)
		{
			//64bit->32bit
			outPath.Replace("\\x64\\", "\\x86\\");
		}
		ret = true;
	}
	if (findpos==-1 && toDir==m_x64Path)
	{
		//32bit->64bit
		TRACE("ChangeProgramFilesPath : 32bit->64bitへ切替\n");
		outPath.Replace(m_x86Path, m_x64Path);

		//OpenCVが含まれているか？
		findpos = path.Find("OpenCV");
		if (findpos != -1)
		{
			//32bit->64bit
			outPath.Replace("\\x86\\", "\\x64\\");
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

   TRACE("DeleteUnnecessaryPath : 指定以外のパスを除外する\n");
   //ここへ渡されるpathは、レジストリへ書き戻すもので
   //%変数が展開されていない
   TRACE("DeleteUnnecessaryPath : in(%s)\n", path);

   basisPath.Format("%s\\OpenRTM-aist", LPCTSTR(info->pFilePath));
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
// 関数名：RegistryReadProc
// 機能　：レジストリエントリ読込処理
// 引数　：無し
// 戻り値：true:正常終了　false:エントリ未定義
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::RegistryReadProc()
{
	CString EnvValue = "";
	CString path, str;
	CString outForGUI, outForReg, otherPath;
	int i, ret;
	bool bRet;

	TRACE("RegistryReadProc: レジストリエントリ読込処理\n");

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
		//レジストリから削除する
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
	//レジストリ書き戻し用
	m_OmniRootForReg = EnvValue; 
	//GUI表示用は%OMNI_ROOT%を展開させる
	ReplaceEnv(EnvValue, m_OmniRoot);

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

	//PATHからOpenRTM-aistが設定したパスを切り出す	
	outForGUI = outForReg = otherPath = "";
	ret = FindStringFromTargetPath("OpenRTM-aist", EnvValue,
						outForGUI, outForReg, otherPath); 
	if (ret == -1)
	{
		//PATH内容を編集しているので、書き込んでから改めて読み出す
		path = otherPath + outForReg;
		bRet = m_RegistryUtil.WriteEnv("PATH", REG_EXPAND_SZ, path);
		if (!bRet)
		{
			RegistryEntryErr("PATH", "Write");
			return false;
		}
		//WM_SETTINGCHANGE
		SettingChange();

		EnvValue = m_RegistryUtil.ReadEnv("PATH");
		if (EnvValue == "")
		{
			RegistryEntryErr("PATH", "Read");
			return false;
		}
		outForGUI = outForReg = otherPath = "";
		FindStringFromTargetPath("OpenRTM-aist", EnvValue,
						outForGUI, outForReg, otherPath); 
	}
	m_RtmPath = outForGUI;
	m_RtmPathForReg = outForReg;
	m_otherPath = otherPath;

	return true;
}

////////////////////////////////////////////////////////
// 関数名：ReplaceEnv
// 機能　：システム環境変数値に含まれる%変数%を展開する
//    RegQueryValueExはレジストリ値に含まれる%%変数を
//    展開してくれないため、展開させる
// 引数　：CString Path　：展開前のパス
//         CString& outPath：展開後のパス
// 戻り値：true :変換あり　false:変換なし
////////////////////////////////////////////////////////
bool  CVCVerChangerDlg::ReplaceEnv(CString Path, CString& outPath)
{
	CString str1, str2, str3, src;
	int VER_len, OMNI_len, CV_len;
	bool ret = false;
	    
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
		ret = true;
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
		ret = true;
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
		ret = true;
	}	
	outPath = src;
	return ret;
}

////////////////////////////////////////////////////////
// 関数名：RegistryWriteProc
// 機能　：レジストリ更新処理
// 引数　：無し
// 戻り値：無し  
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::RegistryWriteProc(CString toDir)
{
	CString outpath, allpath;
	bool ret, ret1;

	ret = ChangeProgramFilesPath(m_RtmBase, toDir, outpath);
	if (ret)
	{
		//レジストリの書換えが必要
		TRACE("RegistryWriteProc : レジストリ更新処理\n");
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
		TRACE("RegistryWriteProc : PATH更新\n");
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
// 関数名：RegistryDeleteProc
// 機能　：レジストリエントリ削除処理
// 引数　：CString target :対象エントリ
//                         ALLを指定すれば全削除
// 戻り値：true:正常終了　false:エントリ未定義
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::RegistryDeleteProc(CString target)
{
	CString EnvValue = "";
	CString outForGUI, outForReg, otherPath;
	bool ret;

	TRACE("RegistryDeleteProc : レジストリエントリ削除処理\n");
    
	ret = m_RegistryUtil.DeleteEnv("RTM_VC_VERSION");
	if (!ret)
	{
		RegistryEntryErr("RTM_VC_VERSION", "Delete");
		return false;
	}

	//アンインストールしても残ってしまうシステム環境変数削除
	//本ツールで値を書換えたものが削除されず残ってしまう
	//OpenRTM-aistの32/64bit版のいずれかのみインストールしている
	//場合は、RTM_VC_VERSIONが残る可能性がある
	//32/64bit版の両方をインストールしている場合、本ツールで切替を
	//実行すると全ての環境変数が残る可能性があるので、引数にALLを
	//指定して実行している

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

		//PATHからOpenRTM-aistを抜き出した残りを書き戻す
		EnvValue = m_RegistryUtil.ReadEnv("PATH");
		if (EnvValue == "")
		{
			RegistryEntryErr("PATH", "Read");
			return false;
		}
		//PATHへOpenRTM-aistが設定したパスを切り出す	
		outForGUI = outForReg = otherPath = "";
		FindStringFromTargetPath("OpenRTM-aist", EnvValue,
							outForGUI, outForReg, otherPath);
		if (outForReg != "")
		{
			//PATHの中にOpenRTM-aistのパスが残っているので、これを
			//除いたものを書き戻す
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
// 関数名：RegistryEntryReadErr
// 機能　：レジストリエントリ読み込みエラー時の処理
//         OpenRTM-aistがインストールされていない可能性
//         が高い場合に表示するメッセージ
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegistryEntryReadErr()
{
	CString str;

	TRACE("RegistryEntryReadErr : OpenRTM-aistインストール確認メッセージ表示\n");
	str.LoadStringA(IDS_WARNING_3);
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
	UpdateData(FALSE);
}

////////////////////////////////////////////////////////
// 関数名：RegistryEntryErr
// 機能　：レジストリエントリRead/Writeエラー時の処理
// 引数　：LPCTSTR EnvName　：エントリ名
//         LPCTSTR proc :処理
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::RegistryEntryErr(LPCTSTR EnvName, LPCTSTR proc)
{
	CString str;

	str.Format("%s : %s Error", EnvName, proc);
	TRACE("RegistryEntryErr : %s\n", str);
	GetDlgItem(IDC_WARNING_MSG)->SetWindowTextA(str);
	UpdateData(FALSE);
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

	TRACE("OnDestroy\n");
	delete [] m_VSInfo;
	delete [] m_archInfo;
	m_VSInfo = NULL;
	m_archInfo = NULL;
}

////////////////////////////////////////////////////////
// 関数名：CleanResult
// 機能　：ダイアログのResult表示初期化
// 引数　：無し
// 戻り値：無し
////////////////////////////////////////////////////////
void CVCVerChangerDlg::CleanResult()
{
	TRACE("CleanResult : ダイアログのResult表示初期化\n");
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

////////////////////////////////////////////////////////
// 関数名：OrganizePath
// 機能　：PATHに含まれる不正パスを削除し整理する
// 引数　：無し
// 戻り値：true:正常　false:異常
////////////////////////////////////////////////////////
bool CVCVerChangerDlg::OrganizePath()
{
	CString arch, allpath;
	int i;
	bool ret;
    
	//アンインストールしているのに残ってしまっているパスを削除する
	//OpenRTM-aistの32/64bit版を両方インストールし、本ツールで不要な
	//パスを整理する以前に一方をアンインストールすると、PATHの設定が
	//残る場合を確認している
    
	//この不正なパスを削除する
	//判断にRTM_BASEの設定を使い、同じアーキテクチャのパスのみ残し
	//レジストリを書き換える
	if (m_RtmBase.Find(m_x86Path) != -1)
	{
		arch = "x86";
	}else{
		arch = "x64";
	}
    
	for (i=0; i<m_arch.GetCount(); i++)
	{
		if (arch == m_archInfo[i].CV_ARCH)
		{
			break;
		}
	}
    
	TRACE("OrganizePath : PATHに含まれている%s以外のパスを削除する\n", m_archInfo[i].BitType);
	m_RtmPathForReg = DeleteUnnecessaryPath(
			m_RtmPathForReg, ";", &m_archInfo[i]);
			allpath = m_otherPath + m_RtmPathForReg;
	ret = m_RegistryUtil.WriteEnv("PATH", REG_EXPAND_SZ, allpath);
	if (!ret)
	{
		RegistryEntryErr("PATH", "Write");
		return false;
	}
    
	//WM_SETTINGCHANGE
	SettingChange();

	//書き換え後のレジストリを読み込む
	ret = RegistryReadProc();
	if (!ret)
	{
		m_RegistryUtil.RegClose();
		return false;
	}

	return true;
}