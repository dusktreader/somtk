NOTES_FILE = notes.txt

HTML_FILE = index.html

PAPERS = $(wildcard *.pdf)

BROWSER = firefox

WEB_HOST = beta.tricity.wsu.edu
WEB_DIR = public_html/astrocv
WEB_DEST = $(WEB_HOST):$(WEB_DIR)

RSYNC_FLAGS = -arvL

default: browse

clean:
	rm -f *~ *.bak index.html

browse: $(HTML_FILE)
	$(BROWSER) $<

install: $(HTML_FILE) $(PAPERS)
	ssh $(WEB_HOST) "[ -d $(WEB_DIR) ] || mkdir -p $(WEB_DIR)"
	scp $^ $(WEB_DEST)

update: $(HTML_FILE) $(PAPERS)
	rsync $(RSYNC_FLAGS) $^ $(WEB_DEST)

index.html: $(NOTES_FILE)
	rst2html $< >$@ || rm -f $@
