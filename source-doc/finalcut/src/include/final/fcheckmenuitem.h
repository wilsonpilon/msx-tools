/***********************************************************************
* fcheckmenuitem.h - Widget FCheckMenuItem                             *
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
 *    ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏*     1▕▔▔▔▔▔▔▔▏
 *    ▕ FCheckMenuItem ▏- - - -▕ FMenu ▏
 *    ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏       ▕▁▁▁▁▁▁▁▏
 */

#ifndef FCHECKMENUITEM_H
#define FCHECKMENUITEM_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include "final/fmenuitem.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FCheckMenuItem
//----------------------------------------------------------------------

class FCheckMenuItem : public FMenuItem
{
  public:
    // Constructors
    explicit FCheckMenuItem (FWidget* = nullptr);
    explicit FCheckMenuItem (const FString&, FWidget* = nullptr);

    // Disable copy constructor
    FCheckMenuItem (const FCheckMenuItem&) = delete;

    // Destructor
    ~FCheckMenuItem() noexcept override;

    // Disable copy assignment operator (=)
    FCheckMenuItem& operator = (const FCheckMenuItem&) = delete;

    // Accessor
    FString             getClassName() const override;

  private:
    // Methods
    void                init();
    void                processToggle() const;
    void                processClicked() override;
};

// FCheckMenuItem inline functions
//----------------------------------------------------------------------
inline FString FCheckMenuItem::getClassName() const
{ return "FCheckMenuItem"; }

}  // namespace finalcut

#endif  // FCHECKMENUITEM_H
