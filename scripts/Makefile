CC=g++
EXT=cpp
CCFLAGS=
LDFLAGS=
LIBS=


SOURCEDIR=..
SCRIPTDIR=.
SCRIPTOBJECTDIR=${SCRIPTDIR}/obj
OBJECTDIR=${SOURCEDIR}/build/Debug/GNU-Linux

OBJECTFILES= \
	${SCRIPTOBJECTDIR}/analyse.o \
	${OBJECTDIR}/action.o \
	${OBJECTDIR}/board.o \
	${OBJECTDIR}/check.o \
	${OBJECTDIR}/eval.o \
	${OBJECTDIR}/hash.o \
	${OBJECTDIR}/init.o \
	${OBJECTDIR}/move.o \
	${OBJECTDIR}/play.o \
	${OBJECTDIR}/search.o \
	${OBJECTDIR}/twiddle.o \
	${OBJECTDIR}/util/doublylinkedlist.o \
	${OBJECTDIR}/util/linkedlist.o \
	${OBJECTDIR}/util/queue.o \
	${OBJECTDIR}/util/stack.o \
	${OBJECTDIR}/util/tree.o


.PHONY: all clean dirs

analyse: dirs ${OBJECTFILES}
	$(LINK.cc) -o $@.exe ${LIBS} ${OBJECTFILES} ${CCFLAGS} ${LDFLAGS}

clean:
	rm -rf analyse ${OBJECTFILES}

dirs:
	mkdir -p ${SCRIPTOBJECTDIR} ${OBJECTDIR}

${SCRIPTOBJECTDIR}/analyse.o: ${SCRIPTDIR}/analyse.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${SCRIPTOBJECTDIR}/analyse.o ${SCRIPTDIR}/analyse.cpp

${OBJECTDIR}/action.o: ${SOURCEDIR}/action.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/action.o ${SOURCEDIR}/action.cpp

${OBJECTDIR}/board.o: ${SOURCEDIR}/board.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/board.o ${SOURCEDIR}/board.cpp

${OBJECTDIR}/check.o: ${SOURCEDIR}/check.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/check.o ${SOURCEDIR}/check.cpp

${OBJECTDIR}/eval.o: ${SOURCEDIR}/eval.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/eval.o ${SOURCEDIR}/eval.cpp

${OBJECTDIR}/hash.o: ${SOURCEDIR}/hash.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/hash.o ${SOURCEDIR}/hash.cpp

${OBJECTDIR}/init.o: ${SOURCEDIR}/init.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/init.o ${SOURCEDIR}/init.cpp

${OBJECTDIR}/move.o: ${SOURCEDIR}/move.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/move.o ${SOURCEDIR}/move.cpp

${OBJECTDIR}/play.o: ${SOURCEDIR}/play.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/play.o ${SOURCEDIR}/play.cpp

${OBJECTDIR}/search.o: ${SOURCEDIR}/search.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/search.o ${SOURCEDIR}/search.cpp

${OBJECTDIR}/twiddle.o: ${SOURCEDIR}/twiddle.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/twiddle.o ${SOURCEDIR}/twiddle.cpp

${OBJECTDIR}/util/doublylinkedlist.o: ${SOURCEDIR}/util/doublylinkedlist.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/util/doublylinkedlist.o ${SOURCEDIR}/util/doublylinkedlist.cpp

${OBJECTDIR}/util/linkedlist.o: ${SOURCEDIR}/util/linkedlist.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/util/linkedlist.o ${SOURCEDIR}/util/linkedlist.cpp

${OBJECTDIR}/util/queue.o: ${SOURCEDIR}/util/queue.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/util/queue.o ${SOURCEDIR}/util/queue.cpp

${OBJECTDIR}/util/stack.o: ${SOURCEDIR}/util/stack.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/util/stack.o ${SOURCEDIR}/util/stack.cpp

${OBJECTDIR}/util/tree.o: ${SOURCEDIR}/util/tree.cpp
	$(COMPILE.cc) -g -I${SOURCEDIR} -I${SOURCEDIR}/util -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/util/tree.o ${SOURCEDIR}/util/tree.cpp
