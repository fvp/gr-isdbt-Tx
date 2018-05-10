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

#ifndef INCLUDED_ISDBT_MAPPER_IMPL_H
#define INCLUDED_ISDBT_MAPPER_IMPL_H

#include <isdbt/mapper.h>
#include <bitset>

using namespace std;

namespace gr {
  namespace isdbt {

    class mapper_impl : public mapper
    {
     private:
      int d_mode, d_constType, d_carriers_per_segment, d_noutput, inputSize, d_counter, d_input_counter;
      bool d_IsFullSeg;
      static const int d_data_carriers_mode1 = 96;
      std::vector< std::deque<bool> *> delay_vector;

      gr_complex symbol_dic_4[4] = {gr_complex(1,1), gr_complex(1,-1), gr_complex(-1,1), gr_complex(-1,-1)
                                    };

      gr_complex symbol_dic_16[16] = {gr_complex(3,3), gr_complex(3,1), gr_complex(1,3), gr_complex(1,1),\
                                      gr_complex(3,-3), gr_complex(3,-1), gr_complex(1,-3), gr_complex(1,-1),\
                                      gr_complex(-3,3), gr_complex(-3,1), gr_complex(-1,3), gr_complex(-1,1),\
                                      gr_complex(-3,-3), gr_complex(-3,-1), gr_complex(-1,-3), gr_complex(-1,-1)
                                    };

      gr_complex symbol_dic_64[64] = {gr_complex(7,7), gr_complex(7,5), gr_complex(5,7), gr_complex(5,5),\
                                      gr_complex(7,1), gr_complex(7,3), gr_complex(5,1), gr_complex(5,3),\
                                      gr_complex(1,7), gr_complex(1,5), gr_complex(3,7), gr_complex(3,5),\
                                      gr_complex(1,1), gr_complex(1,3), gr_complex(3,1), gr_complex(3,3),\
                                      gr_complex(7,-7), gr_complex(7,-5), gr_complex(5,-7), gr_complex(5,-5),\
                                      gr_complex(7,-1), gr_complex(7,-3), gr_complex(5,-1), gr_complex(5,-3),\
                                      gr_complex(1,-7), gr_complex(1,-5), gr_complex(3,-7), gr_complex(3,-5),\
                                      gr_complex(1,-1), gr_complex(1,-3), gr_complex(3,-1), gr_complex(3,-3),\
                                      gr_complex(-7,7), gr_complex(-7,5), gr_complex(-5,7), gr_complex(-5,5),\
                                      gr_complex(-7,1), gr_complex(-7,3), gr_complex(-5,1), gr_complex(-5,3),\
                                      gr_complex(-1,7), gr_complex(-1,5), gr_complex(-3,7), gr_complex(-3,5),\
                                      gr_complex(-1,1), gr_complex(-1,3), gr_complex(-3,1), gr_complex(-3,3),\
                                      gr_complex(-7,-7), gr_complex(-7,-5), gr_complex(-5,-7), gr_complex(-5,-5),\
                                      gr_complex(-7,-1), gr_complex(-7,-3), gr_complex(-5,-1), gr_complex(-5,-3),\
                                      gr_complex(-1,-7), gr_complex(-1,-5), gr_complex(-3,-7), gr_complex(-3,-5),\
                                      gr_complex(-1,-1), gr_complex(-1,-3), gr_complex(-3,-1), gr_complex(-3,-3)
                                    };
     
     public:
      mapper_impl(int mode, int constType, bool isFullSeg);
      ~mapper_impl();

      gr_complex mapQPSK(unsigned char data);
      gr_complex map16QAM(unsigned char data);
      gr_complex map64QAM(unsigned char data);

      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace isdbt
} // namespace gr

#endif /* INCLUDED_ISDBT_MAPPER_IMPL_H */

