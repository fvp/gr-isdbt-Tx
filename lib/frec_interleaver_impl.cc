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
#include "frec_interleaver_impl.h"

namespace gr {
  namespace isdbt {

    static const int d_data_carriers_mode1 = 96;
    static const int d_data_carriers_mode2 = 192;
    static const int d_data_carriers_mode3 = 384;

    int d_segments_total = 1;

    static int d_active_carriers;
    static int d_noutput;

    gr_complex out_before_rand[192*13];
    gr_complex data[192][13] = {}; 
    gr_complex data_interleaved[13][192] = {};

    /* TODO: The case when we do not send 1-seg */
    /* in the full-seg mode we assume 1-seg available */
    frec_interleaver::sptr
    frec_interleaver::make(int mode, bool IsFullSeg)
    {
      //Define number of segments in transmission
      if (IsFullSeg)
      {
        d_segments_total = 13;
      }
      switch (mode)
      {
        case 1:
        {
          d_active_carriers = d_data_carriers_mode1;
          break;          
        }
        case 2:
        {
          d_active_carriers = d_data_carriers_mode2;
          break;          
        }
        case 3:
        {
          d_active_carriers = d_data_carriers_mode3;
          break;          
        }
        default:
        {
          printf("H. Comb) Error: Incorrect mode\n");
          d_active_carriers = 0;
          break;          
        }
      }
      d_noutput = d_active_carriers*d_segments_total;
      
      return gnuradio::get_initial_sptr
        (new frec_interleaver_impl(mode, IsFullSeg));
    }

    /*
     * The private constructor
     */
    frec_interleaver_impl::frec_interleaver_impl(int mode, bool IsFullSeg)
    : gr::sync_block("frec_interleaver",
            gr::io_signature::make(1, 1, sizeof(gr_complex)*d_segments_total*d_active_carriers),
            gr::io_signature::make(1, 1, sizeof(gr_complex)*d_segments_total*d_active_carriers))
    {
      d_mode = mode; 
      d_noutput = d_segments_total*d_active_carriers;
      d_IsFullSeg = IsFullSeg;
    }



    int
    frec_interleaver_impl::intra_segment_interleaver(gr_complex* in)
    {
      //printf("1. Intra-Segment interleaving\n");
      //1. Intra-Segment interleaving
      int k=0; 
      for(int i=0; i < d_active_carriers; i++)
      {
        for (int j=0; j<d_segments_total-1; j++)
        {
          // First we write the matrix rows-first
          data[i][j] = in[k];
          k++;
         }
      }   
      for (int j=0; j<d_segments_total-1; j++)
      {
        for(int i=0; i<d_active_carriers; i++)
        {
          // First we write the matrix rows-first
          data_interleaved[j][i] = data[i][j];
        }
      }
      return 0; 
    }

    int
    frec_interleaver_impl::inter_segment_rotation()
    {
      //printf("2. Inter-segment rotation start\n");
      //2. Inter-segment rotation
      int k = 0;
      for(int i=0; i<d_segments_total-1; i++)
      {
        for (int j=0; j<d_active_carriers; j++)
        {
          // First we re-write the data matrix, rows-first
          out_before_rand[k] = data_interleaved[i][(i + j+1) % d_active_carriers];
          k++;
        }
      }
      return 0; 
    }

    int
    frec_interleaver_impl::randomizer(gr_complex* out, int segment)
    {
      //printf("3. Randomizer start\n");
      int index = 0;
      switch(d_mode)
      {
        case 1:
        {
          for(int i=0; i < d_active_carriers; i++)
          {
            index = rand_mode_1[i];
            out[index] = out_before_rand[i + d_active_carriers*(segment-1)];
            //printf("out[%i] = %f \n", index, out[index].real());
          }
          break;
        }
        case 2:
        {
          for(int i=0; i < d_active_carriers; i++)
          {
            index = rand_mode_2[i];
            out[index] = out_before_rand[i + d_active_carriers*(segment-1)];
          }
          break;
        }
        case 3:
        {
          for(int i=0; i<d_active_carriers; i++)
          {
            index = rand_mode_3[i];
            out[index] = out_before_rand[i + d_active_carriers*(segment-1)];
          }
          break;
        }
        default:
        {
          printf("Error: Incorrect mode\n");
          break;
        }
      }
      return 0;  
    }
    /*
     * Our virtual destructor.
     */
    frec_interleaver_impl::~frec_interleaver_impl()
    {
    }

    int
    frec_interleaver_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      gr_complex *in = (gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      for (int output = 0; output < noutput_items; output++)
      {
        if (d_IsFullSeg)
        {
          //Segment 0; from in[0]->in[d_active_carriers - 1]
          memcpy(&out_before_rand, in + output*d_noutput, d_active_carriers*sizeof(gr_complex));

          this->randomizer(out + output*d_noutput, 1);
          //Segments 1 to 12; from in[192]->in[12*d_active_carriers-1]
          this->intra_segment_interleaver(in + d_active_carriers + output*d_noutput);
          this->inter_segment_rotation();

          
          for (int seg = 1; seg < d_segments_total; seg++)
          {
            this->randomizer(out + seg*d_active_carriers + output*d_noutput, seg);
          }
        }
        else
        {
          /*TODO: we should review this case*/
          //Segment 0; from in[0]->in[d_active_carriers - 1]
          //memcpy(&out_before_rand, in + output*d_noutput, d_noutput*sizeof(gr_complex));
          printf("Error in freq interleaver\n");
        }
      }
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */
