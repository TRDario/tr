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
	target_compile_definitions(${TARGET} PUBLIC $<CONFIG:Debug>:TR_ENABLE_GL_CHECKS $<$<CONFIG:Debug>:TR_ENABLE_ASSERTS>$<$<CONFIG:RelWithDebInfo>:TR_ENABLE_ASSERTS>)
	if(TR_HAS_STD_FORMAT)
		target_compile_definitions(${TARGET} PUBLIC TR_HAS_STD_FORMAT)
	endif()
endfunction()