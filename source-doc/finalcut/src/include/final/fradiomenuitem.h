/***********************************************************************
* fradiomenuitem.h - Widget FRadioMenuItem                             *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2015-2020 Markus Gans                                      *
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

/*  Inheritance diagram
 *  ═══════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▏ ▕▔▔▔▔▔▔▔▔▔▏
 * ▕ FVTerm  ▏ ▕ FObject ▏
 * ▕▁▁▁▁▁▁▁▁▁▏ ▕▁▁▁▁▁▁▁▁▁▏
 *      ▲           ▲
 *      │           │
 *      └─────┬─────┘
 *            │
 *       ▕▔▔▔▔▔▔▔▔▔▏
 *       ▕ FWidget ▏
 *       ▕▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *      ▕▔▔▔▔▔▔▔▔▔▔▔▏
 *      ▕ FMenuItem ▏
 *      ▕▁▁▁▁▁▁▁▁▁▁▁▏
 *            ▲
 *            │
 *    ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏*       1▕▔▔▔▔▔▔▔▏
 *    ▕ FRadioMenuItem ▏- - - - -▕ FMenu ▏
 *    ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏         ▕▁▁▁▁▁▁▁▏
 */

#ifndef FRADIOMENUITEM_H
#define FRADIOMENUITEM_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include "final/fmenuitem.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FRadioMenuItem
//----------------------------------------------------------------------

class FRadioMenuItem : public FMenuItem
{
  public:
    // Constructors
    explicit FRadioMenuItem (FWidget* = nullptr);
    explicit FRadioMenuItem (const FString&, FWidget* = nullptr);

    // Disable copy constructor
    FRadioMenuItem (const FRadioMenuItem&) = delete;

    // Destructor
    ~FRadioMenuItem() noexcept override;

    // Disable copy assignment operator (=)
    FRadioMenuItem& operator = (const FRadioMenuItem&) = delete;

    // Accessor
    FString       getClassName() const override;

  private:
    // Methods
    void          init();
    void          processToggle() const;
    void          processClicked() override;
};

// FRadioMenuItem inline functions
//----------------------------------------------------------------------
inline FString FRadioMenuItem::getClassName() const
{ return "FRadioMenuItem"; }

}  // namespace finalcut

#endif  // FRADIOMENUITEM_H
