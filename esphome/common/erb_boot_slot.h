// Authors: Grok and Eldon R. Brown - WA0UWH
// Rev: 20261104o
// erb_boot_slot - "app0 (USB/factory)" / "app1 (OTA)" etc. for the Boot Slot text_sensor (ESP32 only).
// Method: a USB factory image written at 0x0 covers otadata (0x9000-0xAFFF) with 0xFF, so otadata has no valid
// entry -> esp_ota_get_state_partition() = ESP_ERR_NOT_FOUND -> USB/factory. Every OTA (ESPHome web/OTA,
// esp_ota_set_boot_partition) writes a CRC-valid otadata entry for the slot it boots -> ESP_OK -> OTA.
#pragma once
#include <string>
#include "esp_ota_ops.h"
inline std::string erb_boot_slot() {
  const esp_partition_t *p = esp_ota_get_running_partition();
  if (p == nullptr) return "unknown";
  esp_ota_img_states_t st;
  bool ota = esp_ota_get_state_partition(p, &st) == ESP_OK;
  return std::string(p->label) + (ota ? " (OTA)" : " (USB/factory)");
}
// End
