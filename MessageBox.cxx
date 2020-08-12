/* ==================================================
    File name: MessageBox.cxx

    Purpose:  Registro de mensajes y alertas por pantalla

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

#include "MessageBox.hpp"



///////////////////////////////////////////////////////////////////////////
CMessageBox::CMessageBox() : 
    m_TitleApp(ConfigInstance::Get().Find(SECTIONS::SETTINGS, "Name") )
///////////////////////////////////////////////////////////////////////////
{ }


////////////////////////////
CMessageBox::~CMessageBox()
////////////////////////////
{ }



///////////////////////////////////////////////////////////////////////
void CMessageBox::ShowError( const string& msg)
///////////////////////////////////////////////////////////////////////
{
    FXMessageBox::error( ConfigInstance::Get().GetAppInstance(),
                        MBOX_OK, m_TitleApp.c_str() ,msg.c_str()
                        );
}




/////////////////////////////////////////////////////////////////////
void CMessageBox::ShowAlert(const string& msg)
/////////////////////////////////////////////////////////////////////
{

    FXMessageBox::warning( ConfigInstance::Get().GetAppInstance(),
                        MBOX_OK, m_TitleApp.c_str(), msg.c_str()
                        );
}



//////////////////////////////////////////////////////////
void CMessageBox::ShowInformation(FXWindow* parent,
                                  const string& msg)
//////////////////////////////////////////////////////////
{   
    parent->showCursor(true);
    FXMessageBox::information(parent , MBOX_OK, m_TitleApp.c_str() , msg.c_str() );
    parent->showCursor(false);
}


