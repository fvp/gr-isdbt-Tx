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

#ifndef INCLUDED_ISDBT_BYTE_INTERLEAVER_IMPL_H
#define INCLUDED_ISDBT_BYTE_INTERLEAVER_IMPL_H

#include <isdbt/byte_interleaver.h>

namespace gr {
  namespace isdbt {

    class byte_interleaver_impl : public byte_interleaver
    {
     private:
      int d_mode, d_mod_scheme, d_delay_tsp;
      int TSP_size = 204;
      std::vector< std::deque<unsigned char> *> delay_vector;

     public:
      byte_interleaver_impl(int mode, int conv_code, int mod_scheme, int segments);
      ~byte_interleaver_impl();

      float d_conv_code;

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace isdbt
} // namespace gr

#endif /* INCLUDED_ISDBT_BYTE_INTERLEAVER_IMPL_H */

