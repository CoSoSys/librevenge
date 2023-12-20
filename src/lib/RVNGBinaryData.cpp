/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* librevenge
 * Version: MPL 2.0 / LGPLv2.1+
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Major Contributor(s):
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Lesser General Public License Version 2.1 or later
 * (LGPLv2.1+), in which case the provisions of the LGPLv2.1+ are
 * applicable instead of those above.
 */

#include "librevenge.h"



#include <algorithm>
#include <iterator>
#include <memory>
#include <vector>
#include <string>
#include <stdarg.h>
#include <cstdio>

#include "RVNGMemoryStream.h"


namespace librevenge
{

namespace
{

struct DataImpl
{
	DataImpl() : m_buf(), m_stream() {}

	std::vector<unsigned char> m_buf;
	std::unique_ptr<RVNGMemoryInputStream> m_stream;
};

void convertToBase64(std::string &result, const std::vector<unsigned char> & /*source*/)
{
	result.clear();

	// TODO: this might be used ==> reimplement (Dani)

	//auto numPadding = unsigned((3- (source.size()%3)) %3);

	//typedef boost::archive::iterators::base64_from_binary<
	//boost::archive::iterators::transform_width<std::vector<unsigned char>::const_iterator, 6, 8 > > base64_encoder;

	//// Encode the buffer and create a string
	//std::copy(
	//    base64_encoder(source.begin()),
	//    base64_encoder(source.end()), std::back_inserter(result));

	//result.append(numPadding, '=');  // add '=' for each padded character
}

} // anonymous namespace

struct RVNGBinaryDataImpl
{
	RVNGBinaryDataImpl();

	void makeUnique();

	std::shared_ptr<DataImpl> m_ptr;
};

RVNGBinaryDataImpl::RVNGBinaryDataImpl()
	: m_ptr(new DataImpl())
{
}

void RVNGBinaryDataImpl::makeUnique()
{
	if (!m_ptr.unique())
	{
		std::shared_ptr<DataImpl> ptr(new DataImpl());
		ptr->m_buf = m_ptr->m_buf;
		m_ptr = ptr;
	}
}

RVNGBinaryData::~RVNGBinaryData()
{
	delete m_binaryDataImpl;
}

RVNGBinaryData::RVNGBinaryData() :
	m_binaryDataImpl(new RVNGBinaryDataImpl)
{
}

RVNGBinaryData::RVNGBinaryData(const RVNGBinaryData &data) :
	m_binaryDataImpl(new RVNGBinaryDataImpl)
{
	m_binaryDataImpl->m_ptr = data.m_binaryDataImpl->m_ptr;
}

RVNGBinaryData::RVNGBinaryData(const unsigned char *buffer, const unsigned long bufferSize) :
	m_binaryDataImpl(nullptr)
{
	std::unique_ptr<RVNGBinaryDataImpl> impl(new RVNGBinaryDataImpl());
	if (buffer)
		impl->m_ptr->m_buf.assign(buffer, buffer + bufferSize);
	m_binaryDataImpl = impl.release();
}

RVNGBinaryData::RVNGBinaryData(const RVNGString &/*base64*/) :
	m_binaryDataImpl(nullptr)
{
	// TODO: this function might be used ==> reimplement trim (Dani)

	/*std::unique_ptr<RVNGBinaryDataImpl> impl(new RVNGBinaryDataImpl());
	std::string base64String(base64.cstr(), base64.size());
	boost::trim(base64String);
	convertFromBase64(impl->m_ptr->m_buf, base64String);
	m_binaryDataImpl = impl.release();*/
}

RVNGBinaryData::RVNGBinaryData(const char * /*base64*/) :
	m_binaryDataImpl(nullptr)
{
	std::unique_ptr<RVNGBinaryDataImpl> impl(new RVNGBinaryDataImpl());

	// TODO: this portion might be used ==> reimplement trim (Dani)
	/*if (base64)
	{
		std::string base64String(base64);
		boost::trim(base64String);
		convertFromBase64(impl->m_ptr->m_buf, base64String);
	}*/
	m_binaryDataImpl = impl.release();
}

void RVNGBinaryData::append(const RVNGBinaryData &data)
{
	m_binaryDataImpl->makeUnique();

	unsigned long previousSize = (unsigned long) m_binaryDataImpl->m_ptr->m_buf.size();
	m_binaryDataImpl->m_ptr->m_buf.reserve(previousSize + data.m_binaryDataImpl->m_ptr->m_buf.size());
	const auto &src = data.m_binaryDataImpl->m_ptr->m_buf;
	std::copy(src.begin(), src.end(), std::back_inserter(m_binaryDataImpl->m_ptr->m_buf));
}

void RVNGBinaryData::append(const unsigned char *buffer, const unsigned long bufferSize)
{
	if (buffer && bufferSize > 0)
	{
		m_binaryDataImpl->makeUnique();

		std::vector<unsigned char> &buf = m_binaryDataImpl->m_ptr->m_buf;
		buf.reserve(buf.size() + bufferSize);
		buf.insert(buf.end(), buffer, buffer + bufferSize);
	}
}

void RVNGBinaryData::append(const unsigned char c)
{
	m_binaryDataImpl->makeUnique();

	m_binaryDataImpl->m_ptr->m_buf.push_back(c);
}

void RVNGBinaryData::clear()
{
	m_binaryDataImpl->makeUnique();

	// clear and return allocated memory
	std::vector<unsigned char>().swap(m_binaryDataImpl->m_ptr->m_buf);
}

unsigned long RVNGBinaryData::size() const
{
	return (unsigned long)m_binaryDataImpl->m_ptr->m_buf.size();
}
bool RVNGBinaryData::empty() const
{
	return (unsigned long)m_binaryDataImpl->m_ptr->m_buf.empty();
}

RVNGBinaryData &RVNGBinaryData::operator=(const RVNGBinaryData &dataBuf)
{
	m_binaryDataImpl->m_ptr = dataBuf.m_binaryDataImpl->m_ptr;
	return *this;
}

const unsigned char *RVNGBinaryData::getDataBuffer() const
{
	if (m_binaryDataImpl->m_ptr->m_buf.empty())
		return nullptr;
	return m_binaryDataImpl->m_ptr->m_buf.data();
}

const RVNGString RVNGBinaryData::getBase64Data() const
{
	std::string base64;
	convertToBase64(base64, m_binaryDataImpl->m_ptr->m_buf);
	return RVNGString(base64.c_str());
}

RVNGInputStream *RVNGBinaryData::getDataStream() const
{
	std::shared_ptr<DataImpl> data = m_binaryDataImpl->m_ptr;
	if (data->m_stream)
	{
		data->m_stream.reset();
	}
	if (data->m_buf.empty())
		return nullptr;
	data->m_stream.reset(new RVNGMemoryInputStream(data->m_buf.data(), (unsigned long) data->m_buf.size()));
	return data->m_stream.get();
}

}

/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
