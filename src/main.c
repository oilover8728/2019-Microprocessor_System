#include "stm32l476xx.h"
extern void GPIO_init();
int Map[4][4] = {{1,2,3,10}, {4,5,6,11}, {7,8,9,12}, {15,0,14,13}};
int record[16]= {0};
void keypad_init(){
	RCC->AHB2ENR = RCC->AHB2ENR|0x7;	//enable a,b,c
	GPIOB->MODER=GPIOB->MODER&0xFFFFFF00;	//pb 0 1 2 3 as output
	GPIOB->MODER=GPIOB->MODER|0x55;
	GPIOB->PUPDR=GPIOB->PUPDR&0xFFFFFF00;
	GPIOB->PUPDR=GPIOB->PUPDR|0x55;
	GPIOB->OSPEEDR=GPIOB->OSPEEDR|0x55;
	GPIOB->ODR=GPIOB->ODR|0xF;
	GPIOC->MODER=GPIOC->MODER&0xFFFFFF00;	//pc 0 1 2 3as input
	GPIOC->PUPDR=GPIOC->PUPDR&0xFFFFFF00;
	GPIOC->PUPDR=GPIOC->PUPDR|0xAA;
}
void wheel_init(){
	RCC->AHB2ENR = RCC->AHB2ENR|0x7;	//enable a,b,c
	GPIOA->MODER=GPIOA->MODER&0xFFFFFF00;	//pb 0 1 2 3 as output
	GPIOA->MODER=GPIOA->MODER|0x55;
	GPIOA->PUPDR=GPIOA->PUPDR&0xFFFFFF00;
	GPIOA->PUPDR=GPIOA->PUPDR|0x55;
	GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x55;
	GPIOA->ODR=GPIOA->ODR|0xF;
}
void keypad_init_change_bc(){
	GPIOB->MODER=GPIOB->MODER&0xFFFFFF00;	//pb 0 1 2 3 as input
	GPIOB->PUPDR=GPIOB->PUPDR&0xFFFFFF00;
	GPIOB->PUPDR=GPIOB->PUPDR|0xAA;
	GPIOC->MODER=GPIOC->MODER&0xFFFFFF00;	//pc 0 1 2 3 as output
	GPIOC->MODER=GPIOC->MODER|0x55;
	GPIOC->PUPDR=GPIOC->PUPDR&0xFFFFFF00;
	GPIOC->PUPDR=GPIOC->PUPDR|0x55;
	GPIOC->OSPEEDR=GPIOC->OSPEEDR|0x55;
	GPIOC->ODR=GPIOC->ODR|0xF;
}
char keypad_scan(){
    int row = 0,col = 0, num = 0, tmp = 0, aaa=0;
    char val = 0;
    int counter;
    keypad_init();
    for(int i = 0; i < 16; i++){
    		record[i] = 0;
    }
	GPIOB->ODR=GPIOB->ODR|0xF;
	int flag = GPIOC->IDR&0xF;
	if(flag){
		counter = 1000;
   		while(counter!=0){
   			flag = GPIOC->IDR&0xF;
   			counter--;
   		}
   		if(flag){
	        for(row = 0; row < 4; row++){				//first scan
	            for(col = 0; col < 4 ; col++){
	            	GPIOB->ODR=GPIOB->ODR&0x0;
	            	GPIOB->ODR=GPIOB->ODR|(1<<col);
	            	flag = GPIOC->IDR&0xF;
	                int flag_shift_row = flag >> row;		// if flag == b1000 (the col 3 (pc3) has been pressed), so right-shift three times.
	                int pressed = flag_shift_row&1;		// to check if it's been pressed
	                if(pressed){
	                    tmp = Map[row][col];
	                    record[tmp] = 1;
	                }
	            }
	        }
	    }
	    keypad_init_change_bc();						//choose another way to scan
	    for(col = 0; col < 4; col++){				//second scan
	        for(row = 0; row < 4 ; row++){
	           	GPIOC->ODR=GPIOC->ODR&0x0;
	            GPIOC->ODR=GPIOC->ODR|(1<<row);
	            flag = GPIOB->IDR&0xF;
	            int flag_shift_col = flag >> col;
	            int pressed = flag_shift_col&1;
	            if(pressed){
	                tmp = Map[row][col];
	                record[tmp] = 1;
	            }
	        }
	    }
	    int pa=0x0;//pa0-1=left, pa5-7=right
	    /*if(record[2]){
	    	pa|=0x21;
	    }else if(record[8]){
	    	pa|=0xC2;
	    }*/
	    if(record[4] && record[2]){
	    	pa|=0x20;
	    }else if(record[6] && record[2]){
	    	pa|=0x1;
	    }else if(record[2]){
	    	pa|=0x21;
	    }else if(record[4] && record[8]){
	    	pa|=0xC0;
	    }else if(record[6] && record[8]){
	    	pa|=0x42;
	    }else if(record[8]){
	    	pa|=0xC2;
	    }
	    GPIOA->ODR=pa;
    }
    else{
    	for(int i = 0; i < 16; i++){
    		record[i] = 0;
    	}
    	return -1;
    }
	return val;
}
int main(){

	GPIO_init();
	keypad_init();
	wheel_init();
	while(1){
		keypad_scan();
	}
}
