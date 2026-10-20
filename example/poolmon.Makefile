PACKAGE = poolmon

ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS = $(shell root-config --glibs)

# the pool classes, one directory up
POOLDIR = $(abspath ..)

CXXFLAGS = -I.  -I$(POOLDIR) $(ROOTFLAGS) -I$(ONLINE_MAIN)/include -I$(OFFLINE_MAIN)/include -fPIC
RCFLAGS = -I.  -I$(ONLINE_MAIN)/include -I$(OFFLINE_MAIN)/include

LDFLAGS = -Wl,--no-as-needed  -L$(ONLINE_MAIN)/lib -L$(OFFLINE_MAIN)/lib -lpmonitor -lEvent -lNoRootEvent -lmessage  $(ROOTLIBS) -fPIC



HDRFILES = $(PACKAGE).h
LINKFILE = $(PACKAGE)LinkDef.h


ADDITIONAL_SOURCES =
ADDITIONAL_LIBS = -L$(POOLDIR) -Wl,-rpath,$(POOLDIR) -lROC_pool


SO = lib$(PACKAGE).so

$(SO) : $(PACKAGE).cc $(PACKAGE)_dict.C $(ADDITIONAL_SOURCES) $(LINKFILE) $(POOLDIR)/libROC_pool.so
	$(CXX) $(CXXFLAGS) -o $@ -shared  $<  $(ADDITIONAL_SOURCES) $(PACKAGE)_dict.C $(LDFLAGS)  $(ADDITIONAL_LIBS)


$(PACKAGE)_dict.C : $(HDRFILES) $(LINKFILE)
	rootcint -f $@  -c $(RCFLAGS) $^

$(POOLDIR)/libROC_pool.so:
	$(MAKE) -C $(POOLDIR) libROC_pool.so


.PHONY: clean

clean:
	rm -f $(SO) $(PACKAGE)_dict.C $(PACKAGE)_dict.h *.pcm

