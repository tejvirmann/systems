/*
    Creates a queue and returns its pointer. Given size. 
*/
Queue *CreateStringQueue(int size)

/*
    Adds a string to the end of the queue. 
*/
void EnqueueString(Queue *q, char *string)

/*
    removes a string to the end of the queue. 
*/
char * DequeueString(Queue *q)

/*
    Prints the Information
*/
void PrintQueueStats(Queue *q)