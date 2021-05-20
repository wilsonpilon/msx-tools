/***********************************************************************
* fcolorpalette.h - Define RGB color value for a palette entry         *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2018-2020 Markus Gans                                      *
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
 * ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 * ▕ FColorPalette ▏
 * ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef FCOLORPALETTE_H
#define FCOLORPALETTE_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include <functional>

#include "final/fstring.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FColorPalette
//----------------------------------------------------------------------

class FColorPalette
{
  public:
    // Using-declaration
    using FSetPalette = std::function<void(FColor, int, int, int)>;

    // Constructor
    explicit FColorPalette (const FSetPalette&);

    // Destructor
    virtual ~FColorPalette() noexcept;

    // Accessor
    virtual FString getClassName() const;

    // Methods
    virtual void setColorPalette() = 0;
    virtual void resetColorPalette() = 0;

  protected:
    void setPalette (FColor, int, int, int) const;
    void setVGAdefaultPalette() const;

  private:
    // Data members
    FSetPalette set_palette;
};

// FColorPalette inline functions
//----------------------------------------------------------------------
inline FString FColorPalette::getClassName() const
{ return "FColorPalette"; }


/*  Inheritance diagram
 *  ═══════════════════
 *
 *      ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *      ▕ FColorPalette ▏
 *      ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *              ▲
 *              │
 *  ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *  ▕ default8ColorPalette ▏
 *  ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

//----------------------------------------------------------------------
// class default8ColorPalette
//----------------------------------------------------------------------

class default8ColorPalette final : public FColorPalette
{
  public:
    // Constructor
    explicit default8ColorPalette (const FSetPalette&);

    // Destructor
    ~default8ColorPalette() noexcept override;

    // Accessor
    FString getClassName() const override;

    // Methods
    void setColorPalette() override;
    void resetColorPalette() override;
};

// default8ColorPalette inline functions
//----------------------------------------------------------------------
inline FString default8ColorPalette::getClassName() const
{ return "default8ColorPalette"; }


/*  Inheritance diagram
 *  ═══════════════════
 *
 *      ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *      ▕ FColorPalette ▏
 *      ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *              ▲
 *              │
 *  ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *  ▕ default16ColorPalette ▏
 *  ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

//----------------------------------------------------------------------
// class default16ColorPalette
//----------------------------------------------------------------------

class default16ColorPalette final : public FColorPalette
{
  public:
    // Constructor
    explicit default16ColorPalette (const FSetPalette&);

    // Destructor
    ~default16ColorPalette() noexcept override;

    // Accessor
    FString getClassName() const override;

    // Methods
    void setColorPalette() override;
    void resetColorPalette() override;
};

// default16ColorPalette inline functions
//----------------------------------------------------------------------
inline FString default16ColorPalette::getClassName() const
{ return "default16ColorPalette"; }

/*  Inheritance diagram
 *  ═══════════════════
 *
 *        ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *        ▕ FColorPalette ▏
 *        ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 *                ▲
 *                │
 *  ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 *  ▕ default16DarkColorPalette ▏
 *  ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

//----------------------------------------------------------------------
// class default16DarkColorPalette
//----------------------------------------------------------------------

class default16DarkColorPalette final : public FColorPalette
{
  public:
    // Constructor
    explicit default16DarkColorPalette (const FSetPalette&);

    // Destructor
    ~default16DarkColorPalette() noexcept override;

    // Accessor
    FString getClassName() const override;

    // Methods
    void setColorPalette() override;
    void resetColorPalette() override;
};

// default16ColorPalette inline functions
//----------------------------------------------------------------------
inline FString default16DarkColorPalette::getClassName() const
{ return "default16DarkColorPalette"; }

}  // namespace finalcut

#endif  // FCOLORPALETTE_H
