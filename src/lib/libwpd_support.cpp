/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 * Copyright (C) 2004 Fridrich Strba (fridrich.strba@bluewin.ch)
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

#include "libwpd.h"
#include "libwpd_internal.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

const uint16_t asciiMap[] =
{
	  0,   1,   2,   3,   4,   5,   6,   7,
	  8,   9,  10,  11,  12,  13,  14,  15,
	 16,  17,  18,  19,  20,  21,  22,  23,
	 24,  25,  26,  27,  28,  29,  30,  31,
	 32,  33,  34,  35,  36,  37,  38,  39,
	 40,  41,  42,  43,  44,  45,  46,  47,
	 48,  49,  50,  51,  52,  53,  54,  55,
	 56,  57,  58,  59,  60,  61,  62,  63,
	 64,  65,  66,  67,  68,  69,  70,  71,
	 72,  73,  74,  75,  76,  77,  78,  79,
	 80,  81,  82,  83,  84,  85,  86,  87,
	 88,  89,  90,  91,  92,  93,  94,  95,
	 96,  97,  98,  99, 100, 101, 102, 103,
	104, 105, 106, 107, 108, 109, 110, 111,
	112, 113, 114, 115, 116, 117, 118, 119,
	120, 121, 122, 123, 124, 125, 126, 127,
	128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 138, 139, 140, 141, 142, 143,
	144, 145, 146, 147, 148, 149, 150, 151,
	152, 153, 154, 155, 156, 157, 158, 159,
	160, 161, 162, 163, 164, 165, 166, 167,
	168, 169, 170, 171, 172, 173, 174, 175,
	176, 177, 178, 179, 180, 181, 182, 183,
	184, 185, 186, 187, 188, 189, 190, 191,
	192, 193, 194, 195, 196, 197, 198, 199,
	200, 201, 202, 203, 204, 205, 206, 207,
	208, 209, 210, 211, 212, 213, 214, 215,
	216, 217, 218, 219, 220, 221, 222, 223,
	224, 225, 226, 227, 228, 229, 230, 231,
	232, 233, 234, 235, 236, 237, 238, 239,
	240, 241, 242, 243, 244, 245, 246, 247,
	248, 249, 250, 251, 252, 253, 254, 255,
};

const uint16_t extendedInternationalCharacterMap[] =
{
  229, // lower case 'a' with a small circle
  197, // upper case 'a' with a small circle
  230, // lower case 'ae'
  198, // upper case 'ae'
  228, // lower case 'a' with diathesis
  196, // upper case 'a' with diathesis
  225, // lower case 'a' with acute
  224, // lower case 'a' with grave
  226, // lower case 'a' with circonflex
  227, // lower case 'a' with tilde
  195, // upper case 'a' with tilde
  231, // lower case 'c' with hook
  199, // upper case 'c' with hook
  235, // lower case 'e' with diathesis
  233, // lower case 'e' with acute
  201, // upper case 'e' with acute
  232, // lower case 'e' with grave
  234, // lower case 'e' with circonflex
  237, // lower case 'i' with acute
  241, // lower case 'n' with tilde
  209, // upper case 'n' with tilde
  248, // lower case 'o' with stroke
  216, // upper case 'o' with stroke
  245, // lower case 'o' with tilde
  213, // upper case 'o' with tilde
  246, // lower case 'o' with diathesis
  214, // upper case 'o' with diathesis
  252, // lower case 'u' with diathesis
  220, // upper case 'u' with diathesis
  250, // lower case 'u' with acute
  249, // lower case 'u' with grave
  223 // double s
};

/* Mapping of Apple's MacRoman character set in Unicode (UCS2) 
 * used in the WordPerfect Macintosh file format */
 
const uint16_t macintoshCharacterMap[] =
{
  0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
  0x0028, 0x0029, 0x002a, 0x002b, 0x002c, 0x002d, 0x002e, 0x002f,
  0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
  0x0038, 0x0039, 0x003a, 0x003b, 0x003c, 0x003d, 0x003e, 0x003f,
  0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
  0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
  0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
  0x0058, 0x0059, 0x005a, 0x005b, 0x005c, 0x005d, 0x005e, 0x005f,
  0x0060, 0x0061, 0x0062, 0x0063, 0x0064, 0x0065, 0x0066, 0x0067,
  0x0068, 0x0069, 0x006a, 0x006b, 0x006c, 0x006d, 0x006e, 0x006f,
  0x0070, 0x0071, 0x0072, 0x0073, 0x0074, 0x0075, 0x0076, 0x0077,
  0x0078, 0x0079, 0x007a, 0x007b, 0x007c, 0x007d, 0x007e, 0x007f,
  0x00c4, 0x00c5, 0x00c7, 0x00c9, 0x00d1, 0x00d6, 0x00dc, 0x00e1,
  0x00e0, 0x00e2, 0x00e4, 0x00e3, 0x00e5, 0x00e7, 0x00e9, 0x00e8,
  0x00ea, 0x00eb, 0x00ed, 0x00ec, 0x00ee, 0x00ef, 0x00f1, 0x00f3,
  0x00f2, 0x00f4, 0x00f6, 0x00f5, 0x00fa, 0x00f9, 0x00fb, 0x00fc,
  0x2020, 0x00b0, 0x00a2, 0x00a3, 0x00a7, 0x2022, 0x00b6, 0x00df,
  0x00ae, 0x00a9, 0x2122, 0x00b4, 0x00a8, 0x2260, 0x00c6, 0x00d8,
  0x221e, 0x00b1, 0x2264, 0x2265, 0x00a5, 0x00b5, 0x2202, 0x2211,
  0x220f, 0x03c0, 0x222b, 0x00aa, 0x00ba, 0x03a9, 0x00e6, 0x00f8,
  0x00bf, 0x00a1, 0x00ac, 0x221a, 0x0192, 0x2248, 0x2206, 0x00ab,
  0x00bb, 0x2026, 0x00a0, 0x00c0, 0x00c3, 0x00d5, 0x0152, 0x0153,
  0x2013, 0x2014, 0x201c, 0x201d, 0x2018, 0x2019, 0x00f7, 0x25ca,
  0x00ff, 0x0178, 0x2044, 0x20ac, 0x2039, 0x203a, 0xfb01, 0xfb02,
  0x2021, 0x00b7, 0x201a, 0x201e, 0x2030, 0x00c2, 0x00ca, 0x00c1,
  0x00cb, 0x00c8, 0x00cd, 0x00ce, 0x00cf, 0x00cc, 0x00d3, 0x00d4,
  0xf8ff, 0x00d2, 0x00da, 0x00db, 0x00d9, 0x0131, 0x02c6, 0x02dc,
  0x00af, 0x02d8, 0x02d9, 0x02da, 0x00b8, 0x02dd, 0x02db, 0x02c7
};

