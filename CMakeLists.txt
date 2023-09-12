cmake_minimum_required(VERSION 3.16)
include(local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(ccommon)
version_from_git(
	LOG       OFF
	TIMESTAMP "%Y%m%d%H%M%S"
	)
project(Bookings VERSION ${VERSION} LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

find_package(Qt6 COMPONENTS Core )

if (Qt6_FOUND)
	add_library(booking include/booking.hpp src/booking.cpp)

	target_compile_definitions(booking PRIVATE WINDOWS_BOOKI)
	target_include_directories(booking PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/booking>")
	target_link_libraries(booking PUBLIC
		Qt6::Core
		)

	install(TARGETS booking EXPORT bookingTargets
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		)
	install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/booking
		)
	install(EXPORT bookingTargets
		FILE bookingTargets.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/booking
		)
	include(CMakePackageConfigHelpers)
	# generate the config file that is includes the exports
	configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
		"${CMAKE_CURRENT_BINARY_DIR}/bookingConfig.cmake"
		INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/booking"
		)
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/bookingConfigVersion.cmake"
		VERSION ${VERSION}
		COMPATIBILITY SameMajorVersion
		)
	install(FILES
		${CMAKE_CURRENT_BINARY_DIR}/bookingConfig.cmake
		${CMAKE_CURRENT_BINARY_DIR}/bookingConfigVersion.cmake
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/booking
		)
	export(EXPORT bookingTargets
		FILE "${CMAKE_CURRENT_BINARY_DIR}/bookingTargets.cmake"
		)
endif(Qt6_FOUND)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(CPACK_PACKAGE_CONTACT "estervtech")
	include(CTest)
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
	if(NOT BUILD_SHARED_LIBS)
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-static")
	endif(NOT BUILD_SHARED_LIBS)
	include(CPack)
endif()
if(BUILD_DOCS)
	get_target_property(build_docs cmake_build_docs SOURCES)
	include(${build_docs})
endif()
