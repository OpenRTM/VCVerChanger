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

// CVCVerChangerDlg ダイアログ
class CVCVerChangerDlg : public CDialogEx
{
// コンストラクション
public:
	CVCVerChangerDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
enum { IDD = IDD_VCVERCHANGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
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
	bool	RegistryReadProc(); //レジストリ・エントリ読込処理
	bool	RegistryWriteProc(CString toDir); //レジストリ・エントリ書き込み処理
	bool	RegistryDeleteProc(CString target); //レジストリ・エントリ削除処理
	void	RegistryEntryReadErr();
	void	RegistryEntryErr(LPCTSTR EnvName, LPCTSTR proc);

	//targetの中から指定文字列を含むパスを抜き出す
	int	FindStringFromTargetPath(
				CString str,
				CString target,
				CString& outForGUI, 
				CString& outForReg,
				CString& otherPath);
	
	bool ReplaceEnv(CString Path, CString& outPath);	//システム環境変数値に含まれる%変数%を展開する
	void    SettingChange();			//WM_SETTINGCHANGEメッセージ送信
	int		InstalledArchCheck();		//OpenRTM-aistのアーキテクチャを調べる
	bool	CorrectSystemEnvValue();	//OpenRTM-aistのインストール状況を調べる
	void	CleanResult();				//ダイアログのResult表示初期化

	//指定のパスに含まれるProgram Filesを切り替える
	bool	ChangeProgramFilesPath(CString path, 
					CString toDir, CString& outPath);

	//PATHにOpenRTM-aistの32bit/64bitの両定義が存在チェック
	int 	PathDoubleDefinitionCheck(CString Path);

	//ARCH_INFO構造体で指定されたタイプ(32bit/64bit)以外のパスを除外する
	CString	DeleteUnnecessaryPath(CString path, 
				CString delimit, ARCH_INFO* info);
    
	bool	OrganizePath();

	//「vc**」の文字列を「%RTM_VC_VERSION%」に置き換える
	bool	ReplaceFromVCxxToRTMVCVERSION(CString path, CString& outPath);

	CString m_VcVersion;
	CString m_RtmBase;
	CString m_RtmRoot;
	CString m_RtmJavaRoot;
	CString m_OmniRoot;			//Dialog表示用
	CString m_OmniRootForReg;	//レジストリ書込用（%変数を含む）
	CString m_OpencvDir;
	CString m_OpenrtmDir;
	CString m_RtmPath;			//Dialog表示用
	CString m_RtmPathForReg;	//レジストリ書込用（%変数を含む）
	CString m_otherPath;		//OpenRTM-aist以外のPATH設定
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

	//PATH中のOpenRTM-aist 32bit/64bit両定義状態を示すフラグ
	bool m_32b64bDualDefineFlg;	//true:両方のパスが存在する　flase:存在しない

	//OpenRTM-aist 32bit/64bitインストール状態を示すフラグ
	bool m_32b64bInstFlg;	//true:両方インストールされている　flase:一方のバージョンのみ
	
};
