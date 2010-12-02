
VERSION = 1.0.0
RELEASE_DIR = release
PACKAGE_EN_DIR = urg_library-$(VERSION)
PACKAGE_JP_DIR = urg_library_jp-$(VERSION)


all :
	cd current/ && $(MAKE)

clean : release_clean
	cd current/ && $(MAKE) clean
	$(RM) -rf $(RELEASE_DIR)

install : dist
	cd $(RELEASE_DIR)/$(PACKAGE_JP_DIR) && $(MAKE) install

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
		cp current/Makefile.release $(RELEASE_DIR)/$$i/Makefile; \
		cp current/src/Makefile.release $(RELEASE_DIR)/$$i/src/Makefile; \
		cp current/samples/Makefile.release $(RELEASE_DIR)/$$i/samples/Makefile ; \
		cp current/include/*.h $(RELEASE_DIR)/$$i/include/; \
		cp current/src/*.c $(RELEASE_DIR)/$$i/src/; \
		cp current/samples/*.sh $(RELEASE_DIR)/$$i/samples/; \
		cat current/urg_c-config.in | sed -e "s/VERSION/$(VERSION)/g" > $(RELEASE_DIR)/$$i/urg_c-config.in ; \
	done
	ruby split_comment.rb -e current/include/*.h $(RELEASE_DIR)/$(PACKAGE_EN_DIR)/include/
	ruby split_comment.rb -e current/src/*.c $(RELEASE_DIR)/$(PACKAGE_EN_DIR)/src/
	ruby split_comment.rb -e current/samples/*.c $(RELEASE_DIR)/$(PACKAGE_EN_DIR)/samples/
	ruby split_comment.rb -j current/include/*.h $(RELEASE_DIR)/$(PACKAGE_JP_DIR)/include/
	ruby split_comment.rb -j current/src/*.c $(RELEASE_DIR)/$(PACKAGE_JP_DIR)/src/
	ruby split_comment.rb -j current/samples/*.c $(RELEASE_DIR)/$(PACKAGE_JP_DIR)/samples/
	for i in $(TARGET_DIR) ; \
	do \
		cd $(PWD); \
		cd $(RELEASE_DIR)/ && (zip -r $$i.zip $$i) && mv $$i.zip ../; \
	done

release_clean :
	$(RM) -rf $(RELEASE_DIR)/$(PACKAGE_EN_DIR) $(RELEASE_DIR)/$(PACKAGE_JP_DIR)
	$(RM) $(PACKAGE_EN_DIR).zip $(PACKAGE_JP_DIR).zip
