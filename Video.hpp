/* ==================================================
	File name: Video.hpp

	Purpose:	Colección de archivos de videos.
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


#pragma once

#include <fx.h>
#include "IDevice.hpp"
#include "Playlist.hpp"
#include "Config.hpp"
#include "Log.hpp"
#include "MessageBox.hpp"
#include "FilePlaylist.hpp"

#include <sstream>


class CVideo  : public IDevice, public CPlayList
{
public:
    
    enum VIDEO_STATE {
        STOP,
        END
    };

	CVideo(FXWindow*);
	virtual ~CVideo();
	bool Create_PlayList(string& ,  const string& );
	void Release();
    void Start();
	void Stop();
	void SetReplayList(const string&);
	void SetReplayList(bool);
	bool GetReplayList() const;

    bool isTheEndList() const;


private:

	unsigned int m_CountEvents;
	unsigned int m_CountItems;

	FXWindow* m_WinParent;

	VIDEO_STATE m_Status;
	bool m_bReplayList;
	vector<const char*> m_ptr_argv;
	vector<string> m_argv; 

	void SetState(VIDEO_STATE state){  m_Status = state; }
	VIDEO_STATE GetState() const { return m_Status; }
	static void wait_playing(libvlc_media_list_player_t*);
	static void vlc_event(const libvlc_event_t*, void*);
};



