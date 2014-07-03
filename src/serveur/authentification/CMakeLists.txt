
# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

########### Authentication ###############

file(GLOB_RECURSE sources_Authentication Authentication/*.cpp Authentication/*.h)
file(GLOB_RECURSE sources_PrecompiledHeaders PrecompiledHeaders/*.cpp PrecompiledHeaders/*.h)
file(GLOB_RECURSE sources_Realms Realms/*.cpp Realms/*.h)
file(GLOB_RECURSE sources_Server Server/*.cpp Server/*.h)
file(GLOB sources_Localdir *.cpp *.h)

source_group(CMake FILES CMakeLists.txt)
source_group(Authentication FILES ${sources_Authentication})
source_group(PrecompiledHeaders FILES ${sources_PrecompiledHeaders})
source_group(Realms FILES ${sources_Realms})
source_group(Server FILES ${sources_Server})
source_group(localdir FILES ${sources_Localdir})

if (USE_COREPCH)
  set(Authentication_PCH_HDR PrecompiledHeaders/authPCH.h)
  set(Authentication_PCH_SRC PrecompiledHeaders/authPCH.cpp)
endif()

set(Authentication_SRCS
  ${Authentication_SRCS}
  ${sources_Authentication}
  ${sources_Realms}
  ${sources_Server}
  ${sources_Localdir}
)

if( WIN32 )
  set(Authentication_SRCS
    ${Authentication_SRCS}
    ${sources_windows_Debugging}
  )
  if ( MSVC )
    set(Authentication_SRCS
      ${Authentication_SRCS}
      authentification.rc
    )
  endif ()
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/serveur/commun
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Database
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Debugging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Packets
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Cryptography
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Cryptography/Authentication
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Logging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Threading
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Utilities
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Authentication
  ${CMAKE_CURRENT_SOURCE_DIR}/Realms
  ${CMAKE_CURRENT_SOURCE_DIR}/Server
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_executable(Authentication
  ${Authentication_SRCS}
  ${Authentication_PCH_SRC}
)

add_dependencies(Authentication revision.h)

if( NOT WIN32 )
  set_target_properties(Authentication PROPERTIES
    COMPILE_DEFINITIONS _TRINITY_REALM_CONFIG="${CONF_DIR}/authentification.conf"
  )
endif()

target_link_libraries(Authentication
  commun
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${ACE_LIBRARY}
)

if( WIN32 )
  if ( MSVC )
    add_custom_command(TARGET Authentication
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/authentification.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
    )
  elseif ( MINGW )
    add_custom_command(TARGET Authentication
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/authentification.conf.dist ${CMAKE_BINARY_DIR}/bin/
    )
  endif()
endif()

if( UNIX )
  install(TARGETS Authentication DESTINATION bin)
  install(FILES  authentification.conf.dist DESTINATION ${CONF_DIR})
elseif( WIN32 )
  install(TARGETS Authentication DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES authentification.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

# Generate precompiled header
if (USE_COREPCH)
  add_cxx_pch(Authentication ${Authentication_PCH_HDR} ${Authentication_PCH_SRC})
endif()
