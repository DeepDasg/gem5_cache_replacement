/**
 * Copyright (c) 2018 Inria
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __MEM_CACHE_REPLACEMENT_POLICIES_DIP_RP_HH__
#define __MEM_CACHE_REPLACEMENT_POLICIES_DIP_RP_HH__

#include "base/sat_counter.hh"
#include "mem/cache/replacement_policies/bip_rp.hh"

struct DIPRPParams;
/*Dedicated set selection*/


class DIPRP : public BIPRP
{
  protected:
    /*10 bit saturating counter*/
    mutable SatCounter PSEL;

    /* number of sets dedicated to each policy */
    uint32_t K;

    /*number of sets obtained from params */
    uint32_t N;

    /*number of offset bits */
    uint32_t num_offset_bits;

    /*number of constituency bits */
    uint32_t num_constituency_bits;

  public:
    /** Convenience typedef. */
    typedef DIPRPParams Params;
    
    enum SET_TYPE {FOLLOWER = 0, DEDICATE_LRU = 1, DEDICATE_BIP = 2};

    /**
     * Construct and initiliaze this replacement policy.
     */
    DIPRP(const Params *p);

    /**
     * Destructor.
     */
    ~DIPRP() {}
    
    
    /**
     * Reset replacement data for an entry. Used when an entry is inserted.
     * Uses the bimodal throtle parameter to decide whether the new entry
     * should be inserted as MRU, or LRU.
     *
     * @param replacement_data Replacement data to be reset.
     */
    void reset(const std::shared_ptr<ReplacementData>& replacement_data) const
                                                                     override;
};

#endif // __MEM_CACHE_REPLACEMENT_POLICIES_DIP_RP_HH__
