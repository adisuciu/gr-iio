/* -*- c++ -*- */
/*
 * Copyright 2017 Analog Devices Inc.
 * Author: Paul Cercueil <paul.cercueil@analog.com>
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

#include "pluto_source_impl.h"

#include <iio.h>
#include <stdio.h>

namespace gr {
  namespace iio {

pluto_source::sptr pluto_source::make(const std::string& uri,
		unsigned long long frequency, unsigned long samplerate,
		unsigned long bandwidth,
		unsigned long buffer_size, bool quadrature, bool rfdc,
		bool bbdc, const char *gain, double gain_value,
		const char *filter, bool auto_filter)
{
	fmcomms2_source::sptr block = fmcomms2_source::make(
			uri.empty() ? pluto_source_impl::get_uri() : uri,
			frequency, samplerate,
			bandwidth, true, true, false, false,
			buffer_size, quadrature, rfdc, bbdc,
			gain, gain_value, NULL, 0.0, "A_BALANCED",
			filter, auto_filter);

	return gnuradio::get_initial_sptr(
			new pluto_source_impl(block));
}

std::string pluto_source_impl::get_uri()
{
	struct iio_scan_context *ctx = iio_create_scan_context("usb", 0);
	if (!ctx)
		throw std::runtime_error("Unable to create scan context");

	struct iio_context_info **info;
	int ret = iio_scan_context_get_info_list(ctx, &info);
	if (ret < 0) {
		iio_scan_context_destroy(ctx);
		throw std::runtime_error("Unable to scan for Pluto devices");
	}

	if (ret == 0) {
		iio_context_info_list_free(info);
		iio_scan_context_destroy(ctx);
		throw std::runtime_error("No Pluto device found");
	}

	if (ret > 1) {
		printf("More than one Pluto found:\n");

		for (unsigned int i = 0; i < (size_t) ret; i++) {
			printf("\t%d: %s [%s]\n", i,
				iio_context_info_get_description(info[i]),
				iio_context_info_get_uri(info[i]));
		}

		printf("We will use the first one.\n");
	}

	std::string uri(iio_context_info_get_uri(info[0]));
	iio_context_info_list_free(info);
	iio_scan_context_destroy(ctx);

	return uri;
}

pluto_source_impl::pluto_source_impl(fmcomms2_source::sptr block) :
	hier_block2("pluto_source",
			io_signature::make(0, 0, 0),
			io_signature::make(1, 1, sizeof(gr_complex))),
	fmcomms2_source_f32c(true, false, block)
{
}

void pluto_source_impl::set_params(unsigned long long frequency,
		unsigned long samplerate,
		unsigned long bandwidth,
		bool quadrature, bool rfdc, bool bbdc,
		const char *gain, double gain_value,
		const char *filter, bool auto_filter)
{
	fmcomms2_source_f32c::set_params(frequency, samplerate, bandwidth,
			quadrature, rfdc, bbdc, gain, gain_value, NULL, 0.0,
			"A_BALANCED", filter, auto_filter);
}

        } // namespace iio
} // namespace gr
