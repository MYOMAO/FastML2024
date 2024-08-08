#source /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/bin/sphenix_setup.csh –n

#source /opt/sphenix/core/bin/sphenix_setup.csh -n ana.355

#source /opt/sphenix/core/bin/sphenix_setup.csh -n new

source /opt/sphenix/core/bin/sphenix_setup.csh -n ana.376
#setenv MYINSTALL $PWD/install/
#setenv LD_LIBRARY_PATH $MYINSTALL/lib:$LD_LIBRARY_PATH
#set path = ( $MYINSTALL/bin $path )

#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/EvtGenPullRequest/macros/common:$ROOT_INCLUDE_PATH

#setenv MYINSTALL /sphenix/user/xyu3/PublicBuild/install/
setenv MYINSTALL /sphenix/user/xyu3/ForZhaozhong/PublicBuild/install/
setenv LD_LIBRARY_PATH $MYINSTALL/lib:$LD_LIBRARY_PATH
set path = ( $MYINSTALL/bin $path )

#cd coresofteare/

#cd coresoftware/simulation/g4simulation/g4eval/


#autogen.sh --prefix=$MYINSTALL
#make -j10 install
#cd ../../../../


#setenv LD_LIBRARY_PATH /sphenix/user/zshi/FastMLUConn/NewBuild/FastMLUConn/HFMLTriggerCodes/install/lib/:$LD_LIBRARY_PATH

#setenv LD_LIBRARY_PATH /sphenix/user/zshi/FastMLUConn/NewBuild/FastMLUConn/HFMLTriggerCodes/HFMLTrigger_LANL/install/lib/:$LD_LIBRARY_PATH

setenv LD_LIBRARY_PATH ${PWD}/install/lib/:$LD_LIBRARY_PATH

setenv ROOT_INCLUDE_PATH ${PWD}/macros/common:$ROOT_INCLUDE_PATH


#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/FastMLLatest/HFMLTriggerCodes/macros/common:$ROOT_INCLUDE_PATH
