# include gtk+-2.0
FIND_PACKAGE (GTK2 REQUIRED COMPONENTS gtk)
INCLUDE_DIRECTORIES (${GTK2_INCLUDE_DIRS})
# enable warning
ADD_DEFINITIONS ("-g -Wall")
ADD_DEFINITIONS (-DPKGDATADIR="${pkgdatadir}")
# set targets
SET (SRC_LIST main.c interface.c about.c linker.c welcome.c)
ADD_EXECUTABLE (cugbfreer ${SRC_LIST})
TARGET_LINK_LIBRARIES (cugbfreer ${GTK2_LIBRARIES})
INSTALL (TARGETS cugbfreer DESTINATION bin)
