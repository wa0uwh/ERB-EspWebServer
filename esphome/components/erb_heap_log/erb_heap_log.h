// Authors: Grok and Eldon R. Brown - WA0UWH
// Rev: 20261104l
// ErbHeapLog - every 60 s store {stamp, free heap, min free heap, boot} (8 B) in a 2560-record ring
// kept as 40 NVS blobs of 64 records (namespace "erbheap"). NVS spreads wear; partition table unchanged.
// Any NVS failure -> warning + disabled; the device keeps running (never blocks boot).
// GET /heap_history.csv -> "epoch_or_uptime_s,free_B,minfree_B,boot" oldest first (chunked).
#pragma once
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include "esphome/components/web_server_base/web_server_base.h"
#include <nvs.h>
#include <esp_heap_caps.h>
#include <esp_http_server.h>
#include <time.h>
#include <cstring>
#include <cstdio>

namespace esphome {
namespace erb_heap_log {

static const char *const TAG = "erb_heap_log";
static const uint32_t MAGIC = 0x45524248;  // "ERBH"
static const uint16_t PER_CHUNK = 64, CHUNKS = 40, TOTAL = PER_CHUNK * CHUNKS;

struct __attribute__((packed)) Rec {
  uint32_t stamp;   // epoch s (if > 1e9) else uptime s
  uint16_t free16;  // free heap / 16
  uint16_t bm;      // (boot & 0xF) << 12 | min free heap / 256
};
struct Hdr {
  uint32_t magic;
  uint16_t head;    // next write index
  uint16_t count;
  uint32_t boot;
};

class ErbHeapLog : public Component, public AsyncWebHandler {
 public:
  float get_setup_priority() const override { return setup_priority::LATE; }

  void setup() override {
    if (nvs_open("erbheap", NVS_READWRITE, &this->h_) != ESP_OK) {
      ESP_LOGW(TAG, "NVS open failed - heap history disabled");
      return;
    }
    size_t n = sizeof(this->hdr_);
    if (nvs_get_blob(this->h_, "hdr", &this->hdr_, &n) != ESP_OK || n != sizeof(this->hdr_) ||
        this->hdr_.magic != MAGIC || this->hdr_.head >= TOTAL || this->hdr_.count > TOTAL) {
      this->hdr_ = {MAGIC, 0, 0, 0};
    }
    this->hdr_.boot++;
    this->load_chunk_(this->hdr_.head / PER_CHUNK);
    this->ok_ = this->save_hdr_();
    if (!this->ok_) ESP_LOGW(TAG, "NVS write failed - heap history disabled");
    if (web_server_base::global_web_server_base != nullptr)
      web_server_base::global_web_server_base->add_handler(this);
    this->set_interval("erb_heap", 60000, [this]() { this->sample_(); });
  }

  void dump_config() override {
    ESP_LOGCONFIG(TAG, "ERB heap log: %s, boot %u, %u/%u records, GET /heap_history.csv", this->ok_ ? "on" : "OFF",
                  (unsigned) this->hdr_.boot, this->hdr_.count, TOTAL);
  }

  void clear() {
    if (!this->ok_) return;
    for (uint16_t c = 0; c < CHUNKS; c++) {
      char k[8];
      snprintf(k, sizeof(k), "c%u", c);
      nvs_erase_key(this->h_, k);
    }
    this->hdr_.head = 0;
    this->hdr_.count = 0;
    memset(this->buf_, 0, sizeof(this->buf_));
    this->save_hdr_();
    ESP_LOGI(TAG, "heap history cleared");
  }

  bool canHandle(AsyncWebServerRequest *request) const override {
    char b[AsyncWebServerRequest::URL_BUF_SIZE];
    return request->method() == HTTP_GET && request->url_to(b) == "/heap_history.csv";
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    httpd_req_t *r = *request;
    httpd_resp_set_type(r, "text/csv");
    httpd_resp_set_hdr(r, "Cache-Control", "no-store");
    httpd_resp_sendstr_chunk(r, "stamp,free,minfree,boot\n");
    if (this->ok_ && this->hdr_.count) {
      uint16_t cnt = this->hdr_.count, start = (this->hdr_.head + TOTAL - cnt) % TOTAL;
      Rec chunk[PER_CHUNK];
      int loaded = -1;
      char line[1400];
      size_t len = 0;
      for (uint16_t i = 0; i < cnt; i++) {
        uint16_t idx = (start + i) % TOTAL, c = idx / PER_CHUNK;
        if (c != loaded) {
          memset(chunk, 0, sizeof(chunk));
          char k[8];
          snprintf(k, sizeof(k), "c%u", c);
          size_t n = sizeof(chunk);
          nvs_get_blob(this->h_, k, chunk, &n);
          loaded = c;
        }
        const Rec &e = chunk[idx % PER_CHUNK];
        if (!e.stamp && !e.free16) continue;
        len += snprintf(line + len, sizeof(line) - len, "%u,%u,%u,%u\n", (unsigned) e.stamp, (unsigned) e.free16 * 16,
                        (unsigned) (e.bm & 0xFFF) * 256, (unsigned) (e.bm >> 12));
        if (len > sizeof(line) - 48) {
          httpd_resp_send_chunk(r, line, len);
          len = 0;
        }
      }
      if (len) httpd_resp_send_chunk(r, line, len);
    }
    httpd_resp_send_chunk(r, nullptr, 0);
  }

 protected:
  void load_chunk_(uint16_t c) {
    memset(this->buf_, 0, sizeof(this->buf_));
    char k[8];
    snprintf(k, sizeof(k), "c%u", c);
    size_t n = sizeof(this->buf_);
    nvs_get_blob(this->h_, k, this->buf_, &n);
  }
  bool save_hdr_() {
    return nvs_set_blob(this->h_, "hdr", &this->hdr_, sizeof(this->hdr_)) == ESP_OK && nvs_commit(this->h_) == ESP_OK;
  }
  void sample_() {
    if (!this->ok_) return;
    time_t now = time(nullptr);
    uint32_t st = now > 1600000000 ? (uint32_t) now : millis() / 1000;
    uint32_t fr = heap_caps_get_free_size(MALLOC_CAP_INTERNAL), mn = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
    uint16_t c = this->hdr_.head / PER_CHUNK;
    Rec &e = this->buf_[this->hdr_.head % PER_CHUNK];
    e.stamp = st;
    e.free16 = fr / 16 > 0xFFFF ? 0xFFFF : fr / 16;
    e.bm = (uint16_t) (((this->hdr_.boot & 0xF) << 12) | (mn / 256 > 0xFFF ? 0xFFF : mn / 256));
    char k[8];
    snprintf(k, sizeof(k), "c%u", c);
    if (nvs_set_blob(this->h_, k, this->buf_, sizeof(this->buf_)) != ESP_OK) {
      ESP_LOGW(TAG, "NVS blob write failed (heap history paused)");
      return;
    }
    this->hdr_.head = (this->hdr_.head + 1) % TOTAL;
    if (this->hdr_.count < TOTAL) this->hdr_.count++;
    if (this->hdr_.head % PER_CHUNK == 0) this->load_chunk_(this->hdr_.head / PER_CHUNK);  // next chunk (old data overwritten)
    this->save_hdr_();
  }

  nvs_handle_t h_{0};
  Hdr hdr_{MAGIC, 0, 0, 0};
  Rec buf_[PER_CHUNK]{};
  bool ok_{false};
};

}  // namespace erb_heap_log
}  // namespace esphome
// End
