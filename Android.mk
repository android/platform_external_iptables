ifneq ($(TARGET_SIMULATOR),true)
  BUILD_IPTABLES := 1
endif
ifeq ($(BUILD_IPTABLES),1)

LOCAL_PATH:= $(call my-dir)

#
# Build libraries
#

# libiptc

include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= \
	$(KERNEL_HEADERS) \
	$(LOCAL_PATH)/include/

LOCAL_CFLAGS:=-DNO_SHARED_LIBS

LOCAL_SRC_FILES:= \
	libiptc/libip4tc.c \
	libiptc/libip6tc.c

LOCAL_MODULE_TAGS:=
LOCAL_MODULE:=libiptc

include $(BUILD_STATIC_LIBRARY)

# libext

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS:=
LOCAL_MODULE:=libext

# LOCAL_MODULE_CLASS must be defined before calling $(local-intermediates-dir)
#
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
intermediates := $(call local-intermediates-dir)

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/include/ \
	$(KERNEL_HEADERS) \
	$(intermediates)/extensions/

LOCAL_CFLAGS:=-DNO_SHARED_LIBS
LOCAL_CFLAGS+=-D_INIT=$*_init
LOCAL_CFLAGS+=-DIPTABLES_VERSION=\"1.3.7\"

PF_EXT_SLIB:=ah addrtype comment 2connmark conntrack 2dscp 2ecn esp 
PF_EXT_SLIB+=hashlimit helper icmp iprange length limit mac multiport #2mark
PF_EXT_SLIB+=owner physdev pkttype policy realm sctp standard state tcp 
PF_EXT_SLIB+=2tcpmss 2tos 2ttl udp unclean CLASSIFY CONNMARK DNAT LOG #DSCP ECN
PF_EXT_SLIB+=MASQUERADE MIRROR NETMAP NFQUEUE NOTRACK REDIRECT REJECT #MARK
PF_EXT_SLIB+=SAME SNAT ULOG # TOS TCPMSS TTL

EXT_FUNC+=$(foreach T,$(PF_EXT_SLIB),ipt_$(T))

# generated headers

GEN_INITEXT:= $(intermediates)/extensions/gen_initext.c
$(GEN_INITEXT): PRIVATE_PATH := $(LOCAL_PATH)
$(GEN_INITEXT): PRIVATE_CUSTOM_TOOL = $(PRIVATE_PATH)/extensions/create_initext "$(EXT_FUNC)" > $@
$(GEN_INITEXT): PRIVATE_MODULE := $(LOCAL_MODULE)
$(GEN_INITEXT):
	$(transform-generated-source)

$(intermediates)/extensions/initext.o : $(GEN_INITEXT)

LOCAL_GENERATED_SOURCES:= $(GEN_INITEXT)

LOCAL_SRC_FILES:= \
	$(foreach T,$(PF_EXT_SLIB),extensions/libipt_$(T).c) \
	extensions/initext.c

LOCAL_STATIC_LIBRARIES := \
	libc

include $(BUILD_STATIC_LIBRARY)

#
# Build iptables
#

include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/include/ \
	$(KERNEL_HEADERS)

LOCAL_CFLAGS:=-DNO_SHARED_LIBS
LOCAL_CFLAGS+=-DIPTABLES_VERSION=\"1.3.7\" # -DIPT_LIB_DIR=\"$(IPT_LIBDIR)\"
#LOCAL_CFLAGS+=-DIPT_LIB_DIR=\"$(IPT_LIBDIR)\"

LOCAL_SRC_FILES:= \
	iptables.c \
	iptables-standalone.c 

LOCAL_MODULE_TAGS:=
LOCAL_MODULE:=iptables

LOCAL_STATIC_LIBRARIES := \
	libiptc \
	libext

include $(BUILD_EXECUTABLE)

# libext6

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS:=debug
LOCAL_MODULE:=libext6

# LOCAL_MODULE_CLASS must be defined before calling $(local-intermediates-dir)
#
LOCAL_MODULE_CLASS := STATIC_LIBRARIES
intermediates := $(call local-intermediates-dir)

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/include/ \
	$(KERNEL_HEADERS) \
	$(intermediates)/extensions/

LOCAL_CFLAGS:=-DNO_SHARED_LIBS
LOCAL_CFLAGS+=-D_INIT=$*_init
LOCAL_CFLAGS+=-DIPTABLES_VERSION=\"1.3.7\"

PF6_EXT_SLIB:=2connmark eui64 2hl icmp6 length limit mac multiport # 2mark
PF6_EXT_SLIB+=owner physdev policy standard state tcp udp CONNMARK HL LOG
PF6_EXT_SLIB+=NFQUEUE MARK

EXT_FUNC6:=$(foreach T,$(PF6_EXT_SLIB),ip6t_$(T))

# generated headers

GEN_INITEXT6:= $(intermediates)/extensions/gen_initext6.c
$(GEN_INITEXT6): PRIVATE_PATH := $(LOCAL_PATH)
$(GEN_INITEXT6): PRIVATE_CUSTOM_TOOL = \
	$(PRIVATE_PATH)/extensions/create_initext "$(EXT_FUNC6)" > $@
$(GEN_INITEXT6): PRIVATE_MODULE := $(LOCAL_MODULE)
$(GEN_INITEXT6):
	$(transform-generated-source)

$(intermediates)/extensions/initext6.o : $(GEN_INITEXT6)

LOCAL_GENERATED_SOURCES:= $(GEN_INITEXT6)

LOCAL_SRC_FILES:= \
	$(foreach T,$(PF6_EXT_SLIB),extensions/libip6t_$(T).c) \
	extensions/initext6.c

LOCAL_STATIC_LIBRARIES := \
	libc

include $(BUILD_STATIC_LIBRARY)

#
# Build ip6tables
#

include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH)/include/ \
	$(KERNEL_HEADERS)

LOCAL_CFLAGS:=-DNO_SHARED_LIBS
LOCAL_CFLAGS+=-DIPTABLES_VERSION=\"1.3.7\" # -DIPT_LIB_DIR=\"$(IPT_LIBDIR)\"
#LOCAL_CFLAGS+=-DIPT_LIB_DIR=\"$(IPT_LIBDIR)\"

LOCAL_SRC_FILES:= \
	ip6tables.c \
	ip6tables-standalone.c

LOCAL_MODULE_TAGS:=debug
LOCAL_MODULE:=ip6tables

LOCAL_STATIC_LIBRARIES := \
	libiptc \
	libext6

include $(BUILD_EXECUTABLE)

endif
