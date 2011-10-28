/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Copyright (C) 2006 Fridrich Strba (fridrich.strba@bluewin.ch)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#include "WP1PointSizeGroup.h"
#include "libwpd_internal.h"

WP1PointSizeGroup::WP1PointSizeGroup(WPXInputStream *input, WPXEncryption *encryption, uint8_t group) :
	WP1FixedLengthGroup(group),
	m_pointSize(0)
{
	_read(input, encryption);
}

WP1PointSizeGroup::~WP1PointSizeGroup()
{
}

void WP1PointSizeGroup::_readContents(WPXInputStream *input, WPXEncryption *encryption)
{
	input->seek(1, WPX_SEEK_CUR);
	m_pointSize = readU8(input, encryption);
}

void WP1PointSizeGroup::parse(WP1Listener *listener)
{
	WPD_DEBUG_MSG(("WordPerfect: handling the Extended Character group\n"));
	if (m_pointSize)
		listener->fontPointSize(m_pointSize);
}
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
