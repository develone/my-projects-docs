#!/bin/bash
counter=$1
Lab="192.168.1.159"
SpareBR="192.168.1.175"
Bath="192.168.1.160"
FrontBRlf="192.168.1.178"
LvgRm="192.168.1.177"
mobile="192.168.1.176"
#echo $2
while [ $counter -gt 0 ]
do 
	ping -c 1 $Lab
	iperf -c $Lab
	echo $Lab
	
	ping -c 1 $SpareBR
	iperf -c $SpareBR
	echo $SpareBR
	
	ping -c 1 $Bath
	iperf -c $Bath
	echo $Bath

	
	ping -c 1 $FrontBRlf
	iperf -c $FrontBRlf
	echo $FrontBRlf
	
	ping -c 1 $LvgRm
	iperf -c $LvgRm
	echo $LvgRm	

	
	ping -c 1 $mobile
	iperf -c $mobile
	echo $mobile
				
	echo $counter
	
	counter=$(( $counter - 1 ))
done

