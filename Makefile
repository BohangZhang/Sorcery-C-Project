CXX = g++
CXXFLAGS = -g -std=c++14 -Wall -MMD
EXEC = sorcery
OBJECTS = main.o act_minion.o ascii_graphics.o card.o database.o deck.o enchantment.o  game.o graveyard.o hand.o minion.o player.o ritual.o slot.o spell.o tri_minion.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
