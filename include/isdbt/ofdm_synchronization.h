/* -*- c++ -*- */
/*  
 * Copyright 2015
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


#ifndef INCLUDED_ISDBT_OFDM_SYNCHRONIZATION_H
#define INCLUDED_ISDBT_OFDM_SYNCHRONIZATION_H

#include <isdbt/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace isdbt {

    /*!
     * \brief This block performs the whole OFDM synchronization: symbol timing, frequency correction and channel
     * equalization. 
     * \ingroup isdbt
     *
     */
    class ISDBT_API ofdm_synchronization 
        : virtual public gr::block
   {
     public:
      typedef boost::shared_ptr<ofdm_synchronization> sptr;

      /*!
       * \brief This block performs the whole OFDM synchronization: symbol timing, frequency correction and channel
       * equalization. 
       * \return a sptr pointer.
       */
      static sptr make(int mode, float cp_length);
    };

  } // namespace isdbt
} // namespace gr

#endif /* INCLUDED_ISDBT_OFDM_SYNCHRONIZATION_H */

