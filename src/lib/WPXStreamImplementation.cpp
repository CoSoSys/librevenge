/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2007 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2007 Novell, Inc. (http://www.novell.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02111-1301 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

#include "WPXStreamImplementation.h"
#include "WPXOLEStream.h"

#include <fstream>
#include <sstream>
#include <string>
#include <limits>

using namespace libwpd;

class WPXFileStreamPrivate
{
public:
	WPXFileStreamPrivate();
	~WPXFileStreamPrivate();
	std::fstream file;
	std::stringstream buffer;
	unsigned long streamSize;
	unsigned char *buf;
	unsigned char *readBuffer;
	unsigned long readBufferLength;
	unsigned long readBufferPos;
private:
	WPXFileStreamPrivate(const WPXFileStreamPrivate &);
	WPXFileStreamPrivate &operator=(const WPXFileStreamPrivate &);
};

class WPXStringStreamPrivate
{
public:
	WPXStringStreamPrivate(const std::string str);
	~WPXStringStreamPrivate();
	std::stringstream buffer;
	unsigned long streamSize;
	unsigned char *buf;
private:
	WPXStringStreamPrivate(const WPXStringStreamPrivate &);
	WPXStringStreamPrivate &operator=(const WPXStringStreamPrivate &);
};

WPXFileStreamPrivate::WPXFileStreamPrivate() :
	file(),
	buffer(std::ios::binary | std::ios::in | std::ios::out),
	streamSize(0),
	buf(0),
	readBuffer(0),
	readBufferLength(0),
	readBufferPos(0)
{
}

WPXFileStreamPrivate::~WPXFileStreamPrivate()
{
	if (buf)
		delete [] buf;
	if (readBuffer)
		delete [] readBuffer;
}

WPXStringStreamPrivate::WPXStringStreamPrivate(const std::string str) :
	buffer(str, std::ios::binary | std::ios::in),
	streamSize(0),
	buf(0)
{
}

WPXStringStreamPrivate::~WPXStringStreamPrivate()
{
	if (buf)
		delete [] buf;
}

WPXFileStream::WPXFileStream(const char *filename) :
	WPXInputStream(),
	d(new WPXFileStreamPrivate)
{
	d->file.open( filename, std::ios::binary | std::ios::in );
	d->file.seekg( 0, std::ios::end );
	d->streamSize = (d->file.good() ? (unsigned long)d->file.tellg() : (unsigned long)-1L);
	if (d->streamSize == (unsigned long)-1) // tellg() returned ERROR
		d->streamSize = 0;
	// preventing possible unsigned/signed issues later by truncating the file
	if (d->streamSize > (std::numeric_limits<unsigned long>::max)() / 2)
		d->streamSize = (std::numeric_limits<unsigned long>::max)() / 2;
	d->file.seekg( 0, std::ios::beg );
}

WPXFileStream::~WPXFileStream()
{
	delete d;
}

#define BUFFER_MAX 65536

const unsigned char *WPXFileStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	numBytesRead = 0;

	if (numBytes == 0 || /* atEOS() || */ numBytes > (std::numeric_limits<unsigned long>::max)()/2
	        || !d->file.good())
		return 0;

	// can we read from the buffer?
	if (d->readBuffer && (d->readBufferPos + numBytes > d->readBufferPos)
	        && (d->readBufferPos + numBytes <= d->readBufferLength))
	{
		const unsigned char *pTmp = d->readBuffer + d->readBufferPos;
		d->readBufferPos += numBytes;
		numBytesRead = numBytes;
		return pTmp;
	}

	// hmm, we cannot: go back by the bytes we read ahead && invalidate the buffer
	if (d->readBuffer)
	{
		d->file.seekg((unsigned long)d->file.tellg() - d->readBufferLength, std::ios::beg);
		d->file.seekg(d->readBufferPos, std::ios::cur);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	unsigned long curpos = tell();
	if (curpos == (unsigned long)-1)  // tellg() returned ERROR
		return 0;

	if ( (curpos + numBytes < curpos) /*overflow*/ ||
	        (curpos + numBytes >= d->streamSize) ) /*reading more than available*/
	{
		numBytes = d->streamSize - curpos;
	}

	if (numBytes < BUFFER_MAX)
	{
		if (BUFFER_MAX < d->streamSize - curpos)
			d->readBufferLength = BUFFER_MAX;
		else /* BUFFER_MAX >= d->streamSize - curpos */
			d->readBufferLength = d->streamSize - curpos;
	}
	else
		d->readBufferLength = numBytes;

	d->file.seekg(d->readBufferLength, std::ios::cur);
	d->file.seekg(curpos, std::ios::beg);

	d->readBuffer = new unsigned char[d->readBufferLength];
	d->file.read((char *)(d->readBuffer), d->readBufferLength);

	if (!d->file.good())
		d->file.clear();
	d->readBufferPos = 0;
	if (d->readBufferLength == 0)
		return 0;

	numBytesRead = numBytes;

	d->readBufferPos += numBytesRead;
	return const_cast<const unsigned char *>( d->readBuffer );
}

long WPXFileStream::tell()
{
	return d->file.good() ? (long)((long)d->file.tellg() - d->readBufferLength + d->readBufferPos) : -1L;
}

int WPXFileStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_CUR)
		offset += tell();

	if (offset < 0)
		offset = 0;
	if (offset > (long)d->streamSize)
		offset = (long)d->streamSize;

	if (d->file.good() && offset < d->file.tellg() && (unsigned long)offset >= (unsigned long)d->file.tellg() - d->readBufferLength)
	{
		d->readBufferPos = offset + d->readBufferLength - d->file.tellg();
		return 0;
	}

	if (d->readBuffer) // seeking outside of the buffer, so invalidate the buffer
	{
		d->file.seekg((unsigned long)d->file.tellg() - d->readBufferLength, std::ios::beg);
		d->file.seekg(d->readBufferPos, std::ios::cur);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	if(d->file.good())
	{
		d->file.seekg(offset, std::ios::beg);
		return (int) ((long)d->file.tellg() == -1) ;
	}
	else
		return -1;
}

