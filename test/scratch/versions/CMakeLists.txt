define_example_program(test.scratch.versions main.cpp)
target_link_libraries(test.scratch.versions
	PRIVATE
		cstring::core
		shwild::core
)

if(Diagnosticism_FOUND)

	target_link_libraries(test.scratch.versions
		PRIVATE
			Diagnosticism::core
	)
endif()

target_link_libraries(test.scratch.versions
	PRIVATE
		$<$<STREQUAL:${STLSOFT_INCLUDE_DIR},>:STLSoft::STLSoft>
)

if(TARGET xTests::core)

	target_link_libraries(test.scratch.versions
		PRIVATE
			xTests::core
	)
endif()
