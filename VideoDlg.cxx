/* ==================================================
    File name: VideoDlg.cxx

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


#include "VideoDlg.hpp"


FXDEFMAP(CVideoDlg) CVideoDlgMap[]={
    /* El botón se deshabilita si se está repr. video */
    FXMAPFUNC(SEL_COMMAND,  CVideoDlg::ID_SAVE, CVideoDlg::onSave),
    FXMAPFUNC(SEL_UPDATE,   CVideoDlg::ID_SAVE, CVideoDlg::onUpSave),

    /* La lista se deshabilita si se está repr. video */
    FXMAPFUNC(SEL_DOUBLECLICKED,  CVideoDlg::ID_LIST_VIDEO, CVideoDlg::onDoubleClick),
    FXMAPFUNC(SEL_UPDATE,   CVideoDlg::ID_LIST_VIDEO, CVideoDlg::onUpSave), // reciclado ID_SAVE
    
    /* La lista se deshabilita si se está repr. video */
    FXMAPFUNC(SEL_DOUBLECLICKED,  CVideoDlg::ID_LIST_PLAYLIST, CVideoDlg::onDoubleClick),
    FXMAPFUNC(SEL_UPDATE,   CVideoDlg::ID_LIST_PLAYLIST, CVideoDlg::onUpSave), // reciclado ID_SAVE
    
    /* Se deshabilita si no existen itemes en ID_LIST_PLAYLIST */   
    FXMAPFUNC(SEL_COMMAND, CVideoDlg::ID_PREVIEW, CVideoDlg::onPlayTest),
    FXMAPFUNC(SEL_UPDATE,  CVideoDlg::ID_PREVIEW, CVideoDlg::onUpPlayTest),
    
    /* Se deshabilita si no existen itemes en ID_LIST_PLAYLIST o si se está repr. vídeo */  
    FXMAPFUNC(SEL_COMMAND,  CVideoDlg::ID_CLEAR, CVideoDlg::onClear),
    FXMAPFUNC(SEL_UPDATE,   CVideoDlg::ID_CLEAR, CVideoDlg::onUpClear),
    
    FXMAPFUNC(SEL_COMMAND,  CVideoDlg::ID_REPLAY, CVideoDlg::onReplay),
    FXMAPFUNC(SEL_UPDATE,   CVideoDlg::ID_REPLAY, CVideoDlg::onUpClear), //reciclado ID_CLEAR

    FXMAPFUNC(SEL_COMMAND,  CVideoDlg::ID_HELP, CVideoDlg::onShowHelp),
    
    FXMAPFUNC(SEL_KEYPRESS, 0, CVideoDlg::onKeyPress),
    
    FXMAPFUNC(SEL_UPDATE,   CVideoDlg::ID_STATUSLINE, CVideoDlg::onUpStatusLine)
};



FXIMPLEMENT(CVideoDlg, FXDialogBox, CVideoDlgMap, ARRAYNUMBER(CVideoDlgMap))



//////////////////////////////////////////////////////////////////////////
CVideoDlg::CVideoDlg(FXWindow* parent, CVideo* pVideo, CAudio* pAudio) : 
    FXDialogBox(parent, "Trailer's", DECOR_TITLE | DECOR_RESIZE,0,0,380,400,0,0,0,0,0,0),
    m_pVideo(pVideo), m_pAudio(pAudio), m_Parent(parent)
