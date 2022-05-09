/**
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include "srsenb/hdr/stack/mac/schedulers/sched_time_rr_sliced.h"

namespace srsenb {

sched_time_rr_sliced::sched_time_rr_sliced(const sched_cell_params_t& cell_params_, const sched_interface::sched_args_t& sched_args,bool workshare_) : workshare(workshare_)
{
  cc_cfg = &cell_params_;
  srsran::console("[slicer metric] worksharing: %u", workshare_);

}

/*****************************************************************
 *                         Dowlink
 *****************************************************************/
std::vector<uint16_t> cur_slice_rntis, other_slice_rntis, no_slice_rntis;
void sched_time_rr_sliced::sched_dl_users(sched_ue_list& ue_db, sf_sched* tti_sched)
{
  if (ue_db.empty()) {
    return;
  }

  // Divide UE RNTIs into groups: cur_slice, other_slice, no_slice
  //std::vector<uint16_t> cur_slice_rntis, other_slice_rntis, no_slice_rntis;
  auto iter = ue_db.begin();
  for (uint32_t ue_count = 0; ue_count < ue_db.size(); ++iter, ++ue_count) {
    // srslte::console("rnti: 0x%x  slice_status: %u\n", iter->second.get_rnti(), iter->second.get_slice_status());
    switch (iter->second.get_slice_status()) {
      case IN_CUR_SLICE:
        cur_slice_rntis.push_back(iter->first);
        break;
      case IN_OTHER_SLICE:
        other_slice_rntis.push_back(iter->first);
        break;
      case IN_NO_SLICE:
        no_slice_rntis.push_back(iter->first);
        break;
    }

  // give priority in a time-domain RR basis.
  uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)ue_db.size();
  sched_dl_retxs(ue_db, tti_sched, priority_idx);
  sched_dl_newtxs(ue_db, tti_sched, priority_idx);
}

