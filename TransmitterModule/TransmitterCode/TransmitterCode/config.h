/*
 * Config.h
 *
 * Created: 10.08.2022 13:36:04
 *  Author: filek
 */ 

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdint.h>

#define F_CPU 16000000
#define BAUD 9600
#define BRC ((F_CPU/16/BAUD) - 1)

#endif /* CONFIG_H_ */