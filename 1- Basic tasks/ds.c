#include "ds.h"

//This function shift a given array "arr" of length "length" to the left by "shiftby" elements
//if "zeroTheRest = 1" the last n element of the array are set to 0 whene n = length - shiftby

void lshift(uint8_t* arr , uint32_t length , uint32_t shiftby , uint8_t zeroTheRest)
{
	memmove(&arr[0] , &arr[shiftby] , (length-shiftby)*sizeof(int));
	if (zeroTheRest == 1)
	{
		int i ;
		for(i = length - shiftby ; i <length ; i++)
		{
			arr[i] = 0 ;
		}
	}
}