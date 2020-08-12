/* ==================================================
    File name: Playlist.cxx

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

#include "Playlist.hpp"



///////////////////////////////////////////////////////////////////////////
//                          CPlayList Impl.
///////////////////////////////////////////////////////////////////////////


const int CPlayList::ENABLE  = -1;
const int CPlayList::DISABLE = -2;


///////////////////////
CPlayList::CPlayList() 
///////////////////////
{
/*#ifdef WIN32
    srand( 0 );
#else
    srand( time(0) ); 
#endif*/
}




//////////////////////////////////////////////////////////////////////
bool CPlayList::FindFiles(string& sPath, const string& sFormat)
//////////////////////////////////////////////////////////////////////
// - Aqui se buscan *.* los archivos
// - Si se llama por 2nda vez se elimina los elementos de la lista.
// - Se filtran las coincidencias en base a sFormat
// - sFormat contiene extensiones, no nombres de archivos.
// - TODO: no permite buscar un archivo en particular en
//   un arbol de directorios.

{
    if( sPath.size() == 0 || sFormat.size() == 0 ) { 
        return false;
    }

    //Si se llama por segunda vez...
    if( m_Index.size() != 0 )   {  
        //...se limpia
        m_Index.clear();
        m_Data.clear();
    }

    vector<string> vTokenFmt = TokenFormat(sFormat,";");

    VerifyDirSlash(sPath);
    FindFiles(sPath, vTokenFmt);
    return ( GetMaxItems() == 0 ) ? false : true;
}




////////////////////////////////////////////////////////////////////
void CPlayList::FindFiles(const string& sPath, 
                                const vector<string>& vTokenFmt)
////////////////////////////////////////////////////////////////////
{
    vector<string> vsDir;   //contenedor de directorios
    unsigned int iDir = 0UL;
    const string sIgnore = ConfigInstance::Get().Find(SECTIONS::DIRECTORY, "Ignorar");
    const string ROOTBACK("..");
    const string HERE(".");

#ifdef WIN32
    
    const string ALLFILES("*.*");
    HANDLE hFile;
    WIN32_FIND_DATA tFindData;
    BOOL bNextFile = TRUE;
    
    vsDir.push_back( sPath + ALLFILES );

    while( iDir < vsDir.size() )    {
        hFile = FindFirstFile(vsDir[iDir].c_str(), &tFindData);
        
        while( TRUE == bNextFile ) {
            if( ( bNextFile = FindNextFile(hFile, &tFindData) ) && ( tFindData.cFileName != ROOTBACK ) ) {
                if(tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
                    if( strcmp(tFindData.cFileName, sIgnore.c_str() ) ) {
                        vsDir.push_back( vsDir[iDir].substr(0,vsDir[iDir].length() - 3) + 
                                         tFindData.cFileName + ConfigInstance::Get().SEPARATOR + ALLFILES 
                                       );
                    }
                } else {
                    if(true == isValidFormat(tFindData.cFileName,vTokenFmt) ) {
                       SetDataValue(vsDir[iDir].substr(0,vsDir[iDir].length() - 3) + tFindData.cFileName);
                       SetIndexValue(DISABLE);
                    }
                }
            }
        }//END_WHILE
        
        ++iDir;
        FindClose(hFile);
        bNextFile=TRUE;

    }//END_WHILE

#else //LINUX

    struct dirent* entry;
    struct stat st;

    vsDir.push_back( sPath );

    while( iDir < vsDir.size() )    {
        DIR* dir = opendir( vsDir[iDir].c_str() );

        while( 0 != dir && (entry = readdir(dir) ) != NULL ) {
            string str( vsDir[iDir] + entry->d_name);

            if( !str.compare(str.length()-1, 1, HERE) || 
                !str.compare(str.length()-1,2, ROOTBACK) ) {
                continue; 
            }

            lstat( str.c_str() , &st);

            if( S_ISDIR(st.st_mode) ) { //es un directorio?
                if( string::npos == str.find(sIgnore) ) { //si !encontrado sIgnore
                    vsDir.push_back( str + ConfigInstance::Get().SEPARATOR );
                }
            } else if(S_ISREG(st.st_mode) ) { // archivo regular
            
                if(true == isValidFormat(entry->d_name, vTokenFmt) ) {
                    SetDataValue(str);
                    SetIndexValue(DISABLE);

                }
            } else {
                const string msg("lstat indefinido= \n" + str);
                LogInstance::Get().Write("***CPlayList::FindFiles",msg);
            }

        } //END_WHILE
        
        ++iDir;
        closedir(dir);

    }//END_WHILE*/
#endif

/*	FXString* listFiles;
	FXint nfiles = 0;
	nfiles = FX::FXDir::listFiles(listFiles, sPath.c_str());
	for ( FXint i = 0; i < nfiles; ++i)
	{
	  if ( FX::FXStat::isFile(listFiles[i]) )
	  {
	  	if ( true == isValidFormat(listFiles[i].text(), vTokenFmt) )
	  	{
	  	  SetDataValue(std::string(listFiles[i].text() ));
	  	  SetIndexValue(DISABLE);
	  	}
	  }
	}*/
}

///////////////////////////////////////////////////////////
vector<string> CPlayList::TokenFormat(const string& sFmt,
                            const char* cFormat)
