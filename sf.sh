gcc main.c
if [ -f ./a.out ]
then 
./a.out
rm a.out
else 
echo "Error de compilacion"
fi
