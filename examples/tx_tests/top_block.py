#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Thu Oct 25 00:40:20 2018
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
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import isdbt
import sip
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
        self.variable = variable = 0.00001
        self.samp_rate_0 = samp_rate_0 = 32000
        self.rf_gain = rf_gain = 64
        self.if_gain = if_gain = 15
        self.bb_gain = bb_gain = 15

        ##################################################
        # Blocks
        ##################################################
        self._variable_range = Range(0.00001, 0.01, 0.00001, 0.00001, 200)
        self._variable_win = RangeWidget(self._variable_range, self.set_variable, 'variable', "counter_slider", float)
        self.top_layout.addWidget(self._variable_win)
        self._rf_gain_range = Range(0, 200, 1, 64, 200)
        self._rf_gain_win = RangeWidget(self._rf_gain_range, self.set_rf_gain, 'rf_gain', "counter_slider", float)
        self.top_layout.addWidget(self._rf_gain_win)
        self.rational_resampler_xxx_0 = filter.rational_resampler_ccc(
                interpolation=63,
                decimation=64,
                taps=None,
                fractional_bw=None,
        )
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	(4096+512)*2, #size
        	8127000, #samp_rate
        	"Senal en el tiempo", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)
        
        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")
        
        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_time_sink_x_0.disable_legend()
        
        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
        	(4096+512)*2, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	8127000, #bw
        	"ESPECTRO OFDM", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(True)
        self.qtgui_freq_sink_x_0.enable_grid(False)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)
        
        if not True:
          self.qtgui_freq_sink_x_0.disable_legend()
        
        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_freq_sink_x_0.set_plot_pos_half(not True)
        
        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.qtgui_const_sink_x_0 = qtgui.const_sink_c(
        	8192, #size
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_const_sink_x_0.set_update_time(0.10)
        self.qtgui_const_sink_x_0.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_0.enable_autoscale(False)
        self.qtgui_const_sink_x_0.enable_grid(False)
        self.qtgui_const_sink_x_0.enable_axis_labels(True)
        
        if not True:
          self.qtgui_const_sink_x_0.disable_legend()
        
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
                self.qtgui_const_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_const_sink_x_0_win = sip.wrapinstance(self.qtgui_const_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_const_sink_x_0_win)
        self.isdbt_tsp_resize_0_2_1 = isdbt.tsp_resize()
        self.isdbt_tsp_resize_0_2_0 = isdbt.tsp_resize()
        self.isdbt_tsp_resize_0_2 = isdbt.tsp_resize()
        self.isdbt_time_interleaver_0 = isdbt.time_interleaver(3, 4, True)
        self.isdbt_sync_adjust_0_1 = isdbt.sync_adjust()
        self.isdbt_sync_adjust_0_0 = isdbt.sync_adjust()
        self.isdbt_sync_adjust_0 = isdbt.sync_adjust()
        self.isdbt_ofdm_frame_structure_0 = isdbt.ofdm_frame_structure(3, True, 3, 3, 3, 0, 0, 0, 4, 4, 4, 1, 6, 6)
        self.isdbt_mapper_0_1 = isdbt.mapper(3, 6, 6, 0.125)
        self.isdbt_mapper_0_0 = isdbt.mapper(3, 6, 6, 0.125)
        self.isdbt_mapper_0 = isdbt.mapper(3, 6, 1, 0.125)
        self.isdbt_hierarchical_divisor_0 = isdbt.hierarchical_divisor(3, 0.125, 1, 6, 0.5, 6, 6, 0.5, 6, 6, 0.5)
        self.isdbt_hierarchical_combination_0 = isdbt.hierarchical_combination(3, 1, 6, 6)
        self.isdbt_frec_interleaver_0 = isdbt.frec_interleaver(3, True)
        self.isdbt_energy_dispersal_0_1 = isdbt.energy_dispersal(72*6)
        self.isdbt_energy_dispersal_0_0 = isdbt.energy_dispersal(432)
        self.isdbt_energy_dispersal_0 = isdbt.energy_dispersal(72)
        self.isdbt_byte_interleaver_0_1 = isdbt.byte_interleaver(3, 0, 6, 6)
        self.isdbt_byte_interleaver_0_0 = isdbt.byte_interleaver(3, 0, 6, 6)
        self.isdbt_byte_interleaver_0 = isdbt.byte_interleaver(3, 0, 6, 1)
        self._if_gain_range = Range(0, 30, 1, 15, 200)
        self._if_gain_win = RangeWidget(self._if_gain_range, self.set_if_gain, 'if_gain', "counter_slider", float)
        self.top_layout.addWidget(self._if_gain_win)
        self.fft_vxx_0 = fft.fft_vcc(4096*2, False, (window.rectangular(4096*2)), True, 1)
        self.dtv_dvbt_reed_solomon_enc_0_0_1 = dtv.dvbt_reed_solomon_enc(2, 8, 0x11d, 255, 239, 8, 51, 1)
        self.dtv_dvbt_reed_solomon_enc_0_0_0 = dtv.dvbt_reed_solomon_enc(2, 8, 0x11d, 255, 239, 8, 51, 1)
        self.dtv_dvbt_reed_solomon_enc_0_0 = dtv.dvbt_reed_solomon_enc(2, 8, 0x11d, 255, 239, 8, 51, 1)
        self.dtv_dvbt_inner_coder_0_1 = dtv.dvbt_inner_coder(1, 1512, dtv.MOD_64QAM, dtv.ALPHA2, dtv.C1_2)
        self.dtv_dvbt_inner_coder_0_0 = dtv.dvbt_inner_coder(1, 1512, dtv.MOD_64QAM, dtv.ALPHA2, dtv.C1_2)
        self.dtv_dvbt_inner_coder_0 = dtv.dvbt_inner_coder(1, 1512, dtv.MOD_64QAM, dtv.ALPHA2, dtv.C1_2)
        self.digital_ofdm_cyclic_prefixer_0 = digital.ofdm_cyclic_prefixer(4096*2, 4096*2+512*2, 0, "")
        self.blocks_vector_to_stream_1_0 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 4096*2)
        self.blocks_vector_to_stream_1 = blocks.vector_to_stream(gr.sizeof_gr_complex*1, 4096*2)
        self.blocks_vector_to_stream_0_1_2 = blocks.vector_to_stream(gr.sizeof_char*1, 1512)
        self.blocks_vector_to_stream_0_1_1 = blocks.vector_to_stream(gr.sizeof_char*1, 1512)
        self.blocks_vector_to_stream_0_1 = blocks.vector_to_stream(gr.sizeof_char*1, 1512)
        self.blocks_vector_to_stream_0_0_2 = blocks.vector_to_stream(gr.sizeof_char*204, 144*6)
        self.blocks_vector_to_stream_0_0_1 = blocks.vector_to_stream(gr.sizeof_char*204, 144*6)
        self.blocks_vector_to_stream_0_0_0_1 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_0_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_0_0 = blocks.vector_to_stream(gr.sizeof_char*1, 204)
        self.blocks_vector_to_stream_0_0 = blocks.vector_to_stream(gr.sizeof_char*204, 144)
        self.blocks_stream_to_vector_1_1 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 384*6)
        self.blocks_stream_to_vector_1_0 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 384*6)
        self.blocks_stream_to_vector_1 = blocks.stream_to_vector(gr.sizeof_gr_complex*1, 384)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_gr_complex*1)
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vcc((variable, ))
        self.blocks_message_debug_0 = blocks.message_debug()
        self.blocks_file_source_0_0_0 = blocks.file_source(gr.sizeof_char*204*2304*2, '/home/habbo/Documents/BTS_Validos/BTS_ISDB_T_3_CAPAS.ts', True)
        self.blocks_complex_to_mag_0 = blocks.complex_to_mag(1)
        self._bb_gain_range = Range(0, 30, 1, 15, 200)
        self._bb_gain_win = RangeWidget(self._bb_gain_range, self.set_bb_gain, 'bb_gain', "counter_slider", float)
        self.top_layout.addWidget(self._bb_gain_win)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_complex_to_mag_0, 0), (self.qtgui_time_sink_x_0, 0))    
        self.connect((self.blocks_file_source_0_0_0, 0), (self.isdbt_hierarchical_divisor_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.blocks_complex_to_mag_0, 0))    
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.rational_resampler_xxx_0, 0))    
        self.connect((self.blocks_stream_to_vector_1, 0), (self.isdbt_hierarchical_combination_0, 0))    
        self.connect((self.blocks_stream_to_vector_1_0, 0), (self.isdbt_hierarchical_combination_0, 1))    
        self.connect((self.blocks_stream_to_vector_1_1, 0), (self.isdbt_hierarchical_combination_0, 2))    
        self.connect((self.blocks_vector_to_stream_0_0, 0), (self.isdbt_tsp_resize_0_2, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_0, 0), (self.dtv_dvbt_inner_coder_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_0_0, 0), (self.dtv_dvbt_inner_coder_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_0_1, 0), (self.dtv_dvbt_inner_coder_0_1, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_1, 0), (self.isdbt_tsp_resize_0_2_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_0_2, 0), (self.isdbt_tsp_resize_0_2_1, 0))    
        self.connect((self.blocks_vector_to_stream_0_1, 0), (self.isdbt_mapper_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_1, 0), (self.isdbt_mapper_0_0, 0))    
        self.connect((self.blocks_vector_to_stream_0_1_2, 0), (self.isdbt_mapper_0_1, 0))    
        self.connect((self.blocks_vector_to_stream_1, 0), (self.qtgui_const_sink_x_0, 0))    
        self.connect((self.blocks_vector_to_stream_1_0, 0), (self.qtgui_freq_sink_x_0, 0))    
        self.connect((self.digital_ofdm_cyclic_prefixer_0, 0), (self.blocks_multiply_const_vxx_0, 0))    
        self.connect((self.dtv_dvbt_inner_coder_0, 0), (self.blocks_vector_to_stream_0_1, 0))    
        self.connect((self.dtv_dvbt_inner_coder_0_0, 0), (self.blocks_vector_to_stream_0_1_1, 0))    
        self.connect((self.dtv_dvbt_inner_coder_0_1, 0), (self.blocks_vector_to_stream_0_1_2, 0))    
        self.connect((self.dtv_dvbt_reed_solomon_enc_0_0, 0), (self.isdbt_sync_adjust_0, 0))    
        self.connect((self.dtv_dvbt_reed_solomon_enc_0_0_0, 0), (self.isdbt_sync_adjust_0_0, 0))    
        self.connect((self.dtv_dvbt_reed_solomon_enc_0_0_1, 0), (self.isdbt_sync_adjust_0_1, 0))    
        self.connect((self.fft_vxx_0, 0), (self.blocks_vector_to_stream_1_0, 0))    
        self.connect((self.fft_vxx_0, 0), (self.digital_ofdm_cyclic_prefixer_0, 0))    
        self.connect((self.isdbt_byte_interleaver_0, 0), (self.blocks_vector_to_stream_0_0_0, 0))    
        self.connect((self.isdbt_byte_interleaver_0_0, 0), (self.blocks_vector_to_stream_0_0_0_0, 0))    
        self.connect((self.isdbt_byte_interleaver_0_1, 0), (self.blocks_vector_to_stream_0_0_0_1, 0))    
        self.connect((self.isdbt_energy_dispersal_0, 0), (self.isdbt_byte_interleaver_0, 0))    
        self.connect((self.isdbt_energy_dispersal_0_0, 0), (self.isdbt_byte_interleaver_0_0, 0))    
        self.connect((self.isdbt_energy_dispersal_0_1, 0), (self.isdbt_byte_interleaver_0_1, 0))    
        self.connect((self.isdbt_frec_interleaver_0, 0), (self.isdbt_ofdm_frame_structure_0, 0))    
        self.connect((self.isdbt_hierarchical_combination_0, 0), (self.isdbt_time_interleaver_0, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 0), (self.blocks_vector_to_stream_0_0, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 1), (self.blocks_vector_to_stream_0_0_1, 0))    
        self.connect((self.isdbt_hierarchical_divisor_0, 2), (self.blocks_vector_to_stream_0_0_2, 0))    
        self.connect((self.isdbt_mapper_0, 0), (self.blocks_stream_to_vector_1, 0))    
        self.connect((self.isdbt_mapper_0_0, 0), (self.blocks_stream_to_vector_1_0, 0))    
        self.connect((self.isdbt_mapper_0_1, 0), (self.blocks_stream_to_vector_1_1, 0))    
        self.connect((self.isdbt_ofdm_frame_structure_0, 0), (self.blocks_vector_to_stream_1, 0))    
        self.connect((self.isdbt_ofdm_frame_structure_0, 0), (self.fft_vxx_0, 0))    
        self.connect((self.isdbt_sync_adjust_0, 0), (self.isdbt_energy_dispersal_0, 0))    
        self.connect((self.isdbt_sync_adjust_0_0, 0), (self.isdbt_energy_dispersal_0_0, 0))    
        self.connect((self.isdbt_sync_adjust_0_1, 0), (self.isdbt_energy_dispersal_0_1, 0))    
        self.connect((self.isdbt_time_interleaver_0, 0), (self.isdbt_frec_interleaver_0, 0))    
        self.connect((self.isdbt_tsp_resize_0_2, 0), (self.dtv_dvbt_reed_solomon_enc_0_0, 0))    
        self.connect((self.isdbt_tsp_resize_0_2_0, 0), (self.dtv_dvbt_reed_solomon_enc_0_0_0, 0))    
        self.connect((self.isdbt_tsp_resize_0_2_1, 0), (self.dtv_dvbt_reed_solomon_enc_0_0_1, 0))    
        self.connect((self.rational_resampler_xxx_0, 0), (self.blocks_null_sink_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_variable(self):
        return self.variable

    def set_variable(self, variable):
        self.variable = variable
        self.blocks_multiply_const_vxx_0.set_k((self.variable, ))

    def get_samp_rate_0(self):
        return self.samp_rate_0

    def set_samp_rate_0(self, samp_rate_0):
        self.samp_rate_0 = samp_rate_0

    def get_rf_gain(self):
        return self.rf_gain

    def set_rf_gain(self, rf_gain):
        self.rf_gain = rf_gain

    def get_if_gain(self):
        return self.if_gain

    def set_if_gain(self, if_gain):
        self.if_gain = if_gain

    def get_bb_gain(self):
        return self.bb_gain

    def set_bb_gain(self, bb_gain):
        self.bb_gain = bb_gain


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
