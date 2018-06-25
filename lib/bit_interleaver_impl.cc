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
#include "bit_interleaver_impl.h"

namespace gr {
  namespace isdbt {

    const int bit_interleaver_impl::
      d_delay_qpsk[2] = {1, 120}; 
    const int bit_interleaver_impl::
      d_delay_16qam[4] = {1, 40, 80, 120};
    const int bit_interleaver_impl::
      d_delay_64qam[6] = {1, 24, 48, 72, 96, 120}; 


    bit_interleaver::sptr
    bit_interleaver::make(int mode, int segments, int constellation_size)
    {
      return gnuradio::get_initial_sptr
        (new bit_interleaver_impl(mode, segments, constellation_size));
    }

    /*
     * The private constructor
     */
    bit_interleaver_impl::bit_interleaver_impl(int mode, int segments, int constellation_size)
      : gr::block("bit_interleaver",
              gr::io_signature::make(1, 1, sizeof(unsigned char)*segments*1),
              gr::io_signature::make(1, 1, sizeof(unsigned char)*segments*1))
    {
      d_mode = mode;
      d_const_size = constellation_size;
      d_nsegments = segments;
      queue_index = 0;

      // Set the delay correction
      switch (d_const_size)
      {
         case 4:  
            d_delay = d_delay_qpsk;
            d_dcorrection = (d_mode*384)*d_nsegments - 240;
            d_nbits = 2;
            break;
         case 16:  
            d_delay = d_delay_16qam;
            d_dcorrection = (d_mode*384*2)*d_nsegments - 480;
            d_nbits = 4;
            break;
         case 64:
            d_delay = d_delay_64qam;
            d_dcorrection = (d_mode*384*3)*d_nsegments - 720;
            d_nbits = 6;
            break;
         default:
            std::cout << "bit_interleaver: error in d_const_size\n";
            break;
      }

      for (int i = 0; i < (d_nbits); i++){
        d_shift.push_back(new std::deque<bool>(d_delay[i]-1+d_dcorrection,0));
      }      

    }

    /*
     * Our virtual destructor.
     */
    bit_interleaver_impl::~bit_interleaver_impl()
    {

      for (unsigned int i=0; i<d_shift.size();i++){
        delete d_shift.back();
        d_shift.pop_back();
      }
    }

    void
    bit_interleaver_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = noutput_items;
    }

    int
    bit_interleaver_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];


      for (int i = 0; i < noutput_items; i++){
        out[i] = 0x00;
        
        for (int j = 0; j < 8; j++){

          if (((0x01 << j) & in[i]) > 0){
            d_shift[queue_index]->push_back(1);
          }
          else{
            d_shift[queue_index]->push_back(0);
          }
            // > 0 means that is 1, then push 1 into the queue 
          
          // if the last item in the queue is 1, then set the output bit as 1
          bit = d_shift[queue_index]->front();
          
          d_shift[queue_index]->pop_front();

          if (bit){
                out[i] = ((0x01 << j)|out[i]);
              } 
          
          queue_index = (queue_index + 1) % d_nbits;
        }
      }

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    } /* general work*/

  } /* namespace isdbt */
} /* namespace gr */

