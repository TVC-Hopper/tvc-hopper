
set(HEADER_LIST "../include/stcp.h")


add_library(stcp STATIC stcp.c ${HEADER_LIST})

target_include_directories(stcp PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
  )