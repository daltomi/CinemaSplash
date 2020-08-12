/* ==================================================
    File name: IDevice.hpp

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

#pragma once

#include <vlc/vlc.h>

class IDevice
{
    public:

        virtual void Start() = 0;
        virtual void Stop() = 0;

        void Play() const;
        //void SetVolume(unsigned int);
        void SetVolumeDown();
        void SetVolumeUp();
        void Pause();
        float Get_Volume();
        void Set_Volume(float);
        bool isPlaying() const;

    protected:

        IDevice() : 
            m_bPause(false),
            m_steps_vol(0.0f),
            m_delay_vol(0UL),
            m_min_vol(0.0f),
            m_nivel_vol(0.0f),
            m_VLCInstance(nullptr),
            m_MediaList(nullptr),
            m_MediaPlayer(nullptr),
            m_MediaListPlayer(nullptr),
            m_MediaEvt(nullptr)
    {   }

        libvlc_instance_t*      m_VLCInstance;
        libvlc_media_player_t*  m_MediaPlayer; 
        libvlc_event_manager_t* m_EventManager;
        libvlc_media_list_t*    m_MediaList; 
        libvlc_media_list_player_t* m_MediaListPlayer;
        libvlc_event_manager_t* m_MediaEvt;

        bool            m_bPause;
        unsigned short  m_delay_vol;

		float	m_steps_vol;
        float   m_min_vol;
        float   m_nivel_vol;
};



