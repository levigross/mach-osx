#include <mach/mach.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

kern_return_t lsPorts(task_t TargetTask);

kern_return_t lsPorts(task_t TargetTask)
{
    kern_return_t kr;
    mach_port_name_array_t portNames = NULL;
    mach_msg_type_number_t portNamesCount;
    mach_port_type_array_t portRightTypes = NULL;
    mach_msg_type_number_t portRightTypesCount;

    kr = mach_port_names(TargetTask, &portNames, &portNamesCount, &portRightTypes, &portRightTypesCount);
    if (kr != KERN_SUCCESS)
    {
        fprintf(stderr, "Unable to obtain MACH port names. %d\n", kr);
        return kr;
    }
    for(unsigned int p = 0; p < portNamesCount;p++)
    {
        printf("0x%x 0x%x\n", portNames[p], portRightTypes[p]);
    }
    
    return kr;
}

int main(int argc, const char * argv[])
{
    task_t targetTask;
    kern_return_t kr;
    uid_t my_uid;
    
    my_uid = geteuid();
    if (my_uid != 0)
    {
        fprintf(stderr, "You must run this program as root\n");
        exit(-1);
    }
    
    if (argv[0] == NULL) {
        fprintf(stderr, "You must provide a target PID\n");
        exit(-1);
    }
    
    int pid = atoi(argv[1]);
    kr = task_for_pid(current_task(), pid, &targetTask);
    lsPorts(targetTask);
    kr = mach_port_deallocate(current_task(), targetTask);
    return kr;
    
}
