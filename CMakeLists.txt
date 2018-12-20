cmake_minimum_required(VERSION 3.5)

project(HW10L20-Threads)
set(EXECUTE_FILE_NAME bulkmt)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wpedantic -Wall -Wextra)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cmake/GetGitRevisionDescription)

find_package(Threads)

add_executable(${EXECUTE_FILE_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${version_file})

target_link_libraries(${EXECUTE_FILE_NAME}
  ${CMAKE_THREAD_LIBS_INIT})


#enable_testing()
#add_subdirectory(tests)


install(TARGETS ${EXECUTE_FILE_NAME} DESTINATION bin)

set(CPACK_PACKAGE_NAME ${EXECUTE_FILE_NAME})
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION ${VERSION_SHORT})
set(CPACK_PACKAGE_CONTACT xxx@xxx.xx)
#set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

include(CPack)
