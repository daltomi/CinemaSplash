cmake_minimum_required( VERSION 2.8 )
project( CinemaSplash )
set( APP cinemasplash)

file(GLOB SOURCES "*.cxx" "*.hpp" )

#set(CMAKE_BUILD_TYPE release)
set(CMAKE_BUILD_TYPE debug)

if( WIN32 ) #WINDOWS
    if( ${CMAKE_BUILD_TYPE} STREQUAL "release")
        set( CMAKE_CXX_FLAGS "-fpermissive -Ofast -std=gnu++0x -DWIN32 -mwindows")
    else()
        set( CMAKE_CXX_FLAGS "-fpermissive -g -DDEBUG -std=gnu++0x -DWIN32 -mwindows")
    endif()
else()     #LINUX
    if( ${CMAKE_BUILD_TYPE} STREQUAL "release")
      set( CMAKE_CXX_FLAGS "-fpermissive -Ofast -march=native -mtune=native -flto -funroll-loops -std=c++11")
    else()
      set( CMAKE_CXX_FLAGS "-Wall -fpermissive -g -O -DDEBUG -std=c++11")
    endif()
endif()

include (FindPkgConfig)
pkg_search_module(FOX REQUIRED fox17)
pkg_search_module(VLC REQUIRED libvlc)
pkg_search_module(VLC_PLUGIN REQUIRED vlc-plugin)

link_directories(
    ${FOX_LIBRARY_DIRS}
    ${VLC_LIBRARY_DIRS}
)

include_directories(
    ${FOX_INCLUDE_DIRS}
      ${VLC_INCLUDE_DIRS} 
)

if( WIN32 )
  add_custom_command(
    OUTPUT    window_res/app.o
    COMMAND   windres  
    ARGS      -i window_res/app.rc -o window_res/app.o 
    )
    add_executable( ${APP} window_res/app.o ${SOURCES} ${FOX_INCLUDE_DIRS} )
    target_link_libraries( ${APP} ${FOX_LIBRARIES} ${VLC_LIBRARIES} ${VLC_PLUGIN} -lpng -ljpeg -lz -limm32 -lwsock32 -lws2_32 -lmpr)
else()
    add_executable( ${APP} ${SOURCES} ${FOX_INCLUDE_DIRS} )
    target_link_libraries( ${APP} ${FOX_LIBRARIES} ${VLC_LIBRARIES} ${VLC_PLUGIN} )
endif()


#if(WIN32)
# if( ${CMAKE_BUILD_TYPE} STREQUAL "release")
#   add_custom_command(
#        TARGET ${APP}
#        COMMAND upx -9 ${APP}.exe
#        )
# endif()
#
#else()
# if( ${CMAKE_BUILD_TYPE} STREQUAL "release")
#   add_custom_command(
#        TARGET ${APP}
#        COMMAND upx -9 ${APP}
#        )
# endif()
#endif()

