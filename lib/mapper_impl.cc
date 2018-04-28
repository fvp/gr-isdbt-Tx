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
      printf("Entrando al constructor\n");
      d_mode = mode; 
      d_noutput = d_total_segments*d_carriers_per_segment;
      d_IsFullSeg = isFullSeg;
      d_constType = constType;
      d_counter = 0;
      d_input_counter = 0;


      //create delay queue, according to modulation
      printf("Saliendo del constructor\n");
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
      printf("Dentro de forecast \n");
      ninput_items_required[0] = noutput_items;
    }

    int
    mapper_impl::next2bits(const unsigned char inputData)
    {
      printf("Dentro de next2bits \n");
      //obtengo siguientes 2 de la entrada
      int next = 0;

      top:
      switch (d_counter)
      {
        case 0:
        {
          next = (inputData & 0x03);
          d_counter++;
          break;
        } 
        case 1:
        {
          next = inputData & 0x0C;
          next = (next >> 2);
          d_counter++;
          break;
        }
        case 2:
                {
          next = inputData & 0x30;
          next = (next >> 4);
          d_counter++;
          break;
        }
        case 3:
        {
          next = inputData & 0xC0;
          next = (next >> 6);
          d_counter++;
          break;
        }
        default:
        {
          d_input_counter++;
          d_counter = 0;
          goto top;
        }
      }
      return next;
    }

    gr_complex
    mapper_impl::mapQPSK(int data)
    {
      printf("dentro de mapQPSK \n");
      int temp = 0;
      //QPSK
      temp = next2bits(data);
      bitset<1> b0 = temp & 0x01;
      bitset<1> b1 = temp & 0x02;
      //b1 goes to queue, b0 goes out
      printf("ACA SE CAE MIRA\n");
      //delay_vector[0]->push_back(b1);

      //obtain new b1, delayed 120 samples
      if (true)
      {
        b1.set(0);
      }
      else
      {
        b1.reset(0);
      }
      //convert bits into constelation
      if (b0.test(0))
      {
        if (b1.test(0))
        {
          return  ((-1 / sqrt(2)), (-1 / sqrt(2)));
        }
        else
        {
          return  ((1 / sqrt(2)), (-1 / sqrt(2)));
        }
      } 
      else
      {
        if (b1.test(0))
        {
          return  ((-1 / sqrt(2)), (1 / sqrt(2)));
        }
        else
        {
          return ((1 / sqrt(2)), (1 / sqrt(2)));
        }
      }
    }

    int
    mapper_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      printf("GENERAL WORK \n");
      const unsigned char *in = (const unsigned char *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      int j = 0;
      printf("Antes de entrar. noutput_items: %i\n", noutput_items);

      for (int i=0; i<noutput_items; i++)
      {
        //obtengo bits
        switch (d_constType)
        {
          case 1:
          {
            printf("Caso QPSK \n");
            int data = next2bits(in[i]);
            out[j] = mapQPSK(data);
          }
          case 2:
          {
            //16QAM
            printf("Caso 16QAM \n");
            break;
          }
          case 3:
          {
            printf("Caso 64QAM \n");
            //64QAM
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

