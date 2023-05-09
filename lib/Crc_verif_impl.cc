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
#include "Crc_verif_impl.h"

namespace gr {
  namespace CounterClockwiseAlarms {

    Crc_verif::sptr
    Crc_verif::make(double frequency,uint8_t sf)
    {
      return gnuradio::get_initial_sptr
        (new Crc_verif_impl(frequency,sf));
    }


    /*
     * The private constructor
     */
    Crc_verif_impl::Crc_verif_impl(double frequency,uint8_t sf)
      : gr::block("Crc_verif",
              gr::io_signature::make(1, 1, sizeof(uint32_t)),
              gr::io_signature::make(0, 1, sizeof(uint8_t))),
        m_frequency(frequency),
        m_sf(sf)
    {
        m_payload_len = 1 + m_crc_presence * 4;
         message_port_register_out(pmt::mp("msg"));
    }

    /*
     * Our virtual destructor.
     */
    Crc_verif_impl::~Crc_verif_impl()
    {
    }

    void
    Crc_verif_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = 1; //m_payload_len;
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

    int
    Crc_verif_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const uint32_t *in = (const uint32_t *) input_items[0];
      uint8_t *out = (uint8_t *) output_items[0];
      if(ninput_items[0] >= (int)m_payload_len){
        for(int i = 0;i < m_payload_len;i++){
          in_buff.push_back(in[i]);
        }
        int crcResult = Calculate_crc16(in_buff,m_payload_len);
        if(crcResult == 0){
          std::cout<<"CRC valid"<<std::endl;
        }else{
          std::cout<<"CRC Invalid"<<std::endl;
        }
        out[0] = in[0];
        message_port_pub(pmt::intern("msg"), pmt::from_uint64(in[0]));
         consume_each (m_payload_len);
        return 1;
      }else{
        return 0;
      }
      
      
      
      // Tell runtime system how many input items we consumed on
      // each input stream.
     
    }

  } /* namespace CounterClockwiseAlarms */
} /* namespace gr */

