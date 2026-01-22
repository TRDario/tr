###########################################################################################################################################
##                                                                                                                                       ##
## Provides a utility for setting a target's properties in a standard manner.                                                            ##
##                                                                                                                                       ##
## The target will be set to use C++20.                                                                                                  ##
## The target will have a 'd' suffix in debug builds, for example 'foobar' -> foobard.exe.                                               ##
## The target will have a number of compiler flags set, mostly enabling warnings, but with some floating point optimizations enabled.    ##
## On Linux, the libstdc++ debug mode will be used in the debug configuration.                                                           ##
## If std::format is detected, TR_HAS_STD_FORMAT is defined.                                                                             ##
## In the RelWithDebInfo and Debug configurations, TR_ENABLE_ASSERTS is defined.                                                         ##
## In the Debug configuration, TR_ENABLE_GL_CHECKS is defined.                                                                           ##
##                                                                                                                                       ##
###########################################################################################################################################

# Sets a target's properties.
function(tr_target_template TARGET)
	set_target_properties (${TARGET} PROPERTIES DEBUG_POSTFIX "d")
	target_compile_features(${TARGET} PUBLIC cxx_std_20)

	if(TR_HAS_CLANG OR TR_HAS_GCC)
		target_compile_options(${TARGET} PRIVATE
			-Wall
			-Wextra
			-Wpedantic
			-Wshadow
			-fno-trapping-math
			-fno-math-errno
			-fno-signed-zeros
			-fassociative-math
			-ffp-contract=fast
		)
		if(TR_HAS_CLANG)
			target_link_options(${TARGET} PRIVATE -fuse-ld=lld)
		endif()
	elseif(TR_HAS_CLANG_CL)
		target_compile_options(${TARGET} PRIVATE
			/clang:-Wall
			/clang:-Wextra
			/clang:-Wpedantic
			/clang:-Wshadow
			/clang:-fno-trapping-math
			/clang:-fno-math-errno
			/clang:-fno-signed-zeros
			/clang:-fassociative-math
			/clang:-ffp-contract=fast
		)
	elseif(TR_HAS_MSVC)
		target_compile_options(${TARGET} PUBLIC
			/Zc:preprocessor
			/W3 
			/WX
		)
	endif()
	
	if(LINUX)
		target_compile_definitions(${TARGET} PUBLIC $<$<CONFIG:Debug>:_GLIBCXX_DEBUG _GLIBCXX_DEBUG_PEDANTIC>)
	endif()
	if(TR_HAS_STD_FORMAT)
		target_compile_definitions(${TARGET} PUBLIC TR_HAS_STD_FORMAT)
	endif()
	target_compile_definitions(${TARGET} PUBLIC
		$<$<CONFIG:Debug>:TR_ENABLE_GL_CHECKS>
		$<$<CONFIG:Debug>:TR_ENABLE_ASSERTS>$<$<CONFIG:RelWithDebInfo>:TR_ENABLE_ASSERTS>
	)
endfunction()