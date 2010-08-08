
VERSION = 1.0.0
RELEASE_DIR = release
PACKAGE_EN_DIR = urg_library-$(VERSION)
PACKAGE_JP_DIR = urg_library_jp-$(VERSION)


all :
	cd current/ && $(MAKE)

clean : release_clean
	cd current/ && $(MAKE) clean
	-rmdir $(RELEASE_DIR)

dist : release_clean
	mkdir -p $(RELEASE_DIR)
	mkdir -p $(RELEASE_DIR)/$(PACKAGE_EN_DIR)
	mkdir -p $(RELEASE_DIR)/$(PACKAGE_JP_DIR)
	ls # !!! copy files
	cd $(RELEASE_DIR)/ && (zip -r $(PACKAGE_EN_DIR).zip $(PACKAGE_EN_DIR)) && mv $(PACKAGE_EN_DIR).zip ../
	cd $(RELEASE_DIR)/ && (zip -r $(PACKAGE_JP_DIR).zip $(PACKAGE_JP_DIR)) && mv $(PACKAGE_JP_DIR).zip ../

release_clean :
	$(RM) -rf $(RELEASE_DIR)/$(PACKAGE_EN_DIR) $(RELEASE_DIR)/$(PACKAGE_JP_DIR)
	$(RM) $(PACKAGE_EN_DIR).zip $(PACKAGE_JP_DIR).zip
