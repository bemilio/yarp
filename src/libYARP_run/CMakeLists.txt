# Copyright (C) 2006-2019 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

project(YARP_run)

set(YARP_run_HDRS include/yarp/run/Run.h)

set(YARP_run_IMPL_HDRS include/yarp/run/impl/RunCheckpoints.h
                       include/yarp/run/impl/RunProcManager.h
                       include/yarp/run/impl/RunReadWrite.h
                       include/yarp/run/impl/PlatformSysPrctl.h)

if(NOT YARP_NO_DEPRECATED) # Since YARP 3.0.0
  set(YARP_run_os_HDRS include/yarp/os/Run.h)
endif()

set(YARP_run_SRCS src/RunCheckpoints.cpp
                  src/Run.cpp
                  src/RunProcManager.cpp
                  src/RunReadWrite.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Source Files"
             FILES ${YARP_run_SRCS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Header Files"
             FILES ${YARP_run_HDRS}
                   ${YARP_run_IMPL_HDRS}
                   ${YARP_run_os_HDRS})

add_library(YARP_run ${YARP_run_SRCS}
                     ${YARP_run_HDRS}
                     ${YARP_run_IMPL_HDRS}
                     ${YARP_run_os_HDRS})
add_library(YARP::YARP_run ALIAS YARP_run)

target_include_directories(YARP_run PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(YARP_run PRIVATE YARP::YARP_OS)
list(APPEND YARP_run_PRIVATE_DEPS YARP_OS)

if(YARP_HAS_ACE)
  target_link_libraries(YARP_run PRIVATE ${ACE_LIBRARIES})
  target_include_directories(YARP_run SYSTEM PRIVATE ${ACE_INCLUDE_DIRS})
  list(APPEND YARP_run_PRIVATE_DEPS ACE)
endif()

target_compile_features(YARP_run PUBLIC cxx_override)

set_property(TARGET YARP_run PROPERTY PUBLIC_HEADER ${YARP_run_HDRS})
set_property(TARGET YARP_run PROPERTY PRIVATE_HEADER ${YARP_run_IMPL_HDRS})
set_property(TARGET YARP_run PROPERTY VERSION ${YARP_VERSION_SHORT})
set_property(TARGET YARP_run PROPERTY SOVERSION ${YARP_SOVERSION})

install(TARGETS YARP_run
        EXPORT YARP_run
        COMPONENT YARP_run
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/run"
        PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/run/impl")
if(NOT YARP_NO_DEPRECATED) # Since YARP 3.0.0
  install(FILES ${YARP_run_os_HDRS}
          COMPONENT deprecated
          DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/yarp/os")
endif()

include(YarpInstallBasicPackageFiles)
yarp_install_basic_package_files(YARP_run
                                 DEPENDENCIES ${YARP_run_PUBLIC_DEPS}
                                 PRIVATE_DEPENDENCIES ${YARP_run_PRIVATE_DEPS})

set_property(TARGET YARP_run PROPERTY FOLDER "Libraries/Private")
