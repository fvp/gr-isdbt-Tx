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
#include "agregar_cp_impl.h"
#include <stdio.h>

namespace gr {
  namespace isdbt {

    
    /*  BLOQUE CP
    Este bloque recibe un vector complejo, y como salida devuelve el mismo vector complejop
    al que se le ha agregado al final, la secuencia de n primeros bits. 
    */
    
    agregar_cp::sptr
          /*Agregamos las variables que se configuran en el bloque pero no son entradas ni salidas*/
    agregar_cp::make(int mode, float cp_length)
    {
      return gnuradio::get_initial_sptr
        (new agregar_cp_impl(mode, cp_length));
    }

    /*
     * The private constructor
     */
    agregar_cp_impl::agregar_cp_impl(int mode, float cp_length)
      : gr::block("agregar_cp",
                  /* Entra el parametro CP (ej 1/4, 1/8) y el vector de bits de tamano 2^(10+modo) */
              gr::io_signature::make(1, 1, sizeof(gr_complex)*pow(2.0,10+mode)),
                  /* Sale solo el vector de bits, con su largo cambiado, ahora es 2^(10+modo) + (CP)*(2^(10+modo)) */
              gr::io_signature::make(1, 1, (1+cp_length)*sizeof(gr_complex)*pow(2.0,10+mode))
                 )
    {
      //Set block global variables
      d_mode = mode;
      d_cp_length = cp_length;
    }

    /*
     * Our virtual destructor.
     */
    agregar_cp_impl::~agregar_cp_impl()
    {
    }

    void agregar_cp_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int agregar_cp_impl::general_work (int noutput_items,         /* Number of output vectors required by grc */
                       gr_vector_int &ninput_items,               /* Vector containing input size for all inputs */
                       gr_vector_const_void_star &input_items,    /* Vector containing a pointer to the first element of every input stream */
                       gr_vector_void_star &output_items)         /* Vector containing a pointer to the first element of every output stream */
    {
      // 1-> Cast buffers
      const gr_complex *in = (const gr_complex *) input_items[0];   /* in stores the input data*/
      gr_complex *out = (gr_complex *) output_items[0];             /* out stores pointer to output array*/
      
      // 2-> Process Data
      int d_cp_items =  static_cast<int> ((d_cp_length)*pow(2.0,10+d_mode));            /* d_cp_items is the number of samples in cp*/
      int d_total_input_size = static_cast<int> (pow(2.0, 10+d_mode));
      int output_size = static_cast<int> ((1 + d_cp_length)*pow(2.0,10+d_mode));
      
      printf("output_size: %i\n", output_size);
      printf("d_cp_items: %i\n", d_cp_items);
      printf("d_total_input_size: %i\n", d_total_input_size);

      for (int i=0; i<noutput_items; i++)   
      {
        memcpy(out + i*output_size, in + i*d_total_input_size, d_cp_items);
        memcpy(out + d_cp_items + i*output_size, in + i*d_total_input_size, d_total_input_size); 
      }

      // 3-> Consume the inputs
      this->consume(0, noutput_items); //consume port 0 input
      //this->consume_each(noutput_items); //or shortcut to consume on all inputs

      // 4-> Return produced
      return noutput_items;
    } /* general_work*/

  } /* namespace isdbt */
} /* namespace gr */

