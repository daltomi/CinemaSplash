/* ==================================================
	 File name: Audio.cxx

	 Purpose:    Colección de archivos de audio.
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

#include "Audio.hpp"

//////////////////////
CAudio::CAudio() 
//////////////////////
{ }




////////////////////////////
bool CAudio::Initialize()
//protected
///////////////////////////
{
	 m_VLCInstance = libvlc_new(0,nullptr);

	 if( !m_VLCInstance ) {
		  const string msg("No pudo iniciar instancia de VLC");
		  LogInstance::Get().Write("CAudio::Initialize",msg);
		  MsgBoxInstance::Get().ShowError(msg);
		  return false;
	 }
	 std::istringstream iss( ConfigInstance::Get().GetConcatenateValues(SECTIONS::AUDIO_CFG) );
	 iss >>  m_delay_vol >>  m_steps_vol  >>  m_min_vol  >>  m_nivel_vol;  
	 return true;
}




///////////////////
CAudio::~CAudio()
///////////////////
{
	 Release();
}



/////////////////////////
void CAudio::Release() 
//public
/////////////////////////
{
#ifdef DEBUG
	 cerr << "Destroy CAudio" << endl;
#endif

	 libvlc_release(m_VLCInstance);
}



//////////////////////////////////
void CAudio::PlayNextRandom()
//public
//////////////////////////////////
{
	 if(isPlaying() ) {
		  Stop_Volume_Down();
	 }
	 Start();
}




//////////////////////
void CAudio::Start()
//protected
//////////////////////
{
   const string& file_name = GetNextRandomItem();

   libvlc_media_t *media  = libvlc_media_new_path(m_VLCInstance, file_name.c_str() );
   m_MediaPlayer = libvlc_media_player_new_from_media(media);
   
   if( nullptr == m_MediaPlayer) {
	  libvlc_media_release(media);
	  if( 1 != GetMaxItems() ) { // Protección NOP recursivo
			PlayNextRandom();
	  }
   }

   //libvlc_media_release(media);
   
   if( nullptr != m_MediaPlayer ) {
	  m_EventManager = libvlc_media_player_event_manager(m_MediaPlayer);
	  libvlc_event_attach(m_EventManager, libvlc_MediaPlayerEndReached , vlc_event, this);
  	//  libvlc_event_attach(m_EventManager, libvlc_MediaPlayerPlaying /*libvlc_MediaPlayerBuffering*/, vlc_event, this);
  	  //libvlc_event_attach(m_EventManager, libvlc_MediaPlayerVout, vlc_event, this);
  	 
	  Play();
	//  libvlc_state_t state;
    //  do {
    //      state = libvlc_media_player_get_state (m_MediaPlayer);
    //  } while(state == libvlc_Buffering);

  	//  SetVolumeInitial();
   	 }
}

void CAudio::SetVolumeInitial()
{
  Set_Volume(m_nivel_vol);
}



///////////////////////
void CAudio::Stop() 
//public
///////////////////////
{  
   if( nullptr != m_MediaPlayer && isPlaying() ) {
	  libvlc_event_detach(m_EventManager, libvlc_MediaPlayerEndReached | libvlc_MediaPlayerBuffering, vlc_event, this);
	  libvlc_media_player_stop(m_MediaPlayer);
	  libvlc_media_player_release(m_MediaPlayer);
	  m_MediaPlayer = nullptr;
	  std::cout << "CAudio::Stop" << std::endl;
   }
}





/////////////////////////////////
void CAudio::Stop_Volume_Down()
//public
/////////////////////////////////
{
	 if( isPlaying() ) {
		  float vol = m_nivel_vol - m_steps_vol;

		  while( isPlaying() && (vol > m_min_vol) ) {

				Set_Volume(vol);
		  std::cout << "vol: " << vol << std::endl;
				
				vol -= m_steps_vol;
#ifdef WIN32
				Sleep(m_delay_vol);
#else
				usleep(m_delay_vol * 1000);
#endif
		  };

	 }
	 Stop(); 
}

 

///////////////////////////////////////////////////////////////////////
bool CAudio::Create_PlayList(string& sPath,  const string& sFormat) 
//public
///////////////////////////////////////////////////////////////////////
{  
	 if( false == FindFiles(sPath,sFormat) ) {
		  const string msg("Error: No se encontraron archivos de audio para reproducir en:\n" + sPath);
		  LogInstance::Get().Write("CAudio::Create_Playlist",msg);
		  MsgBoxInstance::Get().ShowError( msg);
		  return false;
	 }
	 return true;
}



/////////////////////////////////////////////////////////////////
void CAudio::vlc_event(const libvlc_event_t* event, void* obj)
//private
/////////////////////////////////////////////////////////////////
{
   CAudio* pAudio = (CAudio*)obj;

   if ( pAudio == nullptr)  { return; }

   if (event->type == libvlc_MediaPlayerEndReached )
   {
	  pAudio->Stop();
	  pAudio->PlayNextRandom();
   }
   else if ( event->type == libvlc_MediaPlayerPlaying /*libvlc_MediaPlayerBuffering*/ )
   {
   	 pAudio->SetVolumeInitial();
   }
}


