/* -*- c++ -*- */
/*  
 * Copyright 2015, 2016, 2017
 *   Federico "Larroca" La Rocca <flarroca@fing.edu.uy>
 *   Pablo Belzarena 
 *   Gabriel Gomez Sena 
 *   Pablo Flores Guridi 
 *   Victor Gonzalez Barbone
 * 
 *   Instituto de Ingenieria Electrica, Facultad de Ingenieria,
 *   Universidad de la Republica, Uruguay.
 *  
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *  
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 * 
 */

#ifndef INCLUDED_ISDBT_OFDM_FRAME_STRUCTURE_IMPL_H
#define INCLUDED_ISDBT_OFDM_FRAME_STRUCTURE_IMPL_H

#include <isdbt/ofdm_frame_structure.h>
#include <bitset>

using namespace std;

namespace gr {
  namespace isdbt {

    class ofdm_frame_structure_impl : public ofdm_frame_structure
    {
     private:
      /*!
      * \brief Given an incoming complex symbol, it outputs the corresponding bit-word assuming a 
      * 16QAM modulation. 
      *
      * Taking advantage of the Grey's coding used in the standard, this method is relatively fast. 
      */
      int sp_segment_keywords[13];

      enum carrier_mod_scheme
      {
        DQPSK = 0,
        QPSK,
        QAM_16,
        QAM_64,
        UNUSED
      };

      carrier_mod_scheme d_ModSchemeA;
      carrier_mod_scheme d_ModSchemeB;
      carrier_mod_scheme d_ModSchemeC; 

      enum convolutional_coding_rate
      {
        c_1_2 = 0,
        c_2_3,
        c_3_4,
        c_5_6,
        c_7_8,
        RESERVED,
        UNUSED_HL
      };

      convolutional_coding_rate d_ConvCodeA;
      convolutional_coding_rate d_ConvCodeB;
      convolutional_coding_rate d_ConvCodeC; 

      bitset<11> sp_keyword;
      bitset<16> TMCC_sync_word;
      bitset<204> TMCCword;
      bitset<204> TMCCword_SP0_w0;
      bitset<204> TMCCword_SP0_w1;
      bitset<204> TMCCword_SP1_w0;
      bitset<204> TMCCword_SP1_w1;

      bool d_IsFullSeg;
      int InputIndex;     //Variable to cover input array
      int d_mode, d_carrier_pos, TMCCindex, SPindex, frame_counter;

      int d_IntLengthA, d_IntLengthB, d_IntLengthC, d_LayerA_seg, d_LayerB_seg, d_LayerC_seg, d_zero_pad_left;
      
      void parity_TMCC(bitset<204> *TMCCword, int n, int k);
      int get_degree(bitset<204> r, int max);

      gr_complex write_TMCC(int frame_counter, int SegmentNumber);

      gr_complex write_SP(int SPindex, int d_mode, int SegmentNumber); 

      void fill_segment_mode1(gr_complex* in, gr_complex* out, int SegmentNumber, int output_item);
      void fill_segment_mode2(gr_complex* in, gr_complex* out, int SegmentNumber, int output_item);
      void fill_segment_mode3(gr_complex* in, gr_complex* out, int SegmentNumber, int output_item);

     public:
      ofdm_frame_structure_impl(int mode, bool IsFullSeg, 
                                int ModSchemeA, int ModSchemeB, int ModSchemeC, 
                                int ConvCodeA, int ConvCodeB, int ConvCodeC,
                                int IntLengthA, int IntLengthB, int IntLengthC,
                                int LayerA_seg, int LayerB_seg, int LayerC_seg);

      ~ofdm_frame_structure_impl();
      
      int active_carriers_mod_1 = 96;
      int active_carriers_mod_2 = 192;
      int active_carriers_mod_3 = 384;

      int total_carriers_mod_1 = 108;
      int total_carriers_mod_2 = 216;
      int total_carriers_mod_3 = 432;

      int zero_pad_left[3] = {322, 644, 1288};

      int contador = 0;

      gr_complex tmcc_out_1 = gr_complex(-4.0/3.0, 0);
      gr_complex tmcc_out_0 = gr_complex(4.0/3.0, 0);
      gr_complex TMCC_complex_SP0_w0[204] = {};
      gr_complex TMCC_complex_SP0_w1[204] = {};
      gr_complex TMCC_complex_SP1_w0[204] = {};
      gr_complex TMCC_complex_SP1_w1[204] = {};
      gr_complex TMCC_sym;

      gr_complex SP_values[216*13] = {};

      bool primera = true;
      bool EsPar = true;
      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace isdbt
} // namespace gr

#endif /* INCLUDED_ISDBT_OFDM_FRAME_STRUCTURE_IMPL_H */

