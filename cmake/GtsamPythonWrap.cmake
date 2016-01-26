#Setup cache options
#set(GTSAM_BUILD_PYTHON_FLAGS "" CACHE STRING "Extra flags for running Matlab PYTHON compilation")
#set(GTSAM_PYTHON_INSTALL_PATH "" CACHE PATH "Python toolbox destination, blank defaults to CMAKE_INSTALL_PREFIX/borg/python")
#if(NOT GTSAM_PYTHON_INSTALL_PATH)
#  set(GTSAM_PYTHON_INSTALL_PATH "${CMAKE_INSTALL_PREFIX}/borg/python")
#endif()

# Build python wrapper
# Author: Andrew Melim and Frank Dellaert
# Arguments:
# interfaceHeader:  The relative path to the wrapper interface definition file.
function(wrap_python interfaceHeader)

	# Find path to interface header file. NOTE(frank): don't know why second line needed
	get_filename_component(modulePath "${interfaceHeader}" PATH)
	get_filename_component(absoluteInterfaceHeader "${interfaceHeader}" ABSOLUTE)
	
	# Devise module name from interface header file
	get_filename_component(moduleName "${interfaceHeader}" NAME_WE)
	
	# Create directory for generated files
	set(generated_files_path "${PROJECT_BINARY_DIR}/wrap/${moduleName}")
	file(MAKE_DIRECTORY "${generated_files_path}")
	
	# The product of wrapping is this file, which will be a dependency for a library
	set(generated_cpp_file "${generated_files_path}/${moduleName}_python.cpp")
	
	# Set up generation of module source file
	add_custom_command(
		OUTPUT ${generated_cpp_file}
		DEPENDS ${absoluteInterfaceHeader} wrap
		COMMAND wrap ${modulePath} ${moduleName} ${generated_files_path} python
		WORKING_DIRECTORY ${generated_files_path}
		VERBATIM
	)

	# Create the library
	add_library(${moduleName} SHARED ${generated_cpp_file})
	set_target_properties(${moduleName} PROPERTIES
		OUTPUT_NAME         ${moduleName}
		SKIP_BUILD_RPATH    TRUE   # TODO(frank): try without this !
		CLEAN_DIRECT_OUTPUT 1
	)

	target_link_libraries(${moduleName}
	                      ${Boost_PYTHON${BOOST_PYTHON_VERSION_SUFFIX_UPPERCASE}_LIBRARY}
	                      ${PYTHON_LIBRARY} gtsam)

	# Cause the library to be output in the correct directory.
	# TODO: Change below to work on different systems (currently works only with Linux)
	add_custom_command(
		OUTPUT ${CMAKE_BINARY_DIR}/python/gtsam/_lib${moduleName}.so
		DEPENDS ${moduleName}
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${moduleName}> ${CMAKE_BINARY_DIR}/python/gtsam/_lib${moduleName}.so
		COMMENT "Copying extension module to python/gtsam/_lib${moduleName}.so"
	)
	add_custom_target(copy_python_module ALL DEPENDS ${CMAKE_BINARY_DIR}/python/gtsam/_lib${moduleName}.so)
endfunction(wrap_python)

#Author: Paul Furgale Modified by Andrew Melim
# User-friendly wrapping function.  Builds a mex module from the provided
# interfaceHeader.  For example, for the interface header gtsam.h,
# this will build the wrap module 'gtsam'.
#
# Arguments:
# interfaceHeader:  The relative path to the wrapper interface definition file.
# linkLibraries:    Any *additional* libraries to link.  Your project library
#                   (e.g. `lba`), libraries it depends on, and any necessary
#                   Python libraries will be linked automatically.  So normally,
#                   leave this empty.
# extraIncludeDirs: Any *additional* include paths required by dependent
#                   libraries that have not already been added by
#                   include_directories.  Again, normally, leave this empty.
function(wrap_and_install_python interfaceHeader linkLibraries extraIncludeDirs)

