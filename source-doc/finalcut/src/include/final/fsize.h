/***********************************************************************
* fsize.h - Height and width of a two-dimensional surface              *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2014-2020 Markus Gans                                      *
*                                                                      *
* FINAL CUT is free software; you can redistribute it and/or modify    *
* it under the terms of the GNU Lesser General Public License as       *
* published by the Free Software Foundation; either version 3 of       *
* the License, or (at your option) any later version.                  *
*                                                                      *
* FINAL CUT is distributed in the hope that it will be useful, but     *
* WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
* GNU Lesser General Public License for more details.                  *
*                                                                      *
* You should have received a copy of the GNU Lesser General Public     *
* License along with this program.  If not, see                        *
* <http://www.gnu.org/licenses/>.                                      *
***********************************************************************/

/*  Standalone class
 *  ════════════════
 *
 * ▕▔▔▔▔▔▔▔▏
 * ▕ FSize ▏
 * ▕▁▁▁▁▁▁▁▏
 */

#ifndef FSIZE_H
#define FSIZE_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include <limits>
#include <iostream>
#include <utility>

#include "final/fstring.h"
#include "final/ftypes.h"

namespace finalcut
{

// class forward declaration
class FPoint;

//----------------------------------------------------------------------
// class FSize
//----------------------------------------------------------------------

class FSize
{
  public:
    // Constructors
    FSize () noexcept = default;
    FSize (std::size_t, std::size_t) noexcept;

    // Overloaded operators
    FSize& operator += (const FSize&);
    FSize& operator -= (const FSize&);

    // Accessors
    FString               getClassName() const;
    std::size_t           getWidth() const;
    std::size_t           getHeight() const;
    std::size_t           getArea() const;
    void                  setWidth (std::size_t);
    void                  setHeight (std::size_t);
    void                  setSize (const FSize&);
    void                  setSize (std::size_t, std::size_t);

    // Inquiry
    bool                  isEmpty() const;

    // Side references
    std::size_t&          width_ref();
    std::size_t&          height_ref();

    // Methods
    void                  scaleBy (int, int);
    void                  scaleBy (const FPoint&);

  private:
    // Data members
    std::size_t width{0};
    std::size_t height{0};

    // Friend operator functions
    friend bool operator <  (const FSize&, const FSize&);
    friend bool operator <= (const FSize&, const FSize&);
    friend bool operator == (const FSize&, const FSize&);
    friend bool operator != (const FSize&, const FSize&);
    friend bool operator >= (const FSize&, const FSize&);
    friend bool operator >  (const FSize&, const FSize&);
    friend FSize operator + (const FSize&, const FSize&);
    friend FSize operator - (const FSize&, const FSize&);

    friend std::ostream& operator << (std::ostream&, const FSize&);
    friend std::istream& operator >> (std::istream&, FSize&);
};

// FSize inline functions
//----------------------------------------------------------------------
inline FSize::FSize (std::size_t w, std::size_t h) noexcept
  : width{w}
  , height{h}
{ }

//----------------------------------------------------------------------
inline FString FSize::getClassName() const
{ return "FSize"; }

//----------------------------------------------------------------------
inline std::size_t FSize::getWidth() const
{ return width; }

//----------------------------------------------------------------------
inline std::size_t FSize::getHeight() const
{ return height; }

//----------------------------------------------------------------------
inline std::size_t FSize::getArea() const
{ return width * height; }

//----------------------------------------------------------------------
inline std::size_t& FSize::width_ref()
{ return width; }

//----------------------------------------------------------------------
inline std::size_t& FSize::height_ref()
{ return height; }


// FSize non-member operators
//----------------------------------------------------------------------
inline bool operator < (const FSize& s1, const FSize& s2)
{ return s1.width < s2.width && s1.height < s2.height; }

//----------------------------------------------------------------------
inline bool operator <= (const FSize& s1, const FSize& s2)
{ return s1.width <= s2.width && s1.height <= s2.height; }

//----------------------------------------------------------------------
inline bool operator == (const FSize& s1, const FSize& s2)
{ return s1.width == s2.width && s1.height == s2.height; }

//----------------------------------------------------------------------
inline bool operator != (const FSize& s1, const FSize& s2)
{ return s1.width != s2.width || s1.height != s2.height; }

//----------------------------------------------------------------------
inline bool operator >= (const FSize& s1, const FSize& s2)
{ return s1.width >= s2.width && s1.height >= s2.height; }

//----------------------------------------------------------------------
inline bool operator > (const FSize& s1, const FSize& s2)
{ return s1.width > s2.width && s1.height > s2.height; }

//----------------------------------------------------------------------
inline FSize operator + (const FSize& s1, const FSize& s2)
{
  constexpr std::size_t max = std::numeric_limits<std::size_t>::max();
  const std::size_t w = ( s1.width < max - s2.width) ? s1.width + s2.width : max;
  const std::size_t h = ( s1.height < max - s2.height) ? s1.height + s2.height : max;
  return {w, h};
}

//----------------------------------------------------------------------
inline FSize operator - (const FSize& s1, const FSize& s2)
{
  const std::size_t w = ( s1.width >= s2.width ) ? s1.width - s2.width : 0;
  const std::size_t h = ( s1.height >= s2.height ) ? s1.height - s2.height : 0;
  return {w, h};
}

}  // namespace finalcut

#endif  // FSIZE_H
