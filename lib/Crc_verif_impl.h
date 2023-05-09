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

#ifndef INCLUDED_COUNTERCLOCKWISEALARMS_CRC_VERIF_IMPL_H
#define INCLUDED_COUNTERCLOCKWISEALARMS_CRC_VERIF_IMPL_H

#include <CounterClockwiseAlarms/Crc_verif.h>

namespace gr {
  namespace CounterClockwiseAlarms {

    class Crc_verif_impl : public Crc_verif
    {
     private:
        uint32_t m_payload_len;///< Payload length in bytes
        bool m_crc_presence;///< Indicate if there is a payload CRC
        uint16_t m_crc;///< The CRC calculated from the received payload
        std::string message_str;///< The payload string
        char m_char;///< A new char of the payload
        bool new_frame; ///<indicate a new frame
        std::vector<uint8_t> in_buff;///< input buffer containing the data bytes and CRC if any

        uint32_t cnt=0;///< count the number of frame
        uint8_t m_frequency,m_sf;


        /**
         *  \brief  Handles the payload length received from the header_decoder block.
         */
        void header_pay_len_handler(pmt::pmt_t payload_len);
        /**
         *  \brief  Handles the crc_presence received from the header_decoder block.
         */
        void header_crc_handler(pmt::pmt_t crc_presence);
        /**
         *  \brief  Calculate the CRC 16 using poly=0x1021 and Init=0x0000
         *
         *  \param  data
         *          The pointer to the data beginning.
         *  \param  len
         *          The length of the data in bytes.
         */
        unsigned int Calculate_crc16(uint8_t* data, uint32_t len);

     public:
      Crc_verif_impl(double frequency,uint8_t sf);
      ~Crc_verif_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace CounterClockwiseAlarms
} // namespace gr

#endif /* INCLUDED_COUNTERCLOCKWISEALARMS_CRC_VERIF_IMPL_H */

