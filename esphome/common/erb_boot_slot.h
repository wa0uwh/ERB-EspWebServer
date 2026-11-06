// Authors: Grok and Eldon R. Brown - WA0UWH
// Rev: 20261106a
// erb_boot_slot - "app0 (USB/factory)" / "app1 (OTA)" etc. for the Boot Slot text_sensor (ESP32 only).
// Method (rev p): read the two raw otadata entries (otadata partition, 0x9000 and 0xA000, 32 B each) and do
// what the IDF bootloader does: the ACTIVE entry is the CRC-valid one (crc == crc32_le(~0, ota_seq, 4), seq != ~0)
// with the highest ota_seq, state not INVALID/ABORTED. It must select the running slot AND have ota_seq >= 2 -> "(OTA)",
// else "(USB/factory)". ota_seq 1 is never made by an OTA: when both otadata copies are blank/CRC-invalid (fresh USB
// factory flash) the IDF 5.5.6 bootloader itself writes otadata[0] = {seq 1, VALID} on first boot of app0
// (bootloader_utility.c set_actual_ota_seq). esp_ota_set_boot_partition always picks a seq above the current max,
// so the first OTA is seq 2 (app1), the next seq 3 (app0), etc.
// USB sentinel: the factory.bin is post-processed (see NOTES.md) so both otadata copies start with
//   "ERBUSB" + 26 zero bytes  (ota_seq 0x55425245, crc 0 = CRC-INVALID -> bootloader ignores it, boots app0).
// It is not 0xFF, so a flasher that skips all-0xFF pages still overwrites old OTA records when the factory.bin
// is written at 0x0. The .ota.bin has no sentinel. A later OTA writes one valid entry -> OTA wins over the sentinel.
// Rev o bug: it used esp_ota_get_state_partition()==ESP_OK, which is true for the bootloader's seq-1 entry after every
// USB flash (so always "app0 (OTA)"), and for ANY CRC-valid leftover entry (even INVALID state or not active).
// The bootloader's seq-1 write erases sector 0 (our sentinel there); the copy at 0xA000 stays until the first OTA.
#pragma once
#include <string>
#include <cstring>
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_flash_partitions.h"
#include "esp_rom_crc.h"
inline bool erb_otadata_valid(const esp_ota_select_entry_t &e) {
  if (e.ota_seq == UINT32_MAX || e.ota_state == ESP_OTA_IMG_INVALID || e.ota_state == ESP_OTA_IMG_ABORTED) return false;
  return e.crc == esp_rom_crc32_le(UINT32_MAX, (const uint8_t *) &e.ota_seq, 4);
}
inline std::string erb_boot_slot() {
  const esp_partition_t *Run = esp_ota_get_running_partition();
  if (Run == nullptr) return "unknown";
  std::string Name(Run->label);
  const esp_partition_t *Od = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_OTA, nullptr);
  int Count = esp_ota_get_app_partition_count();
  if (Od == nullptr || Count == 0 || Run->subtype < ESP_PARTITION_SUBTYPE_APP_OTA_MIN || Run->subtype >= ESP_PARTITION_SUBTYPE_APP_OTA_MAX)
    return Name + " (USB/factory)";
  esp_ota_select_entry_t E[2];
  int Best = -1;
  for (int i = 0; i < 2; i++) {
    if (esp_partition_read(Od, i * Od->erase_size, &E[i], sizeof(E[i])) != ESP_OK) continue;
    if (!erb_otadata_valid(E[i])) continue;  // erased, CRC-invalid, ERBUSB sentinel, or rolled back
    if (Best < 0 || E[i].ota_seq > E[Best].ota_seq) Best = i;
  }
  bool Ota = Best >= 0 && E[Best].ota_seq >= 2 && (int) ((E[Best].ota_seq - 1) % Count) == (int) (Run->subtype - ESP_PARTITION_SUBTYPE_APP_OTA_MIN);
  return Name + (Ota ? " (OTA)" : " (USB/factory)");
}
// End
