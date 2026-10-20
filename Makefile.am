AUTOMAKE_OPTIONS = foreign

# used as #include <ROC_pool/ROC_pool.h> (automake would make the
# directory lowercase, from the package name)
pkgincludedir = $(includedir)/ROC_pool

pkginclude_HEADERS = \
  ROC_pool.h \
  h2gcroc3_10G_pool.h \
  caloroc1b_pool.h

LINKFILE = ROC_poolLinkDef.h

noinst_HEADERS = $(LINKFILE)

AM_CPPFLAGS = -I$(includedir) -I$(ONLINE_MAIN)/include -isystem@ROOTINC@

lib_LTLIBRARIES = libROC_pool.la

libROC_pool_la_SOURCES = \
  ROC_pool_Dict.C \
  ROC_pool.cc \
  h2gcroc3_10G_pool.cc \
  caloroc1b_pool.cc

libROC_pool_la_LIBADD = \
  -L$(libdir) \
  -L$(ONLINE_MAIN)/lib \
  -lEvent -lNoRootEvent -lmessage \
  $(ROOTLIBS)

# checks the pools against the per-Packet decoders on a data file
bin_PROGRAMS = pool_validate

pool_validate_SOURCES = pool_validate.cc

pool_validate_LDADD = \
  libROC_pool.la \
  -L$(libdir) \
  -L$(ONLINE_MAIN)/lib \
  -lEvent -lNoRootEvent -lmessage \
  $(ROOTLIBS)

pcmdir = $(libdir)
nobase_dist_pcm_DATA = \
  ROC_pool_Dict_rdict.pcm

ROC_pool_Dict.C: $(pkginclude_HEADERS) $(LINKFILE)
	rootcint -f $@ @CINTDEFS@ $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

# get the dependency for the pcm files
%_Dict_rdict.pcm: %_Dict.C ;

clean-local:
	rm -f *Dict* *.pcm
