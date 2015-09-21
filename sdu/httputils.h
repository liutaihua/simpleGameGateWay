#pragma once

#ifndef HTTPUTILS_H
#define HTTPUTILS_H

#include <string>

namespace Crown
{
std::string UrlEncode(const std::string &orig);

std::string CharToHex(char ch);
}

#endif // HTTPUTILS_H
