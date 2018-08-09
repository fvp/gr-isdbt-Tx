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
#include "null_remove_impl.h"

namespace gr {
  namespace isdbt {

    null_remove::sptr
    null_remove::make()
    {
      return gnuradio::get_initial_sptr
        (new null_remove_impl());
    }

    /*
     * The private constructor
     */
    null_remove_impl::null_remove_impl()
      : gr::sync_block("null_remove",
            gr::io_signature::make(1, 1, 204*sizeof(unsigned char)),
            gr::io_signature::make(1, 1, 204*sizeof(unsigned char)))
    {}

    /*
     * Our virtual destructor.
     */
    null_remove_impl::~null_remove_impl()
    {
    }

    int
    null_remove_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      int k = 0;
      int output = 0;

      while(output < noutput_items)
      {
        if (in[203 + output*tsp_size] == 0x47)
        {
          memcpy(out + output*tsp_size, in + (k + output)*tsp_size, tsp_size);
          output++;
          //printf("Sync OK\n");
        }
        else
        {
          k++;
          printf("Sync NOT ok: %i\n", k);
          consume_each(1);
        }
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

