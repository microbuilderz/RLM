MESSAGE(STATUS "Looking for dl")

FIND_PATH(dl_INCLUDE_DIR dl.h)
SET(dl_NAMES ${dl_NAMES} dl)
FIND_LIBRARY(dl_LIBRARY NAMES ${dl_NAMES} HPATHS /usr/lib/x86_64-linux-gnu)

IF (dl_LIBRARY)
    SET(dl_LIBRARIES ${dl_LIBRARY})
    SET(dl_FOUND "YES")
ELSE (dl_LIBRARY)
    SET(dl_FOUND "NO")
ENDIF (dl_LIBRARY)

IF (dl_FOUND)
    IF (NOT dl_FIND_QUIETLY)
        MESSAGE(STATUS "Found dl library: ${dl_LIBRARIES}")
    ENDIF (NOT dl_FIND_QUIETLY)
ELSE (dl_FOUND)
    IF (dl_FIND_REQUIRED)
        MESSAGE(FATAL_ERROR "Could not find dl library")
    ENDIF (dl_FIND_REQUIRED)
ENDIF (dl_FOUND)