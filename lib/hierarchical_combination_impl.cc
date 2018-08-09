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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "hierarchical_combination_impl.h"

namespace gr {
  namespace isdbt {

    static const int d_data_carriers_mode1 = 96;
    static const int d_data_carriers_mode2 = 192;
    static const int d_data_carriers_mode3 = 384;

    const int d_segments_total = 13;

    static int d_active_carriers;
    int d_segments_a;
    int d_segments_b;
    int d_segments_c;

    hierarchical_combination::sptr
    hierarchical_combination::make(int mode, int segments_a, int segments_b, int segments_c)
    {
      d_segments_a = segments_a;
      d_segments_b = segments_b;
      d_segments_c = segments_c;
      switch (mode)
      {
        case 1:
        {
          d_active_carriers = d_data_carriers_mode1;
          break;          
        }
        case 2:
        {
          d_active_carriers = d_data_carriers_mode2;
          break;          
        }
        case 3:
        {
          d_active_carriers = d_data_carriers_mode3;
          break;          
        }
        default:
        {
          printf("H. Comb) Error: Incorrect mode\n");
          d_active_carriers = 0;
          break;          
        }
      }
      return gnuradio::get_initial_sptr
        (new hierarchical_combination_impl());
    }

    /*
     * The private constructor
     */
    hierarchical_combination_impl::hierarchical_combination_impl()
      : gr::sync_block("hierarchical_combination",
              gr::io_signature::make3(3, 3, d_segments_a*d_active_carriers*sizeof(gr_complex),
                                            d_segments_b*d_active_carriers*sizeof(gr_complex),
                                            d_segments_c*d_active_carriers*sizeof(gr_complex)),
              gr::io_signature::make(1, 1, d_segments_total*d_active_carriers*sizeof(gr_complex))
                  )
    {}

    /*
     * Our virtual destructor.
     */
    hierarchical_combination_impl::~hierarchical_combination_impl()
    {
    }

    int
    hierarchical_combination_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in_a = (const gr_complex *) input_items[0];
      const gr_complex *in_b = (const gr_complex *) input_items[1];
      const gr_complex *in_c = (const gr_complex *) input_items[2];
      gr_complex *out = (gr_complex *) output_items[0];

      for (int output = 0; output<noutput_items; output++)
      {
        //Layer A segments to output
        memcpy(out + output*d_segments_total, in_a + output*d_segments_total, d_active_carriers*d_segments_a);
        //Layer B segments to output
        memcpy(out + d_active_carriers*d_segments_a + output*d_segments_total, in_b + output*d_segments_total, d_active_carriers*d_segments_a);
        //Layer C segments to output
        memcpy(out + d_active_carriers*d_segments_a + d_active_carriers*d_segments_b + output*d_segments_total, in_c + output*d_segments_total, 192*d_segments_a);
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

