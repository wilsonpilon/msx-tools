# The FINAL CUT newfont
## *A graphical text font for X11 and the Linux console*


### How can I use the new graphical font under the Linux text console?

Simply start your program with the parameter `--newfont`.

### How can I use the new graphical font under X11?

The use of the new graphical font under X11 is only possible with
the terminal emulators 
[xterm](http://invisible-island.net/xterm/xterm.html)
or 
[urxvt](http://software.schmorp.de/pkg/rxvt-unicode.html),
because only these terminal emulators can change the font by
using an escape sequence.  For an xterm, the "[Allow Font Ops](../doc/xterm.txt)"
option must be set.

#### Install the gzip compressed X11 pcf bitmap font 8x16graph.pcf.gz on your xserver:

1. Create as user root the font directory<br />
`mkdir /usr/share/fonts/X11/misc/finalcut/`

2. Copy the files 8x16graph.pcf.gz, fonts.dir and fonts.alias
into the just created directory<br />
`cp 8x16graph.pcf.gz fonts.dir fonts.alias /usr/share/fonts/X11/misc/finalcut/`

3. Add the new dir to X font path<br />
`xset fp+ /usr/share/fonts/X11/misc/finalcut/`<br />
*(You can insert this command into the ~/.xinitrc to make the font path permanently usable)*

4. Check if the font is available to the xserver<br />
`xlsfonts | grep 8x16graph`

5. Start your program with the parameter `--newfont`<br />
`ui --newfont`

