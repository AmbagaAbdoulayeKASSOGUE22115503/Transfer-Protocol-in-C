CC  = gcc
SYS = -std=c11 -Wall -DUNIX
INC = -I.

# ============================= #
NAME =
# ============================= #

SRCDIR = src
OBJDIR = obj
BINDIR = bin
FILESDIR = fichiers

SENDER = $(BINDIR)/emetteur
RECEIVER = $(BINDIR)/recepteur

OBJ_COMMON = $(OBJDIR)/config.o $(OBJDIR)/services_reseau.o $(OBJDIR)/couche_transport.o
OBJ_APP_NC = $(OBJDIR)/appli_non_connectee.o
OBJ_APP_C  = $(OBJDIR)/appli_connectee.o

OBJ_TDD0_S = $(OBJDIR)/proto_tdd_v0_emetteur.o
OBJ_TDD0_R = $(OBJDIR)/proto_tdd_v0_recepteur.o

OBJ_TDD1_S = $(OBJDIR)/proto_tdd_v1_emetteur.o
OBJ_TDD1_R = $(OBJDIR)/proto_tdd_v1_recepteur.o

OBJ_TDD2_S = $(OBJDIR)/proto_tdd_v2_emetteur.o
OBJ_TDD2_R = $(OBJDIR)/proto_tdd_v2_recepteur.o

OBJ_TDD3_S = $(OBJDIR)/proto_tdd_v3_emetteur.o
OBJ_TDD3_R = $(OBJDIR)/proto_tdd_v3_recepteur.o

OBJ_TDD4_S = $(OBJDIR)/proto_tdd_v4_emetteur.o
OBJ_TDD4_R = $(OBJDIR)/proto_tdd_v4_recepteur.o

# TDD v%
# -------
tdd%: dirs $(OBJ_COMMON)
	$(eval export OBJ_APP=$(shell if [ $* = 4 ]; \
		then echo $(OBJ_APP_C); \
		else echo $(OBJ_APP_NC); fi))
	$(eval export OBJ_TDD_S = $(OBJ_TDD$*_S))
	$(eval export OBJ_TDD_R = $(OBJ_TDD$*_R))
	make $(SENDER) $(RECEIVER)

# EXEC
# -----
$(SENDER): $(OBJ_COMMON) $(OBJ_APP) $(OBJ_TDD_S)
	$(CC) -o $(SENDER) $(OBJ_COMMON) $(OBJ_APP) $(OBJ_TDD_S)

$(RECEIVER): $(OBJ_COMMON) $(OBJ_APP) $(OBJ_TDD_R)
	$(CC) -o $(RECEIVER) $(OBJ_COMMON) $(OBJ_APP) $(OBJ_TDD_R)

# '%' matches filename
# $@  for the pattern-matched target
# $<  for the pattern-matched dependency
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -o $@ -c $< $(SYS) $(INC)

dirs:
	@if [ ! -d "./$(OBJDIR)" ]; then mkdir $(OBJDIR); fi
	@if [ ! -d "./$(BINDIR)" ]; then mkdir $(BINDIR); fi

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/* $(FILESDIR)/out.*

deliver:
	mkdir $(NAME)
	cp -r src $(NAME)
	cp README.txt $(NAME)
	zip -r $(NAME).zip $(NAME)
	rm -rf $(NAME)
	@echo " "
	@echo "=================================================="
	@echo "Vous devez maintenant déposer l'archive $(NAME).zip sur Moodle."
	@echo "=================================================="
