/* Author - Deepayan Dasgupta
 * For ECE565 Course project - Fall 2021
 */


#include "mem/cache/replacement_policies/drrip_rp.hh"
#include <cstdlib>
#include <iostream>

#include "base/intmath.hh"
#include "base/logging.hh"
#include <memory>
#include <bitset>
#include "base/random.hh"
#include "params/DRRIPRP.hh"
#include "debug/PSEL_value.hh"

DRRIPRP::DRRIPRP(const Params *p)
    :BRRIPRP(p), 
     PSEL(p->PSEL_width, 1<<(p->PSEL_width-1)),
     K(p->K),
     N(p->size / (p->block_size * p->assoc)),
     num_offset_bits(floorLog2(N/K)),
     num_constituency_bits(floorLog2(K))
{
}

void
DRRIPRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    uint64_t setidx = replacement_data->setindex;
    SET_TYPE _set_type;
    
    uint32_t smaller = (num_offset_bits<num_constituency_bits) ? num_offset_bits : num_constituency_bits;
    uint32_t offset = setidx & ((1<<smaller)-1);
    uint32_t constituency = (setidx>>smaller) & ((1<<smaller)-1);

    if(constituency == offset)
        _set_type = DEDICATE_RRIP;
    else if(constituency == (offset ^ ((1<<smaller)-1))) 
        _set_type = DEDICATE_BRRIP;
    else
        _set_type = FOLLOWER;  

    std::shared_ptr<BRRIPReplData> casted_replacement_data =
        std::static_pointer_cast<BRRIPReplData>(replacement_data);    

    switch(_set_type) {
        case FOLLOWER: 
            DPRINTF(PSEL_value, "FOLLOWER: PSEL = %d\n", PSEL);
            if(PSEL.calcSaturation() > 0.5) 
                BRRIPRP::reset(replacement_data);
            else 
               {
                casted_replacement_data->rrpv.saturate();

                casted_replacement_data->rrpv--;

                // Mark entry as ready to be used
                casted_replacement_data->valid = true;
               }
            
            break;

        case DEDICATE_RRIP: //choose SRRIP
            PSEL++;
            
            DPRINTF(PSEL_value, "RRIP: PSEL = %d\n", PSEL);
            casted_replacement_data->rrpv.saturate();

            casted_replacement_data->rrpv--;
            // Mark entry as ready to be used
            casted_replacement_data->valid = true;
            
            break;
        case DEDICATE_BRRIP: //choose BRRIP
            PSEL--;
            DPRINTF(PSEL_value, "BRRIP: PSEL = %d\n", PSEL);
            BRRIPRP::reset(replacement_data);
            break;

    }
}

DRRIPRP*
DRRIPRPParams::create()
{
    return new DRRIPRP(this);
}
