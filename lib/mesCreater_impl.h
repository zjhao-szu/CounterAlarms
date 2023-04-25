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

#ifndef INCLUDED_COUNTERCLOCKWISEALARMS_MESCREATER_IMPL_H
#define INCLUDED_COUNTERCLOCKWISEALARMS_MESCREATER_IMPL_H

#include <CounterClockwiseAlarms/mesCreater.h>
#include <string>
#include <iostream>
namespace gr {
  namespace CounterClockwiseAlarms {

    class mesCreater_impl : public mesCreater
    {
     private:
      // Nothing to declare in this block.
      uint8_t m_sf;
      uint8_t m_mesDownId;
      uint8_t m_framelen;
      std::ofstream m_outFile;
     public:
      mesCreater_impl(uint8_t mesDownId,uint8_t sf,uint8_t framelen);
      ~mesCreater_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

    };

  } // namespace CounterClockwiseAlarms
} // namespace gr

#endif /* INCLUDED_COUNTERCLOCKWISEALARMS_MESCREATER_IMPL_H */

