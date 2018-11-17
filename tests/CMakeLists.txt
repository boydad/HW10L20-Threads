if(BUILD_TESTING)

  find_package(GTest REQUIRED)

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

  add_executable(myTest mainTest.cpp)

  set_target_properties(myTest PROPERTIES
      CXX_STANDARD 14
      CXX_STANDARD_REQUIRED ON
  )

  target_link_libraries(myTest ${GTEST_LIBRARIES} pthread)

  add_test(TestName myTest)


endif()