/* WP Extended Character -> Unicode (UCS2) Mappings by Ariya Hidayat <ariyahidayat@yahoo.de> for the
   KWord project, licensed under the LGPL */

/* WP multinational characters (charset 1) */
const uint16_t multinationalMap[] = {
  0x0300, 0x00b7, 0x0303, 0x0302, 0x0335, 0x0338, 0x0301, 0x0308,
  0x0304, 0x0313, 0x0315, 0x02bc, 0x0326, 0x0315, 0x030a, 0x0307,
  0x030b, 0x0327, 0x0328, 0x030c, 0x0337, 0x0305, 0x0306, 0x00df,
  0x0138, 0x006a, 0x00c1, 0x00e1, 0x00c2, 0x00e2, 0x00c4, 0x00e4,
  0x00c0, 0x00e0, 0x00c5, 0x00e5, 0x00c6, 0x00e6, 0x00c7, 0x00e7,
  0x00c9, 0x00e9, 0x00ca, 0x00ea, 0x00cb, 0x00eb, 0x00c8, 0x00e8,
  0x00cd, 0x00ed, 0x00ce, 0x00ee, 0x00cf, 0x00ef, 0x00cc, 0x00ec,
  0x00d1, 0x00f1, 0x00d3, 0x00f3, 0x00d4, 0x00f4, 0x00d6, 0x00f6,
  0x00d2, 0x00f2, 0x00da, 0x00fa, 0x00db, 0x00fb, 0x00dc, 0x00fc,
  0x00d9, 0x00f9, 0x0178, 0x00ff, 0x00c3, 0x00e3, 0x0110, 0x0111,
  0x00d8, 0x00f8, 0x00d5, 0x00f5, 0x00dd, 0x00fd, 0x00d0, 0x00f0,
  0x00de, 0x00fe, 0x0102, 0x0103, 0x0100, 0x0101, 0x0104, 0x0105,
  0x0106, 0x0107, 0x010c, 0x010d, 0x0108, 0x0109, 0x010a, 0x010b,
  0x010e, 0x010f, 0x011a, 0x011b, 0x0116, 0x0117, 0x0112, 0x0113,
  0x0118, 0x0119, 0x01f4, 0x01f5, 0x011e, 0x011f, 0x01e6, 0x01e7,
  0x0122, 0x0123, 0x011c, 0x011d, 0x0120, 0x0121, 0x0124, 0x0125,
  0x0126, 0x0127, 0x0130, 0x0069, 0x012a, 0x012b, 0x012e, 0x012f,
  0x0128, 0x0129, 0x0132, 0x0133, 0x0134, 0x0135, 0x0136, 0x0137,
  0x0139, 0x013a, 0x013d, 0x013e, 0x013b, 0x013c, 0x013f, 0x0140,
  0x0141, 0x0142, 0x0143, 0x0144, 0x004e, 0x0149, 0x0147, 0x0148,
  0x0145, 0x0146, 0x0150, 0x0151, 0x014c, 0x014d, 0x0152, 0x0153,
  0x0154, 0x0155, 0x0158, 0x0159, 0x0156, 0x0157, 0x015a, 0x015b,
  0x0160, 0x0161, 0x015e, 0x015f, 0x015c, 0x015d, 0x0164, 0x0165,
  0x0162, 0x0163, 0x0166, 0x0167, 0x016c, 0x016d, 0x0170, 0x0171,
  0x016a, 0x016b, 0x0172, 0x0173, 0x016e, 0x016f, 0x0168, 0x0169,
  0x0174, 0x0175, 0x0176, 0x0177, 0x0179, 0x017a, 0x017d, 0x017e,
  0x017b, 0x017c, 0x014a, 0x014b, 0x0044, 0x0064, 0x004c, 0x006c,
  0x004e, 0x006e, 0x0052, 0x0072, 0x0053, 0x0073, 0x0054, 0x0074,
  0x0059, 0x0079, 0x1ef2, 0x1ef3, 0x010e, 0x010f, 0x01a0, 0x01a1,
  0x01af, 0x01b0, 0x0114, 0x0115, 0x012c, 0x012d, 0x0049, 0x0131,
  0x014e, 0x014f
};

/* WP phonetic symbol (charset 2) */
const uint16_t phoneticMap[] = {
  0x02b9, 0x02ba, 0x02bb, 0xf813, 0x02bd, 0x02bc, 0xf814, 0x02be,
  0x02bf, 0x0310, 0x02d0, 0x02d1, 0x0306, 0x032e, 0x0329, 0x02c8,
  0x02cc, 0x02c9, 0x02ca, 0x02cb, 0x02cd, 0x02ce, 0x02cf, 0x02c6,
  0x02c7, 0x02dc, 0x0325, 0x02da, 0x032d, 0x032c, 0x0323, 0x0308,
  0x0324, 0x031c, 0x031d, 0x031e, 0x031f, 0x0320, 0x0321, 0x0322,
  0x032a, 0x032b, 0x02d2, 0x02d3, 0xf815, 0xf816, 0x005f, 0x2017,
  0x033e, 0x02db, 0x0327, 0x0233, 0x030d, 0x02b0, 0x02b6, 0x0250,
  0x0251, 0x0252, 0x0253, 0x0299, 0x0254, 0x0255, 0x0297, 0x0256,
  0x0257, 0x0258, 0x0259, 0x025a, 0x025b, 0x025c, 0x025d, 0x029a,
  0x025e, 0x025f, 0x0278, 0x0261, 0x0260, 0x0262, 0x029b, 0x0263,
  0x0264, 0x0265, 0x0266, 0x0267, 0x029c, 0x0268, 0x026a, 0x0269,
  0x029d, 0x029e, 0x026b, 0x026c, 0x026d, 0x029f, 0x026e, 0x028e,
  0x026f, 0x0270, 0x0271, 0x0272, 0x0273, 0x0274, 0x0276, 0x0277,
  0x02a0, 0x0279, 0x027a, 0x027b, 0x027c, 0x027d, 0x027e, 0x027f,
  0x0280, 0x0281, 0x0282, 0x0283, 0x0284, 0x0285, 0x0286, 0x0287,
  0x0288, 0x0275, 0x0289, 0x028a, 0x028c, 0x028b, 0x028d, 0x03c7,
  0x028f, 0x0290, 0x0291, 0x0292, 0x0293, 0x0294, 0x0295, 0x0296,
  0x02a1, 0x02a2, 0x0298, 0x02a3, 0x02a4, 0x02a5, 0x02a6, 0x02a7,
  0x02a8
};

