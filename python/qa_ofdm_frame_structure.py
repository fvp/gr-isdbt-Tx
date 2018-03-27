#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2017 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import isdbt_swig as isdbt
import cmath
import numpy
import operator

####################### GENERAL COMMENTS #######################
# - prefix aux_ denotes auxiliary functions for the tests
# - prefix test_ denotes functions where the test are processed
# - 



class qa_ofdm_frame (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None
    
    def aux_get_SP_index (self, sp_to_test, n_symbols, mode, n_seg):
        '''Returns a vector with the absolute SP index over the OFDM symbol (THIS NOT INCLUDES ANY ZERO-PADDING!).
        :param: sp_to_test  number of the SP that we desire to be tested. Relative to the segment. Example: with 0, the functions returns the index of the firsts SP for each segment for the n_symbols
        :param: n_symbols   number of symbols OFDM, ussualy 204 but may be less if we want another kind of test
        :param: mode        transmission mode
        :param: n_seg       number of segments
        '''
        segment_size = (48*pow(2, mode)+12*pow(2, mode-1)) # number of carriers of a segment including pilot carriers
        sp_index = ()
        for i in range(0, n_symbols):
            k = i*segment_size*n_seg
            sp_index = sp_index + tuple(range(sp_to_test + (i%4)*3 + k, 12*segment_size+1+9+k, segment_size)) # Index of the first carriers per segment per symbol

        return sp_index

# Defines some auxiliary functions to make the tests
    global carrier_mod_scheme, conv_coding_rate, interleaving_length, number_of_segments, size_by_mode
    carrier_mod_scheme = {"DQPSK": 0, "QPSK": 1, "16QAM": 2, "64QAM": 3, "UNUSED": 4}
    conv_coding_rate = {"1/2": 0, "2/3": 1, "3/4": 2, "5/6": 3, "7/8": 4, "RESERVED": 5, "UNUSED": 6}
    interleaving_length = {"0": 0, "1": 1, "2": 2, "4": 4, "8": 8, "16": 16}
    number_of_segments = {"RESERVED0": 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 6: 6, 7: 7, 8: 8, 9: 9, 10: 10, 11: 11, 12: 12, 13: 13, "RESERVED14": 14, "UNUSED": 15}
    size_by_mode = {1: 96, 2: 192, 3: 384}
	
    def aux_get_TMCC_index (self, n_symbols, mode, n_seg):
        """Returns a vector (various if mode > 1) with the TMCC indexs"""
        # los tmcc_index_mode son vectores cuyas entradas son el indice de TMCC absoluto en los 13 segmentos
        segment_size = (48*pow(2, mode)+12*pow(2, mode-1))

        if mode == 1:
            tmcc_index_mode1 = ()
            # CREO QUE NO ES SIZE[MODE] PORQUE SERIA 96 EN VEZ DE 108
            for i in range(0, n_symbols):
                k = i*n_seg*segment_size
                tmcc_index_mode1 = tmcc_index_mode1 + (70 + k, 25+1*108 + k, 17+2*108 + k, 86+3*108 + k, 44+4*108 + k, 47+5*108 + k, 49+6*108 + k, 31+7*108 + k, 83+8*108 + k, 61+9*108 + k, 85+10*108 + k, 101+11*108 + k, 23+12*108 + k)
            return tmcc_index_mode1

        if mode == 2:
            tmcc1_index_mode2 = ()
            tmcc2_index_mode2 = ()
            
            for i in range(0, n_symbols):
                k = i*n_seg*segment_size
                tmcc1_index_mode2 = tmcc1_index_mode2 + (70 + k, 17+1*216 + k, 44+2*216 + k, 49+3*216 + k, 83+4*216 + k, 85+5*216 + k, 23+6*216 + k, 25+7*216 + k, 86+8*216 + k, 47+9*216 + k, 31+10*216 + k, 61+11*216 + k, 101+12*216 + k)
                tmcc2_index_mode2 = tmcc2_index_mode2 + (133 + k, 194+1*216 + k, 155+2*216 + k, 139+3*216 + k, 169+4*216 + k, 209+5*216 + k, 178+6*216 + k, 125+7*216 + k, 152+8*216 + k, 157+9*216 + k, 191+10*216 + k, 193+11*216 + k, 131+12*216 + k)
            return tmcc1_index_mode2 + tmcc2_index_mode2

        if mode == 3:
            tmcc1_index_mode3 = ()
            tmcc2_index_mode3 = ()
            tmcc3_index_mode3 = ()
            tmcc4_index_mode3 = ()

            for i in range(0, n_symbols):
                k = i*n_seg*segment_size
                tmcc1_index_mode3 = tmcc1_index_mode3 + (70 + k, 44+1*432 + k, 83+2*432 + k, 23+3*432 + k, 86+4*432 + k, 31+5*432 + k, 101+6*432 + k, 17+7*432 + k, 49+8*432 + k, 85+9*432 + k, 25+10*432 + k, 47+11*432 + k, 61+12*432 + k)
                tmcc2_index_mode3 = tmcc2_index_mode3 + (133 + k, 155+1*432 + k, 169+2*432 + k, 178+3*432 + k, 152+4*432 + k, 191+5*432 + k, 131+6*432 + k, 194+7*432 + k, 139+8*432 + k, 209+9*432 + k, 125+10*432 + k, 157+11*432 + k, 193+12*432 + k)
                tmcc3_index_mode3 = tmcc3_index_mode3 + (233 + k, 265+1*432 + k, 301+2*432 + k, 241+3*432 + k, 263+4*432 + k, 277+5*432 + k, 286+6*432 + k, 260+7*432 + k, 299+8*432 + k, 239+9*432 + k, 302+10*432 + k, 247+11*432 + k, 317+12*432 + k)
                tmcc4_index_mode3 = tmcc4_index_mode3 + (410 + k, 355+1*432 + k, 425+2*432 + k, 341+2*432 + k, 373+4*432 + k, 409+5*432 + k, 349+6*432 + k, 371+7*432 + k, 385+8*432 + k, 394+9*432 + k, 368+10*432 + k, 407+11*432 + k, 347+12*432 + k)
            return tmcc1_index_mode3 + tmcc2_index_mode3 + tmcc3_index_mode3 + tmcc4_index_mode3



    def aux_get_AC_index (self, n_symbols, mode, n_seg):
        """Returns the AC index"""
        segment_size = (48*pow(2, mode)+12*pow(2, mode-1))

        if mode == 1:
            ac1_index_mode1 = ()
            ac2_index_mode1 = ()

            for i in range(0, n_symbols):
                k = i*n_seg*segment_size
                ac1_index_mode1 = ac1_index_mode1 + (10 + k, 53+1*108 + k, 61+2*108 + k, 11+3*108 + k, 20+4*108 + k, 74+5*108 + k, 35+6*108 + k, 76+7*108 + k, 4+8*108 + k, 40+9*108 + k, 8+10*108 + k, 7+11*108 + k, 98+12*108 + k)
                ac2_index_mode1 = ac2_index_mode1 + (28 + k, 83+1*108 + k, 100+2*108 + k, 101+3*108 + k, 40+4*108 + k, 100+5*108 + k, 79+6*108 + k, 97+7*108 + k, 89+8*108 + k, 89+9*108 + k, 64+10*108 + k, 89+11*108 + k, 101+12*108 + k)
            return ac1_index_mode1 + ac2_index_mode1

        if mode == 2:
            ac1_index_mode2 = ()
            ac2_index_mode2 = ()
            ac3_index_mode2 = ()
            ac4_index_mode2 = ()

            for i in range(0, n_symbols):
                k = i*n_seg*segment_size
                ac1_index_mode2 = ac1_index_mode2 + (10 + k, 61+1*216 + k, 20+2*216 + k, 35+3*216 + k, 4+4*216 + k, 8+5*216 + k, 98+6*216 + k, 53+7*216 + k, 11+8*216 + k, 74+9*216 + k, 76+10*216 + k, 40+11*216 + k, 7+12*216 + k)
                ac2_index_mode2 = ac2_index_mode2 + (28 + k, 100+1*216 + k, 40+2*216 + k, 79+3*216 + k, 89+4*216 + k, 64+5*216 + k, 101+6*216 + k, 83+7*216 + k, 101+8*216 + k, 100+9*216 + k, 97+10*216 + k, 89+11*216 + k, 89+12*216 + k)
                ac3_index_mode2 = ac3_index_mode2 + (161 + k, 119+1*216 + k, 182+2*216 + k, 184+3*216 + k, 148+4*216 + k, 115+5*216 + k, 118+6*216 + k, 169+7*216 + k, 128+8*216 + k, 143+9*216 + k, 112+10*216 + k, 116+11*216 + k, 206+12*216 + k)
                ac4_index_mode2 = ac4_index_mode2 + (191 + k, 209+1*216 + k, 208+2*216 + k, 205+3*216 + k, 197+4*216 + k, 197+5*216 + k, 136+6*216 + k, 208+7*216 + k, 148+8*216 + k, 187+9*216 + k, 197+10*216 + k, 172+11*216 + k, 209+12*216 + k)
            return ac1_index_mode2 + ac2_index_mode2 + ac3_index_mode2 + ac4_index_mode2

        if mode == 3:
            ac1_index_mode3 = ()
            ac2_index_mode3 = ()
            ac3_index_mode3 = ()
            ac4_index_mode3 = ()
            ac5_index_mode3 = ()
            ac6_index_mode3 = ()
            ac7_index_mode3 = ()
            ac8_index_mode3 = ()

            for i in range(0, n_symbols):
                k = i*n_seg*segment_size
                ac1_index_mode3 = ac1_index_mode3 + (10 + k, 20+1*432 + k, 4+2*432 + k, 98+3*432 + k, 11+4*432 + k, 76+5*432 + k, 7+6*432 + k, 61+7*432 + k, 35+8*432 + k, 8+9*432 + k, 53+10*432 + k, 74+11*432 + k, 40+12*432 + k)
                ac2_index_mode3 = ac2_index_mode3 + (28 + k, 40+1*432 + k, 89+2*432 + k, 101+3*432 + k, 101+4*432 + k, 97+5*432 + k, 89+6*432 + k, 100+7*432 + k, 79+8*432 + k, 64+9*432 + k, 83+10*432 + k, 100+11*432 + k, 89+12*432 + k)
                ac3_index_mode3 = ac3_index_mode3 + (161 + k, 182+1*432 + k, 148+2*432 + k, 118+3*432 + k, 128+4*432 + k, 112+5*432 + k, 206+6*432 + k, 119+7*432 + k, 184+8*432 + k, 115+9*432 + k, 169+10*432 + k, 143+11*432 + k, 116+12*432 + k)
                ac4_index_mode3 = ac4_index_mode3 + (191 + k, 208+1*432 + k, 197+2*432 + k, 136+3*432 + k, 148+4*432 + k, 197+5*432 + k, 209+6*432 + k, 209+7*432 + k, 205+8*432 + k, 197+9*432 + k, 208+10*432 + k, 187+11*432 + k, 172+12*432 + k)
                ac5_index_mode3 = ac5_index_mode3 + (277 + k, 251+1*432 + k, 224+2*432 + k, 269+3*432 + k, 290+4*432 + k, 256+5*432 + k, 226+6*432 + k, 236+7*432 + k, 220+8*432 + k, 314+9*432 + k, 227+10*432 + k, 292+11*432 + k, 223+12*432 + k)
                ac6_index_mode3 = ac6_index_mode3 + (316 + k, 295+1*432 + k, 280+2*432 + k, 299+3*432 + k, 316+4*432 + k, 305+5*432 + k, 244+6*432 + k, 256+7*432 + k, 305+8*432 + k, 317+9*432 + k, 317+10*432 + k, 313+11*432 + k, 305+12*432 + k)
                ac7_index_mode3 = ac7_index_mode3 + (335 + k, 400+1*432 + k, 331+2*432 + k, 385+3*432 + k, 359+4*432 + k, 332+5*432 + k, 377+6*432 + k, 398+7*432 + k, 364+8*432 + k, 334+9*432 + k, 344+10*432 + k, 328+11*432 + k, 422+12*432 + k)
                ac8_index_mode3 = ac8_index_mode3 + (425 + k, 421+1*432 + k, 413+2*432 + k, 424+3*432 + k, 403+4*432 + k, 388+5*432 + k, 407+6*432 + k, 424+7*432 + k, 413+8*432 + k, 352+9*432 + k, 364+10*432 + k, 413+11*432 + k, 425+12*432 + k)
            return ac1_index_mode3 + ac2_index_mode3 + ac3_index_mode3 + ac4_index_mode3 + ac5_index_mode3 + ac6_index_mode3 + ac7_index_mode3 + ac8_index_mode3




    # def aux_expected_data (data_source, mode):

    #     j = 0
    #     for i in range(pow(2,10+mode)):
    #         # This loop generates the data in the expected result vector
    #         if i is not (sp_index + tmcc_index + ac_index):
    #             data_out[j] = data_source[i]
    #             j += 1
    #     return data_out



    def aux_remove_zero_padding (self, ofdm_frame, n_symbols, mode):
        '''Auxiliary function that removes the zero padding. (i.e. in a given
        OFDM frame of (108*13+644)*204 carriers (mode 1) will remove the carriers that corresponds
        to the zero-padding

        :param tuple:   ofdm_frame expects the 204 symbol-OFDM Frame with zero-padding included
        :param int:     mode expects the transmission mode (1,2,3)'''

        #TODO: CONSIDERAR EL PILOTO CONTINUO A LA DERECHA DEL ESPECTRO

        # Idea: consider the tuple formed as |322-zeros||DATA||322-zeros||322-zeros||DATA||322-zeros| ....

        size = 13*(48*pow(2, mode)+12*pow(2, mode-1))
        zero_pad_size = (pow(2, 10 + mode) - size)/2         # 644/2 for MODE 1, etc...

        out = []
        source = list(ofdm_frame)

        for i in range(0,n_symbols):
            one_ofdm_sym = source[i*pow(2,10+mode):(i+1)*pow(2,10+mode)]     # one ofdm symbol with zero padding
            del one_ofdm_sym[0:zero_pad_size]
            del one_ofdm_sym[size:]
            out = out + one_ofdm_sym
        return out


    def aux_get_data (self, ofdm_frame, mode):
        '''
        Extracts the data from the OFDM frame produced ignoring the TMCC, SP, AC
        
        :param tuple:       ofdm_frame expects the 13 OFDM frames including the zero padding carriers (i.e. the size of ofdm_frame is 204*2^(10+mode))
        :param int mode:    transmission mode
        ''' 
        size = 13*(48*pow(2, mode)+12*pow(2, mode-1))
        data_result = []
        sp_index = self.aux_get_SP_index(mode)
        tmcc_index_one_sym = self.aux_get_TMCC_index(mode) # TMCC index for 1 OFDM Symbol
        ac_index_one_sym = self.aux_get_AC_index(mode)
        
        # we must conform a tuple with the TMCC and AC index for the 204 symbols
        tmcc_index = ()
        ac_index = ()
        for k in range(0, 204):
            tmcc_index = tmcc_index + tuple([x+(k*size) for x in tmcc_index_one_sym])
            ac_index = ac_index + tuple([y+(k*size) for y in ac_index_one_sym])
        
        # Removes the zero padding carriers to work with the OFDM Frame of 204 symbols of data
        ofdm_frame_without_zp = self.aux_remove_zero_padding(ofdm_frame, mode)


        pilots = sp_index + tmcc_index + ac_index
        
        n = 0                       # this n is because when remove one item from the list, the index are shifted 1
        sort_pilots = sorted(pilots)
        
        # Remove the AC, TMCC, SP
        for i in sort_pilots:    
            del ofdm_frame_without_zp[i-n]
            n += 1

        data_result = ofdm_frame_without_zp
        return data_result




    def aux_tplayer_info (self, mode, param_layer):
        """Takes the parameters for some layer and .,..
        :param param_layer: tuple with the parameters
        """
        expected_param_layer = numpy.array([0]*13)
        
        # Carrier modulation scheme
        #carrier_mod_scheme = {"DQPSK": 0, "QPSK": 1, "16QAM": 2, "64QAM": 3, "RESERVED4":4, "RESERVED5": 5, "RESERVED6": 6, "UNUSED": 7}
        binary_repr = numpy.binary_repr(carrier_mod_scheme[param_layer[0]])
        expected_param_layer[[range(0,4)]] = numpy.array(list(binary_repr), dtype=int)

        # Convolutional Coding rate
        #conv_coding_rate = {"1/2": 0, "2/3": 1, "3/4": 2, "5/6": 3, "RESERVED4":4, "RESERVED5": 5, "RESERVED6": 6, "UNUSED": 7}
        binary_repr = numpy.binary_repr(conv_coding_rate[param_layer[1]])
        expected_param_layer[[range(4,7)]] = numpy.array(list(binary_repr), dtype=int)

        # Interleaving length
        #interleaving_length = {(0,1): 0, (0,2): 0, (0,3): 0, (4,1): 1, (2,2): 1, (1,3): 1, (8,1): 2, (4,2): 2, (2,3): 2, (16,1): 3, (8,2): 3, (4,3): 3, "RESERVED4":4, "RESERVED5": 5, "RESERVED6": 6, "UNUSED": 7}
        binary_repr = numpy.binary_repr(interleaving_length[param_layer[2]])
        expected_param_layer[[range(7,10)]] = numpy.array(list(binary_repr), dtype=int)

        # Number of segments
        #number_of_segments = {"RESERVED0": 0, 1: 1, 2: 2, 3: 3, 4: 4, 5: 5, 6: 6, 7: 7, 8: 8, 9: 9, 10: 10, 11: 11, 12: 12, 13: 13, "RESERVED14": 14, "UNUSED": 15}
        if (param_layer[3] == 0):
            binary_repr = (0,0,0,0)
        else:
            binary_repr = numpy.binary_repr(number_of_segments[param_layer[3]])

        expected_param_layer[[range(10,13)]] = numpy.array(list(binary_repr), dtype=int)

        # expected_param_layer = (carrier_mod_scheme, conv_coding_rate, interleaving_length, nsegments)
        return expected_param_layer




    def aux_expected_tmcc (self, data_source, mode, param_layer_A, param_layer_B, param_layer_C):
        """Size 204"""
        tmcc = numpy.array([0]*204)

        # 0 - Reference for differential demodulation
        tmcc[0] = 0
        
        # 1-16 Synchronizing signal. The alrternation test between w0 and w1, will be made in test_tmcc_ofdm_frame
        w0 = numpy.array([0,0,1,1,0,1,0,1,1,1,1,0,1,1,1,0])
        tmcc[[range(1,17)]] = w0

        # 17-19 Segment type identification: 000 for synchronous
        tmcc[[17,18,19]] = 0

        # 20-21 System identification: 00 for this standard ARIB STD – B31
        tmcc[[20,21]] = 0

        # 22-25 Indicator of transmission parameter switching: 1111 Normal value.
        tmcc[[range(22,26)]] = 1

        # 26 Emergency flag: 0 No startup control
        tmcc[26] = 0

        # CURRENT INFORMATION
        # 27 Partial reception flag: 1 Partial reception available
        tmcc[27] = 1

        # Transmis0sion parameter information for LAYER A
        tmcc[[range(28,41)]] = self.aux_tplayer_info(mode, param_layer_A)

        # Transmission parameter information for LAYER B
        tmcc[[range(41,54)]] = self.aux_tplayer_info(mode, param_layer_B)

        # Transmission parameter information for LAYER C
        tmcc[[range(54,67)]] = self.aux_tplayer_info(mode, param_layer_C)

        # NEXT INFORMATION
        # Assume that the system parameters not changes
        tmcc[[range(67,107)]] = tmcc[[range(27,67)]]

        # 107- 109 Phase shift correction value for connected segment transmission (1 for all bits)
        tmcc[[range(107,110)]] = 1

        # 110-121 Reserved bits (1 for all)
        tmcc[[range(110, 122)]] = 1

        return tmcc




    def aux_expected_ac (self):
        """Sets the Auxiliary Channel expected (this is optional, so we fill with zeros)"""

        expected_ac = numpy.array([0]*204) 
        return expected_ac





    def open_file_and_load_to_list (self, file_path):
        """Opens a file source and load its data on a list"""
        
        # Se asume que el archivo tiene la siguiente forma:
        # [S1] 0 1 2 3 4 ..... 108 0 1 2 3 .... 108 1 2 3 ... 108 
        # [S"] .
        # [S2] .
        # [Sk] .
        # [S204] .
        # 
        # En data_source se devuelve una lista (array) de 1x(108*13*204) con el orden: [S1, S2, S3 .... S204]

        #os.path.join(os.path.expanduser('~'), 'documents', 'python', 'file.txt')
        
        with open(file_path, 'r') as f:     # usar with open tiene la ventaja de que automaticamente se cierra el file
            lines = f.read().split()   # devuelve una lista del tipo ['valor1' 'valor2' ....]
                
        data_source = [float(i) for i in lines] # "converts" the ASCII symbols to their respective floats
        return data_source
    
        # TODO:
        # PENSAR MAS TESTS

    def bit2bpsk (self, bit):
        '''Maps a bit to a BPSK value. 
        1 is mapped to the complex -4/3 + 0j
        otherwise is mapped to +4/3 + 0j

        :param bit: 1 or 0, is modulated into BPSK
        '''
        return complex(-4.0/3.0, 0) if (bit == 1) else complex(4.0/3.0, 0)


# Now, we start with the tests

    # def test_sp_ofdm_frame (self):
    #     '''Test only the SP values for the first carrier of each segment.'''
        
    #     mode = 1
    #     n_file_source_segments = 13
    #     fullseg = True # 0 - False, is not one seg
    #     param_layer_A = ("16QAM", "3/4", "8", 6)
    #     param_layer_B = ("QPSK", "1/2", "16", 7)
    #     param_layer_C = ("UNUSED", "UNUSED", "0", 0)
        
    #     carriers_number = pow(2,10+mode) # number of carriers at the "OFDM Frame Structure" block output

    #     #file_path = "/home/santiago/file_source_mode1_zeros.txt"


    #     data_source = [0,0]*(size_by_mode[mode]*13)  # creates a data source vector
    #     n_symbols = 2 # must be consistent with the data source

    #     #data_source = self.open_file_and_load_to_list(file_path)
    #     k = 0       # This variable is used to set what SP we want test, For example n=0 test the first SP on each segment, 
    #     sp_index = self.aux_get_SP_index(k, n_symbols, mode, n_file_source_segments)


    #     expected_sp = [self.bit2bpsk(1), self.bit2bpsk(1), self.bit2bpsk(0), self.bit2bpsk(0), self.bit2bpsk(1), self.bit2bpsk(0), self.bit2bpsk(0),
    #                     self.bit2bpsk(0), self.bit2bpsk(0), self.bit2bpsk(0), self.bit2bpsk(0), self.bit2bpsk(1), self.bit2bpsk(1), self.bit2bpsk(1),
    #                     self.bit2bpsk(1), self.bit2bpsk(1), self.bit2bpsk(1), self.bit2bpsk(0), self.bit2bpsk(1), self.bit2bpsk(0), self.bit2bpsk(0),
    #                     self.bit2bpsk(0), self.bit2bpsk(0), self.bit2bpsk(1), self.bit2bpsk(0), self.bit2bpsk(1)]

        
    #     ##########
    #     # Bloques
    #     ##########
    #     src1 = blocks.vector_source_c(data_source, False, size_by_mode[mode]*n_file_source_segments)
        
    #     dst = blocks.vector_sink_c(carriers_number)
        
    #     #XML: isdbt.ofdm_frame_structure($mode, $IsOneSeg, $ModeA, $ModeB, $ModeC, $ConvCodeA, $ConvCodeB, $ConvCodeC, $IntLenA, $IntLenB, $IntLenC, $LayASeg, $LayBSeg, $LayCSeg)
        
    #     sqr = isdbt.ofdm_frame_structure(mode, fullseg, carrier_mod_scheme[param_layer_A[0]], carrier_mod_scheme[param_layer_B[0]], carrier_mod_scheme[param_layer_C[0]], 

    #                                     conv_coding_rate[param_layer_A[1]], conv_coding_rate[param_layer_B[1]], conv_coding_rate[param_layer_C[1]],

    #                                     interleaving_length[param_layer_A[2]], interleaving_length[param_layer_B[2]], interleaving_length[param_layer_C[2]],

    #                                     param_layer_A[3], param_layer_B[3], param_layer_C[3])

    #     ############
    #     # Conexiones
    #     ############
    #     self.tb.connect(src1, sqr)
    #     self.tb.connect(sqr, dst)
    #     self.tb.run()

    #     # Saves the processed data to a vector
    #     result_data_zp = dst.data()

    #     # Removes the zero padding
    #     result_data = self.aux_remove_zero_padding(result_data_zp, n_symbols, mode)
    #     #print "Result data ZP length"
    #     # print len(result_data_zp)
    #     # for i in range(0, 350):
    #     #     print i
    #     #     print result_data_zp[i]
    

    #     # Get the elements which index are the elements on sp_index
    #     result_sp = numpy.take(result_data, sp_index)

    #     self.assertFloatTuplesAlmostEqual(expected_sp, result_sp)
        

  

    # def test_data_ofdm_frame (self):
    #     """Check that the data in the OFDM produced (not consider SP, AC, TMCC) has the right structure"""
            
    #     mode = 1
    #     n_file_source_segments = 13
    #     fullseg = False # 0 - False, is not one seg
    #     param_layer_A = ("16QAM", "3/4", 8, 6)
    #     param_layer_B = ("QPSK", "1/2", 16, 7)
    #     param_layer_C = ("UNUSED", "UNUSED", "UNUSED", "UNUSED")
        

    #     size_by_mode = {1: 96, 2: 192, 3: 384}
    #     carriers_number = 2048
    #     #file_path = "/home/santiago/file_source_mode1_zeros.txt"

    #     data_source = [0]*(96*13)*204
    #     # Si el archivo fuente contiene las TMCC, AC, SP (como puede ser un archivo generado a partir de gr-isdbt),
    #     # en ese caso se utiliza expected_data = self.aux_get_data(ofdm_frames, mode)
        
    #     expected_data = data_source

    #     ##########
    #     # Bloques
    #     ##########
    #     src1 = blocks.vector_source_c(data_source, False, size_by_mode[mode]*n_file_source_segments)
    #     dst = blocks.vector_sink_c(carriers_number)
    #     # sqr = isdbt.ofdm_frame_structure(
    #     #                                 mode, fullseg, carrier_mod_scheme[param_layer_A[0]], carrier_mod_scheme[param_layer_B[0]], carrier_mod_scheme[param_layer_C[0]],
				# 				# 		conv_coding_rate[param_layer_A[1]], conv_coding_rate[param_layer_B[1]], conv_coding_rate[param_layer_C[1]],
    #     #                                 interleaving_length[(param_layer_A[2], mode)], interleaving_length[(param_layer_B[2], mode)], interleaving_length[(param_layer_C[2], mode)],
				# 				# 		number_of_segments[param_layer_A[3]], number_of_segments[param_layer_B[3]], number_of_segments[param_layer_C[3]]
    #     #                                 )

    #     sqr = isdbt.ofdm_frame_structure(mode, fullseg, 2, 1, 4, 2, 0 ,6, 8, 16, 0, 6, 7, 0)
        

    #     ############
    #     # Conexiones
    #     ############
    #     self.tb.connect(src1, sqr)
    #     self.tb.connect(sqr, dst)
    #     self.tb.run()

        
    #     result_ofdm_frames= dst.data()
    #     result_data = self.aux_get_data(result_ofdm_frames, mode)
    #     self.assertFloatTuplesAlmostEqual(expected_data, result_data)
        
        
        
        
    # def test_tmcc_ofdm_frame (self):
    #     mode = 1
    #     n_file_source_segments = 13
    #     fullseg = False # 0 - False, is not one seg
    #     param_layer_A = ("16QAM", "3/4", "8", 6)
    #     param_layer_B = ("QPSK", "1/2", "16", 7)
    #     param_layer_C = ("UNUSED", "UNUSED", "0", 0)
        
    #     carriers_number = pow(2,10+mode) # number of carriers at the "OFDM Frame Structure" block output

    #     data_source = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]*(size_by_mode[mode]*13)  # creates a data source vector 
    #     n_symbols = 204
    #     #file_path = "/home/santiago/file_source_mode1_zeros.txt"
    #     #data_source = self.open_file_and_load_to_list(file_path)
        


    #     expected_tmcc = self.aux_expected_tmcc(data_source, mode, param_layer_A, param_layer_B, param_layer_C)  # es un array de numpy
    #     tmcc_index = self.aux_get_TMCC_index(2, mode, n_file_source_segments)
        
    #     # *** Chequear la alternancia entre los w0 y w1
    #     # el_rango_correspondiente_a_w0y1(result o result_tmcc)
    #     # for i in tmcc_index:
    #     #      (el_rango_correspondiente_a_w0y1(result o result_tmcc) AND BIT A BIT(el de indice i menor)
    #     #     si ese and es 0 => OK, paso al siguiente
    #     #     si ese and <> 0 => FAIL, retorno ERRROR
    #     # len(tmcc_index)
    #     # ****
        
    #     ##########
    #     # Bloques
    #     ##########
    #     src1 = blocks.vector_source_c(data_source, False, size_by_mode[mode]*n_file_source_segments)
    #     dst = blocks.vector_sink_c(carriers_number)
    #     sqr = isdbt.ofdm_frame_structure(mode, fullseg, carrier_mod_scheme[param_layer_A[0]], carrier_mod_scheme[param_layer_B[0]], carrier_mod_scheme[param_layer_C[0]], 

    #                                     conv_coding_rate[param_layer_A[1]], conv_coding_rate[param_layer_B[1]], conv_coding_rate[param_layer_C[1]],

    #                                     interleaving_length[param_layer_A[2]], interleaving_length[param_layer_B[2]], interleaving_length[param_layer_C[2]],

    #                                     param_layer_A[3], param_layer_B[3], param_layer_C[3])

    #     ############
    #     # Conexiones
    #     ############
    #     self.tb.connect(src1, sqr)
    #     self.tb.connect(sqr, dst)
    #     self.tb.run()

    #     result_data_zp = dst.data()
    #     result_data = self.aux_remove_zero_padding(result_data_zp, n_symbols, mode)
    #     result_tmcc = numpy.take(result_data, tmcc_index) # get the elements wich index are the element on tmcc_index

    #     print "expected len tmcc"
    #     print len(expected_tmcc)

    #     print "result len t,cc"
    #     print len(result_tmcc)
    #     self.assertFloatTuplesAlmostEqual(expected_tmcc, result_tmcc)
        
        
    def test_ac_ofdm_frame (self):
        mode = 1
        n_file_source_segments = 13
        fullseg = True
        param_layer_A = ("16QAM", "3/4", "8", 6)
        param_layer_B = ("QPSK", "1/2", "16", 7)
        param_layer_C = ("UNUSED", "UNUSED", "0", 0)
        
        carriers_number = pow(2,10+mode) # number of carriers at the "OFDM Frame Structure" block output
        
        #file_path = "/home/santiago/file_source_mode1_zeros.txt"
        #data_source = self.open_file_and_load_to_list(file_path)

        data_source = [1,1,1,1,1,1,1]*(size_by_mode[mode]*13)  # creates a data source vector 
        n_symbols = 7

        ac_index = self.aux_get_AC_index(n_symbols, mode, n_file_source_segments)
        print "Expected ac len"
        expected_ac = [0]*len(ac_index)             # As the AC are optional, we expect that be 0
        
        
        ##########
        # Bloques
        ##########
        src1 = blocks.vector_source_c(data_source, False, size_by_mode[mode]*n_file_source_segments)
        dst = blocks.vector_sink_c(carriers_number)
        
        #XML: isdbt.ofdm_frame_structure($mode, $IsOneSeg, $ModeA, $ModeB, $ModeC, $ConvCodeA, $ConvCodeB, $ConvCodeC, $IntLenA, $IntLenB, $IntLenC, $LayASeg, $LayBSeg, $LayCSeg)
        
        sqr = isdbt.ofdm_frame_structure(mode, fullseg, carrier_mod_scheme[param_layer_A[0]], carrier_mod_scheme[param_layer_B[0]], carrier_mod_scheme[param_layer_C[0]], 

                                        conv_coding_rate[param_layer_A[1]], conv_coding_rate[param_layer_B[1]], conv_coding_rate[param_layer_C[1]],

                                        interleaving_length[param_layer_A[2]], interleaving_length[param_layer_B[2]], interleaving_length[param_layer_C[2]],

                                        param_layer_A[3], param_layer_B[3], param_layer_C[3])

        ############
        # Conexiones
        ############
        self.tb.connect(src1, sqr)
        self.tb.connect(sqr, dst)
        self.tb.run()

        result_data_zp = dst.data()



        result_data = self.aux_remove_zero_padding(result_data_zp, n_symbols, mode)
        result_ac = numpy.take(result_data, ac_index) # get the elements wich index is on ac_index


        self.assertFloatTuplesAlmostEqual(expected_ac, result_ac)

        
        
        
        
    # def test_001_ofdm_frame (self):
    #     self.tb = None
        ###########################################
        # # Variables
        # ###########################################
        # src_segments = []         #define un array 
        # mode = 1                  #modo de transmision
        # size = 2048       #cantidad total de portadoras, incluyendo las SP,TMCC,AC.
        # sp_step = 12              #una SP cada 12 portadoras
        # sp_index = range(0,size,sp_step) #indices absolutos de las SP en un simbolo OFDM (0,12,24.48,...,2040 para modo 1)
        
        # tmcc_index_mode1 = 322+(70+11*108, 25+9*108, 17+7*108, 86+5*108, 44+3*108, 47+1*108, 49, 31+2*108, 83+4*108, 61+6*108, 85+8*108, 101+10*108, 23+12*108)
        # ac1_index_mode1 = 322+(10+11*108, 53+9*108, 61+7*108, 11+5*108, 20+3*108, 74+1*108, 35, 76+2*108, 4+4*108, 40+6*108, 8+8*108, 7+10*108, 98+12*108)
        # ac2_index_mode1 = 322+(28+11*108, 83+9*108, 100+7*108, 101+5*108, 40+3*108, 100+1*108, 79, 97+2*108, 89+4*108, 89+6*108, 64+8*108, 89+10*108, 101+12*108)

        # SP = complex(-4.0/3.0, 0) #Valor constante de la portadora. Debe ser una funcion
        # expected_result = []
        # TMCC = -50
        # AC1 = 999
        # AC2 = -999
     #    [x+1 for x in mylist]
        # for i in range(1,14):     #asigna 96 complejos a cada uno de los 13 segmentos
     #      #src_segments.append([complex(i,i)]*96)
            
     #        src_segments = src_segments + [complex(i,i)]*96 

     #    print("SRC_SEGMENTS")
     #    print(len(src_segments))
        
     #    #Armo el vector esperado
        
     #    expected_result = [0]*322   #Zero-Padding

     #    for i in range(1,14):     #asigna 96 complejos a cada uno de los 13 segmentos
     #      #expected_result.append([complex(i,i)]*108)
     #      expected_result = expected_result + [complex(i,i)]*108

     #    expected_result = expected_result + [0]*322 #Zero-Padding

     #    for i in range(0, size):
     #        if i in sp_index:
     #             expected_result[i] = SP
     #        if i in tmcc_index_mode1:
     #             expected_result[i] = TMCC
     #        if i in ac1_index_mode1:
     #             expected_result[i] = AC1
     #        if i in ac2_index_mode1:
     #             expected_result[i] = AC2
     #    filename = "esperado.txt"
     #    file = open(filename, 'w')

     #    for i in range(len(expected_result)):
     #        file.write("%0.1f+%0.1fj || " % (expected_result[i].real, expected_result[i].imag))
     #    file.close()
     #    ###########################################
     #    # Bloques
     #    ###########################################
     #    src1 = blocks.vector_source_c(src_segments, False, 96*13)
     #    dst = blocks.vector_sink_c(size)
     #    sqr = isdbt.ofdm_frame_structure(mode)

     #    ############################################
     #    # Conexiones
     #    ############################################
     #    self.tb.connect(src1, sqr)
     #    self.tb.connect(sqr, dst)
     #    self.tb.run()

     #    result_data = dst.data()

     #    print(expected_result)
     #    print(result_data)

     #    self.assertFloatTuplesAlmostEqual(expected_result, result_data)
if __name__ == '__main__':
    gr_unittest.run(qa_ofdm_frame, "qa_ofdm_frame.xml")