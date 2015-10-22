#!/bin/bash

cd /users/enseig/lesenech/Documents/OSEM/almos-tsar-mipsel-1.0
source SourceMe

export SOCLIB_TTY=FILES

cd /users/enseig/lesenech/Documents/OSEM/almos-tsar-mipsel-1.0/test/pf1

function watch_and_kill {
while [ 1 -eq 1 ]
do
	echo "about to KAT <=================="
	cat tty1 | grep TheEndIsTheMeanToAnEnd 
	if [ "$?" -eq 0 ]
	then 
		echo "Into <<< the <<< LOOOOp ééééééééééé"
		killall tsar-sim1
		#kill "$1"
		killall soclib-fb
		echo "e" > /tmp/hEEEEEEEEEEEEEEEElp

		echo "**************************************************************"
		break
	fi 
	echo "Going to sleep!"
	sleep 1
done

cd /users/enseig/lesenech/Documents/OSEM/almos-tsar-mipsel-1.0/test/pf1
file=`pwd`
echo "Let's make a fileee! $file"

nom="traces_sim_$2"
mkdir "$nom"
cp tty* "$nom"

  }


  rm tty1
  touch tty1
  make sim1 &
  pid=$$
  watch_and_kill "$pid"  1


  rm tty1
  touch tty1
  make sim4 &
  pid=$$
  watch_and_kill "$pid"  4

  rm tty1
  touch tty1
  make sim16 &
  pid=$$
  watch_and_kill "$pid"  16


  rm tty1
  touch tty1
  make sim64 &
  pid=$$
  watch_and_kill "$pid"  64



  echo "################################DONE"




