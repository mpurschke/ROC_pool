ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

CXXFLAGS = -O2 -g -Wall -std=c++20 -fPIC -I. $(ROOTFLAGS) -I$(ONLINE_MAIN)/include
LDFLAGS  = -Wl,--no-as-needed -L$(ONLINE_MAIN)/lib -Wl,-rpath,$(ONLINE_MAIN)/lib -lEvent -lNoRootEvent -lmessage $(ROOTLIBS)

POOL_SOURCES = ROC_pool.cc h2gcroc3_10G_pool.cc caloroc1b_pool.cc
POOL_HEADERS = ROC_pool.h h2gcroc3_10G_pool.h caloroc1b_pool.h

all: libROC_pool.so pool_validate

libROC_pool.so: $(POOL_SOURCES) $(POOL_HEADERS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $(POOL_SOURCES) $(LDFLAGS)

pool_validate: pool_validate.cc libROC_pool.so
	$(CXX) $(CXXFLAGS) -o $@ $< -L. -Wl,-rpath,$(CURDIR) -lROC_pool $(LDFLAGS)

.PHONY: all clean

clean:
	rm -f libROC_pool.so pool_validate
