#!/bin/csh
setenv HOME /star/u/$LOGNAME
#setenv HOME /sphenix/user/$LOGNAME

source /etc/csh.login
foreach i (/etc/profile.d/*.csh)
 source $i
end

source $HOME/.login
#source /direct/star+u/zshi/.login

#source /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/sphenix_setup.csh -n ana.141

#source /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/sphenix_setup.csh -n
#source /cvmfs/sphenix.sdcc.bnl.gov/x8664_sl7/opt/sphenix/core/bin/setup_root6.csh

#source /opt/sphenix/core/bin/sphenix_setup.csh -n
#source /opt/sphenix/core/bin/setup_root6.csh

#source /opt/sphenix/core/bin/setup_root6_include_path.csh


echo "START PRINT ENV"

#printenv


echo "DONE PRINt ENV"

set NEvent=$argv[1]
set Name=$argv[2]
set Path=$argv[3]



#source /opt/sphenix/core/bin/sphenix_setup.csh -n





#source Build.sh


echo "Now PWD"

pwd

ls

echo "DONE CHECK"

cd workdir

mkdir ${Name}

cp -r ../macros/ ${Name} 
cp ../Link.sh  ${Name}/


cd ${Name}

echo "NowList"

ls *


#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/FastMLWork7/JobSub/workdir/${Name}/macros/common:$ROOT_INCLUDE_PATH





#cd HFMLTrigger_LANL 

source Link.sh


#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/EvtGenTestJobSub/workdir/${Name}/macros/common:$ROOT_INCLUDE_PATH


echo "DONE BUILD"

cd macros/detectors/sPHENIX/

#rm MyQAFile.root



root -b -l -q Fun4All_G4_sPHENIX.C'('${NEvent}')'

ls *root


ls bbbarHFML.json > Filelist.txt

echo "Now Check JSON"

root -b -l -q CheckJSON.C

echo "DONE Check JSON"

echo "Final Check if exists"
ls bbbarHFML.json
echo "DONE"


mv bbbarHFML.json /sphenix/tg/tg01/hf/zshi/TestProd/bbbar_${Name}.jsons



cd ../../../../

rm -rf ${Name}



