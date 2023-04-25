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
#include "mesCreater_impl.h"

namespace gr {
  namespace CounterClockwiseAlarms {

    mesCreater::sptr
    mesCreater::make(uint8_t mesDownId,uint8_t sf,uint8_t framelen)
    {
      return gnuradio::get_initial_sptr
        (new mesCreater_impl(mesDownId,sf,framelen));
    }


    /*
     * The private constructor
     */
    mesCreater_impl::mesCreater_impl(uint8_t mesDownId,uint8_t sf,uint8_t framelen)
      : gr::block("mesCreater",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(uint8_t)))
    {
      m_mesDownId = mesDownId;
      m_sf = sf;
      m_framelen =  framelen;
      if( m_mesDownId < 0 && m_mesDownId >= ( 1 << sf ) ){
        std::cout<<RED<<"MES ID is out of range: "<<m_mesDownId<<" sf: "<<m_sf<<std::endl;
        exit(0);
      }
      m_outFile.open("msgCreaterRecord.txt",std::ios::out | std::ios::trunc);
      m_outFile<<"Id\tpayloadStr"<<std::endl;
    }

    /*
     * Our virtual destructor.
     */
    mesCreater_impl::~mesCreater_impl()
    {
      m_outFile.close();
    }

    void
    mesCreater_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    mesCreater_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const uint8_t *in = (const uint8_t *) input_items[0];
      uint8_t *out = (uint8_t *) output_items[0];
      noutput_items = 0;
      if(!m_sendMes){
        return noutput_items;
      }


      pmt_t frame_len = pmt::from_long(framelen); //通过几个标点符号决定信标长度，一般由一个chirp决定
      std::string payload_str;
      payload_str.push(m_mesDownId);
      m_outFile<<m_mesDownId<<"\t"<<(char)m_mesDownId<<std::endl;
      add_item_tag(0,nitems_written(0),pmt::string_to_symbol("frame_len"),frame_len);
      add_item_tag(0,nitems_written(0),pmt::string_to_symbol("payload_str"),pmt::string_to_symbol(payload_str));

      //Id为32位数据，将32位数字转换为4个8进制数组进行传输
      out[0] = m_mesDownId;
      
      noutput_items = frame_len;

      consume_each (0);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace CounterClockwiseAlarms */
} /* namespace gr */

