# Registry of OTA components, see memfault/nrfconnect_port/fota_component.h
#
# The registry is an iterable section. The CMake linker generator describes it in CMake, the
# other linker setups take the linker script snippet in sections.ld.
if(CONFIG_CMAKE_LINKER_GENERATOR)
  zephyr_iterable_section(NAME memfault_fota_component KVMA RAM_REGION GROUP RODATA_REGION)
else()
  zephyr_linker_sources(SECTIONS ${CMAKE_CURRENT_LIST_DIR}/sections.ld)
endif()
