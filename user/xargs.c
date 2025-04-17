#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    char *nargs[MAXARG];
    int argCount = 0;
    
    // Lưu command và các đối số ban đầu vào nargs
    for(int i = 1; i < argc; i++) {
        nargs[argCount++] = argv[i];
    }

    char tmpchar = 0;
    char buffer[1024];

    int status = 1;
    while(status) {
        int pos = 0;
        int argStartPos = 0;
        
        // Đọc và xử lý từng dòng
        while(1) {
            status = read(0, &tmpchar, 1);
            if(status == 0) exit(0);

            if(tmpchar != ' ' && tmpchar != '\n') {
                buffer[pos++] = tmpchar;

            } 
            
            else if(tmpchar == ' ') {
                buffer[pos] = 0;
                nargs[argCount++] = &buffer[argStartPos];
                argStartPos = pos + 1;
                pos++;
            } 
            
            else if(tmpchar == '\n') {
                buffer[pos] = 0;
                nargs[argCount] = &buffer[argStartPos];  // Thêm đối số cuối cùng của dòng
                nargs[argCount + 1] = 0;  // NULL-terminate mảng đối số
                
                // Thực thi lệnh
                if(fork() == 0) {
                    exec(nargs[0], nargs);
                    fprintf(2, "exec %s failed\n", nargs[0]);
                    exit(1);
                } 
                
                else {
                    wait(0);
                    // Reset argCount về các đối số ban đầu cho lần lặp tiếp theo
                    argCount = argc - 1;
                }
                break;
            }
        }
    }
    
    exit(0);
}