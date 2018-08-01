/* -*- c++ -*- */

#define ISDBT_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "isdbt_swig_doc.i"

%{
#include "isdbt/ofdm_sym_acquisition.h"
#include "isdbt/sync_and_channel_estimation.h"
#include "isdbt/symbol_demapper.h"
#include "isdbt/tmcc_decoder.h"
#include "isdbt/subset_of_carriers.h"
#include "isdbt/frequency_deinterleaver.h"
#include "isdbt/time_deinterleaver.h"
#include "isdbt/bit_deinterleaver.h"
#include "isdbt/energy_descrambler.h"
#include "isdbt/byte_deinterleaver.h"
//#include "isdbt/bit_deinterleaver_streamed.h"
#include "isdbt/ofdm_sym_acquisition_dvbt.h"
#include "isdbt/reed_solomon_dec_isdbt.h"
#include "isdbt/frequency_deinterleaver_1seg.h"
#include "isdbt/time_deinterleaver_1seg.h"
#include "isdbt/symbol_demapper_1seg.h"
#include "isdbt/viterbi_decoder.h"
#include "isdbt/ofdm_synchronization.h"
#include "isdbt/ofdm_synchronization_1seg.h"
#include "isdbt/tmcc_decoder_1seg.h"
#include "isdbt/agregar_cp.h"
#include "isdbt/ofdm_frame_structure.h"
#include "isdbt/time_interleaver.h"
#include "isdbt/frec_interleaver.h"
#include "isdbt/mapper.h"
#include "isdbt/ts_remux.h"
#include "isdbt/byte_interleaver.h"
#include "isdbt/energy_dispersal.h"
#include "isdbt/hierarchical_divisor.h"
#include "isdbt/tsp_resize.h"
#include "isdbt/sync_adjust.h"
%}


%include "isdbt/ofdm_sym_acquisition.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, ofdm_sym_acquisition);
%include "isdbt/sync_and_channel_estimation.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, sync_and_channel_estimation);

%include "isdbt/subset_of_carriers.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, subset_of_carriers);
%include "isdbt/frequency_deinterleaver.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, frequency_deinterleaver);
%include "isdbt/time_deinterleaver.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, time_deinterleaver);
%include "isdbt/symbol_demapper.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, symbol_demapper);
%include "isdbt/tmcc_decoder.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, tmcc_decoder);
%include "isdbt/bit_deinterleaver.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, bit_deinterleaver);

%include "isdbt/energy_descrambler.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, energy_descrambler);
%include "isdbt/byte_deinterleaver.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, byte_deinterleaver);

//%include "isdbt/bit_deinterleaver_streamed.h"
//GR_SWIG_BLOCK_MAGIC2(isdbt, bit_deinterleaver_streamed);

%include "isdbt/ofdm_sym_acquisition_dvbt.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, ofdm_sym_acquisition_dvbt);
%include "isdbt/reed_solomon_dec_isdbt.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, reed_solomon_dec_isdbt);
%include "isdbt/frequency_deinterleaver_1seg.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, frequency_deinterleaver_1seg);
%include "isdbt/time_deinterleaver_1seg.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, time_deinterleaver_1seg);
%include "isdbt/symbol_demapper_1seg.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, symbol_demapper_1seg);


%include "isdbt/viterbi_decoder.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, viterbi_decoder);
%include "isdbt/ofdm_synchronization.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, ofdm_synchronization);
%include "isdbt/ofdm_synchronization_1seg.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, ofdm_synchronization_1seg);
%include "isdbt/tmcc_decoder_1seg.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, tmcc_decoder_1seg);
%include "isdbt/agregar_cp.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, agregar_cp);
%include "isdbt/ofdm_frame_structure.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, ofdm_frame_structure);
%include "isdbt/time_interleaver.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, time_interleaver);
%include "isdbt/frec_interleaver.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, frec_interleaver);
%include "isdbt/mapper.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, mapper);
%include "isdbt/ts_remux.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, ts_remux);
%include "isdbt/byte_interleaver.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, byte_interleaver);

%include "isdbt/energy_dispersal.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, energy_dispersal);

%include "isdbt/hierarchical_divisor.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, hierarchical_divisor);
%include "isdbt/tsp_resize.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, tsp_resize);

%include "isdbt/sync_adjust.h"
GR_SWIG_BLOCK_MAGIC2(isdbt, sync_adjust);
