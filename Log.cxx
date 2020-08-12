/* ==================================================
    File name: Log.cxx

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

#include "Log.hpp"



//////////////
CLog::CLog()
//////////////
{
    string filename = ConfigInstance::Get().GetTempDirectory();
    filename += ConfigInstance::Get().SEPARATOR + "cinema_splash_log.txt";

    m_ofile.open(filename.c_str() ,std::ios_base::app);

#ifdef WIN32    
    
    std::stringstream ss;
    SYSTEMTIME st;
    GetSystemTime(&st);
    ss << "Fecha: " << st.wDay << "/" << st.wMonth << "/" << st.wYear;
    m_ofile << "\n>----------------------------\n" << ss.str() << endl;

#else

    time_t tm;
    tm = time(NULL);
    m_ofile << "\n>----------------------------\n" <<  ctime(&tm) << endl;

#endif
    
}




//////////////
CLog::~CLog()
//////////////
{
    m_ofile << "----------------------------<" << endl;
    m_ofile.close();
}




//////////////////////////////////////////////////////////////
void CLog::Write(const string& name, const string& message)
//////////////////////////////////////////////////////////////
{
    m_ofile << "[*] " << name << ", " << message << endl;
}


