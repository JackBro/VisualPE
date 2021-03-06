#pragma once
#include <shellapi.h>
#include "ScalableLayout.h"

class CMainWnd :
	public WindowImplBase
{
public:
	CMainWnd(void);
	~CMainWnd(void);

public:
    LPCTSTR    GetWindowClassName() const;
    CDuiString GetSkinFile();
    CDuiString GetSkinFolder();
	void InitWindow();
	CControlUI *CreateControl(LPCTSTR pstrClass);
	void OnClick(TNotifyUI& msg);
	void OnDropFiles( HDROP hDrop );
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetSelectFile(CDuiString pFileName);

private:
	void ErrorMessage(LPCTSTR pText);

private:
	CButtonUI *m_pSelectFile;
	CButtonUI *m_pTestbtn;
	CButtonUI *m_pZoomout;
	CProgressUI *m_pScaleProgress;
	CContainerUI *m_pContainer;
	CLabelUI *m_pFilePath;
	CHorizontalLayoutUI *m_pStatusbar;
	CScalableLayout *m_pView;
	//CScalableLayout m_view;
};
