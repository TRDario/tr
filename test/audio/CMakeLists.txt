

add_executable(
	audio_test
	audio_buffer.cpp
	audio_context.cpp
	audio_device.cpp
    audio_device_list_view.cpp
	audio_source.cpp
)
target_link_libraries(
	audio_test
	tr::audio
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(audio_test)