/* libwpd2
 * Copyright (C) 2002 William Lachance (wlach@interlog.com)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WPXPARSER_H
#define WPXPARSER_H

#include <stdio.h>
#include <glib.h>
#include "WPXLLListener.h"
#include "WPXHLListener.h"

class WPXHeader;

class WPXParser
{
public:
	WPXParser(FILE * stream, WPXHeader * header);
		
	static WPXParser * constructParser(FILE * stream);
	virtual gboolean parse() = 0;

	void registerListener(WPXLLListener * listener) { m_pLLListener = listener; }
	void registerListener(WPXHLListener * listener) { m_pHLListener = listener; }

	WPXLLListener * getLLListener() { return m_pLLListener; }
	WPXHLListener * getHLListener() { return m_pHLListener; }
		
	WPXHeader * getHeader() { return m_pHeader; }
	FILE * getStream() { return m_pStream; }
private:
	FILE * m_pStream;
	WPXHeader * m_pHeader;
	
	WPXLLListener * m_pLLListener;
	WPXHLListener * m_pHLListener;
};

#endif /* WPXPARSER_H */