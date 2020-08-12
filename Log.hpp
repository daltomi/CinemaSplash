/* ==================================================
	File name: Log.hpp

	Purpose:  Genera un archivo de secuencia de mensajes.

	Copyright (c) 2012, daltomi <daltomi@disroot.org>
 
	This file is part of CinemaSplash.

    CinemaSplash is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CinemaSplash is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CinemaSplash.  If not, see <http://www.gnu.org/licenses/>.
   ===================================================
*/

#pragma once

#include "Config.hpp"
#include "Singleton.hpp"

#include <fstream>
using std::ofstream;
using std::endl;

#include <string>
using std::string;

#ifdef WIN32
	#include <windows.h>
	#include <sstream>
#else
	#include <time.h>
#endif




class CLog  
{
public:
	explicit CLog();
	virtual ~CLog();
	void Write(const string&, const string&);

private:
	ofstream	m_ofile;
};

typedef Singleton<CLog> LogInstance;