//////////////////////////////////////////////////////////////////////////
{ 
    /* ICONOS */
    m_iconClear = new FXICOIcon(ConfigInstance::Get().GetAppInstance(), (const void*)clear_ico,0,0,0,0);
    m_iconSave  = new FXICOIcon(ConfigInstance::Get().GetAppInstance(), (const void*)save_ico,0,0,0,0);
    m_iconPlay  = new FXICOIcon(ConfigInstance::Get().GetAppInstance(), (const void*)play_ico,0,0,0,0);
    m_iconStop  = new FXICOIcon(ConfigInstance::Get().GetAppInstance(), (const void*)stop_ico,0,0,0,0);
    m_iconReplayEnable   = new FXICOIcon(ConfigInstance::Get().GetAppInstance(), (const void*)replay_enable_ico,0,0,0,0);
    m_iconReplayDisable  = new FXICOIcon(ConfigInstance::Get().GetAppInstance(), (const void*)replay_disable_ico,0,0,0,0);
    m_iconHelp  = new FXICOIcon(ConfigInstance::Get().GetAppInstance(), (const void*)help_ico,0,0,0,0);

    /* BARRA DE ESTADO */
    m_statusInfo = new FXStatusBar(this, LAYOUT_SIDE_BOTTOM | FRAME_THICK | LAYOUT_FILL_X );
    FXStatusLine* statusline = m_statusInfo->getStatusLine();
    statusline->setTarget(this);
    statusline->setSelector(ID_STATUSLINE);

    /* CONTENEDORES */
    FXComposite *contents = new FXVerticalFrame(this, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y);
    FXComposite *frame_list = new FXHorizontalFrame(contents,  LAYOUT_FILL_Y | LAYOUT_FILL_X );
    FXComposite *frame_button = new FXHorizontalFrame(contents, LAYOUT_SIDE_BOTTOM | LAYOUT_FILL_X | PACK_UNIFORM_WIDTH);

    /* GRUPOS */
    FXComposite *box_list = new FXGroupBox(frame_list, "Lista de Reproducción",FRAME_GROOVE | LAYOUT_FILL_Y | LAYOUT_FILL_X);
    FXComposite *box_video = new FXGroupBox(frame_list,"Videos disponibles   ",FRAME_GROOVE |  LAYOUT_FILL_Y | LAYOUT_FILL_X);

    /* CUADRO de LISTAS */
    m_ListPlaylist = new FXList(box_list,this, ID_LIST_PLAYLIST, LAYOUT_SIDE_LEFT | LAYOUT_FILL_X | LAYOUT_FILL_Y , LIST_SINGLESELECT);
    m_ListVideo = new FXList(box_video, this, ID_LIST_VIDEO, LAYOUT_SIDE_RIGHT | LAYOUT_FILL_X | LAYOUT_FILL_Y , LIST_SINGLESELECT);

    /* BOTONES */
    m_toggleReplay =  new FXToggleButton(frame_button,"Reiterar\t \tReiterar: desactivado","Reiterar\t \tReiterar: activado",m_iconReplayDisable,m_iconReplayEnable , this,ID_REPLAY,ICON_ABOVE_TEXT | JUSTIFY_CENTER_X | FRAME_RAISED);
    m_btnClear = new FXButton(frame_button, "Limpiar\t \tLimpiar lista de reproducción", m_iconClear, this, ID_CLEAR,ICON_ABOVE_TEXT | JUSTIFY_CENTER_X | FRAME_RAISED );
    m_togglePrev = new FXToggleButton(frame_button, "Previo\t  \tReproducir vista previa","Previo\t \tDetener reproducción", m_iconPlay ,m_iconStop, this, ID_PREVIEW,ICON_ABOVE_TEXT | JUSTIFY_CENTER_X | FRAME_RAISED );
    m_btnSave  = new FXButton(frame_button, "Guardar\t \tGuardar y salir", m_iconSave, this, ID_SAVE,ICON_ABOVE_TEXT | JUSTIFY_CENTER_X  | FRAME_RAISED );
    m_btnHelp  = new FXButton(m_statusInfo, "\t \tAyuda", m_iconHelp, this, ID_HELP,ICON_BEFORE_TEXT | JUSTIFY_RIGHT | LAYOUT_SIDE_RIGHT | FRAME_RAISED);
    

    //los botones con estilo toolbar
    SetStyleToolbar(m_btnHelp);

    // Predeterminado botón Guardar
    m_btnSave->setFocus();

    // Depende de CVideo y la configuración del registro
    m_toggleReplay->setState( m_pVideo->GetReplayList() );

    UpdateListVideos();
}



/////////////////////////
CVideoDlg::~CVideoDlg()
/////////////////////////
{ }




