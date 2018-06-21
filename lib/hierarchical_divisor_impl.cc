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
#include "hierarchical_divisor_impl.h"

namespace gr {
  namespace isdbt {

    hierarchical_divisor::sptr
    hierarchical_divisor::make()
    {
      //Define frames out
      //switch(tx_mode)
      //{
        //case 1:
        //{
        //  frames_out = 1024*(1+cp_length);
        ///}
        //case 2:
        //{
        ///  frames_out = 2048*(1+cp_length);
        // }
        //case 3:
        // /{
        //  frames_out = 4096*(1+cp_length);
        //}
        //default:
        //{
        //  printf("Error: Incorrect transmission mode\n");
        //}
      //}
      return gnuradio::get_initial_sptr
        (new hierarchical_divisor_impl());
    }

    /*
     * The private constructor
     */
    hierarchical_divisor_impl::hierarchical_divisor_impl()
      : gr::block("hierarchical_divisor",
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)),
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)))
    {}

    /*
     * Our virtual destructor.
     */
    hierarchical_divisor_impl::~hierarchical_divisor_impl()
    {
    }

    void
    hierarchical_divisor_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = 80*noutput_items;
    }

    int
    hierarchical_divisor_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      unsigned char tsp_info_byte;
      int tsp_consumed = 0;
      int i = 0;
     // printf("**********NUEVO WORK*****************\n");
     // printf("tsp_consumed: %i\n", tsp_consumed);
      
      for (int input = 0; input<noutput_items; input++)
      {
        bool found_A = false;
        
        while (!found_A)
        {
          //Read first TSP from input
          tsp_info_byte = in[(tsp_consumed + i)*tsp_len + 188 + 1];

          //Obtain layer information in an int
          tsp_info_byte >>= 4;
          
          //Check if its layer A
          if (tsp_info_byte == 1)
          {
            found_A = true;
          }
          else
          {
            i++;
          }
        }
        //Salgo con i la posicion del TSP de interes
        //Copio todo el TSP para Layer A a la salida
        memcpy(out + tsp_len*input, in + (tsp_consumed + i)*tsp_len, tsp_len);
        tsp_consumed += i;
        i = 1;
      }
      // Tell runtime system how many input items we consumed on
      // each input stream.
      tsp_consumed++;
      consume_each(tsp_consumed);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

