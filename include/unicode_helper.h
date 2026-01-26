#pragma once

#include <string>
#include <windows.h>

// 字符串类型定义
#ifdef UNICODE
    using tstring = std::wstring;
    #define T(x) L##x
#else
    using tstring = std::string;
    #define T(x) x
#endif

// UTF-8 到 UTF-16 转换
inline std::wstring Utf8ToWide(const std::string& utf8str) {
    if (utf8str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0,
        utf8str.c_str(), (int)utf8str.size(), NULL, 0);
    if (size_needed <= 0) return std::wstring();

    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(),
        (int)utf8str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

// UTF-16 到 UTF-8 转换
inline std::string WideToUtf8(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0,
        wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    if (size_needed <= 0) return std::string();

    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
        (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// GBK 到 UTF-16 转换（如果需要）
inline std::wstring GbkToWide(const std::string& gbkstr) {
    if (gbkstr.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_ACP, 0,
        gbkstr.c_str(), (int)gbkstr.size(), NULL, 0);
    if (size_needed <= 0) return std::wstring();

    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_ACP, 0, gbkstr.c_str(),
        (int)gbkstr.size(), &wstrTo[0], size_needed);
    return wstrTo;
}