///////////////////////////////////////////
void CVideoDlg::UpdateListVideos()
// Protected
///////////////////////////////////////////
{
    FXString label;
    vector<string> PlaylistData;

    if( 1 == m_pVideo->GetMaxItems() ) {
        label.format("Total: %i video encontrado\n", m_pVideo->GetMaxItems() );
    } else {
        label.format("Total: %i videos encontrados\n",m_pVideo->GetMaxItems() );
    }
    m_statusInfo->getStatusLine()->setNormalText(label);
        
    if( !FilePlaylistInstance::Get().Read(PlaylistData) ) {
        SetState(INVALID);

        for(unsigned int i = 0UL; i < m_pVideo->GetMaxItems(); ++i) {
            m_ListVideo->appendItem( m_pVideo->GetShortNameData(i).c_str() );
        }
        const string msg("La lista de reproduccion esta vacia.");
        LogInstance::Get().Write("CVideoDlg::UpdateListVideos", msg);
        MsgBoxInstance::Get().ShowInformation(m_Parent,msg);
        return;
    } 
    unsigned int item=0UL;
    bool bFound = false;

    SetState(VALID);

    for( unsigned int ic = 0UL; ic < PlaylistData.size(); ic++) {
        for(item = 0UL; item < m_pVideo->GetMaxItems() ; item++) {

            if( !m_pVideo->isEnableIndexItem(item) && // para evitar compar otra vez
                PlaylistData[ic].compare( m_pVideo->GetDataValue(item) ) == 0 
              ) {
                m_ListPlaylist->appendItem( m_pVideo->GetShortNameData(item).c_str() );
                m_pVideo->EnableIndexItem(item); 
                bFound = true;
                break;
            }
        }
    }
    
    //Si no existe un archivo que esta en [PlayListCS], no importa
    // pero si era el único archivo entonces la lista esta vacia, se informa.
    if( !bFound && ( 1 == PlaylistData.size() ) ) {
        const string msg("La lista de reproduccion esta vacia.");
        LogInstance::Get().Write("CVideoDlg::UpdateListVideos", msg);
        MsgBoxInstance::Get().ShowInformation(m_Parent, msg);
    }

    for(item = 0UL; item < m_pVideo->GetMaxItems() ; item++) {
        if( !m_pVideo->isEnableIndexItem(item) ) {
             m_ListVideo->appendItem( m_pVideo->GetShortNameData(item).c_str() ); 
        } 
    }
}





///////////////////////////////////////////
bool CVideoDlg::isValidTrailer() const 
// Public
///////////////////////////////////////////
{ 
    return ( GetState() == VALID) ? true : false;
}






//////////////////////////////////////////////////////
long CVideoDlg::onSave(FXObject*, FXSelector, void*)
//////////////////////////////////////////////////////
{
    MakeTrailer();
    m_Parent->showCursor(false);
    m_Parent->update();
    ungrabKeyboard();
    hide();
}





/////////////////////////////////
void  CVideoDlg::MakeTrailer()
// Protected
/////////////////////////////////
{
    long nItems = m_ListPlaylist->getNumItems();

    if(nItems > 0) {
        std::vector<string> dataFile;
        SetState(VALID);
        m_pVideo->ResetIndexItems();
        
        for(unsigned int ic = 0UL; ic < nItems; ic++) {
            FXString str = m_ListPlaylist->getItemText(ic);
            for(unsigned int item = 0UL; item < m_pVideo->GetMaxItems(); item++) {
                if(m_pVideo->GetShortNameData(item).compare( str.text() ) == 0) {
                    dataFile.push_back(m_pVideo->GetDataValue(item) );
                    break;
                }
            }
        }
        FilePlaylistInstance::Get().Write(dataFile);
    } else {
        const string msg("La lista de reproduccion esta vacia.");
        SetState(INVALID);
        FilePlaylistInstance::Get().Delete();
        LogInstance::Get().Write("CVideoDlg::MakeTrailer", msg);
        MsgBoxInstance::Get().ShowInformation(m_Parent, msg);
    }
}





//////////////////////////////////////////////////////////
long CVideoDlg::onPlayTest(FXObject*, FXSelector, void*)
//////////////////////////////////////////////////////////
{
    MakeTrailer();

    if( m_pVideo && !m_pVideo->isPlaying() ) {
        m_pVideo->Start();
    } else {
        m_pVideo->Stop();
    }

    if(m_pAudio) { // ver CMainWindow::onTimeOut
        m_pAudio->Pause();
    }
    m_Parent->update();
}




///////////////////////////////////////////////////////
long CVideoDlg::onClear(FXObject*, FXSelector, void*)
///////////////////////////////////////////////////////
{
    m_pVideo->ResetIndexItems();
    m_ListVideo->clearItems();
    m_ListPlaylist->clearItems();

    for(unsigned int i = 0UL; i < m_pVideo->GetMaxItems(); ++i) {
        m_ListVideo->appendItem(m_pVideo->GetShortNameData(i).c_str() );
    }
}



//////////////////////////////////////////////////////////////////
long CVideoDlg::onUpClear(FXObject* sender, FXSelector, void*)
//////////////////////////////////////////////////////////////////
{
    if(0 == m_ListPlaylist->getNumItems() ||
            ( m_pVideo  && m_pVideo->isPlaying() ) 
      ) {
        sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_DISABLE),NULL);
    } else {
        sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_ENABLE),NULL);
    }
    return 1;
}





