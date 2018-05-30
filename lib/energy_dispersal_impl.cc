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
      m = 0;
      init_prbs();
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

      // Aca empieza el codigo
      

      for (int i = 0; i < noutput_items; i++)
      {

        
        //out[i*204] = in[i*204]; // Deshabilita XOR durante byte de sincronismo (asumimos que copia el 0x47, es así?)
        //a = clock_prbs(8); // Consume 1 clock_prbs, el a es un bolaso. 
        
        clock_prbs(8);
        for (int j = 0; j < 204; j++)
          // OJO: el primer byte (que corresponde al 0x47) debe ser copiado tal cual sin pasarlo por el XOR
        {
         
          out[i*204 + j] = in[i*204 + j] ^ clock_prbs(8); 
          m = (m+1)% 1280;

          // Chequeamos si se ha producido un cuadro multiplex para reiniciar la palabra
          // (Para Modo 1, con CP = 1/4 entran M=1280 TSP por cuadro)
          if (m == 0)
          {
            init_prbs();
          }
        }

      }

      // Aca terminael codigo
      

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

