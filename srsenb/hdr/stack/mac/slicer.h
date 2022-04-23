#ifndef __SLICER_H_
#define __SLICER_H_

//#include "srsran/common/common.h"
//#include "srsran/common/log.h"
#include <map>
#include <mutex>
#include <string>
#include <stdint.h>
#include <vector>

#include "srsran/interfaces/enb_interfaces.h"
#include "slicer_defs.h"

namespace slicer {

class slicer {
  public:
    slicer();
    ~slicer();

    // for mac
    void init(const srsenb::slicer_args_t& args_);
    std::vector<uint16_t> get_cur_sf_crntis(uint32_t tti_tx_dl);
    std::vector<uint16_t> get_all_slice_crntis();

    // slicer interface for agent
    std::vector<slice_status_t> slice_status(std::vector<std::string> slice_names);
    bool slice_config(std::vector<slice_config_t> slice_configs);
    bool slice_ue_bind(std::string slice_name, std::vector<uint64_t> imsi_list);
    bool slice_ue_unbind(std::string slice_name, std::vector<uint64_t> imsi_list);
    bool slice_delete(std::vector<std::string> slice_names);

    // for rrc
    int upd_member_crnti(uint64_t imsi, uint16_t crnti);
    int upd_member_crnti(uint32_t tmsi, uint16_t crnti);
    int upd_member_crnti(uint16_t old_crnti, uint16_t new_crnti);

    bool enable = false;
    bool initialized = false;
    bool workshare = false;
 private:
    bool read_slice_db_file(std::string db_filename);
    int add_slice(slice_t slice);
    void upd_sf_alloc();
    void upd_slice_crntis(std::string s_name);

    std::map<std::string, std::vector<uint16_t> > slice_to_crnti_vec;
    std::vector<uint16_t> all_slice_crntis;
    std::map<std::string, slice_t> slices;
    std::map<std::string, slice_t>::iterator slice_iter; // reused often
    std::vector<uint32_t> sf_alloc;
    uint32_t total_sf_alloc = 0;
    uint32_t sliced_unsliced_ratio = 20;
    bool has_alloc = false;
    std::mutex slicer_mutex;
    uint32_t alloc_index = 0;

    // for tracking all UE identifiers
    std::map<uint64_t, uint16_t> imsi_to_crnti;
    std::map<uint32_t, uint64_t> tmsi_to_imsi;
};

  // helper functions
  std::vector<std::string> split_string(const std::string& str, char delimiter);
  uint32_t calc_gcf(uint32_t a, uint32_t b);
  uint32_t calc_gcf_vec(std::vector<uint32_t> v);

} // namespace slicer

#endif // __SLICER_H_
