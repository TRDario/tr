add_executable(
	audio_test
	audio_buffer.cpp
	audio_context.cpp
	audio_device.cpp
    audio_device_list_view.cpp
	audio_source.cpp
)
tr_target_template(audio_test)
target_link_libraries(audio_test PUBLIC GTest::gtest_main)
gtest_discover_tests(audio_test)