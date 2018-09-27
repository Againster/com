SUBDIRS= admin branch dbservice devauth email sms usereg usersso auth devaddr icenter moa slogreceiver ucenter usermgr common


#--------------------------------
# PLEASE DO NOT CHANGE SCRIPT BELOW
# AUTHOR: ArrowLee
am__cd = CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)" && cd
#am__make_keepgoing = (target_option=k; $(am__make_running_with_option))
am__make_keepgoing = false

RECURSIVE_TARGETS = all-recursive install-data-recursive install-exec-recursive \
	install-recursive installdirs-recursive uninstall-recursive \
	test-recursive 

RECURSIVE_CLEAN_TARGETS = clean-recursive

am__recursive_targets = \
	$(RECURSIVE_TARGETS) \
  	$(RECURSIVE_CLEAN_TARGETS) \
	$(am__extra_recursive_targets)

all:
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

# This directory's subdirectories are mostly independent; you can cd
# into them and run 'make' without going through this Makefile.
# To change the values of 'make' variables: instead of editing Makefiles,
# (1) if the variable is set in 'config.status', edit 'config.status'
#     (which will cause the Makefiles to be regenerated when you run 'make');
# (2) otherwise, pass the desired values on the 'make' command line.
$(am__recursive_targets):
	@fail=; \
	if $(am__make_keepgoing); then \
	  failcom='fail=yes'; \
	else \
	  failcom='exit 1'; \
	fi; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	case "$@" in \
	  distclean-* | maintainer-clean-*) list='$(DIST_SUBDIRS)' ;; \
	  *) list='$(SUBDIRS)' ;; \
	esac; \
	for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  ($(am__cd) $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	  || eval $$failcom; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

installdirs: installdirs-recursive
installdirs-am:
install: install-recursive
install-exec: install-exec-recursive
install-data: install-data-recursive
uninstall: uninstall-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

all-am: Makefile

clean: clean-recursive
clean-am:

test: test-recursive
test-am:

uninstall-am:

install-exec-am:
install-data-am:
