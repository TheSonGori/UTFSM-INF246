#MAKEFILE JAVA

all: default run clean

JFLAGS = -g
JC = javac
JVM = java
MAIN = Main
FILE = 

.SUFFIXES: .java .class

.java.class: 
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	Hebras.java \
	Forks.java \
	Nada.java

default: classes

classes: $(CLASSES:.java=.class)

run: $(MAIN).class
	$(JVM) $(MAIN) 

clean: 
	$(RM) *.class
