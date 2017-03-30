.PHONY: all clean test fonts

OBJS := font-rasterize.o
CFLAGS := -O3 -Wall -Wextra -std=c11 `pkg-config --cflags gtk+-3.0` -D_XOPEN_SOURCE=500
LDFLAGS := `pkg-config --libs gtk+-3.0` -lm

all: font-rasterize

clean:
	rm -f $(OBJS) font-rasterize

test: all
	./font-rasterize

fonts: all
	./font-rasterize --antialias=none --fontsize 20 "VCR OSD Mono" vcr-osd-mono-20.json
	./json-to-c.py -f -t 60 --create-pnm vcr-osd-mono-20.json
	./font-rasterize --antialias=none --fontsize 30 "VCR OSD Mono" vcr-osd-mono-30.json
	./json-to-c.py -c '0123456789.' -f -t 30 --create-text --text "123.456" --create-pnm vcr-osd-mono-30.json

font-rasterize: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