/* WP box drawing symbol (charset 3) */
const uint16_t boxdrawingMap[] = {
  0x2591, 0x2592, 0x2593, 0x2588, 0x258c, 0x2580, 0x2590, 0x2584,
  0x2500, 0x2502, 0x250c, 0x2510, 0x2518, 0x2514, 0x251c, 0x252c,
  0x2524, 0x2534, 0x253c, 0x2550, 0x2551, 0x2554, 0x2557, 0x255d,
  0x255a, 0x2560, 0x2566, 0x2563, 0x2569, 0x256c, 0x2552, 0x2555,
  0x255b, 0x2558, 0x2553, 0x2556, 0x255c, 0x2559, 0x255e, 0x2565,
  0x2561, 0x2568, 0x255f, 0x2564, 0x2562, 0x2567, 0x256b, 0x256a,
  0x2574, 0x2575, 0x2576, 0x2577, 0x2578, 0x2579, 0x257a, 0x257b,
  0x257c, 0x257e, 0x257d, 0x257f, 0x251f, 0x2522, 0x251e, 0x2521,
  0x252e, 0x2532, 0x252d, 0x2531, 0x2527, 0x2526, 0x252a, 0x2529,
  0x2536, 0x253a, 0x2535, 0x2539, 0x2541, 0x2546, 0x253e, 0x2540,
  0x2544, 0x254a, 0x253d, 0x2545, 0x2548, 0x2543, 0x2549, 0x2547
};

/* WP typographic symbol (charset 4) */
const uint16_t typographicMap[] = {
  0x25cf, 0x25cb, 0x25a0, 0x2022, 0xf817, 0x00b6, 0x00a7, 0x00a1,
  0x00bf, 0x00ab, 0x00bb, 0x00a3, 0x00a5, 0x20a7, 0x0192, 0x00aa,
  0x00ba, 0x00bd, 0x00bc, 0x00a2, 0x00b2, 0x207f, 0x00ae, 0x00a9,
  0x00a4, 0x00be, 0x00b3, 0x201b, 0x2019, 0x2018, 0x201f, 0x201d,
  0x201c, 0x2013, 0x2014, 0x2039, 0x203a, 0x25cb, 0x25a1, 0x2020,
  0x2021, 0x2122, 0x2120, 0x211e, 0x25cf, 0x25e6, 0x25a0, 0x25aa,
  0x25a1, 0x25ab, 0x2012, 0xfb00, 0xfb03, 0xfb04, 0xfb01, 0xfb02,
  0x2026, 0x0024, 0x20a3, 0x20a2, 0x20a0, 0x20a4, 0x201a, 0x201e,
  0x2153, 0x2154, 0x215b, 0x215c, 0x215d, 0x215e, 0x24c2, 0x24c5,
  0x20ac, 0x2105, 0x2106, 0x2030, 0x2116, 0xf818, 0x00b9, 0x2409,
  0x240c, 0x240d, 0x240a, 0x2424, 0x240b, 0xf819, 0x20a9, 0x20a6,
  0x20a8, 0xf81a, 0xf81b, 0xf81c, 0xf81d, 0xf81e, 0xf81f, 0xf820,
  0xf821, 0xf822, 0xf823, 0xf824, 0xf825, 0xf826
};

/* WP iconic symbol (charset 5) */
const uint16_t iconicMap[] = {
  0x2661, 0x2662, 0x2667, 0x2664, 0x2642, 0x2640, 0x263c, 0x263a,
  0x263b, 0x266a, 0x266c, 0x25ac, 0x2302, 0x203c, 0x221a, 0x21a8,
  0x2310, 0x2319, 0x25d8, 0x25d9, 0x21b5, 0x2104, 0x261c, 0x2007,
  0x2610, 0x2612, 0x2639, 0x266f, 0x266d, 0x266e, 0x260e, 0x231a,
  0x231b, 0x2701, 0x2702, 0x2703, 0x2704, 0x260e, 0x2706, 0x2707,
  0x2708, 0x2709, 0x261b, 0x261e, 0x270c, 0x270d, 0x270e, 0x270f,
  0x2710, 0x2711, 0x2712, 0x2713, 0x2714, 0x2715, 0x2716, 0x2717,
  0x2718, 0x2719, 0x271a, 0x271b, 0x271c, 0x271d, 0x271e, 0x271f,
  0x2720, 0x2721, 0x2722, 0x2723, 0x2724, 0x2725, 0x2726, 0x2727,
  0x2605, 0x2606, 0x272a, 0x272b, 0x272c, 0x272d, 0x272e, 0x272f,
  0x2730, 0x2731, 0x2732, 0x2733, 0x2734, 0x2735, 0x2736, 0x2737,
  0x2738, 0x2739, 0x273a, 0x273b, 0x273c, 0x273d, 0x273e, 0x273f,
  0x2740, 0x2741, 0x2742, 0x2743, 0x2744, 0x2745, 0x2746, 0x2747,
  0x2748, 0x2749, 0x274a, 0x274b, 0x25cf, 0x274d, 0x25a0, 0x274f,
  0x2750, 0x2751, 0x2752, 0x25b2, 0x25bc, 0x25c6, 0x2756, 0x25d7,
  0x2758, 0x2759, 0x275a, 0x275b, 0x275c, 0x275d, 0x275e, 0x2036,
  0x2033, 0xf827, 0xf828, 0xf829, 0xf82a, 0x2329, 0x232a, 0x005b,
  0x005d, 0xf82b, 0xf82c, 0xf82d, 0xf82e, 0xf82f, 0xf830, 0xf831,
  0x2190, 0xf832, 0xf833, 0xf834, 0xf835, 0xf836, 0x21e8, 0x21e6,
  0x2794, 0xf838, 0xf839, 0xf83a, 0xf83b, 0xf83c, 0x25d6, 0xf83d,
  0xf83e, 0x2761, 0x2762, 0x2763, 0x2764, 0x2765, 0x2766, 0x2767,
  0x2663, 0x2666, 0x2665, 0x2660, 0x2780, 0x2781, 0x2782, 0x2783,
  0x2784, 0x2785, 0x2786, 0x2787, 0x2788, 0x2789, 0x2776, 0x2777,
  0x2778, 0x2779, 0x277a, 0x277b, 0x277c, 0x277d, 0x277e, 0x277f,
  0x2780, 0x2781, 0x2782, 0x2783, 0x2784, 0x2785, 0x2786, 0x2787,
  0x2788, 0x2789, 0x278a, 0x278b, 0x278c, 0x278d, 0x278e, 0x278f,
  0x2790, 0x2791, 0x2792, 0x2793, 0x2794, 0x2192, 0x2194, 0x2195,
  0x2798, 0x2799, 0x279a, 0x279b, 0x279c, 0x279d, 0x279e, 0x279f,
  0x27a0, 0x27a1, 0x27a2, 0x27a3, 0x27a4, 0x27a5, 0x27a6, 0x27a7,
  0x27a8, 0x27a9, 0x27aa, 0x27ab, 0x27ac, 0x27ad, 0x27ae, 0x27af,
  0xf83f, 0x27b1, 0x27b2, 0x27b3, 0x27b4, 0x27b5, 0x27b6, 0x27b7,
  0x27b8, 0x27b9, 0x27ba, 0x27bb, 0x27bc, 0x27bd, 0x27be
};

