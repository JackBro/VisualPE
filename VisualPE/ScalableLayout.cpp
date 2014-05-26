#include "StdAfx.h"
#include "ScalableLayout.h"

#define COLOR(cr) (cr | 0xFF000000)

CScalableLayout::CScalableLayout(HWND hParentWnd)
	:m_pContainer(0),m_pProgress(0),m_pStatusBar(0),
	m_hParentWnd(hParentWnd)
{
	CDialogBuilder builder;
	CContainerUI *pUI = static_cast<CContainerUI*>(builder.Create(
		_T("scalablelayout.xml")));

	this->Add(pUI);

	m_pStatusBar = static_cast<CHorizontalLayoutUI*>(FindSubControl(_T("statusbar")));
	m_pProgress = static_cast<CProgressUI*>(FindSubControl(_T("scaleprogress")));
	m_pZoomout = static_cast<CButtonUI*>(FindSubControl(_T("zoomout")));
	m_pContainer = static_cast<CContainerUI*>(FindSubControl(_T("container")));

}

void CScalableLayout::DoInit()
{
	GetManager()->AddNotifier(this);
	GetManager()->AddMessageFilter(this);
}

CScalableLayout::~CScalableLayout(void)
{
}

CContainerUI * CScalableLayout::CreateLayout( CScalableNode::Ptr pNode,int nLevel )
{
	CContainerUI *pLayout = pNode->bHor ?
		static_cast<CContainerUI*>(new CHorizontalLayoutUI) : 
		static_cast<CContainerUI*>(new CVerticalLayoutUI);

	pLayout->SetBkColor(COLOR(pNode->crBk));

	for (CScalableNode::Iter i = pNode->ChildBegin();
		i != pNode->ChildEnd();
		i++)
	{
		bool bLeaf = true;
		for (CScalableNode::Iter j = (*i)->ChildBegin();
			j != (*i)->ChildEnd();
			j++)
		{
			if ((*j)->nLevel == nLevel)
			{
				bLeaf = false;
				break;
			}
		}
		if (bLeaf)
		{
			CContainerUI *pItemContainer = new CContainerUI;
			CButtonUI *pItem = new CButtonUI;
			pItem->SetName((*i)->sName);
			pItem->SetBkColor(COLOR((*i)->crBk));
			pItem->SetShowHtml();
			pItem->SetTextStyle(DT_CENTER|DT_VCENTER);
			
			CDuiString sText;
			sText.Format(_T("{p}%s{n}{n}{c #FFCCCCCC}%s{/c}{/p}")
				,(LPCTSTR)(*i)->sText,(LPCTSTR)(*i)->sDescription);
			pItem->SetText(sText);
			
			pItemContainer->Add(pItem);

			pLayout->Add(pItemContainer);
		}
		else
		{
			pLayout->Add(CreateLayout(*i,nLevel));
		}
	}


	if (!pNode->sDescription.IsEmpty())
	{
		CVerticalLayoutUI *pWrapper = new CVerticalLayoutUI;
		pWrapper->Add(pLayout);

		pLayout->SetInset(CDuiRect(5,5,5,0));

		CLabelUI *pDescription = new CLabelUI;
		pDescription->SetFixedHeight(20);
		pDescription->SetTextStyle(DT_CENTER);
		pDescription->SetBkColor(COLOR(pNode->crBk));

		CDuiString sDescription;
		sDescription.Format(_T("%s (%s)"),
			(LPCTSTR)pNode->sDescription,
			(LPCTSTR)pNode->SizeString());
		pDescription->SetText(sDescription);
		pWrapper->Add(pDescription);

		return pWrapper;
	}

	return pLayout;
}

void CScalableLayout::ZoomIn( CDuiString sNodeName )
{
	if (!m_pRootNode)
	{
		return;
	}
	CScalableNode::Ptr pNode = m_pRootNode->FindChild(sNodeName);

	if (!pNode)
	{
		return;
	}

	m_pCurrentNode = pNode;
	m_pContainer->RemoveAll();
	m_pContainer->Add(CreateLayout(pNode,pNode->nLevel + 1));

	m_pStatusBar->SetVisible(pNode->nLevel > 0);
	m_pProgress->SetValue(m_nMaxLevel - pNode->nLevel + 1);
}

void CScalableLayout::ZoomOut()
{
	if (m_pCurrentNode && m_pCurrentNode->GetParent())
	{
		ZoomIn(m_pCurrentNode->GetParent()->sName);
	}
}

#define RandColor() RGB(rand(),rand(),rand())

void CScalableLayout::TestLayout()
{
// 	CScalableNode::Ptr pRoot = 
// 		CScalableNode::New(0,true,RandColor(),_T(""),_T(""),_T("root"))
// 			<< (CScalableNode::New(1,true,RandColor(),_T("child1"),_T("child 1"),_T("description child 1"))
// 			+ (CScalableNode::New(1,false,RandColor(),_T(""),_T(""),_T("child 2"))
// 				<<(CScalableNode::New(1,true,RandColor(),_T("pGchild1"),_T("grand child 1"),_T("description grand child 1"))
// 				+ (CScalableNode::New(1,true,RandColor(),_T("pGchild2"),_T("grand child 2"),_T("description grand child 2"))
// 					<<(CScalableNode::New(2,true,RandColor(),_T("ggchild1"),_T("grand grand child 1"),_T("description grand grand child 1"))
// 					+ CScalableNode::New(2,true,RandColor(),_T("ggchild2"),_T("grand grand child 2"),_T("description grand grand child 2"))
// 					))
// 				))
// 			);
// 
// 
// 	SetContent(pRoot,2);
}

void CScalableLayout::SetContent( CScalableNode::Ptr pRoot,int nMaxLevel )
{
	m_pRootNode = pRoot;
	m_pCurrentNode = pRoot;
	m_nMaxLevel = nMaxLevel;

	m_pProgress->SetMinValue(0);
	m_pProgress->SetMaxValue(m_nMaxLevel + 1);

	ZoomIn(m_pRootNode->sName);
}

void CScalableLayout::Notify( TNotifyUI& msg )
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender == m_pZoomout)
		{
			ZoomOut();
		}
		else
		{
			ZoomIn(msg.pSender->GetName());
		}
	}
}

LRESULT CScalableLayout::MessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled )
{
	if (uMsg == WM_MOUSEWHEEL)
	{
		WORD fwKeys = LOWORD(wParam);    // key flags
		if (fwKeys != 0)
		{
			return 0;
		}

		short zDelta = (short) HIWORD(wParam);    // wheel rotation

		POINT pt;
		pt.x = (short) LOWORD(lParam);    // horizontal position of pointer
		pt.y = (short) HIWORD(lParam);    // vertical position of pointer

		::ScreenToClient(m_hParentWnd,&pt);

		if (zDelta > 0)
		{
			// zoom in
			CControlUI *pHit = GetManager()->FindControl(pt);

			CButtonUI *pHitButton = dynamic_cast<CButtonUI*>(pHit);
			if (pHitButton == 0)
			{
				return 0;
			}

			ZoomIn(pHitButton->GetName());
		}
		else
		{
			RECT rc = m_pContainer->GetPos();
			if (!::PtInRect(&rc,pt))
			{
				return 0;
			}

			ZoomOut();
		}

		bHandled = TRUE;
		return 0;
	}

	return 0;
}