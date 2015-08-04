// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2006 RobotCub Consortium
 * Authors: Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include <yarp/os/QosStyle.h>


yarp::os::QosStyle::QosStyle() :
        threadPriority(0),
        threadPolicy(-1),
        packetPriority(yarp::os::QosStyle::NORM){
}