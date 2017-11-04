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

	def test_001_t (self):
        # set up fg
        self.tb.run ()
        # check data

    #test_001(self):
   	#	"Prueba de tamanio"
   	#	modo = -5
	#	cp_length = (1/4)
	#	src_data = range(12)
	#	expected_data = [9 10 11 0 1 2 3 4 5 6 7 8 9 10 11]
 	#	src = blocks.vector_source_f(src_data)
     #   sqr = isdbt.agregar_cp(modo, cp_length)
      #  dst = blocks.vector_sink_f()
      #  self.tb.connect(src,sqr)
       # self.tb.connect(sqr,dst)
        #self.tb.run ()
       # result_data = dst.data()
        #self.assertFloatTuplesAlmostEqual(expected_data, result_data, 6)
        # check data		


if __name__ == '__main__':
    gr_unittest.run(qa_agregar_cp, "qa_agregar_cp.xml")
