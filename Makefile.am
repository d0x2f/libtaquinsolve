lib_LTLIBRARIES = libtaquinsolve.la
libtaquinsolve_la_SOURCES = taquinsolve.cc \
                            Board.cc \
                            Board.hh \
                            Solver.cc \
                            Solver.hh
include_HEADERS = taquinsolve.hh

bin_PROGRAMS = test
test_SOURCES = test.cc
test_LDADD = libtaquinsolve.la

ACLOCAL_AMFLAGS = -I m4
