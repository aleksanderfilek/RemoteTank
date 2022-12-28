/*
 * config.h
 *
 * Created: 2022-12-28 23:56:08
 *  Author: Alek
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_


#include <stdint.h>

#define F_CPU 16000000
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD) - 1)


#endif /* CONFIG_H_ */