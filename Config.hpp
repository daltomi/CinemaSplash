/* ==================================================
    File name: Config.hpp

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
#pragma once

#include <fx.h>

#include "Singleton.hpp"

#include <string>
using std::string;

#include <random>

#include <iostream>
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include <map>
using std::map;
using std::multimap;
using std::pair;

#include <algorithm>

//TODO: cambiar para cada version del CinemaSplash
static const char* VERSION = "3.3";

struct Section
{
    const FXchar* name;
    const FXchar* key;
    const FXchar* val;
};

enum class SECTIONS {
    SETTINGS=0,     
    FORMAT,
    BITMAP,         
    TOPIC,
    AUDIO_CFG,       
    PLAYER,
    DIRECTORY,
    PLAYLIST
};


using MAP_SEC = multimap<SECTIONS, Section>;
using MAP_SEC_ITER = MAP_SEC::const_iterator;

static const MAP_SEC gMapSectionDefault{
    { SECTIONS::SETTINGS,   { "SETTINGS", "Name","CinemaSplash"} },
    { SECTIONS::SETTINGS,   { "SETTINGS", "RFT", "1"} },
    { SECTIONS::SETTINGS,   { "SETTINGS", "Version", VERSION} },
    { SECTIONS::SETTINGS,   { "SETTINGS", "normalfont", "helvetica,80,bold,normal"} },
    { SECTIONS::FORMAT,     { "FORMAT", "Audio", "*.mp3;*.wav;*.ogg"} },
    { SECTIONS::FORMAT,     { "FORMAT", "Video", "*.avi;*.mpg;*.mp4;*.mkv;*.wmv;*.flv;*.ogv;*.ogg;*.wbm"} },
    { SECTIONS::BITMAP,     { "BITMAP", "Imagen", "cinema_splash.png"} },
    { SECTIONS::TOPIC,      { "TOPIC","Presentacion", "1"} },
    { SECTIONS::TOPIC,      { "TOPIC","Inicio", "cinema_splash_first.avi"} },
    { SECTIONS::TOPIC,      { "TOPIC","Fin", "cinema_splash_last.avi"} },
    { SECTIONS::AUDIO_CFG,  { "AUDIO_CFG", "Retraso", "45"} },
    { SECTIONS::AUDIO_CFG,  { "AUDIO_CFG", "Pasos", "1"} },
    { SECTIONS::AUDIO_CFG,  { "AUDIO_CFG", "Min.Vol", "5"} },
    { SECTIONS::AUDIO_CFG,  { "AUDIO_CFG", "Nivel","50"} },
#ifdef WIN32
    { SECTIONS::PLAYER,     { "PLAYER", "vlc_argv", "--fullscreen --swscale-mode=1  --no-mouse-events --no-video-title-show --mouse-hide-timeout=0 --no-osd --vout=direct3d --no-plugins-cache"} },
#else
    { SECTIONS::PLAYER,     { "PLAYER", "vlc_argv", "--fullscreen --swscale-mode=1  --no-mouse-events --no-video-title-show --mouse-hide-timeout=0 --no-osd --no-plugins-cache"} },
#endif  
    { SECTIONS::PLAYER,     { "PLAYER", "vlc_chroma", "RV16"} },
    { SECTIONS::DIRECTORY,  { "DIRECTORY", "Musica", "Musica"} },
    { SECTIONS::DIRECTORY,  { "DIRECTORY", "Videos", "Videos"} },
    { SECTIONS::DIRECTORY,  { "DIRECTORY", "Imagen", "Imagen"} },
#ifdef WIN32
    { SECTIONS::DIRECTORY,  { "DIRECTORY", "Datos", "\\Datos\\"} },
#else
    { SECTIONS::DIRECTORY,  { "DIRECTORY", "Datos", "/Datos/"} },
#endif
    { SECTIONS::DIRECTORY,  { "DIRECTORY", "Ignorar", "[IGNORAR]"} },
    { SECTIONS::DIRECTORY,  { "DIRECTORY", "Privado", "[PRIVADO]"} },
    { SECTIONS::PLAYLIST,   { "PLAYLIST",  "Reiterar", "0"} },
    { SECTIONS::PLAYLIST,   { "PLAYLIST",  "Archivo", "[PlayListCS]"} }
};


// Random number generator
int rand_int(int low, int high);



class CConfig
{   
    public:
    
        CConfig();
        virtual ~CConfig();
        void Initialize(FXApp* );
        const string GetConcatenateValues(SECTIONS);
        const string Find(SECTIONS , FXchar*);
        const string GetCurrentDir() const;
        const string GetTempDirectory() const;
        bool isFirstRunTime() const;
        FXApp* GetAppInstance() const { return m_AppInstance; }
        void WriteDefault();
    

        template<typename T>
            inline void SAFE_RELEASE(T* & ptr) {
            if( nullptr != ptr ) {
                delete ptr;
                ptr = nullptr;
            }
        }   

#ifdef WIN32
        static const string SEPARATOR;
#else
        static const string SEPARATOR;
#endif

    private:
        
        FXApp* m_AppInstance;
        bool   m_bFirstRunTime;
        bool CheckRunFirstTime();
        bool Find(SECTIONS , FXchar*, Section& );
};

typedef Singleton<CConfig> ConfigInstance;
        
