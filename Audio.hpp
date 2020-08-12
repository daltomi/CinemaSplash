/* ==================================================
	File name: Audio.hpp

	Purpose:	Colección de archivos de audio.
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

#ifdef WIN32
	#include <windows.h>
	#include <cstring>
#endif

#include "IDevice.hpp"
#include "Playlist.hpp"
#include "Config.hpp"
#include "MessageBox.hpp"
#include "Log.hpp"



class CAudio : public IDevice, public CPlayList 
{
public:
	CAudio();
	virtual ~CAudio();

    bool Initialize();
	void Release();
	void PlayNextRandom();
	void Start();
	void SetVolumeInitial();
	void Stop();
    void Stop_Volume_Down();
	bool Create_PlayList(string& ,  const string& );

protected:

	void PlayFile();
	static	void vlc_event(const libvlc_event_t*, void*);

};


