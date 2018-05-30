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
#include "ts_remux_impl.h"

namespace gr {
  namespace isdbt {

    ts_remux::sptr
    ts_remux::make()
    {
      return gnuradio::get_initial_sptr
        (new ts_remux_impl());
    }

    /*
     * The private constructor
     */
    ts_remux_impl::ts_remux_impl()
      : gr::sync_block("ts_remux",
              gr::io_signature::make(1, 1, 188*sizeof(unsigned char)),
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)))
    {}

    /*
     * Our virtual destructor.
     */
    ts_remux_impl::~ts_remux_impl()
    {
    }

    int
    ts_remux_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      
      //printf("Iniciando. noutput_items: %d \n", noutput_items);
      //Dummy TS Remux, adds 16 bytes null after 188 bytes of data
      for (int input=0; input < noutput_items; input++)
      {
        int i=0;
        for (i=0; i<188; i++)
        {
          //Copy first 188 bytes
          out[204*input + i] = in[188*input + i];
        }
        for (int j=0; j<16; j++)
        {
          out[204*input + i + j] = 0x00;
        }
      }
      
      //Imprimo las primeras 3 solo para verificar
      /*if (impresiones < 3)
      {
        for (int k=0; k<204; k++)
        {
          printf("out[%d]=%d \n", k, out[k]);
        }
        impresiones++;
      }*/
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

