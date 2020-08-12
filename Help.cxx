/* ==================================================
    File name: Help.cxx

    Purpose:  Cuadro de dialogo para mostrar ayuda textual

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

#include "Help.hpp"


FXIMPLEMENT(CHelp,FXDialogBox,NULL,0)


/////////////////////////////////////////////////////////////////////////////
CHelp::CHelp(FXWindow *own,const FXString& ttl) : 
    FXDialogBox(own,ttl,DECOR_TITLE|DECOR_BORDER|DECOR_RESIZE|DECOR_CLOSE,100,100,0,400, 6,6,6,6, 4,4)
/////////////////////////////////////////////////////////////////////////////
{

  FXHorizontalFrame *closebox=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_FILL_X|PACK_UNIFORM_WIDTH);
  FXButton *button=new FXButton(closebox,tr("&Cerrar"),NULL,this,FXDialogBox::ID_ACCEPT,BUTTON_INITIAL|BUTTON_DEFAULT|LAYOUT_RIGHT|FRAME_RAISED,0,0,0,0, 20,20,5,5);

  FXHorizontalFrame *editbox=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK,0,0,0,0, 0,0,0,0);
  m_helpText=new FXText(editbox,NULL,0,TEXT_READONLY|TEXT_WORDWRAP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
  m_helpText->setVisibleRows(50);
  m_helpText->setVisibleColumns(90);
  button->setFocus();
}


///////////////////////////////////////////
void CHelp::setHelp(const FXString& help)
///////////////////////////////////////////
{
  m_helpText->setText(help);
}



/////////////////////////////////////
FXString CHelp::getHelp() const 
/////////////////////////////////////
{
  return m_helpText->getText();
}


////////////////
CHelp::~CHelp()
////////////////
{
  m_helpText=(FXText*)-1;
}

