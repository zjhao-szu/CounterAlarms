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
#include "ReceiveDown_impl.h"

namespace gr {
  namespace CounterClockwiseAlarms {

    ReceiveDown::sptr
    ReceiveDown::make(uint8_t sf, bool impl_head)
    {
      return gnuradio::get_initial_sptr
        (new ReceiveDown_impl(sf,impl_head));
    }


    /*
     * The private constructor
     */
    ReceiveDown_impl::ReceiveDown_impl(uint8_t sf, bool impl_head)
      : gr::block("ReceiveDown",
              gr::io_signature::make(1, 1, (1u << sf)*sizeof(gr_complex)),
              gr::io_signature::make(0, 1, sizeof(uint32_t)))
    {
      m_sf = sf;

      m_samples_per_symbol = (uint32_t)(1u << m_sf);
      m_upchirp.resize(m_samples_per_symbol);
      m_downchirp.resize(m_samples_per_symbol);

      // FFT demodulation preparations
      m_fft.resize(m_samples_per_symbol);
      m_dechirped.resize(m_samples_per_symbol);

      set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    ReceiveDown_impl::~ReceiveDown_impl()
    {
    }

    void
    ReceiveDown_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
       ninput_items_required[0] = 1;
    }

    int32_t ReceiveDown_impl::get_symbol_val(const gr_complex *samples) {
        float m_fft_mag[m_samples_per_symbol];
        float rec_en=0;
        kiss_fft_cfg cfg =  kiss_fft_alloc(m_samples_per_symbol,0,0,0);
        kiss_fft_cpx *cx_in = new kiss_fft_cpx[m_samples_per_symbol];
        kiss_fft_cpx *cx_out = new kiss_fft_cpx[m_samples_per_symbol];

        // Multiply with ideal upchirp
        volk_32fc_x2_multiply_32fc(&m_dechirped[0],samples,&m_upchirp[0],m_samples_per_symbol);
        for (int i = 0; i < m_samples_per_symbol; i++) {
          cx_in[i].r = m_dechirped[i].real();
          cx_in[i].i = m_dechirped[i].imag();
        }
        //do the FFT
        kiss_fft(cfg,cx_in,cx_out);
        // Get magnitude
        for (uint32_t i = 0u; i < m_samples_per_symbol; i++) {
            m_fft_mag[i] = cx_out[i].r*cx_out[i].r+cx_out[i].i*cx_out[i].i;
            rec_en+=m_fft_mag[i];
        }

        free(cfg);
        delete[] cx_in;
        delete[] cx_out;
        // Return argmax

        int idx = std::max_element(m_fft_mag, m_fft_mag + m_samples_per_symbol) - m_fft_mag;
       
        return (idx);
    }
    void ReceiveDown_impl::new_frame_handler(int cfo_int){
        //create downchirp taking CFOint into account
        build_upchirp(&m_upchirp[0],mod(cfo_int,m_samples_per_symbol),m_sf);
        volk_32fc_conjugate_32fc(&m_downchirp[0],&m_upchirp[0],m_samples_per_symbol);
        output.clear();
      
    };
    int
    ReceiveDown_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      uint32_t *out = (uint32_t *) output_items[0];
      
      int to_output=0;
      std::vector<tag_t> tags;
      get_tags_in_window(tags,0,0,1,pmt::string_to_symbol("frame_info"));

      if(tags.size()){
        pmt::pmt_t err = pmt::string_to_symbol("error");
        int cfo_int = pmt::to_long (pmt::dict_ref(tags[0].value,pmt::string_to_symbol("cfo_int"),err));
        new_frame_handler(cfo_int);
        tags[0].offset = nitems_written(0);
        add_item_tag(0, tags[0]); //8 LoRa symbols in the header
      }      
      //shift by -1 and use reduce rate if first block (header)
      output.push_back(mod(get_symbol_val(in)-1,(1<<m_sf)));
      block_size = 1;
      if((output.size() == block_size)){
          memcpy(&out[0],&output[0],block_size*sizeof(uint32_t));
          output.clear();
          to_output = block_size;
      }
      else
          to_output = 0;
      consume_each(1);

      // Tell runtime system how many output items we produced.
      return to_output;
    }

  } /* namespace CounterClockwiseAlarms */
} /* namespace gr */

