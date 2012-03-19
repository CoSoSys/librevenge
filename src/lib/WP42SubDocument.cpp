/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WP42SubDocument.h"
#include "WP42Parser.h"
#include "libwpd_internal.h"

WP42SubDocument::WP42SubDocument(WPXInputStream *input, WPXEncryption *encryption, const unsigned dataSize) :
	WPXSubDocument(input, encryption, dataSize)
{
}

void WP42SubDocument::parse(WP42Listener *listener) const
{
	WPXMemoryInputStream *tmpStream = getStream();
	if (!tmpStream)
		return;
	tmpStream->seek(0, WPX_SEEK_SET);
	listener->marginReset(readU8(tmpStream, 0), readU8(tmpStream, 0));
	WP42Parser::parseDocument(tmpStream, 0, listener);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
