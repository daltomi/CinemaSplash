/* ==================================================
    File name: FilePlaylist.cxx

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

#include "FilePlaylist.hpp"

///////////////////////////////
CFilePlaylist::CFilePlaylist()
///////////////////////////////
{
    m_PathAbs = ConfigInstance::Get().GetCurrentDir();
    m_PathAbs += ConfigInstance::Get().Find(SECTIONS::DIRECTORY,"Datos");

    m_NameFile = m_PathAbs + ConfigInstance::Get().Find(SECTIONS::PLAYLIST, "Archivo");

    // actualizamos el directorio para "inicio" "fin"
    m_PathAbs += ConfigInstance::Get().Find(SECTIONS::DIRECTORY, "Videos") + ConfigInstance::Get().SEPARATOR;
    m_PathAbs += ConfigInstance::Get().Find(SECTIONS::DIRECTORY, "Ignorar") + ConfigInstance::Get().SEPARATOR;
    m_PathAbs += ConfigInstance::Get().Find(SECTIONS::DIRECTORY, "Privado") + ConfigInstance::Get().SEPARATOR;

}



///////////////////////////////////////////////
const string& CFilePlaylist::GetName() const
///////////////////////////////////////////////
{
    return m_NameFile;
}

        

//////////////////////////////
void CFilePlaylist::Delete()
//////////////////////////////
{
    FXString str = m_NameFile.c_str();
    FXFile::remove( str );
}




///////////////////////////////////////////////
bool CFilePlaylist::Read(vector<string>& out)
///////////////////////////////////////////////
{
    ifstream file( m_NameFile.c_str() );
    
    if( !file.is_open() ) {
        return false;
    }

    string data;
    const string sTopic = ConfigInstance::Get().Find(SECTIONS::TOPIC, "Presentacion");
    
    //Si el arch. playlist contiene lineas de presetacion,
    // se las ignora
    if( "1" == sTopic) {
        std::getline(file, data);
        data.empty();
    }

    while( !file.eof() ) {
        std::getline(file,data);
        out.push_back(data);
    }

    if( "1" == sTopic) {
        out.erase(out.end()-2, out.end() );
    }

    file.close();
    return true;
}




//////////////////////////////////////////////////
bool CFilePlaylist::ReadAll(vector<string>& out)
//////////////////////////////////////////////////
{
    ifstream file( m_NameFile.c_str() );
    
    if( !file.is_open() ) {
        return false;
    }

    string data;

    while( !file.eof() ) {
        std::getline(file,data);
        
        if( !data.empty() ) {
            out.push_back(data);
        }
    }

    file.close();
    return true;
}

    



//////////////////////////////////////////////////////
void CFilePlaylist::Write(const vector<string>& data)
//////////////////////////////////////////////////////
{
    ofstream ofile( m_NameFile.c_str() );
    ostream_iterator<string> opos( ofile,"\n"); //salto de linea(\n)
    std::vector<string>::const_iterator data_b = data.begin();
    std::vector<string>::const_iterator data_e = data.end();

    const string sTopic = ConfigInstance::Get().Find(SECTIONS::TOPIC, "Presentacion");
    
    //Si el arch. playlist contiene lineas de presetacion,
    if( "1" == sTopic) {
        *opos++ = m_PathAbs + ConfigInstance::Get().Find(SECTIONS::TOPIC,"Inicio"); 
    }

    while( data_b != data_e ) {
        *opos++ = *data_b++; // + se escribe salto de linea(\n)
    }
    
    if( "1" == sTopic) {
        *opos = m_PathAbs + ConfigInstance::Get().Find(SECTIONS::TOPIC,"Fin");
    }
    
    ofile.close();
}

