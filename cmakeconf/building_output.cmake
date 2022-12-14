SET(OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_SYSTEM_NAME}_${ARCH_BITS})

MESSAGE("SYSTEM NAME: " ${CMAKE_SYSTEM_NAME})
MESSAGE("OUTPUT DIRECTORY: " ${OUTPUT_DIRECTORY})

# SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
# SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
# SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

SET(OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY}_Debug)
SET(OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY}_Release)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY_DEBUG})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY_DEBUG})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIRECTORY_DEBUG})

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY_RELEASE})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY_RELEASE})
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_DIRECTORY_RELEASE})