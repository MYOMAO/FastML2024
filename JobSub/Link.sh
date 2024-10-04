source /opt/sphenix/core/bin/sphenix_setup.csh -n ana.376
setenv MYINSTALL ${PWD}/../../../install/
setenv LD_LIBRARY_PATH $MYINSTALL/lib:$LD_LIBRARY_PATH
set path = ( $MYINSTALL/bin $path )


setenv ROOT_INCLUDE_PATH ${PWD}/macros/common:$ROOT_INCLUDE_PATH


#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/FastMLLatest/HFMLTriggerCodes/macros/common:$ROOT_INCLUDE_PATH
