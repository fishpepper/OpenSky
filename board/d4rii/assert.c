#include <stdio.h>

void assert_failed(uint8_t* file, uint32_t line){
  //todo: add own implementation to report the file name and line number,
  //eg  : printf("Wrong parameters value: file %s on line %d\r\n", file, line) 

  //infinite loop
  while (1){}
}

