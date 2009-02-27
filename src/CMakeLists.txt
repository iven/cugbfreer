# include gtk+-2.0
FIND_PACKAGE (GTK2 REQUIRED COMPONENTS gtk)
INCLUDE_DIRECTORIES (${GTK2_INCLUDE_DIRS})
# enable warning
ADD_DEFINITIONS ("-g -Wall")
#ADD_DEFINITIONS (-DG_DISABLE_ASSERT)
ADD_DEFINITIONS (-DPKGDATADIR="${pkgdatadir}")
ADD_DEFINITIONS (-DPKGLOCALEDIR="${pkglocaledir}")
# set targets
SET (SRC_LIST main interface about connection welcome misc keyfile)
ADD_EXECUTABLE (cugbfreer ${SRC_LIST})
TARGET_LINK_LIBRARIES (cugbfreer ${GTK2_LIBRARIES})
INSTALL (TARGETS cugbfreer DESTINATION bin)
