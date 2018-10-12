#include "stdafx.h"
#include "CBitmapDrawer.h"
#include <cassert>
#include <atlimage.h>


CBitmapDrawer::CBitmapDrawer()
{
	m_hDc = NULL;
	m_hFont = NULL;
}

void CBitmapDrawer::ReleaseResourceIfNeed()
{
	if (m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont = NULL;
	}

	if (m_hDc)
	{
		DeleteDC(m_hDc);
		m_hDc = NULL;
	}
}

/**
 * \brief 
 * \param winDc ����DC����Ҫ���ڴ�������DC
 * \param rcId ���뱳��ͼƬ����ԴId
 */
void CBitmapDrawer::SetWindowDc(HINSTANCE instance, HDC winDc, DWORD rcId)
{
	BITMAP bitmap;

	ReleaseResourceIfNeed();

	m_hbitmap = LoadBitmap(instance, MAKEINTRESOURCE(rcId));
	GetObject(m_hbitmap, sizeof(BITMAP), &bitmap);

	m_hDc = CreateCompatibleDC(winDc);

	SelectObject(m_hDc, m_hbitmap);
}

/**
 * \brief 
 * \param path ����ͼƬ��·��
 * \return 
 */
BOOL CBitmapDrawer::SaveBitmap(LPCTSTR path)
{
	BOOL ret = FALSE;

	assert(m_hbitmap != NULL);

	if (m_hbitmap != NULL) {
		CImage img;
		img.Attach(m_hbitmap);
		if( img.Save(path) == S_OK)
		{
			ret = TRUE;
		}
	}

	return (ret);
}


/**
 * \brief �������壬Ҳ����Ĭ�ϲ���������
 * \param fontName ������
 * \param height �ֺ�
 */
void CBitmapDrawer::SetFont(LPCTSTR fontName, int height)
{

	LOGFONT lf = { 0 };
	::GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	wcsncpy_s(lf.lfFaceName, fontName, LF_FACESIZE);
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfHeight = -height;
	lf.lfWeight += FW_BOLD;

	m_hFont = ::CreateFontIndirect(&lf);
}


/**
 * \brief ��д�����ֵ�dcˢ�µ����ڻ�����������DC��
 * \param windowDc ��Ҫ��ʾ�Ĵ���DC
 * \param x 
 * \param y 
 */
void CBitmapDrawer::CopyTo(HDC windowDc, int x, int y)
{
	BITMAP bitmap;
	GetObject(m_hbitmap, sizeof(BITMAP), &bitmap);

	BitBlt(windowDc, x, y, bitmap.bmWidth, bitmap.bmHeight, m_hDc, 0, 0, SRCCOPY);
}

/**
 * \brief 
 * \param x 
 * \param y 
 * \param str �ַ���
 * \param color ��ɫ
 * \remark ע����Ҫ��������Ļ������ȵ���SetFont����Ϊ���]���岻�ᾭ���Ա仯
 * �ӿ��Ƿֿ��ġ�
 */
void CBitmapDrawer::DrawText(int x, int y, LPCTSTR str, COLORREF color)
{
	assert(m_hDc != NULL);

	// û���������������ʹ�ô���Ĭ������
	if( m_hFont != NULL )
	{
		SelectObject(m_hDc, m_hFont);
	}

	SetBkMode(m_hDc, TRANSPARENT);
	SetTextColor(m_hDc, color);
	TextOut(m_hDc, x, y, str, lstrlen(str));
}

CBitmapDrawer::~CBitmapDrawer()
{
	ReleaseResourceIfNeed();
}
