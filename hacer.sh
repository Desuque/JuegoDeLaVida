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
			echo "Compilado ${ARCHIVO}"
			$CC $FLAGS $ARCHIVO
		fi
	done
fi
ARCHIVOS=
for ARCHIVO in *.o
do
	ARCHIVOS="${ARCHIVOS} ${ARCHIVO}"
done

$CC ${ARCHIVOS} -o $PROGRAMA
echo "$CC ${ARCHIVOS} -o $PROGRAMA"

rm *.o

./conway

echo "
"