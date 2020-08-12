/* ==================================================
	File name: Help.hpp

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

#pragma once
#include <fx.h>

#include "Config.hpp"

class CHelp : public FXDialogBox
{
	FXDECLARE(CHelp)

	public:
		CHelp(FXWindow*, const FXString&);
		virtual ~CHelp();
		void setHelp(const FXString&);
		FXString getHelp() const;

	protected:
		CHelp() { }
		CHelp(const CHelp& ) { };

	private:
		FXText* m_helpText;
};


