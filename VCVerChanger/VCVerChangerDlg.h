
// VCVerChangerDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"

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
	void	Init();
	bool	RegistryReadProc(); //レジストリエントリ読込処理

	//レジストリキーのopen/close
	void    RegOpen();			
	void	RegClose();

	//レジストリエントリのread/write
	CString ReadEnv(LPCTSTR EnvName);
	void	RegistryEntryReadErr();
	LONG	WriteEnv(LPCTSTR EnvName, DWORD type, CString str);

	//PATHの中から「OpenRTM-aist」を含むものを抜き出す
	void	GetOpenrtmPath(
				CString Path, 
				CString& outForGUI, 
				CString& outForReg,
				CString& otherPath);
	
	CString ReplaceEnv(CString Path);	//システム環境変数値に含まれる%変数%を展開する
	void    SettingChange();			//WM_SETTINGCHANGEメッセージ送信
	CString GetTime();					//TRACE用。時刻取得。
	int		InstalledArchCheck();		//OpenRTM-aistのアーキテクチャを調べる

	//指定のパスに含まれるProgram Filesを切り替える
	bool	ChangeProgramFilesPath(CString path, 
					ARCH_INFO* info, CString& outpath);

	//PATHにOpenRTM-aistの32bit/64bitの両定義が存在チェック
	bool	PathDoubleDefinitionCheck(CString Path);

	//ARCH_INFO構造体で指定されたタイプ(32bit/64bit)以外のパスを除外する
	CString	DeleteUnnecessaryPath(CString path, 
				CString delimit, ARCH_INFO* info);

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
	CStatic m_WarningMsg;
	CStatic m_staticArch;

	HKEY m_hkey;
	CString m_StatusMsg;
	CComboBox m_VSVer;
	CComboBox m_arch;
	CString m_x86Path;
	CString m_x64Path;
	CString m_orgPATH;
	CString m_otherPath;

	VISUAL_STUDIO*	m_VSInfo;
	ARCH_INFO*		m_archInfo;

	//PATH中のOpenRTM-aist 32bit/64bit両定義状態を示すフラグ
	bool m_32b64bDualDefineFlg;	//true:両方のパスが存在する　flase:存在しない

	//OpenRTM-aist 32bit/64bitインストール状態を示すフラグ
	bool m_32b64bInstFlg;	//true:両方インストールされている　flase:一方のバージョンのみ
};
