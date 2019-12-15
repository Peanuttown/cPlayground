#include "debug.h"
#include <stdio.h>
#include "opcode.h"
#include "array.h"
#include "value.h"
#include "vm.h"
#include "stringObject.h"

void printSimpleInstruction(int offset,char* name){
	printf("%6d   %10s\n",offset,name);
}

static void printConstant(VM* vm,int offset,char* name){
	printSimpleInstruction(offset,name);
	int constantPos = *(uint8_t*)getArray(&vm->code,offset+1);
	printf("%6d   constantPos:%d   value:",offset+1,constantPos);
	printValue(getArray(&vm->constants,constantPos));
}

int disassembleOpCode(VM* vm,int offset,uint8_t opcode){
	switch(opcode){
		case OP_RETURN: {
					printSimpleInstruction(offset,"return");
					return 1;
				}
		case OP_CONSTANT:{
					 printConstant(vm,offset,"constant");
					 return 2;
				 }
		case OP_PRINT:{
				      printSimpleInstruction(offset,"print");
				      return 1;
			      }
		case OP_GET_GLOBAL:{
					   printSimpleInstruction(offset,"getGlobal");
					   int pos = *(uint8_t*)getArray(&vm->code,offset+1);
					   Value* posValue =(Value*)getArray(&vm->constants,pos);
					   struct sObjString* variable = (ObjString*)posValue->obj;
					   printf("%6d   key postion:%d , variable name:%s\n",offset+1,pos,variable->str);//print key pos;
					   return 2;
				   }
		case OP_DEFINE_GLOBAL:{
					      printSimpleInstruction(offset,"defineGlobal");
					      int pos = *(uint8_t*)getArray(&vm->code,offset+1);
					      Value* posValue =(Value*)getArray(&vm->constants,pos);
					      struct sObjString* variable = (ObjString*)posValue->obj;
					      printf("%6d   key postion:%d , variable name:%s\n",offset+1,pos,variable->str);//print key pos;
					      return 2;
				      }
		case OP_SET_GLOBAL:{
					   printSimpleInstruction(offset,"opSetGlobal");
					   return 2;
				   }
		case OP_GET_LOCAL:{
					  printSimpleInstruction(offset,"OP_GET_LOCAL");
					   int pos = *(uint8_t*)getArray(&vm->code,offset+1);
					   printf("%6d    stack slot %d\n",offset+1,pos);
					  return 2;
				  }
		case OP_SET_LOCAL:{
					  printSimpleInstruction(offset,"OP_SET_LOCAL");
					   int pos = *(uint8_t*)getArray(&vm->code,offset+1);
					   printf("%6d    stack slot %d\n",offset+1,pos);
					  return 2;
				  }
		case OP_POP:{
				    printSimpleInstruction(offset,"OP_POP");
				    return 1;
			    }
		default:{
				fprintf(stderr,"undefined opcode %d\n",opcode);
				exit(64);
			}
	}
}

void disassembleChunk(VM* vm){
	printf("offset   OPCODE\n");
	int ip = 0;
	for (;ip<(vm->code.count);){
		int offset = disassembleOpCode(vm,ip,*(uint8_t*)getArray(&vm->code,ip));
		ip += offset;
	}
}
