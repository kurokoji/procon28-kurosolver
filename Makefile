
.PHONEY: all

all: waf
	./waf configure build

waf:
	curl https://waf.io/waf-1.9.4 -o waf
	chmod +x waf
