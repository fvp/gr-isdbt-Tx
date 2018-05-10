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
      printf("Entrando al make\n");

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
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))
                  )
    {
      d_mode = mode; 
      d_noutput = d_total_segments*d_carriers_per_segment;
      d_IsFullSeg = isFullSeg;
      d_constType = constType;
      d_counter = 0;
      d_input_counter = 0;

       switch (d_constType)
        {
          case 1:
          {
            delay_vector.push_back(new std::deque<bool>(120)); 
            break;
          }
          case 2:
          {
            for (int k=1; k<4; k++)
            {
              delay_vector.push_back(new std::deque<bool>(40*k)); 
            }
            break;
          }
          case 3:
          {
            for (int k=1; k<6; k++)
            {
              delay_vector.push_back(new std::deque<bool>(24*k)); 
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
      bool b0, b1;
      gr_complex symbol_out;
      
      bitset<8> temp = bitset<8> (data);
     
      //1) Push bit b1 into queue
      if (temp.test(1))
      {
        delay_vector[0]->push_back(true);
      }
      else
      {
        delay_vector[0]->push_back(false);
      }

      //Assign b0' = b0
      if (temp.test(0))
      {
        b0 = true;
      }
      else
      {
        b0 = false;
      }
      
      //2) Take bit b1' from queue
      b1 = delay_vector[0]->front();
      delay_vector[0]->pop_front();
      
      //3) Map bits into symbol from dictionary
      int result = (2)*(b0 ? 1 : 0) + (1)*(b1 ? 1 : 0);

      symbol_out = symbol_dic_4[result];
      return symbol_out;
    }

    gr_complex
    mapper_impl::map16QAM(unsigned char data)
    {
      bool b3, b2, b1, b0;
      gr_complex symbol_out;
      
      bitset<8> temp = bitset<8> (data);

      //1) Push bits into queue
      for (int i=1; i<4; i++)
      {
        if (temp.test(1))
        {
          delay_vector[i-1]->push_back(true);
        }
        else
        {
          delay_vector[i-1]->push_back(false);
        }
      }
      //Assign b0' = b0
      if (temp.test(0))
      {
        b0 = true;
      }
      else
      {
        b0 = false;
      }
      //Assign b1, b2, b3
      b1 = delay_vector[0]->front();
      delay_vector[0]->pop_front();

      b2 = delay_vector[1]->front();
      delay_vector[1]->pop_front();

      b3 = delay_vector[2]->front();
      delay_vector[2]->pop_front();

      //Map bits into symbol.
      int result = (8)*(b0 ? 1 : 0) + (4)*(b1 ? 1 : 0) + (2)*(b2 ? 1 : 0) + (1)*(b3 ? 1 : 0);
      symbol_out = symbol_dic_16[result];

      return symbol_out;
    }

    gr_complex
    mapper_impl::map64QAM(unsigned char data)
    {
      bool b5, b4, b3, b2, b1, b0;
      gr_complex symbol_out;
      
      bitset<8> temp = bitset<8> (data);

      //1) Push bits into queue
      for (int i=1; i<6; i++)
      {
        if (temp.test(i))
        {
          delay_vector[i-1]->push_back(true);
        }
        else
        {
          delay_vector[i-1]->push_back(false);
        }
      }
      //Obtain bits and map
      if (temp.test(0))
      {
        b0 = false;
      }
      else
      {
        b0 = true;
      }
      b1 = delay_vector[0]->front();
      delay_vector[0]->pop_front();

      b2 = delay_vector[1]->front();
      delay_vector[1]->pop_front();

      b3 = delay_vector[2]->front();
      delay_vector[2]->pop_front();

      b4 = delay_vector[3]->front();
      delay_vector[3]->pop_front();

      b5 = delay_vector[4]->front();
      delay_vector[4]->pop_front();

      //Map bits into symbol.
      int result = (32)*(b0 ? 1 : 0) + (16)*(b1 ? 1 : 0) + (8)*(b2 ? 1 : 0) + (4)*(b3 ? 1 : 0) + (2)*(b4 ? 1 : 0) + (1)*(b5 ? 1 : 0);
      symbol_out = symbol_dic_64[result];

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

      switch (d_constType)
        {
          case 1:
          {
            for (int i=0; i<noutput_items; i++)
            {
              out[i] = mapQPSK(in[i]);
            }
            break;
          }
          case 2:
          {
            for (int i=0; i<noutput_items; i++)
            {
              out[i] = map16QAM(in[i]);
            }
            break;
          }
          case 3:
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
        
      }
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

