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

#ifndef INCLUDED_COUNTERCLOCKWISEALARMS_CRCAPPEND_IMPL_H
#define INCLUDED_COUNTERCLOCKWISEALARMS_CRCAPPEND_IMPL_H

#include <CounterClockwiseAlarms/crcAppend.h>

namespace gr {
  namespace CounterClockwiseAlarms {

    class crcAppend_impl : public crcAppend
    {
     private:
      bool m_has_crc;
      std::vector<uint8_t> m_payload; 
      uint8_t m_payload_len;
      int m_frame_len;
      unsigned int Caculate_crc16(std::vector<uint8_t> DAT, unsigned int length);
     public:
      crcAppend_impl(bool has_crc);
      ~crcAppend_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace CounterClockwiseAlarms
} // namespace gr

#endif /* INCLUDED_COUNTERCLOCKWISEALARMS_CRCAPPEND_IMPL_H */

