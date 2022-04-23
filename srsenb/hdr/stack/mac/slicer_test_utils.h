#ifndef __SLICER_TEST_UTILS_H_
#define __SLICER_TEST_UTILS_H_

#include "slicer_defs.h"
#include <iomanip>
#include <iostream>

namespace slicer_test {

std::vector<slicer::slice_t> slices{
//  name      share      imsis
  { "slice0", { 12 },  { 1010123456789, 1010123456790, 1010123456791 }},
  { "slice1", { 90 },  { 1010123456792, 1010123456793, 1010123456794 }},
  { "slice2", { 30 },  { 1010123456795, 1010123456796, 1010123456797 }},
  { "slice3", { 30 },  { 1010123456798, 1010123456799 }},
};

std::vector<slicer::slice_imsis_t> imsis_to_unbind{
//  name        imsis
  { "slice0", { 1010123456790, 1010123456791 }},
  { "slice1", { 1010123456793, 1010123456794 }},
  { "slice2", { 1010123456796, 1010123456797 }},
  { "slice3", { 1010123456799 }},
};

std::vector<std::string> slices_to_delete{ "slice1", "slice3" };

void print_imsis(std::vector<uint64_t> imsi_list) {
  for (auto it = imsi_list.begin(); it != imsi_list.end(); ++it) {
    std::cout << std::setw(16) << std::left << *it;
  }
  std::cout << std::endl;
}

void print_slice(slicer::slice_t slice) {
  std::cout << "name: " << std::setw(10) << std::left << slice.config.name
            << "share: " << std::setw(6) << std::left << slice.config.prop_alloc_policy.share
            << "imsi_list: ";
  print_imsis(slice.imsi_list);
}

void print_ue_status(slicer::ue_status_t status) {
  std::cout << "imsi: " << std::setw(16) << std::left << status.imsi
            << "connected: " << std::setw(6) << std::left << status.connected
            << "crnti: " << std::setw(8) << std::left << status.crnti;
  std::cout << std::endl;
}

void print_slice_status(slicer::slice_status_t status) {
  std::cout << "name: " << std::setw(10) << std::left << status.config.name
            << "share: " << std::setw(6) << std::left << status.config.prop_alloc_policy.share
            << "ue_list: ";
  std::cout << std::endl;
  for (auto it = status.ue_list.begin(); it != status.ue_list.end(); ++it)
    print_ue_status(*it);
}

} // namespace slicer

#endif // __SLICER_TEST_UTILS_H_