//////////////////////////////////////////////////////////////////
long CVideoDlg::onUpPlayTest(FXObject* sender, FXSelector, void*)
//////////////////////////////////////////////////////////////////
{
    if(0 == m_ListPlaylist->getNumItems() ) {
        sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_DISABLE),NULL);
    } else {
        sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_ENABLE),NULL);
        
        /* Actualiza cuando finaliza la lista de reproducción,
         * ver CMainWindow::onTimeOut
         * */
        if(m_pVideo && m_pVideo->isPlaying() ) {
            ((FXToggleButton*)sender)->setState(true);
        } else {
            ((FXToggleButton*)sender)->setState(false);
        }
    }
    return 1;
}




//////////////////////////////////////////////////////////////////
long CVideoDlg::onUpSave(FXObject* sender, FXSelector, void*)
//////////////////////////////////////////////////////////////////
{
    if( m_pVideo  && m_pVideo->isPlaying() ) {
        sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_DISABLE),NULL);
    } else {
        sender->handle(this, FXSEL(SEL_COMMAND, FXWindow::ID_ENABLE),NULL);
    }
    return 1;
}






//////////////////////////////////////////////////////////////////
long CVideoDlg::onReplay(FXObject* sender, FXSelector, void*)
//////////////////////////////////////////////////////////////////
{
    bool state = ( (FXToggleButton*)sender)->getState();

    if( m_pVideo) {
        m_pVideo->SetReplayList( state );
    }
    return 1;   
}





/////////////////////////////////////////////////////////////////////
long CVideoDlg::onDoubleClick(FXObject* obj, FXSelector sel, void*)
/////////////////////////////////////////////////////////////////////
{
    FXList* cut = (FXList*) obj;
    FXint cur_sel = cut->getCurrentItem();
    FXList* paste = (FXSELID(sel) == ID_LIST_VIDEO) ? m_ListPlaylist : m_ListVideo;
    FXListItem* item = cut->extractItem(cur_sel);
    paste->appendItem(item);
}




//////////////////////////////////////////////////////////////
long CVideoDlg::onKeyPress(FXObject*, FXSelector, void* ptr)
//////////////////////////////////////////////////////////////
{
    int key = ((FXEvent*)ptr)->code;
    if( key == KEY_Escape) { 
        onSave(NULL,0,NULL); 
        return 1;
    }
    return 0;
}




//////////////////////////////////////////////////////////////
long CVideoDlg::onUpStatusLine(FXObject*, FXSelector, void*)
//////////////////////////////////////////////////////////////
{
    return 1;
}




////////////////////////////////////////////////
void CVideoDlg::SetStyleToolbar(FXButton* btn)
////////////////////////////////////////////////
{
    btn->setButtonStyle( btn->getButtonStyle() | BUTTON_TOOLBAR);
}




//////////////////////////////////////////////////////////
long CVideoDlg::onShowHelp(FXObject*, FXSelector, void*)
//////////////////////////////////////////////////////////
{
    CHelp hp(this, "Lista de reproduccion - Ayuda");
    hp.setHelp(help);
    hp.execute(PLACEMENT_DEFAULT);
    return 1;
}


const char CVideoDlg::help[] =
"-----------------------------\n"
"CinemaSplash\n"
"-----------------------------\n"
"\n\n"
"-- Como manejar los cuadros de lista --\n\n"
"\tTanto el cuadro de \'Lista de reproducción\' como el cuadro de \'Videos disponibles\' se manejan con doble clic del mouse. Es decir, para agregar un ítem, o quitarlo, debe hacer doble clic sobre el.\n\n"
"-- Botones --\n\n"
"\t*El botón \'Reiterar\' activa la reproducción repetitiva de la lista.\n"
"\t*El botón \'Limpiar\' permite vaciar la lista de reproducción.\n"
"\tTenga en cuenta que es valido tener la lista vacía, solo que al momento de reproducir los tráiler's no sucederá nada, ni siquiera se dará un aviso en ese momento. Recuerde que CinemaSplash tiene una tecla para salir sin reproducir ningún tráiler, la tecla \'s\'.\n\n"
"\t*El botón \'Previo\' permite testear los tráiler's, es útil para determinar, por ejemplo, si algún vídeo esta corrupto, o la calidad de visualización del mismo.\n\n"
"\t*El botón \'Guardar\' se encarga de guardar la \'Lista de reproducción\' y sale.\n\n"
"\n\n"
;

