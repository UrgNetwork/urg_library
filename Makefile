
VERSION = 1.0.0
RELEASE_DIR = release
PACKAGE_EN_DIR = urg_library-$(VERSION)
PACKAGE_JP_DIR = urg_library_jp-$(VERSION)


all :
	cd current/ && $(MAKE)

clean : release_clean
	cd current/ && $(MAKE) clean
	-rmdir $(RELEASE_DIR)

# !!! use for
TARGET_DIR = $(PACKAGE_EN_DIR) $(PACKAGE_JP_DIR)
dist : release_clean
	mkdir -p $(RELEASE_DIR)
	for i in $(TARGET_DIR) ; \
	do \
		mkdir -p $(RELEASE_DIR)/$$i; \
		mkdir -p $(RELEASE_DIR)/$$i/include; \
		mkdir -p $(RELEASE_DIR)/$$i/src; \
		mkdir -p $(RELEASE_DIR)/$$i/samples; \
		cp current/COPYRIGHT current/Install.txt Readme.txt Releasenotes.txt $(RELEASE_DIR)/$$i/; \
	done
	ls # copy source files
	for i in $(TARGET_DIR) ; \
	do \
		cd $(PWD); \
		cd $(RELEASE_DIR)/ && (zip -r $$i.zip $$i) && mv $$i.zip ../; \
	done

release_clean :
	$(RM) -rf $(RELEASE_DIR)/$(PACKAGE_EN_DIR) $(RELEASE_DIR)/$(PACKAGE_JP_DIR)
	$(RM) $(PACKAGE_EN_DIR).zip $(PACKAGE_JP_DIR).zip
