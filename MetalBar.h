/*
*	Copyright 2009 Griffin Software
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License. 
*/

#pragma once

class CEditCmdFilter;

class MetalBar
{
public:
	struct ScrollbarHandles
	{
		HWND		editor;
		HWND		vert;
		HWND		horiz;
		HWND		resharper;
	};

	MetalBar(ScrollbarHandles& handles, IVsTextView* view);
	~MetalBar();

	IVsTextView*					GetView() const { return m_view; }
	HWND							GetHwnd() const { return m_handles.vert; }

	static void						Init();
	static void						Uninit();
	static unsigned int				IsEnabled() { return s_enabled; }
	static void						SetBarsEnabled(unsigned int enabled);
	static void						ResetSettings();
	static void						SaveSettings();

	// User-controllable parameters.
	static unsigned int				s_barWidth;
	static unsigned int				s_whitespaceColor;
	static unsigned int				s_upperCaseColor;
	static unsigned int				s_characterColor;
	static unsigned int				s_commentColor;
	static unsigned int				s_cursorColor;
	static unsigned int				s_matchColor;
	static unsigned int				s_modifiedLineColor;
	static unsigned int				s_unsavedLineColor;
	static unsigned int				s_breakpointColor;
	static unsigned int				s_bookmarkColor;
	static unsigned int				s_requireAltForHighlight;
	static unsigned int				s_caseSensitive;
	static unsigned int				s_wholeWordOnly;
	static unsigned int				s_codePreviewBg;
	static unsigned int				s_codePreviewFg;
	static unsigned int				s_codePreviewWidth;
	static unsigned int				s_codePreviewHeight;

private:
	static std::set<MetalBar*>		s_bars;
	static unsigned int				s_enabled;

	static bool						ReadRegInt(unsigned int* to, HKEY key, const char* name);
	static void						WriteRegInt(HKEY key, const char* name, unsigned int val);
	static void						ReadSettings();
	static void						RemoveAllBars();

	// Handles and other window-related stuff.
	ScrollbarHandles				m_handles;
	WNDPROC							m_oldProc;

	// Text.
	IVsTextView*					m_view;
	int								m_numLines;
	CEditCmdFilter*					m_editCmdFilter;
	CComBSTR						m_highlightWord;

	// Painting.
	HBITMAP							m_codeImg;
	int								m_codeImgHeight;
	bool							m_codeImgDirty;
	HDC								m_imgDC;
	HBITMAP							m_backBufferImg;
	HDC								m_backBufferDC;
	unsigned int*					m_backBufferBits;
	unsigned int					m_backBufferWidth;
	unsigned int					m_backBufferHeight;

	// Scrollbar stuff.
	int								m_pageSize;
	int								m_scrollPos;
	int								m_scrollMin;
	int								m_scrollMax;
	bool							m_dragging;

	void							OnDrag(bool initial);
	void							OnTrackPreview();
	void							ShowCodePreview();
	void							OnPaint(HDC ctrlDC);
	void							AdjustSize(unsigned int requiredWidth, WINDOWPOS* vertSbPos);
	void							RemoveWndProcHook();

	bool							GetBufferAndText(IVsTextLines** buffer, BSTR* text, long* numLines);
	void							HighlightMatchingWords();
	void							RemoveWordHighlight(IVsTextLines* buffer);

	void							PaintLineFlags(unsigned int* img, int line, int imgHeight, unsigned int flags);
	void							RefreshCodeImg(int barHeight);

	LRESULT							WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	static LRESULT FAR PASCAL		WndProcHelper(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};
