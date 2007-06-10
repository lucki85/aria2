/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#include "CookieParser.h"
#include "Util.h"

void CookieParser::setField(Cookie& cookie, const string& name, const string& value) const
{
  if(name.size() == string("secure").size() &&
     strcasecmp(name.c_str(), "secure") == 0) {
    cookie.secure = true;
  } else if(name.size() == string("domain").size() && strcasecmp(name.c_str(), "domain") == 0) {
    cookie.domain = value;
  } else if(name.size() == string("path").size() && strcasecmp(name.c_str(), "path") == 0) {
    cookie.path = value;
  } else if(name.size() == string("expires").size() && strcasecmp(name.c_str(), "expires") == 0) {
    cookie.expires = Util::httpGMT(value);
    cookie.onetime = false;
  } else {
    cookie.name = name;
    cookie.value = value;
  }
}

Cookie CookieParser::parse(const string& cookieStr) const
{
  Cookie cookie;
  Strings terms;
  Util::slice(terms, Util::trim(cookieStr), ';', true);
  for(Strings::iterator itr = terms.begin(); itr != terms.end(); itr++) {
    pair<string, string> nv;
    Util::split(nv, *itr, '=');
    setField(cookie, nv.first, nv.second);
  }
  return cookie;
}


Cookies CookieParser::parse(istream& s) const
{
  Cookies cookies;
  string line;
  while(getline(s, line)) {
    if(Util::trim(line) == "" || Util::startsWith(line, "#")) {
      continue;
    }
    Cookie cookie = parse(line);
    if(cookie.good()) {
      cookies.push_back(cookie);
    }
  }
  return cookies;
}

