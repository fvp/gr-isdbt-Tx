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
#include "ofdm_frame_structure_impl.h"

namespace gr {
  namespace isdbt {

    ofdm_frame_structure::sptr
    ofdm_frame_structure::make(int mode, bool IsOneSeg, 
                                int ModSchemeA, int ModSchemeB, int ModSchemeC, 
                                int ConvCodeA, int ConvCodeB, int ConvCodeC,
                                int IntLengthA, int IntLengthB, int IntLengthC,
                                int LayerA_seg, int LayerB_seg, int LayerC_seg)
    {
      return gnuradio::get_initial_sptr
        (new ofdm_frame_structure_impl(mode, IsOneSeg, 
                                        ModSchemeA, ModSchemeB, ModSchemeC, 
                                        ConvCodeA, ConvCodeB, ConvCodeC,
                                        IntLengthA, IntLengthB, IntLengthC,
                                        LayerA_seg, LayerB_seg, LayerC_seg));
    }

    /*
     * The private constructor
     */
    ofdm_frame_structure_impl::ofdm_frame_structure_impl(int mode, bool IsOneSeg, 
                                  int ModSchemeA, int ModSchemeB, int ModSchemeC, 
                                  int ConvCodeA, int ConvCodeB, int ConvCodeC,
                                  int IntLengthA, int IntLengthB, int IntLengthC,
                                  int LayerA_seg, int LayerB_seg, int LayerC_seg)
      : gr::block("ofdm_frame_structure",
              gr::io_signature::make(1, 1, sizeof(gr_complex) * 13 * 96 * ((int)pow(2.0,mode-1))),
              gr::io_signature::make(1, 1, sizeof(gr_complex) * ((int)pow(2.0,10 + mode)))
              )
    {
      d_IsOneSeg = IsOneSeg;  /* Transmission mdoe, one seg or full seg*/
      frame_counter = 0;      /* Counts frames*/
      d_carrier_pos = 0;    /* 4 cyclic counter, scattered pilot rotation reference*/
      TMCCindex = 0;        /* TMCC word position counter, accross block */
      SPindex = 0;          /* Scattered pilot counter, accross segment */
      d_mode = mode;        /* Transmission Mode */
      sp_keyword = 0b00000000000; /* SP keyword default*/
      TMCCword.set();
      TMCC_sync_word = 0b0011010111101110;
      InputIndex = 0;
      d_ModSchemeA = (carrier_mod_scheme) ModSchemeA;
      d_ModSchemeB = (carrier_mod_scheme) ModSchemeB;
      d_ModSchemeC = (carrier_mod_scheme) ModSchemeC;
      d_ConvCodeA  = (convolutional_coding_rate) ConvCodeA;
      d_ConvCodeB  = (convolutional_coding_rate) ConvCodeB;
      d_ConvCodeC  = (convolutional_coding_rate) ConvCodeC;
      d_IntLengthA = IntLengthA;
      d_IntLengthB = IntLengthB;
      d_IntLengthC = IntLengthC;
      d_LayerA_seg = LayerA_seg;
      d_LayerB_seg = LayerB_seg;
      d_LayerC_seg = LayerC_seg;
      printf("LayerA_seg: %d\n", d_LayerA_seg);
      printf("LayerB_seg: %d\n", d_LayerB_seg);
      printf("LayerC_seg: %d\n", d_LayerC_seg);
    }

    /*
     * Our virtual destructor.
     */
    ofdm_frame_structure_impl::~ofdm_frame_structure_impl()
    {
    }

    /*
    Writes corresponding Scattered Pilot into symbol
    */
    gr_complex 
    ofdm_frame_structure_impl::write_SP(int SPindex, int d_mode, int SegmentNumber)
    {
      bitset<11> keyword = 0b11111111111;
      bitset<1> temp, bit9, bit11;
      /* Evolve starting word to generate PRBS*/
      for (int i=0; i<SPindex; i++)
      {
        bit9 = keyword.test(8);
        bit11 = keyword.test(10);
        temp = bit9 ^ bit11;
        keyword = (keyword >> 1);
        if (temp.test(0)){
          keyword.set(0);
        } else {
          keyword.reset(0);
        }
      }
      if (keyword.test(10)) /*Return bit value in keyword for SPindex*/
      {
        return std::complex<float>(-4.0/3.0, 0);   
      } else {
        return std::complex<float>(4.0/3.0, 0);
      }
    }

