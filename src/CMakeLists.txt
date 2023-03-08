include_directories(include)

# DO NOT INCLUDE THE STORAGE PART; NOT NEEDED FOR READONLY ACCESS
# add_subdirectory(storage)

add_library(
  sqlite_ext_library OBJECT
  sqlite_db.cpp sqlite_extension.cpp sqlite_scanner.cpp sqlite_stmt.cpp
  # DO NOT INCLUDE THE STORAGE PART; NOT NEEDED FOR READONLY ACCESS
  # sqlite_storage.cpp sqlite_utils.cpp)
  sqlite_utils.cpp)
set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:sqlite_ext_library>
    PARENT_SCOPE)
