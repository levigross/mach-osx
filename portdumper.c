#include <mach/mach.h>
#include <mach/port.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

kern_return_t	lsPorts(task_t TargetTask);

kern_return_t 
lsPorts(task_t TargetTask)
{
	kern_return_t	kr;
	mach_port_name_array_t portNames = NULL;
	mach_msg_type_number_t portNamesCount;
	mach_port_type_array_t portRightTypes = NULL;
	mach_msg_type_number_t portRightTypesCount;
    mach_port_type_t mac_port_type;

	kr = mach_port_names(TargetTask, &portNames, &portNamesCount, &portRightTypes, &portRightTypesCount);
	if (kr != KERN_SUCCESS) {
		fprintf(stderr, "Unable to obtain MACH port names. %d\n", kr);
		return kr;
	}

	for (unsigned int p = 0; p < portNamesCount; p++) {
        printf("Hex Code: ");
		printf("0x%x 0x%x\n", portNames[p], portRightTypes[p]);
        switch(portRightTypes[p])
        {
            case MACH_PORT_TYPE_NONE:
                printf("This port has send right\n");
                break;
            case MACH_PORT_TYPE_RECEIVE:
               printf("This port has receive right\n");
               break;
            case MACH_PORT_TYPE_SEND_ONCE:
                printf("This port has a send-once right\n");
                break;
            case MACH_PORT_TYPE_PORT_SET:
                printf("This port has a port set\n");
                break;
            case MACH_PORT_TYPE_DEAD_NAME:
                printf("The name is a dead name\n");
                break;
            case MACH_PORT_TYPE_DNREQUEST:
                printf("A dead-name request has been registered for the right.\n");
                break;
            case MACH_PORT_TYPE_PORT_RIGHTS:
            case MACH_PORT_TYPE_SEND_RECEIVE:
                printf("Port has both send and receive right\n");
                break;
            case MACH_PORT_TYPE_SEND_RIGHTS:
                printf("Port had send right\n");
                break;
            case MACH_PORT_TYPE_PORT_OR_DEAD:
                printf("Port has send receive rights or is dead\n");
                break;
            case MACH_PORT_TYPE_ALL_RIGHTS:
                printf("Port has all rights\n");
                break;
            default:
                printf("Unknown PORT right 0x%x\n", portRightTypes[p]);
        }
	}

	return kr;
}

int 
main(int argc, const char *argv[])
{
	task_t		targetTask;
	kern_return_t	kr;
	uid_t		my_uid;

	my_uid = geteuid();
	if (my_uid != 0) {
		fprintf(stderr, "You must run this program as root\n");
		exit(-1);
	}
	if (argv[0] == NULL) {
		fprintf(stderr, "You must provide a target PID\n");
		exit(-1);
	}
	int		pid = atoi(argv[1]);
	task_for_pid(current_task(), pid, &targetTask);
	kr = lsPorts(targetTask);
	mach_port_deallocate(current_task(), targetTask);
	return kr;

}
