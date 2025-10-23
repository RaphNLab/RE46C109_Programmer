#!/bin/bash
# This script connects to a serial device using picocom.
# Usage: ./connect_serial.sh [device] [baudrate]

DEVICE=${1:-/dev/ttyACM0}
BAUDRATE=${2:-9600}

picocom -b $BAUDRATE $DEVICE