/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2003 Marc Maurer (j.m.maurer@student.utwente.nl)
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

#ifndef _WP5HEADER_H
#define _WP5HEADER_H

#include "WPXHeader.h"

class WP5Header : public WPXHeader
{
 public:
	WP5Header(GsfInput * input, guint32 documentOffset, guint8 productType, guint8 fileType, guint8 majorVersion, guint8 minorVersion, guint16 documentEncryption);
	virtual ~WP5Header() {};
	
	const guint16 getIndexHeaderOffset() const { return m_indexHeaderOffset; }
	const guint16 getNumPrefixIndices() const { return m_numPrefixIndices; }
	const guint16 getDocumentEncryption() const { return m_documentEncryption; }

protected:
	void _readIndexInformation(GsfInput *input);
		
private:
	guint16 m_indexHeaderOffset;
	guint16 m_numPrefixIndices;
	guint16 m_documentEncryption;
};
#endif /* _WP5HEADER_H  */