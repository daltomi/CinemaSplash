/* ==================================================
    File name: Cinemasplash.cxx

    Purpose:  Main App

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

#include "Cinemasplash.hpp"
#include <clocale>

////////////////////////////////////
int main(int argc, char* argv[] )
////////////////////////////////////
{
    FXApp app("CinemaSplash");
    app.init(argc,argv);
    ConfigInstance::Get().Initialize( &app );

    //Permite restaurar el registro
    // Parámetro: --restore
    // Se ignorarn parám. a FOX
    if(argc == 2) {
        const string param = argv[1];
        if(param == "--restore") {
            ConfigInstance::Get().WriteDefault();
            cerr << "CinemaSplash - Registro restaurado" << endl;
        }
        return 0;
    }
    
   // setLocaleFont(app);

    CMainWindow* pMainWindow =  new CMainWindow( &app );
    app.create();
#ifndef DEBUG
    pMainWindow->fullScreen(true);
#endif
    return app.runModalFor( pMainWindow );
}


/*Por ahora utilizar registro de configuracion para
 * el manejo de fuentes.
 */

/*/////////////////////////////////////
void setLocaleFont(const FXApp& app)
//////////////////////////////////////
{
    string lang=setlocale(LC_MESSAGES,NULL);

    std::locale loc(lang.c_str());
    std::locale::global(std::locale(loc,std::locale::classic(),std::locale::numeric));

    if(lang=="") {
        return;
    }
   
    string slang(lang);

    if( slang.find("es_") != string::npos) {
        FXFontDesc desc={
            "helvetica",
            80,
            FXFont::Bold,
            FXFont::Normal,
            FONTENCODING_ISO_8859_1,
        };

        app.setNormalFont(new FXFont(&app,desc));
    }
#ifdef DEBUG
    cerr << "setLocaleFont : " << lang << endl;
#endif
}*/

