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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP42LISTENER_H
#define WP42LISTENER_H

#include "WPXListener.h"

class WP42Listener : public WPXListener
{
public:
	WP42Listener(std::vector<WPXPageSpan *> *pageList, WPXHLListenerImpl *listenerImpl);
	virtual ~WP42Listener() {};

	virtual void setAlignmentCharacter(const uint16_t character) = 0;
	virtual void setLeaderCharacter(const uint16_t character, const uint8_t numberOfSpaces) = 0;
	virtual void defineTabStops(const bool isRelative, const std::vector<WPXTabStop> &tabStops, 
				    const std::vector<bool> &usePreWP9LeaderMethods) = 0;
	virtual void insertCharacter(const uint16_t character) = 0;
	virtual void insertTab(const uint8_t tabType, const float tabPosition) = 0;
	virtual void handleLineBreak() = 0;
	virtual void insertEOL() = 0;
	virtual void attributeChange(const bool isOn, const uint8_t attribute) = 0;
	virtual void lineSpacingChange(const float lineSpacing) = 0;
	virtual void spacingAfterParagraphChange(const float spacingRelative, const float spacingAbsolute) = 0;
	virtual void justificationChange(const uint8_t justification) = 0;
	virtual void pageMarginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void pageFormChange(const uint16_t length, const uint16_t width, const WPXFormOrientation orientation, const bool isPersistent) = 0;
	virtual void marginChange(const uint8_t side, const uint16_t margin) = 0;
	virtual void paragraphMarginChange(const uint8_t side, const int16_t margin) = 0;
	virtual void indentFirstLineChange(const int16_t offset) = 0;
	virtual void columnChange(const WPXTextColumnType typeColumn, const uint8_t numColumns, const std::vector<float> &columnWidth,
				  const std::vector<bool> &isFixedWidth) = 0;
	virtual void endDocument() = 0;

	virtual void defineTable(const uint8_t position, const uint16_t leftOffset) = 0;
	virtual void addTableColumnDefinition(const uint32_t width, const uint32_t leftGutter, const uint32_t rightGutter,
				const uint32_t attributes, const uint8_t alignment) = 0;
	virtual void startTable() = 0;
 	virtual void insertRow(const uint16_t rowHeight, const bool isMinimumHeight, const bool isHeaderRow) = 0;
 	virtual void insertCell(const uint8_t colSpan, const uint8_t rowSpan, const bool boundFromLeft, const bool boundFromAbove,
				const uint8_t borderBits, const RGBSColor * cellFgColor, const RGBSColor * cellBgColor, 
				const RGBSColor * cellBorderColor, const WPXVerticalAlignment cellVerticalAlignment, 
				const bool useCellAttributes, const uint32_t cellAttributes) = 0;
 	virtual void endTable() = 0;

};

#endif /* WP42LISTENER_H */