/* WP math/scientific (charset 6) */
const uint16_t mathMap[] = {
  0x2212, 0x00b1, 0x2264, 0x2265, 0x221d, 0x002f, 0x2215, 0x2216,
  0x00f7, 0x2223, 0x2329, 0x232a, 0x223c, 0x2248, 0x2261, 0x2208,
  0x2229, 0x2225, 0x2211, 0x221e, 0x00ac, 0x2192, 0x2190, 0x2191,
  0x2193, 0x2194, 0x2195, 0x25b8, 0x25c2, 0x25b4, 0x25be, 0x22c5,
  0xf850, 0x2218, 0x2219, 0x212b, 0x00b0, 0x00b5, 0x203e, 0x00d7,
  0x222b, 0x220f, 0x2213, 0x2207, 0x2202, 0x2032, 0x2033, 0x2192,
  0x212f, 0x2113, 0x210f, 0x2111, 0x211c, 0x2118, 0x21c4, 0x21c6,
  0x21d2, 0x21d0, 0x21d1, 0x21d3, 0x21d4, 0x21d5, 0x2197, 0x2198,
  0x2196, 0x2199, 0x222a, 0x2282, 0x2283, 0x2286, 0x2287, 0x220d,
  0x2205, 0x2308, 0x2309, 0x230a, 0x230b, 0x226a, 0x226b, 0x2220,
  0x2297, 0x2295, 0x2296, 0xf851, 0x2299, 0x2227, 0x2228, 0x22bb,
  0x22a4, 0x22a5, 0x2312, 0x22a2, 0x22a3, 0x25a1, 0x25a0, 0x25ca,
  0x25c6, 0x27e6, 0x27e7, 0x2260, 0x2262, 0x2235, 0x2234, 0x2237,
  0x222e, 0x2112, 0x212d, 0x2128, 0x2118, 0x20dd, 0xf855, 0x25c7,
  0x22c6, 0x2034, 0x2210, 0x2243, 0x2245, 0x227a, 0x227c, 0x227b,
  0x227d, 0x2203, 0x2200, 0x22d8, 0x22d9, 0x228e, 0x228a, 0x228b,
  0x2293, 0x2294, 0x228f, 0x2291, 0x22e4, 0x2290, 0x2292, 0x22e5,
  0x25b3, 0x25bd, 0x25c3, 0x25b9, 0x22c8, 0x2323, 0x2322, 0x25ef,
  0x219d, 0x21a9, 0x21aa, 0x21a3, 0x21bc, 0x21bd, 0x21c0, 0x21c1,
  0x21cc, 0x21cb, 0x21bf, 0x21be, 0x21c3, 0x21c2, 0x21c9, 0x21c7,
  0x22d3, 0x22d2, 0x22d0, 0x22d1, 0x229a, 0x229b, 0x229d, 0x2127,
  0x2221, 0x2222, 0x25c3, 0x25b9, 0x25b5, 0x25bf, 0x2214, 0x2250,
  0x2252, 0x2253, 0x224e, 0x224d, 0x22a8, 0xf857, 0x226c, 0x0285,
  0x2605, 0x226e, 0x2270, 0x226f, 0x2271, 0x2241, 0x2244, 0x2247,
  0x2249, 0x2280, 0x22e0, 0x2281, 0x22e1, 0x2284, 0x2285, 0x2288,
  0x2289, 0xf858, 0xf859, 0x22e2, 0x22e3, 0x2226, 0x2224, 0x226d,
  0x2204, 0x2209, 0xf85a, 0x2130, 0x2131, 0x2102, 0xf85b, 0x2115,
  0x211d, 0x225f, 0x22be, 0x220b, 0x22ef, 0x2026, 0x22ee, 0x22f1,
  0xf85d, 0x20e1, 0x002b, 0x002d, 0x003d, 0x002a, 0x002f, 0xf85f,
  0xf860, 0x210b, 0x2118, 0x2272, 0x2273, 0xf861
};

