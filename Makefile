NAME     = Octomino's SDL Input
VERSION  = 0.1.0-dev
REPO     = https://github.com/clickdevin/octomino-sdl-input
BIN      = octomino-sdl-input.dll

SRC      = $(wildcard src/*.c)

DBFILE   = gamecontrollerdb.txt
DBURL    = https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt

ZIPNAME  = $(BIN:.dll=)-$(VERSION).zip
ZIPFILES = $(BIN) $(DBFILE) LICENSE README.md sources.zip
ZIPSRC   = $(wildcard src/*.c) $(wildcard src/*.h) Makefile

CC       = i686-w64-mingw32-gcc
CFLAGS   = -O2 -flto -fvisibility=hidden \
           -Wall -Wextra -Wpedantic -Wno-unused-parameter \
           -DPLUGIN_NAME=\""$(NAME)"\" \
           -DPLUGIN_VERSION=\""$(VERSION)"\" \
           -DPLUGIN_REPO=\""$(REPO)"\"
LDFLAGS  = -shared -static-libgcc -static \
           -lshlwapi `sdl2-config --static-libs`

$(BIN): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

$(DBFILE):
	wget $(DBURL)

$(ZIPNAME): $(ZIPFILES)
	rm -f $@
	zip $@ $^

sources.zip: $(ZIPSRC)
	zip $@ $^

.PHONY: all clean

all: $(BIN) $(DBFILE) $(ZIPNAME)

clean:
	rm -f $(BIN) $(DBFILE) $(ZIPNAME) sources.zip $(SRC:.c=.o)
