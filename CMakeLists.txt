cmake_minimum_required(VERSION 3.10.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()  
    set(VERSION 0.0.1)
endif()

project(bulk_server VERSION ${VERSION})

find_package( Boost COMPONENTS system REQUIRED )
include_directories( ${Boost_INCLUDE_DIR} )

find_package(Threads)
find_package(Doxygen COMPONENTS dot)

set(EXEC_NAME ${CMAKE_PROJECT_NAME})
set(LIB_NAME ${CMAKE_PROJECT_NAME}_lib)
set(TEST_NAME gtest_${CMAKE_PROJECT_NAME})

include_directories("inc/")
set(SRC_PATH "src/")
set(TEST_PATH "test/")
set(GTEST_SRCDIR "/usr/src/gtest/")

add_executable(${EXEC_NAME} 
  ${SRC_PATH}/main_server.cpp
)

add_library(${LIB_NAME} 
  ${SRC_PATH}/server.cpp
  ${SRC_PATH}/session.cpp
)

set_target_properties(${EXEC_NAME} ${LIB_NAME}  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)
target_include_directories(${EXEC_NAME} 
  PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${LIB_NAME}
  ${Boost_LIBRARIES}
)

target_link_libraries(${EXEC_NAME} PRIVATE
  ${LIB_NAME} 
)

if(EXISTS "${GTEST_SRCDIR}/CMakeLists.txt")
message(${GTEST_SRCDIR})
  add_subdirectory(${GTEST_SRCDIR}/ ${CMAKE_BINARY_DIR}/gtest/ EXCLUDE_FROM_ALL)

  add_executable(${TEST_NAME}
    ${GTEST_SRCDIR}/src/gtest_main.cc
  )

  target_link_libraries(${TEST_NAME}
    PRIVATE
    gtest
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${LIB_NAME}
  )

  enable_testing()
  add_test(${TEST_NAME} ${TEST_NAME})
else()
  message(
    WARNING
    "Test suite will not be launched because GTest sources is not found at "
    "${GTEST_SRCDIR}."
  )
endif()

if(DOXYGEN_FOUND)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY)
  add_custom_target(
    html
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/
    COMMENT "Creating HTML documentation with Doxygen" VERBATIM)
endif()

set_target_properties(${EXEC_NAME} ${TEST_NAME} PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

if (MSVC)
  target_compile_options(${EXEC_NAME} PRIVATE /W4)
  target_compile_options(${LIB_NAME} PRIVATE /W4)
  target_compile_options(${TEST_NAME} PRIVATE /W4)
else ()
  target_compile_options(${EXEC_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
  target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
  target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS ${EXEC_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT wladimir.shishov@gmail.com)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
include(CPack)
