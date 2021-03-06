include(ExternalProject)

ExternalProject_Add(
    MbedTLS
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/MbedTLS
    DEPENDS ZLIB
    URL https://github.com/ARMmbed/mbedtls/archive/mbedtls-2.16.2.tar.gz
    CMAKE_ARGS
        -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
        -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
        -DBUILD_SHARED_LIBS=${MbedTLS_SHARED_LIBS})

