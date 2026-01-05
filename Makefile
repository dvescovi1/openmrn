# Ensures all recursive directories get OPENMRNPATH appropriately.
OPENMRNPATH:=$(realpath .)
export OPENMRNPATH

$(shell touch $(OPENMRNPATH)/build_timestamp)
export HAVE_BUILD_TIMESTAMP=$(OPENMRNPATH)/build_timestamp

SUBDIRS = targets applications

include $(OPENMRNPATH)/etc/recurse.mk

build-applications: build-targets

tests-applications: build-targets

.PHONY: cov
cov:
$(MAKE) -C targets/cov cov

tests:
$(MAKE) -C targets/test tests

tests-single:
$(MAKE) -C targets/test tests-single



alltests: tests

