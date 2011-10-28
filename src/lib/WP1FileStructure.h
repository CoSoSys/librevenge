/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* libwpd
 * Copyright (C) 2003 William Lachance (wrlach@gmail.com)
 * Copyright (C) 2003-2004 Marc Maurer (uwog@uwog.net)
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

#ifndef WP1FILESTRUCTURE_H
#define WP1FILESTRUCTURE_H

// size of the functiongroups 0xC0 to 0xF8
extern int WP1_FUNCTION_GROUP_SIZE[63];

#define WP1_ATTRIBUTE_BOLD 0
#define WP1_ATTRIBUTE_ITALICS 1
#define WP1_ATTRIBUTE_UNDERLINE 2
#define WP1_ATTRIBUTE_OUTLINE 3
#define WP1_ATTRIBUTE_SHADOW 4
#define WP1_ATTRIBUTE_SUPERSCRIPT 5
#define WP1_ATTRIBUTE_SUBSCRIPT 6
#define WP1_ATTRIBUTE_REDLINE 7
#define WP1_ATTRIBUTE_STRIKE_OUT 8

#define WP1_MARGIN_RESET_GROUP 0xC0
#define WP1_SPACING_RESET_GROUP 0xC1
#define WP1_MARGIN_RELEASE_GROUP 0xC2
#define WP1_CENTER_TEXT_GROUP 0xC3
#define WP1_FLUSH_RIGHT_GROUP 0xC4

#define WP1_SET_TABS_GROUP 0xC9

#define WP1_FONT_ID_GROUP 0xCB
#define WP1_LEFT_INDENT_GROUP 0xCC

#define WP1_TOP_MARGIN_SET_GROUP 0xCE
#define WP1_SUPPRESS_PAGE_CHARACTERISTICS_GROUP 0xCF
#define WP1_BOTTOM_MARGIN_SET_GROUP 0xD0
#define WP1_HEADER_FOOTER_GROUP 0xD1

#define WP1_LEFT_RIGHT_INDENT_GROUP 0xE0
#define WP1_EXTENDED_CHARACTER_GROUP 0xE1
#define WP1_FOOTNOTE_ENDNOTE_GROUP 0xE2
#define WP1_PICTURE_GROUP 0xF5

#define WP1_POINT_SIZE_GROUP 0xF4

#define WP1_JUSTIFICATION_GROUP 0xFB

#define WP1_HEADER_FOOTER_GROUP_ALL_BIT 1
#define WP1_HEADER_FOOTER_GROUP_ODD_BIT 2
#define WP1_HEADER_FOOTER_GROUP_EVEN_BIT 4

#endif /* WP1FILESTRUCTURE_H */
/* vim:set shiftwidth=4 softtabstop=4 noexpandtab: */
