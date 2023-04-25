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

#ifndef INCLUDED_COUNTERCLOCKWISEALARMS_MESCREATER_H
#define INCLUDED_COUNTERCLOCKWISEALARMS_MESCREATER_H

#include <CounterClockwiseAlarms/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace CounterClockwiseAlarms {

    /*!
     * \brief <+description of block+>
     * \ingroup CounterClockwiseAlarms
     *
     */
    class COUNTERCLOCKWISEALARMS_API mesCreater : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<mesCreater> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of CounterClockwiseAlarms::mesCreater.
       *
       * To avoid accidental use of raw pointers, CounterClockwiseAlarms::mesCreater's
       * constructor is in a private implementation
       * class. CounterClockwiseAlarms::mesCreater::make is the public interface for
       * creating new instances.
       */
      static sptr make(uint8_t mesDownId,uint8_t sf,uint8_t framelen);
    };

  } // namespace CounterClockwiseAlarms
} // namespace gr

#endif /* INCLUDED_COUNTERCLOCKWISEALARMS_MESCREATER_H */