bool WPXFileStream::atEOS()
{
	return (tell() >= (long)d->streamSize);
}

bool WPXFileStream::isOLEStream()
{
	if (!d->file.good())
		return false;

	if (d->readBuffer)
	{
		d->file.seekg((unsigned long)d->file.tellg() - d->readBufferLength, std::ios::beg);
		d->file.seekg(d->readBufferPos, std::ios::cur);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	if (d->buffer.str().empty())
		d->buffer << d->file.rdbuf();
	Storage tmpStorage( d->buffer );
	seek(0, WPX_SEEK_SET);
	if (tmpStorage.isOLEStream())
		return true;
	return false;
}

WPXInputStream *WPXFileStream::getDocumentOLEStream(const char *name)
{
	if (!d->file.good())
		return (WPXInputStream *)0;

	if (d->readBuffer)
	{
		d->file.seekg((unsigned long)d->file.tellg() - d->readBufferLength, std::ios::beg);
		d->file.seekg(d->readBufferPos, std::ios::cur);
		delete [] d->readBuffer;
		d->readBuffer = 0;
		d->readBufferPos = 0;
		d->readBufferLength = 0;
	}

	if (d->buffer.str().empty())
		d->buffer << d->file.rdbuf();
	Storage *tmpStorage = new Storage( d->buffer );
	Stream tmpStream( tmpStorage, name );
	if (!tmpStorage || (tmpStorage->result() != Storage::Ok)  || !tmpStream.size())
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream *)0;
	}

	if (d->buf)
		delete [] d->buf;
	d->buf = new unsigned char[tmpStream.size()];

	unsigned long tmpLength;
	tmpLength = tmpStream.read((unsigned char *)(d->buf), tmpStream.size());

	// sanity check
	if (tmpLength > tmpStream.size() || tmpLength < tmpStream.size())
		/* something went wrong here and we do not trust the
		   resulting buffer */
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream *)0;
	}

	delete tmpStorage;
	return new WPXStringStream((d->buf), tmpLength);
}

