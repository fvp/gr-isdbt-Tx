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
#include "sync_adjust_impl.h"

namespace gr {
  namespace isdbt {

    sync_adjust::sptr
    sync_adjust::make()
    {
      return gnuradio::get_initial_sptr
        (new sync_adjust_impl());
    }

    /*
     * The private constructor
     */
    sync_adjust_impl::sync_adjust_impl()
      : gr::sync_block("sync_adjust",
            gr::io_signature::make(1, 1, 204*sizeof(unsigned char)),
            gr::io_signature::make(1, 1, 204*sizeof(unsigned char)))
    {}

    /*
     * Our virtual destructor.
     */
    sync_adjust_impl::~sync_adjust_impl()
    {
    }

    int
    sync_adjust_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      
      for (int output = 0; output < noutput_items; output++)
      {
        //Takes last 16 bytes out of TSP
        memcpy(out + output*tsp_size, in + output*tsp_size + 1, tsp_size - 1);
        //Move Sync byte to end
        if (in[output*tsp_size] == 0x47)
        {
          out[203 + output*tsp_size] = 0x47;
        }
        else
        {
          printf("Sync Adjust) Input Error: No Sync byte in in[0]\n");
        }
        
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

