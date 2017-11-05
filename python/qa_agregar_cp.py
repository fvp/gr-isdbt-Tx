#!/usr/bin/env python
# -*- coding: utf-8 -*-
#  
# Copyright 2015, 2016, 2017
#   Federico "Larroca" La Rocca <flarroca@fing.edu.uy>
#   Pablo Belzarena 
#   Gabriel Gomez Sena 
#   Pablo Flores Guridi 
#   Victor Gonzalez Barbone
# 
#   Instituto de Ingenieria Electrica, Facultad de Ingenieria,
#   Universidad de la Republica, Uruguay.
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
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import isdbt_swig as isdbt

class qa_agregar_cp (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_agregar_cp_Mode1_CP_1_4 (self):
        modo = 1
        cp_length = 1.0/4.0
        src_data = range(pow(2,10+modo))
        
        n_portadoras = pow(2,10+modo)
        expected_result = range(n_portadoras-(int)(n_portadoras*cp_length), n_portadoras, 1)+range(n_portadoras)

        src = blocks.vector_source_c(src_data, False, n_portadoras)
        agregar_cp = isdbt.agregar_cp(modo, cp_length)
        dst = blocks.vector_sink_c(int(n_portadoras*(1+cp_length)))

        
        self.tb.connect(src, agregar_cp)
        self.tb.connect(agregar_cp, dst)
        self.tb.run()

        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)

    def test_agregar_cp_Mode1_CP_1_8 (self):
        modo = 1
        cp_length = 1.0/8.0
        src_data = range(pow(2,10+modo))
        
        n_portadoras = pow(2,10+modo)
        expected_result = range(n_portadoras-(int)(n_portadoras*cp_length), n_portadoras, 1)+range(n_portadoras)

        src = blocks.vector_source_c(src_data, False, n_portadoras)
        agregar_cp = isdbt.agregar_cp(modo, cp_length)
        dst = blocks.vector_sink_c(int(n_portadoras*(1+cp_length)))

        
        self.tb.connect(src, agregar_cp)
        self.tb.connect(agregar_cp, dst)
        self.tb.run()

        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)

    def test_agregar_cp_Mode1_CP_1_16 (self):
        modo = 1
        cp_length = 1.0/16.0
        src_data = range(pow(2,10+modo))
        
        n_portadoras = pow(2,10+modo)
        expected_result = range(n_portadoras-(int)(n_portadoras*cp_length), n_portadoras, 1)+range(n_portadoras)

        src = blocks.vector_source_c(src_data, False, n_portadoras)
        agregar_cp = isdbt.agregar_cp(modo, cp_length)
        dst = blocks.vector_sink_c(int(n_portadoras*(1+cp_length)))

        
        self.tb.connect(src, agregar_cp)
        self.tb.connect(agregar_cp, dst)
        self.tb.run()

        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)

    def test_agregar_cp_Mode1_CP_1_32 (self):
        modo = 1
        cp_length = 1.0/32.0
        src_data = range(pow(2,10+modo))
        
        n_portadoras = pow(2,10+modo)
        expected_result = range(n_portadoras-(int)(n_portadoras*cp_length), n_portadoras, 1)+range(n_portadoras)

        src = blocks.vector_source_c(src_data, False, n_portadoras)
        agregar_cp = isdbt.agregar_cp(modo, cp_length)
        dst = blocks.vector_sink_c(int(n_portadoras*(1+cp_length)))

        
        self.tb.connect(src, agregar_cp)
        self.tb.connect(agregar_cp, dst)
        self.tb.run()

        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)

    def test_agregar_cp_Mode2_CP_1_8 (self):
        modo = 2
        cp_length = 1.0/8.0
        src_data = range(pow(2,10+modo))
        
        n_portadoras = pow(2,10+modo)
        expected_result = range(n_portadoras-(int)(n_portadoras*cp_length), n_portadoras, 1)+range(n_portadoras)

        src = blocks.vector_source_c(src_data, False, n_portadoras)
        agregar_cp = isdbt.agregar_cp(modo, cp_length)
        dst = blocks.vector_sink_c(int(n_portadoras*(1+cp_length)))

        
        self.tb.connect(src, agregar_cp)
        self.tb.connect(agregar_cp, dst)
        self.tb.run()

        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)

    def test_agregar_cp_Mode3_CP_1_8 (self):
        modo = 3
        cp_length = 1.0/8.0
        src_data = range(pow(2,10+modo))
        
        n_portadoras = pow(2,10+modo)
        expected_result = range(n_portadoras-(int)(n_portadoras*cp_length), n_portadoras, 1)+range(n_portadoras)

        src = blocks.vector_source_c(src_data, False, n_portadoras)
        agregar_cp = isdbt.agregar_cp(modo, cp_length)
        dst = blocks.vector_sink_c(int(n_portadoras*(1+cp_length)))

        
        self.tb.connect(src, agregar_cp)
        self.tb.connect(agregar_cp, dst)
        self.tb.run()

        result_data = dst.data()
        self.assertFloatTuplesAlmostEqual(expected_result, result_data)

   	    
if __name__ == '__main__':
    gr_unittest.run(qa_agregar_cp, "qa_agregar_cp.xml")
