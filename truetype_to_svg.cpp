// Read TrueType (R) outline, write SVG
// Copyright Don Bright 2013 <hugh.m.bright@gmail.com>
/*

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  License based on zlib license, by Jean-loup Gailly and Mark Adler
*/

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

FT_Vector halfway_between( FT_Vector p1, FT_Vector p2 )
{
	FT_Vector newv;
	newv.x = p1.x + (p2.x-p1.x)/2;
	newv.y = p1.y + (p2.y-p1.y)/2;
	return newv;
}

int main( int argc, char * argv[] )
{
	if (argc!=3) {
		cerr << "usage: " << argv[0] << " file.ttf 0x0042\n";
		exit( 1 );
	}

	cout << "<!--";
	cout << "\nFreetype - example 5\n";

	unsigned char glyphname[64];
	string filename( argv[1] );
	string unicode_s( argv[2] );
	int unicode = strtol( unicode_s.c_str(), NULL, 0 );
  FT_Library library;
  FT_Face face;
	FT_Error error;
	error = FT_Init_FreeType( &library );
	cout << "Init error code: " << error;

	// Load a typeface
	error = FT_New_Face( library, filename.c_str(), 0, &face );
	cout << "\nFace load error code: " << error;
	cout << "\nTrueType filename: " << filename;
	if (error) {
		cerr << "problem loading file " << filename << "\n";
		exit(1);
	}
	cout << "\nFamily Name: " << face->family_name;
	cout << "\nStyle Name: " << face->style_name;
	cout << "\nNumber of faces: " << face->num_faces;
	cout << "\nNumber of glyphs: " << face->num_glyphs;

	// Load the Glyph into the face's Glyph Slot + print details
	FT_UInt glyph_index = FT_Get_Char_Index( face, unicode );
	cout << "\nUnicode requested: " << unicode_s;
	cout << " (decimal: " << unicode << " hex: 0x" << hex << unicode << dec << ")";
	cout << "\nGlyph index for unicode: " << glyph_index;
	error = FT_Load_Glyph( face, glyph_index, FT_LOAD_NO_SCALE );
	cout << "\nLoad Glyph into Face's glyph slot. error code: " << error;
	FT_GlyphSlot slot = face->glyph;

	char glyph_name[1024];
	FT_Get_Glyph_Name( face, glyph_index, glyph_name, 1024 );
	FT_Glyph_Metrics gm = slot->metrics;
	cout << "\nGlyph Name: " << glyph_name;
	cout << "\nGlyph Width: " << gm.width
		<< " Height: " << gm.height
		<< " Hor. Advance: " << gm.horiAdvance
		<< " Vert. Advance: " << gm.vertAdvance;

	// Print outline details, taken from the glyph in the slot.
	FT_Outline outline = slot->outline;
	char* tags = outline.tags;
	short* contours = outline.contours;
  cout << "\nNum points: " << outline.n_points;
  cout << "\nNum contours: " << outline.n_contours;
	cout << "  Endpoint indexes:";
	for ( int i = 0 ; i < outline.n_contours ; i++ ) cout << " " << contours[i];
	FT_Vector* points = outline.points;
	cout << "\n-->\n";


	// SVG output. See these sites for more info.
	// Basic Terms: http://www.freetype.org/freetype2/docs/glyphs/glyphs-3.html
	// FType + outlines: http://www.freetype.org/freetype2/docs/reference/ft2-outline_processing.html
	// FType + contours: http://www.freetype.org/freetype2/docs/glyphs/glyphs-6.html
	// TType contours: https://developer.apple.com/fonts/TTRefMan/RM01/Chap1.html
	// TType contours2: http://www.truetype-typography.com/ttoutln.htm
	// Non-zero winding rule: http://en.wikipedia.org/wiki/Nonzero-rule
	// SVG paths: http://www.w3schools.com/svg/svg_path.asp
	// SVG paths + nonzero: http://www.w3.org/TR/SVG/painting.html#FillProperties

	// Invert y coordinates (SVG = neg at top, TType = neg at bottom)
	for ( int i = 0 ; i < outline.n_points ; i++ )
		points[i].y *= -1;

	stringstream svg;
	int bbheight = face->bbox.yMax - face->bbox.yMin;
	int bbwidth = face->bbox.xMax - face->bbox.xMin;
	svg << "\n<svg width='" << bbwidth << "px'"
		<< " height='" << bbheight << "px'"
    << " xmlns='http://www.w3.org/2000/svg' version='1.1'>";

	svg << "\n\n <!-- draw border -->";
	svg << "\n <rect fill='none' stroke='black'"
		<< " width='" << bbwidth - 1 << "'"
		<< " height='" << bbheight - 1 << "'/>";

	svg << "\n\n <!-- make sure glyph is visible within svg window -->";
	int yadj = gm.horiBearingY + gm.vertBearingY + 100;
	int xadj = 100;
	svg << "\n <g fill-rule='nonzero' "
		<< " transform='translate(" << xadj << " " << yadj << ")'"
		<< ">";

	svg << "\n\n  <!-- draw axes --> ";
	svg << "\n <path stroke='blue' stroke-dasharray='5,5' d='"
		<< " M" << -bbwidth << "," << 0
		<< " L" <<  bbwidth << "," << 0
		<< " M" << 0 << "," << -bbheight
		<< " L" << 0 << "," << bbheight
		<< " '/>";

	svg << "\n\n  <!-- draw bearing + advance box --> ";
	int x1 = 0;
	int x2 = gm.horiAdvance;
	int y1 = -gm.vertBearingY-gm.height;
	int y2 = y1 + gm.vertAdvance;
	svg << "\n <path stroke='blue' fill='none' stroke-dasharray='10,16' d='"
		<< " M" << x1 << "," << y1
		<< " M" << x1 << "," << y2
		<< " L" << x2 << "," << y2
		<< " L" << x2 << "," << y1
		<< " L" << x1 << "," << y1
		<< " '/>";

	svg << "\n\n  <!-- draw points as circles -->";
	int contour_index = 0;
	for ( int i = 0 ; i < outline.n_points ; i++ ) {
		int radius = 5;
		if ( i == 0 ) radius = 10;
		string color;
		if (tags[i] & 1) color = "blue"; else color = "none";
		svg << "\n  <!--" << i << "-->";
		svg << "<circle"
			<< " fill='" << color << "'"
			<< " stroke='black'"
			<< " cx='" << points[i].x << "' cy='" << points[i].y << "'"
			<< " r='" << radius << "'"
			<< "/>";
	}

	svg << "\n\n  <!-- draw straight lines between points -->";
	svg << "\n  <path fill='none' stroke='green' d='";
	svg << "\n   M " << points[0].x << "," << points[0].y << " L ";
	for ( int i = 0 ; i < outline.n_points ; i++ ) {
		svg << " " << points[i].x << "," << points[i].y;
	}
	svg << "\n  '/>";



	svg << "\n\n  <!-- draw actual outline using lines and Bezier curves-->";
	svg	<< "\n\n  <path fill='black' stroke='black'"
		<< " fill-opacity='0.45' "
		<< " stroke-width='2' "
		<< " d='";
	int contour_starti = 0;
	int contour_endi = contours[0];
	int contour_counter = 0;
	svg << "\n   M" << points[0].x << "," << points[0].y;
	for ( int i = 0 ; i < outline.n_points ; i++ ) {
		int previndex = i-1;
		int currindex = i;
		int nextindex = i+1;
		if ( nextindex > contour_endi ) nextindex = contour_starti;
		if ( previndex < contour_starti ) previndex = contour_endi;
		// tag bit 1 indicates whether its a control point on a bez curve or not.
		// two consecutive control points imply another point halfway between them
		if ( tags[currindex] & 1 ) {
			if ( tags[previndex] & 1 ) {
				svg << "\n    L" << points[currindex].x << "," << points[currindex].y;
			}
		} else {
			svg << "\n    Q" << points[currindex].x << "," << points[currindex].y;
			FT_Vector nextp = points[nextindex];
			if ( ! ( tags[nextindex] & 1 ) ) {
				nextp = halfway_between( points[currindex], points[nextindex] );
			}
			svg << " " << nextp.x << "," << nextp.y;
		}
		if ( currindex == contour_endi ) {
			contour_counter = ( contour_counter + 1 ) % outline.n_contours;
			contour_starti = ( currindex + 1 ) % outline.n_points;;
			contour_endi = contours[contour_counter];
			//svg << "\n   " << contour_starti << " " << contour_endi;
			FT_Vector firstp;
			if ( ! ( tags[contour_starti] & 1 ) )
				firstp = halfway_between( points[contour_endi], points[contour_starti] );
			else
				firstp = points[contour_starti];
			svg << "\n  Z M" << firstp.x << "," << firstp.y;
		}
	}
	svg << "\n  '/>";
	svg << "\n\n </g>";
	svg << "\n</svg>";

	cout << svg.str();

	// free memory
	cout << "\n<!--";
	error = FT_Done_Face( face );
	cout << "\nFree face. error code: " << error;
	error = FT_Done_FreeType( library );
	cout << "\nFree library. error code: " << error;
	cout << "\n-->\n";

  return 0;
}
