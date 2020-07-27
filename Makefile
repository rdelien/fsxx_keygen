########################################################################
# Commands
AR:=			$(CROSS_COMPILE)ar
CC:=			$(CROSS_COMPILE)gcc
DEPEND:=		$(CROSS_COMPILE)gcc
RANLIB:=		$(CROSS_COMPILE)ranlib
INSTALL:=		install
MKDIR:=			mkdir
RM:=			rm
RMDIR:=			rmdir

########################################################################
# Flags
ARFLAGS:=		rv
CFLAGS:=		-Wall -Wundef -Wno-multichar
CFLAGS+=		-g
CPPFLAGS:=		-I.
DEPENDFLAGS:=		-M
LDFLAGS:=		-L.
MKDIRFLAGS:=		-p
RMFLAGS:=		-rf

########################################################################
# Directories
OUTPUT:=		$(shell $(CC) -dumpmachine)
DEPENDDIR:=		$(OUTPUT)/.depend
DESTDIR?=

########################################################################
# Target
BIN:=			keygen
SRC:=			main.c
OBJ:=			$(patsubst %.c,$(OUTPUT)/%.o,$(SRC))

########################################################################
# Standard symbolic targets
.PHONY: all
all: clean $(OUTPUT)/$(BIN)

.PHONY: clean
clean:
	$(RM) $(RMFLAGS) $(OBJ)

.PHONY: clobber
clobber: clean
	$(RM) $(RMFLAGS) $(OUTPUT)

.PHONY: install
install: $(BIN)
	$(INSTALL) -m755 -d $(DESTDIR)
	$(INSTALL) $(OUTPUT)/$(BIN) $(DESTDIR)

########################################################################
# Targets for creating the output directory, objects and binary
$(DEPENDDIR):
	$(MKDIR) $(MKDIRFLAGS) $@

$(OUTPUT):
	$(MKDIR) $(MKDIRFLAGS) $@

$(OUTPUT)/$(BIN): $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)

$(OUTPUT)/%.o: %.c | $(OUTPUT) $(DEPENDDIR)
	$(DEPEND) $(DEPENDFLAGS) $(CPPFLAGS) $(CFLAGS) -o $(DEPENDDIR)/$(*F).d $<
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
