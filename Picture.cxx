/* ==================================================
    File name: Picture.cxx

    Purpose: Maneja los archivos de imágenes.
             Permite dibujar la imágen en ventana.

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

#include "Picture.hpp"


////////////////////////////////////////
CPicture::CPicture() : m_pBitmap(0)
////////////////////////////////////////
{ }


//////////////////////
CPicture::~CPicture()
//////////////////////
{
#ifdef DEBUG
    cerr << "Destroy CPicture" << endl;
#endif
    ConfigInstance::Get().SAFE_RELEASE(m_pBitmap);
}



///////////////////////////////////////////////
bool CPicture::Load(const string& sNameFile)
///////////////////////////////////////////////
{
    FXFileStream File;
    
    if( !File.open(sNameFile.c_str() ) ) {
        const string msg("Imagen no encontrada:\n" + sNameFile);
        LogInstance::Get().Write("CPicture::Load",msg);
        MsgBoxInstance::Get().ShowError( msg);
        return false;
    }    

    const FXuint Flags = IMAGE_OWNED | IMAGE_KEEP;
    const string sExt = sNameFile.substr( sNameFile.length() - 3, 3);

    /* 
     * - Estos son los formatos soportados: png, bmp, jpg
     */
    if( sExt == "png") {
        m_pBitmap = new FXPNGImage(ConfigInstance::Get().GetAppInstance(), NULL, Flags);
    } else if( sExt == "bmp")   {
        m_pBitmap = new FXBMPImage(ConfigInstance::Get().GetAppInstance(), NULL, Flags);
    } else if( sExt == "jpg")   {
        m_pBitmap = new FXJPGImage(ConfigInstance::Get().GetAppInstance(), NULL, Flags);
    } else {
        const string msg("Formato de imagen no soportado.\nFormatos válidos: png, bmp, jpg.\nArchivo: " + sNameFile);
        LogInstance::Get().Write("CPicture::Load",msg);
        MsgBoxInstance::Get().ShowError(msg);
        return false;
    }
    
    if( !m_pBitmap->loadPixels(File) ) {
        const string msg("Error interno en archivo de imagen:\n" + sNameFile);
        LogInstance::Get().Write("CPicture::Load",msg);
        MsgBoxInstance::Get().ShowError(msg);
        return false;
    }
   
    m_pBitmap->create();
    return true;
}




//////////////////////////////////////////////////
bool CPicture::Render(FXDCWindow& dc, 
                      const int destWidth,
                      const int destHeight) const
//////////////////////////////////////////////////
{
    m_pBitmap->scale(destWidth, destHeight, 0);
    dc.drawImage(m_pBitmap, 0, 0);
    return true;
}



/////////////////////////////////////
FXImage* CPicture::GetBitmap() const
/////////////////////////////////////
{ 
    return m_pBitmap; 
}
    


//////////////////////////////
bool CPicture::isValid() const
//////////////////////////////
{ 
    return m_pBitmap != 0; 
}

