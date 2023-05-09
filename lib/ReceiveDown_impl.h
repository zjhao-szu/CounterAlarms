/* -*- c++ -*- */
/*
 * Copyright 2023 zjhao.
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
 */

#ifndef INCLUDED_COUNTERCLOCKWISEALARMS_RECEIVEDOWN_IMPL_H
#define INCLUDED_COUNTERCLOCKWISEALARMS_RECEIVEDOWN_IMPL_H

#include <CounterClockwiseAlarms/ReceiveDown.h>

namespace gr {
  namespace CounterClockwiseAlarms {

    class ReceiveDown_impl : public ReceiveDown
    {
     private:
      uint8_t m_sf;           ///< Spreading factor
      uint8_t m_cr;           ///< Coding rate

      uint32_t m_samples_per_symbol;  ///< Number of samples received per lora symbols
      int CFOint; ///< integer part of the CFO

      // variable used to perform the FFT demodulation
      std::vector<gr_complex> m_upchirp;   ///< Reference upchirp
      std::vector<gr_complex> m_downchirp; ///< Reference downchirp
      std::vector<gr_complex> m_dechirped; ///< Dechirped symbol
      std::vector<gr_complex> m_fft;       ///< Result of the FFT


      std::vector<uint32_t> output;   ///< Stores the value to be outputted once a full bloc has been received
      bool is_header;                  ///< Indicate that the first block hasn't been fully received
      uint8_t block_size;             ///< The number of lora symbol in one block


      #ifdef GRLORA_MEASUREMENTS
      std::ofstream energy_file;
      #endif
      #ifdef GRLORA_DEBUG
      std::ofstream idx_file;
      #endif

      /**
       *  \brief  Recover the lora symbol value using argmax of the dechirped symbol FFT.
       *
       *  \param  samples
       *          The pointer to the symbol beginning.
       */
      int32_t get_symbol_val(const gr_complex *samples);

      /**
       *  \brief  Reset the block variables when a new lora packet needs to be decoded.
       */
      void new_frame_handler(int cfo_int);

      /**
       *  \brief  Handles the reception of the coding rate received by the header_decoder block.
       */
      void header_cr_handler(pmt::pmt_t cr);

     public:
      ReceiveDown_impl(uint8_t sf, bool impl_head);
      ~ReceiveDown_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace CounterClockwiseAlarms
} // namespace gr

#endif /* INCLUDED_COUNTERCLOCKWISEALARMS_RECEIVEDOWN_IMPL_H */

