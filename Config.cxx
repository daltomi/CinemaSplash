/* ==================================================
    File name: Config.cxx

    Purpose:   Constantes literales y manejo del registro

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
#include "Config.hpp"


#ifdef WIN32
        const string CConfig::SEPARATOR = "\\";
#else
        const string CConfig::SEPARATOR = "/";
#endif

////////////////////////////////////////////
CConfig::CConfig() : m_AppInstance(nullptr)
////////////////////////////////////////////
{ }



/////////////////////
CConfig::~CConfig()
/////////////////////
{ }



//////////////////////////////////////
void CConfig::Initialize(FXApp* app)
//////////////////////////////////////
{
    m_AppInstance = app;

    if( (m_bFirstRunTime = CheckRunFirstTime() ) == true ) {
        WriteDefault(); 
    }

}




/////////////////////////////////////
bool CConfig::isFirstRunTime() const
/////////////////////////////////////
{
    return m_bFirstRunTime;
}




/////////////////////////////////////
bool CConfig::CheckRunFirstTime()
/////////////////////////////////////
{
    Section section;

    if( true == Find(SECTIONS::SETTINGS, "RFT", section) ) {
        // Si no exite crea con sec.val(1), sino devuelve su valor == 1
        //XXX: si escribo registro con RFT a cero(0) debo cambiar aqui
        if( "1" == m_AppInstance->reg().readStringEntry(section.name, section.key, section.val) ) {
            return true;
        }
    }
    
    return false;
}



//////////////////////////////////////////////////////////////////
bool CConfig::Find(SECTIONS sec_type, FXchar* key, Section& ref_sec)
// Busca en el multimapa estatico.
//////////////////////////////////////////////////////////////////
{
    pair<MAP_SEC_ITER, MAP_SEC_ITER> range = gMapSectionDefault.equal_range(sec_type);
    
    while(range.first != range.second ) {
        ref_sec = (*range.first).second;

        if( FXString(ref_sec.key)  == key ) {
            return true;
        }
        ++range.first;
    }
    return false;
}



/////////////////////////////////////////////////////////////
const string CConfig::Find(SECTIONS sec_type, FXchar* key)
// Por ejemplo: Find(BITMAP, "Image");
//            Find(DIRECTORY, "Datos"); 
////////////////////////////////////////////////////////////
{
    pair<MAP_SEC_ITER, MAP_SEC_ITER> range = gMapSectionDefault.equal_range(sec_type);
    
    while(range.first != range.second ) {
         const Section& section = (*range.first).second;
         const FXchar* find =  m_AppInstance->reg().readStringEntry( section.name, key);
         
         if( nullptr != find ) {
            return string(find);
         }
         ++range.first;
    }
#ifdef DEBUG
    cerr << "** Error: <CConfig::Find[1,2]>: no se encontro " <<  key << endl;
#endif

    return string("");
}



///////////////////////////////////////////////////////////////
const string CConfig::GetConcatenateValues(SECTIONS sec_type)
// Concatena los valores de una seccion, mas un espacio
///////////////////////////////////////////////////////////////
{
    pair<MAP_SEC_ITER, MAP_SEC_ITER> range = gMapSectionDefault.equal_range(sec_type);
    string retStr;

    while(range.first != range.second ) {
        const Section& section = (*range.first).second;
        retStr += m_AppInstance->reg().readStringEntry( section.name, section.key);
        retStr += ' ';
        ++range.first;
    } 
    return retStr;
}



////////////////////////////////
void CConfig::WriteDefault()
////////////////////////////////
{
    for( auto map_sec : gMapSectionDefault ) {
        Section& sec = map_sec.second;
        m_AppInstance->reg().writeStringEntry( sec.name, sec.key, sec.val);
    }
    m_AppInstance->reg().write(); // forzamos a escribir.
}



/////////////////////////////////////////////
const string CConfig::GetCurrentDir() const
/////////////////////////////////////////////
{
    return FX::FXSystem::getCurrentDirectory().text();
}



///////////////////////////////////////////////
const string CConfig::GetTempDirectory() const
///////////////////////////////////////////////
{
    return FX::FXSystem::getTempDirectory().text();
}


/////////////////////////////////
int rand_int(int low, int high)
/////////////////////////////////
{
    static std::default_random_engine re {};
    using Dist = std::uniform_int_distribution<int>;
    static Dist uid {};
    return uid(re, Dist::param_type{low,high});
}

