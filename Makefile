all : flash

TARGET:=temperature

CH32V003FUN:=ch32v003fun/ch32v003fun
MINICHLINK:=ch32v003fun/minichlink
include ch32v003fun/ch32v003fun/ch32v003fun.mk

flash : cv_flash
clean : cv_clean


