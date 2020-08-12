/* ==================================================
    File name: MainWindow.hpp

    Purpose:  Ventana principal

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

#include <thread>

#include <fx.h>


#include "Picture.hpp"
#include "Audio.hpp"
#include "Video.hpp"
#include "VideoDlg.hpp"
#include "MessageBox.hpp"
#include "Help.hpp"
#include "Config.hpp"

class CMainWindow : public FXMainWindow
{
    FXDECLARE(CMainWindow)

    public:
        CMainWindow(FXApp*);
        virtual ~CMainWindow();
        long onPaint(FXObject*, FXSelector, void*);
        long onKeyPress(FXObject*, FXSelector, void*);
        long onTimeOut(FXObject*, FXSelector, void*);

        enum { 
            ID_TIMEOUT = FXWindow::ID_LAST,
            ID_LAST
        };

    protected:
        static const char help_intro[];

        CMainWindow() { }
        virtual void create();
        virtual void destroy();
        void ShowHelp();

    private:

        CPicture* m_pPicture;
        CAudio*   m_pAudio;
        CVideo*   m_pVideo;
        CVideoDlg* m_pVideoDlg;
        FXApp* m_App;
        bool   m_bMarkToExit;

        void SetMarkToExit(bool b){ m_bMarkToExit = b; }
        bool GetMarkToExit() const { return m_bMarkToExit; }

        void InitPicture();
        void InitAudio();
        bool InitVideo();
        void InitVideoDlg();
        void HideCursor();

        /*Acciones*/
        void Exit() const;
        void PlayMusic() const;
        void StopMusic() const;
        void ShowTrailers() const;
        void PlayAndExit();
};


