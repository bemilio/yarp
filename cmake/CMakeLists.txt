# Copyright (C) 2006-2019 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

# YARP CMake Modules
set(YARP_CMAKE_FILES YarpPlugin.cmake
                     YarpIDL.cmake
                     YarpInstallationHelpers.cmake
                     YarpPrintFeature.cmake)
install(FILES ${YARP_CMAKE_FILES}
        COMPONENT scripts
        DESTINATION share/yarp/cmake)

# Templates used by modules
set(YARP_TEMPLATE_FILES template/yarp_plugin_carrier.cpp.in
                        template/yarp_plugin_device.cpp.in
                        template/yarp_plugin_portmonitor.cpp.in
                        template/yarp_plugin_library.cpp.in
                        template/yarp_plugin_yarpdev_main.cpp.in
                        template/YarpTweakIDL.cmake.in
                        template/placeGeneratedYarpIdlFiles.cmake.in)

install(FILES ${YARP_TEMPLATE_FILES}
        COMPONENT scripts
        DESTINATION share/yarp/cmake/template)

# Remove renamed files from installation
foreach(_f YarpDeprecatedWarning.cmake   # since YARP 3.0.0
           deprecated/YarpDeprecatedOption.cmake
           deprecated/YarpRenamedOption.cmake
           deprecated/YarpBackupVariable.cmake)
  if(EXISTS "${CMAKE_INSTALL_PREFIX}/share/yarp/cmake/${_f}")
    install(CODE
 "if(EXISTS \"${CMAKE_INSTALL_PREFIX}/share/yarp/cmake/${_f}\")
    message(STATUS \"Deleted: \\\"${CMAKE_INSTALL_PREFIX}/share/yarp/cmake/${_f}\\\"\")
    file(REMOVE \"${CMAKE_INSTALL_PREFIX}/share/yarp/cmake/${_f}\")
  endif()"
           COMPONENT scripts)
  endif()
endforeach()
