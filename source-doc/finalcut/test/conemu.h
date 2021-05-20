/***********************************************************************
* conemu.h - Emulator for various consoles and terminals               *
*                                                                      *
* This file is part of the FINAL CUT widget toolkit                    *
*                                                                      *
* Copyright 2019-2021 Markus Gans                                      *
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

/*  Standalone test class
 *  ═════════════════════
 *
 * ▕▔▔▔▔▔▔▔▔▏
 * ▕ ConEmu ▏
 * ▕▁▁▁▁▁▁▁▁▏
 */

#ifndef CONEMU_H
#define CONEMU_H

#include <sys/wait.h>
#include <sys/mman.h>

#include <final/final.h>

namespace test
{

//----------------------------------------------------------------------
// class ConEmu
//----------------------------------------------------------------------

class ConEmu
{
  public:
    // Enumeration
    enum class console
    {
      ansi,
      xterm,
      rxvt,
      urxvt,
      kde_konsole,
      gnome_terminal,
      newer_vte_terminal,
      putty,
      win_terminal,
      tera_term,
      cygwin,
      mintty,
      linux_con,
      freebsd_con,
      netbsd_con,
      openbsd_con,
      sun_con,
      screen,
      tmux,
      kterm,
      mlterm,
      kitty
    };

    // Constructors
    ConEmu()
    {
      // Map shared memory
      void* ptr = mmap ( NULL
                       , sizeof(*shared_state)
                       , PROT_READ | PROT_WRITE
                       , MAP_SHARED | MAP_ANONYMOUS, -1
                       , 0 );

      if ( ptr == MAP_FAILED )
      {
        std::cerr << "mmap error: "
                  << strerror(errno)
                  << " (" << errno << ")"
                  << std::endl;
        return;
      }

      shared_state = static_cast<bool*>(ptr);
      *shared_state = false;
    }

    // Disable copy constructor
    ConEmu (const ConEmu&) = delete;

    // Destructor
    ~ConEmu()
    {
      closeMasterPTY();
      closeSlavePTY();
      // Unmap shared memory
      munmap (shared_state, sizeof(*shared_state));
    }

    // Disable assignment operator (=)
    ConEmu& operator = (const ConEmu&) = delete;

  protected:
    // Mutators
    void        enableConEmuDebug (bool);

    // Inquiries
    bool        isConEmuChildProcess (pid_t);

    // Methods
    void        printConEmuDebug();
    void        closeConEmuStdStreams();
    pid_t       forkConEmu();
    void        startConEmuTerminal (console);

  private:
    // Accessors
    const char* getAnswerback (console);
    const char* getDSR (console);
    const char* getDECID (console);
    const char* getDA (console);
    const char* getDA1 (console);
    const char* getSEC_DA (console);

    // Methods
    bool        openMasterPTY();
    bool        openSlavePTY();
    void        closeMasterPTY();
    void        closeSlavePTY();
    void        parseTerminalBuffer (std::size_t, console);

