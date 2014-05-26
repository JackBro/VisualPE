#include "StdAfx.h"
#include "ScalableNode.h"


CScalableNode::CScalableNode(void)
{
}


CScalableNode::~CScalableNode(void)
{
}

CScalableNode::Ptr CScalableNode::New( int nLevel /*= 0*/, bool bHor /*= true*/, COLORREF crBk /*= 0*/, CDuiString sDescription /*= _T("")*/, DWORD dwSize /*= 0*/, CDuiString sName /*= _T("")*/, CDuiString sText /*= _T("")*/ )
{
	Ptr pNew = Ptr(new CScalableNode);
	pNew->m_pThis = pNew;
	pNew->nLevel = nLevel;
	pNew->bHor = bHor;
	pNew->crBk = crBk;
	pNew->sName = sName;
	pNew->sText = sText;
	pNew->sDescription = sDescription;
	pNew->dwSize = dwSize;

	return pNew;
}
void CScalableNode::AppendChild( Ptr pNode )
{
	m_vChildren.push_back(pNode);
	pNode->m_pParent = m_pThis;
}

CScalableNode::Ptr CScalableNode::GetParent()
{
	return m_pParent;
}

CScalableNode::Iter CScalableNode::ChildBegin()
{
	return m_vChildren.begin();
}

CScalableNode::Iter CScalableNode::ChildEnd()
{
	return m_vChildren.end();
}

CScalableNode::Ptr CScalableNode::FindChild( CDuiString sName )
{
	if (this->sName == sName)
	{
		return m_pThis;
	}

	for (CScalableNode::Iter i = m_vChildren.begin();
		i != m_vChildren.end();
		i++)
	{
		Ptr pRet = (*i)->FindChild(sName);
		if (pRet)
		{
			return pRet;
		}
	}

	return Ptr();
}

CDuiString CScalableNode::SizeString()
{
	CDuiString sResult;
	if (dwSize > (2 << 20))
	{
		sResult.Format(_T("%d MB"),dwSize);
	}
	else if (dwSize > (2 << 10))
	{
		sResult.Format(_T("%d KB"),dwSize);
	}
	else
	{
		sResult.Format(_T("%d bytes"),dwSize);
	}

	return sResult;
}
CScalableNode::ListPtr operator+( CScalableNode::Ptr pLeft,CScalableNode::Ptr pRight )
{
	CScalableNode::ListPtr pList(new CScalableNode::List);
	pList->push_back(pLeft);
	pList->push_back(pRight);

	return pList;
}

CScalableNode::ListPtr operator+( CScalableNode::ListPtr pLeft,CScalableNode::Ptr pRight )
{
	pLeft->push_back(pRight);

	return pLeft;
}

CScalableNode::ListPtr operator+( CScalableNode::Ptr pLeft,CScalableNode::ListPtr pRight )
{
	pRight->push_back(pLeft);

	return pRight;
}

CScalableNode::ListPtr operator+( CScalableNode::ListPtr pLeft,CScalableNode::ListPtr pRight )
{
	for (CScalableNode::List::iterator i = pRight->begin();
		i != pRight->end();
		i++)
	{
		pLeft->push_back(*i);
	}

	return pLeft;
}

CScalableNode::Ptr operator<<( CScalableNode::Ptr pLeft,CScalableNode::Ptr pRight )
{
	pLeft->AppendChild(pRight);

	return pLeft;
}

CScalableNode::Ptr operator<<( CScalableNode::Ptr pLeft,CScalableNode::ListPtr pRight )
{
	for (CScalableNode::List::iterator i = pRight->begin();
		i != pRight->end();
		i++)
	{
		pLeft->AppendChild(*i);
	}

	return pLeft;
}