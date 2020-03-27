cd thirdparty

cbp2make -in cpptest.cbp -out cpptest.make
make -f cpptest.make

cbp2make -in csparse.cbp -out csparse.make
make -f csparse.make

cbp2make -in cxsparse.cbp -out cxsparse.make
make -f cxsparse.make

rm -f *.make


cd ../steps

cbp2make -in STEPS.cbp -out STEPS.make
make -f STEPS.make

rm -f *.make
