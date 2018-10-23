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

    mapper::sptr
    mapper::make(int mode, int mod_scheme, int segments, float cp)
    {
      return gnuradio::get_initial_sptr
        (new mapper_impl(mode, mod_scheme, segments, cp));
    }

    /*
     * The private constructor
     */
    mapper_impl::mapper_impl(int mode, int mod_scheme, int segments, float cp)
      : gr::block("mapper",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))
                  )
    {
      d_mode = mode; 
      //d_noutput = d_total_segments*d_carriers_per_segment;
      //d_IsFullSeg = isFullSeg;
      d_segments = segments;
      d_mod_scheme = mod_scheme;
      d_counter = 0;
      d_input_counter = 0;
      d_cp = cp;

      d_tsp_per_frame = (int) round((1+d_cp)*pow(2, 9 + d_mode));

      //Find totay delay given the modulation parameters
      int factor = (int) (pow(2.0,mode-1));
      d_delay_bits = (d_tsp_per_frame*204*8*d_segments - 120*d_mod_scheme)/d_mod_scheme;
      //d_delay_bits = (72*d_segments*204*8-720)/6;
      //d_delay_bits = 100;
      //printf("d_delay_bits: %i\n", d_delay_bits);

      switch (d_mod_scheme)
      {
        case 2:
        {
          printf("Constelacion: QPSK\n");
          delay_vector.push_back(new std::deque<bool>(d_delay_bits,false));
          for (int k=1; k<2; k++)
          {
            delay_vector.push_back(new std::deque<bool>(120+d_delay_bits)); 
          }
          break;
        }
        case 4:
        {
          printf("Constelacion: 16-QAM\n");
          delay_vector.push_back(new std::deque<bool>(d_delay_bits,false));
          for (int k=1; k<4; k++)
          {
            delay_vector.push_back(new std::deque<bool>(40*k+d_delay_bits)); 
          }
          break;
        }
        case 6:
        {
          printf("Constelacion: 64-QAM\n");
          delay_vector.push_back(new std::deque<bool>(d_delay_bits,false));
          for (int k=1; k<6; k++)
          {
            delay_vector.push_back(new std::deque<bool>(24*k+d_delay_bits, false)); 
          }
          break;
        }
        default:
        {
          printf("Caso default \n");
          break;
        }
      }
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
      ninput_items_required[0] = noutput_items;
    }

    gr_complex
    mapper_impl::mapQPSK(unsigned char data)
    {
      // TODO: make the corrections according to the map64QAM
      bool b3, b2, b1, b0;
      gr_complex symbol_out;
      
      bitset<8> temp = bitset<8> (data);
      
      //Packed to Unpacked block gives us
      //  000000b0b1 so we cast it to int
      
      //1) Push bits into queue
      for (int i=0; i<2; i++)
      {
        if (temp.test(i))
        {
          delay_vector[i]->push_back(true);
        }
        else
        {
          delay_vector[i]->push_back(false);
        }
      }

      //Assign b1, b2
      b0 = delay_vector[0]->front();
      delay_vector[0]->pop_front();

      b1 = delay_vector[1]->front();
      delay_vector[1]->pop_front();

      //Map bits into symbol.
      int result = (2)*(b0 ? 1 : 0) + (1)*(b1 ? 1 : 0);

      symbol_out = symbol_dic_4[result];
      symbol_out.real((1/sqrt(2))*symbol_out.real());
      symbol_out.imag((1/sqrt(2))*symbol_out.imag());
      return symbol_out;
    }

    gr_complex
    mapper_impl::map16QAM(unsigned char data)
    {
      // TODO: make the corrections according to the map64QAM
      bool b3, b2, b1, b0;
      gr_complex symbol_out;
      
      bitset<8> temp = bitset<8> (data);

      //1) Push bits into queue
      for (int i=0; i<4; i++)
      {
        if (temp.test(i))
        {
          delay_vector[i]->push_back(true);
        }
        else
        {
          delay_vector[i]->push_back(false);
        }
      }

      //Assign b1, b2, b3
      b0 = delay_vector[0]->front();
      delay_vector[0]->pop_front();

      b1 = delay_vector[1]->front();
      delay_vector[1]->pop_front();

      b2 = delay_vector[2]->front();
      delay_vector[2]->pop_front();

      b3 = delay_vector[3]->front();
      delay_vector[3]->pop_front();

      //Map bits into symbol.
      int result = (8)*(b0 ? 1 : 0) + (4)*(b1 ? 1 : 0) + (2)*(b2 ? 1 : 0) + (1)*(b3 ? 1 : 0);
      symbol_out = symbol_dic_16[result];

      symbol_out.real((1/sqrt(10))*symbol_out.real());
      symbol_out.imag((1/sqrt(10))*symbol_out.imag());
      return symbol_out;
    }

    gr_complex
    mapper_impl::map64QAM(unsigned char data)
    {
      bool b5, b4, b3, b2, b1, b0, carry;
      gr_complex symbol_out;
      
      bitset<8> temp = bitset<8> (data);
      

      
      //Obtain bits and map

      b0 = delay_vector[0]->front();
      delay_vector[0]->pop_front();

      b1 = delay_vector[1]->front();
      delay_vector[1]->pop_front();

      b2 = delay_vector[2]->front();
      delay_vector[2]->pop_front();

      b3 = delay_vector[3]->front();
      delay_vector[3]->pop_front();

      b4 = delay_vector[4]->front();
      delay_vector[4]->pop_front();

      b5 = delay_vector[5]->front();
      delay_vector[5]->pop_front();

      //Map bits into symbol.
      int result = (32)*(b0 ? 1 : 0) + (16)*(b1 ? 1 : 0) + (8)*(b2 ? 1 : 0) + (4)*(b3 ? 1 : 0) + (2)*(b4 ? 1 : 0) + (1)*(b5 ? 1 : 0);
      symbol_out = symbol_dic_64[result];

      symbol_out.real((1/sqrt(42))*symbol_out.real());
      symbol_out.imag((1/sqrt(42))*symbol_out.imag());


      // Push bits into delay queue
      for (int i=0; i<6; i++)
      {
        if (temp.test(i))
        {
          delay_vector[5-i]->push_back(true);
        }
        else
        {
          delay_vector[5-i]->push_back(false);
        }
      }

      return symbol_out;
    }

    int
    mapper_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
      //
      switch (d_mod_scheme)
        {
          case 2:
          {
            for (int i=0; i<noutput_items; i++)
            {
              out[i] = mapQPSK(in[i]);
            }
            break;
          }
          case 4:
          {
            for (int i=0; i<noutput_items; i++)
            {
              out[i] = map16QAM(in[i]);
            }
            break;
          }
          case 6:
          {
            for (int i=0; i<noutput_items; i++)
            {
              out[i] = map64QAM(in[i]);
            }
            break;
          }
          default:
          {
            printf("Caso default \n");
            break;
          }
        }
        
      
      // Tell runtime system how many input items we consumed on
      // each input stream.
      this->consume(0, noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

