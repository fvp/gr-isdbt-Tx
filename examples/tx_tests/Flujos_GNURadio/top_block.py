#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Mon Aug 27 20:09:38 2018
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import dtv
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import isdbt
import sys


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.samp_rate_0 = samp_rate_0 = 32000

        ##################################################
        # Blocks
        ##################################################
        self.isdbt_viterbi_decoder_0 = isdbt.viterbi_decoder(64, 0)
        self.isdbt_tsp_resize_0_2 = isdbt.tsp_resize()
        self.isdbt_sync_adjust_0 = isdbt.sync_adjust()
        self.isdbt_symbol_demapper_1seg_0 = isdbt.symbol_demapper_1seg(2, 64)
        self.isdbt_reed_solomon_dec_isdbt_0 = isdbt.reed_solomon_dec_isdbt()
        self.isdbt_mapper_0 = isdbt.mapper(2, 6, 1)
        self.isdbt_hierarchical_divisor_0 = isdbt.hierarchical_divisor()
        self.isdbt_energy_dispersal_0 = isdbt.energy_dispersal()
        self.isdbt_energy_descrambler_0 = isdbt.energy_descrambler()
        self.isdbt_byte_interleaver_0 = isdbt.byte_interleaver(2, 0, 6, 1)
        self.isdbt_byte_deinterleaver_0 = isdbt.byte_deinterleaver()
        self.isdbt_bit_deinterleaver_0 = isdbt.bit_deinterleaver(2, 1, 64)
        self.dtv_dvbt_reed_solomon_enc_0_0 = dtv.dvbt_reed_solomon_enc(2, 8, 0x11d, 255, 239, 8, 51, 1)
        self.dtv_dvbt_inner_coder_0 = dtv.dvbt_inner_coder(1, 1512, dtv.MOD_64QAM, dtv.ALPHA2, dtv.C1_2)
        self.blocks_vector_to_stream_0_1_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_1_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_1 = blocks.vector_to_stream(gr.sizeof_char*1, 1512)
        self.blocks_vector_to_stream_0_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_0 = blocks.vector_to_stream(gr.sizeof_char*204, 126)
        self.blocks_vector_to_stream_0 = blocks.vector_to_stream(gr.sizeof_char*1, 188)
        self.blocks_unpacked_to_packed_xx_0 = blocks.unpacked_to_packed_bb(6, gr.GR_LSB_FIRST)
        self.blocks_stream_to_vector_1 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 192)
        self.blocks_stream_to_tagged_stream_0_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 204, 72, "frame_begin")
        self.blocks_packed_to_unpacked_xx_0 = blocks.packed_to_unpacked_bb(6, gr.GR_LSB_FIRST)
        self.blocks_null_sink_1 = blocks.null_sink(gr.sizeof_char*204*756)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_char*204*756)
        self.blocks_file_source_0_0_0 = blocks.file_source(gr.sizeof_char*204*2304, '/home/habbo/Documents/BTS_Validos/BTS_ISDB_T_3_CAPAS.ts', False)
        self.blocks_file_sink_0_0_1 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/Caso5/5_BD_in.ts', False)
        self.blocks_file_sink_0_0_1.set_unbuffered(False)
        self.blocks_file_sink_0_0_0_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/Caso5/Punto2.ts', False)
        self.blocks_file_sink_0_0_0_0.set_unbuffered(False)
        self.blocks_file_sink_0_0_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/Caso5/5_ED_in.ts', False)
        self.blocks_file_sink_0_0_0.set_unbuffered(False)
        self.blocks_file_sink_0_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/Caso5/5_RS_in.ts', False)
        self.blocks_file_sink_0_0.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/Caso5/Salida_Test_5.ts', False)
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_file_source_0_0_0, 0), (self.isdbt_hierarchical_divisor_0, 0))    
        self.connect((self.blocks_packed_to_unpacked_xx_0, 0), (self.isdbt_mapper_0, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0_0, 0), (self.blocks_vector_to_stream_0_1_0_0, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0_0, 0), (self.isdbt_energy_descrambler_0, 0))    
        self.connect((self.blocks_stream_to_vector_1, 0), (self.isdbt_symbol_demapper_1seg_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.blocks_file_sink_0_0_0_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.isdbt_viterbi_decoder_0, 0))    
        self.connect((self.blocks_vector_to_stream_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0, 0), (self.isdbt_tsp_resize_0_2, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_0, 0), (self.dtv_dvbt_inner_coder_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1, 0), (self.blocks_packed_to_unpacked_xx_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_0, 0), (self.blocks_file_sink_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_0_0, 0), (self.blocks_file_sink_0_0_0, 0))    
        self.connect((self.dtv_dvbt_inner_coder_0, 0), (self.blocks_vector_to_stream_0_1, 0))    
        self.connect((self.dtv_dvbt_reed_solomon_enc_0_0, 0), (self.isdbt_sync_adjust_0, 0))    
        self.connect((self.isdbt_bit_deinterleaver_0, 0), (self.blocks_unpacked_to_packed_xx_0, 0))    
        self.connect((self.isdbt_byte_deinterleaver_0, 0), (self.blocks_stream_to_tagged_stream_0_0, 0))    
        self.connect((self.isdbt_byte_interleaver_0, 0), (self.blocks_vector_to_stream_0_0_0, 0))    
        self.connect((self.isdbt_energy_descrambler_0, 0), (self.blocks_vector_to_stream_0_1_0, 0))    
        self.connect((self.isdbt_energy_descrambler_0, 0), (self.isdbt_reed_solomon_dec_isdbt_0, 0))    
        self.connect((self.isdbt_energy_dispersal_0, 0), (self.isdbt_byte_interleaver_0, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 1), (self.blocks_null_sink_0, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 2), (self.blocks_null_sink_1, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 0), (self.blocks_vector_to_stream_0_0, 0))    
        self.connect((self.isdbt_mapper_0, 0), (self.blocks_stream_to_vector_1, 0))    
        self.connect((self.isdbt_reed_solomon_dec_isdbt_0, 0), (self.blocks_vector_to_stream_0, 0))    
        self.connect((self.isdbt_symbol_demapper_1seg_0, 0), (self.isdbt_bit_deinterleaver_0, 0))    
        self.connect((self.isdbt_sync_adjust_0, 0), (self.isdbt_energy_dispersal_0, 0))    
        self.connect((self.isdbt_tsp_resize_0_2, 0), (self.dtv_dvbt_reed_solomon_enc_0_0, 0))    
        self.connect((self.isdbt_viterbi_decoder_0, 0), (self.blocks_file_sink_0_0_1, 0))    
        self.connect((self.isdbt_viterbi_decoder_0, 0), (self.isdbt_byte_deinterleaver_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate_0(self):
        return self.samp_rate_0

    def set_samp_rate_0(self, samp_rate_0):
        self.samp_rate_0 = samp_rate_0


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
