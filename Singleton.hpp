/* ==================================================
    File name: Singleton.hpp

    Purpose:  Unica instancia.
              Gracias a unique_ptr se asegura que \
              se llama al destructor de clase siempre, \
              aun siendo estatico.

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
#include <memory>
using std::unique_ptr;

template<typename T>
class Singleton
{
    public:
        static T& Get() {
            static unique_ptr<T> instance = unique_ptr<T>( new T() );
            return *instance.get();
        }
 
    protected:
        Singleton()  { }
        ~Singleton() { }
};


