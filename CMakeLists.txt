project(pepega_installer)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(installer/source installer_SOURCE)
aux_source_directory(installer/include installer_INCLUDE)
aux_source_directory(installer/resources/source resource_SOURCE)
aux_source_directory(installer/resources/include resource_INCLUDE)

set(wxWidgets_ROOT_DIR "C:/C++/wxWidgets")
set(wxWidgets_INCLUDE_DIRS "C:/C++/wxWidgets/include/wx")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(wxWidgets_LIB_DIR "C:/C++/wxWidgets/lib/debug/gcc_x64_lib")
    set(wxWidgets_LIBRARIES "C:/C++/wxWidgets/lib/debug")
    set(wxWidgets_CONFIGURATION mswud)

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_compile_options("-mwindows")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mwindows")
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -mwindows")

    set(wxWidgets_LIB_DIR "C:/C++/wxWidgets/lib/release/gcc_x64_lib")
    set(wxWidgets_LIBRARIES "C:/C++/wxWidgets/lib/release")
    set(wxWidgets_CONFIGURATION mswu)

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O3")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O3")
endif()

find_package(wxWidgets COMPONENTS core base png REQUIRED)
include(${wxWidgets_USE_FILE})

add_executable(installer ${installer_SOURCE} ${installer_INCLUDE} ${resource_SOURCE} ${resource_INCLUDE})
target_compile_features(installer PRIVATE cxx_std_17)
target_link_libraries(installer ${wxWidgets_LIBRARIES})

set(PEPEGA_INSTALLER_LINKFLAGS ${CMAKE_CURRENT_SOURCE_DIR}/installer/resources/source/icon.res)
message(${PEPEGA_INSTALLER_LINKFLAGS})
set_target_properties(installer PROPERTIES LINK_FLAGS ${PEPEGA_INSTALLER_LINKFLAGS})