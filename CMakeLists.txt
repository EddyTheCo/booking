cmake_minimum_required(VERSION 3.16)
project(IOTA-BOOKING LANGUAGES CXX)

include(local_conf.cmake OPTIONAL)

find_package(Qt6 REQUIRED COMPONENTS Core )

add_library(booking booking.cpp include/booking.hpp)

target_include_directories(booking PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	"$<INSTALL_INTERFACE:include>")
target_link_libraries(booking PUBLIC
	Qt6::Core
	)

install(TARGETS booking EXPORT bookingTargets DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/)

install(EXPORT bookingTargets
	FILE bookingTargets.cmake
	DESTINATION lib/cmake/booking
	)
include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/bookingConfig.cmake"
	INSTALL_DESTINATION "lib/cmake/booking"
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/bookingConfigVersion.cmake"
	VERSION "0.1.1.0"
	COMPATIBILITY AnyNewerVersion
	)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/bookingConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/bookingConfigVersion.cmake
	DESTINATION lib/cmake/booking
	)
export(EXPORT bookingTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/bookingTargets.cmake"
	)
