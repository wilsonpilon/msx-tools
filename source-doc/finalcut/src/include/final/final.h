/***********************************************************************
* final.h - Include all in FINAL CUT required functions,               *
*           types and macros                                           *
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

#ifndef FINAL_H
#define FINAL_H

#define USE_FINAL_H

#include <final/emptyfstring.h>
#include <final/sgr_optimizer.h>
#include <final/fkey_map.h>
#include <final/fapplication.h>
#include <final/fbuttongroup.h>
#include <final/fbutton.h>
#include <final/fbusyindicator.h>
#include <final/fc.h>
#include <final/fdata.h>
#include <final/fobject.h>
#include <final/fcolorpalette.h>
#include <final/fcolorpair.h>
#include <final/fcombobox.h>
#include <final/fcharmap.h>
#include <final/fcheckbox.h>
#include <final/fcheckmenuitem.h>
#include <final/fdialog.h>
#include <final/fdialoglistmenu.h>
#include <final/fevent.h>
#include <final/ffiledialog.h>
#include <final/fkeyboard.h>
#include <final/flabel.h>
#include <final/flineedit.h>
#include <final/flistbox.h>
#include <final/flistview.h>
#include <final/flog.h>
#include <final/flogger.h>
#include <final/fmenubar.h>
#include <final/fmenu.h>
#include <final/fmenuitem.h>
#include <final/fmessagebox.h>
#include <final/fmouse.h>
#include <final/foptiattr.h>
#include <final/foptimove.h>
#include <final/fpoint.h>
#include <final/fprogressbar.h>
#include <final/fradiobutton.h>
#include <final/fradiomenuitem.h>
#include <final/frect.h>
#include <final/fscrollbar.h>
#include <final/fscrollview.h>
#include <final/fsize.h>
#include <final/fspinbox.h>
#include <final/fstartoptions.h>
#include <final/fstatusbar.h>
#include <final/fstring.h>
#include <final/fstyle.h>
#include <final/fswitch.h>
#include <final/fsystem.h>
#include <final/fterm.h>
#include <final/ftermbuffer.h>
#include <final/ftermcap.h>
#include <final/ftermcapquirks.h>
#include <final/ftermdata.h>
#include <final/ftermdebugdata.h>
#include <final/ftermdetection.h>
#include <final/ftermios.h>
#include <final/ftermxterminal.h>
#include <final/ftextview.h>
#include <final/ftogglebutton.h>
#include <final/ftooltip.h>
#include <final/ftypes.h>
#include <final/fvterm.h>
#include <final/fwidgetcolors.h>
#include <final/fwidget.h>
#include <final/fwindow.h>

#if defined(UNIT_TEST)
  #include <final/ftermlinux.h>
  #include <final/ftermfreebsd.h>
  #include <final/ftermopenbsd.h>
#elif defined(__linux__)
  #include <final/ftermlinux.h>
#elif defined(__FreeBSD__) || defined(__DragonFly__)
  #include <final/ftermfreebsd.h>
#elif defined(__NetBSD__) || defined(__OpenBSD__)
  #include <final/ftermopenbsd.h>
#endif

#undef USE_FINAL_H

#endif  // FINAL_H
