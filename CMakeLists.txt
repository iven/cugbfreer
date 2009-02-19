CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT (CUGBFREER)

# CMake module path
SET (CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
SET (DOCS AUTHORS COPYING ChangeLog INSTALL NEWS README TODO)
SET (pkgdatadir ${CMAKE_INSTALL_PREFIX}/share/cugbfreer)
SET (pkgdocdir share/doc/cugbfreer)
SET (pkglocaledir ${CMAKE_INSTALL_PREFIX}/share/locale)
# add subdirectories
ADD_SUBDIRECTORY (src bin)
#ADD_SUBDIRECTORY (pixmaps)
# install documents
INSTALL (FILES ${DOCS} DESTINATION ${pkgdocdir})
INSTALL (DIRECTORY pixmaps DESTINATION ${pkgdatadir})
INSTALL (DIRECTORY config/ DESTINATION ${pkgdatadir})
INSTALL (DIRECTORY locale/ DESTINATION ${pkglocaledir})