///////////////////////////////////////////////////////////
// - Protected
// - Separa el formato por el token(;) Ej *.avi;*.mpg;*.mkv;*.mp4
//    y lo coloca en el contenedor de formatos.
{
    string::size_type e = 0; //end
    string::size_type b = 0; //begin
    vector<string> ret;

    do {
        e = sFmt.find(cFormat,b); 
        ret.push_back( sFmt.substr(b, (e-b) ) );
        b = e + 1;
    } while( e < string::npos ); 

    return ret;
}




/////////////////////////////////////////////////////////////////////////////////////////
bool CPlayList::isValidFormat(const char* pFileName, const vector<string> &vpTokensFmt)
/////////////////////////////////////////////////////////////////////////////////////////
// - Protected
// - Verifica si el nombre del archivo coincide con 
//   algun elemento del contenedor de formatos.
{
    return ( vpTokensFmt.end() == find_if(vpTokensFmt.begin(), 
                                          vpTokensFmt.end(), 
                                          CmpFmt(pFileName) ) ) ? false : true;
}


////////////////////////////////////////////////
void CPlayList::VerifyDirSlash(string& sPath)
////////////////////////////////////////////////
// - Protected
// - Si el path no contiene el caracter \\
//   al final entonces lo coloca
{
    const size_t size = sPath.length() - 1;
#ifdef WIN32
    if(sPath.compare(size,1,"\\") ) {
        sPath += "\\";
    }
#else
    if(sPath.compare(size,1,"/") ) {
        sPath += "/";
    }

#endif
}



///////////////////////////////////////////// 
const size_t CPlayList::GetMaxItems() const
/////////////////////////////////////////////
// - Retorna el tamaño del contenedor de listas
{
    return m_Index.size();
}



//////////////////////////////////////////////
const string& CPlayList::GetNextRandomItem()
//////////////////////////////////////////////
// - Obtiene un elemento validado por m_cIndex == 0
// - Si no existen mas elementos, reinicia la lista
// - Funcion recursiva.
{
    //si solo exite 1 elemento
    if( GetMaxItems() == 1) { return  GetDataValue(0UL); } 

    unsigned int irandom = 0UL;
    static unsigned int ipermanent_count = 0UL;

    do {
        irandom = rand_int(0, GetMaxItems() - 1 );

        if( !isEnableIndexItem(irandom) ) {
            ++ipermanent_count; 
            EnableIndexItem(irandom);
            return GetDataValue(irandom);
        }

    } while(ipermanent_count < GetMaxItems() );

    ipermanent_count = 0UL;
    ResetIndexItems();
    GetNextRandomItem(); //call recursive
    return GetDataValue(0);//evitar warnning del compilador.
}




//////////////////////////////////////////////////////////
void CPlayList::SetDataValue(const string& str_data) const
//////////////////////////////////////////////////////////
{
    m_Data.push_back(str_data);
}




///////////////////////////////////////////////////////////////////////
const string& CPlayList::GetDataValue(const unsigned int iTem)  const
///////////////////////////////////////////////////////////////////////
// - Protected
// - Retorna el elemento por parametro
{
    return m_Data[iTem];
}



///////////////////////////////////////////////////////////////////
const int CPlayList::GetIndexValue(const unsigned int iTem) const
///////////////////////////////////////////////////////////////////
// - Protected
// - Retorna el elemento por parametro
{
    return m_Index[iTem];
}

    
///////////////////////////////////
void CPlayList::ResetIndexItems()
///////////////////////////////////
// - Reinicia la lista con m_cIndex = 0
//   para poder reproducir nuevamente la lista
{
    for(unsigned int i=0; i < GetMaxItems(); ++i) {
        DisableIndexItem(i);
    }
}


////////////////////////////////////////////////////////////
void CPlayList::DisableIndexItem(const unsigned int iTem)
////////////////////////////////////////////////////////////
// - Asigna el valor -1 a m_cIndex
{
   SetIndexValue(iTem, DISABLE);
}



///////////////////////////////////////////////////////////
void CPlayList::EnableIndexItem(const unsigned int iTem)
///////////////////////////////////////////////////////////
// - Asigna el valor 0 a m_cIndex
{
   SetIndexValue(iTem, ENABLE);
}



////////////////////////////////////////////////////////////////////////////  
void CPlayList::SetIndexValue(const unsigned int iTem, const int cValue)
////////////////////////////////////////////////////////////////////////////  
// - Permite asignar un valor distinto que
//   DisableIndexItem y EnableIndexItem
{
    m_Index[iTem] = cValue;
}




////////////////////////////////////////////////
void CPlayList::SetIndexValue(const int cValue)
////////////////////////////////////////////////
//- Crea un nuevo valor
{
    m_Index.push_back(cValue);
}



//////////////////////////////////////////////////////////////////
bool CPlayList::isEnableIndexItem(const unsigned int iTem) const
//////////////////////////////////////////////////////////////////
// - Retorna true si m_cIndex != -1 sino false
{
    if( GetIndexValue(iTem) == ENABLE ) {
        return true;
    }
    return  false;
}



////////////////////////////////////////////////////////////////////
const string CPlayList::GetShortNameData(const unsigned int iTem)
////////////////////////////////////////////////////////////////////
// - Devuelve el nombre del archivo descartando el path completo.
{
#ifdef WIN32
    size_t pos = GetDataValue(iTem).rfind("\\") + 1;
#else
    size_t pos = GetDataValue(iTem).rfind("/") + 1;
#endif
    return GetDataValue(iTem).substr(pos);
}


