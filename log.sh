#!/bin/bash
echo Packing log files to log.tar.gz...
tar -cvf log.tar.gz log/* *.ini
echo Packing ended
echo "Press any key to exit"
read -n 1
