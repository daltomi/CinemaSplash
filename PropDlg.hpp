/* ==================================================
	File name: PropDlg.hpp

	Purpose:  Ventana de configuración

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
#include "icons.hpp"
#include "Help.hpp"


class CPropDlg : public FXDialogBox
{
	FXDECLARE(CPropDlg)

	public:

		/*enum {
			ID_CLEAR = FXDialogBox::ID_LAST,
			ID_PREVIEW,
			ID_SAVE,
			ID_LIST_VIDEO,
			ID_LIST_PLAYLIST,
			ID_HELP,
			ID_STATUSLINE,
			ID_REPLAY,
			ID_LAST
		};*/


	protected:
		CPropDlg() { };
		CPropDlg(const CPropDlg& ) { };
};

