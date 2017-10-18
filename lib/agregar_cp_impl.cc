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
              gr::io_signature::make(1, 1, sizeof(gr_complex)*pow(2.0,10+mode) + mode*(sizeof(gr_complex)*pow(2.0,10+mode)))
                 )
    {
    /* aca podemos acceder a una variable y setearla con cp_length*/
       b_cp_length = (int)round(cp_length); 
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

    int agregar_cp_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
            gr_complex *out = (gr_complex *) output_items[0];
      
      int vector_size = sizeof(ninput_items);
      // Agregamos las CP muestras del final, al princpio
      int first = vector_size - b_cp_length*vector_size;
      int j = 0;
      for (int i=first; i<vector_size; i++)
      {
        out[j] = in[i];
        j++;
      }
      //Copio todo el vector de entrada
      for (int i =0; i<vector_size; i++)
      {
        out[j] = in[i];
        j++;
      }
           
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

