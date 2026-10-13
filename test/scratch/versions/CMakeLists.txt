define_example_program(test.scratch.versions main.cpp)
target_link_libraries(test.scratch.versions
	PRIVATE
		$<$<STREQUAL:${STLSOFT_INCLUDE_DIR},>:STLSoft::STLSoft>
		cstring::core
		Diagnosticism::core
		shwild::core
		xTests::core
)
