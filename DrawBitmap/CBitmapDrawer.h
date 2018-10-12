#pragma once
class CBitmapDrawer
{
public:
	CBitmapDrawer();
	void SetWindowDc(HINSTANCE hinstance, HDC winDc, DWORD rcId);
	BOOL SaveBitmap(LPCTSTR path);
	void SetFont(LPCTSTR fontName, int height);
	void CopyTo(HDC windowDc, int x, int y);
	void DrawText(int x, int y, LPCTSTR str, COLORREF color);
	virtual ~CBitmapDrawer();
private:
	void ReleaseResourceIfNeed();
	HDC m_hDc;
	HFONT m_hFont;
	HBITMAP m_hbitmap;
};

