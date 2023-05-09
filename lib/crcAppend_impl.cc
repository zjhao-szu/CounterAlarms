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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "crcAppend_impl.h"

namespace gr {
  namespace CounterClockwiseAlarms {

    crcAppend::sptr
    crcAppend::make(bool has_crc)
    {
      return gnuradio::get_initial_sptr
        (new crcAppend_impl(has_crc));
    }


    /*
     * The private constructor
     */
    crcAppend_impl::crcAppend_impl(bool has_crc)
      : gr::block("crcAppend",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(uint32_t)))
    {
      m_has_crc = has_crc;
      set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    crcAppend_impl::~crcAppend_impl()
    {
    }


    unsigned int Calculate_crc16(std::vector<uint8_t> DAT, unsigned int length);
    {
        unsigned int CRC = 0xffff;
        unsigned char i;
        unsigned char j;
        for (i = 0; i < length; i++)
        {
            CRC = CRC ^ DAT[i];
            for(j = 0; j < 8;j++){
                if(CRC & 0x01){
                    CRC = CRC >> 1;
                    CRC = CRC ^ 0xA001;
                }else{
                    CRC = CRC >> 1;
                }
                
            }
        }
        return CRC;
    }


    void
    crcAppend_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = 1;
    }

    int
    crcAppend_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const uint8_t *in = (const uint8_t *) input_items[0];
      uint32_t *out = (uint32_t *) output_items[0];

      int nitems_to_output = 0;
      noutput_items = std::max(0, noutput_items - 1);//take margin to output CRC
      int nitems_to_process = std::min(ninput_items[0], noutput_items);

      // read tags
      std::vector<tag_t> tags;
      get_tags_in_window(tags, 0, 0, noutput_items, pmt::string_to_symbol("payload_str"));
      if (tags.size())
      {
          if (tags[0].offset != nitems_read(0))
          {
              nitems_to_process = std::min(tags[0].offset - nitems_read(0), (uint64_t)noutput_items);
          }
          else
          {
              if (tags.size() >= 2)
              {
                  nitems_to_process = std::min(tags[1].offset - tags[0].offset, (uint64_t)noutput_items);
              }
              std::string str = pmt::symbol_to_string(tags[0].value);
              std::copy(str.begin(), str.end(), std::back_inserter(m_payload));
              //pass tags downstream
              get_tags_in_window(tags, 0, 0, ninput_items[0], pmt::string_to_symbol("frame_len"));
              m_frame_len = pmt::to_long(tags[0].value);
              tags[0].offset = nitems_written(0);
              tags[0].value = pmt::from_long(m_frame_len + (m_has_crc ? 4 : 0));

              if (nitems_to_process)
                  add_item_tag(0, tags[0]);

              m_cnt = 0;
          }
      }
      if (!nitems_to_process)
      {
          return 0;
      }
      m_cnt += nitems_to_process;
      if (m_has_crc && m_cnt == m_frame_len && nitems_to_process)
      { //append the CRC to the payload
          unsigned int crc = 0x0000;
          m_payload_len = m_payload.size();
          //calculate CRC on the data bytes using Poly=1021 Init=0000
          Calculate_crc16(m_payload,m_payload_len);
          unsigned char first = (value >> 8) & 0xff;
	        unsigned char second = value & 0xff; 
          //Place the CRC in the correct output nibble
          out[nitems_to_process] = second;
          out[nitems_to_process+1] = first;

          nitems_to_output = nitems_to_process + 2;
          m_payload.clear();
      }
      else
      {
          nitems_to_output = nitems_to_process;
      }
      for(int i = 0 ;i < nitems_to_process;i++){
        out[i] = in[i];
      }
      
      consume_each(nitems_to_process);

      // Tell runtime system how many output items we produced.
      return nitems_to_output;
    }

  } /* namespace CounterClockwiseAlarms */
} /* namespace gr */