/* WP math/scientific extended (charset 7) */
const uint16_t mathextMap[] = {
  0x2320, 0x2321, 0xf702, 0xf703, 0x221a, 0xf705, 0xf706, 0xf707,
  0xf708, 0xf709, 0xf70a, 0xf70b, 0xf70c, 0xf70d, 0xf70e, 0xf70f,
  0xf710, 0xf711, 0xf712, 0xf713, 0xf714, 0xf715, 0xf716, 0xf717,
  0xf718, 0xf719, 0xf71a, 0xf71b, 0xf71c, 0xf71d, 0xf71e, 0xf71f,
  0xf720, 0xf721, 0xf722, 0xf723, 0xf724, 0xf725, 0xf726, 0xf727,
  0xf728, 0xf729, 0xf72a, 0xf72b, 0xf72c, 0xf72d, 0xf72e, 0xf72f,
  0xf730, 0xf731, 0xf732, 0xf733, 0xf734, 0xf735, 0xf736, 0xf737,
  0xf738, 0xf739, 0xf73a, 0xf73b, 0xf73c, 0xf73d, 0xf73e, 0xf73f,
  0xf740, 0xf741, 0xf742, 0xf743, 0xf744, 0xf745, 0xf746, 0xf747,
  0xf748, 0xf749, 0xf74a, 0xf74b, 0xf74c, 0xf74d, 0xf74e, 0xf74f,
  0xf750, 0xf751, 0xf752, 0xf753, 0xf754, 0xf755, 0xf756, 0xf757,
  0xf758, 0xf759, 0xf75a, 0xf75b, 0xf75c, 0xf75d, 0xf75e, 0xf75f,
  0xf760, 0xf761, 0xf762, 0xf763, 0xf764, 0xf765, 0xf766, 0xf767,
  0xf768, 0xf769, 0xf76a, 0xf76b, 0xf76c, 0xf76d, 0xf76e, 0xf76f,
  0xf770, 0xf771, 0xf772, 0xf773, 0xf774, 0xf775, 0xf776, 0xf777,
  0xf778, 0xf779, 0x20aa, 0xf77b, 0xf77c, 0xf77d, 0xf77e, 0xf77f,
  0xf780, 0xf781, 0xf782, 0xf783, 0xf784, 0xf785, 0xf786, 0xf787,
  0xf788, 0xf789, 0xf78a, 0xf78b, 0xf78c, 0xf78d, 0xf78e, 0xf78f,
  0xf790, 0xf791, 0xf792, 0xf793, 0xf794, 0xf795, 0xf796, 0xf797,
  0xf798, 0xf799, 0xf79a, 0xf79b, 0xf79c, 0xf79d, 0xf79e, 0xf79f,
  0xf7a0, 0xf7a1, 0xf7a2, 0xf7a3, 0xf7a4, 0xf7a5, 0xf7a6, 0xf7a7,
  0xf7a8, 0xf7a9, 0xf7aa, 0xf7ab, 0xf7ac, 0xf7ad, 0xf7ae, 0xf7af,
  0xf7b0, 0xf7b1, 0xf7b2, 0xf7b3, 0xf7b4, 0xf7b5, 0xf7b6, 0xf7b7,
  0xf7b8, 0xf7b9, 0xf7ba, 0xf7bb, 0xf7bc, 0xf7bd, 0xf7be, 0xf7bf,
  0xf7c0, 0xf7c1, 0xf7c2, 0xf7c3, 0xf7c4, 0xf7c5, 0xf7c6, 0xf7c7
};

/* WP greek (charset 8) */
const uint16_t greekMap[] = {
  0x0391, 0x03b1, 0x0392, 0x03b2, 0x0392, 0x03d0, 0x0393, 0x03b3,
  0x0394, 0x03b4, 0x0395, 0x03b5, 0x0396, 0x03b6, 0x0397, 0x03b7,
  0x0398, 0x03b8, 0x0399, 0x03b9, 0x039a, 0x03ba, 0x039b, 0x03bb,
  0x039c, 0x03bc, 0x039d, 0x03bd, 0x039e, 0x03be, 0x039f, 0x03bf,
  0x03a0, 0x03c0, 0x03a1, 0x03c1, 0x03a3, 0x03c3, 0x03a3, 0x03c2,
  0x03a4, 0x03c4, 0x03a5, 0x03c5, 0x03a6, 0x03c6, 0x03a7, 0x03c7,
  0x03a8, 0x03c8, 0x03a9, 0x03c9, 0x0386, 0x03ac, 0x0388, 0x03ad,
  0x0389, 0x03ae, 0x038a, 0x03af, 0x03aa, 0x03ca, 0x038c, 0x03cc,
  0x038e, 0x03cd, 0x03ab, 0x03cb, 0x038f, 0x03ce, 0x03b5, 0x03d1,
  0x03f0, 0x03d6, 0x03f1, 0x03db, 0x03d2, 0x03d5, 0x03d6, 0x03d7,
  0x00b7, 0x0374, 0x0375, 0x0301, 0x0308, 0xf216, 0xf217, 0x0300,
  0x0311, 0x0313, 0x0314, 0x0345, 0x1fce, 0x1fde, 0x1fcd, 0x1fdd,
  0xf200, 0xf201, 0xf022, 0xf021, 0xf202, 0xf203, 0xf204, 0xf300,
  0xf301, 0xf302, 0xf303, 0xf304, 0xf305, 0x1f70, 0xf100, 0x1fb3,
  0x1fb4, 0x1fb2, 0xf205, 0x1f00, 0x1f04, 0x1f02, 0xf206, 0x1f80,
  0x1f84, 0x1f82, 0xf306, 0x1f01, 0x1f05, 0x1f03, 0xf207, 0x1f81,
  0x1f85, 0x1f83, 0xf307, 0x1f72, 0x1f10, 0x1f14, 0x1f12, 0x1f11,
  0x1f15, 0x1f13, 0x1f74, 0xf101, 0x1fc3, 0x1fc4, 0x1fc2, 0xf208,
  0x1f20, 0x1f24, 0x1f22, 0xf209, 0x1f90, 0x1f94, 0x1f92, 0xf308,
  0x1f21, 0x1f25, 0x1f23, 0xf20a, 0x1f91, 0x1f95, 0x1f93, 0xf309,
  0x1f76, 0xf102, 0xf20b, 0xf20c, 0x1f30, 0x1f34, 0x1f32, 0xf20d,
  0x1f31, 0x1f35, 0x1f33, 0xf20e, 0x1f78, 0x1f40, 0x1f44, 0x1f42,
  0x1f41, 0x1f45, 0x1f43, 0x1fe5, 0x1fe4, 0x1f7a, 0xf103, 0xf20f,
  0xf210, 0x1f50, 0x1f54, 0x1f52, 0xf211, 0x1f51, 0x1f55, 0x1f53,
  0xf212, 0x1f7c, 0xf104, 0x1ff3, 0x1ff4, 0x1ff2, 0xf213, 0x1f60,
  0x1f64, 0x1f62, 0xf214, 0x1fa0, 0x1fa4, 0x1fa2, 0xf30a, 0x1f61,
  0x1f65, 0x1f63, 0xf215, 0x1fa1, 0x1fa5, 0x1fa3, 0xf30b, 0x03da,
  0x03dc, 0x03de, 0x03e0
};