    // Data members
    int                fd_stdin{fileno(stdin)};
    int                fd_stdout{fileno(stdout)};
    int                fd_stderr{fileno(stderr)};
    int                fd_master{-1};
    int                fd_slave{-1};
    bool               debug{false};
    char               buffer[2048]{};
    static bool*       shared_state;
    static const char* colorname[];
};

// static class attributes
bool* ConEmu::shared_state = nullptr;


// private data member of ConEmu
//----------------------------------------------------------------------
const char* ConEmu::colorname[] =
{
  C_STR("0000/0000/0000"),  // 0
  C_STR("bbbb/0000/0000"),  // 1
  C_STR("0000/bbbb/0000"),  // 2
  C_STR("bbbb/bbbb/0000"),  // 3
  C_STR("0000/0000/bbbb"),  // 4
  C_STR("bbbb/0000/bbbb"),  // 5
  C_STR("0000/bbbb/bbbb"),  // 6
  C_STR("bbbb/bbbb/bbbb"),  // 7
  C_STR("5555/5555/5555"),  // 8
  C_STR("ffff/5555/5555"),  // 9
  C_STR("5555/ffff/5555"),  // 10
  C_STR("ffff/ffff/5555"),  // 11
  C_STR("5555/5555/ffff"),  // 12
  C_STR("ffff/5555/ffff"),  // 13
  C_STR("5555/ffff/ffff"),  // 14
  C_STR("ffff/ffff/ffff"),  // 15
  C_STR("0000/0000/0000"),  // 16
  C_STR("0000/0000/5f5f"),  // 17
  C_STR("0000/0000/8787"),  // 18
  C_STR("0000/0000/afaf"),  // 19
  C_STR("0000/0000/d7d7"),  // 20
  C_STR("0000/0000/ffff"),  // 21
  C_STR("0000/5f5f/0000"),  // 22
  C_STR("0000/5f5f/5f5f"),  // 23
  C_STR("0000/5f5f/8787"),  // 24
  C_STR("0000/5f5f/afaf"),  // 25
  C_STR("0000/5f5f/d7d7"),  // 26
  C_STR("0000/5f5f/ffff"),  // 27
  C_STR("0000/8787/0000"),  // 28
  C_STR("0000/8787/5f5f"),  // 29
  C_STR("0000/8787/8787"),  // 30
  C_STR("0000/8787/afaf"),  // 31
  C_STR("0000/8787/d7d7"),  // 32
  C_STR("0000/8787/ffff"),  // 33
  C_STR("0000/afaf/0000"),  // 34
  C_STR("0000/afaf/5f5f"),  // 35
  C_STR("0000/afaf/8787"),  // 36
  C_STR("0000/afaf/afaf"),  // 37
  C_STR("0000/afaf/d7d7"),  // 38
  C_STR("0000/afaf/ffff"),  // 39
  C_STR("0000/d7d7/0000"),  // 40
  C_STR("0000/d7d7/5f5f"),  // 41
  C_STR("0000/d7d7/8787"),  // 42
  C_STR("0000/d7d7/afaf"),  // 43
  C_STR("0000/d7d7/d7d7"),  // 44
  C_STR("0000/d7d7/ffff"),  // 45
  C_STR("0000/ffff/0000"),  // 46
  C_STR("0000/ffff/5f5f"),  // 47
  C_STR("0000/ffff/8787"),  // 48
  C_STR("0000/ffff/afaf"),  // 49
  C_STR("0000/ffff/d7d7"),  // 50
  C_STR("0000/ffff/ffff"),  // 51
  C_STR("5f5f/0000/0000"),  // 52
  C_STR("5f5f/0000/5f5f"),  // 53
  C_STR("5f5f/0000/8787"),  // 54
  C_STR("5f5f/0000/afaf"),  // 55
  C_STR("5f5f/0000/d7d7"),  // 56
  C_STR("5f5f/0000/ffff"),  // 57
  C_STR("5f5f/5f5f/0000"),  // 58
  C_STR("5f5f/5f5f/5f5f"),  // 59
  C_STR("5f5f/5f5f/8787"),  // 60
  C_STR("5f5f/5f5f/afaf"),  // 61
  C_STR("5f5f/5f5f/d7d7"),  // 62
  C_STR("5f5f/5f5f/ffff"),  // 63
  C_STR("5f5f/8787/0000"),  // 64
  C_STR("5f5f/8787/5f5f"),  // 65
  C_STR("5f5f/8787/8787"),  // 66
  C_STR("5f5f/8787/afaf"),  // 67
  C_STR("5f5f/8787/d7d7"),  // 68
  C_STR("5f5f/8787/ffff"),  // 69
  C_STR("5f5f/afaf/0000"),  // 70
  C_STR("5f5f/afaf/5f5f"),  // 71
  C_STR("5f5f/afaf/8787"),  // 72
  C_STR("5f5f/afaf/afaf"),  // 73
  C_STR("5f5f/afaf/d7d7"),  // 74
  C_STR("5f5f/afaf/ffff"),  // 75
  C_STR("5f5f/d7d7/0000"),  // 76
  C_STR("5f5f/d7d7/5f5f"),  // 77
  C_STR("5f5f/d7d7/8787"),  // 78
  C_STR("5f5f/d7d7/afaf"),  // 79
  C_STR("5f5f/d7d7/d7d7"),  // 80
  C_STR("5f5f/d7d7/ffff"),  // 81
  C_STR("5f5f/ffff/0000"),  // 82
  C_STR("5f5f/ffff/5f5f"),  // 83
  C_STR("5f5f/ffff/8787"),  // 84
  C_STR("5f5f/ffff/afaf"),  // 85
  C_STR("5f5f/ffff/d7d7"),  // 86
  C_STR("5f5f/ffff/ffff"),  // 87
  C_STR("8787/0000/0000"),  // 88
  C_STR("8787/0000/5f5f"),  // 89
  C_STR("8787/0000/8787"),  // 90
  C_STR("8787/0000/afaf"),  // 91
  C_STR("8787/0000/d7d7"),  // 92
  C_STR("8787/0000/ffff"),  // 93
  C_STR("8787/5f5f/0000"),  // 94
  C_STR("8787/5f5f/5f5f"),  // 95
  C_STR("8787/5f5f/8787"),  // 96
  C_STR("8787/5f5f/afaf"),  // 97
  C_STR("8787/5f5f/d7d7"),  // 98
  C_STR("8787/5f5f/ffff"),  // 99
  C_STR("8787/8787/0000"),  // 100
  C_STR("8787/8787/5f5f"),  // 101
  C_STR("8787/8787/8787"),  // 102
  C_STR("8787/8787/afaf"),  // 103
  C_STR("8787/8787/d7d7"),  // 104
  C_STR("8787/8787/ffff"),  // 105
  C_STR("8787/afaf/0000"),  // 106
  C_STR("8787/afaf/5f5f"),  // 107
  C_STR("8787/afaf/8787"),  // 108
  C_STR("8787/afaf/afaf"),  // 109
  C_STR("8787/afaf/d7d7"),  // 110
  C_STR("8787/afaf/ffff"),  // 111
  C_STR("8787/d7d7/0000"),  // 112
  C_STR("8787/d7d7/5f5f"),  // 113
  C_STR("8787/d7d7/8787"),  // 114
  C_STR("8787/d7d7/afaf"),  // 115
  C_STR("8787/d7d7/d7d7"),  // 116
  C_STR("8787/d7d7/ffff"),  // 117
  C_STR("8787/ffff/0000"),  // 118
  C_STR("8787/ffff/5f5f"),  // 119
  C_STR("8787/ffff/8787"),  // 120
  C_STR("8787/ffff/afaf"),  // 121
  C_STR("8787/ffff/d7d7"),  // 122
  C_STR("8787/ffff/ffff"),  // 123
  C_STR("afaf/0000/0000"),  // 124
  C_STR("afaf/0000/5f5f"),  // 125
  C_STR("afaf/0000/8787"),  // 126
  C_STR("afaf/0000/afaf"),  // 127
  C_STR("afaf/0000/d7d7"),  // 128
  C_STR("afaf/0000/ffff"),  // 129
  C_STR("afaf/5f5f/0000"),  // 130
  C_STR("afaf/5f5f/5f5f"),  // 131
  C_STR("afaf/5f5f/8787"),  // 132
  C_STR("afaf/5f5f/afaf"),  // 133
  C_STR("afaf/5f5f/d7d7"),  // 134
  C_STR("afaf/5f5f/ffff"),  // 135
  C_STR("afaf/8787/0000"),  // 136
  C_STR("afaf/8787/5f5f"),  // 137
  C_STR("afaf/8787/8787"),  // 138
  C_STR("afaf/8787/afaf"),  // 139
  C_STR("afaf/8787/d7d7"),  // 140
  C_STR("afaf/8787/ffff"),  // 141
  C_STR("afaf/afaf/0000"),  // 142
  C_STR("afaf/afaf/5f5f"),  // 143
  C_STR("afaf/afaf/8787"),  // 144
  C_STR("afaf/afaf/afaf"),  // 145
  C_STR("afaf/afaf/d7d7"),  // 146
  C_STR("afaf/afaf/ffff"),  // 147
  C_STR("afaf/d7d7/0000"),  // 148
  C_STR("afaf/d7d7/5f5f"),  // 149
  C_STR("afaf/d7d7/8787"),  // 150
  C_STR("afaf/d7d7/afaf"),  // 151
  C_STR("afaf/d7d7/d7d7"),  // 152
  C_STR("afaf/d7d7/ffff"),  // 153
  C_STR("afaf/ffff/0000"),  // 154
  C_STR("afaf/ffff/5f5f"),  // 155
  C_STR("afaf/ffff/8787"),  // 156
  C_STR("afaf/ffff/afaf"),  // 157
  C_STR("afaf/ffff/d7d7"),  // 158
  C_STR("afaf/ffff/ffff"),  // 159
  C_STR("d7d7/0000/0000"),  // 160
  C_STR("d7d7/0000/5f5f"),  // 161
  C_STR("d7d7/0000/8787"),  // 162
  C_STR("d7d7/0000/afaf"),  // 163
  C_STR("d7d7/0000/d7d7"),  // 164
  C_STR("d7d7/0000/ffff"),  // 165
  C_STR("d7d7/5f5f/0000"),  // 166
  C_STR("d7d7/5f5f/5f5f"),  // 167
  C_STR("d7d7/5f5f/8787"),  // 168
  C_STR("d7d7/5f5f/afaf"),  // 169
  C_STR("d7d7/5f5f/d7d7"),  // 170
  C_STR("d7d7/5f5f/ffff"),  // 171
  C_STR("d7d7/8787/0000"),  // 172
  C_STR("d7d7/8787/5f5f"),  // 173
  C_STR("d7d7/8787/8787"),  // 174
  C_STR("d7d7/8787/afaf"),  // 175
  C_STR("d7d7/8787/d7d7"),  // 176
  C_STR("d7d7/8787/ffff"),  // 177
  C_STR("d7d7/afaf/0000"),  // 178
  C_STR("d7d7/afaf/5f5f"),  // 179
  C_STR("d7d7/afaf/8787"),  // 180
  C_STR("d7d7/afaf/afaf"),  // 181
  C_STR("d7d7/afaf/d7d7"),  // 182
  C_STR("d7d7/afaf/ffff"),  // 183
  C_STR("d7d7/d7d7/0000"),  // 184
  C_STR("d7d7/d7d7/5f5f"),  // 185
  C_STR("d7d7/d7d7/8787"),  // 186
  C_STR("d7d7/d7d7/afaf"),  // 187
  C_STR("d7d7/d7d7/d7d7"),  // 188
  C_STR("d7d7/d7d7/ffff"),  // 189
  C_STR("d7d7/ffff/0000"),  // 190
  C_STR("d7d7/ffff/5f5f"),  // 191
  C_STR("d7d7/ffff/8787"),  // 192
  C_STR("d7d7/ffff/afaf"),  // 193
  C_STR("d7d7/ffff/d7d7"),  // 194
  C_STR("d7d7/ffff/ffff"),  // 195
  C_STR("ffff/0000/0000"),  // 196
  C_STR("ffff/0000/5f5f"),  // 197
  C_STR("ffff/0000/8787"),  // 198
  C_STR("ffff/0000/afaf"),  // 199
  C_STR("ffff/0000/d7d7"),  // 200
  C_STR("ffff/0000/ffff"),  // 201
  C_STR("ffff/5f5f/0000"),  // 202
  C_STR("ffff/5f5f/5f5f"),  // 203
  C_STR("ffff/5f5f/8787"),  // 204
  C_STR("ffff/5f5f/afaf"),  // 205
  C_STR("ffff/5f5f/d7d7"),  // 206
  C_STR("ffff/5f5f/ffff"),  // 207
  C_STR("ffff/8787/0000"),  // 208
  C_STR("ffff/8787/5f5f"),  // 209
  C_STR("ffff/8787/8787"),  // 210
  C_STR("ffff/8787/afaf"),  // 211
  C_STR("ffff/8787/d7d7"),  // 212
  C_STR("ffff/8787/ffff"),  // 213
  C_STR("ffff/afaf/0000"),  // 214
  C_STR("ffff/afaf/5f5f"),  // 215
  C_STR("ffff/afaf/8787"),  // 216
  C_STR("ffff/afaf/afaf"),  // 217
  C_STR("ffff/afaf/d7d7"),  // 218
  C_STR("ffff/afaf/ffff"),  // 219
  C_STR("ffff/d7d7/0000"),  // 220
  C_STR("ffff/d7d7/5f5f"),  // 221
  C_STR("ffff/d7d7/8787"),  // 222
  C_STR("ffff/d7d7/afaf"),  // 223
  C_STR("ffff/d7d7/d7d7"),  // 224
  C_STR("ffff/d7d7/ffff"),  // 225
  C_STR("ffff/ffff/0000"),  // 226
  C_STR("ffff/ffff/5f5f"),  // 227
  C_STR("ffff/ffff/8787"),  // 228
  C_STR("ffff/ffff/afaf"),  // 229
  C_STR("ffff/ffff/d7d7"),  // 230
  C_STR("ffff/ffff/ffff"),  // 231
  C_STR("0808/0808/0808"),  // 232
  C_STR("1212/1212/1212"),  // 233
  C_STR("1c1c/1c1c/1c1c"),  // 234
  C_STR("2626/2626/2626"),  // 235
  C_STR("3030/3030/3030"),  // 236
  C_STR("3a3a/3a3a/3a3a"),  // 237
  C_STR("4444/4444/4444"),  // 238
  C_STR("4e4e/4e4e/4e4e"),  // 239
  C_STR("5858/5858/5858"),  // 240
  C_STR("6262/6262/6262"),  // 241
  C_STR("6c6c/6c6c/6c6c"),  // 242
  C_STR("7676/7676/7676"),  // 243
  C_STR("8080/8080/8080"),  // 244
  C_STR("8a8a/8a8a/8a8a"),  // 245
  C_STR("9494/9494/9494"),  // 246
  C_STR("9e9e/9e9e/9e9e"),  // 247
  C_STR("a8a8/a8a8/a8a8"),  // 248
  C_STR("b2b2/b2b2/b2b2"),  // 249
  C_STR("bcbc/bcbc/bcbc"),  // 250
  C_STR("c6c6/c6c6/c6c6"),  // 251
  C_STR("d0d0/d0d0/d0d0"),  // 252
  C_STR("dada/dada/dada"),  // 253
  C_STR("e4e4/e4e4/e4e4"),  // 254
  C_STR("eeee/eeee/eeee"),  // 255
  0
};


// ConEmu inline functions

// protected methods of ConEmu
//----------------------------------------------------------------------
inline void ConEmu::enableConEmuDebug (bool enable)
{
  debug = enable;
}

//----------------------------------------------------------------------
inline bool ConEmu::isConEmuChildProcess (pid_t pid)
{
  return bool( pid == 0 );
}

//----------------------------------------------------------------------
inline void ConEmu::printConEmuDebug()
{
  // Printing terminal debug information for some escape sequences

  if ( ! debug )
    return;

  setenv ("DSR",        "\\033[5n", 1);
  setenv ("CURSOR_POS", "\\033[6n", 1);
  setenv ("DECID",      "\\033Z", 1);
  setenv ("DA",         "\\033[c", 1);
  setenv ("DA1",        "\\033[1c", 1);
  setenv ("SEC_DA",     "\\033[>c", 1);
  setenv ("ANSWERBACK", "\\005", 1);
  setenv ("TITLE",      "\\033[21t", 1);
  setenv ("COLOR16",    "\\033]4;15;?\\a", 1);
  setenv ("COLOR88",    "\\033]4;87;?\\a", 1);
  setenv ("COLOR256",   "\\033]4;254;?\\a", 1);

  setenv ("GO_MIDDLE",  "\\033[80D\\033[15C", 1);
  setenv ("GO_RIGHT",   "\\033[79D\\033[40C", 1);

  finalcut::FString line (69, '-');
  std::cout << std::endl << line << std::endl;
  std::cout << "Probe           Escape sequence          Reply";
  std::cout << std::endl << line << std::endl;

  // Command line
  constexpr char debug_command[] = "/bin/bash -c ' \
      for i in DSR CURSOR_POS DECID DA DA1 SEC_DA ANSWERBACK \
               TITLE COLOR16 COLOR88 COLOR256; \
      do \
        eval \"echo -en \\\"$i${GO_MIDDLE}\\\"; \
              echo -n \\\"\\${$i}\\\"; \
              echo -en \\\"${GO_RIGHT}\\${$i}\\\"\"; \
        sleep 0.5; \
        echo -e \"\\r\"; \
      done'";
  system(debug_command);
}

//----------------------------------------------------------------------
inline void ConEmu::closeConEmuStdStreams()
{
  close(fd_stdin);   // stdin
  close(fd_stdout);  // stdout
  close(fd_stderr);  // stderr
}

//----------------------------------------------------------------------
inline pid_t ConEmu::forkConEmu()
{
  // Initialize buffer with '\0'
  std::fill_n (buffer, sizeof(buffer), '\0');

  if ( ! openMasterPTY() )
    return -1;

  if ( ! openSlavePTY() )
    return -1;

  pid_t pid = fork();  // Create a child process

  if ( pid < 0)  // Fork failed
    return -1;

  if ( isConEmuChildProcess(pid) )  // Child process
  {
    struct termios term_settings;
    closeMasterPTY();

    // Creates a session and makes the current process to the leader
    setsid();

#ifdef TIOCSCTTY
    // Set controlling tty
    if ( ioctl(fd_slave, TIOCSCTTY, 0) == -1 )
    {
      *shared_state = true;
      return -1;
    }
#endif

    // Get current terminal settings
    if ( tcgetattr(fd_slave, &term_settings) == -1 )
    {
      *shared_state = true;
      return -1;
    }

    // Set raw mode on the slave side of the PTY
    cfmakeraw (&term_settings);
    tcsetattr (fd_slave, TCSANOW, &term_settings);

#ifdef TIOCSWINSZ
    // Set slave tty window size
    struct winsize size{};
    size.ws_row = 25;
    size.ws_col = 80;

    if ( ioctl(fd_slave, TIOCSWINSZ, &size) == -1)
    {
      *shared_state = true;
      return -1;
    }
#endif

    closeConEmuStdStreams();

    fd_stdin  = dup(fd_slave);  // PTY becomes stdin  (0)
    fd_stdout = dup(fd_slave);  // PTY becomes stdout (1)
    fd_stderr = dup(fd_slave);  // PTY becomes stderr (2)

    closeSlavePTY();

    // The child process is now ready for input
    *shared_state = true;
  }
  else
  {
    constexpr int timeout = 150; // 1.5 seconds
    int i = 0;

    // Wait until the child process is ready for input
    while ( ! *shared_state && i < timeout )
    {
      // Wait 10 ms (= 10,000,000 ns)
      const struct timespec ms[]{{0, 10000000L}};
      nanosleep (ms, NULL);
      i++;
    }

    *shared_state = false;
  }

  return pid;
}



//----------------------------------------------------------------------
inline void ConEmu::startConEmuTerminal (console con)
{
  closeSlavePTY();

  while ( 1 )
  {
    fd_set ifds;
    struct timeval tv;
    ssize_t len;

    FD_ZERO(&ifds);
    FD_SET(fd_stdin, &ifds);
    FD_SET(fd_master, &ifds);
    tv.tv_sec  = 0;
    tv.tv_usec = 750000;  // 750 ms

    // Wait for data from stdin or the master side of PTY
    if ( select(fd_master + 1, &ifds, 0, 0, &tv) < 0 )
      break;

    // Data on standard input
    if ( FD_ISSET(fd_stdin, &ifds) )
    {
      len = read (fd_stdin, buffer, sizeof(buffer));

      if ( len > 0 && std::size_t(len) < sizeof(buffer) )
      {
        buffer[len] = '\0';
        write (fd_master, buffer, len);  // Send data to the master side
      }
    }

    // Data on the master side of PTY
    if ( FD_ISSET(fd_master, &ifds) )
    {
      len = read (fd_master, buffer, sizeof(buffer));

      if ( len == -1 || std::size_t(len) >= sizeof(buffer) )
        break;
      else if ( len > 0 )
      {
        buffer[len] = '\0';
        parseTerminalBuffer (len, con);
      }
    }
  }
}


// private methods of ConEmu
//----------------------------------------------------------------------
inline const char* ConEmu::getAnswerback (console con)
{
  static const char* Answerback[] =
  {
    0,               // Ansi,
    0,               // XTerm
    0,               // Rxvt
    0,               // Urxvt
    0,               // KDE Konsole
    0,               // GNOME Terminal
    0,               // VTE Terminal >= 0.53.0
    C_STR("PuTTY"),  // PuTTY
    0,               // Windows Terminal
    0,               // Tera Term
    0,               // Cygwin
    0,               // Mintty
    0,               // Linux console
    0,               // FreeBSD console
    0,               // NetBSD console
    0,               // OpenBSD console
    0,               // Sun console
    0,               // screen
    0,               // tmux
    0,               // kterm,
    0,               // mlterm - Multi Lingual TERMinal
    0                // kitty
  };

  return Answerback[static_cast<std::size_t>(con)];
}

//----------------------------------------------------------------------
inline const char* ConEmu::getDSR (console con)
{
  static const char* DSR[] =
  {
    0,                 // Ansi,
    C_STR("\033[0n"),  // XTerm
    C_STR("\033[0n"),  // Rxvt
    C_STR("\033[0n"),  // Urxvt
    C_STR("\033[0n"),  // KDE Konsole
    C_STR("\033[0n"),  // GNOME Terminal
    C_STR("\033[0n"),  // VTE Terminal >= 0.53.0
    C_STR("\033[0n"),  // PuTTY
    C_STR("\033[0n"),  // Windows Terminal >= 1.2
    C_STR("\033[0n"),  // Tera Term
    0,                 // Cygwin
    C_STR("\033[0n"),  // Mintty
    C_STR("\033[0n"),  // Linux console
    C_STR("\033[0n"),  // FreeBSD console
    C_STR("\033[0n"),  // NetBSD console
    C_STR("\033[0n"),  // OpenBSD console
    0,                 // Sun console
    C_STR("\033[0n"),  // screen
    C_STR("\033[0n"),  // tmux
    C_STR("\033[0n"),  // kterm
    C_STR("\033[0n"),  // mlterm - Multi Lingual TERMinal
    C_STR("\033[0n")   // kitty
  };

  return DSR[static_cast<std::size_t>(con)];
}

//----------------------------------------------------------------------
inline const char* ConEmu::getDECID (console con)
{
  static const char* DECID[] =
  {
    0,                                     // Ansi,
    C_STR("\033[?63;1;2;6;4;6;9;15;22c"),  // XTerm
    C_STR("\033[?1;2c"),                   // Rxvt
    C_STR("\033[?1;2c"),                   // Urxvt
    C_STR("\033[?1;2c"),                   // KDE Konsole
    C_STR("\033[?62;c"),                   // GNOME Terminal
    C_STR("\033[?65;1;9c"),                // VTE Terminal >= 0.53.0
    C_STR("\033[?6c"),                     // PuTTY
    0,                                     // Windows Terminal
    C_STR("\033[?1;2c"),                   // Tera Term
    0,                                     // Cygwin
    C_STR("\033[?1;2;6;22c"),              // Mintty
    C_STR("\033[?6c"),                     // Linux console
    0,                                     // FreeBSD console
    0,                                     // NetBSD console
    0,                                     // OpenBSD console
    0,                                     // Sun console
    C_STR("\033[?1;2c"),                   // screen
    0,                                     // tmux
    C_STR("\033[?1;2c"),                   // kterm
    C_STR("\033[?63;1;2;3;4;7;29c"),       // mlterm - Multi Lingual TERMinal
    0                                      // kitty
  };

  return DECID[static_cast<std::size_t>(con)];
}

//----------------------------------------------------------------------
inline const char* ConEmu::getDA (console con)
{
  static const char* DA[] =
  {
    0,                                     // Ansi,
    C_STR("\033[?63;1;2;6;4;6;9;15;22c"),  // XTerm
    C_STR("\033[?1;2c"),                   // Rxvt
    C_STR("\033[?1;2c"),                   // Urxvt
    C_STR("\033[?1;2c"),                   // KDE Konsole
    C_STR("\033[?62;c"),                   // GNOME Terminal
    C_STR("\033[?65;1;9c"),                // VTE Terminal >= 0.53.0
    C_STR("\033[?6c"),                     // PuTTY
    C_STR("\033[?1;0c"),                   // Windows Terminal >= 1.2
    C_STR("\033[?1;2c"),                   // Tera Term
    C_STR("\033[?6c"),                     // Cygwin
    C_STR("\033[?1;2;6;22c"),              // Mintty
    C_STR("\033[?6c"),                     // Linux console
    C_STR("\033[?1;2c"),                   // FreeBSD console
    C_STR("\033[?62;6c"),                  // NetBSD console
    C_STR("\033[?62;6c"),                  // OpenBSD console
    0,                                     // Sun console
    C_STR("\033[?1;2c"),                   // screen
    C_STR("\033[?1;2c"),                   // tmux
    C_STR("\033[?1;2c"),                   // kterm
    C_STR("\033[?63;1;2;3;4;7;29c"),       // mlterm - Multi Lingual TERMinal
    C_STR("\033[?62;c")                    // kitty
  };

  return DA[static_cast<std::size_t>(con)];
}

//----------------------------------------------------------------------
inline const char* ConEmu::getDA1 (console con)
{
  static const char* DA1[] =
  {
    0,                                // Ansi,
    0,                                // XTerm
    C_STR("\033[?1;2c"),              // Rxvt
    C_STR("\033[?1;2c"),              // Urxvt
    C_STR("\033[?1;2c"),              // KDE Konsole
    C_STR("\033[?62;c"),              // GNOME Terminal
    C_STR("\033[?65;1;9c"),           // VTE Terminal >= 0.53.0
    C_STR("\033[?6c"),                // PuTTY
    0,                                // Windows Terminal
    C_STR("\033[?1;2c"),              // Tera Term
    C_STR("\033[?6c"),                // Cygwin
    C_STR("\033[?1;2;6;22c"),         // Mintty
    0,                                // Linux console
    0,                                // FreeBSD console
    0,                                // NetBSD console
    0,                                // OpenBSD console
    0,                                // Sun console
    0,                                // screen
    0,                                // tmux
    0,                                // kterm
    C_STR("\033[?63;1;2;3;4;7;29c"),  // mlterm - Multi Lingual TERMinal
    0                                 // kitty
  };

  return DA1[static_cast<std::size_t>(con)];
}

//----------------------------------------------------------------------
inline const char* ConEmu::getSEC_DA (console con)
{
  static const char* SEC_DA[] =
  {
    0,                            // Ansi,
    C_STR("\033[>19;312;0c"),     // XTerm
    C_STR("\033[>82;20710;0c"),   // Rxvt
    C_STR("\033[>85;95;0c"),      // Urxvt
    C_STR("\033[>0;115;0c"),      // KDE Konsole
    C_STR("\033[>1;5202;0c"),     // GNOME Terminal
    C_STR("\033[>65;5300;1c"),    // VTE Terminal >= 0.53.0
    C_STR("\033[>0;136;0c"),      // PuTTY
    C_STR("\033[>0;10;1c"),       // Windows Terminal >= 1.2
    C_STR("\033[>32;278;0c"),     // Tera Term
    C_STR("\033[>67;200502;0c"),  // Cygwin
    C_STR("\033[>77;20402;0c"),   // Mintty
    0,                            // Linux console
    C_STR("\033[>0;10;0c"),       // FreeBSD console
    C_STR("\033[>24;20;0c"),      // NetBSD console
    C_STR("\033[>24;20;0c"),      // OpenBSD console
    0,                            // Sun console
    C_STR("\033[>83;40201;0c"),   // screen
    C_STR("\033[>84;0;0c"),       // tmux
    C_STR("\033[?1;2c"),          // kterm
    C_STR("\033[>24;279;0c"),     // mlterm - Multi Lingual TERMinal
    C_STR("\033[>1;4000;13c")     // kitty
  };

  return SEC_DA[static_cast<std::size_t>(con)];
}

//----------------------------------------------------------------------
inline bool ConEmu::openMasterPTY()
{
  int result;

  // Open a pseudoterminal device
  fd_master = posix_openpt(O_RDWR);

  if ( fd_master < 0 )
    return false;

  // Change the slave pseudoterminal access rights
  result = grantpt(fd_master);

  if ( result != 0 )
    return false;

  // Unlock the pseudoterminal master/slave pair
  result = unlockpt(fd_master);

  if ( result != 0 )
    return false;

  return true;
}

//----------------------------------------------------------------------
inline bool ConEmu::openSlavePTY()
{
  closeSlavePTY();

  // Get PTY filename
  const char* pty_name = ptsname(fd_master);

  if ( pty_name == 0 )
    return false;

  // Open the slave PTY
  fd_slave = open(pty_name, O_RDWR);

  if ( fd_slave < 0 )
    return false;

  return true;
}

//----------------------------------------------------------------------
inline void ConEmu::closeMasterPTY()
{
  if ( fd_master <= 0 )
    return;

  close (fd_master);
  fd_master = -1;
}

//----------------------------------------------------------------------
inline void ConEmu::closeSlavePTY()
{
  if ( fd_slave <= 0 )
    return;

  close (fd_slave);
  fd_slave = -1;
}

//----------------------------------------------------------------------
inline void ConEmu::parseTerminalBuffer (std::size_t length, console con)
{
  for (std::size_t i = 0; i < length; i++)
  {
    if ( buffer[i] == ENQ[0] )  // Enquiry character
    {
      const char* answer = getAnswerback(con);

      if ( answer )
        write(fd_master, answer, std::strlen(answer));
    }
    else if ( i < length - 1  // Terminal ID (DECID)
           && buffer[i] == '\033'
           && buffer[i + 1] == 'Z' )
    {
      const char* DECID = getDECID(con);

      if ( DECID )
        write (fd_master, DECID, std::strlen(DECID));

      i += 2;
    }
    else if ( i < length - 3  // Device status report (DSR)
           && buffer[i] == '\033'
           && buffer[i + 1] == '['
           && buffer[i + 2] == '5'
           && buffer[i + 3] == 'n' )
    {
      const char* DSR = getDSR(con);

      if ( DSR )
        write (fd_master, DSR, std::strlen(DSR));

      i += 4;
    }
    else if ( i < length - 3  // Report cursor position (CPR)
           && buffer[i] == '\033'
           && buffer[i + 1] == '['
           && buffer[i + 2] == '6'
           && buffer[i + 3] == 'n' )
    {
      write (fd_master, "\033[25;80R", 8);  // row 25 ; column 80
      i += 4;
    }
    else if ( i < length - 2  // Device attributes (DA)
           && buffer[i] == '\033'
           && buffer[i + 1] == '['
           && buffer[i + 2] == 'c' )
    {
      const char* DA = getDA(con);

      if ( DA )
        write (fd_master, DA, std::strlen(DA));

      i += 3;
    }
    else if ( i < length - 3  // Device attributes (DA1)
           && buffer[i] == '\033'
           && buffer[i + 1] == '['
           && buffer[i + 2] == '1'
           && buffer[i + 3] == 'c' )
    {
      const char* DA1 = getDA1(con);

      if ( DA1 )
        write (fd_master, DA1, std::strlen(DA1));
      i += 4;
    }
    else if ( i < length - 3  // Secondary device attributes (SEC_DA)
           && buffer[i] == '\033'
           && buffer[i + 1] == '['
           && buffer[i + 2] == '>'
           && buffer[i + 3] == 'c' )
    {
      const char* SEC_DA = getSEC_DA(con);

      if ( SEC_DA )
        write (fd_master, SEC_DA, std::strlen(SEC_DA));

      i += 4;
    }
    else if ( i < length - 4  // Report xterm window's title
           && buffer[i] == '\033'
           && buffer[i + 1] == '['
           && buffer[i + 2] == '2'
           && buffer[i + 3] == '1'
           && buffer[i + 4] == 't' )
    {
      if ( con == console::urxvt )
        write (fd_master, "\033]l", 3);
      else if ( con == console::tera_term )
        write (fd_master, "\033]l\033\\", 5);
      else if ( con == console::screen )
        write (fd_master, "\033]lbash\033\\", 9);
      else if ( con != console::ansi
             && con != console::rxvt
             && con != console::kde_konsole
             && con != console::cygwin
             && con != console::win_terminal
             && con != console::mintty
             && con != console::linux_con
             && con != console::freebsd_con
             && con != console::netbsd_con
             && con != console::openbsd_con
             && con != console::sun_con
             && con != console::tmux
             && con != console::kterm
             && con != console::mlterm
             && con != console::kitty )
        write (fd_master, "\033]lTITLE\033\\", 10);

      i += 5;
    }
    else if ( i < length - 7  // Get xterm color name 0-9
           && buffer[i] == '\033'
           && buffer[i + 1] == ']'
           && buffer[i + 2] == '4'
           && buffer[i + 3] == ';'
           && buffer[i + 4] >= '0' && buffer[i + 4] <= '9'
           && buffer[i + 5] == ';'
           && buffer[i + 6] == '?'
           && buffer[i + 7] == '\a' )
    {
      if ( con != console::ansi
        && con != console::rxvt
        && con != console::kde_konsole
        && con != console::cygwin
        && con != console::win_terminal
        && con != console::mintty
        && con != console::linux_con
        && con != console::freebsd_con
        && con != console::netbsd_con
        && con != console::openbsd_con
        && con != console::sun_con
        && con != console::screen
        && con != console::tmux
        && con != console::kterm )
      {
        int n = buffer[i + 4] - '0';
        write (fd_master, "\033]4;", 4);
        write (fd_master, &buffer[i + 4], 1);
        write (fd_master, ";rgb:", 5);
        write (fd_master, colorname[n], 14);
        write (fd_master, "\a", 1);
      }

      i += 8;
    }
    else if ( i < length - 8  // Get xterm color name 0-9
           && buffer[i] == '\033'
           && buffer[i + 1] == ']'
           && buffer[i + 2] == '4'
           && buffer[i + 3] == ';'
           && buffer[i + 4] >= '0' && buffer[i + 4] <= '9'
           && buffer[i + 5] >= '0' && buffer[i + 5] <= '9'
           && buffer[i + 6] == ';'
           && buffer[i + 7] == '?'
           && buffer[i + 8] == '\a' )
    {
      if ( con != console::ansi
        && con != console::rxvt
        && con != console::kde_konsole
        && con != console::cygwin
        && con != console::win_terminal
        && con != console::mintty
        && con != console::linux_con
        && con != console::freebsd_con
        && con != console::netbsd_con
        && con != console::openbsd_con
        && con != console::sun_con
        && con != console::screen
        && con != console::tmux
        && con != console::kterm )
      {
        int n = (buffer[i + 4] - '0') * 10
              + (buffer[i + 5] - '0');
        write (fd_master, "\033]4;", 4);
        write (fd_master, &buffer[i + 4], 1);
        write (fd_master, &buffer[i + 5], 1);
        write (fd_master, ";rgb:", 5);
        write (fd_master, colorname[n], 14);
        write (fd_master, "\a", 1);
      }

      i += 9;
    }
    else if ( i < length - 9  // Get xterm color name 0-9
           && buffer[i] == '\033'
           && buffer[i + 1] == ']'
           && buffer[i + 2] == '4'
           && buffer[i + 3] == ';'
           && buffer[i + 4] >= '0' && buffer[i + 4] <= '9'
           && buffer[i + 5] >= '0' && buffer[i + 5] <= '9'
           && buffer[i + 6] >= '0' && buffer[i + 6] <= '9'
           && buffer[i + 7] == ';'
           && buffer[i + 8] == '?'
           && buffer[i + 9] == '\a' )
    {
      if ( con != console::ansi
        && con != console::rxvt
        && con != console::kde_konsole
        && con != console::cygwin
        && con != console::win_terminal
        && con != console::mintty
        && con != console::linux_con
        && con != console::freebsd_con
        && con != console::netbsd_con
        && con != console::openbsd_con
        && con != console::sun_con
        && con != console::screen
        && con != console::tmux
        && con != console::kterm )
      {
        int n = (buffer[i + 4] - '0') * 100
              + (buffer[i + 5] - '0') * 10
              + (buffer[i + 6] - '0');

        if ( n < 256 )
        {
          write (fd_master, "\033]4;", 4);
          write (fd_master, &buffer[i + 4], 1);
          write (fd_master, &buffer[i + 5], 1);
          write (fd_master, &buffer[i + 6], 1);
          write (fd_master, ";rgb:", 5);
          write (fd_master, colorname[n], 14);
          write (fd_master, "\a", 1);
        }
      }

      i += 10;
    }
    else
    {
      write (fd_stdout, &buffer[i], 1);  // Send data to stdout
    }
  }
}

}  // namespace test

#endif  // CONEMU_H