    /*
    Writes bits of the TMCC into symbol
    */
    gr_complex 
    ofdm_frame_structure_impl::write_TMCC(int frame_counter, int SegmentNumber)
    {
      //Test TMCC word for writing bit, returns symbol mapped into DBPSK
      bool current_bit, previous_bit;
      //First bit, define TMCC word
      if (frame_counter == 0)
      {
        //Assign b0, check for SP0 value
        gr_complex sp0 = this->write_SP(SPindex, d_mode, SegmentNumber);
        if(sp0.real() < 0)
        {
          TMCCword.set(0);
        } else {
          TMCCword.reset(0);
        }
        //Assign b1-b16
        bool EsPar = ((frame_counter % 2) == 0);
        for (int i = 0; i < 16; i++)
        {
          if (EsPar)
          {
            if (TMCC_sync_word.test(i))
            {
              TMCCword.set(i+1);
            } else {
              TMCCword.reset(i+1);
            }
          }
          else
          {
            if (TMCC_sync_word.test(i))
            {
              TMCCword.reset(i+1);
            } else {
              TMCCword.set(i+1);
            }
          }
        }
        //Segment type identifier
          //This example only uses Coherent modulation
        TMCCword.set(17);
        TMCCword.set(18);
        TMCCword.set(19);
        //TMCC information bits
        //20-21 system id, 00 for terrestrial
        TMCCword.reset(20);
        TMCCword.reset(21);
        //Transmission parameters
        TMCCword.set(22);
        TMCCword.set(23);
        TMCCword.set(24); //Test case: No changes in system
        TMCCword.set(25); 
        TMCCword.reset(26); //Alarm bit, defaulted in 0 //TODO: fijar parametrico, desde fuera del bloque
        if (d_IsOneSeg){
          TMCCword.set(27);
        } else {
          TMCCword.reset(27);
        }
        // Current info
        // Layer A
        switch(d_ModSchemeA) {
         case DQPSK:
          {
            TMCCword.reset(28);
            TMCCword.reset(29);
            TMCCword.reset(30);
             break; 
          }      
          case QPSK:
          {
            TMCCword.reset(28);
            TMCCword.reset(29);
            TMCCword.set(30);
             break; 
          } 
          case QAM_16:
          {
            TMCCword.reset(28);
            TMCCword.set(29);
            TMCCword.reset(30);
             break; 
          } 
          case QAM_64:
          {
            TMCCword.reset(28);
            TMCCword.set(29);
            TMCCword.set(30);
             break; 
          } 
          case UNUSED:
          {
            TMCCword.set(28);
            TMCCword.set(29);
            TMCCword.set(30);
             break; 
          }
          default:
          {
            printf("Error, incorrect mode A modulation scheme \n");
            break;
          }
        }
        switch(d_ConvCodeA) {
         case c_1_2:
          {
            TMCCword.reset(31);
            TMCCword.reset(32);
            TMCCword.reset(33);
             break; 
          }      
          case c_2_3:
          {
            TMCCword.reset(31);
            TMCCword.reset(32);
            TMCCword.set(33);
             break; 
          } 
          case c_3_4:
          {
            TMCCword.reset(31);
            TMCCword.set(32);
            TMCCword.reset(33);
             break; 
          } 
          case c_5_6:
          {
            TMCCword.reset(31);
            TMCCword.set(32);
            TMCCword.set(33);
             break; 
          } 
          case c_7_8:
          {
            TMCCword.set(31);
            TMCCword.reset(32);
            TMCCword.reset(33);
             break; 
          }
          case RESERVED:
          {
            //TMCCword.set(28);
            //TMCCword.reset(29);
            //TMCCword.reset(30);
             break; 
          }
          case UNUSED_HL:
          {
            TMCCword.set(31);
            TMCCword.set(32);
            TMCCword.set(33);
             break; 
          }
          default:
          {
            printf("Error, incorrect mode A convolutional_coding_rate \n");
            break;
          }
        }
        // b34-b40
        if (d_ModSchemeA != UNUSED)
        {
          switch (d_mode)
          {
            printf("Entra al switch con d_mode: %d \n", d_mode);
            case 1: 
            {
              switch (d_IntLengthA) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(34);
                  TMCCword.reset(35);
                  TMCCword.reset(36);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(34);
                  TMCCword.reset(35);
                  TMCCword.set(36);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(34);
                  TMCCword.set(35);
                  TMCCword.reset(36);
                  break;
                }
                case 16:
                {
                  TMCCword.reset(34);
                  TMCCword.set(35);
                  TMCCword.set(36);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer A (Tx mode = %d) \n", d_mode);
                  break; 
                }
              }
            break;
            }
            case 2: 
            {
              printf("Entra al case 2 \n");
              switch (d_IntLengthA) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(34);
                  TMCCword.reset(35);
                  TMCCword.reset(36);
                  break;
                }
                case 2:
                {
                  TMCCword.reset(34);
                  TMCCword.reset(35);
                  TMCCword.set(36);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(34);
                  TMCCword.set(35);
                  TMCCword.reset(36);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(34);
                  TMCCword.set(35);
                  TMCCword.set(36);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer A (Tx mode = %d) \n", d_mode);
                  break;
                }
              }
            break;
            }
            case 3: 
            {
              printf("Entra al case 3 \n");
              switch (d_IntLengthA) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(34);
                  TMCCword.reset(35);
                  TMCCword.reset(36);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(34);
                  TMCCword.reset(35);
                  TMCCword.set(36);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(34);
                  TMCCword.set(35);
                  TMCCword.reset(36);
                  break;
                }
                case 16:
                {
                  TMCCword.reset(34);
                  TMCCword.set(35);
                  TMCCword.set(36);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer A (Tx mode = %d) \n", d_mode);
                  break;
                }
              }
            break;
            }
            default: 
            {
              printf("Entra al default \n");
              printf("Error: Incorrect Transmission mode for layer A, mode: %d\n", d_mode);
              break;
            }
          }
        } else {
          TMCCword.set(34);
          TMCCword.set(35);
          TMCCword.set(36);
        }
        //b37 - b40 Layer A number of segments
        switch (d_LayerA_seg){ //TODO: revisar (if d_LayerA_seg.test(0) == TRUE)
          case 0:
          {
            //Unused layer
            TMCCword.set(37);
            TMCCword.set(38);
            TMCCword.set(39);
            TMCCword.set(40);
            break;
          }
          case 1:
          {
            TMCCword.reset(37);
            TMCCword.reset(38);
            TMCCword.reset(39);
            TMCCword.set(40);
            break;
          }
          case 2:
          {
            TMCCword.reset(37);
            TMCCword.reset(38);
            TMCCword.set(39);
            TMCCword.reset(40);
            break;
          }
          case 3:
          {
            TMCCword.reset(37);
            TMCCword.reset(38);
            TMCCword.set(39);
            TMCCword.set(40);
            break;
          }
          case 4:
          {
            TMCCword.reset(37);
            TMCCword.set(38);
            TMCCword.reset(39);
            TMCCword.reset(40);
            break;
          }
          case 5:
          {
            TMCCword.reset(37);
            TMCCword.set(38);
            TMCCword.reset(39);
            TMCCword.set(40);
            break;
          }
          case 6:
          {
            TMCCword.reset(37);
            TMCCword.set(38);
            TMCCword.set(39);
            TMCCword.reset(40);
            break;
          }
          case 7:
          {
            TMCCword.reset(37);
            TMCCword.set(38);
            TMCCword.set(39);
            TMCCword.set(40);
            break;
          }
          case 8:
          {
            TMCCword.set(37);
            TMCCword.reset(38);
            TMCCword.reset(39);
            TMCCword.reset(40);
            break;
          }
          case 9:
          {
            TMCCword.set(37);
            TMCCword.reset(38);
            TMCCword.reset(39);
            TMCCword.set(40);
            break;
          }
          case 10:
          {
            TMCCword.set(37);
            TMCCword.reset(38);
            TMCCword.set(39);
            TMCCword.reset(40);
            break;
          }
          case 11:
          {
            TMCCword.set(37);
            TMCCword.reset(38);
            TMCCword.set(39);
            TMCCword.set(40);
            break;
          }
          case 12:
          {
            TMCCword.set(37);
            TMCCword.set(38);
            TMCCword.reset(39);
            TMCCword.reset(40);
            break;
          }
          case 13:
          {
            TMCCword.set(37);
            TMCCword.set(38);
            TMCCword.reset(39);
            TMCCword.set(40);
            break;
          }
          default:
          {
            printf("Error: Incorrect segment number for layer A");
            break;
          }
        }
        // LAYER B
        switch(d_ModSchemeB) {
         case DQPSK:
          {
            TMCCword.reset(41);
            TMCCword.reset(42);
            TMCCword.reset(43);
             break; 
          }      
          case QPSK:
          {
            TMCCword.reset(41);
            TMCCword.reset(42);
            TMCCword.set(43);
             break; 
          } 
          case QAM_16:
          {
            TMCCword.reset(41);
            TMCCword.set(42);
            TMCCword.reset(43);
             break; 
          } 
          case QAM_64:
          {
            TMCCword.reset(41);
            TMCCword.set(42);
            TMCCword.set(43);
             break; 
          } 
          case UNUSED:
          {
            TMCCword.set(41);
            TMCCword.set(42);
            TMCCword.set(43);
             break; 
          }
          default:
          {
            printf("Error, incorrect mode B modulation scheme \n");
            break;
          }
        }
        switch(d_ConvCodeB) {
         case c_1_2:
          {
            TMCCword.reset(44);
            TMCCword.reset(45);
            TMCCword.reset(46);
             break; 
          }      
          case c_2_3:
          {
            TMCCword.reset(44);
            TMCCword.reset(45);
            TMCCword.set(46);
             break; 
          } 
          case c_3_4:
          {
            TMCCword.reset(44);
            TMCCword.set(45);
            TMCCword.reset(46);
             break; 
          } 
          case c_5_6:
          {
            TMCCword.reset(44);
            TMCCword.set(45);
            TMCCword.set(46);
             break; 
          } 
          case c_7_8:
          {
            TMCCword.set(44);
            TMCCword.reset(45);
            TMCCword.reset(46);
             break; 
          }
          case RESERVED:
          {
            //TMCCword.set(44);
            //TMCCword.reset(45);
            //TMCCword.reset(45);
             break; 
          }
          case UNUSED_HL:
          {
            TMCCword.set(44);
            TMCCword.set(45);
            TMCCword.set(46);
             break; 
          }
          default:
          {
            printf("Error, incorrect mode B convolutional_coding_rate \n");
            break;
          }
        }
        //Interleaving length
        if (d_ModSchemeB != UNUSED)
        {
          switch (d_mode)
          {
            case 1: 
            {
              switch (d_IntLengthB) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(47);
                  TMCCword.reset(48);
                  TMCCword.reset(49);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(47);
                  TMCCword.reset(48);
                  TMCCword.set(49);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(47);
                  TMCCword.set(48);
                  TMCCword.reset(49);
                  break;
                }
                case 16:
                {
                  TMCCword.reset(47);
                  TMCCword.set(48);
                  TMCCword.set(49);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer B (Tx mode = %d) \n", d_mode);
                  break;
                }
              }
            break;
            }
            case 2: 
            {
              switch (d_IntLengthB) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(47);
                  TMCCword.reset(48);
                  TMCCword.reset(49);
                  break;
                }
                case 2:
                {
                  TMCCword.reset(47);
                  TMCCword.reset(48);
                  TMCCword.set(49);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(47);
                  TMCCword.set(48);
                  TMCCword.reset(49);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(47);
                  TMCCword.set(48);
                  TMCCword.set(49);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer B (Tx mode = %d) \n", d_mode);
                  break; 
                }
              }
            break;
            }
            case 3: 
            {
              switch (d_IntLengthB) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(47);
                  TMCCword.reset(48);
                  TMCCword.reset(49);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(47);
                  TMCCword.reset(48);
                  TMCCword.set(49);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(47);
                  TMCCword.set(48);
                  TMCCword.reset(49);
                  break;
                }
                case 16:
                {
                  TMCCword.reset(47);
                  TMCCword.set(48);
                  TMCCword.set(49);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer B (Tx mode = %d) \n", d_mode);
                  break;
                }
              }
            break;
            }
            default: {
              printf("Error: Incorrect Transmission mode for layer B, mode: %d\n", d_mode);
              break;
            }

          }
        } else {
          TMCCword.set(47);
          TMCCword.set(48);
          TMCCword.set(49);
        }
        switch (d_LayerB_seg){ //TODO: revisar (if d_LayerA_seg.test(0) == TRUE)
          case 0:
          {
            //Unused layer
            TMCCword.set(50);
            TMCCword.set(51);
            TMCCword.set(52);
            TMCCword.set(53);
            break;
          }
          case 1:
          {
            TMCCword.reset(50);
            TMCCword.reset(51);
            TMCCword.reset(52);
            TMCCword.set(53);
            break;
          }
          case 2:
          {
            TMCCword.reset(50);
            TMCCword.reset(51);
            TMCCword.set(52);
            TMCCword.reset(53);
            break;
          }
          case 3:
          {
            TMCCword.reset(50);
            TMCCword.reset(51);
            TMCCword.set(52);
            TMCCword.set(53);
            break;
          }
          case 4:
          {
            TMCCword.reset(50);
            TMCCword.set(51);
            TMCCword.reset(52);
            TMCCword.reset(53);
            break;
          }
          case 5:
          {
            TMCCword.reset(50);
            TMCCword.set(51);
            TMCCword.reset(52);
            TMCCword.set(53);
            break;
          }
          case 6:
          {
            TMCCword.reset(50);
            TMCCword.set(51);
            TMCCword.set(52);
            TMCCword.reset(53);
            break;
          }
          case 7:
          {
            TMCCword.reset(50);
            TMCCword.set(51);
            TMCCword.set(52);
            TMCCword.set(53);
            break;
          }
          case 8:
          {
            TMCCword.set(50);
            TMCCword.reset(51);
            TMCCword.reset(52);
            TMCCword.reset(53);
            break;
          }
          case 9:
          {
            TMCCword.set(50);
            TMCCword.reset(51);
            TMCCword.reset(52);
            TMCCword.set(53);
            break;
          }
          case 10:
          {
            TMCCword.set(50);
            TMCCword.reset(51);
            TMCCword.set(52);
            TMCCword.reset(53);
            break;
          }
          case 11:
          {
            TMCCword.set(50);
            TMCCword.reset(51);
            TMCCword.set(52);
            TMCCword.set(53);
            break;
          }
          case 12:
          {
            TMCCword.set(50);
            TMCCword.set(51);
            TMCCword.reset(52);
            TMCCword.reset(53);
            break;
          }
          case 13:
          {
            TMCCword.set(50);
            TMCCword.set(51);
            TMCCword.reset(52);
            TMCCword.set(53);
            break;
          }
          default:
          {
            printf("Error: Incorrect segment number for layer B");
            break;
          }
        }
        // LAYER C
        switch(d_ModSchemeC) {
         case DQPSK:
          {
            TMCCword.reset(56);
            TMCCword.reset(55);
            TMCCword.reset(54);
             break; 
          }      
          case QPSK:
          {
            TMCCword.reset(54);
            TMCCword.reset(55);
            TMCCword.set(56);
             break; 
          } 
          case QAM_16:
          {
            TMCCword.reset(54);
            TMCCword.set(55);
            TMCCword.reset(56);
             break; 
          } 
          case QAM_64:
          {
            TMCCword.reset(54);
            TMCCword.set(55);
            TMCCword.set(56);
             break; 
          } 
          case UNUSED:
          {
            TMCCword.set(54);
            TMCCword.set(55);
            TMCCword.set(56);
             break; 
          }
          default:
          {
            printf("Error, incorrect mode C modulation scheme \n");
            break;
          }
        }
        switch(d_ConvCodeC) {
         case c_1_2:
          {
            TMCCword.reset(57);
            TMCCword.reset(58);
            TMCCword.reset(59);
             break; 
          }      
          case c_2_3:
          {
            TMCCword.reset(57);
            TMCCword.reset(58);
            TMCCword.set(59);
             break; 
          } 
          case c_3_4:
          {
            TMCCword.reset(57);
            TMCCword.set(58);
            TMCCword.reset(59);
             break; 
          } 
          case c_5_6:
          {
            TMCCword.reset(57);
            TMCCword.set(58);
            TMCCword.set(59);
             break; 
          } 
          case c_7_8:
          {
            TMCCword.set(57);
            TMCCword.reset(58);
            TMCCword.reset(59);
             break; 
          }
          case RESERVED:
          {
            //TMCCword.set(57);
            //TMCCword.reset(58);
            //TMCCword.reset(59);
             break; 
          }
          case UNUSED_HL:
          {
            TMCCword.set(57);
            TMCCword.set(58);
            TMCCword.set(59);
             break; 
          }
          default:
          {
            printf("Error, incorrect mode C convolutional_coding_rate \n");
            break;
          }
        }
        //Interleaving length
        if (d_ModSchemeC != UNUSED)
        {
          switch (d_mode)
          {
            case 1: 
            {
              switch (d_IntLengthC) //TODO: Revisar los IntLength que varian segun el modo
              {                    // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(60);
                  TMCCword.reset(61);
                  TMCCword.reset(62);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(60);
                  TMCCword.reset(61);
                  TMCCword.set(62);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(60);
                  TMCCword.set(61);
                  TMCCword.reset(62);
                  break;
                }
                case 16:
                {
                  TMCCword.reset(60);
                  TMCCword.set(61);
                  TMCCword.set(62);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer C (Tx mode = %d) \n", d_mode);
                  break;
                }
              }
            break;
            }
            case 2: 
            {
              switch (d_IntLengthC) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(60);
                  TMCCword.reset(61);
                  TMCCword.reset(62);
                  break;
                }
                case 2:
                {
                  TMCCword.reset(60);
                  TMCCword.reset(61);
                  TMCCword.set(62);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(60);
                  TMCCword.set(61);
                  TMCCword.reset(62);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(60);
                  TMCCword.set(61);
                  TMCCword.set(62);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer C (Tx mode = %d) \n", d_mode);
                  break;
                }
              }
            break;
            }
            case 3: 
            {
              switch (d_IntLengthC) //TODO: Revisar los IntLength que varian segun el modo
              {                     // ahora el user los tiene que poner bien desde grc
                case 0:
                {
                  TMCCword.reset(60);
                  TMCCword.reset(61);
                  TMCCword.reset(62);
                  break;
                }
                case 4:
                {
                  TMCCword.reset(60);
                  TMCCword.reset(61);
                  TMCCword.set(62);
                  break;
                }
                case 8:
                {
                  TMCCword.reset(60);
                  TMCCword.set(61);
                  TMCCword.reset(62);
                  break;
                }
                case 16:
                {
                  TMCCword.reset(60);
                  TMCCword.set(61);
                  TMCCword.set(62);
                  break;
                }
                default:
                {
                  printf("Error: Incorrect Interleaving length for layer C (Tx mode = %d) \n", d_mode);
                  break;
                }
              }
            break;
            }
            default: {
              printf("Error: Incorrect Transmission mode for layer C, mode: %d\n", d_mode);
              break; 
            }

          }
        } else {
          TMCCword.set(60);
          TMCCword.set(61);
          TMCCword.set(62);
        }
        switch (d_LayerC_seg){ //TODO: revisar (if d_LayerA_seg.test(0) == TRUE)
          case 0:
          {
            //Unused layer
            TMCCword.set(63);
            TMCCword.set(64);
            TMCCword.set(65);
            TMCCword.set(66);
            break;
          }
          case 1:
          {
            TMCCword.reset(63);
            TMCCword.reset(64);
            TMCCword.reset(65);
            TMCCword.set(66);
            break;
          }
          case 2:
          {
            TMCCword.reset(63);
            TMCCword.reset(64);
            TMCCword.set(65);
            TMCCword.reset(66);
            break;
          }
          case 3:
          {
            TMCCword.reset(63);
            TMCCword.reset(64);
            TMCCword.set(65);
            TMCCword.set(66);
            break;
          }
          case 4:
          {
            TMCCword.reset(63);
            TMCCword.set(64);
            TMCCword.reset(65);
            TMCCword.reset(66);
            break;
          }
          case 5:
          {
            TMCCword.reset(63);
            TMCCword.set(64);
            TMCCword.reset(65);
            TMCCword.set(66);
            break;
          }
          case 6:
          {
            TMCCword.reset(63);
            TMCCword.set(64);
            TMCCword.set(65);
            TMCCword.reset(66);
            break;
          }
          case 7:
          {
            TMCCword.reset(63);
            TMCCword.set(64);
            TMCCword.set(65);
            TMCCword.set(66);
            break;
          }
          case 8:
          {
            TMCCword.set(63);
            TMCCword.reset(64);
            TMCCword.reset(65);
            TMCCword.reset(66);
            break;
          }
          case 9:
          {
            TMCCword.set(63);
            TMCCword.reset(64);
            TMCCword.reset(65);
            TMCCword.set(66);
            break;
          }
          case 10:
          {
            TMCCword.set(63);
            TMCCword.reset(64);
            TMCCword.set(65);
            TMCCword.reset(66);
            break;
          }
          case 11:
          {
            TMCCword.set(63);
            TMCCword.reset(64);
            TMCCword.set(65);
            TMCCword.set(66);
            break;
          }
          case 12:
          {
            TMCCword.set(63);
            TMCCword.set(64);
            TMCCword.reset(65);
            TMCCword.reset(66);
            break;
          }
          case 13:
          {
            TMCCword.set(63);
            TMCCword.set(64);
            TMCCword.reset(65);
            TMCCword.set(66);
            break;
          }
          default:
          {
            printf("Error: Incorrect segment number for layer C");
            break;
          }
        } 
        //NEXT INFORMATION b67 - b106
        //Unused in this example
        for (int i = 67; i < 107; i++)
        {
          if (TMCCword.test(i)){
            TMCCword.set(i);
          } else {
          TMCCword.reset(i);
          }
        }
        // b107-b122 Phase shift correcction
        for (int i = 107; i < 122; i++)
        {
          TMCCword.set(i);
        }
        //Parity bits b122-b203
        for (int i = 122; i < 203; i++)
        {
          if (TMCCword.test(i)){
            TMCCword.set(i);
          } else {
          TMCCword.reset(i);
          }
        }
        // Return bit0
        return sp0;
      }
      //
      //General case, decision based on current bit and previous bit
      //
      current_bit = TMCCword.test(frame_counter);
      previous_bit = TMCCword.test(frame_counter - 1);
      if (!((current_bit & previous_bit) || (!current_bit & !previous_bit)))
      {
        // Send 1
        return std::complex<float>(-4.0/3.0, 0);   
      } else {
        // Send 0
        return std::complex<float>(4.0/3.0, 0);
      }
    }

    void
    ofdm_frame_structure_impl::fill_segment_mode1(gr_complex* in, gr_complex* out, int SegmentNumber)
    {
      int SegmentPos = 0;   //Segment position on transmision
      int TMCCPos = 0;      //TMCC position on transmision
      int ACpos1 = 0;       //AC1 position on Transmission
      int ACpos2 = 0;       //AC2 position on Transmission
      switch(SegmentNumber)
      {
        case 0:
        {
          SegmentPos = 6;
          TMCCPos = 49;
          ACpos1 = 35;
          ACpos2 = 79;
          break;
        }
        case 1:
        {
          SegmentPos = 5;
          TMCCPos = 47;
          ACpos1 = 74;
          ACpos2 = 100;
          break;
        }
        case 2:
        {
          SegmentPos = 7;
          TMCCPos = 31;
          ACpos1 = 76;
          ACpos2 = 97;
          break;
        }
        case 3:
        {
          SegmentPos = 4;
          TMCCPos = 44;
          ACpos1 = 20;
          ACpos2 = 40;
          break;
        }
        case 4:
        {
          SegmentPos = 8;
          TMCCPos = 83;
          ACpos1 = 4;
          ACpos2 = 89;
          break;
        }
        case 5:
        {
          SegmentPos = 3;
          TMCCPos = 86;
          ACpos1 = 11;
          ACpos2 = 101;
          break;
        }
        case 6:
        {
          SegmentPos = 9;
          TMCCPos = 61;
          ACpos1 = 40;
          ACpos2 = 89;
          break;
        }
        case 7:
        {
          SegmentPos = 2;
          TMCCPos = 17;
          ACpos1 = 61;
          ACpos2 = 100;
          break;
        }
        case 8:
        {
          SegmentPos = 10;
          TMCCPos = 85;
          ACpos1 = 8;
          ACpos2 = 64;
          break;
        }
        case 9:
        {
          SegmentPos = 1;
          TMCCPos = 25;
          ACpos1 = 53;
          ACpos2 = 83;
          break;
        }
        case 10:
        {
          SegmentPos = 11;
          TMCCPos = 101;
          ACpos1 = 7;
          ACpos2 = 89;
          break;
        }
        case 11:
        {
          SegmentPos = 0;
          TMCCPos = 70;
          ACpos1 = 10;
          ACpos2 = 28;
          break;
        }
        case 12:
        {
          SegmentPos = 12;
          TMCCPos = 23;
          ACpos1 = 98;
          ACpos2 = 101;
          break;
        }
        default:
        {
          printf("Error, incorrect segment number\n");
          break;
        }
      }
      SPindex = 0;
      InputIndex = SegmentPos*96;
      for (int j = 0; j < 108; j++) 
      {
        /*Scattered Pilot*/   
        if ((j % 12) == (3*d_carrier_pos))
        {
        out[108*SegmentPos+j] = this->write_SP(SPindex, d_mode, SegmentNumber);
        SPindex++;
        /* TMCC */
        } else if (j == TMCCPos) {
        out[108*SegmentPos+j] = this->write_TMCC(frame_counter, SegmentNumber);
        TMCCindex++;
        /* AC1 or AC2 */
        } else if ((j == ACpos1) || (j == ACpos2)) {
        out[108*SegmentPos+j] = std::complex<double>(0, 0);
        /* Fill with data*/
        } else {
        out[108*SegmentPos+j] = in[InputIndex];
        InputIndex++;
        }
      }
      return;  
    }

        void
    ofdm_frame_structure_impl::fill_segment_mode2(gr_complex* in, gr_complex* out, int SegmentNumber)
    {
      int SegmentPos = 0;   //Segment position on transmision
      int TMCCPos1 = 0;      //TMCC position on transmision
      int TMCCPos2 = 0;      //TMCC position on transmision
      int ACpos1 = 0;       //AC1 position on Transmission
      int ACpos2 = 0;       //AC2 position on Transmission
      int ACpos3 = 0;       //AC1 position on Transmission
      int ACpos4 = 0;       //AC2 position on Transmission
      switch(SegmentNumber)
      {
        case 0:
        {
          SegmentPos = 6;
          TMCCPos1 = 23;     
          TMCCPos2 = 178;     
          ACpos1 = 98;       
          ACpos2 = 101;       
          ACpos3 = 118;  
          ACpos4 = 136; 
          break;
        }
        case 1:
        {
          SegmentPos = 5;
          TMCCPos1 = 85;     
          TMCCPos2 = 209;     
          ACpos1 = 8;       
          ACpos2 = 64;       
          ACpos3 = 115;  
          ACpos4 = 197; 
          break;
        }
        case 2:
        {
          SegmentPos = 7;
          TMCCPos1 = 25;     
          TMCCPos2 = 125;     
          ACpos1 = 53;       
          ACpos2 = 83;       
          ACpos3 = 169;  
          ACpos4 = 208;
          break;
        }
        case 3:
        {
          SegmentPos = 4;
          TMCCPos1 = 83;     
          TMCCPos2 = 169;     
          ACpos1 = 4;       
          ACpos2 = 89;       
          ACpos3 = 148;  
          ACpos4 = 197;
          break;
        }
        case 4:
        {
          SegmentPos = 8;
          TMCCPos1 = 86;     
          TMCCPos2 = 152;     
          ACpos1 = 11;       
          ACpos2 = 101;       
          ACpos3 = 128;  
          ACpos4 = 148;
          break;
        }
        case 5:
        {
          SegmentPos = 3;
          TMCCPos1 = 49;     
          TMCCPos2 = 139;     
          ACpos1 = 35;       
          ACpos2 = 79;       
          ACpos3 = 184;  
          ACpos4 = 205;
          break;
        }
        case 6:
        {
          SegmentPos = 9;
          TMCCPos1 = 47;     
          TMCCPos2 = 157;     
          ACpos1 = 74;       
          ACpos2 = 100;       
          ACpos3 = 143;  
          ACpos4 = 187;
          break;
        }
        case 7:
        {
          SegmentPos = 2;
          TMCCPos1 = 44;     
          TMCCPos2 = 155;     
          ACpos1 = 20;       
          ACpos2 = 40;       
          ACpos3 = 182;  
          ACpos4 = 208;
          break;
        }
        case 8:
        {
          SegmentPos = 10;
          TMCCPos1 = 31;     
          TMCCPos2 = 191;     
          ACpos1 = 76;       
          ACpos2 = 97;       
          ACpos3 = 112;  
          ACpos4 = 197;
          break;
        }
        case 9:
        {
          SegmentPos = 1;
          TMCCPos1 = 17;     
          TMCCPos2 = 194;     
          ACpos1 = 61;       
          ACpos2 = 100;       
          ACpos3 = 119;  
          ACpos4 = 209;
          break;
        }
        case 10:
        {
          SegmentPos = 11;
          TMCCPos1 = 61;     
          TMCCPos2 = 193;     
          ACpos1 = 40;       
          ACpos2 = 89;       
          ACpos3 = 116;  
          ACpos4 = 172;
          break;
        }
        case 11:
        {
          SegmentPos = 0;
          TMCCPos1 = 70;     
          TMCCPos2 = 133;     
          ACpos1 = 10;       
          ACpos2 = 28;       
          ACpos3 = 161;  
          ACpos4 = 191;
          break;
        }
        case 12:
        {
          SegmentPos = 12;
          TMCCPos1 = 101;     
          TMCCPos2 = 131;     
          ACpos1 = 7;       
          ACpos2 = 89;       
          ACpos3 = 206;  
          ACpos4 = 209;
          break;
        }
        default:
        {
          printf("Error, incorrect segment number\n");
          break;
        }
      }
      SPindex = 0;
      InputIndex = SegmentPos*96;
      for (int j = 0; j < 108; j++) 
      {
        /*Scattered Pilot*/   
        if ((j % 12) == (3*d_carrier_pos))
        {
        out[108*SegmentPos+j] = this->write_SP(SPindex, d_mode, SegmentNumber);
        SPindex++;
        /* TMCC 1 and 2*/
        } else if ((j == TMCCPos1) || (j == TMCCPos2)) {
        out[108*SegmentPos+j] = this->write_TMCC(frame_counter, SegmentNumber);
        TMCCindex++;
        /* AC1, AC2, AC3 or AC4 */
        } else if ((j == ACpos1) || (j == ACpos2) || (j == ACpos3) || (j == ACpos4)) {
        out[108*SegmentPos+j] = std::complex<double>(0, 0);
        /* Fill with data*/
        } else {
        out[108*SegmentPos+j] = in[InputIndex];
        InputIndex++;
        }
      }
      return;  
    }

        void
    ofdm_frame_structure_impl::fill_segment_mode3(gr_complex* in, gr_complex* out, int SegmentNumber)
    {
      int SegmentPos = 0;   //Segment position on transmision
      int TMCCPos1 = 0;      //TMCC position on transmision
      int TMCCPos2 = 0;      //TMCC position on transmision
      int TMCCPos3 = 0;      //TMCC position on transmision
      int TMCCPos4 = 0;      //TMCC position on transmision
      int ACpos1 = 0;       //AC1 position on Transmission
      int ACpos2 = 0;       //AC2 position on Transmission
      int ACpos3 = 0;       //AC3 position on Transmission
      int ACpos4 = 0;       //AC4 position on Transmission
      int ACpos5 = 0;       //AC5 position on Transmission
      int ACpos6 = 0;       //AC6 position on Transmission
      int ACpos7 = 0;       //AC7 position on Transmission
      int ACpos8 = 0;       //AC8 position on Transmission
      switch(SegmentNumber)
      {
        case 0:
        {
          SegmentPos = 6;
          TMCCPos1 = 101;   
          TMCCPos2 = 131;
          TMCCPos3 = 286;     
          TMCCPos4 = 349;      
          ACpos1 = 7;       
          ACpos2 = 89;       
          ACpos3 = 206;  
          ACpos4 = 209;
          ACpos5 = 226;       
          ACpos6 = 244;       
          ACpos7 = 377;  
          ACpos8 = 407; 
          break;
        }
        case 1:
        {
          SegmentPos = 5;
          TMCCPos1 = 31;     
          TMCCPos2 = 191;
          TMCCPos3 = 277;     
          TMCCPos4 = 409;      
          ACpos1 = 76;       
          ACpos2 = 97;       
          ACpos3 = 112;  
          ACpos4 = 197;
          ACpos5 = 256;       
          ACpos6 = 305;       
          ACpos7 = 332;  
          ACpos8 = 388; 
          break;
        }
        case 2:
        {
          SegmentPos = 7;
          TMCCPos1 = 17;     
          TMCCPos2 = 194;
          TMCCPos3 = 260;     
          TMCCPos4 = 371;      
          ACpos1 = 61;       
          ACpos2 = 100;       
          ACpos3 = 119;  
          ACpos4 = 209;
          ACpos5 = 236;       
          ACpos6 = 256;       
          ACpos7 = 398;  
          ACpos8 = 424; 
          break;
        }
        case 3:
        {
          SegmentPos = 4;
          TMCCPos1 = 86;     
          TMCCPos2 = 152;
          TMCCPos3 = 263;     
          TMCCPos4 = 373;      
          ACpos1 = 11;       
          ACpos2 = 101;       
          ACpos3 = 128;  
          ACpos4 = 148;
          ACpos5 = 290;       
          ACpos6 = 316;       
          ACpos7 = 359;  
          ACpos8 = 403; 
          break;
        }
        case 4:
        {
          SegmentPos = 8;
          TMCCPos1 = 49;     
          TMCCPos2 = 139;
          TMCCPos3 = 299;     
          TMCCPos4 = 385;      
          ACpos1 = 35;       
          ACpos2 = 79;       
          ACpos3 = 184;  
          ACpos4 = 205;
          ACpos5 = 220;       
          ACpos6 = 305;       
          ACpos7 = 364;  
          ACpos8 = 413; 
          break;
        }
        case 5:
        {
          SegmentPos = 3;
          TMCCPos1 = 23;     
          TMCCPos2 = 178;
          TMCCPos3 = 241;     
          TMCCPos4 = 341;      
          ACpos1 = 98;       
          ACpos2 = 101;       
          ACpos3 = 118;  
          ACpos4 = 136;
          ACpos5 = 269;       
          ACpos6 = 299;       
          ACpos7 = 385;  
          ACpos8 = 424; 
          break;
        }
        case 6:
        {
          SegmentPos = 9;
          TMCCPos1 = 85;     
          TMCCPos2 = 209;
          TMCCPos3 = 239;     
          TMCCPos4 = 394;      
          ACpos1 = 8;       
          ACpos2 = 64;       
          ACpos3 = 115;  
          ACpos4 = 197;
          ACpos5 = 314;       
          ACpos6 = 317;       
          ACpos7 = 334;  
          ACpos8 = 352; 
          break;
        }
        case 7:
        {
          SegmentPos = 2;
          TMCCPos1 = 83;     
          TMCCPos2 = 169;
          TMCCPos3 = 301;     
          TMCCPos4 = 425;      
          ACpos1 = 4;       
          ACpos2 = 89;       
          ACpos3 = 148;  
          ACpos4 = 197;
          ACpos5 = 224;       
          ACpos6 = 280;       
          ACpos7 = 331;  
          ACpos8 = 413; 
          break;
        }
        case 8:
        {
          SegmentPos = 10;
          TMCCPos1 = 25;     
          TMCCPos2 = 125;
          TMCCPos3 = 302;     
          TMCCPos4 = 368;      
          ACpos1 = 53;       
          ACpos2 = 83;       
          ACpos3 = 169;  
          ACpos4 = 208;
          ACpos5 = 227;       
          ACpos6 = 317;       
          ACpos7 = 344;  
          ACpos8 = 364; 
          break;
        }
        case 9:
        {
          SegmentPos = 1;
          TMCCPos1 = 44;     
          TMCCPos2 = 155;
          TMCCPos3 = 265;     
          TMCCPos4 = 355;      
          ACpos1 = 20;       
          ACpos2 = 40;       
          ACpos3 = 182;  
          ACpos4 = 208;
          ACpos5 = 251;       
          ACpos6 = 295;       
          ACpos7 = 400;  
          ACpos8 = 421; 
          break;
        }
        case 10:
        {
          SegmentPos = 11;
          TMCCPos1 = 47;     
          TMCCPos2 = 157;
          TMCCPos3 = 247;     
          TMCCPos4 = 407;      
          ACpos1 = 74;       
          ACpos2 = 100;       
          ACpos3 = 143;  
          ACpos4 = 187;
          ACpos5 = 292;       
          ACpos6 = 313;       
          ACpos7 = 328;  
          ACpos8 = 413; 
          break;
        }
        case 11:
        {
          SegmentPos = 0;
          TMCCPos1 = 70;     
          TMCCPos2 = 133;
          TMCCPos3 = 233;     
          TMCCPos4 = 410;      
          ACpos1 = 10;       
          ACpos2 = 28;       
          ACpos3 = 161;  
          ACpos4 = 191;
          ACpos5 = 277;       
          ACpos6 = 316;       
          ACpos7 = 335;  
          ACpos8 = 425; 
          break;
        }
        case 12:
        {
          SegmentPos = 12;
          TMCCPos1 = 61;     
          TMCCPos2 = 193;
          TMCCPos3 = 317;     
          TMCCPos4 = 347;      
          ACpos1 = 40;       
          ACpos2 = 89;       
          ACpos3 = 116;  
          ACpos4 = 172;
          ACpos5 = 223;       
          ACpos6 = 305;       
          ACpos7 = 422;  
          ACpos8 = 425; 
          break;
        }
        default:
        {
          printf("Error, incorrect segment number\n");
          break;
        }
      }
      SPindex = 0;
      InputIndex = SegmentPos*96;
      for (int j = 0; j < 108; j++) 
      {
        /*Scattered Pilot*/   
        if ((j % 12) == (3*d_carrier_pos))
        {
        out[108*SegmentPos+j] = this->write_SP(SPindex, d_mode, SegmentNumber);
        SPindex++;
        /* TMCC 1 and 2*/
        } else if ((j == TMCCPos1) || (j == TMCCPos2) || (j == TMCCPos3) || (j == TMCCPos4)) {
        out[108*SegmentPos+j] = this->write_TMCC(frame_counter, SegmentNumber);
        TMCCindex++;
        /* AC1, AC2, AC3 or AC4 */
        } else if ((j == ACpos1) || (j == ACpos2) || (j == ACpos3) || (j == ACpos4) || (j == ACpos5) || (j == ACpos6) || (j == ACpos7) || (j == ACpos8)) {
        out[108*SegmentPos+j] = std::complex<double>(0, 0);
        /* Fill with data*/
        } else {
        out[108*SegmentPos+j] = in[InputIndex];
        InputIndex++;
        }
      }
      return;  
    }

    void
    ofdm_frame_structure_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    ofdm_frame_structure_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
    gr_complex *in = (gr_complex *) input_items[0];
    gr_complex *out = (gr_complex *) output_items[0];

    for (int i = 0; i < noutput_items ; i++) 
    {
      if (frame_counter == 0)
      {
        //First symbol of OFDM structure
        TMCCindex = 0;      //TMCC word position across symbol
      }
      switch (d_mode)
        {
        case 1:
          /* Segment 0*/
          this->fill_segment_mode1(in, out, 0);
          /*Segments 1 to 12*/
          if (!d_IsOneSeg)
          {
            for (int k=1; k<13;k++)
            {
              this->fill_segment_mode1(in, out, k);
            }
          }
          break;
        case 2:
          /* Segment 0*/
          this->fill_segment_mode2(in, out, 0);
          /*Segments 1 to 12*/
          if (!d_IsOneSeg)
          {
            for (int k=1; k<13;k++)
            {
              this->fill_segment_mode2(in, out, k);
            }
          }
          break;
        case 3:
          /* Segment 0*/
          this->fill_segment_mode3(in, out, 0);
          /*Segments 1 to 12*/
          if (!d_IsOneSeg)
          {
            for (int k=1; k<13;k++)
            {
              this->fill_segment_mode3(in, out, k);
            }
          }
          break;
        default:
          printf("Error: incorrect mode \n");
          break; 
        } 
        frame_counter++;  
        d_carrier_pos = (frame_counter % 4);
        frame_counter = (frame_counter % 204);
      }
      this->consume(0, noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */
