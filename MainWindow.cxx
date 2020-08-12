/* ==================================================
    File name: MainWindow.cxx

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

#include "MainWindow.hpp"

FXDEFMAP(CMainWindow) CMainWindowMap[]={
    FXMAPFUNC(SEL_PAINT,        0, CMainWindow::onPaint),
    FXMAPFUNC(SEL_KEYPRESS,     0, CMainWindow::onKeyPress),
    FXMAPFUNC(SEL_TIMEOUT,      CMainWindow::ID_TIMEOUT, CMainWindow::onTimeOut)
};



FXIMPLEMENT(CMainWindow, FXMainWindow, CMainWindowMap, ARRAYNUMBER(CMainWindowMap))




///////////////////////////////////
CMainWindow::CMainWindow(FXApp* fxapp) : 
#ifndef DEBUG
    FXMainWindow(fxapp,"CinemaSplash Application",NULL,NULL,DECOR_NONE,0,0,800,600),
#else
    FXMainWindow(fxapp,"CinemaSplash Application",NULL,NULL,DECOR_ALL,0,0,480,400),
#endif
    m_App(fxapp), m_pPicture(nullptr), m_pAudio(nullptr), m_pVideo(nullptr), m_pVideoDlg(nullptr)
// CMainWindow
// Ctor.
///////////////////////////////////
{ 
    SetMarkToExit(false);
}



//////////////////////////
CMainWindow::~CMainWindow()
//////////////////////////
{ 
    ConfigInstance::Get().SAFE_RELEASE(m_pPicture);
    ConfigInstance::Get().SAFE_RELEASE(m_pAudio);
    ConfigInstance::Get().SAFE_RELEASE(m_pVideo);
    ConfigInstance::Get().SAFE_RELEASE(m_pVideoDlg);
}



///////////////////////////////
void CMainWindow::InitPicture()
///////////////////////////////
{
    m_pPicture = new CPicture();
    
    string pathAbs = ConfigInstance::Get().GetCurrentDir();
    pathAbs+= ConfigInstance::Get().Find(SECTIONS::DIRECTORY,"Datos");
    pathAbs+= ConfigInstance::Get().Find(SECTIONS::DIRECTORY,"Imagen");
    pathAbs+= ConfigInstance::Get().SEPARATOR;
    pathAbs+= ConfigInstance::Get().Find(SECTIONS::BITMAP,"Imagen");

    if( false == m_pPicture->Load(pathAbs) ) {
        ConfigInstance::Get().SAFE_RELEASE(m_pPicture);
    }
}



/////////////////////////////
void CMainWindow::InitAudio()
/////////////////////////////
{
    m_pAudio = new CAudio();

    if( true == m_pAudio->Initialize() ) {
        string pathAbs = ConfigInstance::Get().GetCurrentDir();
        pathAbs+= ConfigInstance::Get().Find(SECTIONS::DIRECTORY, "Datos");
        pathAbs+= ConfigInstance::Get().Find(SECTIONS::DIRECTORY,"Musica");
        
        const string formats = ConfigInstance::Get().Find(SECTIONS::FORMAT, "Audio");

        if( false == m_pAudio->Create_PlayList(pathAbs, formats) ) {
            ConfigInstance::Get().SAFE_RELEASE(m_pAudio);
        }
    } else {
        ConfigInstance::Get().SAFE_RELEASE(m_pAudio);
    }
}




/////////////////////////////
bool CMainWindow::InitVideo()
/////////////////////////////
{ 
    m_pVideo= new CVideo(this);

    string pathAbs = ConfigInstance::Get().GetCurrentDir();
    pathAbs+= ConfigInstance::Get().Find(SECTIONS::DIRECTORY, "Datos");
    pathAbs+= ConfigInstance::Get().Find(SECTIONS::DIRECTORY,"Videos");
    
    const string formats = ConfigInstance::Get().Find(SECTIONS::FORMAT, "Video");

    if( false == m_pVideo->Create_PlayList(pathAbs, formats) ) {
        ConfigInstance::Get().SAFE_RELEASE(m_pVideo);
        return false;
    }
    return true;
}





////////////////////////////////////////
inline void CMainWindow::InitVideoDlg()
////////////////////////////////////////
{
    m_pVideoDlg = new CVideoDlg(this, m_pVideo, m_pAudio);
}




//////////////////////////
void CMainWindow::create() 
// Toda inicializacion aqui
//////////////////////////
{ 
    FXMainWindow::create();
    InitPicture();
    InitAudio();
    
    if( true == InitVideo() ) {
        InitVideoDlg();
    }

#ifndef WIN32
        //LINUX ONLY
        show(PLACEMENT_MAXIMIZED | PLACEMENT_VISIBLE | PLACEMENT_SCREEN); 
#endif

    if( ConfigInstance::Get().isFirstRunTime() ) {
        ShowHelp();
    }

    showCursor(false);

    // 1 second = 1 000 000 000 nanoseconds
    // 1/2 "    =   500 000 000
    m_App->addTimeout(this, ID_TIMEOUT, 500000000, NULL);
}




//////////////////////////////////////////////////////////
long CMainWindow::onTimeOut(FXObject*, FXSelector, void*)
// Controla la lista de reproducción.
// Depende de la llamada a CVideo::vlc_event()
//////////////////////////////////////////////////////////
{
#ifdef DEBUG
    cerr << "TIMEOUT" << endl;
#endif

    if(m_pVideo && m_pVideo->isTheEndList() )
    {
#ifdef DEBUG
        cerr << "TIMEOUT CALL VIDEO STOP" << endl;
#endif
        m_pVideo->Stop();

        // Termina el programa
        // Ver KEY_space
        if(true == GetMarkToExit() ) { 
            close();
            return 0;
        }
        
        if(true == m_pVideo->GetReplayList() ) {
            m_pVideo->Start();
        } else {
            
            if(m_pAudio) {
                m_pAudio->Pause();
            }

            m_pVideoDlg->update();
            update();
        }
    }
    // 1 second = 1 000 000 000 nanoseconds
    // 1/2 "    =   500 000 000
    m_App->addTimeout(this, ID_TIMEOUT, 500000000, NULL);
    return 1;
}




////////////////////////////////////////////////////////////
long CMainWindow::onPaint(FXObject*, FXSelector, void* ptr) 
////////////////////////////////////////////////////////////
{
    if(m_pVideo && !m_pVideo->isPlaying() ) {
        FXEvent* ev = (FXEvent*)ptr;
        FXDCWindow dc(this,ev);
        dc.begin(this);
        if( m_pPicture && m_pPicture->isValid() ) {
            m_pPicture->Render(dc, getWidth(), getHeight() );
        } else {
            dc.setForeground( FXRGB(0,0,0) );
            dc.fillRectangle(0,0, getWidth(),getHeight());
        }
        dc.end();
    }
    return 1; 
}




///////////////////////////
void CMainWindow::destroy()
///////////////////////////
{
    HideCursor();

    m_App->removeTimeout(this, ID_TIMEOUT);
}




/////////////////////////////////////////
void CMainWindow::HideCursor()
//Posiciona el cursor al borde derecho
// de la pantalla
/////////////////////////////////////////
{
    showCursor(false);
    FXWindow* win = getApp()->getRootWindow();
    win->setCursorPosition(win->getWidth(), win->getHeight() / 2);
}




///////////////////////////////////////////////////////////////
long CMainWindow::onKeyPress(FXObject*, FXSelector, void* ptr)
///////////////////////////////////////////////////////////////
{
    //Si el cuadro de dialogo esta visible no se procesan
    // los eventos
    if(m_pVideoDlg && m_pVideoDlg->shown() ) {
        return 1;
    }

    const int key = ((FXEvent*)ptr)->code;

    // Protector de eventos si se está repr. video
    // Ha excepción de la tecla KEY_s
    if(key != KEY_s && (m_pVideo && m_pVideo->isPlaying() ) ) {
        return 1;
    }

    switch(key) {
        case KEY_s: { 
                Exit(); 
                break;
            }
        case KEY_m: {
                PlayMusic();
                break;
            }
        case KEY_d: {
                StopMusic();
                break;
            }
        case KEY_t: {
                ShowTrailers();
                break;
            }
        case KEY_a: {
                ShowHelp();
                break;
            }
        case KEY_space: {
                PlayAndExit();
                break;
            }
    }
    return 1;
}



///////////////////////////////
void CMainWindow::Exit() const
///////////////////////////////
{
    if(m_pAudio ) {
        m_pAudio->Stop_Volume_Down(); 
    }
              
    if(m_pVideo && m_pVideo->isPlaying() ) {
          m_pVideo->Stop();
    }
    close();
}



////////////////////////////////////
void CMainWindow::PlayMusic() const
////////////////////////////////////
{
#ifndef DEBUG
    grabKeyboard();//ver BUG#4
#endif

    if(m_pAudio) { 
        m_pAudio->PlayNextRandom(); 
    }
    ungrabKeyboard(); //ver BUG#4
}



////////////////////////////////////
void CMainWindow::StopMusic() const
////////////////////////////////////
{
    if(m_pAudio) {
        m_pAudio->Stop_Volume_Down(); 
    }
}




///////////////////////////////////////
void CMainWindow::ShowTrailers() const
///////////////////////////////////////
{
    if(m_pVideoDlg) { 
        showCursor(true);
        m_pVideoDlg->show(PLACEMENT_OWNER);
        m_pVideoDlg->grabKeyboard(); 
    }
}



////////////////////////////////
void CMainWindow::PlayAndExit()
////////////////////////////////
{
    HideCursor();

    if(m_pAudio && m_pAudio->isPlaying() )  {
        m_pAudio->Stop_Volume_Down(); 
    }

    ConfigInstance::Get().SAFE_RELEASE(m_pAudio);
    
    // Debe siempre estar antes
    // de Play()
    SetMarkToExit(true);

    if(not m_pVideoDlg->isValidTrailer() )  {
        close();
        return;
    }

    // onTimeOut se encarga de detener
    // la repr. y salir.
    if(m_pVideo )   {
        m_pVideo->Start();
    }
}



////////////////////////////////////
inline void CMainWindow::ShowHelp()
////////////////////////////////////
{

    string title("CinemaSplash ");
    title += VERSION;
    title += " - Ayuda";

    showCursor(true);

    CHelp hp(this, title.c_str() );
    hp.setHelp( help_intro );
    hp.execute(PLACEMENT_OWNER);
    showCursor(false);
}





const char CMainWindow::help_intro[] =
"-----------------------------\n"
"CinemaSplash\n"
"-----------------------------\n"
"\n\n"
"-- Introducción --\n"
"\t* Reproduce una lista de vídeos(tráiler's) seleccionados a trevés de un cuadro de diálogo.\n"
"\t* Permite mostrar una imagen de fondo en pantalla completa.\n"
"\t* Reproduce música aleatoriamente.\n"
"\n\n"
"-- Atajos de teclado (en minúsculas) --\n\n"
"\t* Tecla \"A\" = ayuda\n"
"\t* Tecla \"M\" = música\n"
"\t* Tecla \"T\" = selección de tráiler's\n"
"\t* Tecla \"S\" = salir\n"
"\t* Tecla \"Espacio\" = reproduce los tráiler's\n"
"\n\n"
"¿Cómo obtener ayuda?\n"
"\t* Tecla \"A\" (ayuda):\n\tMuestra el cuadro de diálogo de ayuda actual.\n\n"
"¿Cómo reproducir un archivo de audio?\n"
"\t* Tecla \"M\" (música):\n\tComienza a reproducir un archivo de audio(si existe alguno) de forma aleatoria. Esto sucede automáticamente cada vez que algún tema musical termina, o cada vez que se presiona nuevamente la tecla. Tenga presente que en cada cambio de tema musical"
" el volumen disminuye paulatinamente a cero antes de comenzar con otro tema.\n\n"
"¿Cómo crear la lista de reproducción de tráiler's?\n"
"\t* Tecla \"T\" (tráiler):\n\tMuestra un cuadro de diálogo en el cual permite crear la lista de"
" de reproducción. Para mas información use la ayuda del propio cuadro de dialogo, botón Ayuda.\n\n"
"¿Cómo salgo de CinemaSplash sin reproducir ningún tráiler?\n"
"\t* Tecla \"S\" (salir):\n\tFinaliza CinemaSplash. Si se está reproduciendo algún tema musical, el volumen disminuirá paulatinamente a cero y luego CinemaSplash se cerrará.\n\n"
"¿Cómo comienzo a reproducir los tráiler's de mi lista?\n"
"\t* Tecla de \"Espacio\":\n\tComienza a reproducir los videos de tráiler's(según la lista de reproducción) y luego CinemaSplash finaliza. Tenga presente que si se está reproduciendo algún tema musical, el volúmen disminuirá paulatinamente a cero antes de comenzar a reproducir los videos de tráiler's. Luego que termine de reproducirse la lista de tráiler's, CinemaSplash se cerrará.\n\n"
"¿Qué es un tráiler?\n"
"\tUn tráiler es un pequeño extracto de las imágenes de una película, en forma de video, que se estará estrenando próximamente. Tienen una duración promedio de entre un minuto y medio y dos minutos, suelen acompañarse con música o bien con su sonido original.(fuente http://www.definicionabc.com)\n\n"
"¿Dónde puedo conseguir un tráiler?\n"
"\tGeneralmente en sitios especializados en cine o directamente en youtube.com\n\n"
"¿CinemaSplash puede reproducir vídeos de mayor duración que un tráiler?\n"
"\tSi. CinemaSplash utiliza VLC para el manejo de audio y video, así que podría usarlo para reproducir una película, pero no es la finalidad de CinemaSplash.\n\n"
"¿Existe algun archivo que guarde los errores que suceden?\n"
"\tSi. El archivo se llama cinema_splash_log.txt, y se encuentra en el directorio temporal, en Linux puede ser /tmp y en Windows es el directorio temporal de usuario, por ejemplo para el usuario DIDI seria: Documents and Settings/DIDI/Configuración local/Temp.\n\n"
"¿Como puedo configurar CinemaSplash?\n"
"\tSe utiliza un registro para las configuraciones. En Linux se encuentra en ~/.config/CinemaSplash.rc\n\n"
"¿En que directorio coloco la imagen para ser usada de fondo?\n"
"\tEn el directorio /Datos/Imagen. Puede colocar todas las imágenes que desee aquí, CinemaSplash cargará sólo la que se nombre en el registro. Los formatos soportados son: bmp, jpg, png.\n\n"
"¿En que directorio coloco los archivos de audio?\n"
"\tEn el directorio /Datos/Musica. Coloque todos los archivos de audio allí y CinemaSplash se encargará de buscarlos y crear una lista de reproducción privada. Puede contener directorios y sub-directorios.\n\n"
"¿En que directorio coloco los vídeos de tráiler's?\n"
"\tEn el directorio /Datos/Videos.\n\n"
"\n\n"
"-- Errores y problemas conocidos --\n\n"
"# Se informa que la lista de tráiler's está vacía\n"
"\tSirve para recordarle que si presiona la tecla para reproducir la lista de tráiler's no sucederá nada, ya que no existe dicha lista, CinemaSplash terminará como es habitual.\n\n"
;
