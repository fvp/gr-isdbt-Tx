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

#ifndef INCLUDED_ISDBT_ENERGY_DISPERSAL_IMPL_H
#define INCLUDED_ISDBT_ENERGY_DISPERSAL_IMPL_H

#include <isdbt/energy_dispersal.h>

namespace gr {
  namespace isdbt {

    class energy_dispersal_impl : public energy_dispersal
    {
      private:
        // Packet size
        static const int d_psize;
        // Number of packets after which PRBS is reset
        static const int d_npacks;
        // Number of blocks
        int d_nblocks = 1;
        // SYNC value
        static const int d_SYNC;
        // Negative SYNC value
        static const int d_NSYNC;

        // Register for PRBS
        int d_reg;

        int symbols_coded;

      void init_prbs();
      int clock_prbs(int clocks);

     public:
      energy_dispersal_impl(int tsp_per_frame);
      ~energy_dispersal_impl();

      int tsp_size = 204;
      int d_tsp_per_frame; //Modo 2, TC 1/8 = 2304
      int symbols_in_tsp = 126;

      int work(int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items);
    };

  } // namespace isdbt
} // namespace gr

#endif /* INCLUDED_ISDBT_ENERGY_DISPERSAL_IMPL_H */

