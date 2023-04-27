/* -*- c++ -*- */

#define COUNTERCLOCKWISEALARMS_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "CounterClockwiseAlarms_swig_doc.i"

%{
#include "CounterClockwiseAlarms/mesCreater.h"
#include "CounterClockwiseAlarms/crcAppend.h"
#include "CounterClockwiseAlarms/DownModulate.h"
#include "CounterClockwiseAlarms/ReceiveDown.h"
#include "CounterClockwiseAlarms/Crc_verif.h"
#include "CounterClockwiseAlarms/FrameSync.h"
%}

%include "CounterClockwiseAlarms/mesCreater.h"
GR_SWIG_BLOCK_MAGIC2(CounterClockwiseAlarms, mesCreater);
%include "CounterClockwiseAlarms/crcAppend.h"
GR_SWIG_BLOCK_MAGIC2(CounterClockwiseAlarms, crcAppend);
%include "CounterClockwiseAlarms/DownModulate.h"
GR_SWIG_BLOCK_MAGIC2(CounterClockwiseAlarms, DownModulate);
%include "CounterClockwiseAlarms/ReceiveDown.h"
GR_SWIG_BLOCK_MAGIC2(CounterClockwiseAlarms, ReceiveDown);
%include "CounterClockwiseAlarms/Crc_verif.h"
GR_SWIG_BLOCK_MAGIC2(CounterClockwiseAlarms, Crc_verif);
%include "CounterClockwiseAlarms/FrameSync.h"
GR_SWIG_BLOCK_MAGIC2(CounterClockwiseAlarms, FrameSync);
