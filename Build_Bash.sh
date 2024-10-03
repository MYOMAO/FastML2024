#!/bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n ana.376

#source /opt/sphenix/core/bin/sphenix_setup.sh -n new

rm -rf install

mkdir install
export MYINSTALL=$PWD/install/
export LD_LIBRARY_PATH=$MYINSTALL/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$MYINSTALL/lib64:$LD_LIBRARY_PATH
export PATH=$MYINSTALL/bin:$PATH

#source $OPT_SPHENIX/bin/setup_local.sh $MYINSTALL
echo "Build trackbase_historic"

#cd trackbase_historic

#./autogen.sh --prefix=$MYINSTALL
#make
#make install

#cd ../

echo "-------------- BUILD main ------------------------"

cd HFMLTrigger_LANL

#cd coresoftware/simulation/g4simulation/g4detectors/
./autogen.sh --prefix=$MYINSTALL
make -j20 install
#make install

cd ..

cd AntiTrigger/

#cd coresoftware/simulation/g4simulation/g4detectors/
./autogen.sh --prefix=$MYINSTALL
make -j20 install
#make install

cd ..

#echo "-------------- DONE CHANGING SOFTWARE NOW BRO --------------------"
