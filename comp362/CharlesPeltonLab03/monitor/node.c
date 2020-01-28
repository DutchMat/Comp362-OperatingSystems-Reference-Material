#include "message.h"
int main(int argc, char *argv[]){
mqd_t my_msqid, your_msqid;
MESSAGE rece, sen;
TEMPERATURE nodeTemp;
nodeTemp.temperature = strtol(argv[2], NULL, 0);
printf("CLI: %s detecting temp at: %f\n", argv[1], nodeTemp.temperature);
if ((my_msqid = mq_open(argv[1], O_RDWR)) < 0)
		oops("CLI: Error opening a queue.", errno);
		
		printf("CLI: %s coupled with node queue successfully.\n", argv[1]);
if ((your_msqid = mq_open(MONITOR_QUEUE, O_RDWR)) < 0)
		oops("CLI: Error opening a queue.", errno);

		printf("CLI: %s coupled with monitor queue successfully.\n", argv[1]);

while (true){
if (mq_receive(my_msqid, (char*) &rece, MAX_MSG_SIZE, NULL) >= 0){ printf("%s%s: MONITOR REPORTS: BASE %.2f DEGREES\n",NODE_NAME_PREFIX, argv[1], rece.temperature.temperature);

nodeTemp.temperature
      = (nodeTemp.temperature * 3 + 2 * rece.temperature.temperature) / 5;

if(rece.stable){
printf("%s TERMINATING... STABLE!\n",argv[1]);
exit(0);
}

sen.nodeId = rece.nodeId;
sen.stable = rece.stable;
sen.temperature.temperature = nodeTemp.temperature;
sen.temperature.msqid = your_msqid;
if (mq_send(your_msqid, (const char*) &sen, sizeof(sen), 0) < 0)
				oops("SRV: Cannot respond to a client.", errno);

	}
}

return 0;
}
