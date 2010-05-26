NOTES_FILE = index.txt

HTML_FILE = index.html

BROWSER = firefox

IMAGE_FILES = \
	finder-detected.png \
	finder-undetected.png \

WEB_HOST = beta.tricity.wsu.edu
WEB_DIR = public_html/astrocv
WEB_DEST = $(WEB_HOST):$(WEB_DIR)

default: browse

clean:
	rm -f *~

browse: $(HTML_FILE)
	$(BROWSER) $<

install: $(HTML_FILE) $(IMAGE_FILES)
	ssh $(WEB_HOST) "[ -d $(WEB_DIR) ] || mkdir -p $(WEB_DIR)"
	scp $^ $(WEB_DEST)

index.html: $(NOTES_FILE)
	rst2html $< >$@ || rm -f $@
