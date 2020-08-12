/* ==================================================
	File name: Picture.hpp

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

#pragma once

#include <fx.h>
#include <typeinfo>

#include "Config.hpp"
#include "MessageBox.hpp"
#include "Log.hpp"

class CPicture 
{
public:

	CPicture();
	~CPicture();

	bool Load(const string& );
	bool Render(FXDCWindow& , const int , const int ) const;
    bool isValid() const;
    FXImage* GetBitmap() const;

protected:

	FXImage* m_pBitmap;
    int      m_BmpWidth;
    int      m_BmpHeight;
};


