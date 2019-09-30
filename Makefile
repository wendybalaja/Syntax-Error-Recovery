
parse: parse.cpp scan.cpp
	g++ parse.cpp scan.cpp -o parse

test-error:
	(./parse < input2.txt)
test-correct:
	(./parse < input.txt)
run:
	./parse < input.txt
clean:
	rm parse


