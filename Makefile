NAME     = Octomino's SDL Input
VERSION  = 0.1.1-dev
REPO     = https://github.com/clickdevin/octomino-sdl-input
BIN      = octomino-sdl-input.dll

SRC      = $(wildcard src/*.c)

DBFILE   = gamecontrollerdb.txt
DBURL    = https://raw.githubusercontent.com/gabomdq/SDL_GameControllerDB/master/gamecontrollerdb.txt

ZIPNAME  = $(BIN:.dll=)-$(VERSION).zip
ZIPFILES = $(BIN) $(DBFILE) LICENSE README.md CHANGELOG.md sources.zip
ZIPSRC   = $(wildcard src/*.c) $(wildcard src/*.h) Makefile

CC       = i686-w64-mingw32-gcc
CFLAGS   = -std=c11 -O2 -MMD -flto -fvisibility=hidden \
           -Wall -Wextra -Wpedantic -Wshadow -Wno-unused-parameter \
           -DPLUGIN_NAME=\""$(NAME)"\" \
           -DPLUGIN_VERSION=\""$(VERSION)"\" \
           -DPLUGIN_REPO=\""$(REPO)"\"
LDFLAGS  = -shared -static-libgcc -static \
           -lshlwapi `sdl2-config --static-libs`

$(BIN): $(SRC:.c=.o)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

-include $(SRC:.c=.d)

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
	rm -f $(BIN) $(SRC:.c=.o) $(SRC:.c=.d) $(DBFILE) $(ZIPNAME) sources.zip
