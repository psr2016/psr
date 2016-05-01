



QTDIR=/opt/qt55
QTCORE=$(QTDIR)/include/QtCore
QTGUI=$(QTDIR)/include/QtGui
QTWIDGETS=$(QTDIR)/include/QtWidgets
QTOPENGL=$(QTDIR)/include/QtOpenGL

all: test

test: MainWindow.cpp main.cpp
	$(QTDIR)/bin/moc MainWindow.h > MainWindow_moc.h
	$(QTDIR)/bin/moc oxill.h > oxill_moc.h
	g++ -fPIC -std=c++11 -Wall \
		-I$(QTDIR)/include -I$(QTCORE) -I$(QTGUI) -I$(QTOPENGL) -I$(QTWIDGETS) \
		-I/usr/local/qwt-6.1.3-svn/include \
		-I/usr/include/bullet \
		MainWindow.cpp Body.cpp oxill.cpp Gear.cpp main.cpp World.cpp pid.cpp Input.cpp \
		-o test \
		-L$(QTDIR)/lib \
		-L/usr/local/qwt-6.1.3-svn/lib \
		-lQt5Core -lQt5Gui -lQt5Widgets \
		-lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath \
		-lqwt



clean:
	rm -f test