void sched_time_rr_sliced::sched_dl_retxs(sched_ue_list& ue_db, sf_sched* tti_sched, size_t prio_idx)
{
  // auto iter = ue_db.begin();
  // std::advance(iter, prio_idx);
  // for (uint32_t ue_count = 0; ue_count < ue_db.size(); ++iter, ++ue_count) {
  //   if (iter == ue_db.end()) {
  //     iter = ue_db.begin(); // wrap around
  //   }
  //   sched_ue&           user = *iter->second;
  //   const dl_harq_proc* h    = get_dl_retx_harq(user, tti_sched);
  //   // Check if there is a pending retx
  //   if (h == nullptr) {
  //     continue;
  //   }
  //   try_dl_retx_alloc(*tti_sched, user, *h);
  // }

  if (cur_slice_rntis.empty()) {
    // sf is reserved for general traffic or UEs haven't been associated with this slice.
    // Schedule rntis with no slice to allow for srb0 (msg4) and other traffic,
    // then schedule rntis from slices.

    if (!no_slice_rntis.empty()) {
    //  uint32_t priority_idx = tti_alloc->get_tti_tx_dl() % (uint32_t)no_slice_rntis.size();
      uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)no_slice_rntis.size();
      auto it = no_slice_rntis.begin();
      std::advance(it, priority_idx);

      for (uint32_t ue_count = 0; ue_count < no_slice_rntis.size(); ++it, ++ue_count) {
        if (it == no_slice_rntis.end()) {
          it = no_slice_rntis.begin();
        }
      //  sched_ue* user = &ue_db[*it];
      //  srslte::console("scheduling other UE crnti 0x%x\n", user->get_rnti());
      //  allocate_user(user);
      sched_ue&           user = *it->second;
      const dl_harq_proc* h    = get_dl_retx_harq(user, tti_sched);
      // Check if there is a pending retx
      if (h == nullptr) {
        continue;
      }
      try_dl_retx_alloc(*tti_sched, user, *h);
      }
    }

    if (workshare && !other_slice_rntis.empty()) {
      //uint32_t priority_idx = tti_alloc->get_tti_tx_dl() % (uint32_t)other_slice_rntis.size();
      uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)other_slice_rntis.size();
      auto it = other_slice_rntis.begin();
      std::advance(it, priority_idx);

      for (uint32_t ue_count = 0; ue_count < other_slice_rntis.size(); ++it, ++ue_count) {
        if (it == other_slice_rntis.end()) {
          it = other_slice_rntis.begin();
        }
        //sched_ue* user = &ue_db[*it];
        //allocate_user(user);
      
      sched_ue&           user = *it->second;
      const dl_harq_proc* h    = get_dl_retx_harq(user, tti_sched);
      // Check if there is a pending retx
      if (h == nullptr) {
        continue;
      }
      try_dl_retx_alloc(*tti_sched, user, *h);
      }
    }
  } else {
    // Schedule current slice first. Give priority in a time-domain RR basis.
    uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)cur_slice_rntis.size();
    auto it = cur_slice_rntis.begin();
    std::advance(it, priority_idx);
    // srslte::console("cur slice num UEs %u\n", cur_slice_rntis.size());
    for (uint32_t ue_count = 0; ue_count < cur_slice_rntis.size(); ++it, ++ue_count) {
      if (it == cur_slice_rntis.end()) {
        it = cur_slice_rntis.begin();
      }
      //sched_ue* user = &ue_db[*it];
      // srslte::console("scheduling current slice crnti 0x%x\n", user->get_rnti());
      //allocate_user(user);
      sched_ue&           user = *it->second;
      const dl_harq_proc* h    = get_dl_retx_harq(user, tti_sched);
      // Check if there is a pending retx
      if (h == nullptr) {
        continue;
      }
      try_dl_retx_alloc(*tti_sched, user, *h);
    }

    if (workshare) {
      // Schedule other slices, then UEs without a slice.
      if (!other_slice_rntis.empty()) {
        // srslte::console("other slice num UEs %u\n", other_slice_rntis.size());
        uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)other_slice_rntis.size();
        auto it = other_slice_rntis.begin();
        std::advance(it, priority_idx);

        for (uint32_t ue_count = 0; ue_count < other_slice_rntis.size(); ++it, ++ue_count) {
          if (it == other_slice_rntis.end()) {
            it = other_slice_rntis.begin();
          }
          //sched_ue* user = &ue_db[*it];
          //allocate_user(user);
      sched_ue&           user = *it->second;
      const dl_harq_proc* h    = get_dl_retx_harq(user, tti_sched);
      // Check if there is a pending retx
      if (h == nullptr) {
        continue;
      }
      try_dl_retx_alloc(*tti_sched, user, *h);
        }
      }

      if (!no_slice_rntis.empty()) {
        uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)no_slice_rntis.size();
        auto it = no_slice_rntis.begin();
        std::advance(it, priority_idx);

        for (uint32_t ue_count = 0; ue_count < no_slice_rntis.size(); ++it, ++ue_count) {
          if (it == no_slice_rntis.end()) {
            it = no_slice_rntis.begin();
          }
          //sched_ue* user = &ue_db[*it];
          //allocate_user(user);
        sched_ue&           user = *it->second;
        const dl_harq_proc* h    = get_dl_retx_harq(user, tti_sched);
        // Check if there is a pending retx
        if (h == nullptr) {
          continue;
        }
        try_dl_retx_alloc(*tti_sched, user, *h);
        }
      }
    }
  }
}

