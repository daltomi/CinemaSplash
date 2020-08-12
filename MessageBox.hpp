/* ==================================================
	File name: MessageBox.hpp

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
#pragma once

#include <fx.h>

#include "Config.hpp"
#include "Singleton.hpp"

class CMessageBox
{
	public:
		CMessageBox();
		virtual ~CMessageBox();

		void ShowError(const string&);
		void ShowAlert(const string&);
		void ShowInformation(FXWindow*, const string&);

	private:
		const string m_TitleApp;
};

typedef Singleton<CMessageBox> MsgBoxInstance;

