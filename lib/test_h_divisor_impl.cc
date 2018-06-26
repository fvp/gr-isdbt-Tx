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
#include "test_h_divisor_impl.h"

namespace gr {
  namespace isdbt {

    test_h_divisor::sptr
    test_h_divisor::make()
    {
      return gnuradio::get_initial_sptr
        (new test_h_divisor_impl());
    }

    /*
     * The private constructor
     */
    test_h_divisor_impl::test_h_divisor_impl()
      : gr::sync_block("test_h_divisor",
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)),
              gr::io_signature::make2(2, 2, 204*sizeof(unsigned char),204*sizeof(unsigned char)))
    {}

    /*
     * Our virtual destructor.
     */
    test_h_divisor_impl::~test_h_divisor_impl()
    {
    }

    int
    test_h_divisor_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out0 = (unsigned char *) output_items[0];
      unsigned char *out1 = (unsigned char *) output_items[1];

      unsigned char tsp_info_byte;
      int tsp_len = 204;
      int tsp_0 = 0;
      int tsp_1 = 0;


      //printf("**********NUEVO WORK*****************\n");
      //printf("noutput_items: %i\n", noutput_items);

      for (int input = 0; input<noutput_items; input++)
      {
        //Read first TSP from input
        tsp_info_byte = in[input*tsp_len + 188 + 1];

        //Obtain layer information in an int
        tsp_info_byte >>= 4;
          
        //Check if its layer A
        if (tsp_info_byte == 1)
        {
          //memcpy(out0 + tsp_len*tsp_0, in + input*tsp_len, tsp_len);
          memcpy(out0 + tsp_len*tsp_0, in, tsp_len);
          tsp_0++;
        }
        else
        {
          memcpy(out1 + tsp_len*tsp_1, in + input*tsp_len, tsp_len);
          memcpy(out0 + tsp_len*tsp_0, in, tsp_len);
          tsp_1++;
          tsp_0++;
        }
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

