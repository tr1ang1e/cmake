#include "logging.h"
#include "counters.h"


/* function with variable arguments number
 * as much args as needed, but:
 *   > ellipsis (...) is always on the last place
 *   > arg before ellipsis is always 'int' type = must represent variable args number
 * memory for arguments, corresponding to ellipsis, would be dynamically allocated
 * 
 * */
int print_log_counter = 0;
void print_log(int _args_num_, ...)
{
	_TR_;
	increment_counter(&print_log_counter);
	
	// 1. declare va_list variable
	va_list args;

	// 2. initialize va_list variable using auto counted 'int' arg
	va_start(args, _args_num_);

	// 3. get arguments using their types (each arg might be accessed only once)
	printf(" ::  variadic function \n");
	for (int idx = 0; idx < _args_num_; ++idx)
	{
		printf(" arg_%d : %s \n", idx, va_arg(args, const char*));
	}

	// 4. free memory allocated for va_list
	va_end(args);
}