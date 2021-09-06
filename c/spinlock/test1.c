/* ********************************************
 * FILE NAME  : test1.c
 * PROGRAMMER : zhaozz
 * START DATE : 2021-09-06 16:53:10
 * DESCIPTION : mips spinlock desciption
 * *******************************************/

spin_lock(lock)
1:
	ll t0, lock
	bnez t0, 1b
	li t0, 1
	sc t0, lock
	beqz t0,1b
	sync

spin_unlock(lock)
	sync
	sw zero, lock


