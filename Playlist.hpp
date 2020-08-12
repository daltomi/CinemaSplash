/* ==================================================
	File name: Playlist.hpp

	Purpose:  Busca los archivos de formatos y \
			  los guarda en una colección de archivos

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
#ifdef WIN32
	#include <windows.h>
#endif

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <string>
using std::string;

#include <algorithm>

#include <sstream>
using std::istringstream;

#include <cstring> // strlen

#include "Config.hpp"
#include "Log.hpp"

#ifndef WIN32
	#include <dirent.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
#endif

/*Comparar formato .abc */
class CmpFmt
{
    public:
        CmpFmt(const char* w) : word(w), size_w(strlen(w) ) { }
        bool operator()(const string& fmt) {
            if( word[size_w - 1] == fmt[4] &&
                word[size_w - 2] == fmt[3] &&
                word[size_w - 3] == fmt[2] &&
                word[size_w - 4] == fmt[1] )
            { 
                return true; 
            }
            return false;
        }
    private:
        const char* word;
        size_t size_w;
};






class CPlayList
{
	public:
		CPlayList();
		virtual ~CPlayList() { }

	bool FindFiles(string& sPath, const string& sFormat);

    /* INDEX */
	const size_t GetMaxItems() const;
	void DisableIndexItem(const unsigned int iTem); 
	void EnableIndexItem(const unsigned int  iTem);
    void SetIndexValue(const unsigned int  iTem, const int cValue);
    void SetIndexValue(const int cValue);
    const int GetIndexValue(const unsigned int  iTem) const;
	bool isEnableIndexItem(const unsigned int  iTem) const;
	void ResetIndexItems();

    /* DATA */
	const string& GetDataValue(const unsigned int  iTem) const;
    void SetDataValue(const string&) const;
	const string GetShortNameData(const unsigned int  iTem);
	const string& GetNextRandomItem();


private:

	vector<int> m_Index;
	vector<string> m_Data;

    static const int ENABLE;
    static const int DISABLE;

    void FindFiles(const string&, const vector<string>&);
	
protected:

    void VerifyDirSlash(string& sPath);
    bool isValidFormat(const char* pFileName, const vector<string> &vpTokensFmt);
    vector<string> TokenFormat(const string& sFmt, const char* );
};


