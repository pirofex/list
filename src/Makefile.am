AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = eportfolio

htmlSources = \
        view/index.html

generatedSources = \
	$(htmlSources:.html=.cpp) \
	resources.cpp

staticSources = \
        view/js/jquery.min.js \
        view/js/skel.min.js \
        view/js/skel-layers.min.js \
        view/js/init.js \
        view/css/skel.css \
        view/css/style.css \
        view/css/style-desktop.css

eportfolio_SOURCES = \
        main.cpp \
        IndexController.cpp \
        ListManager.cpp

nodist_eportfolio_SOURCES = \
	$(generatedSources)

eportfolio_LDADD = \
	-ltntnet \
	-lcxxtools \
        -ltntdb

resources.cpp: $(staticSources) Makefile.am
	$(ECPPC) -bb -z -n resources -p -o resources.cpp -I$(srcdir) $(staticSources)

%.cpp: %.html
	$(ECPPC) $< -o $@
