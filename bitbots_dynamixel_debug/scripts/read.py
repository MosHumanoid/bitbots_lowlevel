#!/usr/bin/env python
# -*- coding: utf-8 -*-

from bitbots_dynamixel_debug.connector import Connector
import sys

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("--p1", help="use old protocol version", action="store_true")
parser.add_argument("id")
parser.add_argument("register")
args = parser.parse_args()


id = int(args.id)
reg = int(args.register)

if args.p1:
    protocol = 1
else:
    protocol = 2
baudrate = 2000000
device ="/dev/ttyUSB0".encode('utf-8')

c = Connector(protocol, device, baudrate)

for i in range(1):
	c.read_4(id, reg, doPrint=True)

c.closePort()