/* WP hebrew (charset 9) */
const uint16_t hebrewMap[] = {
  0x05d0, 0x05d1, 0x05d2, 0x05d3, 0x05d4, 0x05d5, 0x05d6, 0x05d7,
  0x05d8, 0x05d9, 0x05da, 0x05db, 0x05dc, 0x05dd, 0x05de, 0x05df,
  0x05e0, 0x05e1, 0x05e2, 0x05e3, 0x05e4, 0x05e5, 0x05e6, 0x05e7,
  0x05e8, 0x05e9, 0x05ea, 0xf862, 0x05c0, 0x05c3, 0x05f3, 0x05f4,
  0x05b0, 0x05b1, 0x05b2, 0x05b3, 0x05b4, 0x05b5, 0x05b6, 0x05b7,
  0x05b8, 0x05b9, 0x05b9, 0x05bb, 0x05bc, 0x05bd, 0x05bf, 0x05b7,
  0xfb1e, 0x05f0, 0x05f1, 0x05f2, 0xf114, 0xf8b0, 0xf863, 0xf864,
  0xf865, 0xf866, 0xf867, 0xf868, 0xf869, 0xf86a, 0xf86b, 0xf86c,
  0xf86d, 0xf86e, 0xf86f, 0xf870, 0xf871, 0xf872, 0xf873, 0xf874,
  0x05f3, 0x05f3, 0x05f4, 0xf876, 0xf877, 0xf878, 0xf879, 0xf87a,
  0xf87b, 0xf87c, 0xf87d, 0xf87e, 0xf115, 0xf116, 0xf87f, 0xf117,
  0xf118, 0xf119, 0xf11a, 0xf11b, 0xf11c, 0xf11d, 0xf11e, 0xf11f,
  0xf120, 0xf121, 0xf122, 0xf123, 0xf124, 0xf125, 0xf126, 0xf127,
  0xf218, 0xf128, 0xf129, 0xf12a, 0xf12b, 0xf12c, 0xf12d, 0xf880,
  0xf12e, 0xf12f, 0xf130, 0xf219, 0x05e9, 0xf131, 0xf132, 0xf140,
  0xf141, 0xf142, 0x20aa
};

/* WP cyrillic (charset 10) */
const uint16_t cyrillicMap[] = {
  0x0410, 0x0430, 0x0411, 0x0431, 0x0412, 0x0432, 0x0413, 0x0433,
  0x0414, 0x0434, 0x0415, 0x0435, 0x0401, 0x0451, 0x0416, 0x0436,
  0x0417, 0x0437, 0x0418, 0x0438, 0x0419, 0x0439, 0x041a, 0x043a,
  0x041b, 0x043b, 0x041c, 0x043c, 0x041d, 0x043d, 0x041e, 0x043e,
  0x041f, 0x043f, 0x0420, 0x0440, 0x0421, 0x0441, 0x0422, 0x0442,
  0x0423, 0x0443, 0x0424, 0x0444, 0x0425, 0x0445, 0x0426, 0x0446,
  0x0427, 0x0447, 0x0428, 0x0448, 0x0429, 0x0449, 0x042a, 0x044a,
  0x042b, 0x044b, 0x042c, 0x044c, 0x042d, 0x044d, 0x042e, 0x044e,
  0x042f, 0x044f, 0x04d8, 0x04d9, 0x0403, 0x0453, 0x0490, 0x0491,
  0x0492, 0x0493, 0x0402, 0x0452, 0x0404, 0x0454, 0x0404, 0x0454,
  0x0496, 0x0497, 0x0405, 0x0455, 0xf159, 0xf889, 0xf15e, 0xf15f,
  0x0406, 0x0456, 0x0407, 0x0457, 0xf88c, 0xf88d, 0x0408, 0x0458,
  0x040c, 0x045c, 0x049a, 0x049b, 0xf160, 0xf161, 0x049c, 0x049d,
  0x0409, 0x0459, 0x04a2, 0x04a3, 0x040a, 0x045a, 0x047a, 0x047b,
  0x0460, 0x0461, 0x040b, 0x045b, 0x040e, 0x045e, 0x04ee, 0x04ef,
  0x04ae, 0x04af, 0x04b0, 0x04b1, 0x0194, 0x0263, 0x04b2, 0x04b3,
  0xf162, 0xf163, 0x04ba, 0x04bb, 0x047e, 0x047f, 0x040f, 0x045f,
  0x04b6, 0x04b7, 0x04b8, 0x04b9, 0xf164, 0xf165, 0x0462, 0x0463,
  0x0466, 0x0467, 0x046a, 0x046b, 0x046e, 0x046f, 0x0470, 0x0471,
  0x0472, 0x0473, 0x0474, 0x0475, 0xf400, 0xf401, 0xf402, 0xf403,
  0xf404, 0xf405, 0xf406, 0xf407, 0xf408, 0xf409, 0xf40a, 0xf40b,
  0xf40c, 0xf40d, 0xf40e, 0xf40f, 0xf410, 0xf411, 0xf412, 0xf413,
  0xf414, 0xf415, 0xf416, 0xf417, 0xf418, 0xf419, 0xf41a, 0xf41b,
  0xf41c, 0xf41d, 0xf41e, 0xf41f, 0xf420, 0xf421, 0xf422, 0xf423,
  0xf424, 0xf425, 0xf426, 0xf427, 0xf428, 0xf429, 0xf42a, 0xf42b,
  0x0301, 0x0300, 0x0308, 0x0306, 0x0326, 0x0328, 0x0304, 0xf893,
  0x201e, 0x201c, 0x10d0, 0x10d1, 0x10d2, 0x10d3, 0x10d4, 0x10d5,
  0x10d6, 0x10f1, 0x10d7, 0x10d8, 0x10d9, 0x10da, 0x10db, 0x10dc,
  0x10f2, 0x10dd, 0x10de, 0x10df, 0x10e0, 0x10e1, 0x10e2, 0x10e3,
  0x10f3, 0x10e4, 0x10e5, 0x10e6, 0x10e7, 0x10e8, 0x10e9, 0x10ea,
  0x10eb, 0x10ec, 0x10ed, 0x10ee, 0x10f4, 0x10ef, 0x10f0, 0x10f5,
  0x10f6, 0xf42c
};

