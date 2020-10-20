#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <regex>
#include <algorithm>
#include <string>


std::string get_clipboard()
{
	std::string clipboard_text;
	HANDLE handle;

	if (!OpenClipboard(NULL))
		return "";

	handle = GetClipboardData(CF_TEXT);
	clipboard_text = (char*)handle;
	CloseClipboard();

	return clipboard_text;
}

std::string replace_all(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return str;
}

void to_clipboard(HWND hwnd, const std::string& s)
{
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) 
	{
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

int main()
{
	std::string clipboard_text = get_clipboard();
	if (clipboard_text == "")
		return 1;

	clipboard_text = replace_all(clipboard_text, "\\x", " ");
	clipboard_text = replace_all(clipboard_text, "00", "??");

	HWND hwnd = GetDesktopWindow();
	to_clipboard(hwnd, clipboard_text);

	std::cout << clipboard_text << "\n";
	return 0;
}

