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

#ifndef INCLUDED_ISDBT_HIERARCHICAL_DIVISOR_IMPL_H
#define INCLUDED_ISDBT_HIERARCHICAL_DIVISOR_IMPL_H

#include <isdbt/hierarchical_divisor.h>

namespace gr {
  namespace isdbt {

    class hierarchical_divisor_impl : public hierarchical_divisor
    {
     private:
      // Nothing to declare in this block.

     public:
      hierarchical_divisor_impl();
      ~hierarchical_divisor_impl();

      int tsp_len = 204;
      int total_tsp_in_multiplex = 2176;
      int layer_a_tsp = 118;
      int layer_b_tsp = 712;
      int layer_c_tsp = 712;
      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace isdbt
} // namespace gr

#endif /* INCLUDED_ISDBT_HIERARCHICAL_DIVISOR_IMPL_H */

