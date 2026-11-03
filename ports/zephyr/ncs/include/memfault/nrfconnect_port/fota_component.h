/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @brief  Registration of non-primary OTA components (modem firmware, bootloader, ...) for the
 * memfault backend (CONFIG_MEMFAULT_ZEPHYR_FOTA_BACKEND_NRF_CLOUD_OVERRIDE).
 *
 * Every registered component is checked against Memfault's multi-component OTA API
 * (ota/latest/url?software_type=<type>&current_version=<version>) for updates.
 * Adding a component is done using the MEMFAULT_FOTA_COMPONENT_DEFINE macro
 *
 * @code
 *   static int mcuboot_version_get(char *buf, size_t len) {
 *     snprintf(buf, len, "mcuboot_%s", my_bootloader_version());
 *     return 0;
 *   }
 *   MEMFAULT_FOTA_COMPONENT_DEFINE(mcuboot, "mcuboot", mcuboot_version_get,
 *                                  NRF_CLOUD_FOTA_BOOTLOADER);
 * @endcode
 *
 * The version string must match, exactly, the component version of the artifact uploaded to the
 * Memfault release for this software type. Components are checked in alphabetical order of the
 * symbol name passed as the first macro argument (the linker sorts the registry by name).
 * The first component with an update wins; one component is installed per poll.
 */

#ifndef MEMFAULT_NRFCONNECT_FOTA_COMPONENT_H
#define MEMFAULT_NRFCONNECT_FOTA_COMPONENT_H

#include <stddef.h>
#include <zephyr/sys/iterable_sections.h>

#include "net/nrf_cloud.h"

struct memfault_fota_component {
	/* The software type of the component. */
	const char *software_type;
	/* Function to get the current version of the component. */
	int (*version_get)(char *buf, size_t buf_len);
	/* The FOTA type of the component. */
	enum nrf_cloud_fota_type fota_type;
};

//! Registers a component. All components are software types of the same Memfault project.
#define MEMFAULT_FOTA_COMPONENT_DEFINE(_name, _software_type, _version_get, _fota_type)            \
	const STRUCT_SECTION_ITERABLE(memfault_fota_component, _name) = {                          \
		.software_type = (_software_type),                                                 \
		.version_get = (_version_get),                                                     \
		.fota_type = (_fota_type),                                                         \
	}

/**
 * @brief Starts the FOTA process for the specified component.
 *
 * @param software_type The software type of the component to update.
 * @return 0 if no update is pending or the poll was started, negative errno on failure
 *         (-ENOENT if no component with that software type is registered)
 */
int memfault_zephyr_fota_component_start(const char *software_type);

/**
 * @brief Fetches the pending download URL for the specified component.
 *
 * @param software_type The software type of the component.
 * @param url Pointer to a char pointer that will be set to the download URL if available.
 * @return 1 if an update is available, 0 if not, negative errno on failure
 */
int memfault_zephyr_fota_component_get_download_url(const char *software_type, char **url);

#endif /* MEMFAULT_NRFCONNECT_FOTA_COMPONENT_H */
