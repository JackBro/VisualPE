// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>
#include <OleCtl.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include "..\..\duilib\include\UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
# ifdef _UNICODE
# pragma comment(lib, "..\\..\\duilib\\lib\\Duilib_ud.lib")
# else # pragma comment(lib, "..\\..\\duilib\\lib\\Duilib_d.lib")
# endif
#else
# ifdef _UNICODE
# pragma comment(lib, "..\\..\\duilib\\lib\\Duilib_u.lib")
# else
# pragma comment(lib, "..\\..\\duilib\\lib\\Duilib.lib")
# endif
#endif

namespace std
{
#ifdef _UNICODE
#define tstring string
#else
#define tstring wstring
#endif
}



#include <vector>
#include <string>
using namespace std;

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/weak_ptr.hpp>
using namespace boost;