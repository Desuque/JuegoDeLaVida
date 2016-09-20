 # uso: hacer mips
 #		hacer 
CC=gcc
FLAGS="-c -std=c99"
PROGRAMA=conway

for ARCHIVO in *.c
do
	if test -f $ARCHIVO
	then
		$CC $FLAGS $ARCHIVO
	fi
done

if [ $1 ==mips ]
then
	for ARCHIVO in *.S
	do
		if test -f $ARCHIVO
		then
			$CC $FLAGS $ARCHIVO
		fi
	done
fi

$CC *.o -o $PROGRAMA

rm *.o

./conway
