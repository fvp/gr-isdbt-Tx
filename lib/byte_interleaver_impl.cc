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
#include "byte_interleaver_impl.h"

namespace gr {
  namespace isdbt {

    byte_interleaver::sptr
    byte_interleaver::make()
    {
      return gnuradio::get_initial_sptr
        (new byte_interleaver_impl());
    }

    /*
     * The private constructor
     */
    byte_interleaver_impl::byte_interleaver_impl()
      : gr::sync_block("byte_interleaver",
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)),
              gr::io_signature::make(1, 1, 204*sizeof(unsigned char)))
    {
      printf("Entro a crear colas: \n");

      for (int index=0; index<12; index++)
      {
          //Create queue
          delay_vector.push_back(new std::deque<unsigned char>((index+1)*17)); 
      }
    }

    /*
     * Our virtual destructor.
     */
    byte_interleaver_impl::~byte_interleaver_impl()
    {
      for (unsigned int i=0; i<delay_vector.size();i++)
      {
        delete delay_vector.back();
        delay_vector.pop_back();
      }
    }

    int
    byte_interleaver_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      
      int queue_index = 0;

      printf("noutput_items: %i\n", noutput_items);

      for (int i=0; i<204; i++)
      {
        queue_index = (i % 12);  
      }


      for (int input = 0; input<noutput_items; input++)
      {
        for (int i=0; i<204; i++)
        {
          queue_index = (i % 12);
          delay_vector[queue_index]->push_back(in[204*input + i]);

          out[204*input + i] = delay_vector[queue_index]->front();
          delay_vector[queue_index]->pop_front(); 
        }
      }

      // Tell runtime system how many output items we produced.
      consume_each(noutput_items);
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