#  message("Wrap and Install Python")
#
#  #Obtain Dependencies
#  find_package(Boost COMPONENTS python REQUIRED)
#  include_directories(${Boost_INCLUDE_DIRS})
#
#  # Find Python
#  find_package(PythonLibs 2.7 REQUIRED)
#  include_directories(${PYTHON_INCLUDE_DIRS})
#
#  # Find NumpyEigen
#  # find_package(numpy_eigen REQUIRED)
#  include_directories(${CMAKE_SOURCE_DIR}/gtsam/3rdparty/numpy_eigen/include/)
#  # include_directories(${numpy_eigen_INCLUDE_DIRS})
#  if(GTSAM_USE_SYSTEM_EIGEN)
#     find_package(Eigen3 REQUIRED)
#  else()
#     include_directories(${CMAKE_SOURCE_DIR}/gtsam/3rdparty/Eigen/)
#  endif()
#
#	get_filename_component(interfaceHeader "${interfaceHeader}" ABSOLUTE)
#	get_filename_component(modulePath "${interfaceHeader}" PATH)
#  get_filename_component(moduleName "${interfaceHeader}" NAME_WE)
#  #set(moduleName python)
#
#	# Paths for generated files
#	set(generated_files_path "${PROJECT_BINARY_DIR}/wrap/${moduleName}")
#	set(generated_cpp_file "${generated_files_path}/${moduleName}_python.cpp")
#
#	message(STATUS "Building wrap module ${moduleName}")
#
#	file(MAKE_DIRECTORY "${generated_files_path}")
#  add_custom_command(
#		OUTPUT ${generated_cpp_file}
#		DEPENDS ${interfaceHeader} wrap
#    COMMAND
#            wrap
#            ${modulePath}
#            ${moduleName}
#            ${generated_files_path}
#            ${matlab_h_path}
#            python
#		WORKING_DIRECTORY ${generated_files_path}
#		VERBATIM
#    )
#
#  set(gtsamLib gtsam)
#  #TODO(Andrew): Reimplement below
#
#  IF(APPLE)
#    # The apple framework headers don't include the numpy headers for some reason.
#    GET_FILENAME_COMPONENT(REAL_PYTHON_INCLUDE ${PYTHON_INCLUDE_DIRS} REALPATH)
#    IF( ${REAL_PYTHON_INCLUDE} MATCHES Python.framework)
#      message("Trying to find extra headers for numpy from ${REAL_PYTHON_INCLUDE}.")
#      message("Looking in ${REAL_PYTHON_INCLUDE}/../../Extras/lib/python/numpy/core/include/numpy")
#      FIND_PATH(NUMPY_INCLUDE_DIR arrayobject.h
#  ${REAL_PYTHON_INCLUDE}/../../Extras/lib/python/numpy/core/include/numpy
#  ${REAL_PYTHON_INCLUDE}/numpy
#  )
#      IF(${NUMPY_INCLUDE_DIR} MATCHES NOTFOUND)
#  message("Unable to find numpy include directories: ${NUMPY_INCLUDE_DIR}")
#      ELSE()
#  message("Found numpy headers at ${NUMPY_INCLUDE_DIR}")
#  INCLUDE_DIRECTORIES(${NUMPY_INCLUDE_DIR})
#  INCLUDE_DIRECTORIES(${NUMPY_INCLUDE_DIR}/..)
#      ENDIF()
#    ENDIF()
#  ENDIF(APPLE)
#
#  if(MSVC)
#    add_library(${moduleName}_python MODULE ${ARGN})
#    set_target_properties(${moduleName}_python PROPERTIES
#        OUTPUT_NAME         ${moduleName}_python
#        CLEAN_DIRECT_OUTPUT 1
#        VERSION             1
#        SOVERSION           0
#        SUFFIX              ".pyd")
#      target_link_libraries(${moduleName}_python ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARY} ${gtsamLib}) #temp
#
#    set(PYLIB_OUTPUT_FILE $<TARGET_FILE:${moduleName}_python>)
#    message(${PYLIB_OUTPUT_FILE})
#    get_filename_component(PYLIB_OUTPUT_NAME ${PYLIB_OUTPUT_FILE} NAME_WE)
#    set(PYLIB_SO_NAME ${PYLIB_OUTPUT_NAME}.pyd)
#
#  ELSE()
#    # Create a shared library
#    add_library(${moduleName}_python SHARED ${generated_cpp_file})
#
#    set_target_properties(${moduleName}_python PROPERTIES
#        OUTPUT_NAME         ${moduleName}_python
#        CLEAN_DIRECT_OUTPUT 1)
#      target_link_libraries(${moduleName}_python ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARY} ${gtsamLib}) #temp
#    # On OSX and Linux, the python library must end in the extension .so. Build this
#    # filename here.
#    get_property(PYLIB_OUTPUT_FILE TARGET ${moduleName}_python PROPERTY LOCATION)
#    set(PYLIB_OUTPUT_FILE $<TARGET_FILE:${moduleName}_python>)
#    message(${PYLIB_OUTPUT_FILE})
#    get_filename_component(PYLIB_OUTPUT_NAME ${PYLIB_OUTPUT_FILE} NAME_WE)
#    set(PYLIB_SO_NAME lib${moduleName}_python.so)
#  ENDIF(MSVC)
#
#  # Installs the library in the gtsam folder, which is used by setup.py to create the gtsam package
#  set(PYTHON_MODULE_DIRECTORY ${CMAKE_SOURCE_DIR}/python/gtsam)
#  # Cause the library to be output in the correct directory.
#  add_custom_command(TARGET ${moduleName}_python
#    POST_BUILD
#    COMMAND cp -v ${PYLIB_OUTPUT_FILE} ${PYTHON_MODULE_DIRECTORY}/_${PYLIB_SO_NAME}
#    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#    COMMENT "Copying library files to python directory" )
#
#  get_directory_property(AMCF ADDITIONAL_MAKE_CLEAN_FILES)
#  list(APPEND AMCF ${PYTHON_MODULE_DIRECTORY}/${PYLIB_SO_NAME})
#  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${AMCF}")

endfunction(wrap_and_install_python)
