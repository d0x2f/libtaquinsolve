libtaquinsolve_la_CXXFLAGS = -lstdc++fs
test_LDFLAGS = -ltaquinsolve

lib_LTLIBRARIES = libtaquinsolve.la
bin_PROGRAMS = test

libtaquinsolve_la_SOURCES = taquinsolve.cc \
                            Board.cc \
                            ASolver.cc \
                            IDASolver.cc \
                            BFSDatabaseGenerator.cc \
                            Solver.cc

test_SOURCES = test.cc

include_HEADERS =   taquinsolve.hh \
                    Board.hh \
                    ASolver.hh \
                    IDASolver.hh \
                    BFSDatabaseGenerator.hh \
                    Solver.hh
