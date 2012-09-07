/*
 * Copyright (C) 2012  Christian Mollekopf <mollekopf@kolabsys.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <curl/curl.h>
#include <string>

std::string uriEncode(const std::string &s)
{
    CURL *easyhandle = curl_easy_init();
    char *result = curl_easy_escape(easyhandle, s.c_str(), static_cast<int>(s.length()));
    curl_easy_cleanup(easyhandle);
    if (!result) {
        return std::string();
    }
    const std::string r(result);
    curl_free(result);
    return r;
}

std::string uriDecode(const std::string &s)
{
    CURL *easyhandle = curl_easy_init();
    int length = 0;
    char *result = curl_easy_unescape(easyhandle, s.c_str(), static_cast<int>(s.length()), &length);
    curl_easy_cleanup(easyhandle);
    if (!length) {
        return std::string();
    }
    const std::string r(result, length);
    curl_free(result);
    return r;
}
