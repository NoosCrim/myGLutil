include make/target.mk

include build.mk

include test/test.mk
$(eval $(call all_target))
$(eval $(call tests_target))