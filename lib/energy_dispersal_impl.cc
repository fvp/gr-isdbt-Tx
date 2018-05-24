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
#include "energy_dispersal_impl.h"

namespace gr {
  namespace isdbt {

    const int energy_dispersal_impl::d_npacks = 8;
    const int energy_dispersal_impl::d_psize = 204;
    const int energy_dispersal_impl::d_SYNC = 0x47;
    const int energy_dispersal_impl::d_NSYNC = 0xB8;

    void
    energy_dispersal_impl::init_prbs()
    {
      d_reg = 0xa9;
    }

    int
    energy_dispersal_impl::clock_prbs(int clocks)
    {
      int res = 0;
      int feedback = 0;

      for (int i = 0; i < clocks; i++) {
        feedback = ((d_reg >> (14 - 1)) ^ (d_reg >> (15 - 1))) & 0x1;
        d_reg = ((d_reg << 1) | feedback) & 0x7fff;
        res = (res << 1) | feedback;
      }
      return res;
    }

    energy_dispersal::sptr
    energy_dispersal::make()
    {
      return gnuradio::get_initial_sptr
        (new energy_dispersal_impl());
    }

    /*
     * The private constructor
     */
    energy_dispersal_impl::energy_dispersal_impl()
      : gr::sync_block("energy_dispersal",
          gr::io_signature::make(1, 1, 204*sizeof(unsigned char)),
          gr::io_signature::make(1, 1, 204*sizeof(unsigned char))
      )
    {
      set_relative_rate(1.0);
    }

    /*
     * Our virtual destructor.
     */
    energy_dispersal_impl::~energy_dispersal_impl()
    {
    }

    int
    energy_dispersal_impl::work(int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items)
   {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];

      int index = 0;
      int count = 0;
      int ret = 0;
      int is_sync = 0;

      // Search for SYNC byte
      while (is_sync == 0 && index < 204/*d_psize*/)
      {
        if (in[index] == d_SYNC) {
          is_sync = 1;
        }
        else {
          index++;
        }
      }
      
      // If we found a SYNC byte
      if (is_sync) 
      {
        //printf("Encontre SYNC \n");
        for (int i = 0; i < noutput_items; i++) 
        {
          init_prbs();
          
          int sync = d_NSYNC;

          //Check integrity of TS
          if (in[index + count] != d_SYNC) 
          {
            printf("Malformed MPEG-TS! index: %i, count:%i \n",index, count);
          }
          //Insert sync byte unchanged
          out[count++] = sync;
          //Instert processed bytes
          for (int k = 1; k < 204/*d_psize*/; k++) 
          {
            out[count] = in[index + count] ^ clock_prbs(d_npacks);
            count++;
          }
            
          sync = d_SYNC;
          clock_prbs(d_npacks);
        }
        consume_each(noutput_items);
        ret = noutput_items;
      }
      else 
      {
        printf("No encontre SYNC \n");
        consume_each(index);
        ret = 0;
      }

      // Tell runtime system how many output items we produced.
      return ret;
    }

  } /* namespace isdbt */
} /* namespace gr */