/* WP japanese (charset 11) */
const uint16_t japaneseMap[] = {
  0xff61, 0xff62, 0xff63, 0xff64, 0xff65, 0xff66, 0xff67, 0xff68,
  0xff69, 0xff6a, 0xff6b, 0xff6c, 0xff6d, 0xff6e, 0xff6f, 0xff70,
  0xff71, 0xff72, 0xff73, 0xff74, 0xff75, 0xff76, 0xff77, 0xff78,
  0xff79, 0xff7a, 0xff7b, 0xff7c, 0xff7d, 0xff7e, 0xff7f, 0xff80,
  0xff81, 0xff82, 0xff83, 0xff84, 0xff85, 0xff86, 0xff87, 0xff88,
  0xff89, 0xff8a, 0xff8b, 0xff8c, 0xff8d, 0xff8e, 0xff8f, 0xff90,
  0xff91, 0xff92, 0xff93, 0xff94, 0xff95, 0xff96, 0xff97, 0xff98,
  0xff99, 0xff9a, 0xff9b, 0xff9c, 0xff9d, 0xff9e, 0xff9f
};

/* WP Tibetan (charset 12)
 *
 * WP6TibetanMap.h is generated automatically, so it doesn't make sense
 * to copy it in here every time it's regenerated.
 */

#include "WP6TibetanMap.h"
#include "WP6FileStructure.h"

int extendedCharacterToUCS2(uint8_t character,
			    uint8_t characterSet, const uint16_t **chars)
{
	int i;

	if (characterSet == 0)
	{
		// if characterset == 0, we have ascii. note that this is different from the doc. body
		// this is not documented in the file format specifications
		*chars = &asciiMap[character];
		return 1;
	}

	switch (characterSet)
	{
	case WP6_MULTINATIONAL_CHARACTER_SET:
		if (character < WP6_NUM_MULTINATIONAL_CHARACTERS) {
			*chars = &multinationalMap[character];
			return 1;
		}

	case WP6_PHONETIC_SYMBOL_CHARACTER_SET:
		if (character < WP6_NUM_PHONETIC_CHARACTERS) {
			*chars = &phoneticMap[character];
			return 1;
		}

	case WP6_BOX_DRAWING_CHARACTER_SET:
		if (character < WP6_NUM_BOX_DRAWING_CHARACTERS) {
			*chars = &boxdrawingMap[character];
			return 1;
		}


	case WP6_TYPOGRAPHIC_SYMBOL_CHARACTER_SET:
		if (character < WP6_NUM_TYPOGRAPHIC_CHARACTERS) {
			*chars = &typographicMap[character];
			return 1;
		}

	case WP6_ICONIC_SYMBOL_CHARACTER_SET:
		if (character < WP6_NUM_ICONIC_CHARACTERS) {
			*chars = &iconicMap[character];
			return 1;
		}

	case WP6_MATH_SCIENTIFIC_CHARACTER_SET:
		if (character < WP6_NUM_MATH_SCIENTIFIC_CHARACTERS) {
			*chars = &mathMap[character];
			return 1;
		}

	case WP6_MATH_SCIENTIFIC_EXTENDED_CHARACTER_SET:
		if (character < WP6_NUM_MATH_SCIENTIFIC_EXTENDED_CHARACTERS) {
			*chars = &mathextMap[character];
			return 1;
		}

	case WP6_GREEK_CHARACTER_SET:
		if (character < WP6_NUM_GREEK_CHARACTERS) {
			*chars = &greekMap[character];
			return 1;
		}

	case WP6_HEBREW_CHARACTER_SET:
		if (character < WP6_NUM_HEBREW_CHARACTERS) {
			*chars = &hebrewMap[character];
			return 1;
		}

	case WP6_CYRILLIC_CHARACTER_SET:
		if (character < WP6_NUM_CYRILLIC_CHARACTERS) {
			*chars = &cyrillicMap[character];
			return 1;
		}

	case WP6_JAPANESE_CHARACTER_SET:
		if (character < WP6_NUM_JAPANESE_CHARACTERS) {
			*chars = &japaneseMap[character];
			return 1;
		}

	case WP6_TIBETAN_CHARACTER_SET:
		if (tibetanMap1[character]) {
			for (i = 0; tibetanMap1[character][i]; i++)
				;
			*chars = tibetanMap1[character];
			return i;
		}
	}

	// last resort: return whitespace
	*chars = &asciiMap[' '];
	return 1;
}

_RGBSColor::_RGBSColor(uint8_t r, uint8_t g, uint8_t b, uint8_t s)
	:	m_r(r),
		m_g(g),
		m_b(b),
		m_s(s)
{
}

_RGBSColor::_RGBSColor()
	:	m_r(0),
		m_g(0),
		m_b(0),
		m_s(0)
{
}

_WPXTabStop::_WPXTabStop(float position, WPXTabAlignment alignment, uint16_t leaderCharacter, uint8_t leaderNumSpaces)
	:	m_position(position),
		m_alignment(alignment),
		m_leaderCharacter(leaderCharacter),
		m_leaderNumSpaces(leaderNumSpaces)
{
}

_WPXTabStop::_WPXTabStop()
	:	m_position(0.0f),
		m_alignment(LEFT),
		m_leaderCharacter('\0'),
		m_leaderNumSpaces(0)
{
}

_WPXColumnDefinition::_WPXColumnDefinition()
	:	m_width(0),
		m_leftGutter(0),
		m_rightGutter(0)
{
}

