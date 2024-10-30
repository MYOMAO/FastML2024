echo "-------------- CHANGING SOFTWARE NOW BRO --------------------"



source /opt/sphenix/core/bin/sphenix_setup.csh -n ana.376



rm -r install

mkdir install
setenv MYINSTALL $PWD/install/
setenv LD_LIBRARY_PATH $MYINSTALL/lib:$LD_LIBRARY_PATH
setenv LD_LIBRARY_PATH $MYINSTALL/lib64:$LD_LIBRARY_PATH
set path = ( $MYINSTALL/bin $path )

#source $OPT_SPHENIX/bin/setup_local.csh $MYINSTALL
echo "Build trackbase_historic"



echo "-------------- BUILD main ------------------------"


cd coresoftware/simulation/g4simulation/g4main/
autogen.sh --prefix=$MYINSTALL
make -j10 install
cd ../../../../



cd HFMLTrigger_LANL
autogen.sh --prefix=$MYINSTALL
make -j20 install
cd ..


cd AntiTrigger/

#cd coresoftware/simulation/g4simulation/g4detectors/
autogen.sh --prefix=$MYINSTALL
make -j20 install
#make install

cd ..

 

#echo "-------------- DONE CHANGING SOFTWARE NOW BRO --------------------"
