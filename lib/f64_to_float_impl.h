/* -*- c++ -*- */
/*
 * Copyright 2016 Analog Devices Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_F64_TO_FLOAT_IMPL_H
#define INCLUDED_F64_TO_FLOAT_IMPL_H

#include <gnuradio/iio/f64_to_float.h>

namespace gr {
	namespace iio {

		class IIO_API f64_to_float_impl : public f64_to_float
		{
			size_t d_vlen;

		public:
			f64_to_float_impl(size_t vlen);

			int work(int noutput_items,
					gr_vector_const_void_star &input_items,
					gr_vector_void_star &output_items);
		};

	} /* namespace iio */
} /* namespace gr */


#endif /* INCLUDED_F64_TO_FLOAT_IMPL_H */
