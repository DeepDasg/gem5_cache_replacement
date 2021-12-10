/* Author - Abhishek Nayak
 * For ECE565 Course project, Fall 2021
 */

#include "mem/cache/replacement_policies/dip.hh"
#include <cstdlib>
#include <iostream>

#include "base/intmath.hh"
#include "base/logging.hh"
#include <memory>
#include <bitset>
#include "base/random.hh"
#include "params/DIPRP.hh"

DIPRP::DIPRP(const Params *p)
    :BIPRP(p), 
     PSEL(p->PSEL_width, 1<<(p->PSEL_width-1)),
     K(p->K),
     N(p->size / (p->block_size * p->assoc)),
     num_offset_bits(floorLog2(N/K)),
     num_constituency_bits(floorLog2(K))
{
}

void
DIPRP::reset(const std::shared_ptr<ReplacementData>& replacement_data) const
{
    uint64_t setidx = replacement_data->setindex;
    SET_TYPE _set_type;
    
    uint32_t smaller = (num_offset_bits<num_constituency_bits) ? num_offset_bits : num_constituency_bits;
    uint32_t offset = setidx & ((1<<smaller)-1);
    uint32_t constituency = (setidx>>smaller) & ((1<<smaller)-1);

    if(constituency == offset)
        _set_type = DEDICATE_LRU;
    else if(constituency == (offset ^ ((1<<smaller)-1))) 
        _set_type = DEDICATE_BIP;
    else
        _set_type = FOLLOWER;  

    switch(_set_type) {
        case FOLLOWER: 
            if(PSEL.calcSaturation() > 0.5) 
                BIPRP::reset(replacement_data);
            else 
                LRURP::reset(replacement_data);
            break;
        case DEDICATE_LRU:
            PSEL++;
            LRURP::reset(replacement_data);
            break;
        case DEDICATE_BIP:
            PSEL--;
            BIPRP::reset(replacement_data);
            break;
    }
}

DIPRP*
DIPRPParams::create()
{
    return new DIPRP(this);
}
