/* libwpd
 * Copyright (C) 2003 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2004 Marc Maurer (j.m.maurer@student.utwente.nl)
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
 
#include "WP5StylesListener.h"
#include "WPXTable.h"
#include "WP5FileStructure.h"
#include "WPXFileStructure.h"
#include "libwpd_internal.h"
#include "WP5SubDocument.h"

WP5StylesListener::WP5StylesListener(std::vector<WPXPageSpan *> *pageList, WPXTableList tableList) : 
	WP5Listener(pageList, NULL),
	m_currentPage(new WPXPageSpan()),
	m_tableList(tableList), 
	m_tempMarginLeft(1.0f),
	m_tempMarginRight(1.0f),
	m_currentPageHasContent(false)
{
}

void WP5StylesListener::endDocument()
{	
	insertBreak(WPX_SOFT_PAGE_BREAK); // pretend we just had a soft page break (for the last page)
	delete(m_currentPage); // and delete the non-existent page that was allocated as a result (scandalous waste!)
}

void WP5StylesListener::insertBreak(const uint8_t breakType)
{
	//if (!isUndoOn())
	//{	
		switch (breakType) 
		{
		case WPX_PAGE_BREAK:
		case WPX_SOFT_PAGE_BREAK:
			if (WPXListener::m_pageList->size() > 0 && (*m_currentPage)==(*(m_pageList->back())))
			{
				int oldPageSpan = m_pageList->back()->getPageSpan();
				m_pageList->back()->setPageSpan(oldPageSpan + 1);
				delete(m_currentPage);
			}
			else
			{
				m_pageList->push_back(m_currentPage);
			}
			m_currentPage = new WPXPageSpan(*(m_pageList->back()));
			m_currentPage->setMarginLeft(m_tempMarginLeft);
			m_currentPage->setMarginRight(m_tempMarginRight);
			m_currentPageHasContent = false;
			break;
		}
	//}
}

void WP5StylesListener::pageMarginChange(const uint8_t side, const uint16_t margin)
{
	//if (!isUndoOn()) 
	//{
		float marginInch = (float)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WPX_TOP:
				m_currentPage->setMarginTop(marginInch);
				break;
			case WPX_BOTTOM:
				m_currentPage->setMarginBottom(marginInch);
				break;
		}
	//}
}

void WP5StylesListener::pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent)
{
	//if (!isUndoOn())
	//{
		float lengthInch = (float)((double)length / (double)WPX_NUM_WPUS_PER_INCH);
		float widthInch = (float)((double)width / (double)WPX_NUM_WPUS_PER_INCH);
		if (!m_currentPageHasContent)
		{
			m_currentPage->setFormLength(lengthInch);
			m_currentPage->setFormWidth(widthInch);
			m_currentPage->setFormOrientation(orientation);
		}
	//}
}


void WP5StylesListener::marginChange(const uint8_t side, const uint16_t margin)
{
	//if (!isUndoOn()) 
	//{		
		float marginInch = (float)((double)margin / (double)WPX_NUM_WPUS_PER_INCH);
		switch(side)
		{
			case WPX_LEFT:
				if (!m_currentPageHasContent)
					m_currentPage->setMarginLeft(marginInch);
				m_tempMarginLeft = marginInch;
				break;
			case WPX_RIGHT:
				if (!m_currentPageHasContent)
					m_currentPage->setMarginRight(marginInch);
				m_tempMarginRight = marginInch;
				break;
		}
		
	//}

}

void WP5StylesListener::headerFooterGroup(const uint8_t headerFooterType, const uint8_t occurenceBits, const WP5SubDocument *subDocument)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: headerFooterGroup (headerFooterType: %i, occurenceBits: %i)\n", 
			       headerFooterType, occurenceBits));
		bool tempCurrentPageHasContent = m_currentPageHasContent;
		if (headerFooterType <= WP5_HEADER_FOOTER_GROUP_FOOTER_B)
		{
			WPXTableList tableList; 
#if 0
			m_currentPage->setHeaderFooter(headerFooterType, occurenceBits, subDocument, tableList);
#endif
			_handleSubDocument(subDocument, true, tableList);
		}
		m_currentPageHasContent = tempCurrentPageHasContent;
	}
}

/*
void WP5StylesListener::suppressPageCharacteristics(const uint8_t suppressCode)
{
	if (!isUndoOn()) 
	{			
		WPD_DEBUG_MSG(("WordPerfect: suppressPageCharacteristics (suppressCode: %u)\n", suppressCode));
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_A)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_HEADER_B)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_HEADER_B, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_A)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_A, true);
		if (suppressCode & WP6_PAGE_GROUP_SUPPRESS_FOOTER_B)
			m_currentPage->setHeadFooterSuppression(WP6_HEADER_FOOTER_GROUP_FOOTER_B, true);			
	}
}
*/
void WP5StylesListener::startTable()
{
	//if (!isUndoOn()) 
	//{			
		m_currentPageHasContent = true;
		m_currentTable = new WPXTable();
		m_tableList.add(m_currentTable);
	//}
}

void WP5StylesListener::insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow)
{
	if (/*!isUndoOn() && */m_currentTable != NULL) 
	{
		m_currentPageHasContent = true;
		m_currentTable->insertRow();
	}
}

void WP5StylesListener::insertCell(const uint8_t colSpan, const uint8_t rowSpan, const uint8_t borderBits, 
				  const RGBSColor * cellFgColor, const RGBSColor * cellBgColor,
				  const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
				  const bool useCellAttributes, const uint32_t cellAttributes)
{
	if (/*!isUndoOn() &&*/ m_currentTable != NULL)
	{
		m_currentPageHasContent = true;
		m_currentTable->insertCell(colSpan, rowSpan, borderBits);
	}
}

void WP5StylesListener::_handleSubDocument(const WPXSubDocument *subDocument, const bool isHeaderFooter, WPXTableList tableList, int nextTableIndice)
{
	// We don't want to actual insert anything in the case of a sub-document, but we
	// do want to capture whatever table-related information is within it..
	if (!isUndoOn()) 
	{
		std::set <const WPXSubDocument *> oldSubDocuments;
		oldSubDocuments = m_subDocuments;
		// prevent entering in an endless loop		
		if ((subDocument) && (oldSubDocuments.find(subDocument) == oldSubDocuments.end()))
		{
			m_subDocuments.insert(subDocument);
			if (isHeaderFooter) 
			{
				WPXTable * oldCurrentTable = m_currentTable;
				WPXTableList oldTableList = m_tableList;
				m_tableList = tableList;

				subDocument->parse(this);

				m_tableList = oldTableList;
				m_currentTable = oldCurrentTable;
			}
			else
			{
				subDocument->parse(this);
			}
			m_subDocuments = oldSubDocuments;

		}
	}
}
