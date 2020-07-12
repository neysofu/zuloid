# Install script for directory: /home/neysofu/Projects/zorro/lib/plibsys/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xCorex" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/neysofu/Projects/zorro/app/avenger/bin/libplibsys.so.0")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xCorex" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/neysofu/Projects/zorro/app/avenger/bin/libplibsys.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/neysofu/Projects/zorro/app/avenger/bin/libplibsys.so.0")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so.0")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/neysofu/Projects/zorro/app/avenger/bin/libplibsys.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libplibsys.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xCorex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/plibsys" TYPE FILE FILES
    "/home/neysofu/Projects/zorro/lib/plibsys/src/patomic.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/ptypes.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pmacros.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pmacroscompiler.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pmacroscpu.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pmacrosos.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pcondvariable.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pcryptohash.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/perror.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/perrortypes.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pdir.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pfile.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/phashtable.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pinifile.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/plibsys.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/plibraryloader.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/plist.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pmain.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pmem.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pmutex.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pprocess.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/prwlock.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/psemaphore.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pshm.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pshmbuffer.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/psocket.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/psocketaddress.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pspinlock.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pstdarg.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/pstring.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/ptimeprofiler.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/ptree.h"
    "/home/neysofu/Projects/zorro/lib/plibsys/src/puthread.h"
    "/home/neysofu/Projects/zorro/app/avenger/bin/plibsysconfig.h"
    )
endif()

