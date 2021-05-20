/***********************************************************************
* emptyfstring.h - Creates an empty FString object                     *
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

/*  Standalone class
 *  ════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▔▔▔▔▔▔▏
 * ▕ emptyFString ▏
 * ▕▁▁▁▁▁▁▁▁▁▁▁▁▁▁▏
 */

#ifndef EMPTYFSTRING_H
#define EMPTYFSTRING_H

#if !defined (USE_FINAL_H) && !defined (COMPILE_FINAL_CUT)
  #error "Only <final/final.h> can be included directly."
#endif

#include <iostream>
#include <memory>

#include "final/flog.h"
#include "final/fstring.h"

namespace finalcut
{

namespace fc
{

//----------------------------------------------------------------------
// class emptyFString
//----------------------------------------------------------------------

class emptyFString final
{
  public:
    // Constructors
    emptyFString() = delete;

    // Disable copy constructor
    emptyFString (const emptyFString&) = delete;

    // Disable copy assignment operator (=)
    emptyFString& operator = (const emptyFString&) = delete;

    static FString getClassName()
    {
      return "emptyFString";
    }

    static const FString& get()
    {
      static const auto& empty_string = make_unique<FString>("");
      return *empty_string.get();
    }
};

}  // namespace fc

}  // namespace finalcut

#endif  // EMPTYFSTRING_H
