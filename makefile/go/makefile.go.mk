# program name
bin_PROGRAMS = usersso
# config file that will be installed
sysconf_DATA = ./src/usersso/conf/usersso.ini
# such as "-tags static"
GOBUILD_FLAGS = -tags static
# such as "-v -cover -coverprofile=cover.out ./..." 
GOTEST_FLAGS = -v -cover -coverprofile=cover.out ./... 
# main package file
GOBUILD_MAIN = ./src/usersso/main.go
# just for go path, if you like, you could delete it
PROJECT_DIR=$(shell cd "$(dirname "$0")" ; pwd -P)/../../..
# GOPATH
INCLUDE_GOPATH = ${PROJECT_DIR}/src/3party:${PROJECT_DIR}/src/libs:${PROJECT_DIR}/src/apps/usersso
# GOROOT
INCLUDE_GOROOT =
# program relative dir, always be empty
BIN_PROGRAM_BASE =
# config file relative dir
SYSCONF_DATA_BASE = ${bin_PROGRAMS}
# PKG_CONFIG_PATH, such as "/usr/lib/pkgconfig:/usr/local/lib/pkgconfig"
INCLUDE_PKG_CONFIG_PATH =


#----------------------------------
# PLEASE DO NOT CHANGE SCRIPT BELOW
# AUGHOR: ArrowLee
# DATE: 20180726
prefix = /sf/scloud
exec_prefix = ${prefix}
bindir = ${exec_prefix}/bin/${BIN_PROGRAM_BASE}
sbindir = ${exec_prefix}/sbin
includedir = ${prefix}/include
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localstatedir = ${prefix}/var
sysconfdir = ${prefix}/etc/${SYSCONF_DATA_BASE}
sharedstatedir = ${prefix}/com
EXEEXT = 
DESTDIR = 
NORMAL_INSTALL = :
NORMAL_UNINSTALL = :
MKDIR_P = /usr/bin/mkdir -p
INSTALL = /usr/bin/install -c
INSTALL_DATA = ${INSTALL} -m 644
GOBUILD = go build
GOTEST = go test
transform = s,x,x,
AWK = gawk
INSTALL_PROGRAM = ${INSTALL}
LIBTOOL = /usr/bin/libtool

am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
am__base_list = \
  sed '$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;$$!N;s/\n/ /g' | \
  sed '$$!N;$$!N;$$!N;$$!N;s/\n/ /g'
am__uninstall_files_from_dir = { \
  test -z "$$files" \
    || { test ! -d "$$dir" && test ! -f "$$dir" && test ! -r "$$dir"; } \
    || { echo " ( cd '$$dir' && rm -f" $$files ")"; \
         $(am__cd) "$$dir" && rm -f $$files; }; \
  }

all: $(bin_PROGRAMS)

gobuild-env:
export GOPATH=$(INCLUDE_GOPATH)
export GOROOT=$(INCLUDE_GOROOT)
export PKG_CONFIG_PATH=$(INCLUDE_PKG_CONFIG_PATH)

$(bin_PROGRAMS): gobuild-env
	@rm -f $(bin_PROGRAMS); \
	echo "building $(bin_PROGRAMS)"; \
	$(GOBUILD) -v -o $(bin_PROGRAMS) $(GOBUILD_FLAGS) $(GOBUILD_MAIN) || exit $$?;

test: gobuild-env
	@echo "testing $(bin_PROGRAMS)"; \
	$(GOTEST) $(GOTEST_FLAGS)

clean: clean-binPROGRAMS

uninstall: uninstall-binPROGRAMS uninstall-sysconfDATA

installdirs:
	for dir in "$(DESTDIR)$(bindir)"; do \
	  test -z "$$dir" || $(MKDIR_P) "$$dir"; \
	done

install: install-binPROGRAMS install-sysconfDATA

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	@list='$(bin_PROGRAMS)'; test -n "$(bindir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(bindir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(bindir)" || exit 1; \
	fi; \
	for p in $$list; do echo "$$p $$p"; done | \
	sed 's/$(EXEEXT)$$//' | \
	while read p p1; do if test -f $$p \
	 || test -f $$p1 \
	  ; then echo "$$p"; echo "$$p"; else :; fi; \
	done | \
	sed -e 'p;s,.*/,,;n;h' \
	    -e 's|.*|.|' \
	    -e 'p;x;s,.*/,,;s/$(EXEEXT)$$//;$(transform);s/$$/$(EXEEXT)/' | \
	sed 'N;N;N;s,\n, ,g' | \
	$(AWK) 'BEGIN { files["."] = ""; dirs["."] = 1 } \
	  { d=$$3; if (dirs[d] != 1) { print "d", d; dirs[d] = 1 } \
	    if ($$2 == $$4) files[d] = files[d] " " $$1; \
	    else { print "f", $$3 "/" $$4, $$1; } } \
	  END { for (d in files) print "f", d, files[d] }' | \
	while read type dir files; do \
	    if test "$$dir" = .; then dir=; else dir=/$$dir; fi; \
	    test -z "$$files" || { \
	    echo " $(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$files '$(DESTDIR)$(bindir)$$dir'"; \
	    $(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) $$files "$(DESTDIR)$(bindir)$$dir" || exit $$?; \
	    } \
	; done


uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_PROGRAMS)'; test -n "$(bindir)" || list=; \
	files=`for p in $$list; do echo "$$p"; done | \
	  sed -e 'h;s,^.*/,,;s/$(EXEEXT)$$//;$(transform)' \
	      -e 's/$$/$(EXEEXT)/' \
	`; \
	test -n "$$list" || exit 0; \
	echo " ( cd '$(DESTDIR)$(bindir)' && rm -f" $$files ")"; \
	cd "$(DESTDIR)$(bindir)" && rm -f $$files
	
clean-binPROGRAMS:
	@list='$(bin_PROGRAMS)'; test -n "$$list" || exit 0; \
	echo " rm -f" $$list; \
	rm -f $$list || exit $$?; \
	test -n "$(EXEEXT)" || exit 0; \
	list=`for p in $$list; do echo "$$p"; done | sed 's/$(EXEEXT)$$//'`; \
	echo " rm -f" $$list; \
	rm -f $$list

install-sysconfDATA: $(sysconf_DATA)
	@$(NORMAL_INSTALL)
	@list='$(sysconf_DATA)'; test -n "$(sysconfdir)" || list=; \
	if test -n "$$list"; then \
	  echo " $(MKDIR_P) '$(DESTDIR)$(sysconfdir)'"; \
	  $(MKDIR_P) "$(DESTDIR)$(sysconfdir)" || exit 1; \
	fi; \
	for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  echo "$$d$$p"; \
	done | $(am__base_list) | \
	while read files; do \
	  echo " $(INSTALL_DATA) $$files '$(DESTDIR)$(sysconfdir)'"; \
	  $(INSTALL_DATA) $$files "$(DESTDIR)$(sysconfdir)" || exit $$?; \
	done

uninstall-sysconfDATA:
	@$(NORMAL_UNINSTALL)
	@list='$(sysconf_DATA)'; test -n "$(sysconfdir)" || list=; \
	files=`for p in $$list; do echo $$p; done | sed -e 's|^.*/||'`; \
	dir='$(DESTDIR)$(sysconfdir)'; $(am__uninstall_files_from_dir)
