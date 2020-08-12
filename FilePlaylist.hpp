/* ==================================================
    File name: FilePlaylist.hpp

    Purpose:  Maneja el archivo de lista de reproducción

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

#include "Singleton.hpp"
#include "Config.hpp"

#include <fstream>
using std::ifstream;
using std::ofstream;

#include <iterator>
using std::ostream_iterator;

#include <string>
using std::string;

#ifdef DEBUG
    #include <iostream>
    using std::cerr;
    using std::endl;
#endif

#include <fx.h>




class CFilePlaylist
{
    public:
        CFilePlaylist();
        virtual ~CFilePlaylist(){ };

        const string& GetName() const;
        void Delete();
        bool Read(vector<string>& );
        bool ReadAll(vector<string>& );
        void Write(const vector<string>& );

    private:

        string m_NameFile;
        string m_PathAbs;
};

typedef Singleton<CFilePlaylist> FilePlaylistInstance;

