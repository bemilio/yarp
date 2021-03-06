# Copyright (C) 2006-2019 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

project(YARP_pcl)

set(YARP_pcl_HDRS include/yarp/pcl/Pcl.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
             PREFIX "Header Files"
             FILES ${YARP_pcl_HDRS})

add_library(YARP_pcl INTERFACE)
add_library(YARP::YARP_pcl ALIAS YARP_pcl)

target_include_directories(YARP_pcl INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(YARP_pcl INTERFACE YARP_sig)
list(APPEND YARP_pcl_PUBLIC_DEPS YARP_sig)

target_include_directories(YARP_pcl SYSTEM INTERFACE ${PCL_INCLUDE_DIR})
target_link_libraries(YARP_pcl INTERFACE ${PCL_LIBRARIES})
list(APPEND YARP_pcl_PUBLIC_DEPS PCL)


install(TARGETS YARP_pcl
        EXPORT YARP_pcl
        COMPONENT YARP_pcl
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/yarp/pcl)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/yarp/pcl
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/yarp)

include(YarpInstallBasicPackageFiles)
yarp_install_basic_package_files(YARP_pcl
                                 DEPENDENCIES ${YARP_pcl_PUBLIC_DEPS}
                                 PRIVATE_DEPENDENCIES ${YARP_pcl_PRIVATE_DEPS})
