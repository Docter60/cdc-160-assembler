file(GLOB SOURCES
    ../include/*.h
    *.cpp
)

add_executable(160asm ${SOURCES})

target_include_directories(160asm PUBLIC 
    "${160asm_BINARY_DIR}/src"
    "${160asm_SOURCE_DIR}/include")