void sched_time_rr_sliced::sched_dl_newtxs(sched_ue_list& ue_db, sf_sched* tti_sched, size_t prio_idx)
{
  // auto iter = ue_db.begin();
  // std::advance(iter, prio_idx);
  // for (uint32_t ue_count = 0; ue_count < ue_db.size(); ++iter, ++ue_count) {
  //   if (iter == ue_db.end()) {
  //     iter = ue_db.begin(); // wrap around
  //   }
  //   sched_ue& user = *iter->second;
  //   if (user.enb_to_ue_cc_idx(cc_cfg->enb_cc_idx) < 0) {
  //     continue;
  //   }
  //   const dl_harq_proc* h = get_dl_newtx_harq(user, tti_sched);
  //   // Check if there is an empty harq for the newtx
  //   if (h == nullptr) {
  //     continue;
  //   }
  //   if (try_dl_newtx_alloc_greedy(*tti_sched, user, *h) == alloc_result::no_cch_space) {
  //     logger.info("SCHED: Couldn't find space in PDCCH/PUCCH for DL tx for rnti=0x%x", user.get_rnti());
  //   }
  // }
  
  if (cur_slice_rntis.empty()) {
    // sf is reserved for general traffic or UEs haven't been associated with this slice.
    // Schedule rntis with no slice to allow for srb0 (msg4) and other traffic,
    // then schedule rntis from slices.

    if (!no_slice_rntis.empty()) {
    //  uint32_t priority_idx = tti_alloc->get_tti_tx_dl() % (uint32_t)no_slice_rntis.size();
      uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)no_slice_rntis.size();
      auto it = no_slice_rntis.begin();
      std::advance(it, priority_idx);

      for (uint32_t ue_count = 0; ue_count < no_slice_rntis.size(); ++it, ++ue_count) {
        if (it == no_slice_rntis.end()) {
          it = no_slice_rntis.begin();
        }
      //  sched_ue* user = &ue_db[*it];
      //  srslte::console("scheduling other UE crnti 0x%x\n", user->get_rnti());
      //  allocate_user(user);
      sched_ue& user = *it->second;
      if (user.enb_to_ue_cc_idx(cc_cfg->enb_cc_idx) < 0) {
        continue;
        }
      const dl_harq_proc* h = get_dl_newtx_harq(user, tti_sched);
      // Check if there is an empty harq for the newtx
      if (h == nullptr) {
        continue;
        }
      if (try_dl_newtx_alloc_greedy(*tti_sched, user, *h) == alloc_result::no_cch_space) {
        logger.info("SCHED: Couldn't find space in PDCCH/PUCCH for DL tx for rnti=0x%x", user.get_rnti());
        }
      }
    }

    if (workshare && !other_slice_rntis.empty()) {
      //uint32_t priority_idx = tti_alloc->get_tti_tx_dl() % (uint32_t)other_slice_rntis.size();
      uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)other_slice_rntis.size();
      auto it = other_slice_rntis.begin();
      std::advance(it, priority_idx);

      for (uint32_t ue_count = 0; ue_count < other_slice_rntis.size(); ++it, ++ue_count) {
        if (it == other_slice_rntis.end()) {
          it = other_slice_rntis.begin();
        }
        //sched_ue* user = &ue_db[*it];
        //allocate_user(user);
      
      sched_ue& user = *it->second;
      if (user.enb_to_ue_cc_idx(cc_cfg->enb_cc_idx) < 0) {
        continue;
        }
      const dl_harq_proc* h = get_dl_newtx_harq(user, tti_sched);
      // Check if there is an empty harq for the newtx
      if (h == nullptr) {
        continue;
        }
      if (try_dl_newtx_alloc_greedy(*tti_sched, user, *h) == alloc_result::no_cch_space) {
        logger.info("SCHED: Couldn't find space in PDCCH/PUCCH for DL tx for rnti=0x%x", user.get_rnti());
        }
      }
    }
  } else {
    // Schedule current slice first. Give priority in a time-domain RR basis.
    uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)cur_slice_rntis.size();
    auto it = cur_slice_rntis.begin();
    std::advance(it, priority_idx);
    // srslte::console("cur slice num UEs %u\n", cur_slice_rntis.size());
    for (uint32_t ue_count = 0; ue_count < cur_slice_rntis.size(); ++it, ++ue_count) {
      if (it == cur_slice_rntis.end()) {
        it = cur_slice_rntis.begin();
      }
      //sched_ue* user = &ue_db[*it];
      // srslte::console("scheduling current slice crnti 0x%x\n", user->get_rnti());
      //allocate_user(user);
      sched_ue& user = *it->second;
      if (user.enb_to_ue_cc_idx(cc_cfg->enb_cc_idx) < 0) {
        continue;
        }
      const dl_harq_proc* h = get_dl_newtx_harq(user, tti_sched);
      // Check if there is an empty harq for the newtx
      if (h == nullptr) {
        continue;
        }
      if (try_dl_newtx_alloc_greedy(*tti_sched, user, *h) == alloc_result::no_cch_space) {
        logger.info("SCHED: Couldn't find space in PDCCH/PUCCH for DL tx for rnti=0x%x", user.get_rnti());
        }
    }

    if (workshare) {
      // Schedule other slices, then UEs without a slice.
      if (!other_slice_rntis.empty()) {
        // srslte::console("other slice num UEs %u\n", other_slice_rntis.size());
        uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)other_slice_rntis.size();
        auto it = other_slice_rntis.begin();
        std::advance(it, priority_idx);

        for (uint32_t ue_count = 0; ue_count < other_slice_rntis.size(); ++it, ++ue_count) {
          if (it == other_slice_rntis.end()) {
            it = other_slice_rntis.begin();
          }
          //sched_ue* user = &ue_db[*it];
          //allocate_user(user);
      sched_ue& user = *it->second;
      if (user.enb_to_ue_cc_idx(cc_cfg->enb_cc_idx) < 0) {
        continue;
        }
      const dl_harq_proc* h = get_dl_newtx_harq(user, tti_sched);
      // Check if there is an empty harq for the newtx
      if (h == nullptr) {
        continue;
        }
      if (try_dl_newtx_alloc_greedy(*tti_sched, user, *h) == alloc_result::no_cch_space) {
        logger.info("SCHED: Couldn't find space in PDCCH/PUCCH for DL tx for rnti=0x%x", user.get_rnti());
        }
      }

      if (!no_slice_rntis.empty()) {
        uint32_t priority_idx = tti_sched->get_tti_tx_dl().to_uint() % (uint32_t)no_slice_rntis.size();
        auto it = no_slice_rntis.begin();
        std::advance(it, priority_idx);

        for (uint32_t ue_count = 0; ue_count < no_slice_rntis.size(); ++it, ++ue_count) {
          if (it == no_slice_rntis.end()) {
            it = no_slice_rntis.begin();
          }
          //sched_ue* user = &ue_db[*it];
          //allocate_user(user);
      sched_ue& user = *it->second;
      if (user.enb_to_ue_cc_idx(cc_cfg->enb_cc_idx) < 0) {
        continue;
          }
      const dl_harq_proc* h = get_dl_newtx_harq(user, tti_sched);
      // Check if there is an empty harq for the newtx
      if (h == nullptr) {
        continue;
          }
      if (try_dl_newtx_alloc_greedy(*tti_sched, user, *h) == alloc_result::no_cch_space) {
        logger.info("SCHED: Couldn't find space in PDCCH/PUCCH for DL tx for rnti=0x%x", user.get_rnti());
          }
        }
      }
    }
  }
  



}

