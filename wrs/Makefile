#
# OMNeT++/OMNEST Makefile for libwrs
#
# This file was generated with the command:
#  opp_makemake -f --deep --make-so -O out
#

# Name of target to be created (-o option)
TARGET = libwrs$(SHARED_LIB_SUFFIX)

# C++ include paths (with -I)
INCLUDE_PATH = -I. -Itemplates -Itemplates/queueing

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/WRScheduler.o \
    $O/WRQueue.o \
    $O/Timer.o \
    $O/WRClassifier_out.o \
    $O/WRClassifier_in.o \
    $O/WRSource.o \
    $O/WR_Input.o \
    $O/Sink.o \
    $O/WRRouter.o \
    $O/WRServer.o \
    $O/WRTrigger.o \
    $O/Job_m.o

# Message files
MSGFILES = \
    Job.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppenvir$D $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(SHLIB_LD) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(LIBS) $(OMNETPP_LIBS) $(LDFLAGS)
	$(SHLIB_POSTPROCESS) $O/$(TARGET)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f wrs wrs.exe libwrs.so libwrs.a libwrs.dll libwrs.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f templates/*_m.cc templates/*_m.h
	-rm -f templates/queueing/*_m.cc templates/queueing/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc templates/*.cc templates/queueing/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/Job_m.o: Job_m.cc \
	Job_m.h \
	QueueingDefs.h
$O/Sink.o: Sink.cc \
	QueueingDefs.h \
	Sink.h \
	Timer.h \
	WRTrigger.h
$O/Timer.o: Timer.cc \
	Timer.h
$O/WRClassifier_in.o: WRClassifier_in.cc \
	QueueingDefs.h \
	WRClassifier_in.h
$O/WRClassifier_out.o: WRClassifier_out.cc \
	QueueingDefs.h \
	Timer.h \
	WRClassifier_out.h \
	WRQueue.h \
	WRTrigger.h
$O/WRQueue.o: WRQueue.cc \
	QueueingDefs.h \
	Timer.h \
	WRQueue.h \
	WRTrigger.h
$O/WRRouter.o: WRRouter.cc \
	QueueingDefs.h \
	WRRouter.h
$O/WRScheduler.o: WRScheduler.cc \
	QueueingDefs.h \
	WRScheduler.h
$O/WRServer.o: WRServer.cc \
	WRServer.h
$O/WRSource.o: WRSource.cc \
	QueueingDefs.h \
	WRSource.h
$O/WRTrigger.o: WRTrigger.cc \
	QueueingDefs.h \
	Timer.h \
	WRTrigger.h
$O/WR_Input.o: WR_Input.cc \
	QueueingDefs.h \
	WR_Input.h

