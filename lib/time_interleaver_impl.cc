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
#include "time_interleaver_impl.h"

namespace gr {
  namespace isdbt {

    time_interleaver::sptr
    time_interleaver::make(int mode, int I)
    {
      return gnuradio::get_initial_sptr
        (new time_interleaver_impl(mode, I));
    }

    /*
     * The private constructor
     */
    time_interleaver_impl::time_interleaver_impl(int mode, int I)
      : gr::sync_block("time_interleaver",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*1*96*((int)pow(2.0,mode-1))),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*1*96*((int)pow(2.0,mode-1))))
    {
      //TODO: Hay que crear el vector.
      d_I = I;
      d_mode = mode;

      int mi = 0;
      for (int carrier = 0; carrier<96; carrier++)
      {
        mi = (5*carrier) % 96; 
        d_delay[carrier] = d_I*mi;
      }
      //1. Calcular los retardos mi
      //2. crear el vector
      //3. para cada elemento del vector, crear el tren de retardos
    }

    /*
     * Our virtual destructor.
     */
    time_interleaver_impl::~time_interleaver_impl()
    {
    }

    int
    time_interleaver_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in  = (const gr_complex *)  input_items[0];
            gr_complex *out = (gr_complex *)        output_items[0];

      for (int i = 0; i<96; i++)
      {
        int coso_retardo = d_delay[i];
        printf("coso_retardo[%d] = %d\n", i, coso_retardo);
      }
      // TODO: Hay que rearmar la entrada
      //1. Recorrer el vector, buscando datos validos
      //2. Si encuentro un dato valido, lo pongo a la salida, y todos avanzan uno
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

