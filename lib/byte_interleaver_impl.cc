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
#include "byte_interleaver_impl.h"

namespace gr {
  namespace isdbt {

    byte_interleaver::sptr
    byte_interleaver::make(int mode, int conv_code, int mod_scheme, int segments)
    {
      return gnuradio::get_initial_sptr
        (new byte_interleaver_impl(mode, conv_code, mod_scheme, segments));
    }

    /*
     * The private constructor
     */
    byte_interleaver_impl::byte_interleaver_impl(int mode, int conv_code, int mod_scheme, int segments)
      : gr::sync_block("byte_interleaver",
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
      int d_tsp = (int)(TSP_size*mod_scheme*d_conv_code*96*factor)/(8*TSP_size);
      d_delay_tsp = d_tsp*segments - 11;
      d_delay_bytes = 204*d_delay_tsp;
      //d_delay_queue = d_delay_bytes / 12;
      printf("Delay (TSPs): %i \n", d_delay_tsp);
      printf("Delay (Bytes): %i \n", d_delay_bytes);
      //printf("Delay (por fila): %i \n", d_delay_queue);
      //Create queues
      for (int i=0; i<12; i++)
      {
        //Create queue
        delay_vector.push_back(new std::deque<unsigned char>(i*17,0)); 
      }
      delay_vector.push_back(new std::deque<unsigned char>(d_delay_bytes,0));
    }

    /*
     * Our virtual destructor.
     */
    byte_interleaver_impl::~byte_interleaver_impl()
    {
      for (unsigned int i=0; i<delay_vector.size();i++)
      {
        delete delay_vector.back();
        delay_vector.pop_back();
      }
    }

    int
    byte_interleaver_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      int index = 0;
      unsigned char temp;
      //printf("noutput_items : %i \n", noutput_items);
      //printf("Delay (TSPs): %i \n", d_delay_tsp);

      for (int output = 0; output<noutput_items; output++)
      {
        for (int i=0; i<204; i++)
        {
          index = i + output*204;
          //push back input
          delay_vector[12]->push_back(in[index]);
          temp = delay_vector[12]->front();
          //Select queue
          //Push byte from input to queue, pop output byte from queue
          delay_vector[index % 12]->push_back(temp);
          out[index] = delay_vector[index % 12]->front();
          //Delete used symbol
          delay_vector[12]->pop_front();
          delay_vector[index % 12]->pop_front(); 
        }
      }

      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

