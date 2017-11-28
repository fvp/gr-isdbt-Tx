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
#include "ofdm_frame_structure_impl.h"

namespace gr {
  namespace isdbt {

    ofdm_frame_structure::sptr
    ofdm_frame_structure::make(int mode)
    {
      return gnuradio::get_initial_sptr
        (new ofdm_frame_structure_impl(mode));
    }

    /*
     * The private constructor
     */
    ofdm_frame_structure_impl::ofdm_frame_structure_impl(int mode)
      : gr::block("ofdm_frame_structure",
              gr::io_signature::make(1, 1, sizeof(gr_complex) * 13 * 96 * ((int)pow(2.0,mode-1))),
              gr::io_signature::make(1, 1, sizeof(gr_complex) * ((int)pow(2.0,10 + mode)))
              )
    {
      d_frame_counter = 0;
      d_symbol_number = 0;
      d_carrier_pos = 0;
      d_mode = mode;
    }

    /*
     * Our virtual destructor.
     */
    ofdm_frame_structure_impl::~ofdm_frame_structure_impl()
    {
    }

    void
    ofdm_frame_structure_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    ofdm_frame_structure_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
    const gr_complex *in = (const gr_complex *) input_items[0];
    gr_complex *out = (gr_complex *) output_items[0];
    int k = 0;
    for (int i = 0; i < noutput_items ; i++) 
    {
      switch (d_mode)
        {
        case 1:
          printf("Mode 1 \n");
          for (int j = 0; j < 96; j++)
          /* Segment 0*/ 
          {
            if ((j % 12) == (3*d_carrier_pos))
            {
              /*Entonces es una portadora*/
              out[108*6+j] = std::complex<double>(10, 10);
            }/* else if (TMCC) {

            } else if (AC0) {
              out[108*6+j] = std::complex<double>(0, 0);
            }*/ else {
              /* Fill with data*/
              out[108*6+j] = in[k];
              k++;
            }
          }
          break;
        case 2:
          printf("Mode 2 \n");
          for (int j = 0; j < 4096; j++)
          {
          out[j] = std::complex<double>(100, 200);
          }
          break;
        case 3:
          printf("Mode 3 \n");
          for (int j = 0; j < 8192; j++)
          {
          out[j] = std::complex<double>(100, 200);
          }
          break;
        default:
          printf("Error: incorrect mode \n");
          break; 
        } 
      d_frame_counter++;  
      d_carrier_pos = (d_frame_counter % 4);
      d_frame_counter = (d_frame_counter % 204);
      }
      //printf("Sequence number: %d \n", d_symbol);
      this->consume(0, noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