int g_static_utf8_strlen (const char *p);
int g_static_unichar_to_utf8 (uint32_t c,  char *outbuf);

static const int8_t g_static_utf8_skip_data[256] = {
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,6,6,1,1
};
#define UTF8_LENGTH(Char)              \
  ((Char) < 0x80 ? 1 :                 \
   ((Char) < 0x800 ? 2 :               \
    ((Char) < 0x10000 ? 3 :            \
     ((Char) < 0x200000 ? 4 :          \
      ((Char) < 0x4000000 ? 5 : 6)))))
#define g_static_utf8_next_char(p) (char *)((p) + g_static_utf8_skip_data[*((uint8_t *)p)])
		


UTF8String::UTF8String() 
{
}

UTF8String::UTF8String(const UTF8String &stringBuf, bool escapeXML) 
{
	if (escapeXML)
	{
		int len = strlen(stringBuf.cstr()); // want to use standard strlen
		const char *p = stringBuf.cstr();
		const char *end = p + len; 
		while (p != end)
		{
			const char *next = g_static_utf8_next_char(p);

			switch (*p)
			{
			case '&':
				m_buf.append("&amp;");
				break;
			case '<':
				m_buf.append("&lt;");
				break;
			case '>':
				m_buf.append("&gt;");
				break;
			case '\'':
				m_buf.append("&apos;");
				break;
			case '"':
				m_buf.append("&quot;");
				break;
			default:
				while (p != next) 
				{		
					m_buf+=(*p);
					p++;
				}
				break;
			}

			p = next;
		}
	}
	else
		this->sprintf("%s", stringBuf.cstr());
}

static void appendUCS4(string &buf, uint32_t ucs4)
{
	int charLength = g_static_unichar_to_utf8(ucs4, NULL);
	char *utf8;
	utf8 = new(char[charLength]);
	g_static_unichar_to_utf8(ucs4, utf8);
	for (int j=0; j<charLength; j++)
		buf+=utf8[j];
	delete[] utf8;
}

UTF8String::UTF8String(const char *str) :
	m_buf(str)
{
}

void UTF8String::append(const uint16_t ucs2)
{
	uint32_t ucs4 = (uint32_t) ucs2;
	appendUCS4(m_buf, ucs2);
}

#define FIRST_BUF_SIZE 128;
#ifdef _MSC_VER
#define vsnprintf _vsnprintf
#endif

void UTF8String::sprintf(const char *format, ...)
{
	va_list args;

	int bufsize = FIRST_BUF_SIZE;
	char * buf = NULL;

	for (;;)
	{
			buf = new char[bufsize];
			va_start(args, format);
			int outsize = vsnprintf(buf, bufsize, format, args);
			va_end(args);
			if ((outsize == -1) | (outsize == bufsize) | (outsize == bufsize - 1))
			{
				bufsize = bufsize * 2;
				delete [] buf;
			}
			else if (outsize > bufsize)
			{
				bufsize = outsize + 2;
				delete [] buf;
			}
			else
				break;
	}

	m_buf = buf;
	delete [] buf;
}

void UTF8String::append(const UTF8String &s)
{
	m_buf += s.cstr();
}

void UTF8String::append(const char *s)
{
	m_buf += s;
}

int UTF8String::len() const
{ 
	return g_static_utf8_strlen(m_buf.c_str()); 
}

bool UTF8String::Iter::next()
{
	int len = strlen(m_buf.c_str());

	if (m_pos == (-1)) 
		m_pos++;
	else if (m_pos < len)
	{
		m_pos+=(int32_t) (g_static_utf8_next_char(&m_buf.c_str()[m_pos]) - &m_buf.c_str()[m_pos]);
	}

	if (m_pos < len)
		return true;
	return false;
}

bool UTF8String::Iter::last()
{
	if (m_pos >= g_static_utf8_strlen(m_buf.c_str()))
		return true;
	return false;
}

const char * UTF8String::Iter::operator()() const
{ 
	if (m_pos == (-1)) return NULL; 

	delete [] m_curChar; m_curChar = NULL;
	int32_t charLength =(int32_t) (g_static_utf8_next_char(&m_buf.c_str()[m_pos]) - &m_buf.c_str()[m_pos]);
	m_curChar = new char[charLength+1];
	for (int i=0; i<charLength; i++)
		m_curChar[i] = m_buf[m_pos+i];
	m_curChar[charLength]='\0';

	return m_curChar;
}

/**
 * g_static_unichar_to_utf8:
 *
 * stolen from glib 2.4.1
 *
 * @c: a ISO10646 character code
 * @outbuf: output buffer, must have at least 6 bytes of space.
 *       If %NULL, the length will be computed and returned
 *       and nothing will be written to @outbuf.
 * 
 * Converts a single character to UTF-8.
 * 
 * Return value: number of bytes written
 **/
int
g_static_unichar_to_utf8 (uint32_t c,
			  char   *outbuf)
{
	unsigned int len = 0;    
	int first;
	int i;
    
	if (c < 0x80)
	{
		first = 0;
		len = 1;
	}
	else if (c < 0x800)
	{
		first = 0xc0;
		len = 2;
	}
	else if (c < 0x10000)
	{
		first = 0xe0;
		len = 3;
	}
	else if (c < 0x200000)
	{
		first = 0xf0;
		len = 4;
	}
	else if (c < 0x4000000)
	{
		first = 0xf8;
		len = 5;
	}
	else
	{
		first = 0xfc;
		len = 6;
	}
    
	if (outbuf)
	{
		for (i = len - 1; i > 0; --i)
		{
			outbuf[i] = (c & 0x3f) | 0x80;
			c >>= 6;
		}
		outbuf[0] = c | first;
	}
    
	return len;
}

/**
 * g_static_utf8_strlen:
 * @p: pointer to the start of a UTF-8 encoded string.

 * 
 * Returns the length of the string in characters.
 *
 * Return value: the length of the string in characters
 **/
int
g_static_utf8_strlen (const char *p)
{
	long len = 0;
	const char *start = p;
	if (p == NULL)
		return 0;

	while (*p)
	{
		p = g_static_utf8_next_char (p);
		++len;
	}

	return len;
}
