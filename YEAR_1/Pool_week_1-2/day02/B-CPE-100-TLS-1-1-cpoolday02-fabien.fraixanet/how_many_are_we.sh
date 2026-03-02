if [ -z "$@" ]
   
then
    wc -l

else
     grep -i -w $@ | wc -l

fi
