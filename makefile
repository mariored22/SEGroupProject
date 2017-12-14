all: iPrologCpp

iPrologCpp: IMap.h Main.cpp Clause.cpp Engine.cpp IntStack.cpp IntList.cpp IntMap.cpp Prog.cpp Spine.cpp Toks.cpp
	g++ -Wall -pedantic IMap.h Main.cpp Clause.cpp Engine.cpp IntList.cpp IntStack.cpp IntMap.cpp Prog.cpp Spine.cpp Toks.cpp -o iPrologCpp