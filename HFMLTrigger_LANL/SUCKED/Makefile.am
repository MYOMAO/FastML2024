##############################################
# please add new classes in alphabetical order

AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = \
   libhfmltrigger.la

AM_CPPFLAGS = \
  -I/sphenix/user/zshi/FastMLUConn/HFMLTriggerCodes/HepMC3Local/HepMC3/build/include/   \
  -I$(includedir) \
  -I$(OFFLINE_MAIN)/include \
  -I$(OFFLINE_MAIN)/include/eigen3 \
  -I$(ROOTSYS)/include \
  -I/cvmfs/sphenix.sdcc.bnl.gov/gcc-8.3/opt/sphenix/core/rapidjson/include/ \
  -I/sphenix/user/zshi/FastMLUConn/HFMLTriggerCodes/HepMC3Local/HepMC3/interfaces/HepMCCompatibility/include/ 

libhfmltrigger_la_LDFLAGS = \
  -L/sphenix/user/zshi/FastMLUConn/HFMLTriggerCodes/MyOkLib \
  -L$(libdir) \
  -L$(ROOTSYS)/lib \
  -L$(OPT_SPHENIX)/lib \
  -L/sphenix/user/zshi/FastMLUConn/HFMLTriggerCodes/HepMC3Local/HepMC3/build/lib64/ 


pkginclude_HEADERS = \
  HFMLTriggerOccupancy.h \
  HFMLTriggerInterface.h \
  HFMLTriggerHepMCTrigger.h 


ROOTDICTS = \
  HFMLTriggerHepMCTrigger_Dict.C \
  HFMLTriggerInterface_Dict.C \
  HFMLTriggerOccupancy_Dict.C


pcmdir = $(libdir)
nobase_dist_pcm_DATA = \
  HFMLTriggerHepMCTrigger_Dict_rdict.pcm \
  HFMLTriggerInterface_Dict_rdict.pcm \
  HFMLTriggerOccupancy_Dict_rdict.pcm


libhfmltrigger_la_SOURCES = \
  $(ROOTDICTS) \
  HFMLTriggerInterface.C \
  HFMLTriggerHepMCTrigger.C \
  HFMLTriggerOccupancy.C

libhfmltrigger_la_LIBADD = \
  -lHepMC3rootIO \
  -lHepMC3 \
  -lfun4all \
  -lg4dst \
  -lg4mvtx \
  -lg4eval 

# Rule for generating table CINT dictionaries.
%_Dict.C: %.h %LinkDef.h
	rootcint -f $@ @CINTDEFS@ $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

#just to get the dependency
%_Dict_rdict.pcm: %_Dict.C ;



################################################
# linking tests

noinst_PROGRAMS = testexternals

BUILT_SOURCES = testexternals.C

testexternals_SOURCES = \
	testexternals.C

testexternals_LDADD = \
  libhfmltrigger.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm
