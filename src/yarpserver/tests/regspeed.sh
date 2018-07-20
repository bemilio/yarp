#!/bin/sh

# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# Copyright (C) 2006-2010 RobotCub Consortium
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

let x=1
let top=200
while [ $x -le $top ]; do
    echo $x of $top
    echo Hello | yarp write /write
    let x=x+1
done

