struct sum_args {
    int arg1;
    int arg2;
};

program PROG_ARITH {
    version UNO {
        int PROC_SUM(sum_args numbers) = 1;
    } = 1; 
} = 0x20000000;