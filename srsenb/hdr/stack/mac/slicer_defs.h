#ifndef __SLICER_DEFS_H_
#define __SLICER_DEFS_H_

#include "stdint.h"
#include <string>
#include <vector>

#define IN_CUR_SLICE 0
#define IN_OTHER_SLICE 1
#define IN_NO_SLICE 2

namespace slicer {

typedef struct prop_alloc_policy {
  uint32_t share;
} prop_alloc_policy_t;

typedef struct slice_config {
  std::string name;
  prop_alloc_policy_t prop_alloc_policy;
} slice_config_t;

typedef struct slice_imsis {
  std::string name;
  std::vector<uint64_t> imsi_list;
} slice_imsis_t;

typedef struct slice {
  slice_config_t config;
  std::vector<uint64_t> imsi_list;
} slice_t;

typedef struct ue_status {
    uint64_t imsi;
    bool connected;
    uint16_t crnti;
} ue_status_t;

typedef struct slice_status {
  slice_config_t config;
  std::vector<ue_status_t> ue_list;
} slice_status_t;

} // namespace slicer



#endif // __SLICER_DEFS_H_
