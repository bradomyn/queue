#
# OMNeT++/OMNEST Makefile for fifo_wrs1
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I../wrs -L../wrs/out/$(CONFIGNAME) -lwrs -KWRS_PROJ=../wrs
#

# Name of target to be created (-o option)
TARGET = fifo_wrs1$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = -I../wrs -I. -Iresults

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../wrs/out/$(CONFIGNAME)  -lwrs
LIBS += -Wl,-rpath,`abspath ../wrs/out/$(CONFIGNAME)`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/Useful.o \
    $O/JobList.o \
    $O/Timer.o \
    $O/Source.o \
    $O/PassiveQueue.o \
    $O/WRClassifier_in.o \
    $O/Job.o \
    $O/WRSource.o \
    $O/WRS.o \
    $O/AbstractWRS.o \
    $O/Server.o \
    $O/WRSink.o \
    $O/SelectionStrategies.o \
    $O/Job_m.o

# Message files
MSGFILES = \
    Job.msg

# Other makefile variables (-K)
WRS_PROJ=../wrs

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
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

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
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

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
	-rm -f fifo_wrs1 fifo_wrs1.exe libfifo_wrs1.so libfifo_wrs1.a libfifo_wrs1.dll libfifo_wrs1.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f results/*_m.cc results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/AbstractWRS.o: AbstractWRS.cc \
	AbstractWRS.h \
	Useful.h \
	WRSink.h
$O/Job.o: Job.cc \
	Job.h \
	JobList.h \
	Job_m.h \
	$(WRS_PROJ)/Job.h \
	$(WRS_PROJ)/JobList.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/QueueingDefs.h
$O/JobList.o: JobList.cc \
	Job.h \
	JobList.h \
	Job_m.h \
	$(WRS_PROJ)/Job.h \
	$(WRS_PROJ)/JobList.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/QueueingDefs.h
$O/Job_m.o: Job_m.cc \
	Job_m.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/QueueingDefs.h
$O/PassiveQueue.o: PassiveQueue.cc \
	IPassiveQueue.h \
	Job.h \
	Job_m.h \
	PassiveQueue.h \
	SelectionStrategies.h \
	$(WRS_PROJ)/IPassiveQueue.h \
	$(WRS_PROJ)/Job.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/PassiveQueue.h \
	$(WRS_PROJ)/QueueingDefs.h \
	$(WRS_PROJ)/SelectionStrategies.h
$O/SelectionStrategies.o: SelectionStrategies.cc \
	IPassiveQueue.h \
	IServer.h \
	PassiveQueue.h \
	SelectionStrategies.h \
	Server.h \
	$(WRS_PROJ)/IPassiveQueue.h \
	$(WRS_PROJ)/IServer.h \
	$(WRS_PROJ)/PassiveQueue.h \
	$(WRS_PROJ)/QueueingDefs.h \
	$(WRS_PROJ)/SelectionStrategies.h \
	$(WRS_PROJ)/Server.h
$O/Server.o: Server.cc \
	IPassiveQueue.h \
	IServer.h \
	Job.h \
	Job_m.h \
	SelectionStrategies.h \
	Server.h \
	$(WRS_PROJ)/IPassiveQueue.h \
	$(WRS_PROJ)/IServer.h \
	$(WRS_PROJ)/Job.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/QueueingDefs.h \
	$(WRS_PROJ)/SelectionStrategies.h \
	$(WRS_PROJ)/Server.h
$O/Source.o: Source.cc \
	Job.h \
	Job_m.h \
	Timer.h \
	Useful.h \
	$(WRS_PROJ)/Job.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/QueueingDefs.h \
	$(WRS_PROJ)/Timer.h
$O/Timer.o: Timer.cc \
	Timer.h \
	$(WRS_PROJ)/Timer.h
$O/Useful.o: Useful.cc \
	Useful.h
$O/WRClassifier_in.o: WRClassifier_in.cc \
	Job.h \
	Job_m.h \
	Useful.h \
	WRClassifier_in.h \
	$(WRS_PROJ)/Job.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/QueueingDefs.h \
	$(WRS_PROJ)/WRClassifier_in.h
$O/WRS.o: WRS.cc \
	AbstractWRS.h \
	Useful.h \
	WRS.h \
	WRSink.h \
	$(WRS_PROJ)/QueueingDefs.h \
	$(WRS_PROJ)/Sink.h \
	$(WRS_PROJ)/WRPacket.h
$O/WRSink.o: WRSink.cc \
	Useful.h \
	WRSink.h
$O/WRSource.o: WRSource.cc \
	Job.h \
	Job_m.h \
	Timer.h \
	Useful.h \
	$(WRS_PROJ)/Job.h \
	$(WRS_PROJ)/Job_m.h \
	$(WRS_PROJ)/QueueingDefs.h \
	$(WRS_PROJ)/Timer.h

