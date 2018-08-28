#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Mon Aug 27 20:10:17 2018
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
from gnuradio import digital
from gnuradio import dtv
from gnuradio import eng_notation
from gnuradio import fft
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from optparse import OptionParser
import isdbt
import sip
import sys


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self, fft_size=4096):
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
        # Parameters
        ##################################################
        self.fft_size = fft_size

        ##################################################
        # Variables
        ##################################################
        self.mode = mode = 2
        self.freq = freq = 551e6

        ##################################################
        # Blocks
        ##################################################
        self.qtgui_const_sink_x_0_1_0_0_0_1 = qtgui.const_sink_c(
        	4096, #size
        	"Time Denint OUT", #name
        	1 #number of inputs
        )
        self.qtgui_const_sink_x_0_1_0_0_0_1.set_update_time(0.1)
        self.qtgui_const_sink_x_0_1_0_0_0_1.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_0_1_0_0_0_1.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_0_1_0_0_0_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0_1_0_0_0_1.enable_autoscale(False)
        self.qtgui_const_sink_x_0_1_0_0_0_1.enable_grid(True)
        self.qtgui_const_sink_x_0_1_0_0_0_1.enable_axis_labels(True)
        
        if not True:
          self.qtgui_const_sink_x_0_1_0_0_0_1.disable_legend()
        
        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
                  "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_0_1_0_0_0_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0_1_0_0_0_1.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0_1_0_0_0_1.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0_1_0_0_0_1.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0_1_0_0_0_1.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0_1_0_0_0_1.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0_1_0_0_0_1.set_line_alpha(i, alphas[i])
        
        self._qtgui_const_sink_x_0_1_0_0_0_1_win = sip.wrapinstance(self.qtgui_const_sink_x_0_1_0_0_0_1.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_const_sink_x_0_1_0_0_0_1_win)
        self.isdbt_viterbi_decoder_0_1 = isdbt.viterbi_decoder(64, 0)
        self.isdbt_viterbi_decoder_0_0 = isdbt.viterbi_decoder(64, 0)
        self.isdbt_viterbi_decoder_0 = isdbt.viterbi_decoder(64, 0)
        self.isdbt_tsp_resize_0_0_0 = isdbt.tsp_resize()
        self.isdbt_tsp_resize_0_0 = isdbt.tsp_resize()
        self.isdbt_tsp_resize_0 = isdbt.tsp_resize()
        self.isdbt_tmcc_decoder_0 = isdbt.tmcc_decoder(2, False)
        self.isdbt_time_interleaver_0 = isdbt.time_interleaver(2, 8, True)
        self.isdbt_time_deinterleaver_0 = isdbt.time_deinterleaver(2, 1, 8, 6, 8, 6, 8)
        self.isdbt_symbol_demapper_0 = isdbt.symbol_demapper(2, 1, 64, 6, 64, 6, 64)
        self.isdbt_reed_solomon_dec_isdbt_0_1 = isdbt.reed_solomon_dec_isdbt()
        self.isdbt_reed_solomon_dec_isdbt_0_0 = isdbt.reed_solomon_dec_isdbt()
        self.isdbt_reed_solomon_dec_isdbt_0 = isdbt.reed_solomon_dec_isdbt()
        self.isdbt_ofdm_synchronization_0 = isdbt.ofdm_synchronization(2, 0.125, False)
        self.isdbt_ofdm_frame_structure_0 = isdbt.ofdm_frame_structure(2, True, 3, 3, 3, 0, 0, 0, 8, 8, 8, 1, 6, 6)
        self.isdbt_mapper_0_0_0 = isdbt.mapper(2, 6, 6)
        self.isdbt_mapper_0_0 = isdbt.mapper(2, 6, 6)
        self.isdbt_mapper_0 = isdbt.mapper(2, 6, 1)
        self.isdbt_hierarchical_divisor_0 = isdbt.hierarchical_divisor()
        self.isdbt_hierarchical_combination_0 = isdbt.hierarchical_combination(2, 1, 6, 6)
        self.isdbt_frequency_deinterleaver_0 = isdbt.frequency_deinterleaver(False, 2)
        self.isdbt_frec_interleaver_0 = isdbt.frec_interleaver(2, True)
        self.isdbt_energy_dispersal_0_0_0 = isdbt.energy_dispersal()
        self.isdbt_energy_dispersal_0_0 = isdbt.energy_dispersal()
        self.isdbt_energy_dispersal_0 = isdbt.energy_dispersal()
        self.isdbt_energy_descrambler_0_1 = isdbt.energy_descrambler()
        self.isdbt_energy_descrambler_0_0 = isdbt.energy_descrambler()
        self.isdbt_energy_descrambler_0 = isdbt.energy_descrambler()
        self.isdbt_byte_interleaver_0_0_0 = isdbt.byte_interleaver(2, 0, 6, 6)
        self.isdbt_byte_interleaver_0_0 = isdbt.byte_interleaver(2, 0, 6, 6)
        self.isdbt_byte_interleaver_0 = isdbt.byte_interleaver(2, 0, 6, 1)
        self.isdbt_byte_deinterleaver_0_1 = isdbt.byte_deinterleaver()
        self.isdbt_byte_deinterleaver_0_0 = isdbt.byte_deinterleaver()
        self.isdbt_byte_deinterleaver_0 = isdbt.byte_deinterleaver()
        self.isdbt_bit_deinterleaver_0_1 = isdbt.bit_deinterleaver(2, 6, 64)
        self.isdbt_bit_deinterleaver_0_0 = isdbt.bit_deinterleaver(2, 6, 64)
        self.isdbt_bit_deinterleaver_0 = isdbt.bit_deinterleaver(2, 1, 64)
        self.fft_vxx_0 = fft.fft_vcc(fft_size, False, (window.blackmanharris(fft_size)), True, 1)
        self.dtv_dvbt_reed_solomon_enc_0_0_0 = dtv.dvbt_reed_solomon_enc(2, 8, 0x11d, 255, 239, 8, 51, 1)
        self.dtv_dvbt_reed_solomon_enc_0_0 = dtv.dvbt_reed_solomon_enc(2, 8, 0x11d, 255, 239, 8, 51, 1)
        self.dtv_dvbt_reed_solomon_enc_0 = dtv.dvbt_reed_solomon_enc(2, 8, 0x11d, 255, 239, 8, 51, 1)
        self.dtv_dvbt_inner_coder_0_0_0 = dtv.dvbt_inner_coder(1, 1512, dtv.MOD_64QAM, dtv.ALPHA2, dtv.C1_2)
        self.dtv_dvbt_inner_coder_0_0 = dtv.dvbt_inner_coder(1, 1512, dtv.MOD_64QAM, dtv.ALPHA2, dtv.C1_2)
        self.dtv_dvbt_inner_coder_0 = dtv.dvbt_inner_coder(1, 1512, dtv.MOD_64QAM, dtv.ALPHA2, dtv.C1_2)
        self.digital_ofdm_cyclic_prefixer_0 = digital.ofdm_cyclic_prefixer(4096, 4096+512, 0, "")
        self.blocks_vector_to_stream_1_2_0_0_0_0_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 192*13)
        self.blocks_vector_to_stream_0_1_0_1_1 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_1_0_1_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_1_0_1 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_1_0_0 = blocks.vector_to_stream(gr.sizeof_char*204, 756)
        self.blocks_vector_to_stream_0_1_0 = blocks.vector_to_stream(gr.sizeof_char*204, 756)
        self.blocks_vector_to_stream_0_1 = blocks.vector_to_stream(gr.sizeof_char*204, 126)
        self.blocks_vector_to_stream_0_0_1_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_0_1 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_0_0_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 1512)
        self.blocks_vector_to_stream_0_0_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 1512)
        self.blocks_vector_to_stream_0_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 1512)
        self.blocks_vector_to_stream_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_unpacked_to_packed_xx_0_1 = blocks.unpacked_to_packed_bb(6, gr.GR_LSB_FIRST)
        self.blocks_unpacked_to_packed_xx_0_0 = blocks.unpacked_to_packed_bb(6, gr.GR_LSB_FIRST)
        self.blocks_unpacked_to_packed_xx_0 = blocks.unpacked_to_packed_bb(6, gr.GR_LSB_FIRST)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, 512/63e-6,True)
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_char*204, 'LayA', "frame_begin"); self.blocks_tag_debug_0.set_display(True)
        self.blocks_stream_to_vector_0_1 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 192*6)
        self.blocks_stream_to_vector_0_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 192*6)
        self.blocks_stream_to_vector_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 192*1)
        self.blocks_packed_to_unpacked_xx_0_0_0 = blocks.packed_to_unpacked_bb(6, gr.GR_LSB_FIRST)
        self.blocks_packed_to_unpacked_xx_0_0 = blocks.packed_to_unpacked_bb(6, gr.GR_LSB_FIRST)
        self.blocks_packed_to_unpacked_xx_0 = blocks.packed_to_unpacked_bb(6, gr.GR_LSB_FIRST)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((0.01, ))
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_char*204*2304, '/home/habbo/Documents/BTS_Validos/BTS_ISDB_T_3_CAPAS.ts', False)
        self.blocks_file_sink_0_1 = blocks.file_sink(gr.sizeof_char*188, '/home/habbo/Documents/PruebasISDBT/Salidas/Layer_C.ts', False)
        self.blocks_file_sink_0_1.set_unbuffered(False)
        self.blocks_file_sink_0_0_0_2 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/fs__VDin_layA.ts', False)
        self.blocks_file_sink_0_0_0_2.set_unbuffered(False)
        self.blocks_file_sink_0_0_0_1_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/fs__VDin_layC.ts', False)
        self.blocks_file_sink_0_0_0_1_0.set_unbuffered(False)
        self.blocks_file_sink_0_0_0_1 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/fs__RSin_layC.ts', False)
        self.blocks_file_sink_0_0_0_1.set_unbuffered(False)
        self.blocks_file_sink_0_0_0_0_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/fs__VDin_layB.ts', False)
        self.blocks_file_sink_0_0_0_0_0.set_unbuffered(False)
        self.blocks_file_sink_0_0_0_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/fs__RSin_layB.ts', False)
        self.blocks_file_sink_0_0_0_0.set_unbuffered(False)
        self.blocks_file_sink_0_0_0 = blocks.file_sink(gr.sizeof_char*1, '/home/habbo/Documents/PruebasISDBT/Salidas/fs__RSin_layA.ts', False)
        self.blocks_file_sink_0_0_0.set_unbuffered(False)
        self.blocks_file_sink_0_0 = blocks.file_sink(gr.sizeof_char*188, '/home/habbo/Documents/PruebasISDBT/Salidas/Layer_B.ts', False)
        self.blocks_file_sink_0_0.set_unbuffered(False)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*188, '/home/habbo/Documents/PruebasISDBT/Salidas/Layer_A.ts', False)
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_file_source_0, 0), (self.isdbt_hierarchical_divisor_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_throttle_0_0, 0))    
        self.connect((self.blocks_packed_to_unpacked_xx_0, 0), (self.isdbt_mapper_0, 0))    
        self.connect((self.blocks_packed_to_unpacked_xx_0_0, 0), (self.isdbt_mapper_0_0, 0))    
        self.connect((self.blocks_packed_to_unpacked_xx_0_0_0, 0), (self.isdbt_mapper_0_0_0, 0))    
        self.connect((self.blocks_stream_to_vector_0, 0), (self.isdbt_hierarchical_combination_0, 0))    
        self.connect((self.blocks_stream_to_vector_0_0, 0), (self.isdbt_hierarchical_combination_0, 1))    
        self.connect((self.blocks_stream_to_vector_0_1, 0), (self.isdbt_hierarchical_combination_0, 2))    
        self.connect((self.blocks_throttle_0_0, 0), (self.isdbt_ofdm_synchronization_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.blocks_file_sink_0_0_0_2, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.isdbt_viterbi_decoder_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0_0, 0), (self.blocks_file_sink_0_0_0_0_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0_0, 0), (self.isdbt_viterbi_decoder_0_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0_1, 0), (self.blocks_file_sink_0_0_0_1_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0_1, 0), (self.isdbt_viterbi_decoder_0_1, 0))    
        self.connect((self.blocks_vector_to_stream_0_0, 0), (self.dtv_dvbt_inner_coder_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_0, 0), (self.blocks_packed_to_unpacked_xx_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_0_0, 0), (self.blocks_packed_to_unpacked_xx_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_0_0_0, 0), (self.blocks_packed_to_unpacked_xx_0_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_1, 0), (self.dtv_dvbt_inner_coder_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_1_0, 0), (self.dtv_dvbt_inner_coder_0_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1, 0), (self.isdbt_tsp_resize_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_0, 0), (self.isdbt_tsp_resize_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_0_0, 0), (self.isdbt_tsp_resize_0_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_0_1, 0), (self.blocks_file_sink_0_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_0_1_0, 0), (self.blocks_file_sink_0_0_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_0_1_1, 0), (self.blocks_file_sink_0_0_0_1, 0))    
        self.connect((self.blocks_vector_to_stream_1_2_0_0_0_0_0, 0), (self.qtgui_const_sink_x_0_1_0_0_0_1, 0))    
        self.connect((self.digital_ofdm_cyclic_prefixer_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.dtv_dvbt_inner_coder_0, 0), (self.blocks_vector_to_stream_0_0_0, 0))    
        self.connect((self.dtv_dvbt_inner_coder_0_0, 0), (self.blocks_vector_to_stream_0_0_0_0, 0))    
        self.connect((self.dtv_dvbt_inner_coder_0_0_0, 0), (self.blocks_vector_to_stream_0_0_0_0_0, 0))    
        self.connect((self.dtv_dvbt_reed_solomon_enc_0, 0), (self.isdbt_energy_dispersal_0, 0))    
        self.connect((self.dtv_dvbt_reed_solomon_enc_0_0, 0), (self.isdbt_energy_dispersal_0_0, 0))    
        self.connect((self.dtv_dvbt_reed_solomon_enc_0_0_0, 0), (self.isdbt_energy_dispersal_0_0_0, 0))    
        self.connect((self.fft_vxx_0, 0), (self.digital_ofdm_cyclic_prefixer_0, 0))    
        self.connect((self.isdbt_bit_deinterleaver_0, 0), (self.blocks_unpacked_to_packed_xx_0, 0))    
        self.connect((self.isdbt_bit_deinterleaver_0_0, 0), (self.blocks_unpacked_to_packed_xx_0_0, 0))    
        self.connect((self.isdbt_bit_deinterleaver_0_1, 0), (self.blocks_unpacked_to_packed_xx_0_1, 0))    
        self.connect((self.isdbt_byte_deinterleaver_0, 0), (self.blocks_tag_debug_0, 0))    
        self.connect((self.isdbt_byte_deinterleaver_0, 0), (self.isdbt_energy_descrambler_0, 0))    
        self.connect((self.isdbt_byte_deinterleaver_0_0, 0), (self.isdbt_energy_descrambler_0_0, 0))    
        self.connect((self.isdbt_byte_deinterleaver_0_1, 0), (self.isdbt_energy_descrambler_0_1, 0))    
        self.connect((self.isdbt_byte_interleaver_0, 0), (self.blocks_vector_to_stream_0_0, 0))    
        self.connect((self.isdbt_byte_interleaver_0_0, 0), (self.blocks_vector_to_stream_0_0_1, 0))    
        self.connect((self.isdbt_byte_interleaver_0_0_0, 0), (self.blocks_vector_to_stream_0_0_1_0, 0))    
        self.connect((self.isdbt_energy_descrambler_0, 0), (self.blocks_vector_to_stream_0_1_0_1, 0))    
        self.connect((self.isdbt_energy_descrambler_0, 0), (self.isdbt_reed_solomon_dec_isdbt_0, 0))    
        self.connect((self.isdbt_energy_descrambler_0_0, 0), (self.blocks_vector_to_stream_0_1_0_1_0, 0))    
        self.connect((self.isdbt_energy_descrambler_0_0, 0), (self.isdbt_reed_solomon_dec_isdbt_0_0, 0))    
        self.connect((self.isdbt_energy_descrambler_0_1, 0), (self.blocks_vector_to_stream_0_1_0_1_1, 0))    
        self.connect((self.isdbt_energy_descrambler_0_1, 0), (self.isdbt_reed_solomon_dec_isdbt_0_1, 0))    
        self.connect((self.isdbt_energy_dispersal_0, 0), (self.isdbt_byte_interleaver_0, 0))    
        self.connect((self.isdbt_energy_dispersal_0_0, 0), (self.isdbt_byte_interleaver_0_0, 0))    
        self.connect((self.isdbt_energy_dispersal_0_0_0, 0), (self.isdbt_byte_interleaver_0_0_0, 0))    
        self.connect((self.isdbt_frec_interleaver_0, 0), (self.isdbt_ofdm_frame_structure_0, 0))    
        self.connect((self.isdbt_frequency_deinterleaver_0, 0), (self.isdbt_time_deinterleaver_0, 0))    
        self.connect((self.isdbt_hierarchical_combination_0, 0), (self.isdbt_time_interleaver_0, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 0), (self.blocks_vector_to_stream_0_1, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 1), (self.blocks_vector_to_stream_0_1_0, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 2), (self.blocks_vector_to_stream_0_1_0_0, 0))    
        self.connect((self.isdbt_mapper_0, 0), (self.blocks_stream_to_vector_0, 0))    
        self.connect((self.isdbt_mapper_0_0, 0), (self.blocks_stream_to_vector_0_0, 0))    
        self.connect((self.isdbt_mapper_0_0_0, 0), (self.blocks_stream_to_vector_0_1, 0))    
        self.connect((self.isdbt_ofdm_frame_structure_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.isdbt_ofdm_synchronization_0, 0), (self.isdbt_tmcc_decoder_0, 0))    
        self.connect((self.isdbt_reed_solomon_dec_isdbt_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.isdbt_reed_solomon_dec_isdbt_0_0, 0), (self.blocks_file_sink_0_0, 0))    
        self.connect((self.isdbt_reed_solomon_dec_isdbt_0_1, 0), (self.blocks_file_sink_0_1, 0))    
        self.connect((self.isdbt_symbol_demapper_0, 0), (self.isdbt_bit_deinterleaver_0, 0))    
        self.connect((self.isdbt_symbol_demapper_0, 1), (self.isdbt_bit_deinterleaver_0_0, 0))    
        self.connect((self.isdbt_symbol_demapper_0, 2), (self.isdbt_bit_deinterleaver_0_1, 0))    
        self.connect((self.isdbt_time_deinterleaver_0, 0), (self.blocks_vector_to_stream_1_2_0_0_0_0_0, 0))    
        self.connect((self.isdbt_time_deinterleaver_0, 0), (self.isdbt_symbol_demapper_0, 0))    
        self.connect((self.isdbt_time_interleaver_0, 0), (self.isdbt_frec_interleaver_0, 0))    
        self.connect((self.isdbt_tmcc_decoder_0, 0), (self.isdbt_frequency_deinterleaver_0, 0))    
        self.connect((self.isdbt_tsp_resize_0, 0), (self.dtv_dvbt_reed_solomon_enc_0, 0))    
        self.connect((self.isdbt_tsp_resize_0_0, 0), (self.dtv_dvbt_reed_solomon_enc_0_0, 0))    
        self.connect((self.isdbt_tsp_resize_0_0_0, 0), (self.dtv_dvbt_reed_solomon_enc_0_0_0, 0))    
        self.connect((self.isdbt_viterbi_decoder_0, 0), (self.isdbt_byte_deinterleaver_0, 0))    
        self.connect((self.isdbt_viterbi_decoder_0_0, 0), (self.isdbt_byte_deinterleaver_0_0, 0))    
        self.connect((self.isdbt_viterbi_decoder_0_1, 0), (self.isdbt_byte_deinterleaver_0_1, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_fft_size(self):
        return self.fft_size

    def set_fft_size(self, fft_size):
        self.fft_size = fft_size

    def get_mode(self):
        return self.mode

    def set_mode(self, mode):
        self.mode = mode

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq


def argument_parser():
    parser = OptionParser(usage="%prog: [options]", option_class=eng_option)
    return parser


def main(top_block_cls=top_block, options=None):
    if options is None:
        options, _ = argument_parser().parse_args()

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
