import sys
from datetime import datetime

date_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
with open(sys.argv[1], "w") as last_compile:
    last_compile.write('#ifndef LAST_COMPILE_H \n')
    last_compile.write('#define LAST_COMPILE_H \n')
    last_compile.write('\n')
    last_compile.write(f'const char* _LC_ = "Last compile time {date_time}";  // last compile time \n')
    last_compile.write('\n')
    last_compile.write('#endif // LAST_COMPILE_H \n')