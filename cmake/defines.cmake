###########################################################################################################################################
##                                                                                                                                       ##
## Provides miscellaneous CMake defines.                                                                                                 ##
##                                                                                                                                       ##
## TR_HAS_GCC, TR_HAS_CLANG, TR_HAS_CLANG_CL, TR_HAS_MSVC are defined for simpler compiler detection.                                    ##
##                                                                                                                                       ##
## TR_DO_NOTHING defines a dummy command that when called does nothing, useful in generator expressions.                                 ##
##                                                                                                                                       ##
###########################################################################################################################################

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	if(CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
		set(TR_HAS_CLANG_CL 1 CACHE BOOL INTERNAL)
	else()
		set(TR_HAS_CLANG 1 CACHE BOOL INTERNAL)
	endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(TR_HAS_GCC 1 CACHE BOOL INTERNAL)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(TR_HAS_MSVC 1 CACHE BOOL INTERNAL)
endif()

if(UNIX)
    set(TR_DO_NOTHING "true" CACHE STRING INTERNAL)
elseif(WIN32)
    set(TR_DO_NOTHING "break" CACHE STRING INTERNAL)
endif()