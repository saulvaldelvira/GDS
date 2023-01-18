status=0
for t in ./bin/* 
do 
valgrind --leak-check=yes --error-exitcode=1 "$t" 
status=$?
if [ $status -ne 0 ] ;
then 
exit $status
fi
done