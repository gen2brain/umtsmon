UMTSMONBASEINCS=../.. \
	-I../../src/model \
        -I../../src/view \
        -I../../src/base \
        -I../../src/controller

ALLDIRS=`ls -1d *Tester`

test:
	export UMTSMONBASEINCS="${UMTSMONBASEINCS}"; \
	for I in ${ALLDIRS}; do cd $${I} && qmake; make all; cd ..; done;

all: distclean test

run:
	for I in ${ALLDIRS}; do cd $${I}; ./$${I}; cd ..; done;

clean:
	for I in ${ALLDIRS}; do make -C $${I} clean; done;

distclean:
	for I in ${ALLDIRS}; do make -C $${I} distclean; done;

test:
	export UMTSMONBASEINCS="${UMTSMONBASEINCS}"; \
	for I in ${ALLDIRS}; do cd $${I} && qmake; make all; cd ..; done;
