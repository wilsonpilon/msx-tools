/***********************************************************************
* fmenulist.cpp - Menu item container base class                       *
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

#include <vector>

#include "final/fc.h"
#include "final/fmenulist.h"
#include "final/fmenuitem.h"

namespace finalcut
{

//----------------------------------------------------------------------
// class FMenuList
//----------------------------------------------------------------------

// constructor and destructor
//----------------------------------------------------------------------
FMenuList::~FMenuList()  // destructor
{
  // delete all items
  if ( item_list.empty() )
    return;

  std::vector<FMenuItem*>::iterator iter;
  iter = item_list.begin();

  while ( iter != item_list.end() )
  {
    (*iter)->setSuperMenu(nullptr);
    iter = item_list.erase(iter);
  }
}


// public methods of FMenuList
//----------------------------------------------------------------------
void FMenuList::insert (FMenuItem* i)
{
  item_list.push_back(i);
}

//----------------------------------------------------------------------
void FMenuList::remove (FMenuItem* i)
{
  std::vector<FMenuItem*>::iterator iter;

  if ( item_list.empty() )
    return;

  iter = item_list.begin();

  while ( iter != item_list.end() )
  {
    if ( (*iter) == i )
    {
      iter = item_list.erase(iter);
      i->setSuperMenu(nullptr);
      break;
    }
    else
      ++iter;
  }
}

//----------------------------------------------------------------------
void FMenuList::remove (int pos)
{
  if ( int(getCount()) < pos )
    return;

  item_list.erase (item_list.begin() + pos - 1);
}

//----------------------------------------------------------------------
void FMenuList::clear()
{
  item_list.clear();
  item_list.shrink_to_fit();
}

//----------------------------------------------------------------------
void FMenuList::selectFirstItem()
{
  std::vector<FMenuItem*>::const_iterator iter;
  std::vector<FMenuItem*>::const_iterator end;
  iter = item_list.begin();
  end = item_list.end();

  if ( item_list.empty() )
    return;

  if ( hasSelectedItem() )
    unselectItem();

  while ( iter != end )
  {
    if ( (*iter)->isEnabled() && ! (*iter)->isSeparator() )
    {
      // select first enabled item
      (*iter)->setSelected();
      setSelectedItem(*iter);
      break;
    }

    ++iter;
  }
}

//----------------------------------------------------------------------
void FMenuList::unselectItem()
{
  if ( hasSelectedItem() )
    getSelectedItem()->unsetSelected();

  setSelectedItem(nullptr);
}

}  // namespace finalcut
