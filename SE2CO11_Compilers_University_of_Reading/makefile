default: cPlusLexer.c cPlusParser.tab.c cPlusParser.tab.h
	gcc -o compiler -L/usr/local/opt/flex/lib -DYYDEBUG=1 cPlusLexer.c cPlusParser.tab.c

cPlusLexer.c: cPlusGrammar.l
	@$(MAKE) flex

cPlusParser.tab.c: cPlusGrammar.y
	@$(MAKE) yacc

cPlusParser.tab.h: cPlusGrammar.y
	@$(MAKE) yacc

flex: cPlusGrammar.l
	flex -o cPlusLexer.c cPlusGrammar.l

yacc: cPlusGrammar.y
	yacc --debug -d -b cPlusParser cPlusGrammar.y

clean:
	@rm -f compiler
	@rm -f output
	@rm -f *.c
	@rm -f *.h
	@rm -f cPlusParser.output
