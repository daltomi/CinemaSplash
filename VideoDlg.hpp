/* ==================================================
    File name: VideoDlg.hpp

    Purpose:  Ventana para la creación de la lista de reproduccion

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
#include "Video.hpp"
#include "Audio.hpp"
#include "Config.hpp"
#include "icons.hpp"
#include "FilePlaylist.hpp"
#include "Help.hpp"

#ifdef DEBUG
    #include <iostream>
    using std::cerr;
    using std::endl;
#endif


class CVideoDlg : public FXDialogBox
{
    FXDECLARE(CVideoDlg)

    public:

        enum {
            ID_CLEAR = FXDialogBox::ID_LAST,
            ID_PREVIEW,
            ID_SAVE,
            ID_LIST_VIDEO,
            ID_LIST_PLAYLIST,
            ID_HELP,
            ID_STATUSLINE,
            ID_REPLAY,
            ID_LAST
        };

        CVideoDlg(FXWindow*, CVideo*, CAudio*);
        virtual ~CVideoDlg();
        long onDoubleClick(FXObject*, FXSelector, void*);
        long onSave(FXObject*, FXSelector, void*);
        long onClear(FXObject*, FXSelector, void*);
        long onKeyPress(FXObject*, FXSelector, void*);
        long onPlayTest(FXObject*, FXSelector, void*);
        long onUpStatusLine(FXObject*, FXSelector, void*);
        long onShowHelp(FXObject*, FXSelector, void*);
        long onReplay(FXObject*, FXSelector, void*);

        /* UPDATE */
        long onUpClear(FXObject*, FXSelector, void*);
        long onUpPlayTest(FXObject*, FXSelector, void*);
        long onUpSave(FXObject*, FXSelector, void*);
        long onUpReplay(FXObject*, FXSelector, void*);

        bool isValidTrailer() const;

    protected:
        CVideoDlg() { };
        CVideoDlg(const CVideoDlg& ) { };
        void UpdateListVideos();
        void MakeTrailer();
        void SetStyleToolbar(FXButton* );

        /*STATE*/
        enum TRAILER_STATE{ VALID, INVALID};
        void SetState(TRAILER_STATE state) {  m_State = state; }
        TRAILER_STATE GetState() const { return m_State; }
        TRAILER_STATE m_State;

        CVideo* m_pVideo;
        CAudio* m_pAudio;
        
        FXWindow* m_Parent;

        FXStatusBar* m_statusInfo;
        FXButton* m_btnClear;
        FXButton* m_btnSave;
        FXButton* m_btnHelp;
        FXToggleButton* m_togglePrev;
        FXToggleButton* m_toggleReplay;
        FXList*  m_ListVideo;
        FXList*   m_ListPlaylist;
        FXICOIcon*  m_iconSave;
        FXICOIcon*  m_iconPlay;
        FXICOIcon*  m_iconStop;
        FXICOIcon*  m_iconClear;
        FXICOIcon*  m_iconHelp;
        FXICOIcon*  m_iconReplayEnable;
        FXICOIcon*  m_iconReplayDisable;
        static const char help[];
};

