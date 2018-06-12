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
#include "delay_adjustment_impl.h"

namespace gr {
  namespace isdbt {

    delay_adjustment::sptr
    delay_adjustment::make(int mode, int conv_code, int mod_scheme, int segments)
    {
      return gnuradio::get_initial_sptr
        (new delay_adjustment_impl(mode, conv_code, mod_scheme, segments));
    }

    /*
     * The private constructor
     */
    delay_adjustment_impl::delay_adjustment_impl(int mode, int conv_code, int mod_scheme, int segments)
      : gr::sync_block("delay_adjustment",
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)),
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)))
    {
      switch (conv_code)
      {
        case 0:
        {
          d_conv_code = (float) 1/2;
          break;
        }
        case 1:
        {
          d_conv_code = (float) 2/3;
          break;
        }
        case 2:
        {
          d_conv_code = (float) 3/4;
          break;
        }
        case 3:
        {
          d_conv_code = (float) 5/6;
          break;
        }
        case 4:
        {
          d_conv_code = (float) 7/8;
          break;
        }
        default:
        {
          d_conv_code = 0;
          printf("Error: Incorrect Convolution Code Rate\n");
          break;
        }
      }
      //Find totay delay given the modulation parameters
      int factor = (int) (pow(2.0,mode-1));
      int d_tsp = (int)(204*mod_scheme*d_conv_code*96*factor)/(8*204);
      d_delay_tsp = d_tsp*segments - 11;
      printf("Delay (TSPs): %i \n", d_delay_tsp);
      //Construct a delay queue
      for (int i=0; i<204; i++)
      {
          delay_vector.push_back(new std::deque<unsigned char>(d_delay_tsp,0)); 
      }
    }

    /*
     * Our virtual destructor.
     */
    delay_adjustment_impl::~delay_adjustment_impl()
    {
      for (unsigned int i=0; i<delay_vector.size();i++)
      {
        delete delay_vector.back();
        delay_vector.pop_back();
      }
    }

    int
    delay_adjustment_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];


      for (int input = 0; input<noutput_items; input++)
      {
        for (int i=0; i<204; i++)
        {
          //Push byte from input to queue, pop output byte from queue
          delay_vector[i]->push_back(in[input*204 + i]);
          out[input*204 + i] = delay_vector[i]->front();
          //Delete used symbol
          delay_vector[i]->pop_front(); 
        }
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

