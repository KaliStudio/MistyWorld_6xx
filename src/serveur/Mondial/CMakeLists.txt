
# Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources_CommandLine CommandLine/*.cpp CommandLine/*.h)
file(GLOB_RECURSE sources_PrecompiledHeaders PrecompiledHeaders/*.cpp PrecompiledHeaders/*.h)
file(GLOB_RECURSE sources_RemoteAccess RemoteAccess/*.cpp RemoteAccess/*.h)
file(GLOB_RECURSE sources_TCSoap TCSoap/*.cpp TCSoap/*.h)
file(GLOB_RECURSE sources_WorldThread WorldThread/*.cpp WorldThread/*.h)
file(GLOB sources_localdir *.cpp *.h)

source_group(CMake FILES CMakeLists.txt)
source_group(CommandLine FILES ${sources_CommandLine})
source_group(PrecompiledHeaders FILES ${sources_PrecompiledHeaders})
source_group(RemoteAcces FILES ${sources_RemoteAccess})
source_group(TCSoap FILES ${sources_TCSoap})
source_group(WorldThread FILES ${sources_WorldThread})
source_group(localdir FILES ${sources_localdir})

if (USE_COREPCH)
  set(Mondial_PCH_HDR PrecompiledHeaders/worldPCH.h)
  set(Mondial_PCH_SRC PrecompiledHeaders/worldPCH.cpp)
endif()

set(Mondial_SRCS
  ${Mondial_SRCS}
  ${sources_CommandLine}
  ${sources_RemoteAccess}
  ${sources_TCSoap}
  ${sources_WorldThread}
  ${sources_localdir}
)

if( WIN32 )
  set(Mondial_SRCS
    ${Mondial_SRCS}
    ${sources_windows_Debugging}
  )
  if ( MSVC )
    set(Mondial_SRCS
      ${Mondial_SRCS}
      Mondial.rc
    )
  endif()
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/dep/gsoap
  ${CMAKE_SOURCE_DIR}/dep/sockets/include
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/src/serveur/collision
  ${CMAKE_SOURCE_DIR}/src/serveur/collision/Management
  ${CMAKE_SOURCE_DIR}/src/serveur/collision/Models
  ${CMAKE_SOURCE_DIR}/src/serveur/commun
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Configuration
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Cryptography
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Cryptography/Authentication
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Database
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/DataStores
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Debugging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Dynamic/LinkedReference
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Dynamic
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Logging
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Packets
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Threading
  ${CMAKE_SOURCE_DIR}/src/serveur/commun/Utilities
  ${CMAKE_SOURCE_DIR}/src/serveur/game
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Accounts
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Achievements
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Addons
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI/CoreAI
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI/ScriptedAI
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AI/SmartScripts
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AuctionHouse
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/AuctionHouse/AuctionHouseBot
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Battlegrounds
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Battlegrounds/Zones
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Calendar
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Chat
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Chat/Channels
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Combat
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Conditions
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/DataStores
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/DungeonFinding
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/AreaTrigger
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Creature
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Corpse
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/DynamicObject
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/GameObject
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Item
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Item/Container
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Object
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Object/Updates
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Pet
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Player
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Totem
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Unit
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Vehicle
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Entities/Transport
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Events
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Globals
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids/Cells
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids/Notifiers
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Grids
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Groups
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Guilds
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Handlers
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Instances
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Loot
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Mails
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Maps
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Miscellaneous
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/MovementGenerators
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/Spline
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Movement/Waypoints
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/OutdoorPvP
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Pools
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/PrecompiledHeaders
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Quests
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Reputation
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Scripting
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Server/Protocol
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Server
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Skills
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Spells
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Spells/Auras
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Tools
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Warden
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Warden/Modules
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/Weather
  ${CMAKE_SOURCE_DIR}/src/serveur/jeu/World
  ${CMAKE_SOURCE_DIR}/src/serveur/authentification/Server
  ${CMAKE_SOURCE_DIR}/src/serveur/authentification/Realms
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/CommandLine
  ${CMAKE_CURRENT_SOURCE_DIR}/RemoteAccess
  ${CMAKE_CURRENT_SOURCE_DIR}/TCSoap
  ${CMAKE_CURRENT_SOURCE_DIR}/WorldThread
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

add_executable(Mondial
  ${Mondial_SRCS}
  ${Mondial_PCH_SRC}
)

if( NOT WIN32 )
  set_target_properties(Mondial PROPERTIES
    COMPILE_DEFINITIONS _TRINITY_CORE_CONFIG="${CONF_DIR}/Mondial.conf"
  )
endif()

add_dependencies(Mondial revision.h)

if( UNIX AND NOT NOJEM )
  set(Mondial_LINK_FLAGS "-pthread -lncurses ${Mondial_LINK_FLAGS}")
endif()

set_target_properties(Mondial PROPERTIES LINK_FLAGS "${Mondial_LINK_FLAGS}")

target_link_libraries(Mondial
  game
  commun
  scripts
  collision
  g3dlib
  gsoap
  Detour
  ${JEMALLOC_LIBRARY}
  ${READLINE_LIBRARY}
  ${TERMCAP_LIBRARY}
  ${ACE_LIBRARY}
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

if( WIN32 )
  if ( MSVC )
    add_custom_command(TARGET Mondial
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Mondial.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
    )
  elseif ( MINGW )
    add_custom_command(TARGET Mondial
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/Mondial.conf.dist ${CMAKE_BINARY_DIR}/bin/
    )
  endif()
endif()

if( UNIX )
  install(TARGETS Mondial DESTINATION bin)
  install(FILES Mondial.conf.dist DESTINATION ${CONF_DIR})
elseif( WIN32 )
  install(TARGETS Mondial DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES Mondial.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()

# Generate precompiled header
if( USE_COREPCH )
  add_cxx_pch(Mondial ${Mondial_PCH_HDR} ${Mondial_PCH_SRC})
endif()