/*****************************************************************
 *                         Uplink
 *****************************************************************/

void sched_time_rr_sliced::sched_ul_users(sched_ue_list& ue_db, sf_sched* tti_sched)
{
  if (ue_db.empty()) {
    return;
  }
  // give priority in a time-domain RR basis.
  uint32_t priority_idx = tti_sched->get_tti_tx_ul().to_uint() % (uint32_t)ue_db.size();
  sched_ul_retxs(ue_db, tti_sched, priority_idx);
  sched_ul_newtxs(ue_db, tti_sched, priority_idx);
}

void sched_time_rr_sliced::sched_ul_retxs(sched_ue_list& ue_db, sf_sched* tti_sched, size_t prio_idx)
{
  auto iter = ue_db.begin();
  std::advance(iter, prio_idx);
  for (uint32_t ue_count = 0; ue_count < ue_db.size(); ++iter, ++ue_count) {
    if (iter == ue_db.end()) {
      iter = ue_db.begin(); // wrap around
    }
    sched_ue&           user = *iter->second;
    const ul_harq_proc* h    = get_ul_retx_harq(user, tti_sched);
    // Check if there is a pending retx
    if (h == nullptr) {
      continue;
    }
    alloc_result code = try_ul_retx_alloc(*tti_sched, user, *h);
    if (code == alloc_result::no_cch_space) {
      logger.debug("SCHED: Couldn't find space in PDCCH for UL retx of rnti=0x%x", user.get_rnti());
    }
  }
}

void sched_time_rr_sliced::sched_ul_newtxs(sched_ue_list& ue_db, sf_sched* tti_sched, size_t prio_idx)
{
  auto iter = ue_db.begin();
  std::advance(iter, prio_idx);
  for (uint32_t ue_count = 0; ue_count < ue_db.size(); ++iter, ++ue_count) {
    if (iter == ue_db.end()) {
      iter = ue_db.begin(); // wrap around
    }
    sched_ue&           user = *iter->second;
    const ul_harq_proc* h    = get_ul_newtx_harq(user, tti_sched);
    // Check if there is a empty harq
    if (h == nullptr) {
      continue;
    }
    uint32_t pending_data = user.get_pending_ul_new_data(tti_sched->get_tti_tx_ul(), cc_cfg->enb_cc_idx);
    // Check if there is a empty harq, and data to transmit
    if (pending_data == 0) {
      continue;
    }
    uint32_t     pending_rb = user.get_required_prb_ul(cc_cfg->enb_cc_idx, pending_data);
    prb_interval alloc      = find_contiguous_ul_prbs(pending_rb, tti_sched->get_ul_mask());
    if (alloc.empty()) {
      continue;
    }
    alloc_result ret = tti_sched->alloc_ul_user(&user, alloc);
    if (ret == alloc_result::no_cch_space) {
      logger.info(
          "SCHED: rnti=0x%x, cc=%d, Couldn't find space in PDCCH for UL tx", user.get_rnti(), cc_cfg->enb_cc_idx);
    }
  }
}

} // namespace srsenb
