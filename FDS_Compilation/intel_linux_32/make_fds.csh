#!/bin/csh -f
set platform=ia32
set dir=`pwd`
set target=$dir:t


source ../Scripts/set_fort.csh $platform
if ($status == 1) exit

echo Building $target
make VPATH="../../FDS_Source" -f ../makefile $target
