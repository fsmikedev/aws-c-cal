include(CMakeFindDependencyMacro)

if (NOT BYO_CRYPTO AND NOT WIN32 AND NOT APPLE)
    find_dependency(LibCryptoCAL)
endif()

if (BUILD_SHARED_LIBS)
    include(${CMAKE_CURRENT_LIST_DIR}/shared/@CMAKE_PROJECT_NAME@-targets.cmake)
else()
    include(${CMAKE_CURRENT_LIST_DIR}/static/@CMAKE_PROJECT_NAME@-targets.cmake)
endif()

