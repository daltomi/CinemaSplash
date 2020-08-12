/* ==================================================
    File name: IDevice.cxx

    Purpose:    Interfaz de VLC
                

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

#include "IDevice.hpp"
#include <cassert>
#include <iostream>

//////////////////////////////////////////
//void IDevice::SetVolume(unsigned int vol) { m_nivel_vol = vol; }


//////////////////////////////
void IDevice::SetVolumeDown() { --m_nivel_vol; }



////////////////////////////
void IDevice::SetVolumeUp() { ++m_nivel_vol; }



///////////////////////
void IDevice::Pause()
///////////////////////
{
    if(m_MediaPlayer != nullptr) {
        libvlc_media_player_pause(m_MediaPlayer);
    }
}



////////////////////////////
float IDevice::Get_Volume() 
////////////////////////////
{
    return  libvlc_audio_get_volume(m_MediaPlayer);
}


///////////////////////////////////
void IDevice::Set_Volume(float vol)
///////////////////////////////////
{
    if(vol >= 0) {
	  int ret = libvlc_audio_set_volume(m_MediaPlayer, vol); 
	  std::cout << "ret: " << ret << std::endl;
	  //assert(ret == 0);
    }
}



//////////////////////////////////
bool IDevice::isPlaying() const
//////////////////////////////////
{
    return ( m_MediaPlayer != nullptr && libvlc_media_player_is_playing(m_MediaPlayer) );
}

////////////////////////////
void IDevice::Play() const
////////////////////////////
{
    libvlc_media_player_play(m_MediaPlayer);
}

