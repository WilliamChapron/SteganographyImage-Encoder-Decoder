#pragma once

#include <string>     

std::string WStringToString(const std::wstring& wstr) {
    return std::string(wstr.begin(), wstr.end());
}