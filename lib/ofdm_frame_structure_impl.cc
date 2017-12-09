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
                                int IntLengthA, int IntLengthB, int IntLengthC)
    {
      return gnuradio::get_initial_sptr
        (new ofdm_frame_structure_impl(mode, IsOneSeg, 
                                        ModSchemeA, ModSchemeB, ModSchemeC, 
                                        ConvCodeA, ConvCodeB, ConvCodeC,
                                        IntLengthA, IntLengthB, IntLengthC));
    }

    /*
     * The private constructor
     */
    ofdm_frame_structure_impl::ofdm_frame_structure_impl(int mode, bool IsOneSeg, 
                                  int ModSchemeA, int ModSchemeB, int ModSchemeC, 
                                  int ConvCodeA, int ConvCodeB, int ConvCodeC,
                                  int IntLengthA, int IntLengthB, int IntLengthC)
      : gr::block("ofdm_frame_structure",
              gr::io_signature::make(1, 1, sizeof(gr_complex) * 13 * 96 * ((int)pow(2.0,mode-1))),
              gr::io_signature::make(1, 1, sizeof(gr_complex) * ((int)pow(2.0,10 + mode)))
              )
    {
      d_IsOneSeg = IsOneSeg;  /* Transmission mdoe, one seg or full seg*/
      Frame_counter = 0;      /* Counts frames*/
      d_symbol_counter = 0;  /* Counts symbols across frames */
      d_carrier_pos = 0;    /* 4 cyclic counter, scattered pilot rotation reference*/
      TMCCindex = 0;        /* TMCC word position counter, accross block */
      SPindex = 0;          /* Scattered pilot counter, accross segment */
      d_mode = mode;        /* Transmission Mode */
      sp_keyword = 0b00000000000; /* SP keyword default*/
      TMCCword.set();
      TMCC_sync_word = 0b0011010111101110;
      d_ModSchemeA = (carrier_mod_scheme) ModSchemeA;
      d_ModSchemeB = (carrier_mod_scheme) ModSchemeB;
      d_ModSchemeC = (carrier_mod_scheme) ModSchemeC;
      d_ConvCodeA  = (convolutional_coding_rate) ConvCodeA;
      d_ConvCodeB  = (convolutional_coding_rate) ConvCodeB;
      d_ConvCodeC  = (convolutional_coding_rate) ConvCodeC;
      d_IntLengthA = IntLengthA;
      d_IntLengthB = IntLengthB;
      d_IntLengthC = IntLengthC;
    }

    /*
     * Our virtual destructor.
     */
    ofdm_frame_structure_impl::~ofdm_frame_structure_impl()
    {
    }

    
    bitset<11> 
    ofdm_frame_structure_impl::ObtainStartingWord(int SegmentNumber, int d_mode)
    {
      switch(d_mode) // 
      {
      case 1 :
      {  
        sp_segment_keywords[0] = 0b11001000010;
        sp_segment_keywords[1] = 0b00101111010;
        sp_segment_keywords[2] = 0b00010000100;
        sp_segment_keywords[3] = 0b11011100101;
        sp_segment_keywords[4] = 0b10010100000;
        sp_segment_keywords[5] = 0b01000101110;
        sp_segment_keywords[6] = 0b11110110000;
        sp_segment_keywords[7] = 0b01101011110;
        sp_segment_keywords[8] = 0b00001011000;
        sp_segment_keywords[9] = 0b11011001111;
        sp_segment_keywords[10]= 0b10100100111;
        sp_segment_keywords[11]= 0b11111111111;
        sp_segment_keywords[12]= 0b01110001001;
        break;
      }       
      case 2 : 
        sp_segment_keywords[0] = 0b11011100101;
        sp_segment_keywords[1] = 0b00010011100;
        sp_segment_keywords[2] = 0b00000100100;
        sp_segment_keywords[3] = 0b10010100000;
        sp_segment_keywords[4] = 0b00100011001;
        sp_segment_keywords[5] = 0b11001000010;
        sp_segment_keywords[6] = 0b01100111001;
        sp_segment_keywords[7] = 0b11011100101;
        sp_segment_keywords[8] = 0b11100110110;
        sp_segment_keywords[9] = 0b01101011110;
        sp_segment_keywords[10]= 0b00101010001;
        sp_segment_keywords[11]= 0b11111111111;
        sp_segment_keywords[12]= 0b00100001011;
        break;
      case 3 :
        sp_segment_keywords[0] = 0b10010100000;
        sp_segment_keywords[1] = 0b11100110110;
        sp_segment_keywords[2] = 0b11100111101;
        sp_segment_keywords[3] = 0b00100011001;
        sp_segment_keywords[4] = 0b01101010011;
        sp_segment_keywords[5] = 0b01110001001;
        sp_segment_keywords[6] = 0b10111010010;
        sp_segment_keywords[7] = 0b10010100000;
        sp_segment_keywords[8] = 0b01100010010;
        sp_segment_keywords[9] = 0b11011100101;
        sp_segment_keywords[10] = 0b11110100101;
        sp_segment_keywords[11] = 0b11111111111;
        sp_segment_keywords[12] = 0b00010011100;
        break;
      default:
        printf("Error: invalid Transmission mode \n");
        break;
      }
      bitset<11> Keyword = (sp_segment_keywords[SegmentNumber]);
      return Keyword;
    }

    /*
    Writes corresponding Scattered Pilot into symbol
    */
    gr_complex 
    ofdm_frame_structure_impl::write_SP(int SPindex, int d_mode, int SegmentNumber)
    {
      if (SPindex == 0)
      {
        sp_keyword = this->ObtainStartingWord(SegmentNumber, d_mode);
      }
      if (sp_keyword.test(SPindex)) /*Return bit value in keyword for SPindex*/
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
    ofdm_frame_structure_impl::write_TMCC(int SymbolNumber, int Frame_counter, int SegmentNumber)
    {
      //printf("Dentro de write TMCC, SymbolNumber: %d \n", SymbolNumber);
      //Test TMCC word for writing bit, returns symbol mapped into DBPSK
      bool current_bit, previous_bit;
      //First bit, only decide on it
      if (SymbolNumber == 0)
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
        bool EsPar = ((Frame_counter % 2) == 0);
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
        TMCCword.set(24); //TODO: Verificar los cambios del sistema
        TMCCword.set(25);
        // Alarm bit, defaulted in 0
        TMCCword.reset(26); //TODO: fijar parametrico, desde fuera del bloque
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
        //if (capaA != UNUSED) switch mode
        if (d_ModSchemeA != UNUSED)
        {
          switch (d_mode)
          {
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
            }
            case 2: 
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
            }
            case 3: 
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
            }
            default: {
              printf("Error: Incorrect Transmission mode\n");
              break;
            }

          }
        } else {
          TMCCword.set(34);
          TMCCword.set(35);
          TMCCword.set(36);
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
            }
            default: {
              printf("Error: Incorrect Transmission mode\n");
              break;
            }

          }
        } else {
          TMCCword.set(47);
          TMCCword.set(48);
          TMCCword.set(49);
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
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
                  printf("Error: Incorrect Interleaving length for this mode\n");
                  break;
                }
              }
            }
            default: {
              printf("Error: Incorrect Transmission mode\n");
              break;
            }

          }
        } else {
          TMCCword.set(60);
          TMCCword.set(61);
          TMCCword.set(62);
        }

        //b60 - b106
        for (int i = 107; i < 122; i++)
        {
          //107-109 parity bits
          //110-121 reserved, always 1
          TMCCword.set(i);
        }
      //Bits de paridad
        //Assign b122-b203

        // Return bit0
        return sp0;
      }
      //General case, decision based on current bit and previous bit
      current_bit = TMCCword.test(SymbolNumber);
      previous_bit = TMCCword.test(SymbolNumber - 1);
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
    const gr_complex *in = (const gr_complex *) input_items[0];
    gr_complex *out = (gr_complex *) output_items[0];
    int k = 0;
    //TODO: Si IsOneSeg es TRUE, solo rellenamos el segmento 0, de lo
    // contrario, rellenamos todos menos el 0
    for (int i = 0; i < noutput_items ; i++) 
    {
        if (d_symbol_counter == 0)
        {
          //printf("Estoy en el primer simbolo del cuadro\n");
          Frame_counter++;
          TMCCindex = 0;
        }
      switch (d_mode)
        {
        case 1:
          /* Segment 0*/
          for (int j = 0; j < 96; j++) 
          {
            if ((j % 12) == (3*d_carrier_pos))
            {
              /*Scattered Pilot*/
              out[108*6+j] = this->write_SP(SPindex, d_mode, 0 /*Segment number*/);
              SPindex++;
            } else if (j == 49) {
              /* TMCC */
              //printf("Antes de llamar a writeTMCC, d_symbol_counter %d\n", d_symbol_counter);
              out[108*6+j] = this->write_TMCC(d_symbol_counter, Frame_counter, 0);
              TMCCindex++;
              //printf("TMCCindex: %d \n", TMCCindex);
              //printf("out[TMCC]: (%f,%f) \n", out[108*6+j].real(), out[108*6+j].imag());
            } else if ((j == 35) || (j == 79)) {
              /* AC1 or AC2 */
              out[108*6+j] = std::complex<double>(0, 0);
            } else {
              /* Fill with data*/
              out[108*6+j] = in[k];
              k++;
            }
          }
          SPindex = 0;
          break;
        case 2:
          printf("Mode 2 \n");
          for (int j = 0; j < 4096; j++)
          {
          out[j] = std::complex<double>(100, 200);
          }
          break;
        case 3:
          printf("Mode 3 \n");
          for (int j = 0; j < 8192; j++)
          {
          out[j] = std::complex<double>(100, 200);
          }
          break;
        default:
          printf("Error: incorrect mode \n");
          break; 
        } 
        d_symbol_counter++;  
        d_carrier_pos = (d_symbol_counter % 4);
        d_symbol_counter = (d_symbol_counter % 204);
      }
      this->consume(0, noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

