# Copyright (c) 2015-2016, Humanoid Lab, Georgia Tech Research Corporation
# Copyright (c) 2015-2017, Graphics Lab, Georgia Tech Research Corporation
# Copyright (c) 2016-2017, Personal Robotics Lab, Carnegie Mellon University
# This file is provided under the "BSD-style" License

# Find CCD
#
# This sets the following variables:
# CCD_FOUND
# CCD_INCLUDE_DIRS
# CCD_LIBRARIES
# CCD_VERSION

set(CCD_ROOT_DIR "../IPPP_third_party/" CACHE PATH "ccd root dir")

find_package(PkgConfig QUIET)

# Check to see if pkgconfig is installed.
pkg_check_modules(PC_CCD ccd QUIET)

find_path(CCD_INCLUDE_DIRS
        NAMES ccd/ccd.h
        HINTS ${PC_CCD_INCLUDEDIR}
        PATHS "${CMAKE_INSTALL_PREFIX}/include" "${CCD_ROOT_DIR}/include"
        PATH_SUFFIXES
        include)

find_library(CCD_LIBRARIES
        NAMES ccd
        HINTS ${PC_CCD_LIBDIR}
        PATHS "${CMAKE_INSTALL_PREFIX}" "${CCD_ROOT_DIR}"
        PATH_SUFFIXES
        lib)

# Version
set(CCD_VERSION ${PC_CCD_VERSION})

# Set (NAME)_FOUND if all the variables and the version are satisfied.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CCD
        FAIL_MESSAGE  DEFAULT_MSG
        REQUIRED_VARS CCD_INCLUDE_DIRS CCD_LIBRARIES
        VERSION_VAR CCD_VERSION)