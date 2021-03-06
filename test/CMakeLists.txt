
project(test_spoon)
cmake_minimum_required(VERSION 3.2)


if(TARGET libspoon)
  add_library(libspoon::libspoon ALIAS libspoon)
message(INFO "TARGET")
else()  
  find_package(libspoon CONFIG REQUIRED)
endif()



hunter_add_package(Boost COMPONENTS)
set(BOOST_ALL_DYN_LINK         ON)
set(Boost_USE_MULTITHREADED    ON)
set(Boost_USE_STATIC_LIBS      OFF)
set(Boost_USE_STATIC_RUNTIME   OFF)

find_package(Boost COMPONENTS REQUIRED )
link_directories (${Boost_LIBRARY_DIRS})
include_directories (${Boost_INCLUDE_DIRS})
add_definitions(-DBOOST_LOG_DYN_LINK)

include_directories(${CMAKE_CURRENT_LIST_DIR} )


function(add_new_test test_name file_path)
    set(executable_name "${PROJECT_NAME}-${test_name}")

    file(GLOB SPOON_TEST_SOURCE_CPP "${CMAKE_CURRENT_SOURCE_DIR}/${file_path}")

    add_executable ("${executable_name}"  "${SPOON_TEST_SOURCE_CPP}")
    target_link_libraries (${executable_name}
                                libspoon::libspoon 
                                ${Boost_LIBRARIES} 
                                ${PSAPI_LIBRARIES} 
                                pthread                                
                                )
                                
    add_test (NAME ${executable_name} COMMAND $<TARGET_FILE:${executable_name}>)

endfunction(add_new_test)


add_subdirectory(unit_test)

