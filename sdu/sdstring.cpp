#include "SDuPCH.h"
#include "sdstring.h"
#include <algorithm>
#include <string.h>

namespace Crown
{

    char* SDAPI SDStrlwr(char *str)
    {
#if (defined(WIN32) || defined(WIN64))
        return strlwr(str);
#else
        std::transform(str, str + SDStrlen(str),
                       str, ::tolower);
        return str;
#endif
    }

    char* SDAPI SDStrupr(char *str)
    {
#if (defined(WIN32) || defined(WIN64))
        return strupr(str);
#else
        std::transform(str, str + SDStrlen(str),
                       str, ::toupper);
        return str;
#endif
    }

    wchar_t* SDAPI SDWcslwr(wchar_t *str)
    {
#if (defined(WIN32) || defined(WIN64))
        return wcslwr(str);
#else
        std::transform(str, str + SDWcslen(str),
                       str, ::tolower);
        return str;
#endif
    }

    wchar_t* SDAPI SDWcsupr(wchar_t *str)
    {
#if (defined(WIN32) || defined(WIN64))
        return wcsupr(str);
#else
        std::transform(str, str + SDWcslen(str),
                       str, ::towupper);
        return str;
#endif
    }

    std::vector<std::string> SDAPI SDSplitStrings(const std::string& sSource, char delim)
    {
        std::string::size_type pos1, pos2;
        std::vector<std::string> strParams;

        pos2 = 0;
        while (pos2 != std::string::npos)
        {
            pos1 = sSource.find_first_not_of(delim, pos2);
            if (pos1 == std::string::npos)
                break;
            pos2 = sSource.find_first_of(delim, pos1 + 1);
            if (pos2 == std::string::npos)
            {
                if (pos1 != sSource.size())
                    strParams.push_back(sSource.substr(pos1));
                break;
            }
            strParams.push_back(sSource.substr(pos1, pos2 - pos1));
        }
        return strParams;
    }

    std::string  SDAPI SDStrupr(std::string& str)
    {
        return SDStrupr(const_cast<char*>(str.c_str()));
    }

    std::string  SDAPI SDStrlwr(std::string& str)
    {
        return SDStrlwr(const_cast<char*>(str.c_str()));
    }

    std::string SDAPI SDBufferToHex(const char *pBuf, uint32 bufLen, const char *pSplitter, int32 lineLen)
    {
		static unsigned char szHex[] = "0123456789abcdef";

		char* p = (char*)pBuf;
		std::string strResult;
		for (unsigned int i = 0; i < bufLen; ++i)
		{
			strResult += szHex[(p[i] >> 4) & 0xF];
			strResult += szHex[p[i] & 0xF];
			if (pSplitter != nullptr && *pSplitter && i < bufLen - 1)
			{
				strResult.append(pSplitter);
			}
			if (lineLen > 0  && (i + 1) % lineLen == 0)
            {
                strResult += "\n";
            }
		}
		return strResult;
    }

    std::string SDAPI SDIntToHex(int32 num)
    {
        char szBuf[16] = {0};
        SDSnprintf(szBuf, sizeof(szBuf), "0x%x",num);
        return szBuf;
    }

    std::string  SDAPI SDStrTrimLeft(std::string & str, const char *trimStr )
    {
        for (std::string::iterator itr = str.begin(); itr != str.end(); ++itr)
        {
            if (strchr(trimStr,*itr) == nullptr)
            {
                str.erase(str.begin(), itr);
                break;
            }
        }
        return str;
    }

    std::string  SDAPI SDStrTrimRight(std::string & str, const char* trimStr )
    {
        for (std::string::reverse_iterator itr = str.rbegin(); itr != str.rend(); ++itr)
        {
            if (strchr(trimStr,*itr) == nullptr)
            {
                str.erase(itr.base(), str.end());
                break;
            }
        }
        return str;
    }

    std::string  SDAPI SDStrTrim(std::string & str, const char* pTrimStr)
    {
        SDStrTrimRight(str, pTrimStr);
        return SDStrTrimLeft(str, pTrimStr);
    }

    uint32 SDAPI SDAtou(const char* pStr)
    {
        uint32 result = 0;
        const char * pos = pStr;
        while (*pos != 0)
        {
            if (SDIsDigit(*pos) || *pos == '+')
            {
                uint32 d = *pos - '0';
                result = result * 10 + d;
            }
            else
            {
                return 0;
            }
            pos ++;
        }
        return result;
    }

	uint64 SDAPI SDAtou64(const char* pStr)
	{
		uint64 result = 0;
		const char * pos = pStr;
		while (*pos != 0 )
		{
			if (SDIsDigit(*pos) || *pos == '+')
			{
				uint64 d = *pos - '0';
				result = result * 10 + d;
			}
			else
			{
				return 0;
			}
			pos ++;
		}
		return result;
	}

	std::string ToString(int32 number)
	{
        return std::to_string(static_cast<int64>(number));
	}

	std::string ToString(uint32 number)
	{
        return std::to_string(static_cast<uint64>(number));
	}

	std::string ToString(int64 number)
	{
		return std::to_string(number);
	}

	std::string ToString(uint64 number)
	{
		return std::to_string(number);
	}

	std::string ToString(double number)
	{
        return std::to_string(static_cast<long double>(number));
	}
}


