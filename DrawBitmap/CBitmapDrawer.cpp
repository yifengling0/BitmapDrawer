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
 * \param winDc 窗口DC，主要用于创建兼容DC
 * \param rcId 载入背景图片的资源Id
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
 * \param path 保存图片的路径
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
 * \brief 设置字体，也可以默认不进行设置
 * \param fontName 字体名
 * \param height 字号
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
 * \brief 将写好文字的dc刷新到窗口或者其他任意DC中
 * \param windowDc 需要显示的窗口DC
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
 * \param str 字符串
 * \param color 颜色
 * \remark 注意需要设置字体的话，请先调用SetFont，因为考慮字体不会经常性变化
 * 接口是分开的。
 */
void CBitmapDrawer::DrawText(int x, int y, LPCTSTR str, COLORREF color)
{
	assert(m_hDc != NULL);

	// 没有设置字体情况下使用窗口默认字体
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
