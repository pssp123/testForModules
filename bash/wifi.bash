#!/bin/bash
#nmcli con up id x900_pierre iface wlxaca2135c6d2c
echo ""
echo "         --------Start read device-------"
nmcli dev | awk '$1!="DEVICE"{print $1}' > tmp.txt
while read line
do
	let i++
	device[$i]=$line
done < tmp.txt

found=0
for val in ${device[*]}
do
	if [ "$val" == "$1" ]; then
		found=1
	fi
done
unset i device

if [ "$found" == "0" ]; then
	echo "Please enter the correct device"
else
	echo "Found the correct device"
	echo "         --------Finished read device-------"
	echo ""
	echo "         --------Start read ssid-------"
	nmcli dev wifi | grep Infra | awk '$1!="--"{for(i=1;i<NF;i++)if($i=="Infra")num=i;printf $(num-1);printf "\n"}' > tmp.txt
	found=0
	while read line
	do
		let i++
		ssid[$i]=$line
	done < tmp.txt

	for val in ${ssid[*]}
	do
		if [ "$val" == "$2" ]; then
				found=1
		fi
	done
	unset i ssid

	if [ "$found" == "0" ]; then
		echo "Please Enter the correct ssid"
	else
		echo "Found the correct ssid"
		echo "         --------Finished read ssid-------"
		echo ""
		nmcli con up id $2 $1
	fi
fi
