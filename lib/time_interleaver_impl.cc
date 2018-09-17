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


    const int time_interleaver_impl::d_data_carriers_mode1 = 96;
    int time_interleaver::d_total_segments = 1; 
  

    time_interleaver::sptr
    time_interleaver::make(int mode, int I, bool IsFullSeg)
    {
      //Define number of segments in transmission
      if (IsFullSeg){
        d_total_segments = 13;
      }else {
        d_total_segments = 1;
      }
      return gnuradio::get_initial_sptr
        (new time_interleaver_impl(mode, I, IsFullSeg));
    }

    /*
     * The private constructor
     */
    time_interleaver_impl::time_interleaver_impl(int mode, int I, bool IsFullSeg)
      : gr::sync_block("time_interleaver",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*d_total_segments*d_data_carriers_mode1*((int)pow(2.0,mode-1))),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*d_total_segments*d_data_carriers_mode1*((int)pow(2.0,mode-1))))
      {
      d_mode = mode; 
      //TODO the length of the interleaver may change from segment to segment. This should be corrected...
      d_I = I; 
      d_carriers_per_segment = d_data_carriers_mode1*((int)pow(2.0,mode-1)); 
      d_noutput = d_total_segments*d_carriers_per_segment;
      
      int sync_delay = 204 - ((95*d_I)%204);

      //printf("Delay calculado: %d \n", sync_delay);

      int mi = 0;

      for (int segment=0; segment<d_total_segments; segment++)
      {
        for (int carrier = 0; carrier<d_carriers_per_segment; carrier++)
        {
          //Add delay in order to make total time multiplex of OFDM period
          //delay_vector.push_back(new std::deque<gr_complex>(sync_delay,0)); 
          //Add delay given by interleaver
          mi = (5*carrier) % d_data_carriers_mode1; 
          delay_vector.push_back(new std::deque<gr_complex>(d_I*mi+sync_delay,0)); 
        }
      }

    }

    /*
     * Our virtual destructor.
     */
    time_interleaver_impl::~time_interleaver_impl()
    {
      for (unsigned int i=0; i<delay_vector.size();i++){
        delete delay_vector.back();
        delay_vector.pop_back();
      }
    }

    int
    time_interleaver_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // TODO CHECK the tag propagatio policy for the frame 
      // beginnning. 
      for (int i=0; i<noutput_items; i++)
      {
        for (int carrier=0; carrier<d_noutput; carrier++)
        {
          // a simple FIFO queue performs the interleaving. 
          // The "difficult" part is setting the correct sizes 
          // for each queue. 
          delay_vector[carrier]->push_back(in[i*d_noutput + carrier]);
          out[i*d_noutput + carrier] = delay_vector[carrier]->front();
          delay_vector[carrier]->pop_front(); 
        }
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

