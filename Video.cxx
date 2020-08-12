/* ==================================================
    File name: Video.cxx

    Purpose:    Colección de archivos de videos.
                Reproduce lista

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

#include "Video.hpp"


////////////////////////////////////////////////////////////////////////////////
CVideo::CVideo(FXWindow* parent) : 
    m_WinParent(parent)
////////////////////////////////////////////////////////////////////////////////
{
    SetState(STOP);

    const string argument = ConfigInstance::Get().Find(SECTIONS::PLAYER,"vlc_argv");
    m_argv = TokenFormat(argument, " ");

    for(int c = 0 ; c < m_argv.size(); ++c ) {
        m_ptr_argv.push_back( &(m_argv[c][0]) );
    }

#ifdef WIN32
    FXSystem::setEnvironment("VLC_PLUGIN_PATH", ConfigInstance::Get().GetCurrentDir().c_str() );
#endif

    m_VLCInstance = libvlc_new(m_ptr_argv.size(), &m_ptr_argv[0]);
    m_MediaPlayer = libvlc_media_player_new(m_VLCInstance);

    m_MediaEvt = libvlc_media_player_event_manager(m_MediaPlayer);
    libvlc_event_attach(m_MediaEvt,libvlc_MediaPlayerEndReached, vlc_event, this);
    
    SetReplayList( ConfigInstance::Get().Find(SECTIONS::PLAYLIST,"Reiterar") );
}


//////////////////////////////////
bool CVideo::isTheEndList() const
//////////////////////////////////
{
   return ( VIDEO_STATE::END == GetState() ) ? true : false;
}


/////////////////
CVideo::~CVideo() 
/////////////////
{ 
#ifdef DEBUG
    cerr << "Destroy CVideo" << endl;
#endif
    
    Release();
}



///////////////////////////////////////////////////////////////////////
bool CVideo::Create_PlayList(string& sPath,  const string& sFormat) 
///////////////////////////////////////////////////////////////////////
{  
    if( false == FindFiles(sPath,sFormat) ) {
        const string msg("No se encontraron archivos de video para reproducir en:\n" + sPath);
        LogInstance::Get().Write("CVideo::Create_Playlist",msg);
        MsgBoxInstance::Get().ShowError(msg);
        return false;
    }
    return true;
}



//////////////////////////////////////////
void CVideo::Start()
// Crea una lista de repr. via API VLC.
//////////////////////////////////////////
{
    if( isPlaying() ) {
        return;
    }

    libvlc_media_t *media;
    
    m_MediaList = libvlc_media_list_new(m_VLCInstance);
    m_MediaListPlayer = libvlc_media_list_player_new(m_VLCInstance);


    /* - No leer desde el ctor. de la clase,
     *  ya que el archivo puede cambiar desde
     *  CVideoDlg.
     */
    vector<string> vData;
    FilePlaylistInstance::Get().ReadAll(vData);

    for( auto it : vData) {
        const char* file = it.c_str();
        media = libvlc_media_new_path(m_VLCInstance, file);
        libvlc_media_list_add_media(m_MediaList, media);
        libvlc_media_release(media);
    }

    m_CountItems = vData.size();
    m_CountEvents = 0;   
    SetState(STOP);

    libvlc_media_list_player_set_media_list(m_MediaListPlayer, m_MediaList);
    libvlc_media_list_player_set_media_player(m_MediaListPlayer, m_MediaPlayer);

#ifdef WIN32
    libvlc_media_player_set_hwnd(m_MediaPlayer, m_WinParent->id() );
#else
    libvlc_media_player_set_xwindow(m_MediaPlayer, m_WinParent->id() );
#endif

    const char* m_valChroma = ConfigInstance::Get().Find(SECTIONS::PLAYER,"vlc_chroma").c_str();

    libvlc_video_set_format(m_MediaPlayer, m_valChroma, m_WinParent->getWidth(), m_WinParent->getHeight(), m_WinParent->getWidth() * 2);
    libvlc_media_list_player_play(m_MediaListPlayer);
    wait_playing(m_MediaListPlayer);
}


///////////////////////////////////////////////////////////////////
void CVideo::wait_playing(libvlc_media_list_player_t *mp)
///////////////////////////////////////////////////////////////////
{
    libvlc_state_t state;
    do {
        state = libvlc_media_list_player_get_state (mp);
    } while(state != libvlc_Playing &&
            state != libvlc_Error &&
            state != libvlc_Ended );
}




////////////////////
void CVideo::Stop()
////////////////////
{
    libvlc_media_player_stop(m_MediaPlayer);
    libvlc_media_list_player_stop(m_MediaListPlayer);
    libvlc_media_list_player_release(m_MediaListPlayer);
    libvlc_media_list_release(m_MediaList);
    SetState(STOP);
}



////////////////////////
void CVideo::Release()
////////////////////////
{   
    libvlc_event_detach(m_MediaEvt, 0, vlc_event, NULL);
    libvlc_media_player_release(m_MediaPlayer);
    libvlc_release(m_VLCInstance);
    m_MediaPlayer = nullptr;
}




//////////////////////////////////////////////////////////////////////////////
void CVideo::vlc_event(const libvlc_event_t* event, void* object)
// - Ver CMainWindow, evento Timer
//////////////////////////////////////////////////////////////////////////////
{
    if( event->type == libvlc_MediaPlayerEndReached) {
        CVideo* pVideo = (CVideo*)object;
        if( ++pVideo->m_CountEvents == pVideo->m_CountItems) {
            pVideo->SetState(CVideo::VIDEO_STATE::END);
        }
    }
}



//////////////////////////////////////////////////
void CVideo::SetReplayList(const string& replay) 
// public
// Utilizado por ConfigInstance en el ctor.
//////////////////////////////////////////////////
{
    cerr << "***CVideo::SetReplayList: todavía no se guarda en el registro" << endl;
    m_bReplayList = (replay == "1") ? true : false; 
}



////////////////////////////////////////
void CVideo::SetReplayList(bool replay)
// public
////////////////////////////////////////
{
    cerr << "***CVideo::SetReplayList: todavía no se guarda en el registro" << endl;
    m_bReplayList = replay;
}



////////////////////////////////////
bool CVideo::GetReplayList() const 
// public
////////////////////////////////////
{ 
    return m_bReplayList; 
}


