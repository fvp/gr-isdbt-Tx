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
#include "mapper_impl.h"

namespace gr {
  namespace isdbt { 

    int mapper::d_total_segments = 1; 

    mapper::sptr
    mapper::make(int mode, int constType, bool isFullSeg)
    {
      
      if (isFullSeg){
        d_total_segments = 13;
      }else {
        d_total_segments = 1;
      }
      
      return gnuradio::get_initial_sptr
        (new mapper_impl(mode, constType, isFullSeg));
    }

    /*
     * The private constructor
     */
    mapper_impl::mapper_impl(int mode, int constType, bool isFullSeg)
      : gr::block("mapper",
              gr::io_signature::make(1, 1, sizeof(bitset<2>)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))
                  )
    {
      d_mode = mode; 
      d_carriers_per_segment = d_data_carriers_mode1*((int)pow(2.0,mode-1)); 
      d_noutput = d_total_segments*d_carriers_per_segment;
      d_IsFullSeg = isFullSeg;
      d_constType = constType;
    }

    /*
     * Our virtual destructor.
     */
    mapper_impl::~mapper_impl()
    {
    }

    void
    mapper_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = 96*2*noutput_items*d_constType;
    }

    int
    mapper_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      for (int i=0; i<noutput_items; i++)
      {
        //first, we push n bits into queue

        //then we take out n bits from queue, and generate symbol.
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