WPXStringStream::WPXStringStream(const unsigned char *data, const unsigned int dataSize) :
	WPXInputStream(),
	d(new WPXStringStreamPrivate(std::string((const char *)data, dataSize)))
{
	d->buffer.seekg( 0, std::ios::end );
	d->streamSize = (d->buffer.good() ? (unsigned long)d->buffer.tellg() : (unsigned long)-1L);
	if (d->streamSize == (unsigned long)-1)
		d->streamSize = 0;
	// preventing possible unsigned/signed issues later by truncating the file
	if (d->streamSize > (std::numeric_limits<unsigned long>::max)() / 2)
		d->streamSize = (std::numeric_limits<unsigned long>::max)() / 2;
	d->buffer.seekg( 0, std::ios::beg );
}

WPXStringStream::~WPXStringStream()
{
	delete d;
}

const unsigned char *WPXStringStream::read(unsigned long numBytes, unsigned long &numBytesRead)
{
	numBytesRead = 0;

	if (numBytes == 0)
		return 0;

	if (atEOS() || numBytes > (std::numeric_limits<unsigned long>::max)()/2)
		return 0;

	unsigned long curpos = d->buffer.tellg();
	if (curpos == (unsigned long)-1) //tellg() returned ERROR
		return 0;

	if ( (curpos + numBytes < curpos) /*overflow*/ ||
	        (curpos + numBytes > d->streamSize) ) /*reading more than available*/
	{
		numBytes = d->streamSize - curpos;
	}

	if (d->buf)
		delete [] d->buf;
	d->buf = new unsigned char[numBytes];

	if(d->buffer.good())
	{
		d->buffer.read((char *)(d->buf), numBytes);
		numBytesRead = (long)d->buffer.tellg() - curpos;
	}

	return d->buf;
}

long WPXStringStream::tell()
{
	return d->buffer.good() ? (long)d->buffer.tellg() : -1L;
}

int WPXStringStream::seek(long offset, WPX_SEEK_TYPE seekType)
{
	if (seekType == WPX_SEEK_SET)
	{
		if (offset < 0)
			offset = 0;
		if (offset > (long)d->streamSize)
			offset = d->streamSize;
	}

	if (seekType == WPX_SEEK_CUR)
	{
		if (tell() + offset < 0)
			offset = -tell();
		if (tell() + offset > (long)d->streamSize)
			offset = d->streamSize - tell();
	}

	if(d->buffer.good())
	{
		d->buffer.seekg(offset, ((seekType == WPX_SEEK_SET) ? std::ios::beg : std::ios::cur));
		return (int) ( (long)d->buffer.tellg() == -1);
	}
	else
		return -1;
}

bool WPXStringStream::atEOS()
{
	return (d->buffer.tellg() >= (long)d->streamSize);
}

bool WPXStringStream::isOLEStream()
{
	if (!d->buffer.good())
		return false;

	Storage tmpStorage( d->buffer );
	if (tmpStorage.isOLEStream())
	{
		seek(0, WPX_SEEK_SET);
		return true;
	}
	seek(0, WPX_SEEK_SET);
	return false;
}

WPXInputStream *WPXStringStream::getDocumentOLEStream(const char *name)
{
	if (!d->buffer.good())
		return NULL;

	Storage *tmpStorage = new Storage( d->buffer );
	Stream tmpStream( tmpStorage, name );
	if (!tmpStorage || (tmpStorage->result() != Storage::Ok)  || !tmpStream.size())
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream *)0;
	}

	if (d->buf)
		delete [] d->buf;
	d->buf = new unsigned char[tmpStream.size()];
	unsigned long tmpLength;
	tmpLength = tmpStream.read((unsigned char *)(d->buf), tmpStream.size());

	// sanity check
	if (tmpLength > tmpStream.size() || tmpLength < tmpStream.size())
		/* something went wrong here and we do not trust the
		   resulting buffer */
	{
		if (tmpStorage)
			delete tmpStorage;
		return (WPXInputStream *)0;
	}

	delete tmpStorage;
	return new WPXStringStream(d->buf, tmpLength);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
